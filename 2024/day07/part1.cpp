#include <iostream>
#include <string>
#include <vector>

#include "lib.h"
#include "parser.h"

using namespace std;


void parse_input(const string line, long long& test_value, vector<int>& nums) {
  parser p(line);
  test_value = p.next_int();

  nums.clear();
  while (!p.done())
    nums.push_back(p.next_int());
}

bool is_true(const long long test_value, const vector<int>& nums) {

  //SHOW(nums.size());
  size_t n_operators = nums.size() - 1;
  size_t operator_combinations = 1 << n_operators;
  
  long long result, ops;
  for (size_t ops_i=0; ops_i<operator_combinations; ops_i++) {
    ops = ops_i;

    result = nums[0];
    for (size_t i=1; i<nums.size(); i++) {
      if (ops % 2 == 0) {
        result *= nums[i];
      }
      else {
        result += nums[i];
      }

      ops >>= 1;
    }
    if (result == test_value)
      return true;

  }

  return false;
}

int main() { 

  string line;
  long long test_value;
  vector<int> nums;
  unsigned long long result = 0;
  while (getline(cin, line)) {
    parse_input(line, test_value, nums);
    if (is_true(test_value, nums)) {
      result += test_value;
    }
  }
  cout << result << endl;
  return 0;
}
