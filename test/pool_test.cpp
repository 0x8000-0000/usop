#include "geometry.h"

#include <sbit/usop.h>

#include <gtest/gtest.h>

#include <random>

using usop_test::Point;

TEST(TestPool, AllocateOne)
{
   ASSERT_EQ(0, Point::references);
   
   {
      sbit::UnsynchronizedObjectPool<Point, 32> pool;

      auto ptr = pool.create(3, 4);
      ASSERT_EQ(3, ptr->x);
      ASSERT_EQ(4, ptr->y);
      ASSERT_EQ(1, Point::references);
   }

   ASSERT_EQ(0, Point::references);
}

TEST(TestPool, AllocateOneWithStats)
{
   ASSERT_EQ(0, Point::references);

   {
      sbit::UnsynchronizedObjectPool<Point, 32> pool;

      auto ptr = pool.create(3, 4);
      ASSERT_EQ(3, ptr->x);
      ASSERT_EQ(4, ptr->y);
      ASSERT_EQ(1, Point::references);
   }

   ASSERT_EQ(0, Point::references);
}

TEST(TestPool, AllocateFourSegments)
{
   ASSERT_EQ(0, Point::references);

   {
      using Pool = sbit::UnsynchronizedObjectPool<Point, 32>;
      Pool pool;

      std::vector<Pool::Pointer> allocations;

      for (size_t ii = 0; ii < 128; ++ii)
      {
         allocations.push_back(pool.create(ii, 2 * ii + 1));
      }

      ASSERT_EQ(128, Point::references);

      for (size_t ii = 0; ii < 32; ++ii)
      {
         allocations[ii] = Pool::Pointer();
      }

      for (size_t ii = 0; ii < 32; ++ii)
      {
         allocations[ii] = pool.create(3 * ii, 4 * ii + 1);
      }

      //ASSERT_EQ(4, stats.allocatedSegments);

      ASSERT_EQ(128, Point::references);
   }

   ASSERT_EQ(0, Point::references);
}

TEST(TestPool, AllocateTwoSegments)
{
   ASSERT_EQ(0, Point::references);

   {
      using Pool = sbit::UnsynchronizedObjectPool<Point, 32>;
      Pool pool;

      std::vector<Pool::Pointer> allocations;

      for (size_t ii = 0; ii < 32; ++ii)
      {
         allocations.push_back(pool.create(ii, 2 * ii + 1));
      }

      for (size_t ii = 0; ii < 16; ++ii)
      {
         allocations[ii] = Pool::Pointer();
      }

      for (size_t ii = 0; ii < 16; ++ii)
      {
         allocations[ii] = pool.create(2 * ii, 3 * ii + 1);
      }
   }

   ASSERT_EQ(0, Point::references);
}

TEST(TestPool, MaintainLiveness)
{
   ASSERT_EQ(0, Point::references);

   {
      using Pool = sbit::UnsynchronizedObjectPool<Point, 32>;
      Pool pool;

      {
         Pool::Pointer ptr;
         {
            ptr = pool.create(3, 14);
            ASSERT_EQ(1, Point::references);
         }
         ASSERT_EQ(1, Point::references);
      }

      ASSERT_EQ(0, Point::references);
   }
}

TEST(TestPool, PseudorandomTest)
{
   ASSERT_EQ(0, Point::references);

   {
      using Pool = sbit::UnsynchronizedObjectPool<Point, 64>;
      Pool pool;

      std::uniform_int_distribution<size_t> distribution(1, 4096);
      std::random_device rd;
      std::mt19937 engine(rd());

      std::vector<Pool::Pointer> objects;
      objects.reserve(64 * 1024);

      bool allocating = true;

      size_t ptrx = 1;
      size_t ptry = 1;

      size_t allocations = 0;
      size_t deallocation = 0;
      size_t emptyEvent = 0;

      for (size_t loops = 0; loops < 1024; ++loops)
      {
         const auto count = distribution(engine);

         for (size_t ii = 0; ii < count; ++ii)
         {
            if (allocating)
            {
               objects.push_back(pool.create(ptrx, ptry));
               ptrx = (ptrx + 3) % 65536;
               ptry = (ptry + 7) % 65536;

               allocations ++;
            }
            else
            {
               if (objects.empty())
               {
                  emptyEvent ++;
                  continue;
               }

               deallocation ++;
               objects.pop_back();
            }
         }

         allocating = ! allocating;
      }

      std::cout << "Alocations: " << allocations << "  deallocations: " << deallocation << " empty: " << emptyEvent << std::endl;

      while (! objects.empty())
      {
         objects.pop_back();
      }

      pool.compactFast();
   }

   ASSERT_EQ(0, Point::references);
}
