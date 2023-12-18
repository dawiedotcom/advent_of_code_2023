#include <iostream>
#include <string>
#include <vector>

#include "lib.h"
#include "parser.h"

using namespace std;

#include "trench.h"

struct small_trench_t : trench_t {
  virtual void input(const string& line) {
    parser parse(line);
    char dir = line[0];
    size_t len = parse.next_int();
    //string color = parse.with(R"(#[a-f0-9]+)");

    auto& [x, y] = corners.back();

    if (dir == 'R') corners.emplace_back(x+len, y);
    if (dir == 'L') corners.emplace_back(x-len, y);
    if (dir == 'U') corners.emplace_back(x, y+len);
    if (dir == 'D') corners.emplace_back(x, y-len);
    circ += len;
  }
};

int main() {
  string line;
  small_trench_t trench;
  while (getline(cin, line)) {
    trench.input(line);
  }
  cout << trench.calc_area() << endl;
  return 0;
}
