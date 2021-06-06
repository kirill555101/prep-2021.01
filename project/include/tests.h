#ifndef PROJECT_INCLUDE_TESTS_H_
#define PROJECT_INCLUDE_TESTS_H_

#include "utils.h"
#include "work_with_file.h"
#include "equality_of_structures.h"

int test_master_write(const char* filename);
int test_transaction_write(const char* filename);
int test_blackrecord(
  const char* record_filename, const char* transaction_filename,
  const char* blackrecord_filename
);
int test_structs_equal();

#endif  // PROJECT_INCLUDE_TESTS_H_
