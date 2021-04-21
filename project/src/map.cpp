#include "map.h"

Map::Map(const std::string& file_name) {
    std::ifstream file(file_name);

    if (!file) {
        throw std::runtime_error("Could not open file");
    }

    file >> rows >> cols;

    std::string object;
    size_t x, y, type_object;

    while (!file.eof()) {
        x = 0;
        y = 0;
        file >> x >> y;
        if (x == 0 && y == 0) {
            break;
        }

        type_object = 0;
        file >> object;

        if (object == "wolf") {
            type_object = wolf_found;
        } else if (object == "dog") {
            type_object = dog_found;
        } else if (object == "rat") {
            type_object = rat_found;
        } else if (object == "armor") {
            type_object = armor_found;
        } else if (object == "helmet") {
            type_object = helmet_found;
        } else if (object == "shield") {
            type_object = shield_found;
        } else if (object == "pants") {
            type_object = pants_found;
        } else if (object == "T-Shirt") {
            type_object = T_Shirt_found;
        }

        field.push_back({x, y, type_object});
    }
    file.close();
}

void Map::check_status(Move& move) const {
    if (pos.x > 0) {
        move.left = true;
        std::cout << "\n * move left";
    } else {
        move.left = false;
    }

    if (pos.x + 1 < rows) {
        move.right = true;
        std::cout << "\n * move right";
    } else {
        move.right = false;
    }

    if (pos.y > 0) {
        move.down = true;
        std::cout << "\n * move down";
    } else {
        move.down = false;
    }

    if (pos.y + 1 < cols) {
        move.up = true;
        std::cout << "\n * move up";
    } else {
        move.up = false;
    }

    if (!(move.down || move.up || move.right || move.left)) {
        std::cout << "\n";
    }
}

void Map::delete_object() {
    for (size_t i = 0; i < field.size(); i++) {
        if (pos.x == field[i].x && pos.y == field[i].y) {
            field.erase(field.begin() + i);
        }
    }
}

int Map::find_object() const {
    if (pos.x > rows || pos.y > cols) {
        return 0;
    }

    for (size_t i = 0; i < field.size(); i++) {
        if (field[i].x == pos.x && field[i].y == pos.y) {
            return field[i].type_object;
        }
    }

    return 0;
}

size_t Map::get_pos_x() const {
    return pos.x;
}

size_t Map::get_pos_y() const {
    return pos.y;
}

void Map::set_pos_x(size_t value) {
    pos.x = value;
}

void Map::set_pos_y(size_t value) {
    pos.y = value;
}
