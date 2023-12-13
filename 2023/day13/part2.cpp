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
      cout << endl;
      return;
    }

    //SHOW(line);
    cout << line << endl;
    lines.push_back(line);
  }

  bool check_h_reflections(const size_t start_i) {
    size_t defects = check_rows(start_i, start_i+1);
    if (start_i==0 || start_i>=lines.size()-2)
      return defects==1;
    size_t i=start_i, j=start_i+1;
    while (i > 0 && j < lines.size()-1) {
      defects += check_rows(--i, ++j); // (lines[--i] != lines[++j])
      SHOW(i, j, defects);
      SHOW(lines[i]);
      SHOW(lines[j]);
      if (defects >= 2) return false;
    }
    return defects == 1;
  }

  bool check_v_reflections(const size_t start_i) {
    size_t defects = check_columns(start_i, start_i+1);
    if (start_i==0 || start_i>=lines[0].size()-2)
      return defects == 1;
    size_t i=start_i, j=start_i+1;
    while (i > 0 && j < lines[0].size()-1) {
      defects += check_columns(--i, ++j);
      SHOW(i, j, defects);
      SHOW(get_column(i));
      SHOW(get_column(j));
      if (defects >= 2) return false;
    }
    return defects == 1;
  }

  string get_column(const size_t i) {
    string s = "";
    for (size_t j=0; j < lines.size(); j++)
      s += lines[j][i];
    return s;
  }

  size_t check_columns(const size_t i, const size_t j) {
    size_t result = 0;
    for (size_t k=0; k<lines.size(); k++) {
      if (lines[k][i] != lines[k][j])
        result++;
    }
    return result;
  }

  size_t check_rows(const size_t i, const size_t j) {
    size_t result = 0;
    for (size_t k=0; k<lines[i].size(); k++) {
      if (lines[i][k] != lines[j][k])
        result++;
    }
    return result;
  }


  void check_reflections() {
    for (size_t i=0; i<lines.size()-1; i++) {
      size_t defects = check_rows(i, i+1);
      if (defects == 0 || defects == 1) {
        if (check_h_reflections(i))
          result += 100 * (i+1);
      }
    }

    for (size_t i=0; i<lines[0].size()-1; i++) {
      size_t defects = check_columns(i, i+1);
      if (defects == 0 || defects == 1) {
        if (check_v_reflections(i))
          result += (i+1);
      }
    }
  }
};

// 43624 too high
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
