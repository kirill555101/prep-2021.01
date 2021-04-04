#include <assert.h>

#include "tests.h"

#define TEST_RECORD_FILENAME "test_record.dat"
#define TEST_TRANSACTION_FILENAME "test_transaction.dat"

int main() {
  // Test record read-write functions
  assert(!test_write_record_to_file(TEST_RECORD_FILENAME));
  assert(!test_read_record_from_file(TEST_RECORD_FILENAME));

  // Test transaction read-write functions
  assert(!test_write_transaction_to_file(TEST_TRANSACTION_FILENAME));
  assert(!test_read_transaction_from_file(TEST_TRANSACTION_FILENAME));

  // Test structs equal function
  assert(!test_structs_equal());

  return EXIT_SUCCESS;
}
