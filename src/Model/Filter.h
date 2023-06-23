#include "Photo.h"
#include "math.h"

namespace s21 {

enum FilterType {
  EMBOSS,
  SHARPEN,
  BOX_BLUR,
  GAUSSIAN,
  LAPLACIAN,
  SOBEL_LEFT,
  SOBEL_RIGHT
};

class Filter {
 private:
  Photo *ph = nullptr;

  Matrix<uint32_t> expandedData;
  Matrix<uint32_t> beginData;
  Matrix<int> xShift;
  Matrix<int> yShift;

  struct pixelRGB {
    float r = 0;
    float g = 0;
    float b = 0;
  } pixelRGB;

  void checkOverflow();
  Matrix<uint32_t> expandMatrixData(int exSize);
  void fillCorners(Matrix<uint32_t> &matrix, int expandSize);
  void multPixel(struct pixelRGB &pix, float num, int i, int j);
  void createShiftMatrix(int kernelSize);
  void kernelToFilter(std::vector<float> k, float mult);

 public:
  explicit Filter(Photo *photo) : ph(photo) { beginData = ph->getDate(); }
  void applyFilter(FilterType ft);
  void applyFilter(Matrix<float> kernel);
  void cancelChanges();
};

}  // namespace s21
