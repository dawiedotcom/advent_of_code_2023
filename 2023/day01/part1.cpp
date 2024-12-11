#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int char_to_int(char c) {
  return (int) (c - '0');
}
int main() {
  string line;
  int sum=0;
  string digits = "0123456789";

  while(getline(cin, line)) {
    char first_digit = *find_first_of(line.begin(), line.end(), digits.begin(), digits.end());
    char last_digit = *find_first_of(line.rbegin(), line.rend(), digits.begin(), digits.end());
    sum += 10 * char_to_int(first_digit) + char_to_int(last_digit);
    cout << first_digit << last_digit << endl;
  }
  cout << sum << endl;

  return 0;
}
