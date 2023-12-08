#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "parser.h"

using namespace std;

struct node_t {
  string left, right;

  void parse(const string& line) {
    //p.to_token("(");
    //left = p.next_alpha_num_word();
    //p.step(2);
    //right = p.next_alpha_num_word();
    size_t l_start = line.find("(")+1;
    left = line.substr(l_start, 3);
    size_t r_start = line.find(", ")+2;
    right = line.substr(r_start, 3);
  }
};

class directions_t {
public:
  unordered_map<string, node_t> nodes;

  directions_t()
  {}


  void parse(const string& line) {
    parser p(line);

    nodes[line.substr(0, 3)].parse(line);
  }

  bool done(const vector<string>& n) const {
    for (auto& s: n)
      if (s[2] != 'Z')
        return false;
    return true;
  }

  size_t run(const string& dir) {
    parser p(dir);
    size_t steps=0;

    vector<string> current_nodes;
    vector<size_t> first_z;

    cout << "Start: ";
    for (auto& [n, _] : nodes) {
      if (n[2] == 'A') {
        current_nodes.push_back(n);
        first_z.push_back(0);
        cout << current_nodes[current_nodes.size()-1] << " ";
      }
    }
    cout << endl;

    while (!done(current_nodes)) {// && steps < 100) {
      if (p.done())
        p.reset();


      if (p.top() == 'R') {
        for (size_t i=0; i<current_nodes.size(); i++)
          current_nodes[i] = nodes[current_nodes[i]].right;
      }
      else {
        for (size_t i=0; i<current_nodes.size(); i++)
          current_nodes[i] = nodes[current_nodes[i]].left;
      }
      p.step(1);
      steps++;

      for (size_t i=0; i<current_nodes.size(); i++) {
        if (current_nodes[i][2] == 'Z' && first_z[i] == 0) {
          first_z[i] = steps;
          cout << "Cycle start: " << i << " " << current_nodes[i] << " @" << steps << endl;
        }
        else if (current_nodes[i][2] == 'Z' && first_z[i] > 0) {
          cout << "Cycle: " << i << " " << (steps - first_z[i]) << " " << current_nodes[i] << " @" << steps << endl;
          first_z[i] = steps;
        }
      }

      //cout << steps << ": ";
      //for (auto& s: current_nodes)
      //  cout << s << " ";
      //cout << endl;

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
