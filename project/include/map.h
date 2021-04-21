#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "clothes.h"
#include "enemy.h"

enum ObjectFound {
    wolf_found = 1,
    dog_found,
    rat_found,
    armor_found,
    helmet_found,
    shield_found,
    pants_found,
    T_Shirt_found
};

struct Field {
    size_t x;
    size_t y;
    size_t type_object;
};

struct PlayerPosition {
    size_t x = 0;
    size_t y = 0;
};

struct Move {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
};

class Map {
 public:
    Map() = default;
    explicit Map(const std::string& file_name);
    ~Map() = default;

    void check_status(Move& move) const;
    void delete_object();
    int find_object() const;

    size_t get_pos_x() const;
    size_t get_pos_y() const;

    void set_pos_x(size_t value);
    void set_pos_y(size_t value);

 private:
    std::vector<Field> field;
    size_t rows = 0;
    size_t cols = 0;
    PlayerPosition pos;
};
