#include <string>
#include <iostream>

using namespace std;

#include "platform.h"

int main() {
  string line;

  platform_t platform;
  while (getline(cin, line)) {
    platform.input(line);
  }

  cout << platform.run_cycles(1000000000) << endl;;

  //cout << platform.calc_load() << endl;

  return 0;
}
