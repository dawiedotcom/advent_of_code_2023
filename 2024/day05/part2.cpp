#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

#include "lib.h"
#include "parser.h"

using namespace std;

void reorder(const unordered_map<int, unordered_set<int> >& ordering, vector<int>& printing) {

  sort(
      printing.begin(),
      printing.end(),
      [&](auto a, auto b) { return ordering.at(a).contains(b); }
      );

}

int main() {
  string line;
  unordered_map<int, unordered_set<int> > ordering;
  // read ordering
  while (getline(cin, line)) {
    if (line.size() == 0)
      break;
    parser p(line);
    int a = p.next_int();
    int b = p.next_int();

    ordering[a].insert(b);
  }

  int result = 0;
  while (getline(cin, line)) {
    vector<int> printing;
    parser p(line);
    while (!p.done()) {
      printing.push_back(p.next_int());
    }
    //SHOW(printing);
    bool is_valid = true;
    for (auto i=printing.begin(); i!=printing.end()-1; i++) {
      auto j = i + 1;
      is_valid = ordering[*i].contains(*j);
      if (!is_valid) break;
    }
    if (!is_valid) {
      reorder(ordering, printing);
      result += printing[printing.size()/2];
    }
  }
  cout << result << endl;

  return 0;
}
