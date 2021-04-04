#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

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

typedef struct {
  int number;
  double cash_payments;
} Transaction;

void print_menu();
void print_about_client();
void print_about_transfer();
int master_write(const char* filename);
int transaction_write(const char* filename);
int blackrecord(
  const char* record_filename, const char* transaction_filename,
  const char* blackrecord_filename
);

#endif  // PROJECT_INCLUDE_UTILS_H_
