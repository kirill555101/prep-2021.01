#define _GNU_SOURCE

#include "parser.h"

// Store functions and state
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
	char* saveptr;
	if (res[0] == '"') {
		res++;
		res = strtok_r(res, "\"", &saveptr);
	} else {
		res = strtok_r(res, "\r\n ", &saveptr);
	}
	data->boundary = res;
}

rule_t syntax[STATE_COUNT][LEXEME_COUNT] = {
                           /*LEXEME_FROM                                                         LEXEME_TO
                             LEXEME_DATE                                                   LEXEME_BOUNDARY*/
/*STATE_BEGIN*/              {{STATE_FROM, store_from},                               {STATE_TO, store_to},
                             {STATE_DATE, store_date},                   {STATE_BOUNDARY, store_boundary}},

/*STATE_FROM*/               {{STATE_ERROR, NULL},                               {STATE_FROM_TO, store_to},
                             {STATE_FROM_DATE, store_date},         {STATE_FROM_BOUNDARY, store_boundary}},

/*STATE_TO*/                 {{STATE_FROM_TO, store_from},                             {STATE_ERROR, NULL},
                             {STATE_TO_DATE, store_date},             {STATE_TO_BOUNDARY, store_boundary}},

/*STATE_DATE*/               {{STATE_FROM_DATE, store_from},                     {STATE_TO_DATE, store_to},
                             {STATE_ERROR, NULL},                   {STATE_DATE_BOUNDARY, store_boundary}},

/*STATE_BOUNDARY*/           {{STATE_FROM_BOUNDARY, store_from},             {STATE_TO_BOUNDARY, store_to},
                             {STATE_DATE_BOUNDARY, store_date},                       {STATE_ERROR, NULL}},

/*STATE_FROM_TO*/            {{STATE_ERROR, NULL},                                     {STATE_ERROR, NULL},
                             {STATE_FROM_TO_DATE, store_date},   {STATE_FROM_TO_BOUNDARY, store_boundary}},

/*STATE_FROM_DATE*/          {{STATE_ERROR, NULL},                          {STATE_FROM_TO_DATE, store_to},
                             {STATE_ERROR, NULL},              {STATE_FROM_DATE_BOUNDARY, store_boundary}},

/*STATE_FROM_BOUNDARY*/      {{STATE_ERROR, NULL},                      {STATE_FROM_TO_BOUNDARY, store_to},
                             {STATE_FROM_DATE_BOUNDARY, store_date},                  {STATE_ERROR, NULL}},

/*STATE_TO_DATE*/            {{STATE_FROM_TO_DATE, store_from},                        {STATE_ERROR, NULL},
                             {STATE_ERROR, NULL},                {STATE_TO_DATE_BOUNDARY, store_boundary}},

/*STATE_TO_BOUNDARY*/        {{STATE_FROM_TO_BOUNDARY, store_from},                    {STATE_ERROR, NULL},
                             {STATE_TO_DATE_BOUNDARY, store_date},                    {STATE_ERROR, NULL}},

/*STATE_DATE_BOUNDARY*/      {{STATE_FROM_DATE_BOUNDARY, store_from}, {STATE_TO_DATE_BOUNDARY, store_date},
                             {STATE_ERROR, NULL},                                     {STATE_ERROR, NULL}},

/*STATE_FROM_TO_DATE*/       {{STATE_ERROR, NULL},                                     {STATE_ERROR, NULL},
                             {STATE_ERROR, NULL},                             {STATE_END, store_boundary}},

/*STATE_FROM_TO_BOUNDARY*/   {{STATE_ERROR, NULL},                                     {STATE_ERROR, NULL},
                             {STATE_END, store_date},                                 {STATE_ERROR, NULL}},

/*STATE_FROM_DATE_BOUNDARY*/ {{STATE_ERROR, NULL},                                   {STATE_END, store_to},
                             {STATE_ERROR, NULL},                                     {STATE_ERROR, NULL}},

/*STATE_TO_DATE_BOUNDARY*/   {{STATE_END, NULL},                                       {STATE_ERROR, NULL},
                             {STATE_ERROR, NULL},                                     {STATE_ERROR, NULL}},
};

// Helpful functions
char* get_memory_from_file(const char* filename, size_t* length) {
	int fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	struct stat sb;

	if (fstat(fd, &sb) == -1) {
		close(fd);
		return NULL;
	}

	char* file_in_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (file_in_memory == NULL) {
		close(fd);
		return NULL;
	}

	close(fd);
	*length = sb.st_size;
	return file_in_memory;
}

char* expand_str(const char* line, char* str) {
	if (line == NULL || str == NULL) {
		return NULL;
	}

	char* saveptr;
	char* new = calloc(1, strlen(str) + strlen(line) + 1);
	if (new == NULL) {
		return NULL;
	}

	char* res = strtok_r(str, "\n\r", &saveptr);
	strncat(new, res, strlen(res));
	strncat(new, line, strlen(line));
	strncpy(res, new, strlen(new) + 1);
	free(new);

	return res;
}

int is_from_lexeme(lexeme_t lexeme, state_t state) {
	if (lexeme == LEXEME_FROM &&
		state != STATE_FROM &&
		state != STATE_FROM_TO &&
		state != STATE_FROM_DATE &&
		state != STATE_FROM_BOUNDARY &&
		state != STATE_FROM_TO_DATE &&
		state != STATE_FROM_TO_BOUNDARY &&
		state != STATE_FROM_DATE_BOUNDARY &&
		state != STATE_END) {
			return 1;
	}

	return 0;
}

int is_to_lexeme(lexeme_t lexeme, state_t state) {
	if (lexeme == LEXEME_TO &&
		state != STATE_TO &&
		state != STATE_FROM_TO &&
		state != STATE_TO_DATE &&
		state != STATE_TO_BOUNDARY &&
		state != STATE_FROM_TO_DATE &&
		state != STATE_TO_DATE_BOUNDARY &&
		state != STATE_END) {
			return 1;
	}

	return 0;
}

int is_date_lexeme(lexeme_t lexeme, state_t state) {
	if (lexeme == LEXEME_DATE &&
		state != STATE_DATE &&
		state != STATE_FROM_DATE &&
		state != STATE_TO_DATE &&
		state != STATE_DATE_BOUNDARY &&
		state != STATE_FROM_TO_DATE &&
		state != STATE_FROM_DATE_BOUNDARY &&
		state != STATE_TO_DATE_BOUNDARY &&
		state != STATE_END) {
			return 1;
	}

	return 0;
}

int is_rule_state_boundary(rule_t rule) {
	if (rule.state == STATE_BOUNDARY ||
		rule.state == STATE_FROM_BOUNDARY ||
		rule.state == STATE_TO_BOUNDARY ||
		rule.state == STATE_DATE_BOUNDARY ||
		rule.state == STATE_FROM_TO_BOUNDARY ||
		rule.state == STATE_FROM_DATE_BOUNDARY ||
		rule.state == STATE_END) {
			return 1;
	}

	return 0;
}

int is_state_boundary(state_t state) {
	if (state == STATE_BOUNDARY ||
		state == STATE_FROM_BOUNDARY ||
		state == STATE_DATE_BOUNDARY ||
		state == STATE_FROM_TO_BOUNDARY ||
		state == STATE_FROM_DATE_BOUNDARY ||
		state == STATE_TO_DATE_BOUNDARY ||
		state == STATE_END) {
			return 1;
	}

	return 0;
}

int expand_data(lexeme_t lexeme, state_t state, const char* line, data_t* data) {
	if (is_from_lexeme(lexeme, state)) {
		if ((data->from = expand_str(line, data->from)) == NULL) {
			return EXIT_FAILURE;
		}
	}

	if (is_to_lexeme(lexeme, state)) {
		if ((data->to = expand_str(line, data->to)) == NULL) {
			return EXIT_FAILURE;
		}
	}

	if (is_date_lexeme(lexeme, state)) {
		if ((data->date = expand_str(line, data->date)) == NULL) {
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int get_data(char* file_in_memory, data_t* input_data, int has_body) {
	data_t data = {0};
	lexeme_t lexeme;
	state_t state = STATE_BEGIN;
	rule_t rule = {17, NULL};

	int has_found_boundary = 0, has_after_part = 0, has_parts_begin = 0;
	char *saveptr, *line = strtok_r(file_in_memory, "\n\r", &saveptr);
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
				line = strtok_r(NULL, "\n\r", &saveptr);
				if (line != NULL) {
					if (strlen(line) != 1) {
						has_after_part = 1;
					}
				}
				continue;
			}
		}
		line = strtok_r(NULL, "\n\r", &saveptr);
		if (line != NULL && has_parts_begin == 0) {
			while (line[0] == ' ' && lexeme < LEXEME_BOUNDARY) {
				if (expand_data(lexeme, state, line, &data) != EXIT_SUCCESS) {
					return EXIT_FAILURE;
				}
				line = strtok_r(NULL, "\n\r", &saveptr);
			}
		}
		if (!((is_rule_state_boundary(rule)) && data.boundary == NULL)) {
			state = rule.state;
		}

		if (is_state_boundary(state)) {
			has_found_boundary = 1;
		}
	}

	if (data.parts != 0 && has_after_part == 0) {
		data.parts--;
	} else if (has_body == 0) {
		data.parts = 0;
	} else if (data.parts == 0) {
		data.parts = 1;
	}

	*input_data = data;
	return EXIT_SUCCESS;
}

// Main functions
int parse_email(const char* filename, data_t* input_data) {
	size_t length = 0;
	char* file_in_memory = get_memory_from_file(filename, &length);
	if (file_in_memory == NULL) {
		return EXIT_FAILURE;
	}

	int has_body = 0, has_found_next_line = 0, has_found_empty_line = 0;
	for (size_t i = 0; i < length; i++) {
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

	if (get_data(file_in_memory, input_data, has_body) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void print_data(data_t data) {
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
}
