#pragma once

#include <iostream>

/*
 * For 2023 day 5 part 2
 * Represents an interval [a, b)
 */
class interval {
public:
  long long start, end;
  /*
   * Constructs an interval inclusive of a and exclusive of b [a, b).
   */
  interval(long long s, long long e)
    : start(s)
    , end(e)
  {

  }

  /*
   * Returns the number of elements in the interval.
   */
  long long length() const;
  /*
   * True if this and other have common elements
   */
  bool intersects(const interval& other) const;

  /*
   * Returns the interval that contain all numbers in this and other.
   * Causes a runtime error if there is no intersection.
   */
  interval intersection(const interval& other) const;

  /*
   * Returns true if there exists elements in this that are smaller than all elements in other.
   */
  bool extends_past_left(const interval& other) const;
  /*
   * Returns the sub-interval containing the elements in this that are smaller than all
   * elements in other;
   */
  interval left_of(const interval& other) const;

  /*
   * Returns the sub-interval containing the elements in this that are larger than all
   * elements in other;
   */
  bool extends_past_right(const interval& other) const;

  /*
   * Returns true if there exists elements in this that are larger than all elements in other.
   */
  interval right_of(const interval& other) const;

  bool operator==(const interval& rhs) const;
  friend std::ostream& operator<<(std::ostream& out, const interval i);
};

std::ostream& operator<<(std::ostream& out, const interval i);
