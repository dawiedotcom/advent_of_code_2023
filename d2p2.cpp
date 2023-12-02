#include <iostream>
#include <string>

using namespace std;

string parse_to_token(const string& line, size_t& pos, const string& token) {
  auto next_semi_pos = line.find(token, pos);
  if (next_semi_pos == string::npos)
    next_semi_pos = line.size();
  string result = line.substr(pos, next_semi_pos-pos);
  pos = next_semi_pos + token.size();
  return result;
}

string parse_next(const string& line, size_t& pos, int (*predicate) (int) ) {
  while (pos < line.size() && !predicate(line[pos])) pos++;
  size_t len = 0;
  while (pos + len < line.size() && predicate(line[pos + len])) len++;
  if (!predicate(line[pos+len-1])) {
    pos = line.size()-1;
    return "";
  }
  //cout << line.substr(pos, len) << endl;
  string result = line.substr(pos, len);
  pos += len;
  return result;
}

size_t parse_next_int(const string& line, size_t& pos) {
  string s = parse_next(line, pos, isdigit);
  return s.size() > 0 ? stoi(s) : 0;
}

string parse_next_word(const string& line, size_t& pos) {
  return parse_next(line, pos, isalpha);
}

size_t parse_game(const string& line, size_t& pos) {
  parse_to_token(line, pos, "Game ");
  return parse_next_int(line, pos);
}

size_t parse_cube(const string& line, size_t& pos, string& color) {
  size_t result = parse_next_int(line, pos);
  color = parse_next_word(line, pos);
  parse_to_token(line, pos, ",");
  return result;
}

void parse_next_round(const string& line, size_t& pos, size_t& red, size_t& green, size_t& blue) {
  string round =  parse_to_token(line, pos, ";");
  string color;
  //cout << round << endl;
  size_t round_pos = 0, count;
  red = green = blue = 0;
  while (round_pos < round.size()) {
    count = parse_cube(round, round_pos, color);
    if (color == "blue") blue = count;
    if (color == "red") red = count;
    if (color == "green") green = count;
  }
}

int process(string& line) {
  size_t pos = 0;

  size_t game = parse_game(line, pos);
  //cout << line << endl;
  //cout << game << endl;
  size_t red=1, green=1, blue=1;
  size_t red_max=1, green_max=1, blue_max=1;
  //for (size_t i=0; i<3; i++) {
  while (true) {
    red = green = blue = 0;
    parse_next_round(line, pos, red, green, blue);
    //cout << red << " red, " << green << " green, " << blue << " blue" << endl;
    if (blue_max < blue) blue_max = blue;
    if (red_max < red) red_max = red;
    if (green_max < green) green_max = green;
    //if (red > 12 || green > 13 || blue > 14) {
    //  cout << "can't be" << endl;
    //  return 0;
    //}

    if (pos >= line.size())
      break;
  }
  //cout << "ok" << endl;

  return red_max * green_max * blue_max;
}

int main() {
  string line;

  int sum = 0;
  while (getline(cin, line)) {
    sum += process(line);
  }
  cout << sum << endl;

  return 0;
}
