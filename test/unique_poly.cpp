#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace
{

struct Point
{
   int x = 0;
   int y = 0;

   Point(int a, int b) : x{a}, y{b}
   {
      references++;
   }

   ~Point()
   {
      references--;
   }

   static int references;
};

int Point::references = 0;

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

int Figure::references = 0;

class Circle : public Figure
{
public:
   Circle(Point center, int radius) : Figure{center}, m_radius{radius}
   {
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
      os << "Square at (" << m_center.x << ", " << m_center.y << ") with a corner at (" << m_corner.x << ", " << m_corner.y << ")";
      return os.str();
   }

private:
   Point m_corner;
};

} // anonymous namespace

int main()
{
   std::vector<std::unique_ptr<Figure>> figures;

   figures.emplace_back(std::make_unique<Circle>(Point{3, 4}, 5));
   figures.emplace_back(std::make_unique<Square>(Point{3, 4}, Point{5, 6}));

   for (const auto& fig: figures)
   {
      std::cout << fig->draw() << std::endl;
   }

   return 0;
}
