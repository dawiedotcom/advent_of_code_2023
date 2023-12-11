#include <iostream>
#include <string>
#include <deque>
#include <set>

using namespace std;

int main() {

  string line;
  getline(cin, line);

  deque<char> q;
  size_t pos = 0;
  for (auto c : line) {
    pos ++;
    q.push_back(c);
    if (q.size() > 4) {
      q.pop_front();
      set<char> s(q.begin(), q.end());
      if (s.size() == q.size()) {
        cout << pos << endl;
        break;
      }
    }
  }

  return 0;
}
