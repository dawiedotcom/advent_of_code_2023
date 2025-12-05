#include <iostream>
#include <string>

#include <text_image.h>

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

unsigned int count_accessable_rolls(grid_t& grid) {
  unsigned int result = 0;
  unordered_map<string, cell_t*> neighbors;

  for (size_t x=0; x<grid.X(); x++) {
  for (size_t y=0; y<grid.Y(); y++) {
    unsigned int n_rolls = 0;
    grid.get_neighbors(x, y, neighbors);
    for (auto& [dir, cell] : neighbors) {
      if (*cell == '@') {
        n_rolls++;
      }
    }
    if (grid[x, y] == '@' && n_rolls < 4)
      result ++;
  }
  }
  return result;
}

int main() {

  grid_t grid;

  get_input(grid);
  auto result = count_accessable_rolls(grid);

  cout << result << endl;

  return 0;
}
