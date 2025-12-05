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

    if (steps > 100) {
      num_zeros += steps / 100;
      steps = steps % 100;
    }


    if (line[0] == 'R') {
      if ((dial > 0) && (dial + steps) >= 100)
        num_zeros ++;
      dial += steps;
    }
    else {
      if ((dial > 0) && (dial - steps) <= 0)
        num_zeros ++;
      dial -= steps;
    }

    dial = dial % 100;
    if (dial < 0) dial+=100;
    //cout << line[0] << " " << steps << " " << (steps/100) << " " << dial << " " << num_zeros << endl;
  }
  cout << num_zeros << endl;

  return 0;
}
