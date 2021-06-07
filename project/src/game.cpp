#include "game.h"

Game::Game(const std::string &file_name, bool is_stage2)
    : is_enemy_alive(false), map(file_name), player(), is_stage2(is_stage2) { }

void Game::check_command(Enemy& enemy, const std::string& command, ObjectFound& object_type,
    Move& move, std::map<std::string, Clothes> &clothes) {
        if (command == "move left" || command == "move right" ||
            command == "move up" || command == "move down") {
                map.delete_object();

                for (auto& [name, characteristic] : clothes) {
                    characteristic.set_found_clothes(false);
                }

                player.move_and_check(move, command, object_type, enemy, clothes, map);
                return;
        }

        if (command == "kick enemy" && is_enemy_alive) {
            player.kick_enemy(enemy, object_type, is_enemy_alive);
            return;
        }

        if (command.compare(0, 4, "pick") == 0) {
            player.pick_clothes(command, clothes, object_type);
            return;
        }

        if (command.compare(0, 5, "throw") == 0) {
            player.throw_clothes(command, clothes);
        }
}

Move& Game::print_supported_actions(ObjectFound object_type, Move& move,
    const std::map<std::string, Clothes>& clothes, bool& is_enemy_alive) const {
        std::cout << "Supported actions:";

        if (object_type >= ObjectFound::wolf_found && object_type <= ObjectFound::rat_found) {
            is_enemy_alive = true;
            std::cout << "\n * kick enemy";
        } else {
            map.recalculate_move_directions(move);
            if (object_type > ObjectFound::rat_found) {
                print_status_clothes(clothes);
            }
        }
        return move;
}


void Game::init_clothes(std::map<std::string, Clothes>& clothes) {
    clothes["armor"] = {ClothesWeight::armor_weight, ClothesArmor::armor_armor};
    clothes["helmet"] = {ClothesWeight::helmet_weight, ClothesArmor::helmet_armor};
    clothes["shield"] = {ClothesWeight::shield_weight, ClothesArmor::shield_armor};
    clothes["pants"] = {ClothesWeight::pants_weight, ClothesArmor::pants_armor};
    clothes["T-Shirt"] = {ClothesWeight::T_Shirt_weight, ClothesArmor::T_Shirt_armor};
}

void Game::run() {
    Enemy enemy;
    ObjectFound object_type = ObjectFound::none_found;
    Move move;
    std::string command;

    std::map<std::string, Clothes> clothes;
    init_clothes(clothes);

    while (player.get_health() > 0) {
        move = print_supported_actions(object_type, move, clothes, is_enemy_alive);

        if (is_stage2) {
            print_player_stage2();
        } else {
            print_player_stage1();
        }

        if (!getline(std::cin, command)) {
            return;
        }

        check_command(enemy, command, object_type, move, clothes);
    }

    std::cout << "\nplayer died\n";
}

void Game::print_player_stage1() const {
    std::cout <<'\n'<< map.get_pos_x() << " x " << map.get_pos_y() <<
                ", hp: " << player.get_health() << " > ";
}

void Game::print_player_stage2() const {
    std::cout <<'\n'<< map.get_pos_x() << " x " << map.get_pos_y() << ", hp: " << player.get_health() <<
                ", armor: " << int(player.get_all_armor()) << " > ";
}

void Game::print_status_clothes(const std::map<std::string, Clothes>& clothes) const {
    for (const auto& [name, characteristic] : clothes) {
        if (characteristic.get_found_clothes() && !characteristic.get_put_on_player()) {
            std::cout << "\n * pick " << name;
        }
    }

    for (const auto& [name, characteristic] : clothes) {
        if (characteristic.get_put_on_player()) {
            std::cout << "\n * throw " << name;
        }
    }
}
