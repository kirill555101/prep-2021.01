#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "filename was not found\n");
        return EXIT_FAILURE;
    }

    const char* path_to_eml = argv[1];

    FILE* file = fopen(path_to_eml, "r");
    if (file == NULL) {
        fprintf(stderr, "file %s was not found\n", path_to_eml);
        return EXIT_FAILURE;
    }

    const char* str = parse_email(file);
    fclose(file);

    if (str == NULL) {
        fprintf(stderr, "parse_email returned NULL\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", str);
    free((char*)str);
    return EXIT_SUCCESS;
}
