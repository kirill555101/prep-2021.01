#include "work_with_file.h"

void write_record_to_file(const char* filename, const char* mode, Record *struct_write) {
  FILE *file = fopen(filename, mode);
  if (file == NULL) {
    exit(1);
  }

  write_to_file(file, struct_write);
  fclose(file);
}

void write_transaction_to_file(const char* filename, const char* mode, Transaction *transaction_write) {
  FILE *file = fopen(filename, mode);
  if (file == NULL) {
    exit(1);
  }

  fprintf(file,
    "%-3d%-6.2f\n",
    transaction_write->number,
    transaction_write->cash_payments);
  fclose(file);
}

void write_to_file(FILE *file, Record *struct_write) {
  fprintf(file,
    "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
    struct_write->number,
    struct_write->name,
    struct_write->surname,
    struct_write->address,
    struct_write->phone,
    struct_write->indebtedness,
    struct_write->credit_limit,
    struct_write->cash_payments);
}

int read_from_file(FILE *file, Record *struct_read) {
  while (fscanf(file,
      "%d%10s%10s%10s%10s%lf%lf%lf",
      &struct_read->number,
      struct_read->name,
      struct_read->surname,
      struct_read->address,
      struct_read->phone,
      &struct_read->indebtedness,
      &struct_read->credit_limit,
      &struct_read->cash_payments) == ELEMENTS_IN_RECORD) {
        return 0;
  }
  return -1;
}
