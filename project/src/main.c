#include "work_with_file.h"

#define ENTER_DATA_CLIENT 1
#define ENTER_DATA_TRANSACTION 2
#define UPDATE_BASE 3

int main(void) {
  int choice = 0, code;
  print_menu();
  while (scanf("%d", &choice) == 1) {
    switch (choice) {
      case ENTER_DATA_CLIENT: {
        FILE *record_file = fopen(RECORD_FILENAME, "r+");
        if (record_file == NULL) {
          perror("record.dat ERROR");
          return FILE_ERROR;
        }

        if ((code = master_write(record_file)) != 0) {
          fclose(record_file);
          return code;
        }
        fclose(record_file);
        break;
      }
      case ENTER_DATA_TRANSACTION: {
        FILE *transaction_file = fopen(TRANSACTION_FILENAME, "r+");
        if (transaction_file == NULL) {
          perror("transaction.dat ERROR");
          return FILE_ERROR;
        }

        if ((code = transaction_write(transaction_file)) != 0) {
          fclose(transaction_file);
          return code;
        }
        fclose(transaction_file);
        break;
      }
      case UPDATE_BASE: {
        FILE *record_file = fopen(RECORD_FILENAME, "r+");
        if (record_file == NULL) {
          perror("record.dat ERROR");
          return FILE_ERROR;
        }

        FILE *transaction_file = fopen(TRANSACTION_FILENAME, "r+");
        if (transaction_file == NULL) {
          perror("transaction.dat ERROR");
          fclose(record_file);
          return FILE_ERROR;
        }

        FILE *black_record_file = fopen(BLACKRECORD_FILENAME, "w");
        if (black_record_file == NULL) {
          perror("blackrecord.dat ERROR");
          fclose(record_file);
          fclose(transaction_file);
          return FILE_ERROR;
        }

        if ((code = black_record(record_file, transaction_file, black_record_file)) != 0) {
          fclose(record_file);
          fclose(transaction_file);
          fclose(black_record_file);
          return code;
        }
        fclose(record_file);
        fclose(transaction_file);
        fclose(black_record_file);
        break;
      }
      default: {
        fprintf(stderr, "Error! Your choice is not valid\n");
        break;
      }
    }
    print_menu();
  }
  return 0;
}
