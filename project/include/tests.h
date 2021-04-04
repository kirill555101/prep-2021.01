#ifndef PROJECT_INCLUDE_TESTS_H_
#define PROJECT_INCLUDE_TESTS_H_

#include "work_with_file.h"
#include "equality_of_structures.h"

// Test record read-write functions
int test_write_record_to_file(const char* filename);
int test_read_record_from_file(const char* filename);

// Test transaction read-write functions
int test_write_transaction_to_file(const char* filename);
int test_read_transaction_from_file(const char* filename);

// Test structs equal function
int test_structs_equal();

#endif  // PROJECT_INCLUDE_TESTS_H_
