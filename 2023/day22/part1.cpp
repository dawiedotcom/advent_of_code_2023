#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cassert>

#include "lib.h"
#include "parser.h"

using namespace std;

struct coord_t {
  int x, y, z;
};

class brick_t;
ostream& operator<<(ostream& out, const brick_t& b);

class brick_t {
public:
  coord_t start, end;
  char label;
  //unordered_set<reference_wrapper<brick_t>> above;
  unordered_set<const brick_t*> above;
  unordered_set<const brick_t*> below;
  mutable unordered_map<const brick_t*, bool> above_cache;
  //unordered_set<const brick_t*> above;
  //vector<reference_wrapper<brick_t>> above;
  //vector<brick_t*> above;

  brick_t()
    : start{0,0,0}
    , end{0,0,0}
    , label(' ')
    , above{}
  {}

  brick_t(const string& line)
    : brick_t()
  {
    parser parse(line);
    start.x = parse.next_int();
    start.y = parse.next_int();
    start.z = parse.next_int();
    end.x = parse.next_int();
    end.y = parse.next_int();
    end.z = parse.next_int();
    assert(start.x <= end.x);
    assert(start.y <= end.y);
    assert(start.z <= end.z);
    assert(
           (start.x == end.x && start.y == end.y)
           || (start.x == end.x && start.z == end.z)
           || (start.y == end.y && start.z == end.z)
           );
  }

  bool is_in_above_children(const brick_t& b) const {
    if (start.z >= b.end.z)
      return false;
    if (above.empty())
      return false;
    //if (above_cache.contains(&b))// && above_cache.at(&b))
    //  return above_cache.at(&b);

    bool result =
      any_of(above.begin(),
             above.end(),
             [&b] (const brick_t* child){
               return child->is_in_above(b)
                 || child->is_in_above_children(b);
             });
    above_cache.emplace(&b, result);
    return result;
  }

  void add_above(const brick_t* b) {
    above.insert(b);
    above_cache.clear();
  }
  void add_below(const brick_t* b) {
    below.insert(b);
  }

  constexpr bool is_above(const brick_t& other) const {
    return overlap_xy(other) && start.z > other.end.z;
  }

  constexpr bool is_in_above(const brick_t& b) const {
    //cout << " above check: "
    //     << label
    //     << ".is_in_above(" << b.label << "): "
    //     << above.contains(&b)
    //     << endl;
    return above.contains(&b);
  }

  constexpr bool is_horizontal() const {
    return start.z == end.z;
  }
  constexpr bool is_horizontal_x() const {
    return is_horizontal() && (start.x == end.x);
  }
  constexpr bool is_horizontal_y() const {
    return is_horizontal() && (start.y == end.y);
  }

  constexpr bool overlap_xy(const brick_t& other) const {
    return
      ((is_horizontal_x()
        && other.is_horizontal_x()
        && start.x == other.start.x
        && ((start.y <= other.start.y && start.y >= other.end.y)
            || (end.y <= other.start.y && end.y >= other.end.y)))
       || 
       (is_horizontal_y()
        && other.is_horizontal_y()
        && start.y == other.start.y
        && ((start.x <= other.start.x && start.x >= other.end.x)
            || (end.x <= other.start.x && end.x >= other.end.x)))
       ||
       (is_horizontal_x()
        && other.is_horizontal_y()
        && start.x >= other.start.x && start.x <= other.end.x
        && start.y <= other.start.y && end.y >= other.end.y)
       ||
       (is_horizontal_y()
        && other.is_horizontal_x()
        && start.y >= other.start.y && start.y <= other.end.y
        && start.x <= other.start.x && end.x >= other.end.x)
       ||
       (~is_horizontal()
        && other.intersects_xy(start.x, start.y))
       ||
       (~other.is_horizontal()
        && intersects_xy(other.start.x, other.start.y))
       );
  }

  constexpr int intersects_xy(int x, int y) const {
    return (
            (~is_horizontal() && start.x == x && start.y == y)
            ||
            (is_horizontal_x()
             && start.x == x
             && start.y <= y 
             && end.y >= y)
            ||
            (is_horizontal_y()
             && start.y == y
             && start.x <= x 
             && end.x >= x));
  }

  string repr() const noexcept{
    std::stringstream ss;
    ss << (*this);
    return ss.str();
  }

  inline bool operator==(const brick_t& rhs) const noexcept{
    return repr() == rhs.repr();
  }
};

bool operator==(const brick_t& lhs, const brick_t& rhs) noexcept{
  return lhs.repr() == rhs.repr();
}

template<>
struct std::hash<brick_t> {
  std::size_t operator()(const brick_t& brick) const noexcept{
    return std::hash<std::string>{}(brick.repr());
  }
};

ostream& operator<<(ostream& out, const brick_t& b) {
  out << b.start.x << "," << b.start.y << "," << b.start.z << "~" << b.end.x << "," << b.end.y << "," << b.end.z;// << ")";
  return out;
}

class pile_t {
public:
  vector<brick_t> bricks;

  pile_t() {}

  void input(const string& line) {
    bricks.emplace_back(line);
    bricks.back().label = 'A' + bricks.size() - 1;
  }

  void build_graph() {
    sort(bricks.begin(), bricks.end(),
         [](brick_t& b1, brick_t& b2) {
           return
             b1.start.z > b2.start.z;
         });

    //for (const auto& b : bricks)
    //  cout << b << endl;
    //return;

    for (size_t i=1; i<bricks.size(); i++) {
      for (size_t j=i; j-- > 0; ) {
        if (bricks[j].is_above(bricks[i])
            && !bricks[i].is_in_above_children(bricks[j])
            ) {
          //cout << (bricks[i]) << " below " << (bricks[j]) << endl;
          //bricks[i].above.insert(&bricks[j]);
          bricks[i].add_above(&bricks[j]);
          bricks[j].add_below(&bricks[i]);
        }
      }

      cout << i << ": " << bricks[i] << " below: ";
      for (const auto a : bricks[i].above)
        cout << (*a) << " ";
      cout << endl;
    }

    size_t i=0;
    for (auto const& brick : bricks) {
      cout << (i++) << ": " << brick << endl;
      if (brick.is_horizontal_x()) {
        assert(brick.above.size() <= (brick.end.y - brick.start.y + 1));
        assert(brick.below.size() <= (brick.end.y - brick.start.y + 1));
      }
      if (brick.is_horizontal_y()) {
        assert(brick.above.size() <= (brick.end.x - brick.start.x + 1));
        assert(brick.below.size() <= (brick.end.x - brick.start.x + 1));
      }
      if (!brick.is_horizontal()) {
        assert(brick.above.size() <= 1);
        assert(brick.below.size() <= 1);
      }
    }
  }

  size_t count_removable() {
    unordered_set<const brick_t*> removable;
    for (const auto& brick : bricks) {
      if (brick.above.empty())
        removable.insert(&brick);
      if (brick.above.size() > 1) {
        for (const auto a : brick.above)
          //if (a->below.size() > 1)
          if (a->above.empty() || all_of(a->above.begin(), a->above.end(),
                     [](const brick_t* above_a) {
                       return above_a->below.size() > 1;
                     }))
              removable.insert(a);
        //removable.insert(brick.above.begin(), brick.above.end());
      }
    }
    cout << "Removable: ";
    for (const auto a : removable)
      cout << (*a).label << " ";
    cout << endl;
    return removable.size();
  }

  void settle() {
    build_graph();
    //sort(bricks.begin(), bricks.end(),
    //     [](brick_t& b1, brick_t& b2) {
    //       return
    //         b1.start.z < b2.start.z;
    //     });
    //for (const auto& brick : bricks) {
    //  for (const auto& above_brick : brick.above) {
    //    int height = above_brick->end.z - above_brick->start.z;
    //    above_brick->start.z = brick.end.z+1;
    //    above_brick->end.z = above_brick->start.z + height;
    //  }
    //}
  }
};

// 1256 -- to high
// 819 -- to high
int main() {
  string line;
  pile_t pile;
  while (getline(cin, line)) {
    if (line[0] == '#')
      continue;
    pile.input(line);
  }
  pile.settle();

  SHOW(pile.bricks.size());
  cout << pile.count_removable() << endl;
  return 0;
}
