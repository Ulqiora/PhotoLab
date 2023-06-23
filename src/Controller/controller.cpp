#include "controller.h"

namespace s21 {

void Controller::openFile(std::string fileName) {
  filter = nullptr;
  simpleFilter = nullptr;
  sliders = nullptr;
  photo = new Photo(fileName);
}

void Controller::setFilter() { filter = new Filter(photo); }

void Controller::kernelFiltration(FilterType ft) {
  if (filter == nullptr) {
    filter = new Filter(photo);
  }
  filter->applyFilter(ft);
}

void Controller::kernelFiltration(Matrix<float> kernel) {
  if (filter == nullptr) {
    filter = new Filter(photo);
  }
  filter->applyFilter(kernel);
}

void Controller::simpleFiltration(SimpleFilterType sft) {
  if (simpleFilter == nullptr) {
    simpleFilter = new SimpleFilter(photo);
  }
  simpleFilter->applySimpleFilter(sft);
}

void Controller::toningSimpleFiltration(int r, int g, int b) {
  if (simpleFilter == nullptr) {
    simpleFilter = new SimpleFilter(photo);
  }
  simpleFilter->toning(r, g, b);
}

void Controller::cancelFiltration() {
  photo->cancelFiltration();
  if (sliders != nullptr) {
    sliders->cancelChanges();
    sliders = nullptr;
  }
  if (simpleFilter != nullptr) {
    simpleFilter->cancelChanges();
    simpleFilter = nullptr;
  }
  if (filter != nullptr) {
    filter->cancelChanges();
    filter = nullptr;
  }
}

void Controller::brightness(float factor) {
  if (sliders == nullptr) {
    sliders = new Sliders(photo);
  }
  sliders->applyBrightness(factor);
}

void Controller::contrast(float factor) {
  if (sliders == nullptr) {
    sliders = new Sliders(photo);
  }
  sliders->applyContrast(factor);
}

void Controller::HSV(int hue, int saturation, int value) {
  if (sliders == nullptr) {
    sliders = new Sliders(photo);
  }
  sliders->applyHSV(hue, saturation, value);
}

void Controller::HSL(int hue, int saturation) {
  if (sliders == nullptr) {
    sliders = new Sliders(photo);
  }
  sliders->applyHSL(hue, saturation);
}

void Controller::applyChanges() {
  if (sliders != nullptr) sliders->applyChanges();
}

}  // namespace s21
