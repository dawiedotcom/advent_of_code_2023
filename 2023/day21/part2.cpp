#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <queue>
#include <cassert>
#include <set>

#include "lib.h"
#include "text_image.h"

using namespace std;

class square_t {
public:
  char c;
  bool is_known;
  bool is_even;
  square_t(char c)
    : c(c)
    , is_known(false)
    , is_even(false)
  {}
};

class garden_t {
public:
  int start_x, start_y;
  text_image<square_t> grid;

  garden_t()
    : start_x(0)
    , start_y(0)
  {}

  void input(const string& line) {
    int x=0;
    for (const auto c : line) {
      grid.emplace_back(c);
      if (c == 'S') {
        start_x = x;
        start_y = grid.Y();
      }
      x++;
    }
    grid.start_new_row();
  }

  void reset() {
    for (size_t x=0; x<grid.X(); x++) 
      for (size_t y=0; y<grid.Y(); y++) 
        grid[x, y].is_known = false;
  }

  size_t count_squares(size_t n_steps) {
    reset();
    map<tuple<int, int, int, int>, size_t> d;
    //map<tuple<int, int, int, int>, size_t> parity;
    priority_queue<tuple<size_t, int, int, int, int>> q;
    //set<tuple<int, int, int>> dist_to_point;
    q.push({0, start_x, start_y, 0, 0});

    while (!q.empty()) {
      auto [dist, xx, yy, x_wrap, y_wrap] = q.top();
      dist = -dist;
      q.pop();

      if (d.contains({xx, yy, x_wrap, y_wrap}))
        continue;

      d[{xx, yy, x_wrap, y_wrap}] = dist;
      //grid[xx, yy].is_known = true;
      //grid[xx, yy].is_even = (dist % 2 == 0);
      //dist_to_point.insert({dist, xx, yy});

      //if (dist >= 2*grid.X()) {
      //  continue;
      //}

      for (auto [new_dir, dx, dy] : vector<tuple<char, int, int>>{
          {'E', 1, 0},
          {'W', -1, 0},
          {'N', 0, -1},
          {'S', 0, 1}
        }) {
        int x = xx + dx;
        int y = yy + dy;
        int new_step = dist + 1;
        int nx_wrap = x_wrap;
        int ny_wrap = y_wrap;

        //if (x < 0 || y < 0 || x == grid.X() || y==grid.Y())
        //  continue;
        if (x<0) {
          x += grid.X(); 
          nx_wrap --;
        }
        if (y<0) {
          y += grid.Y(); 
          ny_wrap --;
        }
        if (x>=grid.X()) {
          x-=grid.X(); 
          nx_wrap ++;
        }
        if (y>=grid.Y()) {
          y-=grid.Y(); 
          ny_wrap ++;
        }

        if (grid[x, y].c == '#')
          continue;

        if (ny_wrap >= 3 || ny_wrap <= -2 || nx_wrap >= 3 || nx_wrap <= -2)
          continue;

        q.push({-new_step, x, y, nx_wrap, ny_wrap});
      }
    }
    SHOW(d.size());

    int min_width = 10*grid.X();
    int min_height = 10*grid.X();
    vector<int> width(grid.Y());
    vector<int> height(grid.X());
    for (int i=0; i<grid.X(); i++) {
      //min_width = std::min(d[{0, i}] + d[{grid.X()-1, i}], min_width);
      //min_height = std::min(d[{i, 0}] + d[{i, grid.Y()-1}], min_height);

      //width[i] = 
      if (d[{0, i, 0, 0}] + d[{grid.X()-1, i, 0, 0}] < min_width)
        min_width = d[{0, i, 0, 0}] + d[{grid.X()-1, i, 0, 0}];

      if (d[{i, 0, 0, 0}] + d[{i, grid.Y()-1, 0, 0}] < min_height)
        min_height = d[{i, 0, 0, 0}] + d[{i, grid.Y()-1, 0, 0}];
    }
 
    size_t result = 0;
    map<tuple<int, int>, int> min_by_wrap;
    for (auto [state, dist] : d) {

      auto [x, y, x_wrap, y_wrap] = state;

      if(!min_by_wrap.contains({x_wrap, y_wrap}))
         min_by_wrap[{x_wrap,y_wrap}] = dist;
      else
        if (min_by_wrap[{x_wrap, y_wrap}] > dist)
          min_by_wrap[{x_wrap, y_wrap}] = dist;

      if (grid[x, y].c == '#')
        continue;

      if (dist > n_steps)
        continue;

      if (dist <= n_steps && dist % 2 == 0) // TODO odd
        result ++;

    }
    //for (auto [state, dist] : d) {
    //  auto [x, y, x_wrap, y_wrap, step] = state;
    //  if (x==10 and y==10)
    //    SHOW(x, y, x_wrap, y_wrap, step);
    //}
    //SHOW(d.size());
    
    SHOW(min_width, min_height);

    int center_x = start_x + grid.X();
    int center_y = start_y + grid.Y();

    for (int yy=0; yy<4*grid.Y(); yy++) {
      if (yy > 0 && yy % grid.Y() == 0)
        cout << string(3*4*(grid.X())+6, '-') << '\n'; 
      for (int xx=0; xx<4*grid.X(); xx++) {
        if (xx % grid.X() == 0 && xx > 0)
          cout << " | ";
        //int x = xx - center_x;
        //int y = yy - center_y;
        int x_mod = xx%grid.X();
        int y_mod = yy%grid.Y();
        int x_wrap = xx / grid.X() - 1;
        int y_wrap = yy / grid.Y() - 1;

        if (xx == center_x && yy == center_y) {
          cout << "[ C]";
        }
        else if (d.contains({x_mod, y_mod, x_wrap, y_wrap})) {

          int dist = d[{x_mod, y_mod, x_wrap, y_wrap}];
          int dist_max = abs(x_mod-start_x) + abs(y_mod - start_y);

          cout
            << "["
            //<< (grid[x_mod, y_mod].is_even ? "e" : "o");
            //<< ","
            //<< (dist == n_steps ? 'O' : grid[x_mod, y_mod].c)
            << setfill(' ') << setw(2)
            //<< - x_wrap * min_width - y_wrap * min_height + dist
            //<< abs(x_wrap) * min_width + abs(y_wrap) * min_height + dist
            //<< dist
            << (dist - min_by_wrap[{x_wrap, y_wrap}])
            << "]"
            ;
        }
        else
          cout
            << "["
            << setfill(' ') << setw(2) 
            << grid[x_mod, y_mod].c
            << "]"
            ;
      }
      cout << '\n';
    }
    //*/



    return result;
  }
};

int main() {
  string line;
  garden_t garden;
  while (getline(cin, line)) {
    garden.input(line);
  }
  if (garden.grid.Y() == 11) {
    for (const auto i : {6, 10, 50}) { //, 100, 500, 1000, 5000}) {
      auto result = garden.count_squares(i);
      cout << i << ": " << result << endl;
    }
  }
  else
    cout << garden.count_squares(64) << endl;
  return 0;
}
