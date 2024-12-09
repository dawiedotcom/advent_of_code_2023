#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ranges>

#include "show.h"
#include "interval.h"

using namespace std;

using chunk_t = interval_<size_t>;

/*
bool operator<(const chunk_t& lhs, const chunk_t& rhs) {
  return lhs.start < rhs.start;
}
*/

// map of file_id -> interval containing the file on disk
using files_t = map<size_t, chunk_t>;
// map of free space first index to it's interval -- could be set<chunk_t> if set's iterators are not const refs
using free_space_t = map<size_t, size_t>;


void read_input(vector<int>& input, int& disk_size) {
  string line;
  getline(cin, line);
  disk_size = 0;
  input.clear();
  for (auto c : line) {
    int digit = (int)(c - '0');
    disk_size += digit;
    input.push_back(digit);
  }
}

void init_disk(const vector<int>& input, files_t& files, free_space_t& free_space) {

  size_t file_id = 0;
  size_t disk_pos = 0;
  for (size_t i=0; i<input.size(); i++) {
    if (i % 2 == 0) {
      files.try_emplace(file_id, disk_pos, disk_pos + input[i]);
      file_id ++;
    }
    else if (input[i] > 0) {
        free_space.try_emplace(disk_pos, disk_pos + input[i]);
    }
    disk_pos += input[i];
  }
}

void agg_free_space(free_space_t& free_space, const free_space_t::iterator& new_space) {
  auto next_chunk = next(new_space, -1);
  auto chunk = next_chunk++;
  auto end_chunk = next(new_space, 2);

  while (next_chunk != free_space.end() && next_chunk != end_chunk) {
    if (chunk->second == next_chunk->first) {
      //cout 
      //  << "joined: [" 
      //  << chunk->first 
      //  << ", "
      //  << chunk->second 
      //  << ") and ["
      //  << next_chunk->first
      //  << ", "
      //  << next_chunk->second
      //  << ")"
      //  << "\n";
      chunk->second = next_chunk->second;
      free_space.erase(next_chunk++);
      continue;
    }
    chunk++;
    next_chunk++;
  }
}
void defrag_disk(files_t& files, free_space_t& free_space) {
  for (auto& [id, file_chunk] : files | views::reverse) {

    auto len = file_chunk.length();

    // Looking from the left, find the first free chunk that the current file will 
    // fit in, that is to the left of it's current position
    auto free_chunk = free_space.begin();
    auto free_chunk_len = free_chunk->second - free_chunk->first;
    for (; free_chunk!= free_space.upper_bound(file_chunk.start); free_chunk++, free_chunk_len = free_chunk->second - free_chunk->first) {
      if (free_chunk_len >= file_chunk.length())
        break;
    }
    if (free_chunk->first >= file_chunk.start)
      continue;

    auto new_start = free_chunk->first;
    auto new_end = new_start + len;

    auto new_free_space_len = free_chunk_len - len;
    if (new_free_space_len > 0) {
      free_space.try_emplace(new_end, new_end+new_free_space_len);
    }

    // New free space where the file use to be
    auto [new_space, _] = free_space.try_emplace(file_chunk.start, file_chunk.end);
    // Remove free space in the file's new location
    free_space.erase(free_chunk);
    // Move the file
    file_chunk.start = new_start;
    file_chunk.end = new_end;
    // Aggregate consecutive free spaces
    agg_free_space(free_space, new_space);
  }
}

unsigned long long disk_checksum(const files_t& files) {
  unsigned long long result = 0;
  for (auto& [id, chunk] : files) 
    for (unsigned long long pos=chunk.start; pos<chunk.end; pos++)
      result += id * pos;
  return result;
}

int main() {
  vector<int> input;
  int disk_size;
  read_input(input, disk_size);

  files_t files;
  free_space_t free_space;
  init_disk(input, files, free_space);

  //SHOW(disk_size, files.size(), free_space.size());
  defrag_disk(files, free_space);
  //SHOW(disk_size, files.size(), free_space.size());

  cout << disk_checksum(files) << endl;
  return 0;
}
