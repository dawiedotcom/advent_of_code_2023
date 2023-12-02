#include "parser.h"

parser::parser(std::string s)
  : line(s)
  , position(0)
{

}

std::string parser::to_token(const std::string& token) {
  if (done()) return "";
  auto next_token_pos = line.find(token, position);
  if (next_token_pos == std::string::npos)
    next_token_pos = line.size();
  std::string result = line.substr(position, next_token_pos-position);
  position = next_token_pos + token.size();
  return result;
}

std::string parser::with(int (*predicate) (int)) {
  if (done()) return "";
  size_t len = 0;

  while (position < line.size() && !predicate(line[position])) position++;
  while (position + len < line.size() && predicate(line[position + len])) len++;
  if (!predicate(line[position+len-1])) {
    position = line.size()-1;
    return "";
  }
  std::string result = line.substr(position, len);
  //std::cout << result << std::endl;
  position += len;
  return result;

}

size_t parser::next_int() {
  std::string s = with(isdigit);
  return s.size() > 0 ? stoi(s) : 0;
}

std::string parser::next_word() {
  return with(isalpha);
}