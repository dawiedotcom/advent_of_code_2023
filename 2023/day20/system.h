#pragma once

#include <vector>
#include <unordered_map>
#include <queue>

#include "lib.h"
#include "parser.h"
#include "number_theory.h"

enum pulse_t {
  LOW,
  HIGH
};

using q_t = queue<tuple<string, string, pulse_t>>;

struct module_t {
  string name;
  bool flip_flop_state;
  vector<string> outputs;
  unordered_map<string, pulse_t> conj_state;

  module_t()
    : name("")
    , flip_flop_state(false)
  {}

  module_t(const string& name)
    : name(name)
    , flip_flop_state(false)
  {}

  void add_input(const string& input) {
    conj_state[input] = LOW;
  }

  void handle(const string& sender, const pulse_t pulse, q_t& q) {
    if (name == "broadcaster") {
      for (const auto& dest : outputs)
        q.emplace(name, dest, pulse);
      return;
    }

    // Conjuction
    if (name[0] == '&') {
      conj_state[sender] = pulse;

      bool all_high = true;
      for (auto [sender, pulse] : conj_state)
        all_high &= (pulse == HIGH);

      for (const auto& dest : outputs)
        q.emplace(name.substr(1), dest, all_high ? LOW : HIGH);

      return;
    }

    // Flip-flop
    if (name[0] == '%') {
      if (pulse == HIGH)
        return;

      flip_flop_state = !flip_flop_state;

      for (const auto& dest : outputs)
        q.emplace(name.substr(1), dest, flip_flop_state ? HIGH : LOW);
      
      return;
    }
    //SHOW(name);
    //assert(false);
  }
};

struct system_t {
  unordered_map<string, module_t> modules;
  
  system_t()
  {}

  void input(const string& line) {
    parser parse(line);
    string name = parse.with(R"([%&a-z]+)");
    string dict_name = (name[0] == '&' || name[0] == '%') ? name.substr(1) : name;
    modules.emplace(dict_name, name);
    parse.to_token("->");
    while(!parse.done())
      modules[dict_name].outputs.emplace_back(parse.next_word());
  }

  void init_inputs() {
    for (auto [name, mod] : modules) {
      for (auto const& output : mod.outputs) {
        if (!modules.contains(output))
          modules.emplace(output, output);
        modules.at(output).add_input(name);
      }
    }
  }

  size_t run(bool part1) {
    bool part2 = !part1;

    q_t q;

    size_t lows=0, highs=0;
    init_inputs();
    const size_t N_STEPS = part2 && modules.contains("rx") ? 10000 : 1000;
    unordered_map<string, int> cycle_count;
    for (size_t i=0; i<N_STEPS; i++) {
      q.emplace("button", "broadcaster", LOW);
      size_t nlows=0, nhighs=0;

      while (!q.empty()) {
        auto [sender, dest, pulse] = q.front();
        q.pop();

        //if (dest == "rx" && pulse == LOW) {
        if (part2 && modules["rx"].conj_state.contains(dest) && pulse==HIGH) {
          if (!cycle_count.contains(sender))
            cycle_count[sender] = i+1;
          //cycle_count[sender] = i - cycle_count[sender];
          SHOW(i, sender, dest, cycle_count[sender]);
          if (cycle_count.size() == modules[dest].conj_state.size()) {
            vector<size_t> nums;
            for (auto [_, c] : cycle_count) 
              nums.push_back(c);

            return lcm(nums);
          }
        }

        //cout << sender << " [" << pulse << "] -> " << dest << endl;

        nlows += pulse == LOW ? 1 : 0;
        nhighs += pulse == HIGH ? 1 : 0;

        modules[dest].handle(sender, pulse, q);
      }
      //SHOW(nlows, nhighs);
      if (i<1000) {
        lows += nlows;
        highs += nhighs;
        if (i==1000-1)
          cout << "Part1: " << (highs * lows) << endl;
      }
    }

    return lows*highs;
  }

  size_t count_pulses() {
    return run(true);
  }

  size_t count_buttons_to_rx() {
    return run(false);
  }
};

