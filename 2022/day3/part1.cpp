#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int priority(const char c) {
  const int p = (int) (c - 'a') + 1;
  //cout << c << " " << p << endl;
  if (p >= 1 && p <= 26)
    return p;
  return (int) (c - 'A') + 27;
}

int get_common_priorities(string& s1, string& s2) {
  auto p1 = s1.begin();
  auto p2 = s2.begin();
  int result=0;

  while (p1 < s1.end() && p2 < s2.end()) {
    //cout << *p1 << " (" << priority(*p1) << ") " << *p2 << " (" << priority(*p2) << ")" << endl;
    if (*p1 == *p2) {
      cout << priority(*p1) << " (" << *p1 << ") "; // << endl;
      result += priority(*p1);
      char c = *p1;
      while (*p1 == c)
        p1++;
      while (*p2 == c)
        p2++;
      continue;
    }
    if (priority(*p1) < priority(*p2))
      p1 ++;
    else
      p2 ++;
  }
  return result;
}

int main() {

  string line, compartment1, compartment2;

  size_t n_items;
  int sum = 0;
  while (getline(cin, line)) {
    n_items = line.size();
    compartment1 = line.substr(0, n_items/2);
    compartment2 = line.substr(n_items/2, n_items);

    sort(compartment1.begin(), compartment1.end(), [](char a, char b) {return priority(a) < priority(b);});
    sort(compartment2.begin(), compartment2.end(), [](char a, char b) {return priority(a) < priority(b);});
    cout << line << " = " << compartment1 << " + " << compartment2 << ": ";//endl;
    sum += get_common_priorities(compartment1, compartment2);
    cout << " [sum = " << sum << "]" << endl;
    //break;
  }
  cout << "Sum of priorities of common items is: " << sum << endl;

  return 0;
}
