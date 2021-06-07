#pragma once

#include "enemy.h"

class Wolf : public Enemy {
 public:
    Wolf();

    void update_health() override;
};
