#pragma once

#include <string>
#include <iostream>

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
  std::string with(int (*pridicate) (int));
  /*
   * Parses the next number in the string:
   * Examples:
   *
   */
  long long next_int();

  /*
   * Parses the next collection of alphabetic characters.
   */
  std::string next_word();

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
private:
  std::string line;
  size_t position;
  size_t len;
  friend std::ostream& operator<<(std::ostream& out, parser const& p);
};

std::ostream& operator<<(std::ostream& out, parser const& p);
