#pragma once
#include <stdio.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Matrix.h"
namespace s21 {

class Photo {
 public:
  enum Component { ALPHA = 0, RED, GREEN, BLUE };
#pragma pack(push, 1)
  struct BmpFileInfo {
    uint16_t file_type{0x4D42};  // File type always BM which is 0x4D42
    uint32_t file_size{0};       // Size of the file (in bytes)
    uint16_t reserved1{0};       // Reserved, always 0
    uint16_t reserved2{0};       // Reserved, always 0
    uint32_t offset_data{0};
  };
#pragma pack(pop)
  struct BmpImageInfo {
    uint32_t size{0};  // Size of this header (in bytes)
    int32_t width{0};  // width of bitmap in pixels
    int32_t height{
        0};  // height of bitmap in pixels
             //       (if positive, bottom-up, with origin in lower left corner)
             //       (if negative, top-down, with origin in upper left corner)
    uint16_t planes{
        1};  // No. of planes for the target device, this is always 1
    uint16_t bit_count{0};    // No. of bits per pixel
    uint32_t compression{0};  // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS
                              // ONLY UNCOMPRESSED BMP images
    uint32_t size_image{0};   // 0 - for uncompressed images
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};  // No. color indexes in the color table. Use 0 for
                              // the max number of colors allowed by bit_count
    uint32_t colors_important{0};
  };
  struct BmpColorHeader {
    uint32_t red_mask{0x00ff0000};          // Bit mask for the red channel
    uint32_t green_mask{0x0000ff00};        // Bit mask for the green channel
    uint32_t blue_mask{0x000000ff};         // Bit mask for the blue channel
    uint32_t alpha_mask{0xff000000};        // Bit mask for the alpha channel
    uint32_t color_space_type{0x73524742};  // Default "sRGB" (0x73524742)
    uint32_t unused[16]{0};                 // Unused data for sRGB color space
  };

 private:
  void ReadBmpFileInfo(std::ifstream& file);
  void ReadBmpImageInfo(std::ifstream& file);
  void ReadBmpColorHeader(std::ifstream& file);
  bool CheckBmpFileInfo();
  bool ImageIsTransparent();
  void readHeaders(std::ifstream& file);
  void ReInitializationOffsetAndSize();
  uint32_t MakeStrideAligned(uint32_t align_stride);
  void ReadSrgbImages(std::ifstream& file);
  void ReadRgbImages(std::ifstream& file);
  void initOffsets();

 public:
  Photo() {}
  explicit Photo(std::string filename);
  const BmpFileInfo& GetFileInfo() const { return fileInfo; }
  const BmpImageInfo& GetImageInfo() const { return imageInfo; }
  const BmpColorHeader& GetColorInfo() const { return colorInfo; }
  const uint32_t& getPixel(int i, int j) const;
  unsigned char getPixelAlphaComponent(int i, int j,
                                       const Matrix<uint32_t>& data);
  unsigned char getPixelRedComponent(int i, int j,
                                     const Matrix<uint32_t>& data);
  unsigned char getPixelGreenComponent(int i, int j,
                                       const Matrix<uint32_t>& data);
  unsigned char getPixelBlueComponent(int i, int j,
                                      const Matrix<uint32_t>& data);
  uint32_t getPixelFromComponents(int r, int g, int b);
  const int32_t& getWidth() const { return imageInfo.width; }
  const int32_t& getHeight() const { return imageInfo.height; }
  Matrix<uint32_t>& getDate() { return data; }
  void setDate(Matrix<uint32_t> m) { data = m; }
  void cancelFiltration() { data = beginData; }

 private:
  BmpFileInfo fileInfo;
  BmpImageInfo imageInfo;
  BmpColorHeader colorInfo;
  uint8_t offsets[4]{0};
  uint32_t row_stride{0};
  Matrix<uint32_t> data;
  Matrix<uint32_t> beginData;

 public:
};

}  // namespace s21
