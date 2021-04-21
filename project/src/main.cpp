#include <cstring>

#include "game.h"

int main(int argc, const char** argv) {
    int map;
    bool stage2 = false;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--view-armor") == 0) {
            stage2 = true;
        }

        if (strcmp(argv[i], "--map") == 0) {
            map = i;
        }
    }

    Game game(argv[map + 1], stage2);
    game.run();
    return 0;
}

