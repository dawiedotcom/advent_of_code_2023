#pragma once

#include <unordered_map>
#include <vector>

#include "lib.h"
#include "parser.h"
#include "interval.h"

using interval = interval_<int>;
using items_t = std::unordered_map<char, interval>;

struct rule_t {
  string dest;
  //string pred;
  char field, comp;
  int value;

  rule_t ()
    : rule_t("", ' ', ' ', 0)
  {}

  rule_t (const string& dest, char field, char comp, int value)
    : dest(dest)
    , field(field)
    , comp(comp)
    , value(value)
  {}

  string apply(const string& rule_name, const unordered_map<char, int>& part) {
    if (field == '.' || is_match(part)) {
      //SHOW(dest, field);
      return dest;
    }
    return rule_name;
  }

  tuple<string, items_t, items_t> apply(items_t& in) {//;, items_t& pass, items_t& fail) {
    //pass.clear();
    //fail.clear();
    items_t pass, fail;
    if ((comp == ' ')
        || (value < in.at(field).start && comp == '>')
        || (value >= in.at(field).end && comp == '<')) {
      // All of in passes
      for (auto [k, v] : in) {
        pass[k] = in.at(k);
        //SHOW(k, in.at(k), pass[k]);
      }
      return {dest, pass, fail};
    }
    else if ((value < in.at(field).start && comp == '<')
             || (value >= in.at(field).end && comp == '>')) {
      // all of in fails
      for (auto [k, v] : in)
        fail[k] = in.at(k);
      return {dest, pass, fail};
    }

    for (auto [k, v] : in) {
      if (k==field) {
        if (comp == '<') {
          auto [lower, upper] = in.at(k).split(value);
          pass[k] = lower;
          fail[k] = upper;
        }
        else if (comp == '>') {
          auto [lower, upper] = in.at(k).split(value+1);
          pass[k] = upper;
          fail[k] = lower;
        }
      }
      else {
        pass[k] = in.at(k);
        fail[k] = in.at(k);
      }
    }
    return {dest, pass, fail};
  }

  bool is_match(const unordered_map<char, int>& part) {
    //SHOW(dest, field, part.at(field), comp, value, comp=='<',(part.at(field) < value));
    if (field == '.')
      return true;

    return (comp == '<') ? (part.at(field) < value) : (part.at(field) > value);
  }
};

struct sorter_t {
  long long sum;
  unordered_map<string, vector<rule_t>> rules;
  vector<string> rule_order;

  sorter_t()
    : sum(0)
  { }
  
  void input(const string& line) {
    parser parse(line);
    string rule_name = parse.next_word();
    rules[rule_name] = {};
    rule_order.push_back(rule_name);
    int i=0;
    while (parse.top() != '}') {
      //string rule = parse.with(R"(^[a-z0-9\<\>:]+)");
      string field = ".", comp=" ";
      int value=0;
      if (parse.match(R"([xams][<>]\d+:)")) {
        field = parse.with(R"([axms])");
        comp = parse.with(R"([<>])");
        value = parse.next_int();
        if (parse.top() == ':') parse.step(1);
      }
      string dest = parse.next_word();
      //SHOW(rule_name, field, comp, value, dest, parse);
      if (parse.top() == ',') parse.step(1);
      rules[rule_name].emplace_back(dest, field[0], comp[0], value);
    }
    //cout << endl;
  }

  void parse_item(const string& line, unordered_map<char, int>& result) {
    parser parse(line);
    result['x'] = parse.next_uint();
    result['m'] = parse.next_uint();
    result['a'] = parse.next_uint();
    result['s'] = parse.next_uint();
  }

  void sort(const string& line) {
    unordered_map<char, int> item;
    parse_item(line, item);

    string rule = "in", next_rule;
    int step=0;
    while (rule != "R" && rule != "A") {
      //SHOW(rule, step, rules[rule].size());
      next_rule = rules[rule][step].apply(rule, item);
      //SHOW(rule, step, next_rule); //rules[rule].size());
      if (next_rule == rule)
        step++;
      else {
        step = 0;
        rule = next_rule;
      }
    }

    if (rule == "A")
      for (auto [_, value] : item)
        sum += value;
  }

  unsigned long long get_all_combinations() {
    items_t all_items;
    all_items.emplace(make_pair<char, interval>('x', {1, 4001}));
    all_items.emplace(make_pair<char, interval>('m', {1, 4001}));
    all_items.emplace(make_pair<char, interval>('a', {1, 4001}));
    all_items.emplace(make_pair<char, interval>('s', {1, 4001}));
    assert(all_items['x'].length() == 4000);

    vector<tuple<string, int, items_t>> rule_items;
    rule_items.emplace_back(string("in"), 0, all_items);

    int step=0;
    unsigned long long result = 0;
    while (rule_items.size() > 0) {
      auto [rule, step, items] = rule_items.back();
      rule_items.pop_back();

      auto [pass_next, pass, fail] = rules[rule][step].apply(items);

      if (pass_next == "A") {
        size_t prod = 1;
        for (auto [prop, interval] : pass) 
          prod *= interval.length();
        result += prod;
      }
      else if (pass_next != "R" && pass.size() > 0)
        rule_items.emplace_back(pass_next, 0, pass);

      if (fail.size() > 0)
        rule_items.emplace_back(rule, step+1, fail);
    }

    return result;
  } 
};

