#include <iostream>
#include <string>
#include <algorithm>

#include "parser.h"

using namespace std;

using ull = unsigned long long;
using ll = long long;

ull process(long long best_time, long long best_distance) {

  ull num_ways_to_win = 0;
  for (ll t=1; t<=best_time; t++) {
    ull speed = t;
    ull distance = speed * ( best_time - t );
    if (distance > best_distance) {
      //cout << "Winner: " << speed << " " << distance << endl;
      num_ways_to_win ++;
    }
  }
  return num_ways_to_win;
}

int main() {
  string line;

  getline(cin, line);
  line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
  parser pt(line);

  getline(cin, line);
  line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
  parser pd(line);

  unsigned long long ans = process(pt.next_int(), pd.next_int());
  cout << ans << endl;

  return 0;
}
