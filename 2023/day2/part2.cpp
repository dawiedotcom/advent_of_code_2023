#include <iostream>
#include <string>

#include "cube_game_parser.h"

using namespace std;

int process(string& line) {
  cube_game_parser parser(line);

  size_t game = parser.game();
  size_t red=1, green=1, blue=1;
  size_t red_max=1, green_max=1, blue_max=1;
  while (!parser.done()) {
    red = green = blue = 0;
    parser.round(red, green, blue);
    cout << "Round: " << red << " red, " << green << " green, " << blue << " blue" << endl;
    if (blue_max < blue) blue_max = blue;
    if (red_max < red) red_max = red;
    if (green_max < green) green_max = green;
  }

  return red_max * green_max * blue_max;
}

int main() {
  string line;

  int sum = 0;
  while (getline(cin, line)) {
    sum += process(line);
  }
  cout << sum << endl;

  return 0;
}
