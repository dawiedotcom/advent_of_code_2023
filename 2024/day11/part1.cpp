#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <ostream>

#include "parser.h"
#include "show.h"

using namespace std;

using number_t = unsigned long long;
using stones_t = list<number_t>;

void get_input(stones_t& stones) {
  string line;
  getline(cin, line);
  parser p(line);
  while (!p.done()) {
    stones.push_back(p.next_int());
  }
}
template <typename T>
ostream& operator<<(ostream& out, const list<T>& lst) {
  for (const auto& item : lst) {
    cout << item << " ";
  }
  return out;
}

void blink(stones_t& stones) {
  auto it = stones.begin();
  while (it != stones.end()) {
    // 0 becomes 1
    if (*it == 0) {
      *it = 1;
      it = next(it, 1);
      continue;
    }

    // split stones with even digits
    int n_digits = (int) log10(*it) + 1;
    if (n_digits % 2 == 0) {
      number_t number = *it;

      number_t divisor = pow(10, n_digits/2);
      *it = number / divisor;
      it = next(it, 1);

      stones.insert(it, number % divisor); 
      //it = next(it, 1);

      continue;
    }

    // Multiply by 2024
    *it *= 2024;
    it = next(it, 1);
  }
  //cout << stones << endl;
}

int main() {
  stones_t stones;
  get_input(stones);

  cout << stones << endl;

  constexpr static size_t n_blinks = 25;
  for (size_t i=0; i<n_blinks; i++) {
    blink(stones);
    SHOW(i, stones.size());
  }

  cout << stones.size() << endl;

  return 0;
}
