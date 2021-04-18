#ifndef PROJECT_INCLUDE_PARSER_H_
#define PROJECT_INCLUDE_PARSER_H_

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FROM_STR "From:"
#define TO_STR "To:"
#define DATE_STR "Date:"
#define CONTENT_TYPE_STR "Content-Type:"
#define BOUNDARY_STR "boundary="

typedef enum {
	STATE_FROM,
	STATE_TO,
	STATE_DATE,
	STATE_CONTENT_TYPE,
	STATE_BOUNDARY,
	STATE_COUNT,
} state_t;

typedef enum {
	LEXEME_FROM,
	LEXEME_TO,
	LEXEME_DATE,
	LEXEME_BOUNDARY,
	LEXEME_COUNT,
	LEXEME_NO
} lexeme_t;

typedef struct {
	char* from;
	char* to;
	char* date;
	char* boundary;
	size_t parts;
} data_t;

int parse_email(const char* filename, data_t* input_data);
void print_data(data_t data);
int free_data(data_t* data);

#endif  // PROJECT_INCLUDE_PARSER_H_
