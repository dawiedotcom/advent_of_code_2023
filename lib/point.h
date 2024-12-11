#pragma once

#include <vector>
#include <ostream>

struct point_t {
  int x, y;
  const int X;

  //point(int x, int y, int X) 
  //  : x(x), y(y), X(X)
  //{}

  //point_t(int x, int y) 
  //  : point_t(x, y, 1023)
  //{ }

  point_t& operator=(const point_t rhs) {
    //if (rhs == *this) return *this;
    x = rhs.x;
    y = rhs.y;
    return *this;
  }
};


point_t operator+(const point_t& lhs, const point_t& rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.X};
}

constexpr bool operator==(const point_t& lhs, const point_t& rhs) noexcept {
  return (lhs.x==rhs.x) && (lhs.y==rhs.y);
}

template<>
struct std::hash<point_t> {
  size_t operator()(const point_t& p) const noexcept {
    return (p.x) * (p.y + p.X);
  }
};

std::ostream& operator<<(std::ostream& out, const point_t& point) {
  return out << "(" << point.x << ", " << point.y << ")";
}
