#include "matrix.h"

Matrix* create_matrix_from_file(const char* path_file) {
  FILE *file = fopen(path_file, "r");
  if (file == NULL) {
    return NULL;
  }

  size_t rows, cols;
  if (fscanf(file, "%zu %zu", &rows, &cols) != 2) {
    fclose(file);
    return NULL;
  }

  Matrix* new_matrix = create_matrix(rows, cols);
  if (new_matrix == NULL) {
    fclose(file);
    return NULL;
  }

  size_t has_forund_error = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if (fscanf(file, "%lf", &new_matrix->data[i][j]) != 1) {
        has_forund_error = 1;
        break;
      }
    }
  }

  fclose(file);
  return has_forund_error ? NULL : new_matrix;
}

Matrix* create_matrix(size_t rows, size_t cols) {
  Matrix* new_matrix = malloc(sizeof(Matrix));
  if (new_matrix == NULL) {
    return NULL;
  }

  new_matrix->rows = rows;
  new_matrix->cols = cols;

  new_matrix->data = (double**) calloc(rows, sizeof(double*));
  if (new_matrix->data == NULL) {
    free(new_matrix);
    return NULL;
  }

  size_t has_forund_error = 0, index_error = 0;
  for (size_t i = 0; i < rows; i++) {
    new_matrix->data[i] = (double*) calloc(cols, sizeof(double));
    if (new_matrix->data[i] == NULL) {
      index_error = i;
      has_forund_error = 1;
      break;
    }
  }

  if (has_forund_error) {
    for (size_t j = 0; j < index_error; j++) {
      free(new_matrix->data[j]);
    }
    free(new_matrix->data);
    free(new_matrix);
    return NULL;
  }
  return new_matrix;
}

int free_matrix(Matrix *matrix) {
  if (matrix == NULL) {
    return 1;
  }

  for (size_t i = 0; i < matrix->rows; i++) {
    free(matrix->data[i]);
  }

  free(matrix->data);
  free(matrix);
  return 0;
}

int get_rows(const Matrix* matrix, size_t* rows) {
  if (matrix == NULL || rows == NULL) {
    return 1;
  }

  *rows = matrix->rows;

  return 0;
}

int get_cols(const Matrix* matrix, size_t* cols) {
  if (matrix == NULL || cols == NULL) {
    return 1;
  }

  *cols = matrix->cols;

  return 0;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
  if (matrix == NULL || row > matrix->rows || col > matrix->cols || val == NULL) {
    return 1;
  }

  *val = matrix->data[row][col];

  return 0;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
  if (matrix == NULL || row > matrix->rows || col > matrix->cols) {
    return 1;
  }

  matrix->data[row][col] = val;

  return 0;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
  if (matrix == NULL) {
    return NULL;
  }

  Matrix* new_matrix = create_matrix(matrix->rows, matrix->cols);
  if (new_matrix == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < matrix->rows; i++) {
    for (size_t j = 0; j < matrix->cols; j++) {
      new_matrix->data[i][j] = matrix->data[i][j] * val;
    }
  }

  return new_matrix;
}

Matrix* transp(const Matrix* matrix) {
  if (matrix == NULL) {
    return NULL;
  }

  Matrix* new_matrix = create_matrix(matrix->cols, matrix->rows);
  if (new_matrix == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < matrix->cols; i++) {
    for (size_t j = 0; j < matrix->rows; j++) {
      new_matrix->data[i][j] = matrix->data[j][i];
    }
  }

  return new_matrix;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL || l->rows != r->rows || l->cols != r->cols) {
    return NULL;
  }

  Matrix* new_matrix = create_matrix(r->rows, r->cols);
  if (new_matrix == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < new_matrix->rows; i++) {
    for (size_t j = 0; j < new_matrix->cols; j++) {
      new_matrix->data[i][j] = l->data[i][j] + r->data[i][j];
    }
  }

  return new_matrix;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL || l->rows != r->rows || l->cols != r->cols) {
    return NULL;
  }

  Matrix* new_matrix = create_matrix(r->rows, r->cols);
  if (new_matrix == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < new_matrix->rows; i++) {
    for (size_t j = 0; j < new_matrix->cols; j++) {
      new_matrix->data[i][j] = l->data[i][j] - r->data[i][j];
    }
  }

  return new_matrix;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
  if (l == NULL || r == NULL || l->cols != r->rows) {
    return NULL;
  }

  Matrix* new_matrix = create_matrix(l->rows, r->cols);
  if (new_matrix == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < l->rows; i++) {
    for (size_t j = 0; j < r->cols; j++) {
      new_matrix->data[i][j] = 0;
      for (size_t k = 0; k < l->cols; k++) {
        new_matrix->data[i][j] += l->data[i][k] * r->data[k][j];
      }
    }
  }

  return new_matrix;
}

int det(const Matrix* matrix, double* val) {
  if (matrix == NULL || matrix->rows != matrix->cols) {
    return 1;
  }

  Matrix* new_matrix = create_matrix(matrix->rows, matrix->cols);
  if (new_matrix == NULL) {
    return 1;
  }

  for (size_t i = 0; i < matrix->rows; i++) {
    for (size_t j = 0; j < matrix->rows; j++) {
      new_matrix->data[i][j] = matrix->data[i][j];
    }
  }

  *val = 1;
  double eps = 1e-9, *temp;
  size_t has_stopped = 0;
  for (size_t i = 0, j = 0; i < matrix->rows && j < matrix->cols; i++, j++) {
    double item = 0.0;
    size_t item_row = 0;

    for (size_t k = i; k < matrix->rows; k++) {
      if (new_matrix->data[k][j] * new_matrix->data[k][j] > eps) {
        item_row = k;
        item = new_matrix->data[k][j];
        break;
      }
    }

    if (item * item <= eps) {
      has_stopped = 1;
      break;
    }

    if (item_row != i) {
      temp = new_matrix->data[i];
      new_matrix->data[i] = new_matrix->data[item_row];
      new_matrix->data[item_row] = temp;
      *val = 0 - (*val);
    }

    for (size_t k = i + 1; k < matrix->rows; k++) {
      item = -new_matrix->data[k][j] / new_matrix->data[i][j];
      new_matrix->data[k][j] = 0.0;
      for (size_t l = j + 1; l < matrix->cols; l++) {
        new_matrix->data[k][l] += item * new_matrix->data[i][l];
      }
    }
  }

  if (has_stopped) {
    *val = 0;
    free_matrix(new_matrix);
    return 0;
  }

  for (size_t i = 0; i < matrix->rows; i++) {
    *val *= new_matrix->data[i][i];
  }

  free_matrix(new_matrix);
  return 0;
}

Matrix* adj(const Matrix* matrix) {
  if (matrix == NULL) {
    return NULL;
  }

  size_t rows, cols;
  get_rows(matrix, &rows);
  get_cols(matrix, &cols);
  if (rows != cols) {
    return NULL;
  }

  Matrix* new_matrix = create_matrix(rows, cols);
  if (new_matrix == NULL) {
    return NULL;
  }

  Matrix* help_matrix = create_matrix(rows - 1, cols - 1);
  if (help_matrix == NULL) {
    free_matrix(new_matrix);
    return NULL;
  }

  double val;
  size_t has_stopped = 0;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      for (size_t k = 0, itr; k < rows - 1; k++) {
        itr = k >= i ? k + 1 : k;
        for (size_t l = 0, jtr; l < cols - 1; l++) {
          jtr = l >= j ? l + 1 : l;
          help_matrix->data[k][l] = matrix->data[itr][jtr];
        }
      }

      val = 0;
      new_matrix->data[j][i] = 0 - new_matrix->data[j][i];
      if (det(help_matrix, &val) != 0) {
        has_stopped = 1;
        free_matrix(new_matrix);
        break;
      }

      new_matrix->data[j][i] = val;

      if ((i + j) % 2 == 0) {
        new_matrix->data[j][i] = 0 - new_matrix->data[j][i];
      }
      new_matrix->data[j][i] = 0 - new_matrix->data[j][i];
    }
    if (has_stopped) {
      break;
    }
  }

  free_matrix(help_matrix);
  return has_stopped ? NULL : new_matrix;
}

Matrix* inv(const Matrix* matrix) {
  if (matrix == NULL || matrix->rows != matrix->cols) {
    return NULL;
  }

  Matrix* adjointed_matrix = adj(matrix);
  if (adjointed_matrix == NULL) {
    return NULL;
  }

  double val;
  if (det(matrix, &val) != 0) {
    free_matrix(adjointed_matrix);
    return NULL;
  }

  Matrix* new_matrix = mul_scalar(adjointed_matrix, 1 / val);
  if (new_matrix == NULL) {
    free_matrix(adjointed_matrix);
    return NULL;
  }

  free_matrix(adjointed_matrix);
  return new_matrix;
}
