#include <iostream>
#include <string>
#include <vector>

#include "lib.h"
#include "parser.h"

using namespace std;

#include "trench.h"

struct big_trench_t : public trench_t {
  virtual void input(const string& line) {
    parser parse(line);
    string color = parse.with(R"(#[a-f0-9]+)");
    char dir = color[6];
    size_t len = stoi("0x" + color.substr(1, 5), nullptr, 16);
    //SHOW(dir, len, "0x" + color.substr(1, 5));

    auto& [x, y] = corners.back();

    if (dir == '0') corners.emplace_back(x+len, y);
    if (dir == '2') corners.emplace_back(x-len, y);
    if (dir == '3') corners.emplace_back(x, y+len);
    if (dir == '1') corners.emplace_back(x, y-len);
    circ += len;
  }
};

int main() {
  string line;
  big_trench_t trench;
  while (getline(cin, line)) {
    trench.input(line);
  }
  cout << trench.calc_area() << endl;
  return 0;
}
