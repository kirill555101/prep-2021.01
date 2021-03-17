#ifndef PROJECT_INCLUDE_WORK_WITH_FILE_H_
#define PROJECT_INCLUDE_WORK_WITH_FILE_H_

#include "utils.h"

#define ELEMENTS_IN_RECORD 8
#define ELEMENTS_IN_TRANSACTION 2

#define FILE_ERROR 1
#define WRITE_ERROR 2
#define READ_ERROR 3
#define EQUAL_ERROR 4

void write_transaction_to_file(FILE*, Transaction*);
int read_transaction_from_file(FILE*, Transaction*);
void write_record_to_file(FILE*, Record*);
int read_record_from_file(FILE*, Record*);

#endif  // PROJECT_INCLUDE_WORK_WITH_FILE_H_
