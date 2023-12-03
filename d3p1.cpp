#include <iostream>
#include <string>

#include "lib/engine_schematic.h"

using namespace std;

int main() {
  string line;
  engine_schematic engine;

  while (getline(cin, line)) {
    engine.parse_input_line(line);
  }
  engine.show();
  cout << engine.get_part_numbers() << endl;

  return 0;
}
