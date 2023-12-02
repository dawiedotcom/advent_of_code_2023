//#pragma once
#ifndef _CUBE_GAME_PARSER_H_
#define _CUBE_GAME_PARSER_H_

#include "parser.h"

class cube_game_parser : public parser {
 public:
 cube_game_parser(std::string s) : parser(s) {};


  size_t game() {
    to_token("Game");
    size_t result = next_int();
    to_token(":");
    return result;
  }

  size_t cube(std::string& color) {
    size_t result = next_int();
    color = next_word();
    return result;
  }

  void round(size_t& red, size_t& green, size_t& blue) {
    std::string color;
    size_t round_pos = 0, count;
    red = green = blue = 0;
    while (!done() && top() != ';') {
      count = cube(color);
      if (color == "blue") blue = count;
      if (color == "red") red = count;
      if (color == "green") green = count;

      if (top() == ' ') to_token(",");
    }
    to_token(";");
  }
};


#endif // _CUBE_GAME_PARSER_H_
