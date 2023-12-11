#include <iostream>
#include <string>
#include <array>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

#include "parser.h"
#include "interval.h"
using interval = interval_<long long>;

using namespace std;

#define NUM_MAPS 7
#define MAX_MAP 100

typedef long long ull;

class lin_map {
public:
  long long from, to, length;
  interval domain;
public:
  lin_map(long long f, long long t, long long l)
    : from(f)
    , to(t)
    , length(l)
    , domain(f, f+l)
  {

  }

  bool contains(long long x) {
    return (x < from + length) && (x >= from);
  }

  long long operator()(long long x) {
    return to + (x-from);
  }

  // assums i is in domain
  interval apply_on_interval(interval i) {
    return interval(operator()(i.start), operator()(i.end));
  }

  void apply_on_intervals(const vector<interval>& intervals, vector<interval>& range, vector<interval>& still_to_map) {
    for (auto& i: intervals) {

      if (i.extends_past_left(domain)) {
        interval left = i.left_of(domain);
        still_to_map.push_back(left);
      }
      if (i.intersects(domain)) {
        interval overlap  = i.intersection(domain);
        range.push_back(apply_on_interval(overlap));
      }
      if (i.extends_past_right(domain)) {
        interval right = i.right_of(domain);
        still_to_map.push_back(right);
      }
      cout << "} ";
    }
  }
};

class lin_maps {
private:
  vector<lin_map> maps;
public:
  lin_maps() {}

  void add_map(long long f, long long t, long long l) {
    maps.push_back(lin_map(f, t, l));
  }

  bool contains(long long x) {
    for (auto& map: maps)
      if (map.contains(x))
        return true;
    return false;
  }

  long long operator()(long long x) {
    for (auto& map: maps)
      if (map.contains(x))
        return map(x);

    return x;
  }

  void apply_on_intervals(const vector<interval>& input, vector<interval>& result) {
    vector<interval> b1(input.begin(), input.end()), b2;
    vector<interval> *domain=&b1, *tmp, *rem_domain=&b2;

    for (auto& map : maps) {
      if (domain->empty())
        break;
      rem_domain->clear();
      map.apply_on_intervals(
        *domain,
        result,
        *rem_domain
      );

      tmp = domain;
      domain = rem_domain;
      rem_domain = tmp;
    }

    for (auto& i : *domain)
      result.push_back(i);
    //for (auto& i : *range)
    //  result.push_back(i);
  }
};

class seed_locations {
private:
  size_t current_input_map;
  //array<unordered_map<size_t, size_t>, NUM_MAPS> maps;
  array<lin_maps, NUM_MAPS> maps;
  vector<long long> seeds;

public:
  seed_locations()
    : current_input_map(NUM_MAPS)
  {

  }

  void parse_line(string& line) {
    if (line.size() == 0) return;

    if (line.contains("map:")) {
      current_input_map = current_input_map == NUM_MAPS ? 0 : current_input_map+1 ;
      return;
    }

    parser p(line);
    if (line.contains("seeds:")) {
      p.to_token("seeds:");
      while (!p.done())
        seeds.push_back(p.next_int());
      return;
    }

    long long to = p.next_int();
    long long from = p.next_int();
    long long len = p.next_int();

    maps[current_input_map].add_map(from, to, len);

  }

  void show() {
    for (size_t i=0; i<MAX_MAP; i++) {
      for (auto& map : maps) {
        cout << i << "->" << (map.contains(i) ? map(i) : i) << "\t";
      }
      cout << endl;
    }
  }

  int lowest_location_nr(){
    long long min = LLONG_MAX;

    vector<interval> v1, v2;
    vector<interval> *in = &v1, *out=&v2, *tmp;

    ull total_seeds = 0;
    for (int i=0; i<seeds.size(); i+=2) {
      in->emplace_back(seeds[i], seeds[i] + seeds[i+1]);
      total_seeds += seeds[i+1];
    }
    for (auto &map : maps) {
      out->clear();
      map.apply_on_intervals(*in, *out);

      tmp = in;
      in = out;
      out = tmp;
    }

    ull after_total_seeds = 0;
    for (auto& r : *in) { // !!! output is in in due to the swap at the last iteration of the loop over maps
      after_total_seeds += r.length();
      if (min > r.start)
        min = r.start;
    }
    cout << "total seeds before: " << total_seeds << " after: " << after_total_seeds << endl;
    assert(total_seeds == after_total_seeds);

    return min;
  }
};

int main() {
  string line;

  seed_locations locations;
  while (getline(cin, line)) {
    locations.parse_line(line);
  }
  cout << locations.lowest_location_nr() << endl;

  return 0;
}
// 161956620 too high
//  46294175 just right
//   5553076 too low
