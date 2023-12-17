#pragma once

#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <cmath>

#include "lib.h"
#include "text_image.h"

struct node_t {
  int x, y;
  char dir;
  size_t n_steps;

  node_t(int x, int y, char dir, size_t steps)
    : x(x)
    , y(y)
    , dir(dir)
    , n_steps(steps)
  {}

  node_t(const node_t& other)
    : node_t(other.x, other.y, other.dir, other.n_steps)
  {}

  string repr() const noexcept{
    std::stringstream ss;
    ss << x << "," << y << "," << dir << "," << n_steps;
    return ss.str();
  }

  inline bool operator==(const node_t& rhs) const noexcept{
    return repr() == rhs.repr();
  }
};

template <>
struct std::hash<node_t> {
  std::size_t operator()(node_t const& node) const noexcept {
    return std::hash<std::string>{}(node.repr());
  }
};

struct dist_node_t {
  size_t heat_loss;
  node_t node;

  dist_node_t(size_t heat_loss, int x, int y, char dir, size_t steps)
    : heat_loss(heat_loss)
    , node(x, y, dir, steps)
  {}

  dist_node_t(size_t hl, const node_t& node)
  : heat_loss(hl)
  , node(node)
  {}

  dist_node_t(const dist_node_t& other)
    : dist_node_t(other.heat_loss, other.node.x, other.node.y, other.node.dir, other.node.n_steps)
  {}
};

struct OrderByHeatLossMin {
  constexpr bool operator()(const dist_node_t& lhs, const dist_node_t& rhs) const {
    return rhs.heat_loss < lhs.heat_loss;
  }
};

struct city_t {
  text_image<size_t> blocks;
  
  city_t()
  { }

  void input(const string& line) {

    size_t x=0;
    for (const auto c : line) {
      //blocks.emplace_back(c-'0', x++, blocks.Y());
      blocks.push_back(c - '0');
    }
    
    blocks.start_new_row();
  }

  char dxdy_to_dir(int dx, int dy) {
    if (dx == 1 && dy == 0) return 'E';
    if (dx == -1 && dy == 0) return 'W';
    if (dx == 0 && dy == 1) return 'N';
    if (dx == 0 && dy == -1) return 'S';
    return ' ';
  }
  char reverse(char c) {
    if (c == 'N') return 'S';
    if (c == 'S') return 'N';
    if (c == 'W') return 'E';
    if (c == 'E') return 'W';
    return ' ';
  }

  size_t get_shortest_path(function<bool(const node_t& prev, const node_t& n)> is_valid) {
    unordered_map<node_t, size_t> d;
    priority_queue<dist_node_t, vector<dist_node_t>, OrderByHeatLossMin> q;
    q.emplace(blocks[0, 0], 0, 0, ' ', 0);

    while (!q.empty()) {
      dist_node_t n(q.top());
      q.pop();
      if (d.contains(n.node))
        continue;
      d[n.node] = n.heat_loss;

      //blocks.get_neighbors(n.x, n.y, neighbors);
      for (auto [new_dir, dx, dy] : vector<tuple<char, int,int>>{
          {'E', 1, 0},
          {'W', -1, 0},
          {'N', 0, 1},
          {'S', 0, -1}}) {
        int x = n.node.x + dx;
        int y = n.node.y + dy;
        if (new_dir == reverse(n.node.dir))
          continue;
        if (x < 0 || y < 0 || x == blocks.X() || y==blocks.Y())
          continue;
        //char new_dir = dxdy_to_dir(dx, dy);
        size_t new_step = (new_dir == n.node.dir) ? n.node.n_steps + 1 : 0;

        node_t new_node(x, y, new_dir, new_step);

        if (!is_valid(n.node, new_node))
          continue;

        size_t new_ht = n.heat_loss + blocks[x, y];
        q.emplace(new_ht, new_node); //x, y, new_dir, new_step);
      }
    }

    cout << "Searched " << d.size() << " nodes\n";
    cout << " in " << (blocks.X()*blocks.Y()) << " blocks\n";
    size_t result = 1e9;
    for (auto [node, dist] : d) {
      if (node.x == blocks.X()-1 && node.y == blocks.Y()-1) {
        result = min(result, dist-blocks[0, 0]);
        SHOW(dist-blocks[0, 0], node.n_steps);
      }
    }
    return result;
  }
};
