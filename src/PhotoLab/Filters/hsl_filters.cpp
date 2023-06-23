#include "hsl_filters.h"

#include "ui_hsl_filters.h"

namespace s21 {

HSL_filters::HSL_filters(QWidget *parent)
    : QDialog(parent), ui(new Ui::HSL_filters) {
  ui->setupUi(this);
  ui->lineEdit->setText("0");
  ui->lineEdit_2->setText("100");
  ui->lineEdit_3->setText("100");
}

HSL_filters::~HSL_filters() { delete ui; }

void HSL_filters::on_HUE_slider_valueChanged(int hue) {
  hslWork.hue_ = hue;
  QString str = QString::number(hue);
  ui->lineEdit->setText(str);
  emit signalToMainWindow(hue, hslWork.saturation_);
}

void HSL_filters::on_SATURATION_HSL_slider_valueChanged(int saturation) {
  hslWork.saturation_ = saturation;
  QString str = QString::number(saturation);
  ui->lineEdit_2->setText(str);
  emit signalToMainWindow(hslWork.hue_, saturation);
}

void HSL_filters::on_LIGHTNESS_HSL_slider_valueChanged(int lightness) {
  hslWork.lightness_ = lightness;
  QString str = QString::number(lightness);
  ui->lineEdit_3->setText(str);
  emit signalToMainWindowLight(lightness);
}

void HSL_filters::on_pushButton_clicked() {
  hslTemp = hslWork;
  emit signalToMainWindowApply();
  close();
}

void HSL_filters::slotFromMainWindow() {
  ui->HUE_slider->setSliderPosition(0);
  ui->SATURATION_HSL_slider->setSliderPosition(100);
  ui->LIGHTNESS_HSL_slider->setSliderPosition(100);
  hslTemp = {0, 100, 100};
}

void HSL_filters::on_lineEdit_textChanged(const QString &arg1) {
  ui->lineEdit->setValidator(new QIntValidator(0, 359, this));
  int pos = arg1.toInt();
  ui->HUE_slider->setSliderPosition(pos);
  emit signalToMainWindow(pos, hslWork.saturation_);
}

void HSL_filters::on_lineEdit_2_textChanged(const QString &arg1) {
  ui->lineEdit_2->setValidator(new QIntValidator(0, 200, this));
  int pos = arg1.toInt();
  ui->SATURATION_HSL_slider->setSliderPosition(pos);
  emit signalToMainWindow(hslWork.hue_, pos);
}

void HSL_filters::on_lineEdit_3_textChanged(const QString &arg1) {
  ui->lineEdit_3->setValidator(new QIntValidator(0, 200, this));
  int pos = arg1.toInt();
  ui->LIGHTNESS_HSL_slider->setSliderPosition(pos);
  emit signalToMainWindowLight(pos);
}

void HSL_filters::on_close_clicked() {
  ui->HUE_slider->setSliderPosition(hslTemp.hue_);
  ui->SATURATION_HSL_slider->setSliderPosition(hslTemp.saturation_);
  ui->LIGHTNESS_HSL_slider->setSliderPosition(hslTemp.lightness_);
  emit signalToMainWindow(hslTemp.hue_, hslTemp.saturation_);
  emit signalToMainWindowLight(hslTemp.lightness_);
  close();
}

}  // namespace s21
