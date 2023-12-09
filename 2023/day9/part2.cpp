#include <iostream>
#include <string>
#include <stack>
#include <vector>

#include "parser.h"

using namespace std;

inline bool all_zeros(const vector<long int>& v) {
  return all_of(v.begin(), v.end(), [](long int a) { return a == 0; });
}

long int process(const string&  line) {
  parser p(line);
  vector<long int> nums;
  stack<long int> heads;
  size_t tail_offset = 0;
  long int result = 0;
  while (!p.done()) {
    nums.push_back(p.next_int());
  }

  cout << tail_offset << ": ";
  for (size_t i=0; i<nums.size(); i++)
    cout << nums[i] << " ";
  cout << endl;

  while (!all_zeros(nums) && tail_offset < nums.size()) {
    heads.push(nums[0]);

    for (size_t i=0; i<nums.size() - tail_offset - 1; i++) {
      nums[i] = nums[i+1] - nums[i];
    }
    for (size_t i=nums.size() - tail_offset-1; i < nums.size(); i++)
      nums[i] = 0;


    tail_offset++;

    cout << tail_offset << ": ";
    for (size_t i=0; i<nums.size(); i++)
      cout << nums[i] << " ";
    cout << endl;
  }
  cout << endl;

  result = 0;
  while (heads.size() > 0) {
    cout << result << " " << heads.top() << endl;
    result = heads.top() - result;
    heads.pop();
  }

  return result;
}

int main() {
  string line;
  long int sum = 0;
  while (getline(cin, line)) {
    sum+= process(line);
  }
  cout << sum << endl;

  return 0;
}
