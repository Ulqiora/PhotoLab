#ifndef HSV_FILTERS_H
#define HSV_FILTERS_H

#include <QDialog>
#include <QIntValidator>

namespace Ui {
class HSV_filters;
}

namespace s21 {

class HSV_filters : public QDialog {
  Q_OBJECT

 public:
  explicit HSV_filters(QWidget *parent = nullptr);
  ~HSV_filters();

 private:
  Ui::HSV_filters *ui;

  struct HSVvalues {
    int hue_ = 0;
    int saturation_ = 0;
    int value_ = 0;
  };

  HSVvalues hsvTemp;
  HSVvalues hsvWork;

 signals:
  void signalToMainWindow(int hue, int sat, int value);
  void signalToMainWindowApply();

 public slots:
  void slotFromMainWindow();

 private slots:
  void on_HUE_slider_valueChanged(int value);
  void on_SATURATION_slider_valueChanged(int value);
  void on_VALUE_slider_valueChanged(int value);
  void on_pushButton_clicked();
  void on_lineEdit_textChanged(const QString &arg1);
  void on_lineEdit_2_textChanged(const QString &arg1);
  void on_lineEdit_3_textChanged(const QString &arg1);

  void on_close_clicked();
};
}  // namespace s21

#endif  // HSV_FILTERS_H
