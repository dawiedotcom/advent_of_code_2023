#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <bitset>
#include <cassert>
#include <numeric>

#include "lib.h"
#include "parser.h"

using namespace std;
using value_t = long long;

bool is_valid(const string& row, const vector<size_t>& descriptor) {
  size_t group = 0, pos, run=0;
  auto it = descriptor.begin();

  pos = row.find('#');
  while (pos <= row.rfind('#')) { //row.size()){
    //for (pos=row.find('#'); pos<row.size(); pos++){
    //SHOW(pos, row[pos], *it, run);
    if (row[pos]=='#') {
      run++;
      pos++;
      if (run > *it)
        return false;
    }
    else {
      if (*it != run)
        return false;
      run = 0;
      it++;
      if (it == descriptor.end())
        return false;
      pos = row.find('#', pos);
      if (pos == string::npos)
        pos = row.size();
    }
  }

  //SHOW(row);
  return (it == descriptor.end()-1) && (*it == run);
}

size_t count_possible_rows(string& row, const vector<size_t>& descriptor) {
  size_t result = 0;
  size_t count = 1;
  size_t d_total = accumulate(descriptor.begin(), descriptor.end(), 0, [](size_t a, size_t b) {return a + b;});
  SHOW(row, row.size(), d_total);
  const size_t MAX_BITCOUNT = 20;

  bitset<MAX_BITCOUNT> mask{0}, test{0};

  size_t bitcount=0, n_checks=0;
  vector<size_t> positions;
  for (size_t i=0; i<row.size(); i++) {
    if (row[i]=='#')
      d_total--;
    if (row[i]=='?') {
      bitcount ++;
      count *= 2;
      positions.push_back(i);
    }
    mask[i] = (row[i] == '?' || row[i] == '#');
    test[i] = (row[i] == '#');
  }
  SHOW(bitcount);
  assert(bitcount <= MAX_BITCOUNT);
  for (size_t i=0; i<count; i++) {
    bitset<MAX_BITCOUNT> b{i};
    if (b.count() != d_total) continue;
    n_checks++;
    for (size_t j=0; j<bitcount; j++) {
      row[positions[j]] = (b[j]) ? '#' : '.';
      test[positions[j]] = b[j];
    }
    if (is_valid(row, descriptor)) {
    //if ((test | mask) == mask) {
      SHOW(row);
      result ++;
      //cout << "valid\n";
    }
  }
  //SHOW(mask);
  SHOW(result, n_checks);
  cout << endl;

  return result;
}

/* Count the number of grouping separated by dots */
size_t group_count(const string& s) {
  size_t result = 0;
  bool in_group = false;
  for (const auto c : s) {
    if (in_group ^ (c == '#' || c=='?')) {
      in_group = (c == '#' || c=='?');
      result += in_group ? 1 : 0;
    }
  }

  return result;
}

void insert_dots(string& s, const vector<size_t>& descriptor) {
  size_t gc = group_count(s);
  while (gc < descriptor.size()) {
    //size_t next_g
    parser parse(s);
    string next_group = parse.with(R"([\?\#]+)");
    SHOW(next_group);
    break;

    gc = group_count(s);
  }
}

value_t process(const string& line) {
  size_t space_pos = line.find(' ');
  string row = line.substr(0,space_pos);
  SHOW(line, group_count(row));
  string d_line = line.substr(space_pos+1);
  //SHOW(d_line);
  parser parse(d_line);
  vector<size_t> descriptor;
  while (!parse.done()) descriptor.push_back(parse.next_int());

  insert_dots(row, descriptor);

  return count_possible_rows(row, descriptor);
}

// 7638 -- too low
int main() {
  string line;
  value_t sum=0;

  while(getline(cin, line)) {
    sum += process(line);
    //break;
  }
  cout << sum << endl;

  return 0;
}
