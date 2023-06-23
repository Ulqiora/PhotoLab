#include "bright_contrast.h"

#include "ui_bright_contrast.h"

namespace s21 {

Bright_Contrast::Bright_Contrast(QWidget *parent)
    : QDialog(parent), ui(new Ui::Bright_Contrast) {
  ui->setupUi(this);
  ui->lineEdit->setText("100");
  ui->lineEdit_2->setText("0");
}

Bright_Contrast::~Bright_Contrast() { delete ui; }

void Bright_Contrast::on_Brightness_slider_valueChanged(int value) {
  bcWork.bright_ = value;
  QString str = QString::number(value);
  ui->lineEdit->setText(str);
  emit signalToMainWindowBright(value);
}

void Bright_Contrast::on_Contrast_slider_valueChanged(int value) {
  bcWork.contrast_ = value;
  QString str = QString::number(value);
  ui->lineEdit_2->setText(str);
  emit signalToMainWindowContrast(value);
}

void Bright_Contrast::on_pushButton_clicked() {
  bcTemp = bcWork;
  emit signalToMainWindowApply();
  close();
}

void Bright_Contrast::slotFromMainWindow() {
  ui->Brightness_slider->setSliderPosition(100);
  ui->Contrast_slider->setSliderPosition(0);
  bcTemp = {100, 0};
}

void Bright_Contrast::on_lineEdit_2_textChanged(const QString &arg1) {
  ui->lineEdit_2->setValidator(new QIntValidator(-100, 100, this));
  int pos = arg1.toInt();
  ui->Contrast_slider->setSliderPosition(pos);
  emit signalToMainWindowContrast(pos);
}

void Bright_Contrast::on_lineEdit_textChanged(const QString &arg1) {
  ui->lineEdit->setValidator(new QIntValidator(0, 200, this));
  int pos = arg1.toInt();
  ui->Brightness_slider->setSliderPosition(pos);
  emit signalToMainWindowBright(pos);
}

void Bright_Contrast::on_close_clicked() {
  ui->Brightness_slider->setSliderPosition(bcTemp.bright_);
  ui->Contrast_slider->setSliderPosition(bcTemp.contrast_);
  emit signalToMainWindowBright(100);
  emit signalToMainWindowContrast(0);
  close();
}

}  // namespace s21
