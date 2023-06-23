#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "Photo.h"

namespace s21 {

enum SimpleFilterType { DISCOLOR, NEGATIVE };

class SimpleFilter {
 private:
  Photo *ph = nullptr;

  Matrix<uint32_t> beginData;

  struct pixelRGBStruct {
    float r = 0;
    float g = 0;
    float b = 0;
  } pixelRGB;

  void checkOverflow();
  void discoloration();
  void negative();

 public:
  explicit SimpleFilter(Photo *photo) : ph(photo) { beginData = ph->getDate(); }
  void applySimpleFilter(SimpleFilterType sft);
  void toning(int rT, int gT, int bT);
  void cancelChanges();
};

}  // namespace s21
#endif  // SIMPLEFILTER_H
