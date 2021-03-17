#include "tests.h"

int main() {
  const char *test_filename = "test.dat";
  FILE* test_file = fopen(test_filename, "w+");
  int code;

  if ((code = test_write_to_file(test_file)) != 0) {
    puts("test1 FAILURE");
    fclose(test_file);
    return code;
  }
  puts("test1 SUCCESS");

  rewind(test_file);
  if ((code = test_read_from_file(test_file)) != 0) {
    puts("test2 FAILURE");
    fclose(test_file);
    return code;
  }
  fclose(test_file);
  puts("test2 SUCCESS");

  Record first_struct = {
    1, "Ivan", "Ivanov", "Moscow",
    "8919000111", 300, 500, 100
  };
  Record second_struct = first_struct;
  if ((code = test_structs_equal(first_struct, second_struct)) != 0) {
    puts("test3 FAILURE");
    return code;
  }
  puts("test3 SUCCESS");
  return 0;
}
