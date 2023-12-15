#include <string>
#include <iostream>

#include "parser.h"
#include "lib.h"

using namespace std;

#include "hash.h"

int main() {
  string line;

  size_t sum=0;
  while (getline(cin, line)) {
    parser parse(line);
    while (!parse.done()) {
      sum += calc_hash(parse.with(R"([A-Za-z0-9=-]+)"));
      if (parse.top() == ',')
        parse.to_token(",");
    }
  }
  cout << sum << endl;
  return 0;
}
