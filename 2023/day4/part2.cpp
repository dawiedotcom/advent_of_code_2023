#include <iostream>
#include <string>
#include <set>
#include <vector>

#include "parser.h"

using namespace std;

class scratch_cards {
private:
  vector<size_t> num_cards;
  size_t total;
  size_t current_card;

public:
  scratch_cards(size_t total_cards)
    : num_cards()
    , total(0)
    , current_card(0)
  {
    num_cards.resize(total_cards, 1);
  };

  void process(string& line) {
    parser p(line);
    p.to_token("Card");
    p.next_int();
    p.to_token(": ");
    set<size_t> winners, played;
    while (p.top() != '|') {
      size_t num = p.next_int();
      winners.insert(num);
      if (p.top() == ' ') p.step(1);
      //cout << num << " ";
      p.show();
    }
    size_t score = 0;
    while (!p.done()) {
      size_t num = p.next_int();
      //played.insert(num);
      p.show();
      if (winners.contains(num)) {
        score ++;
      }
    }
    for (size_t i=1; i<=score; i++) {
      num_cards[current_card+i] += num_cards[current_card];
    }

    total += num_cards[current_card];
    current_card ++;
  }
  inline size_t get_total() const {
    for (size_t i=0; i<current_card; i++)
      cout << num_cards[i] << " ";
    cout << endl;
    return total;
  }


};
int main() {
  string line;
  scratch_cards cards(205);

  int sum = 0;
  while (getline(cin, line)) {
    cards.process(line);
  }
  cout << cards.get_total() << endl;

  return 0;
}
