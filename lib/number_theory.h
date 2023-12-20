#pragma once

#include <numeric>
#include <vector>
#include <algorithm>

/*
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
*/

template<class T>
T gcd(const std::vector<T>& v) {
  return std::accumulate(v.begin(), v.end(), *v.begin(),
                         [](T a, T b) { return std::gcd(a, b); });
}

template<class T>
T lcm(const std::vector<T>& v) {
  return std::accumulate(v.begin(), v.end(), *v.begin(),
                         [](T a, T b) {return std::lcm(a, b); });
}

/*
 * See https://en.wikipedia.org/wiki/Binomial_coefficient#In_programming_languages
 */
template <typename T>
T binomial_coefficient(T n, T k) {
  if (k<0 || k > n)
    return 0;
  if (k==0 || n==0)
    return 1;
  k = std::min(k, n-k);
  T c = 1;
  for (T i=0; i<k; i++) {
    c = c *(n-i)/(i+1);
  }
  return c;
}
