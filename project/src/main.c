#include "work_with_file.h"

#define ENTER_DATA_CLIENT 1
#define ENTER_DATA_TRANSACTION 2
#define UPDATE_BASE 3

#define TRANSACTION_FILENAME "transaction.dat"
#define RECORD_FILENAME "record.dat"
#define BLACKRECORD_FILENAME "blackrecord.dat"

int main(void) {
  int choice = 0;
  print_menu();
  while (scanf("%d", &choice) == 1) {
    switch (choice) {
      case ENTER_DATA_CLIENT:
        if (master_write(RECORD_FILENAME) != EXIT_SUCCESS) {
          return EXIT_FAILURE;
        }
        break;
      case ENTER_DATA_TRANSACTION:
        if (transaction_write(TRANSACTION_FILENAME) != EXIT_SUCCESS) {
          return EXIT_FAILURE;
        }
        break;
      case UPDATE_BASE:
        if (blackrecord(RECORD_FILENAME, TRANSACTION_FILENAME, BLACKRECORD_FILENAME) != EXIT_SUCCESS) {
          return EXIT_FAILURE;
        }
        break;
      default:
        fprintf(stderr, "Error! Your choice is not valid\n");
        break;
    }
    print_menu();
  }
  return EXIT_SUCCESS;
}
