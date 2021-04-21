#include "enemy.h"

Enemy::Enemy(const std::string& name) : name(name) {
    if (name == "wolf") {
        health = wolf_health;
        damage_base = wolf_damage;
    }

    if (name == "dog") {
        health = dog_health;
        damage_base = dog_damage;
    }

    if (name == "rat") {
        health = rat_health;
        damage_base = rat_damage;
    }
}

void Enemy::print_find_enemy() const {
    std::cout <<'\n'<< name << " found, " << get_health() << " hp\n";
}

void Enemy::update_health() {
    if (name == "wolf") {
        set_health(wolf_health);
    } else if (name == "dog") {
        set_health(dog_health);
    } else {
        set_health(rat_health);
    }
}

int Enemy::get_damage_base() const {
    return damage_base;
}

int Enemy::get_health() const {
    return health;
}

int Enemy::get_damage_weapon() const {
    return damage_weapon;
}

void Enemy::set_health(int new_health) {
    health = new_health;
}
