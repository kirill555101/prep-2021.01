#include "tests.h"

int test_write_to_file(FILE *test_file) {
  if (test_file == NULL) {
    return FILE_ERROR;
  }
  Record expected_client_data = {
    1, "Ivan", "Ivanov", "Moscow",
    "8919000111", 300, 500, 100
  };

  write_record_to_file(test_file, &expected_client_data);
  rewind(test_file);
  fseek(test_file, 0, SEEK_END);
  if (!ftell(test_file)) {
    return WRITE_ERROR;
  }

  return 0;
}

int test_read_from_file(FILE *test_file) {
  if (test_file == NULL) {
    return FILE_ERROR;
  }
  Record got_client_data = {0};

  if (read_record_from_file(test_file, &got_client_data) != 0) {
    return READ_ERROR;
  }

  return 0;
}

int test_structs_equal(Record first_client_data, Record second_client_data) {
  if (structs_equal(first_client_data, second_client_data) != 0) {
    return EQUAL_ERROR;
  }

  return 0;
}
