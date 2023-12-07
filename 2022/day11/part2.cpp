#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>

#include "parser.h"

using namespace std;

struct arg_t {
  bool is_old;
  int value;
  inline int get_value(const int old) const {
    return is_old ? old : value;
  }
};

class operation_t {
public:
  string op;
  arg_t left;
  arg_t right;

  unsigned long long operator()(unsigned long long old) {
    unsigned long long result=0;
    if (op == "+") result = left.get_value(old) + right.get_value(old);
    if (op == "*") result = left.get_value(old) * right.get_value(old);

    //cout << left.get_value(old) << " " << op << " " << right.get_value(old) << " = " << result;
    return result;
  }

  void parse(const string& line) {
    parser p(line);
    p.to_token("= ");
    if (p.top() == 'o') {
      left.is_old = true;
      p.to_token("old ");
    }
    else {
      left.is_old = false;
      left.value = p.next_int();
    }

    op = p.to_token(string(1, p.top()));
    p.step(1);

    if (p.top() == 'o') {
      right.is_old = true;
      //p.to_token("old ");
    }
    else {
      right.is_old = false;
      right.value = p.next_int();
    }
  }
};

class test_t {
public:
  int div;

  constexpr bool operator()(unsigned long long value) const {
    return value % div == 0;
  }

  void parse(const string& line) {
    parser p(line);
    div = p.next_int();
  }
};

class monkey_t {
public:
  int inspections;
  int modulo;
  queue<unsigned long long> items;
  operation_t operation;
  test_t test;
  int true_target, false_target;

  monkey_t()
    : inspections(0)
    , modulo(0)
  {}

  void turn(vector<monkey_t>& monkeys) {
    while (!items.empty()) {
      unsigned long long wl = items.front(); items.pop();
      wl = operation(wl);

      if (test(wl)) {
        //cout << " -> " << true_target << endl;
        monkeys[true_target].items.push(wl);
      }
      else {
        //cout << " -> " << false_target << endl;
        monkeys[false_target].items.push(wl);
      }

      inspections ++;
    }
  }
};

class monkey_busyness_t {
public:
  size_t i_monkey;
  int modulo ;
  vector<monkey_t> monkeys;

  monkey_busyness_t()
    : i_monkey(0)
    , modulo(1)
  {}

  void parse(const string& line) {
    if (line.contains("Monkey")) {
      monkeys.emplace_back();
      return;
    }

    if (line.size() == 0) {
      i_monkey++;
      return;
    }

    if (line.contains("Operation:")) {
      monkeys[i_monkey].operation.parse(line);
      return;
    }

    if (line.contains("Test")) {
      monkeys[i_monkey].test.parse(line);
      modulo *= monkeys[i_monkey].test.div;
      //cout << "div: " << monkeys[i_monkey].test.div << endl;
      return;
    }

    parser p(line);
    if (line.contains("Starting items:")) {
      while (!p.done())
        monkeys[i_monkey].items.push(p.next_int());
      return;
    }

    if (line.contains("true")) {
      monkeys[i_monkey].true_target = p.next_int();
      return;
    }

    if (line.contains("false")) {
      monkeys[i_monkey].false_target = p.next_int();
      return;
    }
  }

  void round() {
    for (auto& m : monkeys) {
      m.turn(monkeys);
      //cout << endl;
    }
  }

  void run() {
    cout << "modulo: " << modulo << endl;
    for (auto& m : monkeys)
      m.modulo = modulo;

    for (size_t i=0; i<10000; i++) {
      cout << "Round: " << (i+1) << endl;
      round();

      if ((i+1) == 1 || (i+1) == 20 || (i+1) % 1000 == 0) {
        cout << " After round: " << (i+1) << endl;
        for (size_t i=0; i<monkeys.size(); i++) {
          cout << "Monkey " << i << " made " << monkeys[i].inspections << " inspections" << endl;
        }
      }
    }

    sort(monkeys.begin(), monkeys.end(), [](monkey_t& a, monkey_t&b) {return a.inspections > b.inspections;});
    for (auto& m : monkeys)
      cout << m.inspections << endl;
    cout << (monkeys[0].inspections * monkeys[1].inspections) << endl;
  }

};

int main() {
  string line;

  monkey_busyness_t mb;
  while(getline(cin, line)) {
    mb.parse(line);
  }

  mb.run();
  return 0;
}
