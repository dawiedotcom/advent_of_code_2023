#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <iostream>

inline size_t min(size_t a, size_t b) {
  return a < b ? a : b;
}

class parser {
public:
  parser(std::string s);

  std::string to_token(const std::string& token);
  std::string with(int (*pridicate) (int));
  long long next_int();
  std::string next_word();

  char top() { return done() ? ' ' : line[ position];}
  inline bool done() const { return position >= line.size();}
  void show() { if (!done()) std::cout << line.substr(position) << std::endl;}
  inline size_t get_position() const { return position; }
  inline size_t get_last_token_length() const { return len; }
  void step(size_t N) {
    position = min(position + N, line.size());
  }
private:
  std::string line;
  size_t position;
  size_t len;
};

#endif // _PARSER_H_
