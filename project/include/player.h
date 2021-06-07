#pragma once

#include <string>
#include <map>
#include <iostream>

#include "map.h"
#include "wolf.h"
#include "dog.h"
#include "rat.h"

class Player {
 public:
    void kick_enemy(Enemy& enemy, ObjectFound& enemy_type, bool& enemy_live);
    void pick_clothes(const std::string& command, std::map<std::string, Clothes>& clothes, ObjectFound&object_type);
    void throw_clothes(const std::string& command, std::map<std::string, Clothes>& clothes);

    void move_and_check(
        Move& move, const std::string& command, ObjectFound& object_type, Enemy& enemy,
        std::map<std::string, Clothes>& clothes, Map& map);

    void print_moved(ObjectFound& object_type, const Map& map) const;

    ClothesArmor get_all_armor() const;
    int get_health() const;
    void set_health(int new_health);

 private:
    ClothesWeight weight = ClothesWeight::none_weight;
    ClothesArmor all_armor = ClothesArmor::none_armor;
    int health = 100;
    int damage = 1;
};
