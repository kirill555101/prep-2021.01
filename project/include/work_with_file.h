#ifndef PROJECT_INCLUDE_WORK_WITH_FILE_H_
#define PROJECT_INCLUDE_WORK_WITH_FILE_H_

#include "utils.h"

#define ELEMENTS_IN_RECORD 8
#define ELEMENTS_IN_TRANSACTION 2

void write_transaction_to_file(FILE* file, Transaction* transacion);
int read_transaction_from_file(FILE* file, Transaction* transacion);
void write_record_to_file(FILE* file, Record* record);
int read_record_from_file(FILE* file, Record* record);

#endif  // PROJECT_INCLUDE_WORK_WITH_FILE_H_
