#include "equality_of_structures.h"

int main() {
  const char *test_filename = "test.dat";
  FILE *test_file = fopen(test_filename, "r+");

  if (test_file == NULL) {
    puts("File is not found");
    return 1;
  } else {
    Data expected_client_data = {
      1, "Ivan", "Ivanov", "Moscow",
      "8919000111", 300, 500, 100
    };

    write_to_file(test_file, &expected_client_data);
    Data got_client_data = {0};
    rewind(test_file);
    while (read_from_file(test_file, &got_client_data) == 0) {
      if (are_structs_equal(expected_client_data, got_client_data) == 0) {
        puts("SUCCESS");
      } else {
        puts("FAILURE");
        fclose(test_file);
        return 2;
      }
    }
    fclose(test_file);
  }
  return 0;
}
