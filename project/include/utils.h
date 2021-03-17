#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#define TRANSACTION_FILENAME "transaction.dat"
#define RECORD_FILENAME "record.dat"
#define BLACKRECORD_FILENAME "blackrecord.dat"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int number;
  char name[20];
  char surname[20];
  char address[30];
  char phone[15];
  double indebtedness;
  double credit_limit;
  double cash_payments;
} Record;

typedef struct Transaction {
  int number;
  double cash_payments;
} Transaction;

void print_menu();

void print_about_client();

void print_about_transfer();

void master_write();

void transaction_write();

void black_record();

#endif  // PROJECT_INCLUDE_UTILS_H_


