#ifndef PROJECT_INCLUDE_WORK_WITH_FILE_H_
#define PROJECT_INCLUDE_WORK_WITH_FILE_H_

#include "utils.h"

#define ELEMENTS_IN_RECORD 8
#define ELEMENTS_IN_TRANSACTION 2

void write_record_to_file(const char*, const char*, Record*);
void write_transaction_to_file(const char*, const char*, Transaction*);
void write_to_file(FILE*, Record*);
int read_from_file(FILE*, Record*);

#endif  // PROJECT_INCLUDE_WORK_WITH_FILE_H_
