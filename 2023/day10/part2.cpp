#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

//#include "lib.h"
#include "parser.h"
#include "text_image.h"

using namespace std;

#include "pipe.h"
#include "pipe_image.h"

class pipe_image2 : public pipe_image {
public:
  pipe_image2()
    : pipe_image()
  { }

  int get_answer() {
    unordered_map<string, pipe*> neighbors;
    pipe &S = pipes[start_x, start_y];
    S.dist = 0;
    pipes.get_neighbors(S.x, S.y, neighbors);

    pipe *pos = get_next(S, neighbors);

    size_t dist=1;
    while (pos) {

      pos->dist = dist;
      dist++;

      pipes.get_neighbors(pos->x, pos->y, neighbors);
      pos = get_next(*pos, neighbors);

    }

    size_t result = 0;
    for (size_t x=0; x<pipes.X(); x++) {
      for (size_t y=0; y<pipes.Y(); y++) {

        pipe& p = pipes[x, y];
        if (p.on_loop()) {
          p.in_loop = false;
          continue;
        }

        int num_crossings=0;
        for (size_t xx=0; xx<=x; xx++) {
          const pipe& test_p = pipes[xx, y];
          if (test_p.on_loop() && (test_p.c == 'J' || test_p.c == 'L' || test_p.c == '|'))
            num_crossings ++;

        }
        p.in_loop = !p.on_loop() && (num_crossings % 2 == 1);

        result = (p.in_loop) ? result + 1 : result;
      }
    }

    cout << pipes;

    return result;
  }
};

int main() {
  string line;
  long long sum=0;

  pipe_image2 p;
  size_t line_size = 0;
  while(getline(cin, line)) {
    if (line_size == 0) {
      line_size = line.size() + 2;
      p.parse_input_line(string(line_size, '.'));
    }
    p.parse_input_line("." + line + ".");
  }
  p.parse_input_line(string(line_size, '.'));
  cout << p.get_answer() << endl;

  return 0;
}
