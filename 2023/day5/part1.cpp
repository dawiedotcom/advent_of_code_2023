#include <iostream>
#include <string>
#include <array>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "parser.h"

using namespace std;

#define NUM_MAPS 7
#define MAX_MAP 100

class lin_map {
private:
  unsigned long long from, to, length;
public:
  lin_map(unsigned long long f, unsigned long long t, unsigned long long l)
    : from(f)
    , to(t)
    , length(l)
  {

  }

  bool contains(unsigned long long x) {
    return (x < from + length) && (x >= from);
  }

  unsigned long long operator()(unsigned long long x) {
    return (x-from) + to;
  }
};

class lin_maps {
private:
  vector<lin_map> maps;
public:
  lin_maps() {}

  void add_map(unsigned long long f, unsigned long long t, unsigned long long l) {
    maps.push_back(lin_map(f, t, l));
  }

  bool contains(unsigned long long x) {
    for (auto& map: maps)
      if (map.contains(x))
        return true;
    return false;
  }

  unsigned long long operator()(unsigned long long x) {
    for (auto& map: maps)
      if (map.contains(x))
        return map(x);

    return x;
  }
};

class seed_locations {
private:
  size_t current_input_map;
  //array<unordered_map<size_t, size_t>, NUM_MAPS> maps;
  array<lin_maps, NUM_MAPS> maps;
  vector<unsigned long long> seeds;

public:
  seed_locations()
    : current_input_map(NUM_MAPS)
  {
    //for (size_t i=0; i<NUM_MAPS; i++)
    //  for (size_t j=0; j<MAX_MAP; j++)
    //    maps[i][j] = j;

  }

  void parse_line(string& line) {
    if (line.size() == 0) return;

    if (line.contains("map:")) {
      current_input_map = current_input_map == NUM_MAPS ? 0 : current_input_map+1 ;
      //cout << "current map: " << current_input_map << endl;
      return;
    }

    parser p(line);
    if (line.contains("seeds:")) {
      p.to_token("seeds:");
      while (!p.done())
        seeds.push_back(p.next_int());
      return;
    }

    unsigned long long to = p.next_int();
    unsigned long long from = p.next_int();
    unsigned long long len = p.next_int();

    maps[current_input_map].add_map(from, to, len);
    //for (size_t i=0; i<len; i++) {
    //  maps[current_input_map][from+i] = to+i;
    //}

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
    size_t min=0;

    for (auto& seed : seeds) {
      for (auto& map : maps) {
        cout << seed << " ";
        if (map.contains(seed))
          seed = map(seed);
      }
      cout << seed << endl;;
    }
    //cout << endl;

    return *min_element(seeds.begin(), seeds.end(), [](size_t a, size_t b) {return a < b;});
  }
};

int main() {
  string line;

  seed_locations locations;
  while (getline(cin, line)) {
    locations.parse_line(line);
  }
  cout << "Got input" << endl;
  //locations.show();
  cout << locations.lowest_location_nr() << endl;

  return 0;
}
