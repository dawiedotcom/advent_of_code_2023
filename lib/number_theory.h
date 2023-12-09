#pragma once

#include <numeric>
#include <vector>

template<typename T>
T gcd(T a, T b) {
  T tmp = a, rem;
  if (a < b) {
    tmp = a;
    a = b;
    b = tmp;
  }
  while (b > 0) {
    tmp = a % b;
    a = b;
    b = tmp;
  }
  return a;
}

template<class T>
struct gcd_ {
  inline T operator()(const T& a, const T& b) const {
    return gcd<T>(a, b);
  }
};

template<class T >
T gcd(const std::vector<T>& v) {
  return std::accumulate(v.begin(), v.end(), *v.begin(), gcd_<T>());
}

//template<class InputIt, class T >
//T gcd(InputIt first, InputIt last) {
//  return std::accumulate(first, last, *first, gcd_<T>());
//}


template<typename T>
T lcm(T a, T b) {
  T prod = a*b;
  return (prod > 0) ? prod / gcd<T>(a, b) : -prod/gcd<T>(a, b);
}

template<typename T>
auto lcm_ = [](const T& a, const T& b) {return lcm(a, b);};

template<class T >
T lcm(const std::vector<T>& v) {
  return std::accumulate(v.begin(), v.end(), *v.begin(), lcm_<T>);
}
