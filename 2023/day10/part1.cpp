#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "lib.h"
#include "parser.h"
#include "text_image.h"

using namespace std;

#include "pipe.h"
#include "pipe_image.h"

class pipe_image1 : public pipe_image {
public:
  pipe_image1()
    : pipe_image()
  {

  }

  int update_distances() {
    unordered_map<string, pipe*> neighbors;
    pipe &S = pipes[start_x, start_y];
    S.dist = 0;
    pipes.get_neighbors(S.x, S.y, neighbors);

    size_t dist = 0;
    pipe *left=&S, *right=&S;
    do {
      dist++;

      pipes.get_neighbors(left->x, left->y, neighbors);
      left = get_next(*left, neighbors);
      if (left) left->dist = dist;

      pipes.get_neighbors(right->x, right->y, neighbors);
      right = get_next(*right, neighbors);
      if (right) right->dist = dist;

      //SHOW(left);
      //SHOW(left->c,left->x, left->y);
      //SHOW(right->c,right->x, right->y);

    } while (left && right && ((left->x != right->x) || (right->y != left->y)));
    cout << pipes;

    return dist;

  }
};

int main() {
  string line;
  unsigned int sum=0;

  size_t line_size = 0;
  pipe_image1 g;
  while(getline(cin, line)) {
    if (line_size == 0) {
      line_size = line.size() + 2;
      g.parse_input_line(string(line_size, '.'));
    }
    g.parse_input_line("." + line + ".");
  }
  g.parse_input_line(string(line_size, '.'));
  cout << g.update_distances() << endl;

  return 0;
}
