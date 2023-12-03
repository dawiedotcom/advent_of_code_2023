#pragma once

#include <string>
#include <vector>
#include <set>
#include <cassert>

#include "parser.h"
using namespace std;

class engine_schematic {
private:
  enum entry_type {
    NONE,
    SYMBOL,
    NUMBER,
  };
  struct entry {
    entry_type type;
    size_t value;
    size_t id;
    char symbol;
  };
public:
  engine_schematic() : X(0), Y(0), id(0) {};

  int parse_input_line(string& line) {
    Y++;
    if (X==0) {
      X = line.size();
    }
    cout << line << endl;
    parser p(line);
    size_t x = 0, value=0;
    while (!p.done()) {
      //p.show();

      if(p.top() == '.') {
        x++;
        p.to_token(".");
        //p.step(1);
        entries.push_back({
            .type = NONE,
            .value = 0,
            .id = 0,
            .symbol = '.'
          });
        continue;
      }

      if(isdigit(p.top())) {
        id++;
        value = p.next_int();
        for (size_t i=0; i<p.get_last_token_length(); i++) {
          x++;
          entries.push_back({
              .type = NUMBER,
              .value = value,
              .id = id,
              .symbol='X'
            });
        }
        continue;
      }

      x++;
      entries.push_back({
          .type = SYMBOL,
          .value = 0,
          .id = 0,
          .symbol = p.top()
        });
      //p.to_token("" + p.top());
      p.step(1);
    }
    assert(x==X);

    return 0;
  }

  void show() {
    cout << endl;
    for (size_t i=0; i<entries.size(); i++) {
      cout << entries[i].symbol;
      if ((i+1) % X == 0)
        cout << endl;
    }
    cout << endl;
  }

  int get_part_numbers() {
    vector<size_t> adjacent_idx;
    set<size_t> ids;
    size_t sum = 0;

    for (size_t i=0; i<entries.size(); i++) {
      entry& e = entries[i];
      if (e.type != SYMBOL)
        continue;
      get_adjacent_idx(i, adjacent_idx);
      ids.clear();
      cout << e.symbol << ": ";
      for (auto j : adjacent_idx) {
        entry& ad = entries[j];
        if (ad.type == NUMBER) {
          if (ids.contains(ad.id))
            continue;
          cout << ad.value << " ";
          sum += ad.value;
          ids.insert(ad.id);
        }
      }
      cout << endl;
    }
    return sum;
  }

  int get_gear_ratios() {
    vector<size_t> adjacent_idx;
    set<size_t> ids;
    size_t sum = 0, ratio=1;

    for (size_t i=0; i<entries.size(); i++) {
      entry& e = entries[i];
      if (e.type != SYMBOL)
        continue;
      get_adjacent_idx(i, adjacent_idx);
      ids.clear();
      ratio = 1;
      cout << e.symbol << ": ";
      for (auto j : adjacent_idx) {
        //cout << "[" << j << "] ";
        entry& ad = entries[j];
        if (ad.type == NUMBER) {
          if (ids.contains(ad.id))
            continue;
          cout << ad.value << " ";
          ratio *= ad.value;
          ids.insert(ad.id);
        }
      }
      if (ids.size() == 2 && e.symbol == '*')
        sum += ratio;
      cout << endl;
    }
    return sum;
  }

private:
  //vector<vector<int>> schematic;
  size_t X, Y, id;
  vector<entry> entries;

  inline size_t to_index(const size_t x, const size_t y) const {
    return X * y + x;
  }
  void get_adjacent_idx(const size_t i, vector<size_t>& result) const {
    // Mapping from (x, y) coords to indices
    // (x, y) => X * y + x
    // i => (i % X, i / X)
    //vector<size_t> result;
    result.resize(0);
    size_t x = i % X;
    size_t y = i / Y;

    if (x > 0) result.push_back(to_index(x-1, y));
    if (y > 0) result.push_back(to_index(x, y-1));
    if (x < X-1) result.push_back(to_index(x+1, y));
    if (y < Y-1) result.push_back(to_index(x, y+1));
    if (x > 0 && y > 0) result.push_back(to_index(x-1, y-1));
    if (x > 0 && y < Y-1) result.push_back(to_index(x-1, y+1));
    if (x < X-1 && y > 0) result.push_back(to_index(x+1, y-1));
    if (x < X-1 && y < Y-1) result.push_back(to_index(x+1, y+1));

  }
};
