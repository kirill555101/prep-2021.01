#define _GNU_SOURCE

#include "parser.h"

// Store function and the state itself
lexeme_t get_lexeme(char* line, int* state) {
	if (line == NULL) {
		return LEXEME_NO;
	}

	if (state[STATE_FROM] == 0 && strncasecmp(line, FROM_STR, strlen(FROM_STR)) == 0) {
		return LEXEME_FROM;
	}

	if (state[STATE_TO] == 0 && strncasecmp(line, TO_STR, strlen(TO_STR)) == 0) {
		return LEXEME_TO;
	}

	if (state[STATE_DATE] == 0 && strncasecmp(line, DATE_STR, strlen(DATE_STR)) == 0) {
		return LEXEME_DATE;
	}

	if (strncmp(line, CONTENT_TYPE_STR, strlen(CONTENT_TYPE_STR)) == 0) {
		state[STATE_CONTENT_TYPE] = 1;
	}

	if (state[STATE_CONTENT_TYPE] == 1 && state[STATE_BOUNDARY] == 0 && strcasestr(line, BOUNDARY_STR) != NULL) {
		state[STATE_CONTENT_TYPE] = 0;
		return LEXEME_BOUNDARY;
	}

	return LEXEME_NO;
}

int store(char* line, data_t* data, lexeme_t lexeme, int* state) {
	if (line == NULL || data == NULL) {
		return EXIT_FAILURE;
	}

	char *res, *save_ptr;
	if (lexeme != LEXEME_BOUNDARY) {
		strtok_r(line, ":", &save_ptr);
		res = strtok_r(NULL, "\r\n", &save_ptr);
		res = res + strspn(res, " ");
		if (res == NULL) {
			return EXIT_FAILURE;
		}
	} else {
		res = strcasestr(line, BOUNDARY_STR);
		if (res == NULL) {
			return EXIT_FAILURE;
		}
		if (res != line) {
			if ((*(res - 1) != ' ') && (*(res - 1) != '\t') && (*(res - 1) != ';')) {
				return EXIT_SUCCESS;
			}
		}

		res = res + strlen(BOUNDARY_STR) + strspn(res, " ");
		if (res[0] == '"') {
			res++;
			res = strtok_r(res, "\"", &save_ptr);
		} else {
			res = strtok_r(res, "\r\n ", &save_ptr);
		}
	}

	switch (lexeme) {
		case LEXEME_FROM:
			if(state[STATE_FROM] == 0) {
				data->from = res;
				state[STATE_FROM] = 1;
			}
			break;
		case LEXEME_TO:
			if(state[STATE_TO] == 0) {
				data->to = res;
				state[STATE_TO] = 1;
			}
			break;
		case LEXEME_DATE:
			if(state[STATE_DATE] == 0) {
				data->date = res;
				state[STATE_DATE] = 1;
			}
			break;
		case LEXEME_BOUNDARY:
			if(state[STATE_BOUNDARY] == 0) {
				data->boundary = res;
				state[STATE_BOUNDARY] = 1;
			}
			break;
		default:
			break;
	}

	return EXIT_SUCCESS;
}

// Helpful functions
char* get_memory_from_file(const char* filename, size_t* length) {
	if (filename == NULL || length == NULL) {
		return NULL;
	}

	int fd = open(filename, O_RDONLY);
	struct stat sb;

	if (fstat(fd, &sb) == -1) {
		close(fd);
		return NULL;
	}

	char* file_in_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
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

	char* new = calloc(1, strlen(str) + strlen(line) + 1);
	if (new == NULL) {
		return NULL;
	}

	char *save_ptr, *res = strtok_r(str, "\n\r", &save_ptr);
	strncpy(new, res, strlen(res));
	strncat(new, line, strlen(line));
	strncpy(res, new, strlen(new) + 1);
	free(new);

	return res;
}

int expand_data(lexeme_t lexeme, const char* line, data_t* data) {
	if (line == NULL || data == NULL) {
		return EXIT_FAILURE;
	}

	switch (lexeme) {
		case LEXEME_FROM:
			data->from = expand_str(line, data->from);
			break;
		case LEXEME_TO:
			data->to = expand_str(line, data->to);
			break;
		case LEXEME_DATE:
			data->date = expand_str(line, data->date);
			break;
		default:
			break;
	}

	return EXIT_SUCCESS;
}

int copy_data(data_t* input_data, const data_t* output_data) {
	if (input_data == NULL || output_data == NULL) {
		return EXIT_FAILURE;
	}
	if (output_data->from != NULL) {
		input_data->from = calloc(1, strlen(output_data->from) + 1);
		if (input_data->from == NULL) {
			return EXIT_FAILURE;
		}
		strncpy(input_data->from, output_data->from, strlen(output_data->from) + 1);
	}

	if (output_data->to != NULL) {
		input_data->to = calloc(1, strlen(output_data->to) + 1);
		if (input_data->to == NULL) {
			free(input_data->from);
			return EXIT_FAILURE;
		}
		strncpy(input_data->to, output_data->to, strlen(output_data->to) + 1);
	}

	if (output_data->date != NULL) {
		input_data->date = calloc(1, strlen(output_data->date) + 1);
		if (input_data->date == NULL) {
			free(input_data->from);
			free(input_data->to);
			return EXIT_FAILURE;
		}
		strncpy(input_data->date, output_data->date, strlen(output_data->date) + 1);
	}
	input_data->parts = output_data->parts;
	return EXIT_SUCCESS;
}

int get_data(const char* given_file_in_memory, data_t* input_data, int has_body) {
	if (given_file_in_memory == NULL || input_data == NULL) {
		return EXIT_FAILURE;
	}

	char* file_in_memory = calloc(1, strlen(given_file_in_memory) + 1);
	if (file_in_memory == NULL) {
		return EXIT_FAILURE;
	}
	strncpy(file_in_memory, given_file_in_memory, strlen(given_file_in_memory) + 1);

	data_t data = {0};
	lexeme_t lexeme;
	int state[STATE_COUNT] = {0};

	int has_found_boundary = 0, has_after_part = 0, has_parts_begin = 0, has_body_begin = 0;
	char *save_ptr, *line = strtok_r(file_in_memory, "\n\r", &save_ptr);
	while (line != NULL) {
		lexeme = get_lexeme(line, state);

		if (lexeme < LEXEME_COUNT) {
			if (store(line, &data, lexeme, state) != EXIT_SUCCESS) {
				free(file_in_memory);
				return EXIT_FAILURE;
			}
		}

		if (has_body == 1 && strchr(line, ':') == NULL && line[0] != ' ') {
			has_body_begin = 1;
		}

		if (has_body_begin == 1 && has_found_boundary == 1) {
			if (data.boundary != NULL && strstr(line, data.boundary) != NULL) {
				has_parts_begin = 1;
				has_after_part = 0;
				data.parts++;
			}
			if (has_parts_begin == 1) {
				line = strtok_r(NULL, "\n\r", &save_ptr);
				if (line != NULL) {
					if (strlen(line) != 1) {
						has_after_part = 1;
					}
				}
				continue;
			}
		}

		line = strtok_r(NULL, "\n\r", &save_ptr);
		if (line != NULL && has_parts_begin == 0) {
			while (line[0] == ' ' && lexeme < LEXEME_BOUNDARY) {
				if (expand_data(lexeme, line, &data) != EXIT_SUCCESS) {
					free(file_in_memory);
					return EXIT_FAILURE;
				}
				line = strtok_r(NULL, "\n\r", &save_ptr);
			}
		}

		if (lexeme == LEXEME_BOUNDARY) {
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

	if (copy_data(input_data, &data) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	free(file_in_memory);
	return EXIT_SUCCESS;
}

// Main functions
int parse_email(const char* filename, data_t* input_data) {
	if (filename == NULL || input_data == NULL) {
		return EXIT_FAILURE;
	}

	size_t length = 0;
	char* file_in_memory = get_memory_from_file(filename, &length);
	if (file_in_memory == NULL) {
		return EXIT_FAILURE;
	}

	int has_body = 0, has_found_next_line = 0, has_found_empty_line = 0;
	for (size_t i = 0; i < length; ++i) {
		if (has_found_empty_line == 1 && file_in_memory[i] != '\n' &&
			file_in_memory[i] != '\r' && file_in_memory[i] != '\0') {
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

int free_data(data_t* data) {
	if (data == NULL) {
		return EXIT_FAILURE;
	}

	if (data->from != NULL) {
		free(data->from);
	}

	if (data->to != NULL) {
		free(data->to);
	}

	if (data->date != NULL) {
		free(data->date);
	}

	return EXIT_SUCCESS;
}
