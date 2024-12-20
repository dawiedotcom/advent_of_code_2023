#include "parser.h"

#include <regex>

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
  std::string result = line.substr(position, token.size());
  //len = next_token_pos + token.size() - position;
  len = token.size();
  position = next_token_pos + token.size();
  return result;
}

//std::string parser::with(::) { //}int (*predicate) (int)) {
std::string parser::with(std::function<bool(char)> predicate){ //int (*pridicate) (int));
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

std::string parser::with(const std::string& s) {
  std::regex re(s);
  std::smatch match;
  if (std::regex_search(line.cbegin()+position, line.cend(), match, re)) {
    len = match.length();
    position += match.prefix().length() + len;
    return match.str();
  }

  position = line.size();
  len = 0;
  return "";
}

bool parser::match(const std::string& s) {
  std::regex re(s);
  std::smatch match;
  std::string tmp = line.substr(position);
  if (std::regex_search(tmp, match, re)) {
    len = match.length();
    return true;
  }
  len = 0;
  return false;

}

long long parser::next_int() {
  std::string s = with("(-?\\d+)");
  try {
    return s.size() > 0 ? stoll(s) : 0;
  }
  catch (...) {
    std::cout << "Out of range: "  << s << std::endl;
    return 0;
  }
}

unsigned long long parser::next_uint() {
  std::string s = with(::isdigit);
  try {
    return s.size() > 0 ? stoll(s) : 0;
  }
  catch (...) {
    std::cout << "Out of range: "  << s << std::endl;
    return 0;
  }
}


std::string parser::next_word() {
  return with(::isalpha);
}

void parser::show() {
  std::cout << *this;
}

std::ostream& operator<<(std::ostream& out, parser const& p) {
  if (!p.done())
    out << p.line.substr(p.position);
  return out;
}
