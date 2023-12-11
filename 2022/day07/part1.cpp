#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <numeric>

#include "lib.h"
#include "parser.h"

using namespace std;

#include "directory_tree.h"

int main() {

  string line;
  directory_tree dt;
  while (getline(cin, line)) {
    dt.parse_line(line);
  }
  cout << dt.sum_smallest() << endl;
  return 0;
}
