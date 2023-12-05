#include <iostream>
#include <string>

#include "cube_game_parser.h"

using namespace std;

int process(string& line) {
  cube_game_parser parser(line);

  size_t game = parser.game();
  size_t red=1, green=1, blue=1;
  while (!parser.done()) {
    red = green = blue = 0;
    parser.round(red, green, blue);
    cout << "Round: " << red << " red, " << green << " green, " << blue << " blue" << endl;

    if (red > 12 || green > 13 || blue > 14)
      return 0;
  }

  return game;
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
