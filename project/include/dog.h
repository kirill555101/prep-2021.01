#pragma once

#include "enemy.h"

class Dog : public Enemy {
 public:
    Dog();

    void update_health() override;
};
