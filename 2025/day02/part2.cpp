#include <iostream>
#include <string>
#include <cmath>

#include <parser.h>
#include <show.h>

using namespace std;

using number_t = unsigned long long;

bool is_valid_id(number_t id) {
  int num_digits = log10(id) + 1.0;
  //if (num_digits % 2 == 1)
  //  return false;

  for (int chunk_num_digits=1; chunk_num_digits<=num_digits/2; chunk_num_digits++) {
    if (num_digits % chunk_num_digits != 0)
      continue;
    number_t x = pow(10, chunk_num_digits);

    number_t mask = id / pow(10, num_digits-chunk_num_digits);
    //number_t div = 1; //pow(10, num_digits - j);
    for (int chunk_idx=1; chunk_idx<num_digits/chunk_num_digits+1; chunk_idx++) {
      number_t div = pow(10, num_digits - chunk_idx*chunk_num_digits);
      number_t chunk = (id / div) % x;
      //chunk = chunk % x;
      //SHOW(id, chunk, mask, x, div, chunk_num_digits, chunk_idx, num_digits);
      if (mask != chunk)
        goto next_chunk_size;
      if (chunk_idx == num_digits/chunk_num_digits) {
        //SHOW(id, "valid");
        return true;
      }
      //div *= 10;
    }
next_chunk_size:
  }

  return false;
}


int main() {

  string input;
  cin >> input;
  parser p(input);

  number_t result = 0;

  while (!p.done()) {
    number_t low = p.next_uint();
    number_t high = p.next_uint();
    for (number_t id = low; id<= high; id++) {
      if (is_valid_id(id))
        result += id;
    }
    //cout << low << " - " << high << "\n: " << result << endl;
  }
  cout << result << endl;

  return 0;
}
