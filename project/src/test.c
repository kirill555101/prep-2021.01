#include <assert.h>

#include "tests.h"

#define TEST_RECORD_FILENAME "test_record.dat"
#define TEST_TRANSACTION_FILENAME "test_transaction.dat"
#define BLACKRECORD_FILENAME "test_blackrecord.dat"

int main() {
  assert(!test_master_write(TEST_RECORD_FILENAME));
  assert(!test_transaction_write(TEST_TRANSACTION_FILENAME));
  assert(!test_blackrecord(
    TEST_RECORD_FILENAME, TEST_TRANSACTION_FILENAME,
    BLACKRECORD_FILENAME));
  assert(!test_structs_equal());

  return EXIT_SUCCESS;
}
