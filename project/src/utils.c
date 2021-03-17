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

void master_write(FILE *record_file) {
  if (record_file == NULL) {
    exit(1);
  }

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
        write_record_to_file(record_file, &client);
        print_about_client();
  }
}

void transaction_write(FILE *transaction_file) {
  if (transaction_file == NULL) {
    exit(1);
  }

  Transaction transfer = {0};
  print_about_transfer();
  while (scanf("%d %lf", &transfer.number, &transfer.cash_payments) == ELEMENTS_IN_TRANSACTION) {
    write_transaction_to_file(transaction_file, &transfer);
    print_about_transfer();
  }
}

void black_record(FILE *record_file, FILE *transaction_file, FILE *black_record_file) {
  if (record_file == NULL || transaction_file == NULL || black_record_file == NULL) {
    exit(1);
  }

  Record client_data = {0};
  Transaction transfer = {0};
  while (read_record_from_file(record_file, &client_data) == 0) {
    while (read_transaction_from_file(transaction_file, &transfer) == 0) {
      if (client_data.number == transfer.number && transfer.cash_payments != 0) {
        client_data.credit_limit += transfer.cash_payments;
      }
    }
    rewind(transaction_file);
    write_record_to_file(black_record_file, &client_data);
  }
}
