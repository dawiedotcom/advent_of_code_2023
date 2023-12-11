#include <string>
#include <iostream>
#include <unordered_map>

#include "parser.h"

using namespace std;

struct node_t {
  string left, right;

  void parse(parser& p) {
    p.to_token("(");
    left = p.next_word();
    p.step(2);
    right = p.next_word();
  }
};

class directions_t {
public:
  unordered_map<string, node_t> nodes;

  directions_t()
  {}


  void parse(const string& line) {
    parser p(line);

    nodes[p.next_word()].parse(p);
  }

  size_t run(const string& dir) {
    parser p(dir);
    size_t steps=0;

    string current_node = "AAA";
    while (current_node != "ZZZ") {// && !p.done()) {
      if (p.done())
        p.reset();
      cout << current_node << " going " << p.top() << endl;
      if (p.top() == 'R')
        current_node = nodes[current_node].right;
      else
        current_node = nodes[current_node].left;
      p.step(1);
      steps++;
    }

    return steps;
  }

};

int main() {
  string line;
  string dir;

  getline(cin, dir);
  getline(cin, line);

  directions_t directions;
  while (getline(cin, line)) {
    directions.parse(line);
  }

  cout << directions.run(dir) << endl;

  return 0;
}
