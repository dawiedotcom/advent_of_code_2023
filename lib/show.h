#pragma once

#include <stdio.h>  
#include <iostream>
#include <tuple>
#include <vector>
#include <unistd.h>

#define FORMAT_LIGHT_GRAY (isatty(STDOUT_FILENO) ? "\033[0;37m" : "")
#define FORMAT_END (isatty(STDOUT_FILENO) ? "\033[0m" : "")

#define COUT_FILE_LINE                                            \
  FORMAT_LIGHT_GRAY << "[" << __FILE__ << ":" << __LINE__ << "]: " << FORMAT_END

/* Overload << for std::vector */
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  out << "[";
  for (auto it=v.begin(); it!=v.end(); it++) {
    out << (*it);
    if (v.size() > 1 && it != v.end() - 1)
      out << ", ";
  }
  out << "]";
  return out;
}
/*
 * Helper struct for the SHOW macro.
 * Parses variable names and keeps values in a tuple.
 * See
 *  https://stackoverflow.com/a/6894436
 *  https://en.cppreference.com/w/cpp/language/parameter_pack
 */
template <typename... Args>
struct cout_exprs {
  std::tuple<Args...> values;
  std::vector<std::string> exprs;

  cout_exprs(const std::string& names, std::tuple<Args...> v)
    : values(v)
  {
    std::size_t pos=0, old_pos=0;;
    do {
      pos = names.find(",", old_pos);
      pos = (pos == std::string::npos) ? names.size() : pos;
      exprs.push_back(names.substr(old_pos, pos - old_pos));
      //std::cout << exprs[exprs.size() - 1] << std::endl;
      old_pos = pos+1;
    }
    while(pos < names.size());
  }
};

template <std::size_t I=0, typename... Args>
constexpr typename std::enable_if<I==sizeof...(Args), std::ostream&>::type
operator<<(std::ostream& out, cout_exprs<Args...> const& ce) {
  return out;
}

template <std::size_t I=0, typename... Args>
constexpr typename std::enable_if<I < sizeof...(Args), std::ostream&>::type
operator<<(std::ostream& out, cout_exprs<Args...> const& ce) {
  out << ce.exprs[I] << "=" << std::get<I>(ce.values);
  if constexpr(I < sizeof...(Args)-1)
      out << ", ";
  operator<< <I+1, Args...> (out, ce);
  return out;
}



#define SHOW(...)                                           \
  std::cout << COUT_FILE_LINE                               \
  << cout_exprs(#__VA_ARGS__, std::make_tuple(__VA_ARGS__)) \
  << "\n"
