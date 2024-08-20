#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <functional>
#include "polygon.hpp"
#include "commands.hpp"

namespace gorbunova
{
  std::ostream &operator<<(std::ostream &os, const Point &point)
  {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
  }

  std::istream &operator>>(std::istream &is, Point &point)
  {
    char ignore_char;
    is >> Delimiter{'('} >> point.x >> Delimiter{','} >> point.y >> Delimiter{')'};
    return is;
  }

  std::ostream &operator<<(std::ostream &os, const Polygon &polygon)
  {
    os << "(";
    for (size_t i = 0; i < polygon.points.size(); ++i)
    {
      if (i > 0)
        os << ", ";
      os << polygon.points[i];
    }
    os << ")";
    return os;
  }

  std::istream &operator>>(std::istream &is, Polygon &polygon)
  {
    char ignore_char;
    is >> Delimiter{'('};
    while (is >> Delimiter{'('})
    {
      Point p;
      is >> p;
      polygon.points.push_back(p);
      is >> Delimiter{')'};
      if (is.peek() == ',')
      {
        is >> ignore_char;
      }
    }
    is >> Delimiter{')'};
    return is;
  }

  bool Point::operator==(const Point &other) const
  {
    return x == other.x && y == other.y;
  }

  bool Polygon::operator==(const Polygon &other) const
  {
    return points == other.points;
  }

  double Polygon::calculateArea() const
  {
    size_t n = points.size();
    if (n < 3)
      return 0.0;
    std::vector< double > x_terms(n);
    std::vector< double > y_terms(n);
    std::transform(points.begin(), points.end(), x_terms.begin(),
      [](const Point &p) { return static_cast< double >(p.x); });
    std::transform(points.begin(), points.end(), y_terms.begin(),
      [](const Point &p) { return static_cast< double >(p.y); });
    x_terms.push_back(x_terms[0]);
    y_terms.push_back(y_terms[0]);
    double area = std::inner_product(x_terms.begin(), x_terms.end() - 1,
                                     y_terms.begin() + 1, 0.0) -
                  std::inner_product(y_terms.begin(), y_terms.end() - 1,
                                     x_terms.begin() + 1, 0.0);
    double final_area = std::abs(area) / 2.0;
    return final_area;
  }

  std::vector< Polygon > readPolygonsFromFile(const std::string &filename)
  {
    std::ifstream file(filename);
    std::vector< Polygon > polygons;
    if (!file.is_open())
    {
      std::cerr << "Error: Cannot open file " << filename << std::endl;
      return polygons;
    }
    std::string line;
    while (std::getline(file, line))
    {
      std::istringstream iss(line);
      size_t num_points;
      if (!(iss >> num_points))
      {
        continue;
      }
      Polygon polygon;
      polygon.points.reserve(num_points);
      std::generate_n(std::back_inserter(polygon.points), num_points, [&iss]()
      {
        Point p;
        char ignore_char;
        iss >> Delimiter{'('} >> p.x >> Delimiter{';'} >> p.y >> Delimiter{')'};
        return p; });
      polygons.push_back(polygon);
    }
    return polygons;
  }

  double calculateAreaSum(const std::vector< Polygon > &polygons, std::function< bool(const Polygon &) > filter)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [filter](double sum, const Polygon &p)
      {
        return filter(p) ? sum + p.calculateArea() : sum;
      });
  }

  size_t countPolygons(const std::vector< Polygon > &polygons, std::function< bool(const Polygon &) > filter)
  {
    return std::count_if(polygons.begin(), polygons.end(), filter);
  }

  Polygon parsePolygonFromCommand(std::istream &iss)
  {
    Polygon polygon;
    int num_points;
    iss >> num_points;
    polygon.points.reserve(num_points);
    std::generate_n(std::back_inserter(polygon.points), num_points, [&iss]()
    {
      Point p;
      char ignore_char;
      iss >> Delimiter{'('} >> p.x >> Delimiter{';'} >> p.y >> Delimiter{')'};
      return p; });
      return polygon;
    }

  void processCommand(const std::string &command, std::vector< Polygon > &polygons)
  {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    gorbunova::iofmtguard guard(std::cout);

    if (cmd == "AREA")
    {
      areaCommand(iss, std::cout, polygons);
    }
    else if (cmd == "COUNT")
    {
      countCommand(iss, std::cout, polygons);
    }
    else if (cmd == "MAX")
    {
      maxCommand(iss, std::cout, polygons);
    }
    else if (cmd == "MIN")
    {
      minCommand(iss, std::cout, polygons);
    }
    else if (cmd == "MAXSEQ")
    {
      maxseqCommand(iss, std::cout, polygons);
    }
    else if (cmd == "ECHO")
    {
      echoCommand(iss, std::cout, polygons);
    }
    else
    {
      std::cout << "<INVALID COMMAND>" << std::endl;
    }
  }
}
