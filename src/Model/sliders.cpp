#include "sliders.h"

namespace s21 {

void Sliders::applyBrightness(float factor) {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      pixelRGB = {0, 0, 0};
      pixelRGB.r =
          (float)ph->getPixelRedComponent(i, j, forBrighntness) * factor;
      pixelRGB.g =
          (float)ph->getPixelGreenComponent(i, j, forBrighntness) * factor;
      pixelRGB.b =
          (float)ph->getPixelBlueComponent(i, j, forBrighntness) * factor;
      checkOverflow();
      result(i, j) = ph->getPixelFromComponents(
          (int)pixelRGB.r, (int)pixelRGB.g, (int)pixelRGB.b);
    }
  }
  ph->setDate(result);
}

void Sliders::applyContrast(float factor) {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      pixelRGB = {0, 0, 0};
      pixelRGB.r = (float)ph->getPixelRedComponent(i, j, forContrast) / 255.0;
      pixelRGB.r -= 0.5;
      pixelRGB.r *= factor;
      pixelRGB.r += 0.5;
      pixelRGB.r *= 255;

      pixelRGB.g = (float)ph->getPixelGreenComponent(i, j, forContrast) / 255.0;
      pixelRGB.g -= 0.5;
      pixelRGB.g *= factor;
      pixelRGB.g += 0.5;
      pixelRGB.g *= 255;

      pixelRGB.b = (float)ph->getPixelBlueComponent(i, j, forContrast) / 255.0;
      pixelRGB.b -= 0.5;
      pixelRGB.b *= factor;
      pixelRGB.b += 0.5;
      pixelRGB.b *= 255;

      checkOverflow();
      result(i, j) = ph->getPixelFromComponents(
          (int)pixelRGB.r, (int)pixelRGB.g, (int)pixelRGB.b);
    }
  }
  ph->setDate(result);
}

void Sliders::getHue(int hue, int* r, int* g, int* b) {
  float hueToColor = hue % 60;
  if (hue >= 0 && hue < 60) {
    *r = hueToColor / 60 * 255;
  } else if (hue >= 60 && hue < 120) {
    *r = 255;
    *g = hueToColor / 60 * 255;
  } else if (hue >= 120 && hue < 180) {
    *g = 255;
    *r = 255 - hueToColor / 60 * 255;
  } else if (hue >= 180 && hue < 240) {
    *g = 255;
    *b = hueToColor / 60 * 255;
  } else if (hue >= 240 && hue < 300) {
    *b = 255;
    *g = 255 - hueToColor / 60 * 255;
  } else if (hue >= 300 && hue < 360) {
    *b = 255;
    *r = hueToColor / 60 * 255;
  }
}

void Sliders::applyHSL(int hue, int saturation) {
  int r = 0, g = 0, b = 0;
  getHue(hue, &r, &g, &b);
  HSLfilter(r, g, b, (1 - saturation / 100.0));
}

void Sliders::HSLfilter(int r, int g, int b, float saturation) {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  ph->setDate(beginData);
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      pixelRGB = {0, 0, 0};
      pixelRGB.r =
          (r * 0.7 + (float)ph->getPixelRedComponent(i, j, ph->getDate()));
      pixelRGB.g =
          (g * 0.7 + (float)ph->getPixelGreenComponent(i, j, ph->getDate()));
      pixelRGB.b =
          (b * 0.7 + (float)ph->getPixelBlueComponent(i, j, ph->getDate()));

      float newChanelsValue = 0;
      newChanelsValue += pixelRGB.r * 0.299;
      newChanelsValue += pixelRGB.g * 0.587;
      newChanelsValue += pixelRGB.b * 0.114;

      pixelRGB.r = pixelRGB.r * (1 - saturation) + newChanelsValue * saturation;
      pixelRGB.g = pixelRGB.g * (1 - saturation) + newChanelsValue * saturation;
      pixelRGB.b = pixelRGB.b * (1 - saturation) + newChanelsValue * saturation;

      checkOverflow();
      result(i, j) = ph->getPixelFromComponents(
          (int)pixelRGB.r, (int)pixelRGB.g, (int)pixelRGB.b);
    }
  }
  ph->setDate(result);
  forBrighntness = result;
}

void Sliders::applyHSV(int hue, int saturation, int value) {
  int r = 0, g = 0, b = 0;
  getHue(hue, &r, &g, &b);

  int deltaRed = 255 - r;
  int deltaGreen = 255 - g;
  int deltaBlue = 255 - b;

  r += (int)(deltaRed * (100 - saturation) / 100.0);
  g += (int)(deltaGreen * (100 - saturation) / 100.0);
  b += (int)(deltaBlue * (100 - saturation) / 100.0);

  r -= (int)(r * (100 - value) / 100.0);
  g -= (int)(g * (100 - value) / 100.0);
  b -= (int)(b * (100 - value) / 100.0);

  HSVfilter(r, g, b);
}

void Sliders::HSVfilter(int r, int g, int b) {
  Matrix<uint32_t> result(ph->getDate().getRows(), ph->getDate().getCols());
  ph->setDate(beginData);
  for (int i = 0; i < ph->getDate().getRows(); i++) {
    for (int j = 0; j < ph->getDate().getCols(); j++) {
      pixelRGB = {0, 0, 0};
      pixelRGB.r =
          (r * 0.7 + (float)ph->getPixelRedComponent(i, j, ph->getDate()));
      pixelRGB.g =
          (g * 0.7 + (float)ph->getPixelGreenComponent(i, j, ph->getDate()));
      pixelRGB.b =
          (b * 0.7 + (float)ph->getPixelBlueComponent(i, j, ph->getDate()));
      checkOverflow();
      result(i, j) = ph->getPixelFromComponents(
          (int)pixelRGB.r, (int)pixelRGB.g, (int)pixelRGB.b);
    }
  }
  ph->setDate(result);
}

void Sliders::checkOverflow() {
  if (pixelRGB.r > 255) pixelRGB.r = 255;
  if (pixelRGB.g > 255) pixelRGB.g = 255;
  if (pixelRGB.b > 255) pixelRGB.b = 255;
  if (pixelRGB.r < 0) pixelRGB.r = 0;
  if (pixelRGB.g < 0) pixelRGB.g = 0;
  if (pixelRGB.b < 0) pixelRGB.b = 0;
}

void Sliders::cancelChanges() {
  beginData = ph->getDate();
  forContrast = ph->getDate();
  forBrighntness = ph->getDate();
}

void Sliders::applyChanges() {
  forContrast = ph->getDate();
  forBrighntness = ph->getDate();
}

}  // namespace s21
