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

#define MAX_LINE_LENGTH 50

char* parse_email(FILE* file);
int has_parsed_line(const char* line, char* res, HEADER_NAME* name);
char* str_to_lower(const char* str);

#endif  // PROJECT_INCLUDE_PARSER_H_
