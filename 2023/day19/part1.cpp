#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "lib.h"
#include "parser.h"

using namespace std;

#include "sorter.h"

int main() {
  string line;
  sorter_t sorter;
  bool reading = true;
  while (getline(cin, line)) {
    if (line.size() == 0) {
      reading = false;
      continue;
    }
    if (reading) 
      sorter.input(line);
    else
      sorter.sort(line);

  }
  cout << sorter.sum << endl;
  return 0;
}
