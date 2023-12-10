#pragma once

//#include <string>
//#include <unordered_map>

class pipe_image {
public:
  size_t start_x, start_y;
  text_image<pipe> pipes;

  pipe_image()
    : start_x(0)
    , start_y(0)
  { }

  void parse_input_line(const string& line) {
    cout << line << endl;
    parser p(line);

    size_t x=0;
    while (!p.done()) {
      if (p.top() == 'S') {
        start_x = x;
        start_y = pipes.Y();
      }
      pipes.emplace_back(p.top(), x, pipes.Y());
      p.step(1);
      x++;
    }

    pipes.start_new_row();
  }


  pipe* get_next(const pipe& p, unordered_map<string, pipe*>& n) {
    if (p.connected_west(n["W"]->c) && !n["W"]->on_loop())
      return n["W"];

    if (p.connected_south(n["S"]->c) && !n["S"]->on_loop())
      return n["S"];

    if (p.connected_east(n["E"]->c) && !n["E"]->on_loop())
      return n["E"];

    if (p.connected_north(n["N"]->c) && !n["N"]->on_loop())
      return n["N"];

    return nullptr;
  }


  //virtual int get_answer() = 0;
};
