#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <unordered_map>
#include <bitset>
#include <cassert>
#include <numeric>

#include "lib.h"
#include "parser.h"

using namespace std;
using value_t = long long;

size_t count(const string& line, size_t desc) {
  SHOW(line, desc);
  return 0;
}
//size_t count(const string& line, const vector<size_t>& desc) {
size_t count(const string& rows, const vector<size_t>::iterator begin, const vector<size_t>::iterator end) {
  cout << rows << " ";
  if (begin == end) return 0;
  auto it=begin;
  for (; it!=end; it++)
    cout << *it << " ";
  cout << endl;

  size_t q_pos = rows.find("?");
  SHOW(rows, q_pos);
  string first = rows.substr(0, q_pos+(*begin)+1);
  string rest = rows.substr(q_pos+(*begin)+1, rows.size() - (q_pos+(*begin)+1));
  SHOW(first, rest);

  size_t result = 0;
  result += count(first, *begin)*count(rest, begin+1, end);



  //if (desc.size() == 0)
  //  return 0;
  return 0;
}

size_t process(const string& line) {
  SHOW(line);
  parser parse(line);
  vector<size_t> descriptor;
  //list<string> groups;
  string rows = parse.with(R"([\?#\.]+)");

  //while (parse.top() == '.') parse.step(1);
  //while (!parse.done() && !parse.top() != ' ') {
  //  groups.push_back(parse.with(R"([\?#]+)"));
  //  while (parse.top() == '.') parse.step(1);
  //  if (parse.top() == ' ') break; // ????
  //}

  while (!parse.done()) descriptor.push_back(parse.next_int());

  size_t result = 0;


  return count(rows, descriptor.begin(), descriptor.end()); //result;
}

// 7638 -- too low
int main() {
  string line;
  value_t sum=0;

  while(getline(cin, line)) {
    sum += process(line);
    //break;
    cout << endl;
  }
  cout << sum << endl;

  return 0;
}
