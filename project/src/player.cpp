#include "player.h"

void Player::kick_enemy(Enemy& enemy, size_t& enemy_type, bool& is_enemy_alive) {
    enemy.set_health(enemy.get_health() - damage);

    if (enemy.get_health() <= 0) {
        std::cout << "\nenemy killed\n";
        is_enemy_alive = false;
        enemy.update_health();
        enemy_type = 0;
        return;
    }

    health = all_armor >= (enemy.get_damage_weapon() + enemy.get_damage_base()) ? health - 1 :
        health + all_armor - (enemy.get_damage_weapon() + enemy.get_damage_base());

    if (health > 0) {
        std::cout << "\nenemy kicked. Enemy hp: " << enemy.get_health() <<'\n';
    }
}

void Player::pick_clothes(const std::string& command, std::map<std::string, Clothes>& clothes, size_t& object_type) {
    for (auto& [name, characteristic] : clothes) {
        if (characteristic.get_found_clothes() && command.find(name) > 0 &&
            !characteristic.get_put_on_player() && (weight + characteristic.get_weight()) <= 20) {
            std::cout << "\nclothes worn\n";

            characteristic.set_put_on_player(true);
            characteristic.set_found_clothes(false);

            weight += characteristic.get_weight();
            all_armor += characteristic.get_armor();

            object_type = 0;
            return;
        }
    }
}

void Player::throw_clothes(const std::string& command, std::map<std::string, Clothes>& clothes) {
    for (auto& [name, characteristic] : clothes) {
        if (characteristic.get_put_on_player() && command.find(name) != std::string::npos) {
            std::cout << "\nthe " << name << " is thrown out\n";

            characteristic.set_put_on_player(false);

            all_armor -= characteristic.get_armor();
            weight -= characteristic.get_weight();

            return;
        }
    }
}

void Player::move_and_check(Move &move, const std::string &command, size_t &object_type, Enemy &enemy,
    std::map<std::string, Clothes> &clothes, Map& map) {
        if (command == "move left" && move.left) {
            map.set_pos_x(map.get_pos_x() - 1);
            print_moved(object_type, map);
        }

        if (command == "move right" && move.right) {
            map.set_pos_x(map.get_pos_x() + 1);
            print_moved(object_type, map);
        }

        if (command == "move down" && move.down) {
            map.set_pos_y(map.get_pos_y() - 1);
            print_moved(object_type, map);
        }

        if (command == "move up" && move.up) {
            map.set_pos_y(map.get_pos_y() + 1);
            print_moved(object_type, map);
        }

        Enemy wolf("wolf");
        Enemy dog("dog");
        Enemy rat("rat");

        switch (object_type) {
            case wolf_found:
                enemy = wolf;
                enemy.print_find_enemy();
                return;

            case dog_found:
                enemy = dog;
                enemy.print_find_enemy();
                return;

            case rat_found:
                enemy = rat;
                enemy.print_find_enemy();
                return;

            case armor_found:
                std::cout << "\narmor found\n";
                clothes["armor"].set_found_clothes(true);
                return;

            case helmet_found:
                std::cout << "\nhelmet found\n";
                clothes["helmet"].set_found_clothes(true);
                return;

            case shield_found:
                std::cout << "\nshield found\n";
                clothes["shield"].set_found_clothes(true);
                return;

            case pants_found:
                std::cout << "\npants found\n";
                clothes["pants"].set_found_clothes(true);
                return;

            case T_Shirt_found:
                std::cout << "\nT-Shirt found\n";
                clothes["T-Shirt"].set_found_clothes(true);
                return;

            default:
                return;
        }
}

void Player::print_moved(size_t &object_type, const Map& map) const {
    object_type = map.find_object();

    if (object_type == 0) {
        std::cout << "\nmoved\n";
        return;
    }
}

int Player::get_all_armor() const {
    return all_armor;
}

int Player::get_health() const {
    return health;
}

void Player::set_health(int new_health) {
    health = new_health;
}
