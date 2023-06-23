namespace s21 {

template <typename T>
void Matrix<T>::setSize(int newRows, int newCols) {
  if (newRows < 1 || newCols < 1) throw std::invalid_argument("size < 1");
  if (newRows == _rows && newCols == _cols) return;
  Matrix<T> copy(*this);
  destroyMatrix();
  _rows = newRows;
  _cols = newCols;
  newMatrix(_rows, _cols);
}

// Constructors
template <typename T>
Matrix<T>::Matrix() {
  newMatrix(3, 3);
}

template <typename T>
Matrix<T>::Matrix(int rows, int cols) {
  if (rows > 0 && cols > 0) {
    newMatrix(rows, cols);
  } else {
    throw std::out_of_range("Incorrect input, matrix should have both sizes > 0");
  }
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) : Matrix(other._rows, other._cols) {
  copyMatrix(other._matrix);
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& other) {
  _rows = other._rows;
  _cols = other._cols;
  _matrix = other._matrix;
  other._matrix = nullptr;
}

// Destructor
template <typename T>
Matrix<T>::~Matrix() {
  if (_matrix) {
    destroyMatrix();
  }
}

// Methods
template <typename T>
bool Matrix<T>::eq_matrix(const Matrix<T>& other) const {
  bool result = true;
  if (isEqualSizes(other)) {
    for (int i = 0; i < _rows && result; i++) {
      for (int j = 0; j < _cols && result; j++) {
        if (fabs(_matrix[i][j] - other._matrix[i][j]) > 1e-7) result = false;
      }
    }
  } else {
    result = false;
  }
  return result;
}

template <typename T>
void Matrix<T>::sum_matrix(const Matrix<T>& other) {
  if (isEqualSizes(other)) {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        _matrix[i][j] = _matrix[i][j] + other._matrix[i][j];
      }
    }
  } else {
    throw std::invalid_argument("Invalid argument, matrices must be equal sizes");
  }
}

template <typename T>
void Matrix<T>::sub_matrix(const Matrix& other) {
  if (isEqualSizes(other)) {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        _matrix[i][j] = _matrix[i][j] - other._matrix[i][j];
      }
    }
  } else {
    throw std::invalid_argument("Invalid argument, matrices must be equal sizes");
  }
}

template <typename T>
void Matrix<T>::mul_number(const double num) {
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      _matrix[i][j] *= num;
    }
  }
}

template <typename T>
void Matrix<T>::mul_matrix(const Matrix<T>& other) {
  if (_cols == other._rows) {
    Matrix result(_rows, other._cols);
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < other._cols; j++) {
        for (int c = 0; c < _cols; c++) result._matrix[i][j] += _matrix[i][c] * other._matrix[c][j];
      }
    }
    *this = result;
  } else {
    throw std::invalid_argument(
        "Invalid argument, number of cols of the first matrix must be equal to number of rows of the "
        "second matrix");
  }
}

template <typename T>
int Matrix<T>::getRows() const {
  return _rows;
}

template <typename T>
int Matrix<T>::getCols() const {
  return _cols;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
  Matrix<T> result = *this;
  result.sum_matrix(other);
  return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
  Matrix<T> result = *this;
  result.sub_matrix(other);
  return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
  Matrix<T> result = *this;
  result.mul_matrix(other);
  return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const double num) const {
  Matrix<T> result = *this;
  result.mul_number(num);
  return result;
}

template <typename T>
Matrix<T> operator*(double num, const Matrix<T>& other) {
  Matrix<T> result = other;
  result.mul_number(num);
  return result;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const {
  return eq_matrix(other);
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
  if (this != &other) {
    destroyMatrix();
    newMatrix(other._rows, other._cols);
    copyMatrix(other._matrix);
  }
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+=(const Matrix<T>& other) {
  sum_matrix(other);
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-=(const Matrix<T>& other) {
  sub_matrix(other);
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*=(const Matrix<T>& other) {
  mul_matrix(other);
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*=(const double num) {
  mul_number(num);
  return *this;
}
template <typename T>
const T& Matrix<T>::operator()(int i, int j) const {
  if (i >= 0 && i < _rows && j >= 0 && j < _cols) {
    return _matrix[i][j];
  } else {
    throw std::out_of_range("Incorrect input, index out of matrix sizes");
  }
}

template <typename T>
T& Matrix<T>::operator()(int i, int j) {
  if (i >= 0 && i < _rows && j >= 0 && j < _cols) {
    return _matrix[i][j];
  } else {
    throw std::out_of_range("Incorrect input, index out of matrix sizes");
  }
}

template <typename T>
inline bool Matrix<T>::isEqualSizes(const Matrix<T>& other) const {
  return (_rows == other._rows && _cols == other._cols);
}

template <typename T>
inline bool Matrix<T>::isSquareMatrix() const {
  return (_rows == _cols);
}

template <typename T>
void Matrix<T>::newMatrix(int rows, int cols) {
  _rows = rows;
  _cols = cols;
  _matrix = new T*[_rows];
  for (int i = 0; i < _rows; i++) {
    _matrix[i] = new T[_cols]();
  }
}

template <typename T>
void Matrix<T>::destroyMatrix() {
  for (int i = 0; i < _rows; i++) {
    delete[] _matrix[i];
  }
  delete[] _matrix;
}

template <typename T>
void Matrix<T>::copyMatrix(T** other_matrix) {
  // std::cout<<
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      // std::cout<<i<<' '<<j<<'\n';
      _matrix[i][j] = other_matrix[i][j];
    }
  }
}

template <typename T>
void Matrix<T>::fill_matrix(std::vector<T> arr) {
  int t = 0;
  for (int i = 0; i < _rows; i++)
    for (int j = 0; j < _cols; j++) _matrix[i][j] = arr[t++];
}

}  // namespace s21
