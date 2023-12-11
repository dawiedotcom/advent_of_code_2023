#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

struct input {
  string hand;
  int bid;
  unsigned long long score;
  int rank;
};

vector<input> inputs;

int value(char c) {
  if (c == 'A') return 14;
  if (c == 'K') return 13;
  if (c == 'Q') return 12;
  if (c == 'J') return 11;
  if (c == 'T') return 10;
  return (int) c - '0';
}

int type(string hand) {
  unordered_map<char, int> counts;
  cout << hand << endl;
  for (auto& c: hand) {
    counts[c] ++;
    cout << c << ": " << counts [c] << endl;
  }
  int max=0, second=0;
  vector<int> scounts;
  for (auto& [_, v] : counts) {
    scounts.push_back(v);
  }
  sort(scounts.begin(), scounts.end(), [](int a, int b) {return a>b;});
  cout << max << " " << second << endl;
  cout << scounts[0] << " " << scounts[1] << endl;
  max = scounts[0];
  second = scounts[1];
  cout << endl;
  //max = (max < v) ? v : max;
  if (max == 5) return 7;
  if (max == 4) return 6;
  if (max == 3 && second == 2) return 5;
  if (max == 3) return 4;
  if (max == 2 && second == 2) return 3;
  if (max == 2) return 1;
  return 0;
}

unsigned long long score(string hand) {
  unsigned long long result = 0;
  for (size_t i=0; i<hand.size();i++)
    result = 14*result + value(hand[i]);
  result += 10000000 * type(hand);
  return result;
}

int read(string& line) {
  string hand = line.substr(0, 5);
  int bid = stoi(line.substr(6));
  int s = score(hand);

  input i;
  i.hand=hand;
  i.bid=bid;
  i.score=s;
  i.rank=0;
  inputs.push_back(i);

  return 0;
}



int main() {
  string line;

  while (getline(cin, line)) {
    read(line);
  }

  sort(inputs.begin(), inputs.end(), [](input a, input b) { return a.score < b.score; });

  int sum = 0;
  for (size_t i=0; i<inputs.size(); i++){
    cout << i << ": " << inputs[i].hand << " " << inputs[i].bid << " " << inputs[i].score << endl;

    sum += (i+1) * inputs[i].bid;
  }


  cout << sum << endl;

  return 0;
}
