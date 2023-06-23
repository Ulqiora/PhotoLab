#include <iostream>
#include <thread>

#include "Model/Filter.h"
#include "Model/Photo.h"

#define LAST (a.getHeight() - 1), (a.getWidth() - 1)

void printInfoAboutPixel(s21::Photo& a) {
  std::cout << "Pixel 0,0: " << std::hex << a.getPixel(LAST) << '\n';
  std::cout << "Alpha: " << (int)a.getPixelAlphaComponent(LAST, a.getDate())
            << '\n';
  std::cout << "Red: " << (int)a.getPixelRedComponent(LAST, a.getDate())
            << '\n';
  std::cout << "Green: " << (int)a.getPixelGreenComponent(LAST, a.getDate())
            << '\n';
  std::cout << "Blue: " << (int)a.getPixelBlueComponent(LAST, a.getDate())
            << '\n';
}
int main() {
  try {
    s21::Photo a("../misc/images/sample_640×426 (1).bmp");
    // std::cout<<std::dec<<a.GetImageInfo().width<<'\n';
    // std::cout<<a.GetImageInfo().height<<'\n';
    // std::cout<<a.GetImageInfo().bit_count<<'\n';
    s21::Filter f(&a);
    printInfoAboutPixel(a);

    // f.setData(5);

    float kernelData[] = {-2.0, -1.0, 0.0, -1.0, 1.0, 1.0, 0.0, 1.0, 2.0};
    s21::Matrix<float> kernel;
    kernel.fill_matrix(kernelData);
    f.applyFilter(kernel);
    // f.printExpandData(f.expandedData);
    std::cout << std::endl;
    // f.printExpandData(a.getDate());
    printInfoAboutPixel(a);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
