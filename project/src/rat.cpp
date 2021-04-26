#include "rat.h"

Rat::Rat() : Enemy("rat") {
    health = EnemyHealth::rat_health;
    damage_base = EnemyDamage::rat_damage;
}

void Rat::update_health() {
    set_health(EnemyHealth::rat_health);
}
