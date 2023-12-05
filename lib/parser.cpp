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
  //len = next_token_pos + token.size() - position;
  len = token.size();
  position = next_token_pos + token.size();
  return result;
}

std::string parser::with(int (*predicate) (int)) {
  if (done()) return "";
  len = 0;

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

long long parser::next_int() {
  std::string s = with(isdigit);
  try {
    return s.size() > 0 ? stoll(s) : 0;
  }
  catch (...) {
    std::cout << "Out of range: "  << s << std::endl;
    return 0;
  }
}

std::string parser::next_word() {
  return with(isalpha);
}
