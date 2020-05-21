#ifndef USOP_GEOMETRY_H
#define USOP_GEOMETRY_H

#include <cassert>
#include <sstream>
#include <string>

namespace usop_test
{

struct Point
{
   int x = 0;
   int y = 0;

   Point(int a, int b) : x{a}, y{b}
   {
      references++;
   }

   Point(const Point& other) : x{other.x}, y{other.y}
   {
      references++;
   }

   Point(Point&& other) : x{other.x}, y{other.y}
   {
      references++;
   }

   ~Point()
   {
      assert(references > 0);
      references--;
   }

   static int references;
};

class Figure
{
public:
   Figure(Point center) : m_center{center}
   {
      references++;
   }

   virtual ~Figure()
   {
      references--;
   }

   virtual std::string draw() = 0;

protected:
   Point m_center;

   static int references;
};

class Circle : public Figure
{
public:
   Circle(Point center, int radius) : Figure{center}, m_radius{radius}
   {
   }

   int getRadius()
   {
      return m_radius;
   }

   std::string draw() override
   {
      std::ostringstream os;
      os << "Circle at (" << m_center.x << ", " << m_center.y << ") of radius " << m_radius;
      return os.str();
   }

private:
   int m_radius;
};

class Square : public Figure
{
public:
   Square(Point center, Point corner) : Figure{center}, m_corner{corner}
   {
   }

   std::string draw() override
   {
      std::ostringstream os;
      os << "Square at (" << m_center.x << ", " << m_center.y << ") with a corner at (" << m_corner.x << ", "
         << m_corner.y << ")";
      return os.str();
   }

private:
   Point m_corner;
};

} // namespace usop_test

#endif // USOP_GEOMETRY_H
