#include "Filter.h"

namespace s21 {

void Filter::applyFilter(Matrix<float> kernel) {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  expandedData = expandMatrixData(kernel.getCols());
  createShiftMatrix(kernel.getCols());
  int shift = kernel.getCols() / 2;
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      pixelRGB = {0, 0, 0};
      for (int t = 0; t < kernel.getRows(); t++) {
        for (int k = 0; k < kernel.getCols(); k++) {
          int newX = i + shift + xShift(t, k);
          int newY = j + shift + yShift(t, k);
          multPixel(pixelRGB, kernel(t, k), newX, newY);
        }
      }
      // add check for overflowing of pixel components;
      checkOverflow();
      result(i, j) = ph->getPixelFromComponents(
          (int)pixelRGB.r, (int)pixelRGB.g, (int)pixelRGB.b);
    }
  }
  ph->setDate(result);
}

void Filter::applyFilter(FilterType ft) {
  switch (ft) {
    case EMBOSS:
      kernelToFilter({-2.0, -1.0, 0.0, -1.0, 1.0, 1.0, 0.0, 1.0, 2.0}, 1);
      break;
    case SHARPEN:
      kernelToFilter({0.0, -0.25, 0.0, -0.25, 2.0, -0.25, 0.0, -0.25, 0.0}, 1);
      break;
    case BOX_BLUR:
      kernelToFilter({1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
                     (float)1 / 9);
      break;
    case GAUSSIAN:
      kernelToFilter({1.0, 2.0, 1.0, 2.0, 4.0, 2.0, 1.0, 2.0, 1.0},
                     (float)1 / 16);
      break;
    case LAPLACIAN:
      kernelToFilter({-1.0, -1.0, -1.0, -1.0, 8.0, -1.0, -1.0, -1.0, -1.0}, 1);
      break;
    case SOBEL_LEFT:
      kernelToFilter({1.0, 0.0, -1.0, 2.0, 0.0, -2.0, 1.0, 0.0, -1.0}, 1);
      break;
    case SOBEL_RIGHT:
      kernelToFilter({-1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0}, 1);
      break;
  }
}

void Filter::kernelToFilter(std::vector<float> k, float mult) {
  Matrix<float> kernel;
  kernel.fill_matrix(k);
  if (mult != 1) {
    kernel.mul_number(mult);
  }
  applyFilter(kernel);
}

void Filter::multPixel(struct pixelRGB& pix, float num, int i, int j) {
  pix.r += (float)ph->getPixelRedComponent(i, j, expandedData) * num;
  pix.g += (float)ph->getPixelGreenComponent(i, j, expandedData) * num;
  pix.b += (float)ph->getPixelBlueComponent(i, j, expandedData) * num;
}

void Filter::checkOverflow() {
  if (pixelRGB.r > 255) pixelRGB.r = 255;
  if (pixelRGB.g > 255) pixelRGB.g = 255;
  if (pixelRGB.b > 255) pixelRGB.b = 255;
  if (pixelRGB.r < 0) pixelRGB.r = 0;
  if (pixelRGB.g < 0) pixelRGB.g = 0;
  if (pixelRGB.b < 0) pixelRGB.b = 0;
}

void Filter::createShiftMatrix(int kernelSize) {
  Matrix<int> x(kernelSize, kernelSize);
  Matrix<int> y(kernelSize, kernelSize);
  int shift = -kernelSize / 2;
  for (int i = 0; i < kernelSize; i++) {
    for (int j = 0; j < kernelSize; j++) {
      y(i, j) = shift;
      x(j, i) = shift;
    }
    shift++;
  }
  xShift = x;
  yShift = y;
}

Matrix<uint32_t> Filter::expandMatrixData(int exSize) {
  int expandSide = exSize / 2;
  Matrix<uint32_t> result(ph->getDate().getRows() + 2 * expandSide,
                          ph->getDate().getCols() + 2 * expandSide);
  for (int i = 0; i < result.getRows(); i++) {
    if (i < expandSide) {
      for (int j = expandSide; j < (result.getCols() - expandSide); j++) {
        result(i, j) = ph->getDate()(expandSide - i - 1, j - expandSide);
      }
    } else if (i - (ph->getDate().getRows() - 1) - expandSide > 0) {
      for (int j = expandSide; j < result.getCols() - expandSide; j++) {
        result(i, j) =
            ph->getDate()(ph->getDate().getRows() -
                              (i - (ph->getDate().getRows() - 1) - expandSide),
                          j - expandSide);
      }
    } else {
      for (int j = expandSide; j < result.getCols() - expandSide; j++) {
        result(i, j) = ph->getDate()(i - expandSide, j - expandSide);
      }
    }
  }
  for (int j = 0; j < expandSide; j++) {
    for (int i = expandSide; i < result.getRows() - expandSide; i++) {
      result(i, j) = ph->getDate()(i - expandSide, expandSide - j - 1);
      result(i, result.getCols() - 1 - j) = ph->getDate()(
          i - expandSide, ph->getDate().getCols() - (expandSide - j));
    }
  }
  fillCorners(result, expandSide);
  return result;
}

void Filter::fillCorners(Matrix<uint32_t>& matrix, int expandSize) {
  // left top corner
  for (int i = expandSize - 1; i >= 0; i--)
    for (int j = expandSize - 1; j >= 0; j--)
      matrix(i, j) = (matrix(i + 1, j) + matrix(i, j + 1)) / 2;
  // right top corner
  for (int i = expandSize - 1; i >= 0; i--)
    for (int j = matrix.getCols() - expandSize; j < matrix.getCols(); j++)
      matrix(i, j) = (matrix(i + 1, j) + matrix(i, j - 1)) / 2;
  // left bottom corner
  for (int i = matrix.getRows() - expandSize; i < matrix.getRows(); i++)
    for (int j = expandSize - 1; j >= 0; j--)
      matrix(i, j) = (matrix(i - 1, j) + matrix(i, j + 1)) / 2;
  // right bottom corner
  for (int i = matrix.getRows() - expandSize; i < matrix.getRows(); i++)
    for (int j = matrix.getCols() - expandSize; j < matrix.getCols(); j++)
      matrix(i, j) = (matrix(i - 1, j) + matrix(i, j - 1)) / 2;
}

void Filter::cancelChanges() { ph->setDate(beginData); }

}  // namespace s21
