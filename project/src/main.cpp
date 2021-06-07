#include <cstring>
#include <iostream>

#include "game.h"

int main(int argc, const char** argv) {
    int map = -1;
    bool stage2 = false;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--view-armor") == 0) {
            stage2 = true;
        }

        if (strcmp(argv[i], "--map") == 0) {
            map = i;
        }
    }

    if (map == -1) {
        std::cerr << "No map is input\n";
        return -1;
    }

    Game game(argv[map + 1], stage2);
    game.run();
    return 0;
}
