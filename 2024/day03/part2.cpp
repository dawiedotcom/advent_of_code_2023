#include <iostream>
#include <string>

#include "parser.h"
#include "lib.h"

using namespace std;

int main() {

  const string mul_regex = "^mul\\(\\d{1,3},\\d{1,3}\\)";

  bool do__ = true;
  unsigned long sum = 0;
  string line;
  string program = "";
  while (getline(cin, line)) {
    program += line;
  }
  parser line_parser(program);
  while (!line_parser.done()) {

    //SHOW(line_parser);
    if (line_parser.match("^don't\\(\\)")) {
      line_parser.to_token("do()");
      continue;
    }
    if (line_parser.match(mul_regex)) {

      string mul_clause = line_parser.with(mul_regex);
      SHOW(mul_clause);
      if (mul_clause.size() == 0)
        break;
      parser digit_parser(mul_clause);
      int a = digit_parser.next_int();
      int b = digit_parser.next_int();
      sum += a * b;
      continue;
    }

    line_parser.step(1);
  }

  cout << sum << endl;
  return 0;
}
