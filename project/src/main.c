#include "work_with_file.h"

#define ENTER_DATA_CLIENT 1
#define ENTER_DATA_TRANSACTION 2
#define UPDATE_BASE 3

int main(void) {
  int choice = 0;
  FILE *record_file = fopen(RECORD_FILENAME, "r+");
  FILE *transaction_file = fopen(TRANSACTION_FILENAME, "r+");
  FILE *black_record_file = fopen(BLACKRECORD_FILENAME, "w");

  if (record_file == NULL || transaction_file == NULL || black_record_file == NULL) {
    perror("Files error");
    exit(1);
  }

  print_menu();
  while (scanf("%d", &choice) == 1) {
    switch (choice) {
      case ENTER_DATA_CLIENT:
        master_write(record_file);
        break;
      case ENTER_DATA_TRANSACTION:
        transaction_write(transaction_file);
        break;
      case UPDATE_BASE:
        black_record(record_file, transaction_file, black_record_file);
        break;
      default:
        fprintf(stderr, "Error! Your choice is not valid\n");
        break;
    }
    print_menu();
  }

  fclose(record_file);
  fclose(transaction_file);
  fclose(black_record_file);
  return 0;
}
