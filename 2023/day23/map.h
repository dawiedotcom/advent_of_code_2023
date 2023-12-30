#pragma once

#include <queue>
#include <map>
#include <set>
#include <stack>
#include <cassert>
#include <algorithm>

#include "lib.h"
#include "text_image.h"

using namespace std;

class map_t {
public:
  text_image<char> grid;
  set<pair<int, int>> junctions;
  map<pair<int, int>, vector<tuple<int, int, int>>> graph; 

  map_t()
  {}

  void input(const string& line) {
    for (auto c: line) {
      grid.push_back(c);
    }
    grid.start_new_row();
  }

  bool is_junction(int x, int y) {
    if (junctions.contains({x, y}))
      return true;
    if (grid[x, y] == '#')
      return false;

    size_t count_avail = 0;
    for (auto [nx, ny] : vector<tuple<int, int>>{
        {(x + 1)%grid.X(), y},
        {(x - 1)%grid.X(), y},
        {x, (y + 1)%grid.Y()},
        {x, (y - 1)%grid.Y()},
      }) {

      count_avail += (grid[nx, ny] != '#') ? 1 : 0;
    }
    if (count_avail > 2)
      junctions.emplace(x, y);

    return (count_avail > 2);
  }

  bool is_node(int x, int y) {
    return is_junction(x, y) 
      || (x == grid.X()-2 && y == grid.Y()-1)
      || (x == 1 && y == 0);
  }

  constexpr void add_node(int x, int y) {
    graph.emplace(make_pair(x, y), vector<tuple<int, int, int>>{});
  }


  bool is_valid_move(int x, int y, int nx, int ny) {
    return !((grid[nx, ny] == '#')
        || (grid[nx, ny] == '<' && !(nx == x-1 && ny==y))
        || (grid[nx, ny] == '>' && !(nx == x+1 && ny==y))
        || (grid[nx, ny] == 'v' && !(nx == x && ny==y+1))
        || (grid[nx, ny] == '^' && !(nx == x && ny==y-1)));
  }

  /* DFS to build a graph with junctions as nodes */
  void build_graph() {
    stack<tuple<int, int, int>> s;
    set<pair<int, int>> seen;
    for (size_t x=0; x<grid.X(); x++)
      for (size_t y=0; y<grid.Y(); y++)
        if (is_node(x, y))
          add_node(x, y);
    //show_graph();

    for (auto [point, _] : graph) {
      auto [x_last_node, y_last_node] = point; 

      s.push({x_last_node, y_last_node, 0});
      seen.clear();
      seen.emplace(x_last_node, y_last_node);

      while (!s.empty()) {
        auto [x, y, dist] = s.top();
        s.pop();
        seen.emplace(x, y);

        if (dist > 0 && is_node(x, y)) {
          graph[{x_last_node, y_last_node}].emplace_back(x, y, dist);
          continue;
        }
       
        seen.emplace(x, y);
        for (auto [nx, ny] : vector<tuple<int, int>>{
            {x + 1, y},
            {x - 1, y},
            {x, y + 1},
            {x, y - 1},
            }) {
          if (!grid.in_bounds(nx, ny) || !is_valid_move(x, y, nx, ny) || seen.contains({nx, ny}))
            continue;
          s.emplace(nx, ny, dist+1);
        }
      }
    }

    //show_graph();
  }

  void show_graph() {

    for (auto [node, adj] : graph) {
      auto [x, y] = node;
      cout << "(" << x << "," << y << "): ";
      for (const auto& a : adj) {
        auto [nx, ny, dist] = a;
        cout << "(" << nx << "," << ny << ")[" << dist << "] ";
      }
      cout << endl;
    }
  }

  void remove_slopes() {
    for (size_t y=0; y<grid.Y(); y++) {
      for (size_t x=0; x<grid.X(); x++) {
        if ((grid[x, y] == '>')
            || (grid[x, y] == '<')
            || (grid[x, y] == '^')
            || (grid[x, y] == 'v'))
          grid[x, y] = '.';
        //cout << grid[x, y];
      }
      //cout << endl;
    }
  }
  
  int get_longest_path() {
    //map<tuple<int, int>, size_t> d;
    //set<tuple<int, int>> seen;
    stack<tuple<int, int, int, set<tuple<int, int>>>> s;

    int n_paths = 0;
    int result = 0;
    s.emplace(1, 0, 0, set<tuple<int, int>>{});

    while (!s.empty()) {
      auto [x, y, dist, seen] = s.top();
      s.pop();

      if (x == grid.X()-2 && y == grid.Y()-1) {
        n_paths ++;
        result = std::max(result, dist);
        continue;
      }

      if (!seen.contains({x, y})) {
        seen.emplace(x, y);
        for (auto [nx, ny, nd] : graph[{x, y}])
          s.emplace(nx, ny, dist + nd, seen);
      }

    }
    SHOW(n_paths);
          
    return result;
  }
};
