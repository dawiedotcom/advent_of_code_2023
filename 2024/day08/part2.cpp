#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "show.h"
#include "text_image.h"

using namespace std;

struct point_t {
  const int X;
  int x, y;

  point_t& operator=(const point_t rhs) {
    //if (rhs == *this) return *this;
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

  point_t operator+(const point_t& rhs) const {
    return {X, x+rhs.x, y+rhs.y};
  }
};


bool operator==(const point_t& lhs, const point_t& rhs) noexcept {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

template<>
struct std::hash<point_t> {
  std::size_t operator()(const point_t& point) const noexcept {
    return point.y * point.X + point.x;
  }
};

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
        antennas[c].emplace(line.size(), x, y);
      x++;
    }
    grid.start_new_row();
    x = 0;
    y++;
  }
}

void get_antinode_delta(const point_t& a, const point_t& b, point_t& a1) {
  a1.x = b.x - a.x;
  a1.y = b.y - a.y;
}

int count_antinodes(const grid_t& grid, const antenna_t& antennas) {
  //int result = 0;
  point_t antinode_delta{0, 0, 0};
  point_t antinode{0,0,0};

  unordered_set<point_t> antinode_locations;

  for (const auto& [antenna_label, points] : antennas) {
    for (const auto& a : points) {
      for (const auto& b : points) {
        if (a==b)
          continue;

        get_antinode_delta(a, b, antinode_delta);
        antinode = b + antinode_delta;
        
        antinode_locations.emplace(0, b.x, b.y);

        while (grid.in_bounds(antinode.x, antinode.y)) {
          antinode_locations.emplace(0, antinode.x, antinode.y);
          antinode = antinode + antinode_delta;
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
  //SHOW(grid);
  cout << count_antinodes(grid, antennas) << endl;
  return 0;
}
