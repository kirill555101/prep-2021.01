#pragma once

#include <string>
#include <map>
#include <iostream>

#include "player.h"

class Game {
 public:
    Game() = default;
    explicit Game(const std::string& file_name, bool stage2);

    void run();

 private:
    bool is_enemy_alive;
    void print_status_clothes(const std::map<std::string, Clothes>& clothes) const;
    void init_clothes(std::map<std::string, Clothes>& clothes);
    void print_player_stage1() const;
    void print_player_stage2() const;

    Move& print_supported_actions(
        ObjectFound object_type, Move& move, const std::map<std::string, Clothes>& clothes,
        bool &enemy_live) const;

    void check_command(
        Enemy& enemy, const std::string& command, ObjectFound& object_type,
        Move& move, std::map<std::string, Clothes>& clothes);

    Map map;
    Player player;
    bool is_stage2;
};
