#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <numeric>

#include "lib.h"
#include "parser.h"

using namespace std;
using value_t = long long;

inline size_t count_char(const string_view& s, char check) {
  return accumulate(s.begin(),
                    s.end(),
                    0,
                    [check](size_t sum, char c) {
                      return (c==check) ? sum + 1 : sum;
                    });
}

inline size_t count_broken(const string_view& s) {
  return count_char(s, '#');
}
inline size_t count_working(const string_view& s) {
  return count_char(s, '.');
}
inline size_t count_unknown(const string_view& s) {
  return count_char(s, '?');
}

size_t can_place(const string_view& line, size_t desc) {
  return count_working(line) == 0 ? 1 : 0;
}

//size_t count(const string& line, const vector<size_t>& desc) {
string make_key(const string_view& rows, const vector<size_t>::iterator begin, const vector<size_t>::iterator end, size_t sum_left, size_t n_broken_hist) {
  std::stringstream ss;
  ss
    << rows << "_"
    << *begin << "_"
    << sum_left << "_"
    << n_broken_hist;
  return ss.str();
}

unordered_map<string, size_t> count_mem;

size_t count(const string_view& rows, const vector<size_t>::iterator begin, const vector<size_t>::iterator end, size_t sum_left = 0, size_t n_broken_hist = 0) {
  string key = make_key(rows, begin, end, sum_left, n_broken_hist);
  if (count_mem.contains(key))
    return count_mem[key];

  if (begin == end) return 1;
  size_t num_descriptors = end - begin;
  size_t n_broken = count_broken(rows);
  size_t sum_right = accumulate(begin+1, end, 0);


  size_t result = 0;
  size_t space_for_right = (sum_right + (num_descriptors-1));
  size_t first_len = (*begin);// + 1;
  for (size_t offset=0; offset <= rows.size() - space_for_right - first_len; offset++) {

    size_t pivot = first_len+offset;
    string_view first = rows.substr(offset, first_len);
    string_view rest = rows.substr(pivot, rows.size() - pivot);
    size_t n_broken_rest = count_broken(rest);
    size_t n_broken_prev = n_broken_hist + count_broken(rows.substr(0, offset));

    if (n_broken_rest + n_broken_prev > sum_right + sum_left)
      continue;
    if (rest[0] == '#')
      continue;
    if (offset > 0 && rows[offset-1] == '#')
      continue;

    size_t c1 = can_place(first, *begin);
    if (rest.size() > 0)
      result += c1*count(rest.substr(1), begin+1, end, sum_left + (*begin), n_broken_prev + (*begin));
    else
      result += c1;
  }

  count_mem[key] = result;
  return result;
}

size_t process(const string& line) {
  parser parse(line);
  vector<size_t> descriptor, _d;
  string r, _rows = parse.with(R"([\?#\.]+)");
 
  while (!parse.done()) _d.push_back(parse.next_int());

  const size_t n_folds = 5;
  for (size_t i=0; i<n_folds; i++) {
    r += _rows;
    if (i < n_folds-1)
      r += "?";
    for (const auto& d : _d)
      descriptor.push_back(d);
  }

  string_view rows(r.c_str());
  return count(rows, descriptor.begin(), descriptor.end()); //result;
}

int main() {
  string line;
  value_t sum=0;
  count_mem.reserve(1000);

  while(getline(cin, line)) {
    if (line[0] == '-') continue;
    count_mem.clear();
    sum += process(line);
  }
  cout << "cache size: " << count_mem.size() << endl;
  cout << sum << endl;

  return 0;
}
