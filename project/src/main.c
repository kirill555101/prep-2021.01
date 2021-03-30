#include <stdlib.h>
#include <stdio.h>

#include "parser.h"


int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];

    if (parse_email(path_to_eml) != EXIT_SUCCESS) {
        fprintf(stderr, "FAILURE parse_email");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

