#include <iostream>
#include <string>
#include <limits.h>
#include <cmath>

#include "parser.h"
#include "show.h"

using namespace std;

struct point_t {
  long double x, y;
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
  constexpr static double OFFSET = 10000000000000;
  parse_point(machine.a);
  parse_point(machine.b);
  parse_point(machine.prize);
  machine.prize.x += OFFSET;
  machine.prize.y += OFFSET;
  string line; // check for a new line after this machine
  return getline(cin, line) ? true : false;
}

unsigned long long get_min_tokens(machine_t& machine) {
  constexpr static double TOL = 1e-3;
  if ((int)machine.prize.x % (int)machine.b.x == 0 && (int)machine.prize.y % (int)machine.b.y == 0) {
    cout << "Only B\n";
    exit(1);
  }
  if ((int)machine.prize.x % (int)machine.a.x == 0 && (int)machine.prize.y % (int)machine.a.y == 0) {
    cout << "Only A\n";
    //SHOW(machine.a.x, machine.a.y, machine.b.x, machine.b.y, machine.prize.x, machine.prize.y);
    exit(1);
  }
  // The coordinates of the prize is given by:
  //
  // machine.prize.x = A_press * machine.a.x + B_press * machine.b.x
  // machine.prize.y = A_press * machine.a.y + B_press * machine.b.y

  // Solve for A_press using the x equation:
  //
  // machine.prize.x = A_press * machine.a.x + B_press * machine.b.x
  // A_press * machine.a.x = machine.prize.x - B_press * machine.b.x
  // A_press = (machine.prize.x - B_press * machine.b.x) / machine.a.x 
  //
  // The same is true for the y equation:
  //
  // A_press = (machine.prize.y - B_press * machine.b.y) / machine.a.y 
  //
  // Substitute A_press into the y equation and solve for B_press:
  //
  //  (machine.prize.y - B_press * machine.b.y) / machine.a.y = (machine.prize.x - B_press * machine.b.x) / machine.a.x 
  //  machine.prize.y / machine.a.y - B_press * machine.b.y / machine.a.y = machine.prize.x / machine.a.x - B_press * machine.b.x / machine.a.x 
  //  B_press * machine.b.x / machine.a.x - B_press * machine.b.y / machine.a.y = machine.prize.x / machine.a.x -  machine.prize.y / machine.a.y 
  //  B_press * (machine.b.x / machine.a.x - machine.b.y / machine.a.y) = machine.prize.x / machine.a.x -  machine.prize.y / machine.a.y 
  //  B_press  = (machine.prize.x / machine.a.x -  machine.prize.y / machine.a.y) / (machine.b.x / machine.a.x - machine.b.y / machine.a.y)

  long double B_press  = (-machine.prize.x / machine.a.x +  machine.prize.y / machine.a.y) * ( (machine.a.x * machine.a.y) / ( - machine.b.x * machine.a.y + machine.b.y * machine.a.x));
  long double A_press = (machine.prize.y - B_press * machine.b.y) / machine.a.y ;
  //SHOW(A_press, B_press);

  unsigned long long cost = round(3*A_press + B_press);

  if (A_press < 0 || B_press < 0)
    cost = 0;

  if ((abs(A_press - round(A_press)) > TOL) || (abs(B_press - round(B_press)) > TOL))
    cost = 0;

  //if (cost != 0) {
  //  SHOW(machine.a.x, machine.a.y, machine.b.x, machine.b.y, machine.prize.x, machine.prize.y);
  //  SHOW(A_press, B_press, cost);
  //  cout << endl;
  //}
  return cost;
}

unsigned long long get_min_tokens() {
  machine_t machine;
  unsigned long long result = 0;
  bool has_more_input = true;
  //int machine_num=0;
  while (has_more_input) {
    has_more_input = parse_machine(machine);
    unsigned long long machine_cost = get_min_tokens(machine);
    result += machine_cost;
    //SHOW(++machine_num, machine_cost);
  } 
  return result;
}

int main() {
  cout << get_min_tokens() << endl;
  return 0;
}
