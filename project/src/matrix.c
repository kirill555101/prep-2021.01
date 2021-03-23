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
        return NULL;
      }
    }
  }

  fclose(file);
  return new_matrix;
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
    return error;
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
  return success;
}

int get_rows(const Matrix* matrix, size_t* rows) {
  if (matrix == NULL || rows == NULL) {
    return error;
  }

  *rows = matrix->rows;

  return success;
}

int get_cols(const Matrix* matrix, size_t* cols) {
  if (matrix == NULL || cols == NULL) {
    return error;
  }

  *cols = matrix->cols;

  return success;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
  if (matrix == NULL || row > matrix->rows - 1 || col > matrix->cols || val == NULL) {
    return error;
  }

  *val = matrix->data[row][col];

  return success;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
  if (matrix == NULL || row > matrix->rows - 1 || col > matrix->cols) {
    return error;
  }

  matrix->data[row][col] = val;

  return success;
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
    return error;
  }

  Matrix* new_matrix = create_matrix(matrix->rows, matrix->cols);
  if (new_matrix == NULL) {
    return error;
  }

  for (size_t i = 0; i < matrix->rows; i++) {
    for (size_t j = 0; j < matrix->rows; j++) {
      new_matrix->data[i][j] = matrix->data[i][j];
    }
  }

  *val = 1;
  size_t size = matrix->rows;
  double eps = 1e-9, *temp;
  for (size_t i = 0, k = 0; i < size; i++) {
    k = i;
	  for (size_t j = i + 1; j < size; j++) {
		  if (fabs(new_matrix->data[j][i]) > fabs(new_matrix->data[k][i])) {
        k = j;
      }
    }

	  if (fabs(new_matrix->data[k][i]) < eps) {
      *val = 0;
      free_matrix(new_matrix);
      return success;
	  }

	  temp = new_matrix->data[i];
    new_matrix->data[i] = new_matrix->data[k];
    new_matrix->data[k] = temp;

	  if (i != k) {
		  *val = 0 - *val;
    }

	  *val *= new_matrix->data[i][i];
	  for (size_t j = i  + 1; j < size ; j++) {
		  new_matrix->data[i][j] /= new_matrix->data[i][i];
    }

    for (size_t j = 0; j < size; j++) {
      if (j != i && fabs(new_matrix->data[j][i]) > eps) {
        for (size_t k = i + 1; k < size; k++) {
          new_matrix->data[j][k] -= new_matrix->data[i][k] * new_matrix->data[j][i];
        }
      }
    }
  }

  free_matrix(new_matrix);
  return success;
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

  Matrix* help_matrix = create_matrix(size - 1, size - 1);
  if (help_matrix == NULL) {
    free_matrix(new_matrix);
    return NULL;
  }

  double val = 0;
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      for (size_t _i = 0, itr = 0; _i < size - 1; _i++) {
        itr = _i >= i ? _i + 1 : _i;
        for (size_t _j = 0, jtr = 0; _j < size - 1; _j++) {
          jtr = _j >= j ? _j + 1 : _j;
          help_matrix->data[_i][_j] = matrix->data[itr][jtr];
        }
      }

      if (det(help_matrix, &val) != success) {
        free_matrix(new_matrix);
        free_matrix(help_matrix);
        return NULL;
      }

      new_matrix->data[j][i] = pow(-1, i + j) * val;
    }
  }

  free_matrix(help_matrix);
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
  if (det(matrix, &val) != success) {
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
