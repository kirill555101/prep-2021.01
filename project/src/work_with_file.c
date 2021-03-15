#include "work_with_file.h"

void write_to_file(FILE *file, Data *struct_write) {
  fprintf(
    file,
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

int read_from_file(FILE *file, Data *struct_read) {
  while (fscanf(file,
      "%d%10s%10s%10s%10s%lf%lf%lf",
      &struct_read->number,
      struct_read->name,
      struct_read->surname,
      struct_read->address,
      struct_read->phone,
      &struct_read->indebtedness,
      &struct_read->credit_limit,
      &struct_read->cash_payments) == 8) {
        return 0;
  }
  return -1;
}
