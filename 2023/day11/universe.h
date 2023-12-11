#pragma once

#define ABS(x) ((x) >= 0 ? (x) : -(x))

struct universe;
ostream& operator<<(ostream& out, const universe& u);

struct universe {
  size_t Y;
  size_t exp_dist;
  vector<size_t> x, y;


  universe() : Y(0), exp_dist(1) {}
  universe(size_t ed) : Y(0), exp_dist(ed) {}

  size_t dist(size_t x1, size_t y1, size_t x2, size_t y2) {
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    return ABS(dx) + ABS(dy);
  }

  void parse_input(const string& line) {
    for (size_t i=0; i<line.size(); i++) {
      if (line[i] == '#') {
        x.push_back(i);
        y.push_back(Y);
      }
    }

    Y++;
  }

  void expand(vector<size_t>& v) {
    set<size_t> s(v.begin(), v.end());

    for (size_t i=v.size(); i>0; i--) {
      if (!s.contains(i)) {
        for (size_t j=0; j<v.size(); j++) {
          if (v[j] > i) {
            v[j]+=exp_dist;
          }
        }
      }
    }
  }

  void expand() {
    cout << *this;
    expand(x);
    expand(y);
    cout << *this;
  }

  unsigned long long calc_distances() {
    unsigned long long sum = 0;
    for (size_t i=0; i<x.size()-1; i++) {
      for (size_t j=i+1; j<x.size(); j++) {
        cout << (i+1) << " - " << (j+1) << ": " << dist(x[i], y[i], x[j], y[j]) << endl;
        sum += dist(x[i], y[i], x[j], y[j]);
      }
    }

    return sum;
  }

};

ostream& operator<<(ostream& out, const universe& u) {
  for (size_t i=0; i<u.x.size(); i++) {
    cout << i+1 << ": (" << u.x[i] << ", " << u.y[i] << ")\n";
  }

  return out;
}
