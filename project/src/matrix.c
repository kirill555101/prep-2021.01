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

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if (fscanf(file, "%lf", &new_matrix->data[i][j]) != 1) {
        fclose(file);
        free_matrix(new_matrix);
        return NULL;
      }
    }
  }

  fclose(file);
  return new_matrix;
}

Matrix* create_matrix(size_t rows, size_t cols) {
  Matrix* new_matrix = calloc(1, sizeof(Matrix));
  if (new_matrix == NULL) {
    return NULL;
  }

  new_matrix->rows = rows;
  new_matrix->cols = cols;

  new_matrix->data = (double**) calloc(rows, sizeof(double*));
  if (new_matrix->data == NULL) {
    free_matrix(new_matrix);
    return NULL;
  }

  for (size_t i = 0; i < rows; i++) {
    new_matrix->data[i] = (double*) calloc(cols, sizeof(double));
    if (new_matrix->data[i] == NULL) {
      free_matrix(new_matrix);
      return NULL;
    }
  }

  return new_matrix;
}

int free_matrix(Matrix *matrix) {
  if (matrix == NULL) {
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < matrix->rows; i++) {
    if (matrix->data[i] != NULL) {
      free(matrix->data[i]);
    }
  }

  if (matrix->data != NULL) {
    free(matrix->data);
  }

  free(matrix);
  return EXIT_SUCCESS;
}

int get_rows(const Matrix* matrix, size_t* rows) {
  if (matrix == NULL || rows == NULL) {
    return EXIT_FAILURE;
  }

  *rows = matrix->rows;

  return EXIT_SUCCESS;
}

int get_cols(const Matrix* matrix, size_t* cols) {
  if (matrix == NULL || cols == NULL) {
    return EXIT_FAILURE;
  }

  *cols = matrix->cols;

  return EXIT_SUCCESS;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
  if (matrix == NULL || row > matrix->rows - 1 || col > matrix->cols - 1 || val == NULL) {
    return EXIT_FAILURE;
  }

  *val = matrix->data[row][col];

  return EXIT_SUCCESS;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
  if (matrix == NULL || row > matrix->rows - 1 || col > matrix->cols - 1) {
    return EXIT_FAILURE;
  }

  matrix->data[row][col] = val;

  return EXIT_SUCCESS;
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
    return EXIT_FAILURE;
  }

  size_t size = matrix->rows;
  Matrix* new_matrix = create_matrix(size, size);
  if (new_matrix == NULL) {
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < matrix->rows; i++) {
    for (size_t j = 0; j < matrix->rows; j++) {
      new_matrix->data[i][j] = matrix->data[i][j];
    }
  }

  int sign = 1;
  double eps = 1e-9, *temp;
  for (size_t i = 0, i_max = 0; i < size; i++) {
    i_max = i;
    for (size_t j = i + 1; j < size; j++) {
      if (fabs(new_matrix->data[j][i]) > fabs(new_matrix->data[i_max][i])) {
        i_max = j;
      }
    }

    if (fabs(new_matrix->data[i_max][i]) < eps) {
      *val = 0;
      free_matrix(new_matrix);
      return EXIT_SUCCESS;
    }

    temp = new_matrix->data[i];
    new_matrix->data[i] = new_matrix->data[i_max];
    new_matrix->data[i_max] = temp;

    sign = i != i_max ? -sign : sign;

    for (size_t j = i + 1; j < size; j++) {
      double mul_num = -new_matrix->data[j][i] / new_matrix->data[i][i];
      for (size_t k = i; k < size; k++) {
        new_matrix->data[j][k] += new_matrix->data[i][k] * mul_num;
      }
    }
  }

  double res = 1;
  for (size_t i = 0; i < size; i++) {
    res *= new_matrix->data[i][i];
  }
  *val = sign * res;

  free_matrix(new_matrix);
  return EXIT_SUCCESS;
}

Matrix* adj(const Matrix* matrix) {
  if (matrix == NULL || matrix->rows != matrix->cols) {
    return NULL;
  }

  size_t size = matrix->rows;

  Matrix* new_matrix = create_matrix(size, size);
  if (new_matrix == NULL) {
    return NULL;
  }

  double val = 0;
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      if (get_minor(matrix, i, j, &val) != EXIT_SUCCESS) {
        free_matrix(new_matrix);
        return NULL;
      }

      new_matrix->data[j][i] = pow(-1, i + j) * val;
    }
  }

  return new_matrix;
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
  if (det(matrix, &val) != EXIT_SUCCESS) {
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

int get_minor(const Matrix* matrix, size_t row, size_t col, double* val) {
  if (matrix == NULL || row > matrix->rows - 1 || col > matrix->cols - 1 ||
    matrix->rows != matrix->cols || val == NULL) {
      return EXIT_FAILURE;
  }

  size_t size = matrix->rows;
  Matrix* new_matrix = create_matrix(size - 1, size - 1);
  if (new_matrix == NULL) {
    return EXIT_FAILURE;
  }

  for (size_t i = 0, itr = 0; i < size - 1; i++) {
    itr = i >= row ? i + 1 : i;
    for (size_t j = 0, jtr = 0; j < size - 1; j++) {
      jtr = j >= col ? j + 1 : j;
      new_matrix->data[i][j] = matrix->data[itr][jtr];
    }
  }

  if (det(new_matrix, val) != EXIT_SUCCESS) {
    free_matrix(new_matrix);
    return EXIT_FAILURE;
  }

  free_matrix(new_matrix);
  return EXIT_SUCCESS;
}
