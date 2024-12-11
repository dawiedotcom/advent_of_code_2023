#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "show.h"
#include "text_image.h"

using namespace std;

//struct point_t {
//  const int X;
//  int x, y;
//};
//
//bool operator==(const point_t& lhs, const point_t& rhs) noexcept {
//  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
//}

//template<>
//struct std::hash<point_t> {
//  std::size_t operator()(const point_t& point) const noexcept {
//    return point.y * point.X + point.x;
//  }
//};

using cell_t = char;
using grid_t = text_image<cell_t>;
using antenna_t = unordered_map<cell_t, unordered_set<point_t>>;

void read_input(grid_t& grid, antenna_t& antennas) {
  string line;
  int x = 0, y = 0;

  while (getline(cin, line)) {
    for (const auto c : line) {
      grid.push_back(c);
      if (c != '.')
        antennas[c].emplace(x, y, line.size());
      x++;
    }
    grid.start_new_row();
    x = 0;
    y++;
  }
}

void get_antinode(const point_t& a, const point_t& b, point_t& a1) {
  int dx = b.x - a.x;
  int dy = b.y - a.y;
  a1.x = b.x + dx;
  a1.y = b.y + dy;
}

int count_antinodes(const grid_t& grid, const antenna_t& antennas) {
  //int result = 0;
  point_t antinode{0, 0, 0};
  unordered_set<point_t> antinode_locations;

  for (const auto& [antenna_label, points] : antennas) {
    for (const auto& a : points) {
      for (const auto& b : points) {
        if (a==b)
          continue;

        get_antinode(a, b, antinode);
        if (grid.in_bounds(antinode.x, antinode.y)) {
          //result ++;
          antinode_locations.emplace(antinode.x, antinode.y, 0);
          //SHOW(antenna_label, antinode.x, antinode.y);
        }
      }
    }
  }

  return antinode_locations.size();
}

int main() {
  grid_t grid;
  antenna_t antennas;
  read_input(grid, antennas);
  SHOW(grid);


  cout << count_antinodes(grid, antennas) << endl;
  return 0;
}
