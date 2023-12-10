#include <iostream>
#include <string>
#include <vector>

#include "lib.h"
#include "parser.h"

using namespace std;
using value_t = long long;

const size_t MAX_DIST = 200*200;

struct pipe{
  char c;
  size_t x, y;
  size_t dist;
  bool in_loop;
  pipe(char c, size_t x, size_t y)
    : c(c)
    , x(x)
    , y(y)
    , dist(MAX_DIST)
  {

  }

  pipe()
    : c('.')
    , x(0)
    , y(0)
    , dist(MAX_DIST)
  {

  }

  pipe(const pipe& other)
    : c(other.c)
    , x(other.x)
    , y(other.y)
    , dist(other.dist)
  {

  }

  constexpr bool on_loop() const {
    return dist < MAX_DIST || c == 'S';
  }


  bool connected_north(char north_c) const {
    bool north_valid_pipe = (north_c=='|') || (north_c=='7') || (north_c=='F');
    bool valid_pipe = (c=='|') || (c=='L') || (c=='J') || (c=='S');
    return north_valid_pipe && valid_pipe;
  }

  bool connected_south(char south_c) const {
    bool valid_pipe = (c=='|') || (c=='7') || (c=='F') || (c=='S');
    bool south_valid_pipe = (south_c=='|') || (south_c=='L') || (south_c=='J');
    return south_valid_pipe && valid_pipe;
  }

  bool connected_west(char west_c) const {
    bool west_valid_pipe = (west_c=='-') || (west_c=='L') || (west_c=='F');
    bool valid_pipe = (c=='-') || (c=='7') || (c=='J') || (c=='S');
    return west_valid_pipe && valid_pipe;
  }

  bool connected_east(char east_c) const {
    bool east_valid_pipe = (east_c=='-') || (east_c=='7') || (east_c=='J');
    bool valid_pipe = (c=='-') || (c=='F') || (c=='L') || (c=='S');
    return east_valid_pipe && valid_pipe;
  }

  void show() {
    if (on_loop())
      cout << "\033[0;36m";
    if (in_loop)
      cout << "\033[0;33m";

    switch (c) {
    case '|':
      cout << "┃";
      break;

    case '-':
      cout << "━";
      break;

    case 'F':
      cout << "┏";
      break;

    case 'L':
      cout << "┗";
      break;


    case '7':
      cout << "┓";
      break;

    case 'J':
      cout << "┛";
      break;

    default:
      cout << c;
    }

    cout << "\033[0m";
  }
};

template <typename T>
class text_image {
public:
  size_t X, Y, start_x, start_y;
  vector<T> entries;

  text_image()
    : X(0)
    , Y(0)
    , start_x(0)
    , start_y(0)
  {

  }

  void parse_input_line(const string& line) {
    if (X==0) {
      X = line.size();
    }
    cout << line << endl;
    parser p(line);
    size_t x=0;
    while (!p.done()) {
      if (p.top() == 'S') {
        start_x = x;
        start_y = Y;
      }
      entries.emplace_back(p.top(), x, Y);
      p.step(1);
      x++;
    }

    Y++;
  }

  inline size_t to_index(const size_t x, const size_t y) const {
    return X * y + x;
  }

  void get_adjacent_idx(const size_t i, vector<size_t>& result) const {
    // Mapping from (x, y) coords to indices
    // (x, y) => X * y + x
    // i => (i % X, i / X)
    //vector<size_t> result;
    result.resize(0);
    size_t x = i % X;
    size_t y = i / X;

    if (x > 0) result.push_back(to_index(x-1, y)); // West
    if (y > 0) result.push_back(to_index(x, y-1)); // North
    if (x < X-1) result.push_back(to_index(x+1, y)); // East
    if (y < Y-1) result.push_back(to_index(x, y+1)); // South
    //if (x > 0 && y > 0) result.push_back(to_index(x-1, y-1));
    //if (x > 0 && y < Y-1) result.push_back(to_index(x-1, y+1));
    //if (x < X-1 && y > 0) result.push_back(to_index(x+1, y-1));
    //if (x < X-1 && y < Y-1) result.push_back(to_index(x+1, y+1));

  }

  constexpr T& operator[](const size_t idx) {
    return entries[idx];
  }
  constexpr T& operator[](const size_t x, const size_t y) {
    return entries[to_index(x, y)];
  }

  void show() {
    cout << endl;
    for (size_t i=0; i<entries.size(); i++) {
      entries[i].show();
      if ((i+1) % X == 0)
        cout << endl;
    }
    cout << endl;

  }

};

class pipe_graph {
public:
  pipe_graph() {

  }

  pipe* get_next(text_image<pipe>& pipes, pipe& p, vector<size_t>& n) {
    pipe* next = &pipes[n[0]];
    if (p.connected_west(next->c) && !next->on_loop())
      return next;

    next = &pipes[n[3]];
    if (p.connected_south(next->c) && !next->on_loop())
      return next;

    next = &pipes[n[2]];
    if (p.connected_east(next->c) && !next->on_loop())
      return next;

    next = &pipes[n[1]];
    if (p.connected_north(next->c) && !next->on_loop())
      return next;

    return nullptr;
  }

  int update_distances(text_image<pipe>& pipes) {
    vector<size_t> neighbors;
    pipe &S = pipes.entries[pipes.to_index(pipes.start_x, pipes.start_y)];
    S.dist = 0;
    pipes.get_adjacent_idx(pipes.to_index(S.x, S.y), neighbors);

    pipe *pos = get_next(pipes, S, neighbors);

    size_t dist=1;
    while (pos) {

      pos->dist = dist;
      dist++;

      pipes.get_adjacent_idx(pipes.to_index(pos->x, pos->y), neighbors);
      pos = get_next(pipes, *pos, neighbors);

    }

    size_t result = 0;
    for (size_t x=0; x<pipes.X; x++) {
      for (size_t y=0; y<pipes.Y; y++) {

        pipe& p = pipes.entries[pipes.to_index(x, y)];
        if (p.on_loop()) {
          p.in_loop = false;
          continue;
        }

        int num_crossings=0;
        for (size_t xx=0; xx<=x; xx++) {
          size_t idx = pipes.to_index(xx, y);
          pipe& test_p = pipes.entries[idx];
          if (test_p.on_loop() && (test_p.c == 'J' || test_p.c == 'L' || test_p.c == '|'))
            num_crossings ++;

        }
        p.in_loop = !p.on_loop() && (num_crossings % 2 == 1);

        result = (p.in_loop) ? result + 1 : result;
      }
    }

    pipes.show();


    return result;
  }
};

int main() {
  string line;
  value_t sum=0;

  text_image<pipe> im;
  size_t line_size = 0;
  while(getline(cin, line)) {
    if (line_size == 0) {
      line_size = line.size() + 2;
      im.parse_input_line(string(line_size, '.'));
    }
    im.parse_input_line("." + line + ".");
  }
  im.parse_input_line(string(line_size, '.'));
  pipe_graph g;
  cout << g.update_distances(im) << endl;

  return 0;
}
