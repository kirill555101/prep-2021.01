#pragma once

#include <string>
#include <iostream>

enum class EnemyHealth {
    none_health = 0,
    wolf_health = 6,
    dog_health = 3,
    rat_health = 2
};

enum class EnemyDamage {
    none_damage = 0,
    wolf_damage = 10,
    dog_damage = 5,
    rat_damage = 3
};

class Enemy {
 public:
    Enemy() = default;
    explicit Enemy(const std::string& name);

    virtual void update_health();
    void print_find_enemy() const;

    EnemyDamage get_damage_base() const;
    EnemyHealth get_health() const;
    int get_damage_weapon() const;
    void set_health(EnemyHealth new_health);

 protected:
    std::string name;
    EnemyDamage damage_base = EnemyDamage::none_damage;
    EnemyHealth health = EnemyHealth::none_health;
    int damage_weapon = 1;
};
