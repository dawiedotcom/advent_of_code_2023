#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <ostream>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "parser.h"
#include "show.h"

using namespace std;

//using stone_t = unsigned long long;
struct stone_t {
  long long num;
  size_t blinks_left;
};

using stones_t = list<stone_t>;
using seen_t = map<tuple<long long, size_t>, size_t>;

void get_input(stones_t& stones, size_t n_blinks) {
  string line;
  getline(cin, line);
  parser p(line);
  while (!p.done()) {
    stones.push_back({p.next_int(), n_blinks});
  }
}

ostream& operator<<(ostream& out, const stone_t& stone) {
  return out << stone.num << "[" << stone.blinks_left << "]";
}

template <typename T>
ostream& operator<<(ostream& out, const list<T>& lst) {
  for (const auto& item : lst) {
    cout << item << " ";
  }
  return out;
}

// This version 'blinks' one stone depth first, which allows for memoizing the
// intermediate results. A stack keeps track of 'historical' stones.
size_t count_stones(stones_t& stones_) {
  seen_t seen;
  vector<stone_t> history;
  stones_t stones(stones_);

  auto it = stones.begin();
  while (it != stones.end()) {

    while (it->blinks_left > 0) {
      //cout << "stones: " << stones << endl;
      //cout << "at: " << *it << endl;
      //cout << "history: " << history << endl << endl;
      if (seen.contains({it->num, it->blinks_left})) {
        for (auto& stone : history) {
          seen[{stone.num, stone.blinks_left}] += seen[{it->num, it->blinks_left}];
        }

        goto next_stone;
      }
      
      history.push_back(*it);

      it->blinks_left --;

      //long long new_num;
      int n_digits = (int) log10(it->num) + 1;

      // 0 becomes 1
      if (it->num == 0) {
        it->num = 1;
        continue;
      }

      // split stones with even digits
      if (n_digits % 2 == 0) {
        long long number = it->num;
        long long divisor = pow(10, n_digits/2);
        it->num = number / divisor;
        stones.insert(next(it, 1), {number % divisor, it->blinks_left}); 
        continue;
      }

      // Multiply by 2024
      it->num = it->num * 2024;
    }

    for (auto& stone : history) {
      seen[{stone.num, stone.blinks_left}] += 1;
    }

next_stone:
    it = next(it, 1);
    while (history.back().blinks_left <= it->blinks_left)
      history.pop_back();
  }

  //cout << stones << endl;
  //for (auto& [stone, count] : seen) {
  //  auto& [num, blinks] = stone;
  //  cout << num << "[" << blinks << "] = " << count << endl;
  //}

  size_t result = 0;
  for (auto& stone : stones_) {
    result += seen[{stone.num, stone.blinks_left}];
  }
  
  return result;
}

int main() {
  constexpr static size_t n_blinks = 75;

  stones_t stones;

  get_input(stones, n_blinks);

  cout << stones << endl;
  cout << count_stones(stones) << endl;

  return 0;
}
// answer for example input: 65601038650482
