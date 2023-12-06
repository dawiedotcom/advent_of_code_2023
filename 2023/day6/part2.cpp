#include <iostream>
#include <string>
#include <algorithm>

#include "parser.h"
#include "2023_day6.h"

using namespace std;

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
