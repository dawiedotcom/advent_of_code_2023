#include <iostream>
#include <string>
#include <vector>

#include <interval.h>
#include <parser.h>

using namespace std;

using food_id_t=unsigned long;
using fresh_t=interval_<food_id_t>;

int main() {

  vector<fresh_t> fresh;
  string line;
  while(getline(cin, line)) {
    parser p(line);
    if (line == "")
      break;
    food_id_t a = p.next_uint();
    food_id_t b = p.next_uint()+1;
    fresh.emplace_back(a, b);
  }
  cout << fresh.size() << endl;

  food_id_t id;
  unsigned long int n_fresh=0;
  //unsigned long int n_ids=0;
  while(!cin.eof()) {
    cin >> id;
    //n_ids++;
    for (auto& f : fresh) {
      if (f.is_element(id)) {
        //cout << n_ids << ": fresh: " << id << endl;
        n_fresh++;
        break;
        //goto next_id;
      }
    }
    //cout << n_ids << ": spoilt: " << id << endl;
//next_id:
  }

  cout << n_fresh << endl;

  return 0;
}

