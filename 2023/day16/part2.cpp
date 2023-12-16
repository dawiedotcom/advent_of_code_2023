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
  //contraption.run();
  //cout << contraption.room << endl;
  cout << contraption.find_best_start() << endl;

  return 0;
}
