#include "work_with_file.h"

#define ENTER_DATA_CLIENT 1
#define ENTER_DATA_TRANSACTION 2
#define UPDATE_BASE 3

int main(void) {
  int choice = 0;
  print_menu();
  while (scanf("%d", &choice) == 1) {
    switch (choice) {
      case ENTER_DATA_CLIENT:
        master_write();
        break;
      case ENTER_DATA_TRANSACTION:
        transaction_write();
        break;
      case UPDATE_BASE:
        black_record();
        break;
      default:
        fprintf(stderr, "Error! Your choice is not valid\n");
        break;
    }
    print_menu();
    }
  return 0;
}
