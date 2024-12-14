#include <iostream>
#include <string>
#include <limits.h>
#include <cmath>

#include "parser.h"
//#include "point.h"
#include "show.h"

using namespace std;

struct point_t {
  double x, y;
};

struct machine_t {
  point_t a;
  point_t b;
  point_t prize;
};

void parse_point(point_t& point) {
  string line;
  getline(cin, line);
  parser p(line);
  point.x = p.next_int();
  point.y = p.next_int();
}

bool parse_machine(machine_t& machine) {
  parse_point(machine.a);
  parse_point(machine.b);
  parse_point(machine.prize);
  string line;
  return getline(cin, line) ? true : false;
}

size_t get_min_tokens_bf(machine_t& machine) {
  constexpr static size_t MAX_PRESS = 100;
  for (size_t A=1; A<=MAX_PRESS; A++) {
  for (size_t B=1; B<=MAX_PRESS; B++) {
    if ((A * machine.a.x + B * machine.b.x == machine.prize.x)
        && (A * machine.a.y + B * machine.b.y == machine.prize.y)) {
      SHOW(A, B);
      return 3 * A + B;
    }
  }
  }
  return 0;
}

size_t get_min_tokens() {
  machine_t machine;
  size_t result = 0;
  bool has_more_input = true;
  while (has_more_input) {
    has_more_input = parse_machine(machine);
    size_t machine_cost = get_min_tokens_bf(machine);
    result += machine_cost;
    //cout << endl;
  } 
  return result;
}

int main() {

  cout << get_min_tokens() << endl;
  return 0;
}
