#pragma once

#include <string>
#include <map>
#include <iostream>

#include "map.h"

class Player {
 public:
    Player() = default;
    ~Player() = default;

    void kick_enemy(Enemy& enemy, size_t& enemy_type, bool& enemy_live);
    void pick_clothes(const std::string& command, std::map<std::string, Clothes>& clothes, size_t&object_type);
    void throw_clothes(const std::string& command, std::map<std::string, Clothes>& clothes);

    void move_and_check(
        Move& move, const std::string& command, size_t& object_type, Enemy& enemy,
        std::map<std::string, Clothes>& clothes, Map& map);

    void print_moved(size_t& object_type, const Map& map) const;

    int get_all_armor() const;
    int get_health() const;
    void set_health(int new_health);

 private:
    int weight = 0;
    int all_armor = 0;
    int health = 100;
    int damage = 1;
};
