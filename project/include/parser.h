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

#define LINE_SIZE 4096

#define FROM_STR "from:"
#define TO_STR "to:"
#define DATE_STR "date:"
#define BOUNDARY_STR "boundary="

typedef enum {
	STATE_BEGIN,
	STATE_FROM,
	STATE_TO,
	STATE_DATE,
	STATE_BOUNDARY,
	STATE_FROM_TO,
	STATE_FROM_DATE,
	STATE_FROM_BOUNDARY,
	STATE_TO_DATE,
	STATE_TO_BOUNDARY,
	STATE_DATE_BOUNDARY,
	STATE_FROM_TO_DATE,
	STATE_FROM_TO_BOUNDARY,
	STATE_FROM_DATE_BOUNDARY,
	STATE_TO_DATE_BOUNDARY,
	STATE_COUNT,
	STATE_ERROR,
	STATE_END
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

typedef void (*action_t)(char* line, data_t *data);

typedef struct {
	state_t state;
	action_t action;
} rule_t;

int parse_email(const char* filename);

#endif  // PROJECT_INCLUDE_PARSER_H_

