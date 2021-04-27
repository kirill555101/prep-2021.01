#include <stdlib.h>
#include <stdio.h>

#include "parser.h"


int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];
    data_t data = {0};
    if (parse_email(path_to_eml, &data) != EXIT_SUCCESS) {
        fprintf(stderr, "FAILURE parse_email\n");
        return EXIT_FAILURE;
    }

    print_data(data);
    if (free_data(&data) != EXIT_SUCCESS) {
        fprintf(stderr, "FAILURE free_data\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
