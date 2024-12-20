#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "parser.h"
#include "number_theory.h"

using namespace std;

struct node_t {
  string left, right;

  //void parse(const string& line) {
  void parse(parser& p) {
    p.to_token("(");
    left = p.next_alpha_num_word();
    p.to_token(", ");
    right = p.next_alpha_num_word();
  }
};

class directions_t {
public:
  unordered_map<string, node_t> nodes;

  directions_t()
  {}


  void parse(const string& line) {
    parser p(line);

    nodes[p.next_alpha_num_word()].parse(p);
  }

  bool done(const vector<string>& n) const {
    for (auto& s: n)
      if (s[2] != 'Z')
        return false;
    return true;
  }
  bool cycling(const vector<size_t>& cycles) {
    for (auto c : cycles) {
      if (c == 0) return false;
    }
    cout << "Cycling" << endl;
    return true;
  }
  bool all_same(const vector<size_t>& n) {
    size_t first = n[0];
    for (size_t i=0; i<n.size(); i++)
      if (n[i] != n[0]) return false;
    return true;
  }

  size_t run(const string& dir) {
    parser p(dir);
    size_t steps=0;

    vector<string> current_nodes;
    vector<size_t> first_z, cycle_length;

    cout << "Start: ";
    for (auto& [n, _] : nodes) {
      if (n[2] == 'A') {
        current_nodes.push_back(n);
        first_z.push_back(0);
        cycle_length.push_back(0);
        cout << current_nodes[current_nodes.size()-1] << " ";
      }
    }
    cout << endl;

    while (!cycling(cycle_length)) {
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
          cycle_length[i] = steps - first_z[i];
          cout << "Cycle: " << i << " " << (steps - first_z[i]) << " " << current_nodes[i] << " @" << steps << endl;
          first_z[i] = steps;
        }
      }

      /// c_i * n_i + s_i = N 
    }
    return lcm(cycle_length);
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
