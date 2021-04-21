#pragma once

#include <map>
#include <string>
#include <iostream>

enum ClothesWeight {
    armor_weight = 3,
    helmet_weight = 2,
    shield_weight = 7,
    pants_weight = 1,
    T_Shirt_weight = 1
};

enum ClothesArmor {
    armor_armor = 3,
    helmet_armor = 3,
    shield_armor = 5,
    pants_armor = 1,
    T_Shirt_armor = 1
};

class Clothes {
 public:
    Clothes() = default;
    Clothes(int weight, int armor);
    ~Clothes() = default;

    void set_found_clothes(bool value);
    void set_put_on_player(bool value);

    bool get_found_clothes() const;
    bool get_put_on_player() const;

    int get_weight() const;
    int get_armor() const;

    void print_status_clothes(const std::map<std::string, Clothes>& clothes) const;

 private:
    bool put_on_player = false;
    bool found_clothes = false;
    int weight;
    int armor;
};
