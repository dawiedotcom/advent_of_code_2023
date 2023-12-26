#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "lib.h"
#include "parser.h"

using namespace std;

struct point_t {
  long double x, y, z;
};

ostream& operator<<(ostream& out, pair<const point_t&, const point_t&> pos_vel) {//}const point_t& p, const point_t& v) {
  auto& [p, v] = pos_vel;
  out
    << p.x << ", " << p.y << ", " << p.z << " @ "
    << v.x << ", " << v.y << ", " << v.z;
  return out;
}

class storm_t {
public:
  vector<point_t> pos;
  vector<point_t> vel;

  void input(const string& line) {
    parser parse(line);
    long double x, y, z;
    x = parse.next_int();
    y = parse.next_int();
    z = parse.next_int();
    pos.emplace_back(x, y, z);
    x = parse.next_int();
    y = parse.next_int();
    z = parse.next_int();
    vel.emplace_back(x, y, z);
  }

  pair<long double, long double> get_xy_intersection(const point_t& p1, const point_t& v1, const point_t& p2, const point_t& v2) const {
    /*
     * p1 + t1 * v1 = p2 + t2 * v2
     * p1.x + t1 * v1.x = p2.x + t2 * v2.x
     *        t1 * v1.x = p2.x + t2 * v2.x - p1.x
     *        t1        =(p2.x + t2 * v2.x - p1.x)/v1.x

     * p1.y + t1 * v1.y = p2.y + t2 * v2.y
     * p1.y + [(p2.x + t2 * v2.x - p1.x)/v1.x] * v1.y = p2.y + t2 * v2.y
     * p1.y + t2 * v2.x /v1.x + [(p2.x  - p1.x)/v1.x] * v1.y = p2.y + t2 * v2.y
     * p1.y + t2 * v2.x/v1.x * v1.y + [(p2.x  - p1.x)/v1.x] * v1.y = p2.y + t2 * v2.y
     * t2 * v2.x/v1.x * v1.y - t2 * v2.y = p2.y  - p1.y - [(p2.x  - p1.x)/v1.x] * v1.y
     * t2 * [v2.x/v1.x * v1.y - v2.y] = p2.y - p1.y - [(p2.x - p1.x)/v1.x] * v1.y
     */

    // v1.x, v2.x is never zero in input data
    long double div = v2.x/v1.x * v1.y - v2.y;
    if (abs(div) < 1e-10)
      throw runtime_error("Paths parallel.");

    long double t2 = p2.y/div - p1.y/div - ((p2.x/v1.x - p1.x/v1.x) / div) * v1.y;
    long double t1 = p2.x / v1.x + t2 / v1.x * v2.x - p1.x / v1.x;

    // crossed in the past
    if (t2 < 0 || t1 < 0)
      throw runtime_error("Intersection in past.");

    return {p2.x + t2 * v2.x, p2.y + t2 * v2.y};
  }

  size_t count_intersections(long double test_area_min, long double test_area_max) {
    size_t result = 0;
    for (size_t i=0; i<pos.size(); i++) {
      pos[i].x -= test_area_min;
      pos[i].y -= test_area_min;
    }

    for (size_t i=0; i<pos.size()-1; i++) {
      for (size_t j=i+1; j<pos.size(); j++) {

        cout << make_pair(pos[i], vel[i]) << "\n" << make_pair(pos[j], vel[j]) << "\n";

        try {
          auto [x, y] = get_xy_intersection(pos[i], vel[i], pos[j], vel[j]);
          cout << "intersection at: " << (x+test_area_min) << ", " << (y+test_area_min) << " ";
          if (x >= 0
              && y >= 0
              && x <= (test_area_max - test_area_min)
              && y <= (test_area_max - test_area_min)) {
            result ++;
            cout << "in test area";
          }
          else {
            cout << "outside test area";
          }
        }
        catch (runtime_error e) {
          cout << e.what();
        }
        cout << "\n" << endl;
      }
    }


    return result;
  }
};

// 14798 -- too low
int main() {
  string line;
  storm_t storm;
  while (getline(cin, line)) {
    storm.input(line);
  }
  SHOW(storm.pos.size());
  SHOW(storm.vel.size());
  if (storm.pos.size() == 5)
    cout << storm.count_intersections(7, 27) << endl;
  else
    cout << storm.count_intersections(200000000000000, 400000000000000) << endl;
  return 0;
}
