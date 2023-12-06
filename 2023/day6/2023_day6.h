#pragma once 

#include <cmath>

using ull = unsigned long long;
using ll = unsigned long long;
ull process(ll t0, ll d0) {
  double a = -1.0, b = (double)t0, c = -(double)d0;
  double disc = b*b - 4*a*c;
  double r0 = (-b + sqrt(disc)) / (2*a);
  double r1 = (-b - sqrt(disc)) / (2*a);
  ull interval = floor(r1-1e-14) - floor(r0+1e-14);
  return (ull) (interval);
}
