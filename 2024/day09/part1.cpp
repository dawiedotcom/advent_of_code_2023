#include <iostream>
#include <string>
#include <vector>

#include "show.h"

using namespace std;

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

void init_disk(const vector<int>& input, vector<int>& disk) {

  size_t file_id = 0;
  auto disk_pos = disk.begin();
  SHOW(disk.size());
  for (size_t i=0; i<input.size(); i++) {
    int block_size = input[i];
    int disk_entry = (i % 2 == 0) ? file_id : -1;
    file_id += (i % 2 == 0) ? 1 : 0;

    SHOW(i, file_id, disk_entry, block_size);
    for (size_t j=0; j<block_size; j++) {
      *disk_pos = disk_entry;
      disk_pos ++;
    }
  }
}

void compact_disk(vector<int>& disk) {
  auto front = disk.begin();
  auto back = disk.end()-1;

  while (true) {
    //Move forward to first empty space
    while (*front >= 0) front ++;
    // Move backward to first non-empty space
    while (*back == -1) back --;

    if (front >= back)
      break;

    *front = *back;
    *back = -1;
  }
}

size_t disk_checksum(const vector<int>& disk) {
  size_t result = 0;
  for (size_t i=0; i<disk.size(); i++) {
    if (disk[i] == -1) break;
    result += i * disk[i];
  }
  return result;
}

int main() {
  vector<int> input;
  int disk_size;
  read_input(input, disk_size);
  //SHOW(input);

  vector<int> disk(disk_size);
  init_disk(input, disk);

  compact_disk(disk);
  //SHOW(disk);

  cout << disk_checksum(disk) << endl;
  return 0;
}
