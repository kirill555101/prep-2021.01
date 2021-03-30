#define _GNU_SOURCE

#include "parser.h"

lexeme_t get_lexeme(char* line) {
	if (strncasecmp(line, FROM_STR, strlen(FROM_STR)) == 0) {
		return LEXEME_FROM;
	}

	if (strncasecmp(line, TO_STR, strlen(TO_STR)) == 0) {
		return LEXEME_TO;
	}

	if (strncasecmp(line, DATE_STR, strlen(DATE_STR)) == 0) {
		return LEXEME_DATE;
	}

	if (strcasestr(line, BOUNDARY_STR) != NULL) {
		return LEXEME_BOUNDARY;
	}

	return LEXEME_NO;
}

static char* store_args(char* line) {
	char* saveptr;
	strtok_r(line, ":", &saveptr);
	char* res;
	res = strtok_r(NULL, "\r\n", &saveptr);
	return res + strspn(res, " ");
}

void store_from(char* line, data_t* data) {
	data->from = store_args(line);
}

void store_to(char* line, data_t* data) {
	data->to = store_args(line);
}

void store_date(char* line, data_t* data) {
	data->date = store_args(line);
}

void store_boundary(char* line, data_t* data) {
	char* res = strcasestr(line, BOUNDARY_STR);
	if (strlen(res) != strlen(line)) {
		if ((*(res - 1) != 32) && (*(res - 1) != '\t') && (*(res - 1) != ';')) {
			return;
		}
	}

	res = res + strlen(BOUNDARY_STR) + strspn(res, " ");
	if (res[0] == '"') {
		res++;
		char* saveptr;
		res = strtok_r(res, "\"", &saveptr);
	} else {
		char* saveptr;
		res = strtok_r(res, "\r\n ", &saveptr);
	}
	data->boundary = res;
}

rule_t syntax[STATE_COUNT][LEXEME_COUNT] = {
						/* LEXEME_FROM */			/* LEXEME_TO */			/* LEXEME_DATE */			/* LEXEME_BOUNDARY */
	/*0 STATE_BEGIN */ 				{{STATE_FROM, store_from}, {STATE_TO, store_to}, {STATE_DATE, store_date},
										{STATE_BOUNDARY, store_boundary}},
	/*1 STATE_FROM */  				{{STATE_ERROR, NULL}, {STATE_FROM_TO, store_to}, {STATE_FROM_DATE, store_date},
										{STATE_FROM_BOUNDARY, store_boundary}},
	/*2 STATE_TO */    				{{STATE_FROM_TO, store_from}, {STATE_ERROR, NULL},  {STATE_TO_DATE, store_date},
										{STATE_TO_BOUNDARY, store_boundary}},
	/*3 STATE_DATE */				{{STATE_FROM_DATE, store_from}, {STATE_TO_DATE, store_to}, {STATE_ERROR, NULL},
										{STATE_DATE_BOUNDARY, store_boundary}},
	/*4 STATE_BOUNDARY */ 			{{STATE_FROM_BOUNDARY, store_from}, {STATE_TO_BOUNDARY, store_to},
										{STATE_DATE_BOUNDARY, store_date}, {STATE_ERROR, NULL}},
	/*5 STATE_FROM_TO */  			{{STATE_ERROR, NULL}, {STATE_ERROR, NULL}, {STATE_FROM_TO_DATE, store_date},
										{STATE_FROM_TO_BOUNDARY, store_boundary}},
	/*6 STATE_FROM_DATE */			{{STATE_ERROR, NULL}, {STATE_FROM_TO_DATE, store_to}, {STATE_ERROR, NULL},
										{STATE_FROM_DATE_BOUNDARY, store_boundary}},
	/*7 STATE_FROM_BOUNDARY */		{{STATE_ERROR, NULL}, {STATE_FROM_TO_BOUNDARY, store_to},
										{STATE_FROM_DATE_BOUNDARY, store_date}, {STATE_ERROR, NULL}},
	/*8 STATE_TO_DATE */ 			{{STATE_FROM_TO_DATE, store_from}, {STATE_ERROR, NULL}, {STATE_ERROR, NULL},
										{STATE_TO_DATE_BOUNDARY, store_boundary}},
	/*9 STATE_TO_BOUNDARY */ 		{{STATE_FROM_TO_BOUNDARY, store_from}, {STATE_ERROR, NULL},
										{STATE_TO_DATE_BOUNDARY, store_date}, {STATE_ERROR, NULL}},
	/*10 STATE_DATE_BOUNDARY */ 	{{STATE_FROM_DATE_BOUNDARY, store_from}, {STATE_TO_DATE_BOUNDARY, store_date},
										{STATE_ERROR, NULL}, {STATE_ERROR, NULL}},
	/*11 STATE_FROM_TO_DATE */ 		{{STATE_ERROR, NULL}, {STATE_ERROR, NULL}, {STATE_ERROR, NULL},
										{STATE_END, store_boundary}},
	/*12 STATE_FROM_TO_BOUNDARY */ 	{{STATE_ERROR, NULL}, {STATE_ERROR, NULL}, {STATE_END, store_date},
										{STATE_ERROR, NULL}},
	/*13 STATE_FROM_DATE_BOUNDARY*/ {{STATE_ERROR, NULL}, {STATE_END, store_to}, {STATE_ERROR, NULL},
										{STATE_ERROR, NULL}},
	/*14 STATE_TO_DATE_BOUNDARY */ {{STATE_END, NULL}, {STATE_ERROR, NULL}, {STATE_ERROR, NULL},
										 {STATE_ERROR, NULL}},
};

int parse_email(const char* filename) {
	int fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	struct stat sb;

	if (fstat(fd, &sb) == -1) {
		return EXIT_FAILURE;
	}

	char* file_in_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (file_in_memory == NULL) {
		return EXIT_FAILURE;
	}

	int has_body = 0, has_found_next_line = 0, has_found_empty_line = 0;
	for (int i = 0; i < sb.st_size; i++) {
		if (has_found_empty_line == 1 && file_in_memory[i] != '\n' && file_in_memory[i] != '\r' &&
			file_in_memory[i] != '\0' && file_in_memory[i] != 10) {
				has_body = 1;
				break;
		}

		if (has_found_next_line == 1 && (file_in_memory[i] == '\n' || file_in_memory[i] == '\r')) {
			has_found_empty_line = 1;
			has_body = 0;
		}

		has_found_next_line = 0;
		if (file_in_memory[i] == '\n' || file_in_memory[i] == '\r') {
			has_found_next_line = 1;
		}
	}

	data_t data = {0};
	lexeme_t lexeme;
	state_t state = STATE_BEGIN;
	rule_t rule = {17, NULL};
	int has_found_boundary = 0, has_after_part = 0, has_parts_begin = 0;
	char *saveptr1, *saveptr2, *new, *line = strtok_r(file_in_memory, "\n\r", &saveptr1);
	while (line != NULL) {
		lexeme = get_lexeme(line);
		if (state < STATE_COUNT && lexeme < LEXEME_COUNT) {
			rule = syntax[state][lexeme];
		}

		if (state != STATE_END && state != STATE_ERROR && (lexeme < LEXEME_COUNT) && rule.action) {
			rule.action(line, &data);
		}
		if (has_found_boundary == 1) {
			if (strstr(line, data.boundary) != NULL) {
				has_parts_begin = 1;
				has_after_part = 0;
				data.parts++;
			}
			if (has_parts_begin == 1) {
				line = strtok_r(NULL, "\n\r", &saveptr1);
				if (line != NULL) {
					if (strlen(line) != 1) {
						has_after_part = 1;
					}
				}
				continue;
			}
		}
		line = strtok_r(NULL, "\n\r", &saveptr1);
		if (line != NULL && has_parts_begin == 0) {
			while (line[0] == ' ' && lexeme < LEXEME_BOUNDARY) {
				if (lexeme == LEXEME_FROM &&
					state != STATE_FROM &&
					state != STATE_FROM_TO &&
					state != STATE_FROM_DATE &&
					state != STATE_FROM_BOUNDARY &&
					state != STATE_FROM_TO_DATE &&
					state != STATE_FROM_TO_BOUNDARY &&
					state != STATE_FROM_DATE_BOUNDARY &&
					state != STATE_END) {
						new = calloc(1, strlen(data.from) + strlen(line) + 1);
						if (new == NULL) {
							close(fd);
							return EXIT_FAILURE;
						}
						data.from = strtok_r(data.from, "\n\r", &saveptr2);
						strncat(new, data.from, strlen(data.from));
						strncat(new, line, strlen(line));
						strncpy(data.from, new, strlen(new) + 1);
						free(new);
				}

				if (lexeme == LEXEME_TO &&
					state != STATE_TO &&
					state != STATE_FROM_TO &&
					state != STATE_TO_DATE &&
					state != STATE_TO_BOUNDARY &&
					state != STATE_FROM_TO_DATE &&
					state != STATE_TO_DATE_BOUNDARY &&
					state != STATE_END) {
						new = calloc(1, strlen(data.to) + strlen(line) + 1);
						if (new == NULL) {
							close(fd);
							return EXIT_FAILURE;
						}
						data.to = strtok_r(data.to, "\n\r", &saveptr2);
						strncat(new, data.to, strlen(data.to));
						strncat(new, line, strlen(line));
						strncpy(data.to, new, strlen(new) + 1);
						free(new);
				}

				if (lexeme == LEXEME_DATE &&
					state != STATE_DATE &&
					state != STATE_FROM_DATE &&
					state != STATE_TO_DATE &&
					state != STATE_DATE_BOUNDARY &&
					state != STATE_FROM_TO_DATE &&
					state != STATE_FROM_DATE_BOUNDARY &&
					state != STATE_TO_DATE_BOUNDARY &&
					state != STATE_END) {
						new = calloc(1, strlen(data.date) + strlen(line) + 1);
						if (new == NULL) {
							close(fd);
							return EXIT_FAILURE;
						}
						data.date = strtok_r(data.date, "\n\r", &saveptr2);
						strncat(new, data.date, strlen(data.date));
						strncat(new, line, strlen(line));
						strncpy(data.date, new, strlen(new) + 1);
						free(new);
				}
				line = strtok_r(NULL, "\n\r", &saveptr1);
			}
		}
		if (!((rule.state == STATE_BOUNDARY ||
			rule.state == STATE_FROM_BOUNDARY ||
			rule.state == STATE_TO_BOUNDARY ||
			rule.state == STATE_DATE_BOUNDARY ||
			rule.state == STATE_FROM_TO_BOUNDARY ||
			rule.state == STATE_FROM_DATE_BOUNDARY ||
			rule.state == STATE_END) &&
			data.boundary == NULL)) {
				state = rule.state;
		}

		if (state == STATE_BOUNDARY ||
			state == STATE_FROM_BOUNDARY ||
			state == STATE_DATE_BOUNDARY ||
			state == STATE_FROM_TO_BOUNDARY ||
			state == STATE_FROM_DATE_BOUNDARY ||
			state == STATE_TO_DATE_BOUNDARY ||
			state == STATE_END) {
				has_found_boundary = 1;
		}
	}

	close(fd);

	if (data.parts != 0 && has_after_part == 0) {
		data.parts--;
	} else if (has_body == 0) {
		data.parts = 0;
	} else if (data.parts == 0) {
		data.parts = 1;
	}

	if (data.from != NULL) {
		printf("%s", data.from);
	}

	printf("|");
	if (data.to != NULL) {
		printf("%s", data.to);
	}

	printf("|");
	if (data.date != NULL) {
		printf("%s", data.date);
	}
	printf("|%zu\n", data.parts);

	return EXIT_SUCCESS;
}
