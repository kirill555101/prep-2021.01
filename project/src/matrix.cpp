#include "matrix.h"
#include "exceptions.h"

namespace prep {

  Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    data.resize(rows);

      for (size_t i = 0; i < rows; i++) {
        data[i].resize(cols, 0);
      }
  }

  Matrix::Matrix(std::istream& is) {
    if (!is) {
      throw(InvalidMatrixStream());
    }

    is >> rows >> cols;

    if (!is) {
      throw(InvalidMatrixStream());
    }

    data.resize(rows);

    for (size_t i = 0; i < rows; i++) {
      data[i].resize(cols);
      for (size_t j = 0; j < cols; j++) {
        is >> data[i][j];
        if (!is) {
          throw(InvalidMatrixStream());
        }
      }
    }
  }

  size_t Matrix::getRows() const {
      return rows;
  }

  size_t Matrix::getCols() const {
      return cols;
  }

  double Matrix::operator()(size_t i, size_t j) const {
    if (i >= rows || j >= cols) {
      throw OutOfRange(i, j, *this);
    }
    return data[i][j];
  }

  double& Matrix::operator()(size_t i, size_t j) {
    if (i >= rows || j >= cols) {
      throw OutOfRange(i, j, *this);
    }
    return data[i][j];
  }

  bool Matrix::operator==(const Matrix& rhs) const {
    if (this->rows != rhs.rows || this->cols != rhs.cols) {
      return false;
    }

    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        if (std::abs(this->data[i][j] - rhs.data[i][j]) > this->eps) {
          return false;
        }
      }
    }

    return true;
  }

  bool Matrix::operator!=(const Matrix& rhs) const {
    return !(*this == rhs);
  }

  Matrix Matrix::operator+(const Matrix& rhs) const {
    if (this->rows != rhs.rows || this->cols != rhs.cols) {
      throw(DimensionMismatch(rhs));
    }

    Matrix new_matrix(rows, cols);

    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->cols; j++) {
        new_matrix.data[i][j] = this->data[i][j] + rhs.data[i][j];
      }
    }

    return new_matrix;
  }

  Matrix Matrix::operator-(const Matrix& rhs) const {
    if (this->rows != rhs.rows || this->cols != rhs.cols) {
      throw(DimensionMismatch(rhs));
    }

    Matrix new_matrix(rows, cols);

    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->cols; j++) {
        new_matrix.data[i][j] = this->data[i][j] - rhs.data[i][j];
      }
    }

    return new_matrix;
  }

  Matrix Matrix::operator*(const Matrix& rhs) const {
    if (this->cols != rhs.rows) {
      throw(DimensionMismatch(rhs));
    }

    Matrix new_matrix(this->rows, rhs.cols);

    for (size_t i = 0; i < new_matrix.rows; i++) {
      for (size_t j = 0; j < new_matrix.cols; j++) {
        for (size_t k = 0; k < this->cols; k++) {
          new_matrix.data[i][j] += this->data[i][k] * rhs.data[k][j];
        }
      }
    }

    return new_matrix;
  }

  Matrix Matrix::operator*(double val) const {
    Matrix new_matrix(rows, cols);

    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        new_matrix.data[i][j] = val * data[i][j];
      }
    }

    return new_matrix;
  }

  Matrix Matrix::transp() const {
    Matrix new_matrix(this->cols, this->rows);

    for (size_t i = 0; i < new_matrix.cols; i++) {
      for (size_t j = 0; j < new_matrix.rows; j++) {
        new_matrix.data[j][i] = this->data[i][j];
      }
    }

    return new_matrix;
  }

  Matrix operator*(double val, const Matrix& matrix) {
    Matrix new_matrix(matrix.rows, matrix.cols);

    for (size_t i = 0; i < matrix.rows; i++) {
      for (size_t j = 0; j < matrix.cols; j++) {
        new_matrix.data[i][j] = val * matrix.data[i][j];
      }
    }

    return new_matrix;
  }

  std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.rows << " " << matrix.cols << std::endl;

    for (size_t i = 0; i < matrix.rows; i++) {
      for (size_t j = 0; j < matrix.cols; j++) {
        os << std::setprecision(std::numeric_limits<double>::max_digits10) << matrix.data[i][j];
        os << " ";
      }
      os << std::endl;
    }

    return os;
  }

  double Matrix::det() const {
    if (this->cols != this->rows) {
      throw(DimensionMismatch(*this));
    }

    Matrix new_matrix(this->rows, this->rows);

    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->rows; j++) {
        new_matrix.data[i][j] = this->data[i][j];
      }
    }

    int sign = 1;
    for (size_t i = 0, i_max = 0; i < this->rows; i++) {
      i_max = i;
      for (size_t j = i + 1; j < this->rows; j++) {
        if (std::fabs(new_matrix.data[j][i]) > std::fabs(new_matrix.data[i_max][i])) {
          i_max = j;
        }
      }

      if (std::fabs(new_matrix.data[i_max][i]) < this->eps) {
        return 0;
      }

      std::vector<double> temp = new_matrix.data[i];
      new_matrix.data[i] = new_matrix.data[i_max];
      new_matrix.data[i_max] = temp;

      sign = i != i_max ? -sign : sign;

      for (size_t j = i + 1; j < this->rows; j++) {
        double mul_num = -new_matrix.data[j][i] / new_matrix.data[i][i];
        for (size_t k = i; k < this->rows; k++) {
          new_matrix.data[j][k] += new_matrix.data[i][k] * mul_num;
        }
      }
    }

    double res = 1;
    for (size_t i = 0; i < this->rows; i++) {
      res *= new_matrix.data[i][i];
    }
    res *= sign;

    return res;
  }

  Matrix Matrix::adj() const {
    if (this->cols != this->rows) {
      throw(DimensionMismatch(*this));
    }

    Matrix new_matrix(this->rows, this->rows);
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->rows; j++) {
        new_matrix.data[j][i] = pow(-1, i + j) * this->get_minor(i, j);
      }
    }

    return new_matrix;
  }

  Matrix Matrix::inv() const {
    if (this->cols != this->rows) {
      throw(DimensionMismatch(*this));
    }

    Matrix adjointed_matrix(this->rows, this->cols);
    adjointed_matrix = this->adj();

    double det = this->det();
    if (std::fabs(det) < this->eps) {
      throw(SingularMatrix());;
    }

    return adjointed_matrix * (1 / det);
  }

  double Matrix::get_minor(size_t row, size_t col) const {
    if (this->cols != this->rows) {
      throw(DimensionMismatch(*this));
    }

    Matrix new_matrix(this->rows - 1, this->rows - 1);
    for (size_t i = 0, itr = 0; i < this->rows - 1; i++) {
      itr = i >= row ? i + 1 : i;
      for (size_t j = 0, jtr = 0; j < this->rows - 1; j++) {
        jtr = j >= col ? j + 1 : j;
        new_matrix.data[i][j] = this->data[itr][jtr];
      }
    }

    return new_matrix.det();
  }

}  // namespace prep

