#include <gtest/gtest.h>

#include <iostream>

#include "Controller/controller.h"

#define PHOTO "../misc/images/Lena.bmp"
#define PHOTO_ERROR "../misc/images/rrena.bmp"

s21::Photo p;
s21::Controller ctr(&p);

TEST(test_photolab, open) {
  ctr.openFile(PHOTO);
  ASSERT_NO_THROW(ctr.openFile(PHOTO));
}

TEST(test_photolab, openError) {
  ctr.openFile(PHOTO);
  ASSERT_ANY_THROW(ctr.openFile(PHOTO_ERROR));
}

TEST(test_photolab, sizeCheck) {
  ctr.openFile(PHOTO);
  ASSERT_EQ(ctr.getPhotoHeight(), 512);
  ASSERT_EQ(ctr.getPhotoWidth(), 512);
}

TEST(test_photolab, pixelCanalsCheck) {
  ctr.openFile(PHOTO);
  ASSERT_EQ((int)ctr.getRed(0, 0), 82);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 22);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 57);

  ASSERT_EQ((int)ctr.getRed(100, 100), 115);
  ASSERT_EQ((int)ctr.getGreen(100, 100), 45);
  ASSERT_EQ((int)ctr.getBlue(100, 100), 89);
}

TEST(test_photolab, kernelFiltration_Emboss) {
  ctr.openFile(PHOTO);
  ctr.kernelFiltration(s21::EMBOSS);
  ASSERT_EQ((int)ctr.getRed(100, 100), 72);
  ASSERT_EQ((int)ctr.getGreen(100, 100), 0);
  ASSERT_EQ((int)ctr.getBlue(100, 100), 49);
}

TEST(test_photolab, kernelFiltration_User) {
  ctr.openFile(PHOTO);
  std::vector<float> k = {-1, -1, 2,  -1, -1, -1, 1,  2,  1,  -1, 2,  2, 8,
                          2,  2,  -1, 1,  2,  1,  -1, -1, -1, 2,  -1, -1};
  s21::Matrix<float> kernel;
  kernel.fill_matrix(k);
  ctr.kernelFiltration(kernel);
  ASSERT_EQ((int)ctr.getRed(100, 100), 86);
  ASSERT_EQ((int)ctr.getGreen(100, 100), 8);
  ASSERT_EQ((int)ctr.getBlue(100, 100), 53);
}

TEST(test_photolab, kernelFiltration_Laplasian) {
  ctr.openFile(PHOTO);
  ctr.kernelFiltration(s21::LAPLACIAN);
  ASSERT_EQ((int)ctr.getRed(0, 0), 0);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 0);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 0);
}

TEST(test_photolab, kernelFiltration_BoxBlur) {
  ctr.openFile(PHOTO);
  ctr.kernelFiltration(s21::BOX_BLUR);
  ASSERT_EQ((int)ctr.getRed(0, 0), 82);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 22);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 56);
  ctr.kernelFiltration(s21::BOX_BLUR);
  ctr.kernelFiltration(s21::BOX_BLUR);
  ctr.kernelFiltration(s21::BOX_BLUR);
  ASSERT_EQ((int)ctr.getRed(0, 0), 84);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 22);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 57);
}

TEST(test_photolab, simpleFiltration_Toning) {
  ctr.openFile(PHOTO);
  ctr.toningSimpleFiltration(122, 122, 122);
  ASSERT_EQ((int)ctr.getRed(0, 0), 117);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 75);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 99);

  ctr.toningSimpleFiltration(30, 30, 240);
  ASSERT_EQ((int)ctr.getRed(0, 0), 96);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 67);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 186);
}

TEST(test_photolab, simpleFiltration_Negative) {
  ctr.openFile(PHOTO);
  int red = ctr.getRed(0, 0);
  int green = ctr.getGreen(0, 0);
  int blue = ctr.getBlue(0, 0);
  ctr.simpleFiltration(s21::NEGATIVE);
  ASSERT_EQ((int)ctr.getRed(0, 0), 255 - red);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 255 - green);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 255 - blue);
}

TEST(test_photolab, simpleFiltration_Discolor) {
  ctr.openFile(PHOTO);
  ctr.simpleFiltration(s21::DISCOLOR);
  ASSERT_EQ((int)ctr.getRed(0, 0), 43);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 43);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 43);
}

TEST(test_photolab, brightness) {
  ctr.openFile(PHOTO);
  ctr.brightness(155 / 100.0);
  ASSERT_EQ((int)ctr.getRed(0, 0), 127);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 34);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 88);
}

TEST(test_photolab, contrast) {
  ctr.openFile(PHOTO);
  int contrast = 45;
  float contrastFactor = (100.0 + contrast) / 100.0;
  contrastFactor *= contrastFactor;
  ctr.contrast(contrastFactor);
  ASSERT_EQ((int)ctr.getRed(0, 0), 31);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 0);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 0);
}

TEST(test_photolab, HSV) {
  ctr.openFile(PHOTO);
  ctr.HSV(304, 80, 99);
  ASSERT_EQ((int)ctr.getRed(0, 0), 126);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 57);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 234);
  ctr.HSV(30, 99, 70);
  ASSERT_EQ((int)ctr.getRed(0, 0), 145);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 23);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 58);
}

TEST(test_photolab, HSL) {
  ctr.openFile(PHOTO);
  ctr.HSL(304, 80);
  ASSERT_EQ((int)ctr.getRed(0, 0), 88);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 31);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 201);
}

TEST(test_photolab, cancelFiltration) {
  ctr.openFile(PHOTO);
  ctr.simpleFiltration(s21::DISCOLOR);
  ctr.toningSimpleFiltration(122, 122, 122);
  ctr.cancelFiltration();
  ASSERT_EQ((int)ctr.getRed(0, 0), 82);
  ASSERT_EQ((int)ctr.getGreen(0, 0), 22);
  ASSERT_EQ((int)ctr.getBlue(0, 0), 57);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
