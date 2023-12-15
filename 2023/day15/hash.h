#pragma once

size_t calc_hash(const string& line) {
  size_t current_value = 0;
  for (const auto c : line) {
    current_value += (size_t) c;
    current_value *= 17;
    current_value = current_value % 256;
  }

  return current_value;
}
