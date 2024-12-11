#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>

#include "text_image.h"
#include "show.h"
#include "point.h"

using namespace std;

using cell_t = char;
using grid_t = text_image<cell_t>;

void get_input(grid_t& grid) {
  string line;
  while (getline(cin, line)) {
    for (auto c : line) {
      grid.push_back(c);
    }
    grid.start_new_row();
  }
}

bool is_valid_move(grid_t& grid, point_t p, point_t dp) {
  return grid.in_bounds(p.x + dp.x, p.y + dp.y) && (grid[p+dp] == (grid[p] + 1));
}

int count_paths(grid_t& grid, const point_t& start) {
  set<tuple<int, int>> seen;
  map<tuple<int, int, int>, size_t> d;
  priority_queue<tuple<size_t, int, int, int>> q;
  q.push({0, start.x, start.y, 0});

  while (!q.empty()) {
    auto [dist, x, y, step] = q.top();
    dist = -dist;
    q.pop();

    if (d.contains({x, y, step}))
      continue;

    d[{x, y, step}] = dist;
    for (auto [dx, dy] : vector<tuple<int, int>>{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} }) {


      //SHOW(x, y, dx, dy, dist, grid[x,y], is_valid_move(grid, {x, y, 0}, {dx, dy, 0}));
      if (! grid.in_bounds(x + dx, y + dy))
        continue;

      if (!is_valid_move(grid, {x, y, 0}, {dx, dy, 0}))
        continue;

      if (grid[x+dx, y+dy] == '9') {
        SHOW(x+dx, y+dy, step+1);
        seen.emplace(x+dx, y+dy);
        continue;
      }

      q.push({-dist-1, x+dx, y+dy, step+1});

    }
  }
  return seen.size();

}

int count_paths(grid_t& grid) {
  int result = 0;
  for (int y=0; y<grid.Y(); y++) {
  for (int x=0; x<grid.X(); x++) {
    if (grid[x, y] == '0') {
      int dr = count_paths(grid, {x, y, 0});
      SHOW(x, y, dr);
      result += dr;
    }
  }
  }
  return result;
}
int main() {
  grid_t grid;
  get_input(grid);
  SHOW(grid);

  cout << count_paths(grid) << endl;
  return 0;
}
