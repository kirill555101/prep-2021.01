#include "wolf.h"

Wolf::Wolf() : Enemy("wolf") {
    health = EnemyHealth::wolf_health;
    damage_base = EnemyDamage::wolf_damage;
}

void Wolf::update_health() {
    set_health(EnemyHealth::wolf_health);
}
