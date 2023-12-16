#include <iostream>
#include <string>
#include <stack>

#include "lib.h"
#include "text_image.h"

using namespace std;

#include "contraption.h"

int main() {
  string line;
  contraption_t contraption;
  while (getline(cin, line)) {
    contraption.input(line);
  }
  //contraption.run(0, 0, "E");
  //cout << contraption.room << endl;
  cout << contraption.count_beam(0, 0, "E") << endl;

  return 0;
}
