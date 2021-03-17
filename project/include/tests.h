#ifndef PROJECT_INCLUDE_TESTS_H_
#define PROJECT_INCLUDE_TESTS_H_

#include "equality_of_structures.h"

#define TEST_FILENAME "test.dat"

int test_write_to_file(FILE*);
int test_read_from_file(FILE*);
int test_structs_equal(Record, Record);

#endif  // PROJECT_INCLUDE_TESTS_H_
