#pragma once

#include <vector>
#include <stdexcept>
#include <valarray>

#include "lib.h"
#include "parser.h"
#include "lin_alg.h"

struct point_t {
  long int x, y, z;
};

std::ostream& operator<<(std::ostream& out, const point_t& p) {
  out << p.x << ", " << p.y << ", " << p.z;
  return out;
}

std::ostream& operator<<(std::ostream& out, std::pair<const point_t&, const point_t&> pos_vel) {
  auto& [p, v] = pos_vel;
  out << p << " @ " << v;
  return out;
}

class storm_t {
public:
  std::vector<point_t> pos;
  std::vector<point_t> vel;

  void input(const std::string& line) {
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

  std::pair<point_t, point_t> get_best_pv() {
    /*

      p[i] + t[i] * v[i] = P + t[i] * V

      In general t[i] are rational numbers. For integer steps T[i], we require:
         t[i] = T[i] / gcd(V) = T[i] / gcd(v[i]) 
      for each i. So gcd of components of unknown V should be the the same as gcd of all the known
      velocities.
      If the gcd of at least one v[i] is 1, then gcd of V is 1 and t[i] can only have integer
      values.

          t[i] * (v[i] - V) = P - p[i]
          t[i] = - (P - p[i]) / (V - v[i]) = - P/(dv) + p[i]/dv

      with t[i] integer implies (V-v[i]) is a factor of (P-p[i]) and the remainder of both
      P and p[i] divided by V-v[i] is the same.

      P,V = 24, 13, 10 @ -3, 1, 2
      p[1],v[1] = 19, 13, 30 @ -2, 1, -2
      (19, 13, 30) + 5 * (-2, 1, -2) = (24, 13, 10) + 5 * (-3, 1, 2)

      p[4],v[4] = 12, 31, 28 @ -1, -2, -1
      (12, 31, 28) +  6 * (-1, -2, -1) = (24, 13, 10) + 6 * (-3, 1, 2)

      for N i's

      equations: 3 * N
      unknowns: N + 6

      3 * n = n + 6
      => 2 * n = 6
      => n = 3

      t[i] = (P-p[i]) / (v[i] - V)

      => (P.x - p[i].x) / (V[i].x - V.x) = (P.y - p[i].y) / (V[i].y - V.y)
      => (P.x - p[i].x) * (V[i].y - V.y) = (P.y - p[i].y) * (V[i].x - V.x)

      => (P.x - p[i].x) / (V[i].x - V.x) = (P.z - p[i].z) / (V[i].z - V.z)

      See: https://www.reddit.com/r/adventofcode/comments/18q40he/2023_day_24_part_2_a_straightforward_nonsolver/
    */


    matrix_t<long double> A(6, 6);
    std::valarray<long double> x(0.0, 6), b(0.0, 6);
    for (size_t i=0; i<3; i++) {
      A[2*i, 0] = vel[i+1].y - vel[i].y;
      A[2*i, 1] = vel[i].x - vel[i+1].x;
      A[2*i, 2] = 0;
      A[2*i, 3] = pos[i].y - pos[i+1].y;
      A[2*i, 4] = pos[i+1].x - pos[i].x;
      A[2*i, 5] = 0;

      b[2*i] = pos[i+1].x * vel[i+1].y - pos[i+1].y * vel[i+1].x 
        - pos[i].x * vel[i].y + pos[i].y * vel[i].x;

      A[2*i+1, 0] = vel[i+1].z - vel[i].z;
      A[2*i+1, 1] = 0;
      A[2*i+1, 2] = vel[i].x - vel[i+1].x;
      A[2*i+1, 3] = pos[i].z - pos[i+1].z;
      A[2*i+1, 4] = 0;
      A[2*i+1, 5] = pos[i+1].x - pos[i].x;

      b[2*i+1] = pos[i+1].x * vel[i+1].z - pos[i+1].z * vel[i+1].x 
        - pos[i].x * vel[i].z + pos[i].z * vel[i].x;
    }

    LUPdecompose_t lu(A);
    lu.solve(x, b);

    std::cout << A << std::endl;
    std::cout << b << std::endl;
    std::cout << x << std::endl;

    for (size_t i=0; i<6; i++)
      x[i] += 0.5;

    return {{(long int)x[0],(long int)x[1],(long int)x[2]}, {(long int)x[3],(long int)x[4],(long int)x[5]}};
    
  }

  std::pair<long double, long double> get_xy_intersection(const point_t& p1, const point_t& v1, const point_t& p2, const point_t& v2) const {
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
    long double div = v2.x * v1.y - v2.y * v1.x;
    if (std::abs(div) < 1e-10)
      throw std::runtime_error("Paths parallel.");

    // Testing LU decomposition
    matrix_t<double> m(2, 2);
    m[0, 0] = v1.x;
    m[0, 1] = - v2.x;
    m[1, 0] = v1.y;
    m[1, 1] = -v2.y;

    std::valarray<double> b(0.0, 2);
    b[0] = p2.x - p1.x;
    b[1] = p2.y - p1.y;

    std::valarray<double> t(0.0, 2);
    LUPdecompose_t<double> lu(m);
    lu.solve(t, b);
    
    double t1 = t[0], t2 = t[1];

    /// Alternative for calculating t1 and t2 in O(1) time.
    //long double t2 = p2.y/div - p1.y/div - ((p2.x/v1.x - p1.x/v1.x) / div) * v1.y;
    //long double t1 = p2.x / v1.x + t2 / v1.x * v2.x - p1.x / v1.x;

    // crossed in the past
    if (t2 < 0 || t1 < 0)
      throw std::runtime_error("Intersection in past.");

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

        std::cout << std::make_pair(pos[i], vel[i]) << "\n" << std::make_pair(pos[j], vel[j]) << "\n";

        try {
          auto [x, y] = get_xy_intersection(pos[i], vel[i], pos[j], vel[j]);
          std::cout << "intersection at: " << (x+test_area_min) << ", " << (y+test_area_min) << " ";
          if (x >= 0
              && y >= 0
              && x <= (test_area_max - test_area_min)
              && y <= (test_area_max - test_area_min)) {
            result ++;
            std::cout << "in test area";
          }
          else {
            std::cout << "outside test area";
          }
        }
        catch (std::runtime_error e) {
          std::cout << e.what();
        }
        std::cout << "\n" << std::endl;
      }
    }


    return result;
  }
};
