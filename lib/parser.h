#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <iostream>

class parser {
public:
  parser(std::string s);

  std::string to_token(const std::string& token);
  std::string with(int (*pridicate) (int));
  size_t next_int();
  std::string next_word();

  char top() { return done() ? ' ' : line[ position];}
  inline bool done() const { return position >= line.size()-1;}
  void show() { if (!done()) std::cout << line.substr(position) << std::endl;}
private:
  std::string line;
  size_t position;
};

#endif // _PARSER_H_
