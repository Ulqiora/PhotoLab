#pragma once

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace s21 {

template <class T>
class Matrix {
 private:
  int _rows, _cols;
  T** _matrix;

 public:
  using value_type = T;

  Matrix();
  Matrix(int rows, int cols);
  Matrix(const Matrix<T>& other);
  Matrix(Matrix<T>&& other);
  ~Matrix();
  bool eq_matrix(const Matrix<T>& other) const;
  void sum_matrix(const Matrix<T>& other);
  void sub_matrix(const Matrix<T>& other);
  void mul_number(const double num);
  void mul_matrix(const Matrix<T>& other);

  int getRows() const;
  int getCols() const;
  Matrix<T> operator+(const Matrix& other) const;
  Matrix<T> operator-(const Matrix& other) const;
  Matrix<T> operator*(const Matrix& other) const;
  Matrix<T> operator*(const double num) const;
  bool operator==(const Matrix<T>& other) const;
  Matrix<T>& operator=(const Matrix<T>& other);
  Matrix<T> operator+=(const Matrix<T>& other);
  Matrix<T> operator-=(const Matrix<T>& other);
  Matrix<T> operator*=(const Matrix<T>& other);
  Matrix<T> operator*=(const double num);
  const T& operator()(int i, int j) const;
  T& operator()(int i, int j);
  void setRandom(int rows, int cols);
  void loadMatrix(std::ifstream& file);
  void setSize(int newRows, int newCols);
  void fill_matrix(std::vector<T> arr);

 private:
  inline bool isEqualSizes(const Matrix<T>& other) const;
  inline bool isSquareMatrix() const;
  void newMatrix(int rows, int cols);
  void destroyMatrix();
  void copyMatrix(T** other_matrix);
};
}  // namespace s21
#include "Matrix.inl"
