#include <iostream>

#include "interval.h"
#include "test.h"


using interval = interval_<unsigned int>;

using namespace std;

//test_data_t test_data;
int main() {
  init_test("interval");

  interval a(10, 15);
  TEST(a == a);
  TEST(a.length() == 5);
  TEST(a.intersects(a));
  //cout << a << endl;
  //cout << a.intersection(a) << endl;
  TEST(a.intersection(a) == a);
  TEST(!a.extends_past_left(a));
  TEST(!a.extends_past_right(a));

  interval b(8, 13); // overlaps at the low end of a
  TEST(a.intersects(b));
  TEST(b.intersects(a));
  TEST(b.intersection(a) == interval(10, 13));
  TEST(a.intersection(b) == interval(10, 13));

  TEST(!a.extends_past_left(b));
  TEST(a.extends_past_right(b));
  TEST(b.extends_past_left(a));
  TEST(!b.extends_past_right(a));

  TEST(b.left_of(a) == interval(8, 10));
  TEST(a.right_of(b) == interval(13, 15));

  interval c(3, 5); // no overlap with a, lew end of a
  TEST(!a.intersects(c));
  TEST(!c.intersects(a));

  TEST(!a.extends_past_left(c));
  TEST(a.extends_past_right(c));
  TEST(c.extends_past_left(a));
  TEST(!c.extends_past_right(a));

  interval d(16, 3); // no overlap with a, top end of a
  TEST(!a.intersects(d));
  TEST(!d.intersects(a));

  interval e(9, 13); // overlaps a at top of a
  TEST(a.intersects(e));
  TEST(e.intersects(a));
  TEST(a.intersection(e) == interval(10, 13));

  interval f(9, 15); // orerlaps a, ending at a's largest element
  TEST(a.intersects(f));
  TEST(f.intersects(a));
  //cout << a << endl;
  //cout << f << endl;
  TEST(a.intersection(f) == interval(10, 15));

  interval g(10, 20); // overaps a, starting at a's largest element
  TEST(a.intersects(g));
  TEST(g.intersects(a));
  TEST(a.intersection(g) == interval(10, 15));
  //interval c = a.intersection(b);

  //std::cout << "a: " << a << endl;
  //std::cout << "b: " << b << endl;


  return report();
}
