#include <vector>
#include <string>
#include <sstream>
#include <climits>

#include "test.h"
#include "parser.h"

template <typename T>
std::string make_input(const std::vector<T>& things) {
  std::stringstream strm;
  for (auto& thing : things)
    strm << thing;
  return strm.str();
}

int main() {
  init_test("parser");

  /// Test sizable number of integers
  {
    const size_t i_max = UINT_MAX/1000;
    const size_t i_step = 1323;
    std::stringstream strm;
    for (size_t i=0; i<i_max; i+=i_step) {
      strm << i << " ";
    }

    parser p(strm.str());
    size_t i=0;
    while (!p.done()) {
      size_t num = p.next_int();
      //p.step(1); // get rid of the space
      p.to_token(" "); // also gets rid of the next space
      TEST(i == num);
      i += i_step;
    }
  }

  /// Parse some constant tokens
  {
    std::vector<std::string> tokens = {
      "hello",
      "GAME",
      "<>",
      " ",
      "*",
      "  ",
      "\t",
    };
    parser p(make_input(tokens));
    for (auto& token : tokens) {
      std::string t = p.to_token(token);
      TEST(t == token);
      if (t != token) {
        std::cout << "'" << t << "' '" << token << "'" << std::endl;
      }
    }
    TEST(p.done());
  }
  /// Parse repeated tokens
  {
    parser p("hi hi");
    p.to_token("hi ");
    TEST(!p.done());
    TEST(p.top() == 'h');
  }
  /// Parse repeated tokens
  {
    parser p("*6");
    p.to_token(std::string(1, '*'));
    TEST(!p.done());
    TEST(p.top() == '6');
  }



  return report();
}
