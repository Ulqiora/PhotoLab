#ifndef SLIDERS_H
#define SLIDERS_H

#include "Photo.h"

namespace s21 {

class Sliders {
 private:
  Photo* ph = nullptr;
  Matrix<uint32_t> beginData;
  Matrix<uint32_t> forBrighntness;
  Matrix<uint32_t> forContrast;

  struct pixelRGBStruct {
    float r = 0;
    float g = 0;
    float b = 0;
  } pixelRGB;
  void getHue(int hue, int* r, int* g, int* b);
  void HSLfilter(int r, int g, int b, float saturation);
  void HSVfilter(int r, int g, int b);

 public:
  explicit Sliders(Photo* photo) : ph(photo) {
    beginData = ph->getDate();
    forBrighntness = ph->getDate();
    forContrast = ph->getDate();
  }
  void applyBrightness(float factor);
  void applyContrast(float factor);
  void applyHSL(int hue, int saturation);
  void applyHSV(int hue, int saturation, int value);
  void checkOverflow();
  void cancelChanges();
  void applyChanges();
};
}  // namespace s21
#endif  // SLIDERS_H
