#include "dog.h"

Dog::Dog() : Enemy("dog") {
    health = EnemyHealth::dog_health;
    damage_base = EnemyDamage::dog_damage;
}

void Dog::update_health() {
    set_health(EnemyHealth::dog_health);
}
