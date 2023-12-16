#pragma once

#include <string>
#include <iostream>
#include <functional>

/*
 * Text parser for making sense of puzzle input.
 */
class parser {
public:
  /*
   * Creates a new parser for the given string
   */
  parser(std::string s);

  /*
   * Parses a constant token.
   */
  std::string to_token(const std::string& token);

  /*
   * Parses the next token while a predicate is true.
   * Examples:
   *
   */
  std::string with(std::function<bool(char)> predicate); //int (*pridicate) (int));

  /*
   * Parses the next token using a regular expression.
   * Examples:
   *
   */
  std::string with(const std::string& re);

  /*
   * Returns true if re matches any part of the string from the current position onward.
   */
  bool match(const std::string& re);

  /*
   * Parses the next number in the string:
   * Examples:
   *
   */
  long long next_int();

  /*
   * Parses the next unsigned number in the string:
   * Examples:
   *
   */
  unsigned long long next_uint();


  /*
   * Parses the next collection of alphabetic characters.
   */
  std::string next_word();

  /*
   * Parses the next collection of alphabetic characters.
   */
  std::string next_alpha_num_word() {
    return with([](char c) { return isalpha(c) || isdigit(c); });
  }

  /*
   * Returns the character at the current position.
   */
  inline char top() const { return done() ? ' ' : line[position];}

  /*
   * Returns true if the end of the string has been reached.
   */
  inline bool done() const { return position >= line.size();}

  /*
   * Prints the rest of the string to stdout.
   */
  void show();

  /*
   * Returns the current position.
   */
  constexpr size_t get_position() const { return position; }

  /*
   * Returns the length of the last parse.
   */
  constexpr size_t get_last_token_length() const { return len; }

  /*
   * Advances the position by a given amount.
   */
  void step(size_t N) {
    position = std::min(position + N, line.size());
  }

  /*
   * Sets the position to the start of the string.
   */
  constexpr void reset() {
    position = 0;
    len = 0;
  }
private:
  std::string line;
  size_t position;
  size_t len;
  friend std::ostream& operator<<(std::ostream& out, parser const& p);
};

std::ostream& operator<<(std::ostream& out, parser const& p);
