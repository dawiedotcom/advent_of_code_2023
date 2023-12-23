#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <utility>

/*
 * Class to keep track of 2D arrays of ASCII characters.
 * See:
 *   - https://adventofcode.com/2023/day/3
 *   - https://adventofcode.com/2023/day/10
 *   - https://adventofcode.com/2023/day/14
 *   - https://adventofcode.com/2023/day/16
 */
template<typename cellT>
class text_image {
public:

  /* Constructor */
  text_image()
    : x(0)
    , y(0)
  { }

  /* Find the neigbors of the cell at a given position */
  void get_neighbors(const size_t x, const size_t y, std::unordered_map<std::string, cellT*>& result) {
    result.clear();
    //size_t x = i % X;
    //size_t y = i / X;
    if (x < 0 || x>X()-1 || y < 0 || y > Y()-1)
      return;

    if (x > 0) result["W"] = &entries[to_index(x-1, y)]; // West
    if (y > 0) result["N"] = &operator[](x, y-1); // North
    if (x < X()-1) result["E"] = &operator[](x+1, y); // East
    if (y < Y()-1) result["S"] = &operator[](x, y+1); // South
    if (x > 0 && y > 0) result["NW"] = &operator[](x-1, y-1);
    if (x > 0 && y < Y()-1) result["SW"] = &operator[](x-1, y+1);
    if (x < X()-1 && y > 0) result["NE"] = &operator[](x+1, y-1);
    if (x < X()-1 && y < Y()-1) result["SE"] = &operator[](x+1, y+1);

  }

  /* Add a new item to entries */
  constexpr void push_back(const cellT& cell) {
    update_x();
    entries.push_back(cell);
  }

  template <typename... Args>
  constexpr void emplace_back(Args&&... args) {
    update_x();
    entries.emplace_back(std::forward<Args>(args)...);
  }

  /* Get the number of columns in the image */
  constexpr size_t X() const {
    return x;
  }
  /* Get the number of rows in the image */
  constexpr size_t Y() const {
    return y;
  }
  /* Start adding cells in a new row */
  constexpr void start_new_row() {
    y++;
  }

  /* Access a row and column with the [] operator */
  constexpr cellT& operator[](const size_t x, const size_t y) {
    return entries[to_index(x, y)];
  }

  constexpr bool in_bounds(const int x, const int y) const {
    return (x >= 0) && (y >= 0) && (x < X()) && (y < Y());
  }

private:
  size_t x, y;
  std::vector<cellT> entries;

  /* Return the index in entries from a column and row */
  inline size_t to_index(const size_t x, const size_t y) const {
    return X() * y + x;
  }

  /* Increment x if we are still in the first row */
  inline void update_x() {
    if (Y() == 0) x++;
  }

  template <typename cellTT>
  friend std::ostream& operator<<(std::ostream& out, const text_image<cellTT>& ti);
};

template <typename cellT>
std::ostream& operator<<(std::ostream& out, const text_image<cellT>& ti) {
  for (size_t i=0; i<ti.entries.size(); i++) {
    out << ti.entries[i];
    if ((i+1) % ti.X() == 0)
      out << std::endl;
  }

  return out;
}
