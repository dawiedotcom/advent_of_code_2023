#pragma once

#include <queue>
#include <map>
#include <set>
#include <cassert>

#include "lib.h"
#include "text_image.h"

class map_t {
public:
  text_image<char> grid;
  set<pair<int, int>> junctions;

  map_t()
 {}

  void input(const string& line) {
    for (auto c: line) {
      grid.push_back(c);
    }
    grid.start_new_row();
  }

  bool is_junction(int x, int y) {
    //SHOW(junctions.size());
    if (junctions.contains({x, y}))
      return true;

    size_t count_avail = 0;
    for (auto [nx, ny] : vector<tuple<int, int>>{
        {(x + 1)%grid.X(), y},
        {(x - 1)%grid.X(), y},
        {x, (y + 1)%grid.Y()},
        {x, (y - 1)%grid.Y()},
      }) {

      count_avail += (grid[nx, ny] != '#') ? 1 : 0;
    }
    if (count_avail > 2) {
      //SHOW(x, y, count_avail);
      junctions.emplace(x, y);
      SHOW(junctions.size());
    }
    return (count_avail > 2);
  }

  void no_slopes() {
    for (size_t y=0; y<grid.Y(); y++) {
      for (size_t x=0; x<grid.X(); x++) {
        //if (x > 2 && grid[x, y] == '>' && grid[x-2, y] == '>') {
        //}
        //if (x < grid.X() - 2 && grid[x, y] == '>' && grid[x+2, y] == '>') {
        //  grid[x, y] = '.';
        //  grid[x+2, y] = '<';
        //}
        //else if (y>2 && grid[x, y] == 'v' && grid[x, y-2] == 'v')
        //  grid[x, y] = '.';
        if ((grid[x, y] == '>')
            || (grid[x, y] == '<')
            || (grid[x, y] == '^')
            || (grid[x, y] == 'v'))
          grid[x, y] = '.';
        cout << grid[x, y];
      }
      cout << endl;
    }
  }
  
  size_t get_longest_path() {
    //map<tuple<int, int>, size_t> d;
    set<tuple<size_t, int, int, int, int, set<pair<int, int>>>> d;
    priority_queue<tuple<size_t, int, int, int, int, set<pair<int, int>>>> q;
    q.emplace(0, 1, 0, -1, -1, set<pair<int, int>>{});

    while (!q.empty()) {
      auto [dist, x, y, px, py, path] = q.top();
      //SHOW(dist, x, y);
      q.pop();

      if (d.contains({dist, x, y, px, py, path})) {
        //assert((d[{x, y}] < dist));
        continue;
      }
      if (x == grid.X()-2 && y == grid.Y() - 1)
        SHOW(dist);
        //return dist;
      d.emplace(dist, x, y, px, py, path);// = dist;
      //SHOW(dist, x, y);

      if (x == grid.X()-2 && y == grid.Y()-1) {
        //return dist;
        continue;
      }

      for (auto [nx, ny] : vector<tuple<int, int>>{
          {x + 1, y},
          {x - 1, y},
          {x, y + 1},
          {x, y - 1},
        }) {
        if (px == nx && py == ny)
          continue;
        if (!grid.in_bounds(nx, ny))
          continue;
        if ((grid[nx, ny] == '#')
            || (grid[x, y] == '<' && !(nx == x-1 && ny==y))
            || (grid[x, y] == '>' && !(nx == x+1 && ny==y))
            || (grid[x, y] == 'v' && !(nx == x && ny==y+1))
            || (grid[x, y] == '^' && !(nx == x && ny==y-1)))
          continue;
        if (path.contains({nx, ny}))
          continue;
        if (is_junction(nx, ny))
          path.emplace(nx, ny);
        //SHOW(dist, d.size(), path.size());

        q.emplace(dist+1, nx, ny, x, y, path);
      }
    }
    SHOW(grid.X(), grid.Y());
    size_t result = 0;
    for (auto [dist, x, y, px, py, path] : d) {
      //auto [dist, x, y] = state;
      if (x == grid.X()-2 && y == grid.Y()-1) {
        //SHOW(x, y, dist);
        result = dist > result ? dist : result;
      }
      //result = max(result, d);
    }
    
    return result;
  }
};
