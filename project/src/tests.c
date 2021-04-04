#include "tests.h"

int test_write_record_to_file(const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  Record record = {
    1, "Ivan", "Ivanov", "Moscow",
    "8919000111", 300, 500, 100
  };
  write_record_to_file(file, &record);
  fseek(file, 0, SEEK_END);
  if (!ftell(file)) {
    fclose(file);
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int test_read_record_from_file(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  Record got_client_data = {0};
  if (read_record_from_file(file, &got_client_data) != EXIT_SUCCESS) {
    fclose(file);
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int test_write_transaction_to_file(const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  Transaction transaction = {
    1, 500
  };
  write_transaction_to_file(file, &transaction);
  fseek(file, 0, SEEK_END);
  if (!ftell(file)) {
    fclose(file);
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int test_read_transaction_from_file(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  Transaction transaction = {0};
  if (read_transaction_from_file(file, &transaction) != EXIT_SUCCESS) {
    fclose(file);
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int test_structs_equal() {
  Record first_struct = {
    1, "Ivan", "Ivanov", "Moscow",
    "8919000111", 300, 500, 100
  };
  Record second_struct = first_struct;

  if (structs_equal(first_struct, second_struct) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
