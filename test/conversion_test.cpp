#include "geometry.h"

#include <sbit/usop.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using usop_test::Circle;
using usop_test::Figure;
using usop_test::Point;
using usop_test::Square;

namespace
{

using FigurePool = sbit::UnsynchronizedObjectPool<Figure>;
using CirclePool = sbit::UnsynchronizedObjectPool<Circle>;
using SquarePool = sbit::UnsynchronizedObjectPool<Square>;

} // anonymous namespace

using ::testing::HasSubstr;

TEST(ConversionTest, Conversions)
{
   ASSERT_EQ(0, Point::references);
   {
      CirclePool circlePool{/* size = */ 32};
      auto       circleOne = circlePool.create(Point{3, 4}, /* radius = */ 5);

      std::vector<sbit::PooledPointer<Figure>> figures;
      figures.push_back(circleOne);

      sbit::PooledPointer<Circle> circleBack = sbit::static_pointer_cast<Circle>(figures.back());
      std::string                 render     = circleBack->draw();

      ASSERT_THAT(render, HasSubstr("Circle"));
   }

   ASSERT_EQ(0, Point::references);
}

TEST(ConversionTest, CastDoesNotLeak)
{
   ASSERT_EQ(0, Point::references);

   {
      CirclePool pool{/* size = */ 32};

      {
         auto circleOne = pool.create(Point{3, 4}, /* radius = */ 5);

         ASSERT_EQ(1, Point::references);

         auto figureOne = sbit::static_pointer_cast<Figure>(circleOne);

         ASSERT_EQ(1, Point::references);

         //ASSERT_TRUE(figureOne == circleOne);
      }
   }

   ASSERT_EQ(0, Point::references);
}
