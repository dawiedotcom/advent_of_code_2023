#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "lib.h"

using namespace std;

#include "universe.h"

int main() {
  string line;
  unsigned long long sum=0;

  universe u;
  while(getline(cin, line)) {
    u.parse_input(line);
  }
  u.expand();
  cout << u.calc_distances() << endl;

  return 0;
}
