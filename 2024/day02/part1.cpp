#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "lib.h"

using namespace std;

int main() {

  int safe_count = 0;
  bool increasing = false;
  
  string line;
  while (getline(cin, line)) {
    stringstream line_in(line);
    vector<int> numbers;
    int a, b, diff;
    
    line_in >> a >> b;
    diff = b - a;
    increasing = (diff > 0);

    cout << "\n";
    a = b;
    while ((line_in >> b) && diff != 0 && abs(diff) <= 3) {
      diff = b - a;
      if ((increasing && diff < 0) || (!increasing && diff > 0)) {
        diff = 0;
        break;
      }
      a = b;
    }
    if ((diff != 0) && (abs(diff) <= 3))
      safe_count += (line_in.tellp() == -1) ? 1 : 0;
  }
  cout << safe_count << '\n';

  return 0;
}
