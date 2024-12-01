#include <iostream>
#include <string>
#include <ranges>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {

  vector<int> left;
  unordered_map<int, int> right_counts;
  int a, b;
  while (cin >> a >> b) {
    left.push_back(a);
    if (right_counts.count(b) == 0)
      right_counts[b] = 1;
    else
      right_counts[b] ++;
  }

  int sum =0;
  for (auto number : left) {
    if (right_counts.count(number) > 0) {
      sum += number * right_counts[number];
    }
  }
  cout << sum << endl;


  return 0;
}
