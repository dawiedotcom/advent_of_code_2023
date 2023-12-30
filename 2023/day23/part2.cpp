#include <iostream>
#include <string>

using namespace std;

#include "map.h"

int main() {
  string line;
  map_t map;
  while(getline(cin, line)) {
    map.input(line);
  }
  map.remove_slopes();
  map.build_graph();
  cout << map.get_longest_path() << endl;
  return 0;
}
