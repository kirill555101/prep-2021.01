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

int master_write(const char* filename) {
  FILE* file = fopen(filename, "r+");
  if (file == NULL) {
    return EXIT_FAILURE;
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
        write_record_to_file(file, &client);
        print_about_client();
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int transaction_write(const char* filename) {
  FILE* file = fopen(filename, "r+");
    if (file == NULL) {
      perror("transaction.dat ERROR");
      return EXIT_FAILURE;
    }

  Transaction transfer = {0};
  print_about_transfer();
  while (scanf("%d %lf", &transfer.number, &transfer.cash_payments) == ELEMENTS_IN_TRANSACTION) {
    write_transaction_to_file(file, &transfer);
    print_about_transfer();
  }

  fclose(file);
  return EXIT_SUCCESS;
}

int blackrecord(
  const char* record_filename, const char* transaction_filename,
  const char* blackrecord_filename) {
    FILE* record_file = fopen(record_filename, "r+");
    if (record_file == NULL) {
      return EXIT_FAILURE;
    }

    FILE* transaction_file = fopen(transaction_filename, "r+");
    if (transaction_file == NULL) {
      fclose(record_file);
      return EXIT_FAILURE;
    }

    FILE* blackrecord_file = fopen(blackrecord_filename, "w");
    if (blackrecord_file == NULL) {
      fclose(record_file);
      fclose(transaction_file);
      return EXIT_FAILURE;
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
      write_record_to_file(blackrecord_file, &client_data);
    }

    fclose(record_file);
    fclose(transaction_file);
    fclose(blackrecord_file);
    return EXIT_SUCCESS;
}
