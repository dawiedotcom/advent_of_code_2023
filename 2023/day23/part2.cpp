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
  map.no_slopes();
  cout << map.get_longest_path() << endl;
  //cout << 0 << endl;
  return 0;
}
