#include "work_with_file.h"

void write_transaction_to_file(FILE *file, Transaction *transaction) {
  if (file == NULL) {
    exit(1);
  }

  fprintf(file,
    "%-3d%-6.2f\n",
    transaction->number,
    transaction->cash_payments);
}

int read_transaction_from_file(FILE *file, Transaction *transaction) {
  if (file == NULL) {
    exit(1);
  }

  while (fscanf(file,
      "%d %lf",
      &transaction->number,
      &transaction->cash_payments) == ELEMENTS_IN_TRANSACTION) {
        return 0;
  }
  return -1;
}

void write_record_to_file(FILE *file, Record *record) {
  if (file == NULL) {
    exit(1);
  }

  fprintf(file,
    "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
    record->number,
    record->name,
    record->surname,
    record->address,
    record->phone,
    record->indebtedness,
    record->credit_limit,
    record->cash_payments);
}

int read_record_from_file(FILE *file, Record *record) {
  if (file == NULL) {
    exit(1);
  }

  while (fscanf(file,
      "%d%10s%10s%10s%10s%lf%lf%lf",
      &record->number,
      record->name,
      record->surname,
      record->address,
      record->phone,
      &record->indebtedness,
      &record->credit_limit,
      &record->cash_payments) == ELEMENTS_IN_RECORD) {
        return 0;
  }
  return -1;
}
