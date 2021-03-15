#include "work_with_file.h"

int main(void) {
  int choice = 0;
  print_menu();
  while (scanf("%d", &choice) == 1) {
    switch (choice) {
      case 1:
        master_write();
        break;
      case 2:
        transaction_write();
        break;
      case 3:
        black_record();
        break;
      default:
        puts("Error! Your choice is not valid");
        break;
    }
    print_menu();
    }
  return 0;
}
