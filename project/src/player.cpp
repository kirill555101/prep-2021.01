#include "player.h"

void Player::kick_enemy(Enemy& enemy, ObjectFound& enemy_type, bool& is_enemy_alive) {
    enemy.set_health(EnemyHealth(static_cast<int>(enemy.get_health()) - damage));

    if (static_cast<int>(enemy.get_health()) <= 0) {
        std::cout << "\nenemy killed\n";
        is_enemy_alive = false;
        enemy.update_health();
        enemy_type = ObjectFound::none_found;
        return;
    }

    health = static_cast<int>(all_armor) >= (enemy.get_damage_weapon() + static_cast<int>(enemy.get_damage_base())) ? health - 1 :
        health + static_cast<int>(all_armor) - (enemy.get_damage_weapon() + static_cast<int>(enemy.get_damage_base()));

    if (health > 0) {
        std::cout << "\nenemy kicked. Enemy hp: " << static_cast<int>(enemy.get_health()) <<'\n';
    }
}

void Player::pick_clothes(const std::string& command, std::map<std::string, Clothes>& clothes, ObjectFound& object_type) {
    for (auto& [name, characteristic] : clothes) {
        if (characteristic.get_found_clothes() && command.find(name) > 0 && !characteristic.get_put_on_player()) {
            std::cout << "\nclothes worn\n";

            characteristic.set_put_on_player(true);
            characteristic.set_found_clothes(false);

            weight = ClothesWeight(static_cast<int>(weight) + static_cast<int>(characteristic.get_weight()));
            all_armor = ClothesArmor(static_cast<int>(all_armor) + static_cast<int>(characteristic.get_armor()));

            object_type = ObjectFound::none_found;
            return;
        }
    }
}

void Player::throw_clothes(const std::string& command, std::map<std::string, Clothes>& clothes) {
    for (auto& [name, characteristic] : clothes) {
        if (characteristic.get_put_on_player() && command.find(name) != std::string::npos) {
            std::cout << "\nthe " << name << " is thrown out\n";

            characteristic.set_put_on_player(false);

            all_armor = ClothesArmor(static_cast<int>(all_armor) - static_cast<int>(characteristic.get_armor()));
            weight = ClothesWeight(static_cast<int>(weight) - static_cast<int>(characteristic.get_weight()));

            return;
        }
    }
}

void Player::move_and_check(Move& move, const std::string& command, ObjectFound& object_type, Enemy& enemy,
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

        Wolf wolf;
        Dog dog;
        Rat rat;

        switch (object_type) {
            case ObjectFound::wolf_found:
                enemy = wolf;
                enemy.print_find_enemy();
                return;

            case ObjectFound::dog_found:
                enemy = dog;
                enemy.print_find_enemy();
                return;

            case ObjectFound::rat_found:
                enemy = rat;
                enemy.print_find_enemy();
                return;

            case ObjectFound::armor_found:
                std::cout << "\narmor found\n";
                clothes["armor"].set_found_clothes(true);
                return;

            case ObjectFound::helmet_found:
                std::cout << "\nhelmet found\n";
                clothes["helmet"].set_found_clothes(true);
                return;

            case ObjectFound::shield_found:
                std::cout << "\nshield found\n";
                clothes["shield"].set_found_clothes(true);
                return;

            case ObjectFound::pants_found:
                std::cout << "\npants found\n";
                clothes["pants"].set_found_clothes(true);
                return;

            case ObjectFound::T_Shirt_found:
                std::cout << "\nT-Shirt found\n";
                clothes["T-Shirt"].set_found_clothes(true);
                return;

            default:
                return;
        }
}

void Player::print_moved(ObjectFound& object_type, const Map& map) const {
    object_type = map.find_object();

    if (object_type == ObjectFound::none_found) {
        std::cout << "\nmoved\n";
        return;
    }
}

ClothesArmor Player::get_all_armor() const {
    return all_armor;
}

int Player::get_health() const {
    return health;
}

void Player::set_health(int new_health) {
    health = new_health;
}
