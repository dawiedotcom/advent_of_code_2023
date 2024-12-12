#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <ostream>

#include "text_image.h"
#include "show.h"

using namespace std;

struct cell_t {
  char label;
  bool visited;
};
using grid_t = text_image<cell_t>;
using region_labels_t = set<char>;
using perimeter_t = set<tuple<int, int, int, int>>;

const static auto dxdy = vector<tuple<int,int>>{ {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

ostream& operator<<(ostream& out, const cell_t& cell) {
  return out << (cell.visited ? cell.label : '.');
}

void get_input(grid_t& grid, region_labels_t& region_labels) {
  string line;
  while (getline(cin, line)) {
    for (const auto c : line) {
      grid.push_back({c, false});
      region_labels.insert(c);
    }
    grid.start_new_row();
  }
}

size_t count_sides(perimeter_t& perimeter) {
  size_t n_sides = 2*perimeter.size();
  for (auto& [x, y, dx, dy] : perimeter) {

    // check if positions normal to (dx, dy) are also in the perimiter
    // and facing in the same direction
    if (perimeter.contains({x + dy, y - dx, dx, dy})) {
      n_sides --;
    }
    if (perimeter.contains({x - dy, y + dx, dx, dy})) {
      n_sides --;
    }
  }
  return n_sides/2;
}

int find_region(int start_x, int start_y, grid_t& grid) {
  const auto region_label = grid[start_x, start_y].label;
  int area = 0;
  vector<tuple<int, int>> region_members{{start_x, start_y}};

  perimeter_t perimeter;
  
  while (region_members.size() > 0) {
    auto [x, y] = region_members.back();
    region_members.pop_back();
    if (grid[x, y].visited)
      continue;

    grid[x, y].visited = true;
    area ++;

    for (auto [dx, dy] : dxdy) {
      if (!grid.in_bounds(x+dx, y+dy) || grid[x+dx, y+dy].label != region_label) {
        perimeter.emplace(x, y, dx, dy);
        continue;
      }
      
      if (!grid[x+dx, y+dy].visited)
        region_members.push_back({x+dx, y+dy});
    }
  }

  size_t n_sides = count_sides(perimeter);

  int cost = n_sides * area;
  //SHOW(region_label, area, n_sides, cost);
  return cost;
}

size_t calc_fence_price(char region_label, grid_t& grid) {

  size_t cost = 0;
  for (size_t x=0; x<grid.X(); x++) {
  for (size_t y=0; y<grid.Y(); y++) {
    if (grid[x, y].label != region_label || grid[x, y].visited) 
      continue;

    cost += find_region(x, y, grid);
  }
  }
  return cost;
}

int main() {
  grid_t grid;
  region_labels_t region_labels;
  get_input(grid, region_labels);

  size_t result = 0;
  for (const auto region : region_labels) {

    for (size_t x=0; x<grid.X(); x++) 
      for (size_t y=0; y<grid.Y(); y++) 
        grid[x, y].visited = false;
    result += calc_fence_price(region, grid);
    //SHOW(grid);
  }
  cout << result << endl;
  return 0;
}
