#include <string>
#include <iostream>


using namespace std;

class interpreter_t {
public:
  int X;
  int cycle;
  int ss;

  interpreter_t()
    : X(1)
    , cycle(0)
    , ss(0)
  {

  }

  void step() {
    cycle++;
    if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220) {
      cout << "cycle=" << cycle << " X=" << X << endl;
      ss += cycle * X;
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
  cout << interpreter.ss << endl;

  return 0;
}
