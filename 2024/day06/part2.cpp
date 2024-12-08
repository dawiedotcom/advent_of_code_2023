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

void reset(text_image<cell_t>& grid, guard_t& guard, int startx, int starty, char startdir) {
  for (int x=0; x<grid.X(); x++)
    for (int y=0; y<grid.Y(); y++)
      if (grid[x, y] != '#') 
        grid[x, y] = '.';
  guard.x = startx;
  guard.y = starty;
  guard.dir = startdir;
  grid[startx, starty] = startdir;
}

bool is_loop(text_image<cell_t>& grid, guard_t& guard) {

  while(true) {

    //grid[guard.x, guard.y] = 'X';

    if (guard.dir == '^') {
      if (guard.y == 0) return false;
      if (grid[guard.x, guard.y-1] == '#') {
        if (grid[guard.x, guard.y] == '>') return true;
        else if (grid[guard.x, guard.y] == '.') grid[guard.x, guard.y] = '>';
        guard.dir = '>';
      }
      else
        guard.y--;
      continue;
    }
    if (guard.dir == 'v') {
      if (guard.y == grid.Y()-1) return false;
      if (grid[guard.x, guard.y+1] == '#') {
        if (grid[guard.x, guard.y] == '<') return true;
        else if (grid[guard.x, guard.y] == '.') grid[guard.x, guard.y] = '<';
        guard.dir = '<';
      }
      else
        guard.y++;
      continue;
    }

    if (guard.dir == '>') {
      if (guard.x == grid.X() - 1) return false;
      if (grid[guard.x+1, guard.y] == '#') {
        if (grid[guard.x, guard.y] == 'v') return true;
        else if (grid[guard.x, guard.y] == '.') grid[guard.x, guard.y] = 'v';
        guard.dir = 'v';
      }
      else
        guard.x++;
      continue;
    }
    if (guard.dir == '<') {
      if (guard.x == 0) return false;
      if (grid[guard.x-1, guard.y] == '#') {
        if (grid[guard.x, guard.y] == '^') return true;
        else if (grid[guard.x, guard.y] == '.') grid[guard.x, guard.y] = '^';
        guard.dir = '^';
      }
      else
        guard.x--;
      continue;
    }
  }
  return false;
}

int count_loops(text_image<cell_t>& grid, guard_t& guard) {
  int startx = guard.x;
  int starty = guard.y;
  char startdir = guard.dir;

  int n_loops = 0;
  for (size_t x=0; x<grid.X(); x++) {
    for (size_t y=0; y<grid.Y(); y++) {
      if (grid[x, y] == '.') {

        grid[x, y] = '#';

        if (is_loop(grid, guard)) {
          n_loops ++;
          //SHOW(grid);
        }

        grid[x, y] = '.';
        reset(grid, guard, startx, starty, startdir);
      }
    }
  }

  return n_loops;
}

int main() {

  text_image<cell_t> grid;
  guard_t guard;
  get_input(grid, guard);
  //SHOW(guard.x, guard.y, guard.dir);

  cout << count_loops(grid, guard) << endl;

  return 0;
}
