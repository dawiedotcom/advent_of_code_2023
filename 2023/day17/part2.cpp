#include <iostream>
#include <string>

using namespace std;

#include "city.h"

int main() {
  string line;
  city_t city;
  while(getline(cin, line)) {
    city.input(line);
  }
  cout << city.blocks << endl;
  size_t result = city.get_shortest_path([](const node_t& prev, const node_t& n){
    return (n.n_steps < 10) && (prev.dir == n.dir || prev.n_steps >= 3 || prev.dir == ' ');
  });
  cout << result << endl;
  return 0;
}
