#include <iostream>
#include <string>

#include <parser.h>

using namespace std;
using number_t = long long;

int main() {

  string input;
  number_t result = 0;
  while (getline(cin, input)) {
    auto it_first_digit = max_element(input.begin(), input.end()-1);
    auto it_second_digit = max_element(next(it_first_digit), input.end());
    //cout << (*it_first_digit) << (*it_second_digit) << '\n';
    result += ((*it_first_digit) - '0') * 10;
    result += ((*it_second_digit) - '0');
  }
  cout << result << endl;

  return 0;
}
