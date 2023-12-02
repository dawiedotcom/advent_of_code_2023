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

size_t parse_next_int(const string& line, size_t& pos) {
  while (pos < line.size() && !isdigit(line[pos])) pos++;
  size_t len = 0;
  while (pos + len < line.size() && isdigit(line[pos + len])) len++;
  if (!isdigit(line[pos+len-1])) {
    pos = line.size()-1;
    return 0;
  }
  //cout << line.substr(pos, len) << endl;
  size_t result = stoi(line.substr(pos, len));
  pos ++;
  return result;
}

size_t parse_game(const string& line, size_t& pos) {
  auto game_phrase = parse_to_token(line, pos, ":");
  size_t tmp=0;
  return parse_next_int(game_phrase, tmp);
}

size_t parse_cube(const string& line, size_t& pos, string& color) {
  string s = parse_to_token(line, pos, ",");
  size_t tmp=0;
  size_t result = parse_next_int(s, tmp);
  if (s.find("blue") != string::npos) color = "blue";
  if (s.find("green") != string::npos) color = "green";
  if (s.find("red") != string::npos) color = "red";

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
  //red = parse_next_int(round, round_pos);
  //green = parse_next_int(round, round_pos);
  //blue = parse_next_int(round, round_pos);
}


int process(string& line) {
  size_t pos = 0;

  size_t game = parse_game(line, pos);
  cout << line << endl;
  cout << game << endl;
  size_t red, green, blue;
  //for (size_t i=0; i<3; i++) {
  while (true) {
    red = green = blue = 0;
    parse_next_round(line, pos, red, green, blue);
    cout << red << " red, " << green << " green, " << blue << " blue" << endl;
    if (red > 12 || green > 13 || blue > 14) {
      cout << "can't be" << endl;
      return 0;
    }

    if (pos >= line.size())
      break;
  }
  cout << "ok" << endl;

  return game;
}

int main() {
  string line;

  int sum = 0;
  while (getline(cin, line)) {
    sum += process(line);
  }
  cout << "Answer: " << sum << endl;

  return 0;
}
