#include "tests.h"

int test_master_write(const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  Record record = {
    1, "Ivan", "Ivanov", "Moscow",
    "8919000111", 300, 500, 100
  };

  write_record_to_file(file, &record);
  if (freopen(filename, "r", stdin) == NULL) {
    return EXIT_FAILURE;
  }
  if (master_write(filename) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  fseek(file, 0, SEEK_END);
  if (!ftell(file)) {
    fclose(file);
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int test_transaction_write(const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    return EXIT_FAILURE;
  }
  Transaction transaction = {
    1, 500
  };

  write_transaction_to_file(file, &transaction);
  if (freopen(filename, "r", stdin) == NULL) {
    return EXIT_FAILURE;
  }
  if (transaction_write(filename) != EXIT_SUCCESS) {
    return EXIT_FAILURE;
  }

  fseek(file, 0, SEEK_END);
  if (!ftell(file)) {
    fclose(file);
    return EXIT_FAILURE;
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int test_blackrecord(const char* record_filename, const char* transaction_filename,
  const char* blackrecord_filename) {
    FILE* record_file = fopen(record_filename, "r+");
    if (record_file == NULL) {
      return EXIT_FAILURE;
    }

    FILE* transaction_file = fopen(transaction_filename, "r+");
    if (transaction_file == NULL) {
      fclose(record_file);
      return EXIT_FAILURE;
    }

    Record record = {
      1, "Ivan", "Ivanov", "Moscow",
      "8919000111", 300, 500, 100
    };
    Transaction transaction = {
      1, 500
    };

    write_record_to_file(record_file, &record);
    write_transaction_to_file(transaction_file, &transaction);

    fclose(record_file);
    fclose(transaction_file);

    if (blackrecord(record_filename, transaction_filename, blackrecord_filename) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }

    FILE* blackrecord_file = fopen(blackrecord_filename, "r");
    if (blackrecord_file == NULL) {
      return EXIT_FAILURE;
    }

    fseek(blackrecord_file, 0, SEEK_END);
    if (!ftell(blackrecord_file)) {
      fclose(blackrecord_file);
      return EXIT_FAILURE;
    }

    fclose(blackrecord_file);
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
