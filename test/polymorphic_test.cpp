#include "geometry.h"

#include <sbit/usop.h>

#include <gtest/gtest.h>

using usop_test::Circle;
using usop_test::Figure;
using usop_test::Point;
using usop_test::Square;

namespace
{

using FigurePool = sbit::UnsynchronizedObjectPool<Figure, 32>;
using CirclePool = sbit::UnsynchronizedObjectPool<Circle, 32>;
using SquarePool = sbit::UnsynchronizedObjectPool<Square, 32>;

class PolymorphicTest : public ::testing::Test
{
protected:
   void SetUp() override
   {
      ASSERT_EQ(0, Point::references);
   }

   void TearDown() override
   {
      ASSERT_EQ(0, Point::references);
   }

   CirclePool circlePool;
   SquarePool squarePool;
};

std::vector<sbit::PooledPointer<Figure>> makeFigures(CirclePool& circlePool, SquarePool& squarePool)
{
   auto circleOne = circlePool.create(Point{3, 4}, /* radius = */ 5);
   auto squareTwo = squarePool.create(Point{4, 5}, Point{6, 7});

   std::vector<sbit::PooledPointer<Figure>> figures;
   figures.push_back(circleOne);
   figures.push_back(squareTwo);

   return figures;
}

std::vector<sbit::PooledPointer<Figure>> makeCircles(CirclePool& circlePool)
{
   std::vector<sbit::PooledPointer<Figure>> figures;

   for (int ii = 0; ii < 4; ++ii)
   {
      auto circle = circlePool.create(Point{10 * ii + 3, 10 * ii + 4}, /* radius = */ 10 * ii + 5);

      figures.push_back(circle);
   }

   return figures;
}

} // anonymous namespace

TEST_F(PolymorphicTest, AllocateOne)
{
   std::vector<sbit::PooledPointer<Figure>> figures = makeFigures(circlePool, squarePool);
   for (auto& ptr : figures)
   {
      std::cout << ptr->draw() << std::endl;
   }
}

TEST_F(PolymorphicTest, IterateWithCast)
{
   std::vector<sbit::PooledPointer<Figure>> figures = makeCircles(circlePool);

   int sumOfRadii = 0;

   for (auto ptr : figures)
   {
      sumOfRadii += sbit::static_pointer_cast<Circle>(ptr)->getRadius();
   }

   ASSERT_EQ(80, sumOfRadii);
}
