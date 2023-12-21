#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <set>

#include "lib.h"
#include "text_image.h"

using namespace std;

class garden_t {
public:
  int start_x, start_y;
  int available;
  text_image<char> grid;

  garden_t()
    : start_x(0)
    , start_y(0)
    , available(0)
  {}

  void input(const string& line) {
    int x=0;
    for (const auto c : line) {
      grid.push_back(c);
      if (c != '#')
        available++;
      if (c == 'S') {
        start_x = x;
        start_y = grid.Y();
      }
      x++;
    }
    grid.start_new_row();
  }

  size_t count_squares(size_t n_steps) {
    size_t result = 0;
    map<tuple<int, int, int>, size_t> d;
    priority_queue<tuple<size_t, int, int, int>> q;
    q.push({0, start_x, start_y, 0});

    while (!q.empty()) {
      auto [dist, xx, yy, step] = q.top();
      dist = -dist;
      q.pop();

      if (d.contains({xx, yy, step}))
        continue;

      d[{xx, yy, step}] = dist;
      if (step == n_steps) {
        result++;
        continue;
      }


      for (auto [new_dir, dx, dy] : vector<tuple<char, int, int>>{
          {'E', 1, 0},
          {'W', -1, 0},
          {'N', 0, -1},
          {'S', 0, 1}
        }) {
        int x = xx + dx;
        int y = yy + dy;
        int new_step = step + 1;

        if (x < 0 || y < 0 || x == grid.X() || y==grid.Y())
          continue;
        if (grid[x, y] == '#')
          continue;

        q.push({-dist-1, x, y, new_step});
      }

    }
    SHOW(d.size());

    set<tuple<int, int>> points;
    for (auto [state, dist] : d) {
      auto [x, y, step] = state;
      if (step == n_steps)
        points.insert({x, y});
    }

    for (int x=0; x<grid.X(); x++) {
      for (int y=0; y<grid.Y(); y++) {
        if (points.contains({x, y}))
          cout << "O";
        else
          cout << grid[x, y];
      }
      cout << '\n';
    }

    return result;
  }
};

int main() {
  string line;
  garden_t garden;
  while (getline(cin, line)) {
    garden.input(line);
  }
  SHOW(garden.grid.X(), garden.grid.Y());
  SHOW(garden.available);
  //cout << garden.count_squares( garden.grid.Y() == 11 ? 6 : 64) << endl;
  if (garden.grid.Y() == 11) {
    cout << garden.count_squares(11) << endl;
    //for (int i=0; i<20; i++)
    //  cout << i << ": " << garden.count_squares(i) << endl;
  }
  else
    cout << garden.count_squares(64) << endl;
  return 0;
}
