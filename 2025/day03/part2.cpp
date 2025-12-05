#include <iostream>
#include <string>
#include <cmath>

using namespace std;
using number_t = long long;

int main() {

  string input;
  number_t result = 0;
  while (getline(cin, input)) {

    auto it_digit = input.begin();
    for (int i=11; i>=0; i--) {
      it_digit = max_element(it_digit, input.end()-i);
      //cout << (*it_digit);// << '[' << i << ']';
      result += ((*it_digit) - '0') * pow(10, i);
      it_digit ++;
    }
    //cout << " -> " << result << '\n';
  }
  cout << result << endl;

  return 0;
}
