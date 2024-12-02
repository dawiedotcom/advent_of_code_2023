#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "lib.h"

using namespace std;

bool is_good(vector<int>::iterator start, vector<int>::iterator next, vector<int>::iterator end, int dir = 0, int n_skipped=0) {
  if (n_skipped >= 2)
    // Skipped too many levels
    return false;
  if (next >= end) 
    // Reached the end
    return true;
  // Calculate the step
  const int a = *start;
  const int b = *next;
  const int diff = b - a;
  // Keep validating a decreasing series
  if (dir == -1)
    return (diff < 0 && diff >= -3) 
      && (is_good(next, next+1, end, -1, n_skipped) || is_good(next, next+2, end, -1, n_skipped+1));
  // Keep validating a increasing series
  if (dir == 1)
    return (diff > 0 && diff <= 3) 
      && (is_good(next, next+1, end, 1, n_skipped) || is_good(next, next+2, end, 1, n_skipped+1));
  // Start
  return 
    // Increasing
        ((diff > 0 && diff <=  3) && (is_good(next, next+1, end,  1, n_skipped) || is_good(next, next+2, end,  1, n_skipped+1))) 
    // Decreasing
      || ((diff < 0 && diff >= -3) && (is_good(next, next+1, end, -1, n_skipped) || is_good(next, next+2, end, -1, n_skipped+1)))
    // Skip the first level
      || ((is_good(next, next+1, end, 0, n_skipped+1)))
    // Skep the second level
      || ((is_good(start, next+1, end, 0, n_skipped+1)))
      ;
}

bool is_good(vector<int>::iterator start, vector<int>::iterator end) {
  return is_good(start, start+1, end);
}

int main() {

  int safe_count = 0;
  string line;

  while (getline(cin, line)) {
    stringstream line_in(line);
    vector<int> levels;
    int a;
    // Read the levels
    while (line_in >> a)
      levels.push_back(a);
    // Is it safe
    if (is_good(levels.begin(), levels.end())) {
      safe_count ++;
      //SHOW("SAFE", safe_count, levels);
    }
    else {
      //SHOW("UNSAFE", safe_count, levels);
    }
  }
  cout << safe_count << '\n';

  return 0;
}
