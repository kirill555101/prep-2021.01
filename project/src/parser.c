#include "parser.h"

char* parse_email(FILE* file) {
  if (file == NULL) {
    return NULL;
  }

  char* types_arr[4] = {0};
  for (size_t i = 0; i < 4; i++) {
    if ((types_arr[i] = calloc(3 * MAX_LINE_LENGTH / 4, sizeof(char))) == NULL) {
      for (size_t j = 0; j < i; j++) {
        free(types_arr[j]);
      }
      return NULL;
    }
  }

  char* res = calloc(5 * MAX_LINE_LENGTH, sizeof(char));
  char line[5 * MAX_LINE_LENGTH] = {0};
  char val[MAX_LINE_LENGTH] = {0};

  HEADER_NAME name;
  fgets(line, 5 * MAX_LINE_LENGTH, file);
  while((line[0] != '\n' && line[0] != '\r') || strlen(line) > 3) {
    if (!has_parsed_line(line, val, &name)) {
      fgets(line, MAX_LINE_LENGTH, file);
      continue;
    }

    char* end_pos = strstr(val, "\n");
    if (end_pos != NULL) {
      val[(end_pos - val)] = '\0';
    }

    if (name < content_type) {
      strcpy(types_arr[name], val); 
    } else {
      char *pos = NULL;

      if ((pos = strstr(val, "multipart")) != NULL) {
        
      } else {
        strcpy(types_arr[name], "1");
      }
    }

    fgets(line, MAX_LINE_LENGTH, file);
  }
  
  if (line[0] != '\n' && line[0] != '\r') {
    fgets(line, MAX_LINE_LENGTH, file);
  }

  strcpy(res, "");
  for (size_t i = 0; i < 3; i++) {
    strcat(res, types_arr[i]);
    strcat(res, "|");
  }

  for (size_t i = 0; i < 4; i++) {
    free(types_arr[i]);
  }

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