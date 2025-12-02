#include <iostream>
#include <string>
#include <cmath>

#include <parser.h>

using namespace std;

using number_t = unsigned long long;

bool is_valid_id(number_t id) {
  int num_digits = log10(id) + 1.0;
  if (num_digits % 2 == 1)
    return false;

  number_t x = pow(10, num_digits/2);
  number_t first_half = id / x;
  number_t second_half = id % x;

  return first_half == second_half;
}

int main() {

  string input;
  cin >> input;
  parser p(input);

  number_t result = 0;

  while (!p.done()) {
    number_t low = p.next_uint();
    number_t high = p.next_uint();
    for (number_t id = low; id<= high; id++) {
      if (is_valid_id(id))
        result += id;
    }
    //cout << low << " - " << high << "\n: " << result << endl;
  }
  cout << result << endl;

  return 0;
}
