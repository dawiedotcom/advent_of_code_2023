#include <iostream>
#include <string>

#include "storm.h"

int main() {
  std::string line;
  storm_t storm;
  while (std::getline(std::cin, line)) {
    storm.input(line);
  }
  auto [pos, vel] = storm.get_best_pv();
  std::cout << pos << std::endl;
  std::cout << (pos.x + pos.y + pos.z)  << std::endl;
  return 0;
}
