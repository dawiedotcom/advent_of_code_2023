#include <iostream>
#include <string>

using namespace std;

#include "text_image.h"
#include "lib.h"

using cell_t = char;

struct guard_t {
  int x, y;
  char dir;
};

void get_input(text_image<cell_t>& grid, guard_t& guard) {
  string line;
  int x=0, y=0;
  while(getline(cin, line)) {
    for (auto c : line) {
      grid.push_back(c);
      if (c == '^' || c == '>' || c == 'v' || c == '<') {
        guard.x = x;
        guard.y = y;
        guard.dir = c;
      }
      x++;
    }
    grid.start_new_row();
    x=0;
    y++;
  }
}

int count_steps(text_image<cell_t>& grid, guard_t& guard) {

  int steps = 0;
  while(true) {

    if (grid[guard.x, guard.y] != 'X')
      steps ++;
    grid[guard.x, guard.y] = 'X';

    if (guard.dir == '^') {
      if (guard.y == 0) break;
      if (grid[guard.x, guard.y-1] == '#')
        guard.dir = '>';
      else
        guard.y--;
      continue;
    }
    if (guard.dir == 'v') {
      if (guard.y == grid.Y()-1) break;
      if (grid[guard.x, guard.y+1] == '#')
        guard.dir = '<';
      else
        guard.y++;
      continue;
    }

    if (guard.dir == '>') {
      if (guard.x == grid.X() - 1) break;
      if (grid[guard.x+1, guard.y] == '#')
        guard.dir = 'v';
      else
        guard.x++;
      continue;
    }
    if (guard.dir == '<') {
      if (guard.x == 0) break;
      if (grid[guard.x-1, guard.y] == '#')
        guard.dir = '^';
      else
        guard.x--;
      continue;
    }
  }
  return steps;
}

int main() {

  text_image<cell_t> grid;
  guard_t guard;
  get_input(grid, guard);
  SHOW(guard.x, guard.y, guard.dir);

  cout << count_steps(grid, guard) << endl;

  return 0;
}
