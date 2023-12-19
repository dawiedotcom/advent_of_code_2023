#include <iostream>
#include <string>

using namespace std;

#include "sorter.h"

int main() {
  string line;
  sorter_t sorter;
  bool reading = true;
  while (getline(cin, line)) {
    if (line.size() == 0) {
      break;
    }
    sorter.input(line);
  }
  cout << sorter.get_all_combinations() << endl;
  return 0;
}
