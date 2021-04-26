#include "clothes.h"

Clothes::Clothes(ClothesWeight weight, ClothesArmor armor) : weight(weight), armor(armor) { }

bool Clothes::get_found_clothes() const {
    return found_clothes;
}

bool Clothes::get_put_on_player() const {
    return put_on_player;
}

void Clothes::set_put_on_player(bool value) {
    put_on_player = value;
}

void Clothes::set_found_clothes(bool value) {
    found_clothes = value;
}

ClothesWeight Clothes::get_weight() const {
    return weight;
}

ClothesArmor Clothes::get_armor() const {
    return armor;
}

void Clothes::print_status_clothes(const std::map<std::string, Clothes>& clothes) const {
    for (const auto& [name, characteristic] : clothes) {
        if (characteristic.found_clothes && !characteristic.put_on_player) {
            std::cout << "\n * pick " << name;
        }
    }

    for (const auto& [name, characteristic] : clothes) {
        if (characteristic.put_on_player) {
            std::cout << "\n * throw " << name;
        }
    }
}
