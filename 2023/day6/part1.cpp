#include <iostream>
#include <string>

#include "parser.h"

using namespace std;

using ull = unsigned long long;

ull process(long long best_time, long long best_distance) {

  ull num_ways_to_win = 0;
  for (ull t=1; t<=best_time; t++) {
    ull speed = t;
    ull distance = speed * ( best_time - t );
    if (distance > best_distance) {
      num_ways_to_win ++;
    }
  }
  return num_ways_to_win;
}

int main() {
  string line;

  getline(cin, line);
  parser pt(line);
  getline(cin, line);
  parser pd(line);

  pt.to_token("Time:");
  pd.to_token("Distance:");

  unsigned long long prod = 1;
  while (!pt.done()){

    while (!pt.done() && pt.top() == ' ') pt.step(1);
    while (!pt.done() && pd.top() == ' ') pd.step(1);

    prod *= process(pt.next_int(), pd.next_int());
  }
  cout << prod << endl;

  return 0;
}
