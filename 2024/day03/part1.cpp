#include <iostream>
#include <string>

#include "parser.h"
#include "lib.h"

using namespace std;

int main() {

  unsigned long sum = 0;
  string line;
  while (getline(cin, line)) {
    parser line_parser(line);
    while (!line_parser.done()) {
      string mul_clause = line_parser.with("mul\\(\\d{1,3},\\d{1,3}\\)");
      SHOW(mul_clause);
      if (mul_clause.size() == 0)
        break;
      parser digit_parser(mul_clause);
      int a = digit_parser.next_int();
      int b = digit_parser.next_int();
      sum += a * b;
    }
  }
  cout << sum << endl;
  return 0;
}
