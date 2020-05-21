#include "geometry.h"

#include <sbit/usop.h>

#include <gtest/gtest.h>

using usop_test::Point;

TEST(TestPoolSegment, AllocateOne)
{
   sbit::UnsynchronizedObjectPool<Point, 32>::Segment segment;

   auto* ptr = segment.create(3, 4);
   ASSERT_EQ(3, ptr->t.x);
   ASSERT_EQ(4, ptr->t.y);
   segment.release(ptr);
}

TEST(TestPoolSegment, AllocateTwoReleaseNested)
{
   sbit::UnsynchronizedObjectPool<Point, 32>::Segment segment;

   auto* ptr = segment.create(3, 4);
   ASSERT_EQ(3, ptr->t.x);
   ASSERT_EQ(4, ptr->t.y);

   auto* ptr1 = segment.create(5, 6);
   ASSERT_EQ(5, ptr1->t.x);
   ASSERT_EQ(6, ptr1->t.y);

   segment.release(ptr1);
   segment.release(ptr);
}

TEST(TestPoolSegment, AllocateTwoReleaseInOrder)
{
   sbit::UnsynchronizedObjectPool<Point, 32>::Segment segment;

   auto* ptr = segment.create(3, 4);
   ASSERT_EQ(3, ptr->t.x);
   ASSERT_EQ(4, ptr->t.y);

   auto* ptr1 = segment.create(5, 6);
   ASSERT_EQ(5, ptr1->t.x);
   ASSERT_EQ(6, ptr1->t.y);

   segment.release(ptr);
   segment.release(ptr1);
}
