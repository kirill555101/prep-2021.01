#include "tests.h"

int test_write_to_file(const char *test_filename) {
  FILE *test_file = fopen(test_filename, "w");
  Record expected_client_data = {
    1, "Ivan", "Ivanov", "Moscow",
    "8919000111", 300, 500, 100
  };

  write_to_file(test_file, &expected_client_data);
  rewind(test_file);
  fseek(test_file, 0, SEEK_END);
  if (!ftell(test_file)) {
    fclose(test_file);
    return 1;
  }

  fclose(test_file);
  return 0;
}

int test_read_from_file(const char *test_filename) {
  FILE *test_file = fopen(test_filename, "r");
  Record got_client_data = {0};

  if (read_from_file(test_file, &got_client_data) != 0) {
    fclose(test_file);
    return 2;
  }

  fclose(test_file);
  return 0;
}

int test_structs_equal(Record first_client_data, Record second_client_data) {
  if (structs_equal(first_client_data, second_client_data) != 0) {
    return 3;
  }

  return 0;
}
