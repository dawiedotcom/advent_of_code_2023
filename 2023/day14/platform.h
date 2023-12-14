#pragma once

#include <sstream>
#include <unordered_map>

#include "lib.h"
#include "text_image.h"

struct platform_t {
  text_image<char> ti;
  unordered_map<char, size_t> counts;

  platform_t()
  {}

  void input(const string& line) {
    for (const auto c : line) {
      ti.push_back(c);
      counts[c] ++;
    }

    ti.start_new_row();
  }

  void roll_north() {
    for (size_t y=1; y<ti.Y(); y++) {
      for (size_t x=0; x<ti.X(); x++) {
        if (ti[x, y] == 'O') {
          size_t k=y;
          while (k>0 && ti[x, k-1] == '.') {
            ti[x, k-1] = 'O';
            ti[x, k] = '.';
            --k;
          }
        }
      }
    }
  }

  void roll_west() {
    for (size_t y=0; y<ti.Y(); y++) {
      for (size_t x=1; x<ti.X(); x++) {
        if (ti[x, y] == 'O') {
          size_t k=x;
          while (k>0 && ti[k-1, y] == '.') {
            ti[k-1, y] = 'O';
            ti[k, y] = '.';
            --k;
          }
        }
      }
    }
  }


  void roll_south() {
    for (size_t y=ti.Y()-1; y>0; y--) {
      for (size_t x=0; x<ti.X(); x++) {
        if (ti[x, y-1] == 'O') {
          size_t k=y;
          while (k<ti.X() && ti[x, k] == '.') {
            ti[x, k] = 'O';
            ti[x, k-1] = '.';
            ++k;
          }
        }
      }
    }
  }

  void roll_east() {
    for (size_t y=0; y<ti.X(); y++) {
      for (size_t x=ti.Y()-1; x>0; x--) {
        if (ti[x-1, y] == 'O') {
          //SHOW(y, x, ti[x, y]);
          size_t k=x;
          while (k<ti.Y() && ti[k, y] == '.') {
            ti[k, y] = 'O';
            ti[k-1, y] = '.';
            ++k;
          }
        }
      }
    }
  }

  string to_string() {
    std::stringstream ss;
    ss << ti;
    return ss.str();
  }

  size_t run_cycles(size_t n_roll_cycles) {
    string input, output;
    size_t loop_length=0, loop_start=0;

    unordered_map<string, string> cycle_step;
    unordered_map<string, size_t> cycle_load;

    for (size_t c=1; c<=n_roll_cycles; c++) {
      input = to_string();

      if (cycle_step.contains(input)) {
        cout << "loop detected at " << (c) << "!!!" << endl; 
        if (loop_start == 0)
          loop_start = c;

        output = input;
        vector<size_t> step_load;

        do {
          step_load.push_back(cycle_load[output]);
          output = cycle_step[output];
          loop_length++;
        } while (input != output);
        size_t loop_step_at_end = ((n_roll_cycles - c) % loop_length);

        SHOW(loop_length);
        SHOW(step_load.size());
        cout << "Looping through loads: " << step_load << endl;
        SHOW(loop_step_at_end);

        return step_load[loop_step_at_end];
      }

      roll_north();
      roll_west();
      roll_south();
      roll_east();

      output = to_string();
      cycle_step[input] = output;
      cycle_load[input] = calc_load();
    }
    return calc_load();
  }

  size_t calc_load() {
    size_t load = 0;
    for (size_t x=0; x<ti.X(); x++) {
      for (size_t y=0; y<ti.Y(); y++) {
        if (ti[x, y] == 'O') {
          load += ti.Y() - y;
        }
      }
    }
    return load;
  }
};
