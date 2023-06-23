#include "hsv_filters.h"

#include "ui_hsv_filters.h"

namespace s21 {
HSV_filters::HSV_filters(QWidget *parent)
    : QDialog(parent), ui(new Ui::HSV_filters) {
  ui->setupUi(this);
  ui->lineEdit->setText("0");
  ui->lineEdit_2->setText("0");
  ui->lineEdit_3->setText("0");
}

HSV_filters::~HSV_filters() { delete ui; }

void HSV_filters::slotFromMainWindow() {
  ui->HUE_slider->setSliderPosition(0);
  ui->SATURATION_slider->setSliderPosition(0);
  ui->VALUE_slider->setSliderPosition(0);
  hsvTemp = {0, 0, 0};
}

void HSV_filters::on_HUE_slider_valueChanged(int hue) {
  hsvWork.hue_ = hue;
  QString str = QString::number(hue);
  ui->lineEdit->setText(str);
  emit signalToMainWindow(hue, hsvWork.saturation_, hsvWork.value_);
}

void HSV_filters::on_SATURATION_slider_valueChanged(int saturation) {
  hsvWork.saturation_ = saturation;
  QString str = QString::number(saturation);
  ui->lineEdit_2->setText(str);
  emit signalToMainWindow(hsvWork.hue_, saturation, hsvWork.value_);
}

void HSV_filters::on_VALUE_slider_valueChanged(int value) {
  hsvWork.value_ = value;
  QString str = QString::number(value);
  ui->lineEdit_3->setText(str);
  emit signalToMainWindow(hsvWork.hue_, hsvWork.saturation_, value);
}

void HSV_filters::on_pushButton_clicked() {
  hsvTemp = hsvWork;
  emit signalToMainWindowApply();
  close();
}

void HSV_filters::on_lineEdit_textChanged(const QString &arg1) {
  ui->lineEdit->setValidator(new QIntValidator(0, 359, this));
  int pos = arg1.toInt();
  ui->HUE_slider->setSliderPosition(pos);
  emit signalToMainWindow(pos, hsvWork.saturation_, hsvWork.value_);
}

void HSV_filters::on_lineEdit_2_textChanged(const QString &arg1) {
  ui->lineEdit_2->setValidator(new QIntValidator(0, 359, this));
  int pos = arg1.toInt();
  ui->SATURATION_slider->setSliderPosition(pos);
  emit signalToMainWindow(hsvWork.hue_, pos, hsvWork.value_);
}

void HSV_filters::on_lineEdit_3_textChanged(const QString &arg1) {
  ui->lineEdit_3->setValidator(new QIntValidator(0, 359, this));
  int pos = arg1.toInt();
  ui->VALUE_slider->setSliderPosition(pos);
  emit signalToMainWindow(hsvWork.hue_, hsvWork.saturation_, pos);
}

void HSV_filters::on_close_clicked() {
  ui->HUE_slider->setSliderPosition(hsvTemp.hue_);
  ui->SATURATION_slider->setSliderPosition(hsvTemp.saturation_);
  ui->VALUE_slider->setSliderPosition(hsvTemp.value_);
  emit signalToMainWindow(hsvTemp.hue_, hsvTemp.saturation_, hsvTemp.value_);
  close();
}

}  // namespace s21
