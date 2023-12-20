#include <iostream>
#include <string>

using namespace std;

#include "system.h"

int main() {
  string line;
  system_t system;
  while (getline(cin, line)) {
    system.input(line);
  }
  cout << system.count_pulses() << endl;
  return 0;
}
