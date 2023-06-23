#include "SimpleFilter.h"

namespace s21 {

void SimpleFilter::applySimpleFilter(SimpleFilterType sft) {
  switch (sft) {
    case DISCOLOR:
      discoloration();
      break;
    case NEGATIVE:
      negative();
      break;
  }
}

void SimpleFilter::discoloration() {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      float newChanelsValue = 0;
      newChanelsValue +=
          (float)ph->getPixelRedComponent(i, j, ph->getDate()) * 0.299;
      newChanelsValue +=
          (float)ph->getPixelGreenComponent(i, j, ph->getDate()) * 0.587;
      newChanelsValue +=
          (float)ph->getPixelBlueComponent(i, j, ph->getDate()) * 0.114;
      result(i, j) = ph->getPixelFromComponents(
          (int)newChanelsValue, (int)newChanelsValue, (int)newChanelsValue);
    }
  }
  ph->setDate(result);
}

void SimpleFilter::negative() {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      pixelRGB = {0, 0, 0};
      pixelRGB.r = 255 - ph->getPixelRedComponent(i, j, ph->getDate());
      pixelRGB.g = 255 - ph->getPixelGreenComponent(i, j, ph->getDate());
      pixelRGB.b = 255 - ph->getPixelBlueComponent(i, j, ph->getDate());
      result(i, j) = ph->getPixelFromComponents(
          (int)pixelRGB.r, (int)pixelRGB.g, (int)pixelRGB.b);
    }
  }
  ph->setDate(result);
}

void SimpleFilter::toning(int rT, int gT, int bT) {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      pixelRGB = {0, 0, 0};
      pixelRGB.r =
          (rT * 0.7 + (float)ph->getPixelRedComponent(i, j, ph->getDate())) *
          0.7;
      pixelRGB.g =
          (gT * 0.7 + (float)ph->getPixelGreenComponent(i, j, ph->getDate())) *
          0.7;
      pixelRGB.b =
          (bT * 0.7 + (float)ph->getPixelBlueComponent(i, j, ph->getDate())) *
          0.7;
      checkOverflow();
      result(i, j) = ph->getPixelFromComponents(
          (int)pixelRGB.r, (int)pixelRGB.g, (int)pixelRGB.b);
    }
  }
  ph->setDate(result);
}

void SimpleFilter::checkOverflow() {
  if (pixelRGB.r > 255) pixelRGB.r = 255;
  if (pixelRGB.g > 255) pixelRGB.g = 255;
  if (pixelRGB.b > 255) pixelRGB.b = 255;
  if (pixelRGB.r < 0) pixelRGB.r = 0;
  if (pixelRGB.g < 0) pixelRGB.g = 0;
  if (pixelRGB.b < 0) pixelRGB.b = 0;
}

void SimpleFilter::cancelChanges() { ph->setDate(beginData); }

}  // namespace s21
