#include <string>
#include <iostream>


using namespace std;

class interpreter_t {
public:
  int X;
  int cycle;
  int ss;
  int h;

  interpreter_t()
    : X(1)
    , cycle(0)
    , ss(0)
    , h(0)
  {

  }

  void step() {
    cycle++;

    if (X-1 <= h && h <=X+1)
      cout << "#";
    else
      cout << ".";

    h++;

    if (h==40) {
      cout << endl;
      h = 0;
    }

  }

  void execute(string& inst) {
    if (inst.contains("noop")) {
      step();
      return;
    }

    if (inst.contains("addx")) {
      int sp = inst.find(" ");
      int v = stoi(inst.substr(sp+1));
      step();
      step();
      X += v;
      return;
    }
  }
};

int main() {
  string line;

  interpreter_t interpreter;
  while (getline(cin, line)) {
    interpreter.execute(line);
  }

  return 0;
}
