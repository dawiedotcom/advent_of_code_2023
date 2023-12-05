#include "interval.h"

#include <algorithm>
#include <cassert>

long long interval::length() const {
  return end - start;
}

bool interval::intersects(const interval& other) const {
  return !(
    (this->start >= other.end)
    || (this->end <= other.start)
    //(this->start >= other.start) && (this->start <= other.end())
    //|| (other.start >= this->start) && (other.start <= this->end())
    //|| (other.start >= this->start) && (other.end() <= this->end())
  );
}

bool interval::operator==(const interval& rhs) const {
  return (this->start == rhs.start) && (this->end == rhs.end);
}


interval interval::intersection(const interval& other) const {
  assert(this->intersects(other));
  return interval(
    std::max(this->start, other.start),
    std::min(this->end, other.end)
  );
}

bool interval::extends_past_left(const interval& other) const {
  return (this->start < other.start);
}
interval interval::left_of(const interval& other) const {
  assert(this->extends_past_left(other));
  return interval(
    this->start,
    std::min(this->end, other.start)
  );
}
bool interval::extends_past_right(const interval& other) const {
  return (this->end > other.end);
}

interval interval::right_of(const interval& other) const {
  assert(this->extends_past_right(other));
  return interval(
    std::max(this->start, other.end),
    this->end
  );
}
  ;

std::ostream& operator<<(std::ostream& out, const interval i)
{
  out << "[" << i.start << " " << i.end << ")";
  return out;
}
