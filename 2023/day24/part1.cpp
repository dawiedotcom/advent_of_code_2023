#include <iostream>
#include <string>

#include "storm.h"

using namespace std;

int main() {
  string line;
  storm_t storm;
  while (getline(cin, line)) {
    storm.input(line);
  }
  if (storm.pos.size() == 5)
    cout << storm.count_intersections(7, 27) << endl;
  else
    cout << storm.count_intersections(200000000000000, 400000000000000) << endl;
  return 0;
}
