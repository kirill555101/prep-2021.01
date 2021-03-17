#include "work_with_file.h"

void print_menu() {
  puts(
    "Please enter action\n"
    "1 enter data client:\n"
    "2 enter data transaction:\n"
    "3 update base:\n");
}

void print_about_client() {
  puts(
    "1 Number account:\n"
    "2 Client name:\n"
    "3 Surname:\n"
    "4 Addres client:\n"
    "5 Client Telnum:\n"
    "6 Client indebtedness:\n"
    "7 Client credit limit:\n"
    "8 Client cash payments:\n");
}

void print_about_transfer() {
  puts(
    "1 Number account:\n"
    "2 Client cash payments:\n");
}

void master_write() {
  Record client = {0};
  print_about_client();
  while (scanf("%d%10s%10s%10s%10s%lf%lf%lf",
      &client.number,
      client.name,
      client.surname,
      client.address,
      client.phone,
      &client.indebtedness,
      &client.credit_limit,
      &client.cash_payments) == ELEMENTS_IN_RECORD) {
        write_record_to_file(RECORD_FILENAME, "a+", &client);
        print_about_client();
  }
}

void transaction_write() {
  Transaction transfer = {0};
  print_about_transfer();
  while (scanf("%d %lf", &transfer.number, &transfer.cash_payments) == ELEMENTS_IN_TRANSACTION) {
    write_transaction_to_file(TRANSACTION_FILENAME, "a+", &transfer);
    print_about_transfer();
  }
}

void black_record() {
  FILE *record_file;
  FILE  *transaction_file;
  FILE *black_record_file;
  record_file = fopen(RECORD_FILENAME, "r");
  if (record_file == NULL) {
    perror("FAILURE: File record.dat is not found");
    exit(1);
  }

  transaction_file = fopen(TRANSACTION_FILENAME, "r");
  if (transaction_file == NULL) {
    perror("FAILURE: File transaction.dat is not found");
    fclose(record_file);
    exit(1);
  }

  black_record_file = fopen(BLACKRECORD_FILENAME, "w");
  if (black_record_file == NULL) {
    perror("FAILURE: File black_record.dat is not found");
    fclose(record_file);
    fclose(transaction_file);
    exit(1);
  }

  Record client_data = {0};
  Transaction transfer = {0};
  while (read_from_file(record_file, &client_data) == 0) {
    while (fscanf(transaction_file,
        "%d %lf",
        &transfer.number,
        &transfer.cash_payments) == ELEMENTS_IN_TRANSACTION) {
          if (client_data.number == transfer.number && transfer.cash_payments != 0) {
            client_data.credit_limit += transfer.cash_payments;
          }
    }
    write_to_file(black_record_file, &client_data);
    rewind(transaction_file);
  }

  fclose(record_file);
  fclose(transaction_file);
  fclose(black_record_file);
}
