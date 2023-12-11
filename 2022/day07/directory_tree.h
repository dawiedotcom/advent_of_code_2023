#pragma once

struct directory_tree {
  deque<string> pwd;
  unordered_map<string, size_t> sizes;
  string current_cmd;

  directory_tree()
  {}

  void parse_line(const string& line) {
    parser p(line);

    if (line[0] != '$') {
      ls_result(p);
      return;
    }

    p.to_token("$ ");
    if (p.top() == 'c')
      cd(p);
    else
      ls(p);
  }

  void ls_result(parser& p) {
    if (p.top() == 'd') {

    }
    else {
      size_t s = p.next_int();
      for (size_t i=0; i<depth(); i++) {
        sizes[make_full_path(i)] += s;
        //SHOW(make_full_path(i), sizes[make_full_path(i)]);
      }
    }
  }

  void ls(parser& p) {

  }

  void cd(parser& p) {
    p.to_token("cd ");
    string dir_name = p.with(R"([\.\w\d/]+)");
    if (dir_name == "..")
      pwd.pop_back();
    else {
      pwd.push_back(dir_name);
      //SHOW(p, dir_name, make_full_path(0));
      sizes[make_full_path(0)] = 0;
    }
  }

  string make_full_path(size_t from_top) const {
    return accumulate(pwd.begin(), pwd.end()-from_top, string(""), [](string a, string b) { return a + ((a == "" || a == "/") ? "" : "/") + b; });
  }

  inline size_t depth() const {
    return pwd.size();
  }

  size_t sum_smallest() const {
    size_t result = 0;
    for (const auto& [dirname, size] : sizes) {
      SHOW(dirname, size);
      if (size < 100000)
        result += size;
    }

    return result;;
  }

  size_t find_smallest_dir_larger_than(const size_t required_space) const {
    const size_t max_space = 70000000;
    const size_t space_in_use = sizes.at("/");
    size_t result = space_in_use;
    SHOW(space_in_use);
    for (const auto& [dirname, size] : sizes) {
      cout << size << "\t\t"  << (space_in_use - size) << "\t\t" << dirname;
      if (space_in_use - size > max_space - required_space ) {
        cout << "  [too small]\n";
      }
      else if ((size < result) && (space_in_use - size < max_space - required_space)) {
        cout << "  [smallest so far]\n";
        result = size;
      }
      else {
        cout << "\n";
      }
    }
    return result;
  }

};
