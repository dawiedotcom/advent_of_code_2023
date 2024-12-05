#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>

#include "lib.h"
#include "text_image.h"

using namespace std;

void get_input(text_image<char>& ti) {
  string line;
  while (getline(cin, line)) {
    for (auto c : line) {
      ti.push_back(c);
    }
    ti.start_new_row();
  }
  //SHOW(ti);
}

struct word_t {
  static constexpr array<string, 8> directions = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};

  //int start_x, start_y;
  int end_x, end_y;
  int len;
  const string direction;
  const string w;

};

bool operator==(const word_t& rhs, const word_t& lhs) noexcept {
  return (rhs.end_x == lhs.end_x) && (rhs.end_y == lhs.end_y) && (rhs.len ==  lhs.len) && (rhs.direction == lhs.direction);
}

template<>
struct std::hash<word_t> {
  std::size_t operator()(const word_t& word) const noexcept {
    return word.end_x * word.len * 100 + word.end_y + hash<string>{}(word.direction); 
  }
};

ostream& operator<<(ostream& out, const word_t& word) {
  return out << word.w;
}

int count_words(text_image<char>& input, unordered_set<word_t>& words, int len=1) {
  static const string xmas = "XMAS";
  //SHOW(words.size());
  //for (const auto word : words)
  //  SHOW(word);
  if (len == 5) 
    return words.size();
  if (len == 1) {
    for (size_t i=0; i<input.X(); i++) {
      for (size_t j=0; j<input.X(); j++) {
        if (input[i, j] == 'X') {
          for (const string dir : word_t::directions)
            words.emplace(i, j, 1, dir, "X");
        }
      }
    }
    return count_words(input, words, len=2);
  }

  unordered_set<word_t> new_words;
  unordered_map<string, char*> neighbors;
  
  for (const word_t word : words) {

    input.get_neighbors(word.end_x, word.end_y, neighbors);
    for (const auto [dir, neighbor] : neighbors) {
      if (dir == word.direction && *neighbor == xmas[len-1]) {
        int new_end_x = word.end_x, new_end_y = word.end_y;
        if (dir == "N") { 
          new_end_y --;
        }
        else if (dir == "S") {
          new_end_y ++;
        }
        else if (dir == "W") {
          new_end_x --;
        }
        else if (dir == "E") {
          new_end_x ++;
        }
        else if (dir == "NE") {
          new_end_x ++;
          new_end_y --;
        }
        else if (dir == "NW") {
          new_end_x --;
          new_end_y --;
        }
        else if (dir == "SW") {
          new_end_x --;
          new_end_y ++;
        }
        else if (dir == "SE") {
          new_end_x ++;
          new_end_y ++;
        }
        new_words.emplace(new_end_x, new_end_y, len, dir, word.w + xmas[len-1]);
      }
    }
  }

  return count_words(input, new_words, len=len+1);
}

int main() {
  text_image<char> input;
  unordered_set<word_t> words;
  get_input(input);

  cout << count_words(input, words) << "\n";
  return 0;
}
