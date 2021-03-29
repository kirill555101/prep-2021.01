#include "parser.h"

char* parse_email(FILE* file) {
  if (file == NULL) {
    return NULL;
  }
  
  char* types_arr[4] = {0};
  for (size_t i = 0; i < 4; i++) {
    if ((types_arr[i] = calloc(3 * MAX_LINE_LENGTH / 4, sizeof(char))) == NULL) {
      free_array(types_arr, 4);
      return NULL;
    }
  }

  char* res = calloc(MAX_LINE_LENGTH, sizeof(char));
  char line[MAX_LINE_LENGTH] = {0};
  char new_line[MAX_LINE_LENGTH] = {0};
  char val[MAX_LINE_LENGTH] = {0};

  HEADER_NAME name;
  char *begin_pos = NULL, *end_pos = NULL, *pos = NULL, *start = NULL;
  fgets(new_line, MAX_LINE_LENGTH, file);
  while(line[0] != '\n' && line[0] != '\r') {
    strcpy(line, new_line);
    if (!has_parsed_line(line, val, &name)) {
      fgets(new_line, MAX_LINE_LENGTH, file);
      continue;
    }

    end_pos = strstr(val, "\n");
    if (end_pos != NULL) {
      val[(end_pos - val)] = '\0';
    }

    fgets(new_line, MAX_LINE_LENGTH, file);
    if (isspace(new_line[0]) && isspace(new_line[1])) {
      size_t i = 0;
      for (i = 0; isspace(line[i]); i++) ;
      strcat(val, new_line + i);

      end_pos = strstr(val, "\n");
      if (end_pos != NULL) {
        val[(end_pos - val)] = '\0';
      }
    }
    else if (isspace(new_line[0])) {
      strcat(val, new_line);

      end_pos = strstr(val, "\n");    
      if (end_pos != NULL) {
        val[(end_pos - val)] = '\0';
      }
    }

    if (name < content_type) {
      strcpy(types_arr[name], val);
    } else {
      if ((pos = strstr(val, "multipart")) != NULL) {
        strcat(val, new_line);
        end_pos = strstr(val, "\n");
        if (end_pos != NULL) {
          val[(end_pos - val)] = '\0';
        }

        begin_pos = strstr(pos, "boundary=");
        end_pos = strrchr(val, '"');
        start = begin_pos + strlen("boundary=");
        if (end_pos != NULL) {
          strncpy(types_arr[name], start + 1, end_pos - start - 1);
        } else {
          strcpy(types_arr[name], start);
        }
      } else {
        strcpy(types_arr[name], "1");
      }
    }
  }
  
  if (line[0] != '\n' && line[0] != '\r') {
    fgets(new_line, MAX_LINE_LENGTH, file);
  }

  if (feof(file)) {
    strcpy(types_arr[name], "0");
  }

  strcpy(res, "");
  for (size_t i = 0; i < 3; i++) {
    strcat(res, types_arr[i]);
    strcat(res, "|");
  }

  if (strcmp(types_arr[3], "0") != 0 && strcmp(types_arr[3], "1") != 0) {
    size_t count = get_count(file, types_arr[3]);
    
    char num_str[3] = "";
    sprintf(num_str, "%ld", count);
    strcat(res, num_str);
  } else {
    strcat(res, types_arr[3]);
  }

  free_array(types_arr, 4);

  return res;
}

int has_parsed_line(const char* line, char* res, HEADER_NAME* name) {
  if (line == NULL || res == NULL || name == NULL) {
    return 0;
  }

  const char* types[4] = {
    "from:", "to:",
    "date:", "content-type:"
  };

  char *pos = NULL, *lowered_str;
  if ((lowered_str = str_to_lower(line)) == NULL) {
    *name = 0;
    return 0;
  }

  for (size_t i = 0; i < 4; i++) {
    if ((pos = strstr(lowered_str, types[i])) != NULL && (pos - lowered_str) == 0 ) {
      pos = pos + strlen(types[i]);
      if (isspace(lowered_str[pos - lowered_str])) {
        pos++;
      }
      strcpy(res, line + (pos - lowered_str));
      *name = i;
      free(lowered_str);
      return 1;
    }
  }

  free(lowered_str);
  
  *name = 0;
  return 0;
}

char* str_to_lower(const char* str) {
  if (str == NULL) {
    return NULL;
  }

  char* res = calloc(strlen(str), sizeof(char));
  if (res == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < strlen(str); i++) {
    res[i] = tolower(str[i]);
  }

  return res;
}

int free_array(char* array[], size_t length) {
  if(array == NULL) {
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < length; i++) {
    if (array[i] != NULL) {
      free(array[i]);
    }
  }

  return EXIT_SUCCESS;
}

size_t get_count(FILE* file, const char* content) {
  size_t count = 1;
  char line[MAX_LINE_LENGTH] = {0};
  while (!feof(file)) {
    char* pos = strstr(line, content);
    if (pos != NULL) {
      if (line[(pos + strlen(content) - line)] != '-') {
        count++;
      }
    }

    fgets(line, MAX_LINE_LENGTH, file);
  }

  return count;
}