#pragma once

#include <iostream>
#include <utility>
#include <cassert>

/*
 * Represents an interval [a, b).
 * The name with an underscore is chosen so that if you want intervals of uint32_t,
 * using interval = interval_<uint32_t>;
 * makes the rest of the code a bit cleaner.
 *
 * For 2023, Day 5 Part 2
 * See:
 *   - 2023, Day 19 Part 2
 */
template<typename T>
class interval_ {
public:
  T start, end;
  interval_()
    : start(0)
    , end(0)
  {
    //assert(false);
  }
  /*
   * Constructs an interval inclusive of a and exclusive of b [a, b).
   */
  interval_(T s, T e)
    : start(s)
    , end(e)
  {

  }

  /*
   * Returns the number of elements in the interval.
   */
  constexpr T length() const { return end - start; }
  /*
   * True if this and other have common elements
   */
  bool intersects(const interval_<T>& other) const;

  /*
   * Returns the interval that contain all numbers in this and other.
   * Causes a runtime error if there is no intersection.
   */
  interval_<T> intersection(const interval_<T>& other) const;

  /*
   * Returns true if there exists elements in this that are smaller than all elements in other.
   */
  bool extends_past_left(const interval_<T>& other) const;
  /*
   * Returns the sub-interval containing the elements in this that are smaller than all
   * elements in other;
   */
  interval_<T> left_of(const interval_<T>& other) const;

  /*
   * Returns the sub-interval containing the elements in this that are larger than all
   * elements in other;
   */
  bool extends_past_right(const interval_<T>& other) const;

  /* Returns [a, pivot) [pivot, end) */
  std::pair<interval_<T>, interval_<T>> split(T pivot) {
    assert( (start < pivot && pivot < end) );
    return std::make_pair(interval_<T>(start, pivot), interval_<T>(pivot, end));
  }

  /* Returns true if val is an element of this interval */
  bool is_element(T val) {
    return (val >= start) && (val < end);
  }

  /*
   * Returns true if there exists elements in this that are larger than all elements in other.
   */
  interval_<T> right_of(const interval_<T>& other) const;

  bool operator==(const interval_<T>& rhs) const;

  template<typename TT>
  friend std::ostream& operator<<(std::ostream& out, const interval_<TT> i);
};

//std::ostream& operator<<(std::ostream& out, const interval_<T> i);

template<typename T>
bool interval_<T>::intersects(const interval_<T>& other) const {
  return !(
    (this->start >= other.end)
    || (this->end <= other.start)
  );
}

template<typename T>
bool interval_<T>::operator==(const interval_<T>& rhs) const {
  return (this->start == rhs.start) && (this->end == rhs.end);
}


template <typename T>
interval_<T> interval_<T>::intersection(const interval_<T>& other) const {
  assert(this->intersects(other));
  return interval_<T>(
    std::max(this->start, other.start),
    std::min(this->end, other.end)
  );
}

template <typename T>
bool interval_<T>::extends_past_left(const interval_<T>& other) const {
  return (this->start < other.start);
}

template <typename T>
interval_<T> interval_<T>::left_of(const interval_<T>& other) const {
  assert(this->extends_past_left(other));
  return interval_(
    this->start,
    std::min(this->end, other.start)
  );
}

template <typename T>
bool interval_<T>::extends_past_right(const interval_<T>& other) const {
  return (this->end > other.end);
}

template <typename T>
interval_<T> interval_<T>::right_of(const interval_<T>& other) const {
  assert(this->extends_past_right(other));
  return interval_(
    std::max(this->start, other.end),
    this->end
  );
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const interval_<T> i)
{
  out << "[" << i.start << " " << i.end << ")";
  return out;
}
