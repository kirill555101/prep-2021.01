#ifndef PROJECT_INCLUDE_PARSER_H_
#define PROJECT_INCLUDE_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
  from,
  to,
  date,
  content_type
} HEADER_NAME;

#define MAX_LINE_LENGTH 250

char* parse_email(FILE* file);
int parse_line(const char* line, char* res, HEADER_NAME* name);
char* str_to_lower(const char* str);
int get_count_of_content_type(FILE* file, const char* content, size_t* val);

char** make_array(size_t length);
int free_array(char** array, size_t length);

#endif  // PROJECT_INCLUDE_PARSER_H_
