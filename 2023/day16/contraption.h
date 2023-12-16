#pragma once

struct tile_t {
  char type;
  bool has_beam_e;
  bool has_beam_w;
  bool has_beam_n;
  bool has_beam_s;

  tile_t(char c)
    : type(c)
    , has_beam_e(false)
    , has_beam_w(false)
    , has_beam_n(false)
    , has_beam_s(false)
  {}

  void set_beam(string beam_dir) {
    if (beam_dir == "N") has_beam_n = true;
    if (beam_dir == "S") has_beam_s = true;
    if (beam_dir == "E") has_beam_e = true;
    if (beam_dir == "W") has_beam_w = true;
  }
  constexpr bool has_beam() const {
    return has_beam_e
      || has_beam_n
      || has_beam_s
      || has_beam_w
      ;
  }
  constexpr bool has_beam(string beam_dir) const {
    if (beam_dir == "N") return has_beam_n;
    if (beam_dir == "S") return has_beam_s;
    if (beam_dir == "E") return has_beam_e;
    if (beam_dir == "W") return has_beam_w;
    return false;
  }

  inline void reset() {
    has_beam_e = has_beam_n = has_beam_s = has_beam_w = false;
  }
};

ostream& operator<<(ostream& out, const tile_t& tile) {
  //out << tile.type;
  out << (tile.has_beam() ? '#' : '.'); //
  return out;
}

struct beam_t {
  int x, y;
  string dir;

  beam_t()
    : x(0)
    , y(0)
    , dir("E")
  { }

  beam_t(int x, int y, const string& dir)
    : x(x)
    , y(y)
    , dir(dir)
  { }

  beam_t(const beam_t& other)
    : x(other.x)
    , y(other.y)
    , dir(other.dir)
  {}
  
  void step() {
    if (dir == "N") y--;
    if (dir == "S") y++;
    if (dir == "E") x++;
    if (dir == "W") x--;
  }
  beam_t split() const {
    beam_t other;
    other.x = this->x;
    other.y = this->y;
    switch (dir[0]) {
    case 'N':
      other.dir = "S";
      break;
    case 'S':
      other.dir = "N";
      break;
    case 'W':
      other.dir = "E";
      break;
    case 'E':
      other.dir = "W";
      break;
    }
    return other;
  }
};

struct contraption_t {
  text_image<tile_t> room;
  
  contraption_t() {
    
  }

  void input(const string& line) {
    for (const char& c : line) {
      room.emplace_back(c);
    }
    
    room.start_new_row();
  }

  void run(int start_x, int start_y, const string& start_dir) {
    stack<beam_t> beams;
    beams.emplace(start_x, start_y, start_dir);
    //while (beams.size() > 0) {
    //SHOW(beams.size());
    while (!beams.empty()) {
      beam_t beam(beams.top());
      beams.pop();

      
      while (beam.x > -1
             && beam.x < room.X()
             && beam.y > -1
             && beam.y < room.Y()) {
        tile_t& tile = room[beam.x, beam.y];

        if (tile.has_beam(beam.dir))
          break;
        tile.set_beam(beam.dir);

        //SHOW(beams.size(), beam.x, beam.y, beam.dir, tile.type);
        //cout << room;

        if (tile.type == '|' && (beam.dir == "E" || beam.dir == "W")){
          beam.dir = "N";
          beams.push(beam.split());
        }
        else if (tile.type == '-' && (beam.dir == "N" || beam.dir == "S")){
          beam.dir = "W";
          beams.push(beam.split());
        }

        else if (tile.type == '/') {
          if (beam.dir == "N") beam.dir = "E";
          else if (beam.dir == "S") beam.dir = "W";
          else if (beam.dir == "E") beam.dir = "N";
          else if (beam.dir == "W") beam.dir = "S";
        } 
        else if (tile.type == '\\') {
          if (beam.dir == "N") beam.dir = "W";
          else if (beam.dir == "S") beam.dir = "E";
          else if (beam.dir == "E") beam.dir = "S";
          else if (beam.dir == "W") beam.dir = "N";
        } 

        //if (tile.has_beam(beam.dir))
        //  break;
        //tile.set_beam(beam.dir);


        beam.step();
      }
    }
  }

  void reset() {
    for (size_t x=0; x<room.X(); x++)
      for (size_t y=0; y<room.Y(); y++)
        room[x, y].reset();
  }

  size_t count_beam(int start_x, int start_y, const string& dir) {
    reset();
    run(start_x, start_y, dir);
    size_t result = 0;
    for (size_t x=0; x<room.X(); x++) {
      for (size_t y=0; y<room.Y(); y++) {
        if (room[x, y].has_beam()) result++;
      }
    }
    return result;
  }

  size_t find_best_start() {
    size_t best=0, result;
    for (size_t x=0; x<room.X(); x++) {
      for (size_t y=0; y<room.Y(); y+=room.Y()-1) {
        result = count_beam(x, y, (y==0) ? "S" : "N");
        best = max(best, result);
      }
    }

    for (size_t x=0; x<room.X(); x+=room.X()-1) {
      for (size_t y=0; y<room.Y(); y++) {
        result = count_beam(x, y, (x==0) ? "E" : "W");
        best = max(best, result);
      }
    }
    return best;
  }
};

