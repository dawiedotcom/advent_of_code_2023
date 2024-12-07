#include "show.h"
#include <vector>

int main() {
  int a=10, b=20;
  SHOW(a, b);
  SHOW(a+b);
  std::vector<int> v = {a, b};
  SHOW(v);
  return 0;
}
