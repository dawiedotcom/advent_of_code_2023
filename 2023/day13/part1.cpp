#include <iostream>
#include <string>
#include <vector>

#include "lib.h"

using namespace std;
using value_t = long long;

struct pattern {
  size_t result;
  size_t mult;
  vector<string> lines;

  pattern()
    : result(0)
    , mult(1)
  {}

  void input(const string& line) {
    if (line.size() == 0) {
      check_reflections();
      //SHOW(result);
      lines.clear();
      //cout << endl;
      return;
    }

    //SHOW(line);
    lines.push_back(line);
  }

  bool check_h_reflections(const size_t start_i) {
    if (start_i==0 || start_i>=lines.size()-2)
      return true;
    size_t i=start_i, j=start_i+1;
    while (i > 0 && j < lines.size()-1) {
      //SHOW(i-1, j+1);
      //SHOW(lines[i-1]);
      //SHOW(lines[j+1]);
      if (lines[--i] != lines[++j])
        return false;
    }
    return true;
  }

  bool check_v_reflections(const size_t start_i) {
    if (start_i==0 || start_i>=lines[0].size()-2)
      return true;
    size_t i=start_i, j=start_i+1;
    while (i > 0 && j < lines[0].size()-1) {
      //SHOW(i-1, j+1);
      //SHOW(get_column(i-1));
      //SHOW(get_column(j+1));
      if (!check_columns(--i, ++j))
        return false;
    }
    return true;
  }

  string get_column(const size_t i) {
    string s = "";
    for (size_t j=0; j < lines.size(); j++)
      s += lines[j][i];
    return s;
  }

  bool check_columns(const size_t i, const size_t j) {
    for (size_t k=0; k<lines.size(); k++) {
      if (lines[k][i] != lines[k][j])
        return false;
    }
    return true;
  }

  void check_reflections() {
    for (size_t i=0; i<lines.size()-1; i++) {
      if (lines[i] == lines[i+1]) {
        if (check_h_reflections(i))
          result += 100 * (i+1);
      }
    }

    for (size_t i=0; i<lines[0].size()-1; i++) {
      if (check_columns(i, i+1)) {
        if (check_v_reflections(i))
          result += (i+1);
      }
    }
  }
};

int main() {
  string line;
  value_t sum=0;
  pattern pat;

  while(getline(cin, line)) {
    pat.input(line);
  }
  pat.check_reflections();
  cout << pat.result << endl;

  return 0;
}
