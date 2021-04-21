#pragma once

#include <string>
#include <iostream>

enum EnemyHealth {
    wolf_health = 6,
    dog_health = 3,
    rat_health = 2
};

enum EnemyDamage {
    wolf_damage = 10,
    dog_damage = 5,
    rat_damage = 3
};

class Enemy {
 public:
    Enemy() = default;
    explicit Enemy(const std::string& name);
    ~Enemy() = default;

    void update_health();
    void print_find_enemy() const;

    int get_damage_base() const;
    int get_health() const;
    int get_damage_weapon() const;
    void set_health(int new_health);

 protected:
    std::string name;
    int damage_base = 0;
    int health = 0;
    int damage_weapon = 1;
};
