# Day 6: Wait For It

* https://adventofcode.com/2023/day/6

## The long way

Looping over times turned out fast enough for part 2. 

```c++
using ull = unsigned long long;
ull process(long long best_time, long long best_distance) {

  ull num_ways_to_win = 0;
  for (ull t=1; t<=best_time; t++) {
    ull speed = t;
    ull distance = speed * ( best_time - t );
    if (distance > best_distance) {
      num_ways_to_win ++;
    }
  }
  return num_ways_to_win;
}

```

## The smart way

However, there is a solution in constant time. The distance the boat travels is
$d=v (t_0-t)$ and the solution is to count all integer distances where $d > d_0$.
It is also given that $v = t$. After a bit of rearranging and substitution, the 
solution is the number of integers between the roots of the parabola 
$d - d_0 = t(t_0-t) - d_0 = -t^2 + t_0 t - d_0 = 0$ .

Using the quadratic formula, gives the following code:
```c++
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
```

## Notes

 * C++ does not convert `unsigned long long` to `double` in a sensible way.
 * The edge case when the polynomial has integer roots needs to be handled. Shifting the roots by `1e-14`
 worked for my input and test cases.
 * The 'long way' was easier to implement and runs really fast even with long input.


