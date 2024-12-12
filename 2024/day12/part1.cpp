#include <iostream>
#include <string>
#include <set>
#include <vector>

#include "text_image.h"
#include "show.h"

using namespace std;

struct cell_t {
  char label;
  bool visited;
};
using grid_t = text_image<cell_t>;
using region_labels_t = set<char>;

const static auto dxdy = vector<tuple<int,int>>{ {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

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

int find_region(int start_x, int start_y, grid_t& grid) {
  const auto region_label = grid[start_x, start_y].label;
  int area = 0;
  int perimiter = 0;
  vector<tuple<int, int>> region_members{{start_x, start_y}};
  
  while (region_members.size() > 0) {
    auto [x, y] = region_members.back();
    region_members.pop_back();
    if (grid[x, y].visited)
      continue;

    grid[x, y].visited = true;
    area ++;

    for (auto [dx, dy] : dxdy) {
      if (!grid.in_bounds(x+dx, y+dy) || grid[x+dx, y+dy].label != region_label) {
        perimiter += 1;
        continue;
      }
      if (!grid[x+dx, y+dy].visited)
        region_members.push_back({x+dx, y+dy});
    }
  }

  int cost = perimiter * area;
  //SHOW(region_label, area, perimiter, cost);
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
    result += calc_fence_price(region, grid);
  }
  cout << result << endl;
  return 0;
}
