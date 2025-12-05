#include <iostream>
#include <string>
#include <parser.h>

using namespace std;

int main() {
  string line;
  int dial = 50;
  int num_zeros = 0;

  while (cin >> line) {
    parser p(line);
    int steps = p.next_int();
    cout << line[0] << " " << steps << endl;
    if (line[0] == 'R')
      dial += steps;
    else
      dial -= steps;

    dial = dial % 100;
    if (dial == 0) 
      num_zeros ++;
  }
  cout << num_zeros << endl;

  return 0;
}
