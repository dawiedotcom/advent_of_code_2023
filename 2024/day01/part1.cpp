#include <iostream>
#include <string>
#include <vector>
#include <ranges>

using namespace std;

int main() {

  vector<int> left, right;
  int a, b;
  while (cin >> a >> b) {
    left.push_back(a);
    right.push_back(b);
  }
  sort(left.begin(), left.end());
  sort(right.begin(), right.end());

  int sum =0;
  for (auto [a, b] : views::zip(left, right)) {
    sum +=abs(a - b);
  }
  cout << sum << endl;


  return 0;
}
