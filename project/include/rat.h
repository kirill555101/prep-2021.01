#pragma once

#include "enemy.h"

class Rat : public Enemy {
 public:
    Rat();

    void update_health() override;
};
