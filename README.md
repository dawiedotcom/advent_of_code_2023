# Advent of Code

Solutions to [Advent of Code](https://adventofcode.com/2023).

## Usage

Each year gets its own directory and every day has a subdirectory.
Solutions to the two parts should be named `part1` and `part2`.

C++ solutions can be compiled and run with
```bash
make run
```
The Makefile calls `run.sh` which will execute programs 
and pipe all files that matches `*input` to stdin.
The solution is expected to be on the last line of output.
If an input file has a corresponding file ending in `.ans.1` or `.ans.2`
the first line of that file is compared to the last line of output to
'test' a program.

## Lib

Some useful utilities are available in [lib/](lib/).

### lib

The `SHOW` macro will print its argumens and their values.

**Example**:
```c++
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
```
**Output**:
```
[examples/show.cpp:6]: a=10,  b=20
[examples/show.cpp:7]: a+b=30
[examples/show.cpp:9]: v=[10, 20]
```

### Parser

TODO

### Text Image

TODO

### Interval

TOD
