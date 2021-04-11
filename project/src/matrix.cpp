#include "matrix.h"
#include "exceptions.h"

namespace prep {

  Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(rows, std::vector<double>(cols)) {}

  Matrix::Matrix(std::istream& is) {
    if (!is) {
      throw InvalidMatrixStream();
    }

    is >> rows >> cols;

    if (!is) {
      throw InvalidMatrixStream();
    }

    data.resize(rows);
    for (auto itr = data.begin(); itr < data.end(); ++itr) {
      itr->resize(cols);
      for (auto jtr = itr->begin(); jtr < itr->end(); ++jtr) {
        is >> *jtr;
        if (!is) {
          throw InvalidMatrixStream();
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

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
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
      throw DimensionMismatch(rhs);
    }

    Matrix newMatrix = *this;
    for (size_t i = 0; i < this->rows; ++i) {
      for (size_t j = 0; j < this->cols; ++j) {
        newMatrix.data[i][j] += rhs.data[i][j];
      }
    }

    return newMatrix;
  }

  Matrix Matrix::operator-(const Matrix& rhs) const {
    if (this->rows != rhs.rows || this->cols != rhs.cols) {
      throw DimensionMismatch(rhs);
    }

    Matrix newMatrix = *this;
    for (size_t i = 0; i < this->rows; ++i) {
      for (size_t j = 0; j < this->cols; ++j) {
        newMatrix.data[i][j] -= rhs.data[i][j];
      }
    }

    return newMatrix;
  }

  Matrix Matrix::operator*(const Matrix& rhs) const {
    if (this->cols != rhs.rows) {
      throw DimensionMismatch(rhs);
    }

    Matrix newMatrix(this->rows, rhs.cols);
    for (size_t i = 0; i < newMatrix.rows; ++i) {
      for (size_t j = 0; j < newMatrix.cols; ++j) {
        for (size_t k = 0; k < this->cols; ++k) {
          newMatrix.data[i][j] += this->data[i][k] * rhs.data[k][j];
        }
      }
    }

    return newMatrix;
  }

  Matrix Matrix::operator*(double val) const {
    Matrix newMatrix = *this;

    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        newMatrix.data[i][j] *= val;
      }
    }

    return newMatrix;
  }

  Matrix Matrix::transp() const {
    Matrix newMatrix(this->cols, this->rows);

    for (size_t i = 0; i < newMatrix.cols; ++i) {
      for (size_t j = 0; j < newMatrix.rows; ++j) {
        newMatrix.data[j][i] = this->data[i][j];
      }
    }

    return newMatrix;
  }

  Matrix operator*(double val, const Matrix& matrix) {
    Matrix newMatrix = matrix;

    for (size_t i = 0; i < matrix.rows; ++i) {
      for (size_t j = 0; j < matrix.cols; ++j) {
        newMatrix.data[i][j] *= val;
      }
    }

    return newMatrix;
  }

  std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.rows << " " << matrix.cols << std::endl;

    for (auto i = matrix.data.begin(); i < matrix.data.end(); ++i) {
      for (auto j = i->begin(); j < i->end(); ++j) {
        os << std::setprecision(std::numeric_limits<double>::max_digits10) << *j;
        os << " ";
      }
      os << std::endl;
    }

    return os;
  }

  double Matrix::det() const {
    if (this->cols != this->rows) {
      throw DimensionMismatch(*this);
    }

    int sign = 1;
    Matrix newMatrix = *this;
    for (size_t i = 0, iMax = i; i < this->rows; ++i) {
      iMax = i;
      for (size_t j = i + 1; j < this->rows; ++j) {
        if (std::fabs(newMatrix.data[j][i]) > std::fabs(newMatrix.data[iMax][i])) {
          iMax = j;
        }
      }

      if (std::fabs(newMatrix.data[iMax][i]) < this->eps) {
        return 0;
      }

      std::vector<double> temp = newMatrix.data[i];
      newMatrix.data[i] = newMatrix.data[iMax];
      newMatrix.data[iMax] = temp;

      sign = i != iMax ? -sign : sign;

      for (size_t j = i + 1; j < this->rows; ++j) {
        double mul_num = -newMatrix.data[j][i] / newMatrix.data[i][i];
        for (size_t k = i; k < this->rows; ++k) {
          newMatrix.data[j][k] += newMatrix.data[i][k] * mul_num;
        }
      }
    }

    double res = 1;
    for (size_t i = 0; i < this->rows; ++i) {
      res *= newMatrix.data[i][i];
    }
    res *= sign;

    return res;
  }

  Matrix Matrix::adj() const {
    if (this->cols != this->rows) {
      throw DimensionMismatch(*this);
    }

    Matrix newMatrix(this->rows, this->rows);
    for (size_t i = 0; i < this->rows; ++i) {
      for (size_t j = 0; j < this->rows; ++j) {
        newMatrix.data[j][i] = pow(-1, i + j) * this->getMinor(i, j);
      }
    }

    return newMatrix;
  }

  Matrix Matrix::inv() const {
    if (this->cols != this->rows) {
      throw DimensionMismatch(*this);
    }

    Matrix adjointedMatrix = this->adj();
    double det = this->det();
    if (std::fabs(det) < this->eps) {
      throw SingularMatrix();
    }

    return adjointedMatrix * (1 / det);
  }

  double Matrix::getMinor(size_t row, size_t col) const {
    if (this->cols != this->rows) {
      throw DimensionMismatch(*this);
    }

    Matrix newMatrix(this->rows - 1, this->rows - 1);
    for (size_t i = 0, itr = 0; i < this->rows - 1; ++i) {
      itr = i >= row ? (i + 1) : i;
      for (size_t j = 0, jtr = 0; j < this->rows - 1; ++j) {
        jtr = j >= col ? (j + 1) : j;
        newMatrix.data[i][j] = this->data[itr][jtr];
      }
    }

    return newMatrix.det();
  }

}  // namespace prep
