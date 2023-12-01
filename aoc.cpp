#include <iostream>
#include <string>

using namespace std;

int char_to_int(const char c) {
  return (int) (c - '0');
}

/*
  Returns '1'-'9' if s, at offset pos, starts with "one", "two", etc.
  Return '.' if s, at offset pos, does not have a 'digit' word.
 */
char parse_word(const string& s, const size_t pos) {
  if (s.find("one", pos) == pos) return '1';
  if (s.find("two", pos) == pos) return '2';
  if (s.find("three", pos) == pos) return '3';
  if (s.find("four", pos) == pos) return '4';
  if (s.find("five", pos) == pos) return '5';
  if (s.find("six", pos) == pos) return '6';
  if (s.find("seven", pos) == pos) return '7';
  if (s.find("eight", pos) == pos) return '8';
  if (s.find("nine", pos) == pos) return '9';
  return '.';
}

int main() {
  string line;
  char first_digit, last_digit;
  int sum=0;

  while(getline(cin, line)) {
    first_digit = '.';
    last_digit = '0';

    for (size_t i=0; i<line.size(); i++) {
      char c = line[i];
      if (!isdigit(c))
        c = parse_word(line, i);

      if (isdigit(c)) {
        last_digit = c;
        if (!isdigit(first_digit))
          first_digit = c;
      }
    }
    sum += 10 * char_to_int(first_digit) + char_to_int(last_digit);
    //cout << first_digit << last_digit << endl;
  }
  cout << "The sum is: " << sum << endl;

  return 0;
}
