#pragma once

#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <cassert>
#include <algorithm>

#include "lib.h"
#include "text_image.h"

using namespace std;

/*
 * Hashable struct that represents a node in the graph. This can be
 * used with unordered_set, and gives about 50% speedup over set<tuple<int, int>>.
 */
struct node_t {
  int x, y;

};

constexpr bool operator==(const node_t& lhs, const node_t& rhs) noexcept {
  return lhs.x==rhs.x && lhs.y==rhs.y;
}

template<>
struct std::hash<node_t> {
  size_t operator()(const node_t& n) const noexcept {
    return (n.x + 103) * (n.y + 101);
  }
};

class map_t {
public:
  text_image<char> grid;
  unordered_set<node_t> junctions;
  unordered_map<node_t, vector<pair<node_t, int>>> graph; 
  unordered_set<node_t> seen;
  node_t end;

  map_t()
  {
    seen.reserve(50);
  }

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
    graph.emplace(node_t{x, y}, vector<pair<node_t, int>>{});
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
    end.x = grid.X()-2;
    end.y = grid.Y()-1;
    stack<pair<node_t, int>> s;
    unordered_set<node_t> seen;
    for (size_t x=0; x<grid.X(); x++)
      for (size_t y=0; y<grid.Y(); y++)
        if (is_node(x, y))
          add_node(x, y);
    //show_graph();

    for (auto [last_node, _] : graph) {
      //auto [x_last_node, y_last_node] = point; 

      s.push({last_node, 0});
      seen.clear();
      seen.insert(last_node);

      while (!s.empty()) {
        auto [node, dist] = s.top();
        s.pop();
        seen.insert(node);

        if (dist > 0 && is_node(node.x, node.y)) {
          graph[last_node].emplace_back(node, dist);
          continue;
        }
       
        //seen.emplace(x, y);
        for (auto [nx, ny] : vector<tuple<int, int>>{
            {node.x + 1, node.y},
            {node.x - 1, node.y},
            {node.x, node.y + 1},
            {node.x, node.y - 1},
            }) {
          if (!grid.in_bounds(nx, ny) 
              || !is_valid_move(node.x, node.y, nx, ny) 
              || seen.contains(node_t{nx, ny}))
            continue;
          s.emplace(node_t{nx, ny}, dist+1);
        }
      }
    }

    //show_graph();
  }

  void show_graph() {

    for (auto [node, adj] : graph) {
      cout << "(" << node.x << "," << node.y << "): ";
      for (const auto& a : adj) {
        auto [next, dist] = a;
        cout << "(" << next.x << "," << next.y << ")[" << dist << "] ";
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
    stack<tuple<node_t, int, unordered_set<node_t>>> s;

    int n_paths = 0;
    int result = 0;
    s.emplace(node_t{1, 0}, 0, unordered_set<node_t>{});

    while (!s.empty()) {
      auto [node, dist, seen] = s.top();
      s.pop();

      if (node == end) {
        n_paths ++;
        result = std::max(result, dist);
        //SHOW(n_paths, result, dist, s.size());
        continue;
      }

      if (!seen.contains(node)) {
        seen.insert(node);
        for (auto [next, nd] : graph[node])
          s.emplace(next, dist + nd, seen);
      }

    }
    SHOW(n_paths);
          
    return result;
  }

  int get_longest_path_rec(node_t node = {1, 0}) {
    if (node == end)
      return 0;

    int result = -1;
    seen.insert(node);
    for (auto [next, dist] : graph[node]) {
      if (!seen.contains(next)) {
        //SHOW(x, y, nx, ny, nd, result);
        int n_result = get_longest_path_rec(next);
        result = n_result >= 0 
          ? std::max(result, n_result + dist)
          : result;
      }
    }
    seen.erase(node);

    return result;
  }
};
