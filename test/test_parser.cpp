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
    strm << thing << " ";
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
  /// Parse negative numbers
  {
    std::vector<std::string> tokens = {
      "10", "-10"
    };
    parser p(make_input(tokens));
    for (auto& token : tokens) {
      long long t = p.next_int();
      TEST(t == stoll(token));
      if (t != stoll(token)) {
        std::cout << "'" << t << "' '" << token << "'" << std::endl;
      }
    }
    p.step(1);
    TEST(p.done());
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
      p.step(1);
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
  /// Parse words tokens
  {
    std::vector<std::string> words = {
      "hi ",
      "i ",
      "aoesuato ",
      "abc"
    };
    parser p(make_input(words));
    for (auto& word : words) {
      std::string t = p.next_word();
      if (word[word.size() - 1] == ' ')
        word = word.substr(0, word.size()-1);
      TEST(t == word);
      if (t != word) {
        std::cout
          << "parser at: " << p.get_position()
          << "'" << t
          << "' '" << word <<
          "'" << std::endl;
      }
    }
    p.step(1);
    TEST(p.done());
  }
  /// Parse alpha-numeric tokens
  {
    std::vector<std::string> words = {
      "11abc ",
        "90aoue097e ",
        "abcd ",
        "118098"
    };
    parser p(make_input(words));
    for (auto& word : words) {
      std::string t = p.next_alpha_num_word();
      if (word[word.size() - 1] == ' ')
        word = word.substr(0, word.size()-1);
      TEST(t == word);
      if (t != word) {
        std::cout
          << "parser at: " << p.get_position()
          << " got '" << t
          << "' '" << word <<
          "'" << std::endl;
      }
    }
    p.step(1);
    TEST(p.done());
  }

  /// Parse with regular expressions
  {
    std::vector<std::string> words = {
      "hello",
      "world",
      "Hello",
      "World"
    };
    parser p(make_input(words));
    auto it = words.begin();
    while (!p.done()) {
      auto token = p.with("(\\w+)");
      TEST(token == *it++);
      p.step(1);
    }
  }
  /// test match
  {
    parser parse("hi bye");
    TEST(!parse.match("^bye"));
    TEST(parse.get_last_token_length() == 0);

    TEST(!parse.match("hello"));
    TEST(parse.get_last_token_length() == 0);
    TEST(parse.match("hi"));
    TEST(parse.get_last_token_length() == 2);
    TEST(parse.match("^hi"));
    TEST(parse.get_last_token_length() == 2);
    TEST(parse.match("bye"));
    TEST(parse.get_last_token_length() == 3);
    parse.to_token("hi ");
    TEST(!parse.match("hi"));
    TEST(parse.get_last_token_length() == 0);
    TEST(parse.match("bye"));
    TEST(parse.get_last_token_length() == 3);
    TEST(parse.match("^bye"));
    TEST(parse.get_last_token_length() == 3);
  }

  return report();
}
