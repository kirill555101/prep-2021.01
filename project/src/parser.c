#include "parser.h"

char* parse_email(FILE* file) {
  if (file == NULL) {
    return NULL;
  }
  
  char** types_arr = make_array(4);
  if (types_arr == NULL) {
    return NULL;
  }

  char* res = calloc(MAX_LINE_LENGTH, sizeof(char));
  if (res == NULL) {
    return NULL;
  }

  char line[MAX_LINE_LENGTH] = {0};
  char new_line[MAX_LINE_LENGTH] = {0};
  char val[MAX_LINE_LENGTH] = {0};

  HEADER_NAME name = 0;
  char *begin_pos = NULL, *end_pos = NULL, *pos = NULL, *start = NULL;
  fgets(new_line, MAX_LINE_LENGTH, file);
  while(line[0] != '\n' && line[0] != '\r') {
    strcpy(line, new_line);
    if (parse_line(line, val, &name) != EXIT_SUCCESS) {
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
      strcpy(line, new_line);
      


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
    size_t count = 0;
    rewind(file);

    if (get_count_of_content_type(file, types_arr[3], &count) == EXIT_SUCCESS) {
      char num_str[3] = "";
      sprintf(num_str, "%ld", count - 1);
      strcat(res, num_str);
    }
  } else {
    strcat(res, types_arr[3]);
  }

  free_array(types_arr, 4);

  return res;
}

int parse_line(const char* line, char* res, HEADER_NAME* name) {
  if (line == NULL || res == NULL || name == NULL) {
    return EXIT_FAILURE;
  }

  const char* types_arr[4] = {
    "from:", "to:",
    "date:", "content-type:"
  };

  char *pos = NULL, *lowered_str = NULL;
  if ((lowered_str = str_to_lower(line)) == NULL) {
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < 4; i++) {
    if ((pos = strstr(lowered_str, types_arr[i])) != NULL && (pos - lowered_str) == 0 ) {
      pos = pos + strlen(types_arr[i]);
      
      if (isspace(lowered_str[pos - lowered_str])) {
        pos++;
      }
      strcpy(res, line + (pos - lowered_str));

      *name = i;
      free(lowered_str);
      return EXIT_SUCCESS;
    }
  }

  free(lowered_str);
  return EXIT_FAILURE;
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

int get_count_of_content_type(FILE* file, const char* content, size_t* val) {
  if (file == 0 || content == 0 || val == NULL) {
    return EXIT_FAILURE;
  }

  size_t count = 0;
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

  *val = count;

  return EXIT_SUCCESS;
}

char** make_array(size_t length) {
  char** array = calloc(length, sizeof(char*));
  if (array == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < 4; i++) {
    if ((array[i] = calloc(3 * MAX_LINE_LENGTH / 4, sizeof(char))) == NULL) {
      free_array(array, 4);
      return NULL;
    }
  }

  return array;
}

int free_array(char** array, size_t length) {
  if(array == NULL) {
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < length; i++) {
    if (array[i] != NULL) {
      free(array[i]);
    }
  }

  free(array);
  return EXIT_SUCCESS;
}
