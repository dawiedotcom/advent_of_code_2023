#include "interval.h"

#include <iostream>
#include <cassert>

using namespace std;

int main() {
  interval a(10, 15);
  assert(a == a);
  assert(a.length() == 5);
  assert(a.intersects(a));
  cout << a << endl;
  cout << a.intersection(a) << endl;
  assert(a.intersection(a) == a);
  assert(!a.extends_past_left(a));
  assert(!a.extends_past_right(a));

  interval b(8, 13); // overlaps at the low end of a
  assert(a.intersects(b));
  assert(b.intersects(a));
  assert(b.intersection(a) == interval(10, 13));
  assert(a.intersection(b) == interval(10, 13));

  assert(!a.extends_past_left(b));
  assert(a.extends_past_right(b));
  assert(b.extends_past_left(a));
  assert(!b.extends_past_right(a));

  assert(b.left_of(a) == interval(8, 10));
  assert(a.right_of(b) == interval(13, 15));

  interval c(3, 5); // no overlap with a, lew end of a
  assert(!a.intersects(c));
  assert(!c.intersects(a));

  assert(!a.extends_past_left(c));
  assert(a.extends_past_right(c));
  assert(c.extends_past_left(a));
  assert(!c.extends_past_right(a));

  interval d(16, 3); // no overlap with a, top end of a
  assert(!a.intersects(d));
  assert(!d.intersects(a));

  interval e(9, 13); // overlaps a at top of a
  assert(a.intersects(e));
  assert(e.intersects(a));
  assert(a.intersection(e) == interval(10, 13));

  interval f(9, 15); // orerlaps a, ending at a's largest element
  assert(a.intersects(f));
  assert(f.intersects(a));
  cout << a << endl;
  cout << f << endl;
  assert(a.intersection(f) == interval(10, 15));

  interval g(10, 20); // overaps a, starting at a's largest element
  assert(a.intersects(g));
  assert(g.intersects(a));
  assert(a.intersection(g) == interval(10, 15));
  //interval c = a.intersection(b);

  std::cout << "a: " << a << endl;
  std::cout << "b: " << b << endl;


  return 0;
}
