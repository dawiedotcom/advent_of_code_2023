#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <numeric>

#include "parser.h"
#include "lib.h"

using namespace std;

#include "hash.h"

struct lens_t {
  string label;
  size_t focal_length;

  lens_t(const string& label, size_t fl)
    : label(label)
    , focal_length(fl)
  { }

  constexpr size_t focus_power(size_t box_nr, size_t box_position) const {
    return focal_length * (box_nr+1) * (box_position +1);
  }
};

struct box_t {
  list<lens_t> content;

  box_t() {}
  
  void insert(const string& label, size_t fl) {
    for (auto& lens : content) {
      if (lens.label == label) {
        lens.focal_length = fl;
        return;
      }
    }
    content.emplace_back(label, fl);
  }

  void remove(const string& label) {
    erase_if(content, [label](lens_t& lens) {return lens.label == label;});
  }

  size_t focus_power(size_t box_nr) const {
    size_t i=0;
    return accumulate(content.begin(), content.end(), 0,
                      [box_nr, &i](size_t fp, const lens_t& lens) {
                        return fp + lens.focus_power(box_nr, i++);
                      });
  }
};

struct hashmap_t {
  unordered_map<size_t, box_t> boxes;

  hashmap_t() {}

  void place_in_box(const string& label, size_t fl) {
    size_t box_nr = calc_hash(label);
    boxes[box_nr].insert(label, fl);
  }

  void remove_from_box(const string& label) {
    size_t box_nr = calc_hash(label);
    boxes[box_nr].remove(label);
  }

  void input(parser& parse) {
    //parser parse(line);

    //string label = parse.with(R"([A-Za-z]+)");
    string label = parse.next_word();
    
    if (parse.top() == '=') {
      float fl = parse.next_uint();
      place_in_box(label, fl);
    }
    else if (parse.top() == '-') {
      remove_from_box(label);
    }

    //cout << "After: " << line << " (hash: " << calc_hash(label) << ")" << endl;
    //show();
    //cout << endl;

  }

  size_t focus_power() const {
    return accumulate(boxes.begin(), boxes.end(), 0,
                      [](size_t fp, const auto& p) {
                        //[](size_t fp, const auto& [box_nr, box]) {
                        return fp + p.second.focus_power(p.first);
                        //return fp + box.focus_power(box_nr);
                      });
  }

  void show() {
    for (auto& [box_nr, box] : boxes) {
      if (box.content.size() > 0) {
        cout << "Box: " << box_nr; //<< endl;
        for (const auto& lens : box.content) {
          cout << " [" << lens.label << " " << lens.focal_length << "]";
        }
        cout << endl;
      }
    }
  }
};

int main() {
  string line;

  size_t sum=0;
  hashmap_t hashmap;
  while (getline(cin, line)) {
    parser parse(line);
    while (!parse.done()) {
      //string step = parse.with(R"([A-Za-z0-9=-]+)");

      hashmap.input(parse);

      if (parse.top() == ',')
        parse.to_token(",");
    }
  }
  cout << hashmap.focus_power() << endl;
  return 0;
}
