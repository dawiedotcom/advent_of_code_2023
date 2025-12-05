#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ranges>

#include <interval.h>
#include <parser.h>

using namespace std;

using food_id_t=unsigned long long;
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

  sort(fresh.begin(), fresh.end(), [](auto& a, auto& b) {return a.start > b.start;});
  for (auto& f: fresh)
    cout << f << endl;
  cout << endl;

  map<food_id_t, fresh_t> consolidated;
  //auto last = *(fresh.end()-1);
  auto first = fresh[0];
  consolidated[first.start] = first;

  for (auto& f : fresh | views::drop(1)) {
    bool should_add = true;
    if (consolidated.contains(f.start)) {
      //consolidated[f.start].end = max(f.end, consolidated[f.start].end);
      f.end = max(f.end, consolidated[f.start].end);
      consolidated.erase(consolidated.find(f.start));
      //continue;
    }
    while (true) {
      auto ub = consolidated.upper_bound(f.start);
      if (ub == consolidated.end())
        break;
      if (ub->second.intersects(f)) {
        //cout << (ub->second) << " + " << f << " = ";
        consolidated.erase(ub);
        auto start = min(f.start, ub->second.start);
        consolidated[start] = {start, max(f.end, ub->second.end)};
        //cout << consolidated[f.start] << endl;
        should_add = false;
      }
      else
        break;
    }
    /*
    auto lb = consolidated.lower_bound(f.start);
    if (lb != consolidated.end() && lb->second.intersects(f)) {
      cout << (lb->second) << " + " << f << " = ";
      lb->second.end = max(lb->second.end, f.end);
      cout << lb->second << endl;
      should_add = false;
    }
    if (lb != consolidated.end()) {

    }
    */

    if (should_add) {
      //cout << f << endl;
      consolidated[f.start] = f;
    }
  }

  food_id_t total_ids = 0;
  cout << endl;
  for (auto& [_, f]: consolidated) {
    cout << f << endl;
    total_ids += f.length();
  }
  cout << total_ids << endl;



  return 0;
}
// too low:  303544434901385
//           314651002694159
//           314651002694159
//
//           341753674214273
// too high: 341753674214274
