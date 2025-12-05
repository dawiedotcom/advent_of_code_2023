#include <iostream>
#include <string>

#include <text_image.h>
#include <show.h>

using namespace std;

using cell_t=char;
using grid_t=text_image<cell_t>;

void get_input(grid_t& grid) {
  string line;
  while (getline(cin, line)) {
    for (const auto c : line) {
      grid.push_back(c);
    }
    grid.start_new_row();
  }
}

bool is_accessable(size_t x, size_t y, grid_t& grid) {
  unordered_map<string, cell_t*> neighbors;
  unsigned int n_rolls = 0;
  grid.get_neighbors(x, y, neighbors);
  for (auto& [dir, cell] : neighbors) {
    if (*cell == '@') {
      n_rolls++;
    }
  }
  return grid[x, y] == '@' && n_rolls < 4;
}

/*
unsigned int count_accessable_rolls(grid_t& grid) {
  unsigned int result = 0;

  for (size_t x=0; x<grid.X(); x++) {
  for (size_t y=0; y<grid.Y(); y++) {
    if (is_accessable(x, y, grid))
      result ++;
  }
  }
  return result;
}
*/

unsigned int remove_accessable_rolls(grid_t& grid) {
  vector<tuple<size_t, size_t>> rolls_to_remove;
  for (size_t x=0; x<grid.X(); x++) {
  for (size_t y=0; y<grid.Y(); y++) {
    if (is_accessable(x, y, grid))
      rolls_to_remove.emplace_back(x, y);
  }
  }
  for (auto& [x, y] : rolls_to_remove)
    grid[x, y] = '.';
  return rolls_to_remove.size();
}

int main() {

  grid_t grid;

  get_input(grid);

  unsigned int result = 0, n_removable_rolls = 1;

  SHOW(grid);
  for (;;) {
    //n_removable_rolls = count_accessable_rolls(grid);

    n_removable_rolls = remove_accessable_rolls(grid);
    if (n_removable_rolls == 0)
      break;
    result += n_removable_rolls;
    //cout << "\033[2J\033[1;1H";
    SHOW(n_removable_rolls, result);
    SHOW(grid);
  }

  cout << result << endl;

  return 0;
}
