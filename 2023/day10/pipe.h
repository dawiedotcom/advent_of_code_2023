#pragma once

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
    , in_loop(false)
  { }

  pipe()
    : c('.')
    , x(0)
    , y(0)
    , dist(MAX_DIST)
    , in_loop(false)
  { }

  pipe(const pipe& other)
    : c(other.c)
    , x(other.x)
    , y(other.y)
    , dist(other.dist)
    , in_loop(false)
  { }

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
};

std::ostream& operator<<(std::ostream& out, const pipe& p) {
  if (p.in_loop) {
    out << "\033[0;33m█\033[0m";
    return out;
  }
  if (!p.in_loop && !p.on_loop()) {
    out << '.';
    return out;
  }
  if (p.on_loop())
    out << "\033[0;36m";

  switch (p.c) {
  case '|':
    out << "┃";
    break;

  case '-':
    out << "━";
    break;

  case 'F':
    out << "┏";
    break;

  case 'L':
    out << "┗";
    break;

  case '7':
    out << "┓";
    break;

  case 'J':
    out << "┛";
    break;

  default:
    out << p.c;
  }

  out << "\033[0m";
  return out;
}
