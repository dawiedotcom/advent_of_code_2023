#include <iostream>
#include <string>
#include <set>

#include "parser.h"

using namespace std;

int process(string& line) {
  parser p(line);
  p.to_token("Card");
  p.next_int();
  p.to_token(": ");
  set<size_t> winners, played;
  while (p.top() != '|') {
    //p.show();
    size_t num = p.next_int();
    winners.insert(num);
    if (p.top() == ' ') p.step(1);
    //cout << num << " ";
  }
  size_t score = 0;
  while (!p.done()) {
    size_t num = p.next_int();
    //played.insert(num);
    if (winners.contains(num)) {
      if (score == 0)
        score = 1;
      else
        score *= 2;
    }

  }

  return score;
}

int main() {
  string line;

  int sum = 0;
  while (getline(cin, line)) {
    sum += process(line);
  }
  cout << sum << endl;

  return 0;
}
