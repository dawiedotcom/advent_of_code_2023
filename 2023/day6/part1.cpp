#include <iostream>
#include <string>

#include "parser.h"
#include "2023_day6.h"

using namespace std;

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
