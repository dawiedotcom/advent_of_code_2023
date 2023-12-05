#include <iostream>
#include <string>

using namespace std;

int char_to_int(char c) {
  return (int) (c - '0');
}
int main() {
  string line;
  char first_digit, last_digit;
  int sum=0;

  while(getline(cin, line)) {
    first_digit = '.';
    last_digit = '0';
    for (auto& c : line) {
      if (isdigit(c)) {
        last_digit = c;
        if (!isdigit(first_digit))
          first_digit = c;
      }
    }
    sum += 10 * char_to_int(first_digit) + char_to_int(last_digit);
    cout << first_digit << last_digit << endl;
  }
  cout << sum << endl;

  return 0;
}
