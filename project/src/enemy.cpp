#include "enemy.h"

Enemy::Enemy(const std::string& name) : name(name) { }

void Enemy::print_find_enemy() const {
    std::cout <<'\n'<< name << " found, " << int(get_health()) << " hp\n";
}

void Enemy::update_health() { }

EnemyDamage Enemy::get_damage_base() const {
    return damage_base;
}

EnemyHealth Enemy::get_health() const {
    return health;
}

int Enemy::get_damage_weapon() const {
    return damage_weapon;
}

void Enemy::set_health(EnemyHealth new_health) {
    health = new_health;
}
