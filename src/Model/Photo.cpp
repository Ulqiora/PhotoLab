#include "Photo.h"

namespace s21 {
void Photo::ReadBmpFileInfo(std::ifstream& file) {
  file.read((char*)&fileInfo, sizeof(fileInfo));
}

void Photo::ReadBmpImageInfo(std::ifstream& file) {
  file.read((char*)&imageInfo, sizeof(imageInfo));
}

void Photo::ReadBmpColorHeader(std::ifstream& file) {
  if (imageInfo.size >= (sizeof(BmpFileInfo) + sizeof(BmpColorHeader))) {
    file.read((char*)&colorInfo, sizeof(colorInfo));
  } else {
    std::cout << "Image info, size: " << imageInfo.size << '\n';
    std::cout << "BmpFileInfo + BmpColorHeader bytes: "
              << (sizeof(BmpFileInfo) + sizeof(BmpColorHeader)) << '\n';
    throw std::invalid_argument("Error! Unrecognized file format.");
  }
}

bool Photo::CheckBmpFileInfo() { return fileInfo.file_type == 0x4D42; }

bool Photo::ImageIsTransparent() { return imageInfo.bit_count == 32; }

void Photo::readHeaders(std::ifstream& file) {
  if (!file.is_open()) throw std::runtime_error("This file can't be opened");
  ReadBmpFileInfo(file);
  if (!CheckBmpFileInfo())
    throw std::invalid_argument("This file have a not correct type!");
  ReadBmpImageInfo(file);
  if (ImageIsTransparent()) ReadBmpColorHeader(file);
}

void Photo::ReInitializationOffsetAndSize() {
  if (imageInfo.bit_count == 32) {
    imageInfo.size = sizeof(BmpImageInfo) + sizeof(BmpColorHeader);
    fileInfo.offset_data =
        sizeof(BmpFileInfo) + sizeof(BmpImageInfo) + sizeof(BmpColorHeader);
  } else {
    imageInfo.size = sizeof(BmpImageInfo);
    fileInfo.offset_data = sizeof(BmpFileInfo) + sizeof(BmpImageInfo);
  }
  fileInfo.file_size = fileInfo.offset_data;
}

Photo::Photo(std::string filename) {
  std::ifstream file(filename, std::ios_base::binary);
  readHeaders(file);
  file.seekg(fileInfo.offset_data, file.beg);
  ReInitializationOffsetAndSize();
  data.setSize(imageInfo.height, imageInfo.width);
  if (imageInfo.height < 0)
    throw std::runtime_error(
        "The program can treat only BMP images with the origin in the bottom "
        "left corner!");
  if (ImageIsTransparent())
    ReadSrgbImages(file);
  else
    ReadRgbImages(file);
  file.close();
  initOffsets();
  beginData = data;
}

void Photo::ReadRgbImages(std::ifstream& file) {
  uint32_t new_stride = MakeStrideAligned(4);
  std::vector<uint8_t> padding_row(new_stride - row_stride);
  for (int i = 0; i < data.getRows(); i++) {
    for (int j = 0; j < data.getCols(); j++) {
      if (imageInfo.width % 4 == 0) {
        file.read((char*)(&data(i, j)), 3);
      }
    }
    if (imageInfo.width % 4 == 0)
      file.read((char*)padding_row.data(), padding_row.size());
  }
}

void Photo::ReadSrgbImages(std::ifstream& file) {
  for (int i = 0; i < data.getRows(); i++) {
    for (int j = 0; j < data.getCols(); j++) {
      if (imageInfo.width % 4 == 0) {
        file.read((char*)(&data(i, j)), sizeof(uint32_t));
        // data(i, j)<<=8;
      }
    }
  }
}

uint32_t Photo::MakeStrideAligned(uint32_t align_stride) {
  uint32_t new_stride = row_stride;
  while (new_stride % align_stride != 0) new_stride++;
  return new_stride;
}

void Photo::initOffsets() {
  offsets[ALPHA] = ((colorInfo.alpha_mask & 0x0001) == 1)           ? 0
                   : (((colorInfo.alpha_mask >> 8) & 0x0001) == 1)  ? 8
                   : (((colorInfo.alpha_mask >> 16) & 0x0001) == 1) ? 16
                                                                    : 24;
  offsets[RED] = ((colorInfo.red_mask & 0x0001) == 1)           ? 0
                 : (((colorInfo.red_mask >> 8) & 0x0001) == 1)  ? 8
                 : (((colorInfo.red_mask >> 16) & 0x0001) == 1) ? 16
                                                                : 24;
  offsets[GREEN] = ((colorInfo.green_mask & 0x0001) == 1)           ? 0
                   : (((colorInfo.green_mask >> 8) & 0x0001) == 1)  ? 8
                   : (((colorInfo.green_mask >> 16) & 0x0001) == 1) ? 16
                                                                    : 24;
  offsets[BLUE] = ((colorInfo.blue_mask & 0x0001) == 1)           ? 0
                  : (((colorInfo.blue_mask >> 8) & 0x0001) == 1)  ? 8
                  : (((colorInfo.blue_mask >> 16) & 0x0001) == 1) ? 16
                                                                  : 24;
}

unsigned char Photo::getPixelAlphaComponent(int i, int j,
                                            const Matrix<uint32_t>& data) {
  return (data(i, j) & colorInfo.alpha_mask) >> offsets[ALPHA];
}
unsigned char Photo::getPixelRedComponent(int i, int j,
                                          const Matrix<uint32_t>& data) {
  return (data(i, j) & colorInfo.red_mask) >> offsets[RED];
}
unsigned char Photo::getPixelGreenComponent(int i, int j,
                                            const Matrix<uint32_t>& data) {
  return (data(i, j) & colorInfo.green_mask) >> offsets[GREEN];
}
unsigned char Photo::getPixelBlueComponent(int i, int j,
                                           const Matrix<uint32_t>& data) {
  return (data(i, j) & colorInfo.blue_mask) >> offsets[BLUE];
}
const uint32_t& Photo::getPixel(int i, int j) const { return data(i, j); }

uint32_t Photo::getPixelFromComponents(int r, int g, int b) {
  uint32_t pixel = 0;
  pixel = r << offsets[RED];
  pixel += g << offsets[GREEN];
  pixel += b << offsets[BLUE];
  return pixel;
}

}  // namespace s21
