#pragma once

#include "../Model/Filter.h"
#include "../Model/Photo.h"
#include "../Model/SimpleFilter.h"
#include "../Model/sliders.h"

namespace s21 {

class Controller {
 private:
  s21::Photo *photo = nullptr;
  s21::Filter *filter = nullptr;
  s21::SimpleFilter *simpleFilter = nullptr;
  s21::Sliders *sliders = nullptr;

 public:
  explicit Controller(Photo *p) : photo(p) {}
  ~Controller() {}
  void openFile(std::string fileName);
  void setFilter();
  int getPhotoHeight() { return photo->getHeight(); }
  int getPhotoWidth() { return photo->getWidth(); }

  void kernelFiltration(FilterType ft);
  void kernelFiltration(Matrix<float> kernel);
  void simpleFiltration(SimpleFilterType sft);
  void toningSimpleFiltration(int r, int g, int b);
  void cancelFiltration();
  void brightness(float factor);
  void contrast(float factor);
  void HSV(int hue, int saturation, int value);
  void HSL(int hue, int saturation);
  void applyChanges();

  unsigned char getRed(int i, int j) {
    return photo->getPixelRedComponent(i, j, photo->getDate());
  }
  unsigned char getGreen(int i, int j) {
    return photo->getPixelGreenComponent(i, j, photo->getDate());
  }
  unsigned char getBlue(int i, int j) {
    return photo->getPixelBlueComponent(i, j, photo->getDate());
  }
};
}  // namespace s21
