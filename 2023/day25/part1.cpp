#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>

#include "lib.h"
#include "parser.h"

using namespace std;

class wiring_t {
public:
  unordered_map<string, set<string>> graph;

  wiring_t() {}

  void input(const string& line) {
    parser parse(line);
    string node = parse.next_word();
    while (!parse.done()) {
      string other = parse.next_word();
      graph[node].insert(other);
      graph[other].insert(node);
    }
  }

  void update_cut_uncut(set<string>& a, set<string>& b, unordered_map<string, int>& cut, unordered_map<string, int>& uncut) {
    for (auto [node, neighbors] : graph) {
      cut[node] = 0;
      uncut[node] = 0;
      for (auto& neighbor : neighbors) {
        if ((a.contains(node) && a.contains(neighbor))
            || (b.contains(node) && b.contains(neighbor))) {
          uncut[node] ++;
        }
        else {
          cut[node] ++;
        }
      }
    }
  }

  void update_cut_uncut(set<string>& a, set<string>& b, unordered_map<string, int>& cut, unordered_map<string, int>& uncut, set<string>& nodes) {
    //for (auto [node, neighbors] : graph) {
    for (auto& node : nodes) {
      auto& neighbors = graph[node];
      cut[node] = 0;
      uncut[node] = 0;
      for (auto neighbor : neighbors) {
        if ((a.contains(node) && a.contains(neighbor))
            || (b.contains(node) && b.contains(neighbor))) {
          uncut[node] ++;
        }
        else {
          cut[node] ++;
        }
      }
    }
  }


  size_t find_partition() {

    set<string> a, b;
    unordered_map<string, int> uncut, cut;
    priority_queue<tuple<int, string>> q;

    size_t i=0;
    for (auto [node, adj] : graph) {
      if (i==0) {
        b.insert(node);
      }
      else {
        a.insert(node);
      }
      i++;
    }

    update_cut_uncut(a, b, cut, uncut);
    int cut_total = 0;
    for (auto& node : b)
      cut_total += cut[node];
    for (auto& node : a)
      q.push({cut[node], node});

    //for (auto [node, adj] : graph) {
    //  cout << node
    //       << "[cut:" << cut[node]
    //       << ","
    //       << "uncut:" << uncut[node]
    //       << "]: ";
    //  for (auto& a : adj)
    //    cout << a << " ";
    //  cout << endl;
    //}

    while (!q.empty() && cut_total != 3) {
      auto [c, node] = q.top();
      q.pop();
      if (b.contains(node))
        continue;

      a.erase(node);
      b.insert(node);

      //cut[node] 
      set<string> tmp{node};
      update_cut_uncut(a, b, cut, uncut, tmp);
      update_cut_uncut(a, b, cut, uncut, graph[node]);

      cut_total = 0;
      for (auto& node : b)
        cut_total += cut[node];

      //while (!q.empty()) q.pop();
      for (auto& node : a)
        q.push({cut[node], node});

    }

    SHOW(a.size());
    SHOW(b.size());
    return a.size() * b.size();
  }
  
};

int main() {
  string line;
  wiring_t wiring;
  while (getline(cin,line)) {
    wiring.input(line);
  }
  cout << wiring.find_partition() << endl;

  return 0;
}
