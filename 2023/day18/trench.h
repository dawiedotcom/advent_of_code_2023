#pragma once

struct trench_t {
  size_t circ;
  vector<pair<long, long>> corners;

trench_t()
  : circ(0)
  {
    corners.emplace_back(0, 0);
  }

  virtual void input(const string& line) = 0;

  long long calc_area() {
    long long result = 0;
    size_t N = corners.size();

    for (size_t i=0; i<N-1; i++) {
      auto [x, y] = corners[i];
      auto [x_next, y_next] = corners[(i+1)];

      result += (x*y_next - x_next*y);
    }
    return abs(result/2) + circ/2 + 1;
  }
};
