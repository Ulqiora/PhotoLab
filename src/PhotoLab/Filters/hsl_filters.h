#ifndef HSL_FILTERS_H
#define HSL_FILTERS_H

#include <QDialog>
#include <QIntValidator>

namespace Ui {
class HSL_filters;
}

namespace s21 {
class HSL_filters : public QDialog {
  Q_OBJECT

 public:
  HSL_filters(QWidget *parent = nullptr);
  ~HSL_filters();

 private slots:
  void on_HUE_slider_valueChanged(int value);

  void on_SATURATION_HSL_slider_valueChanged(int saturation);

  void on_LIGHTNESS_HSL_slider_valueChanged(int lightness);

  void on_pushButton_clicked();

  void on_lineEdit_textChanged(const QString &arg1);

  void on_lineEdit_2_textChanged(const QString &arg1);

  void on_lineEdit_3_textChanged(const QString &arg1);

  void on_close_clicked();

 private:
  Ui::HSL_filters *ui;

  struct HSLvalues {
    int hue_ = 0;
    int saturation_ = 100;
    int lightness_ = 100;
  };

  HSLvalues hslTemp;
  HSLvalues hslWork;

 signals:
  void signalToMainWindow(int hue, int sat);
  void signalToMainWindowLight(int lightness);
  void signalToMainWindowApply();

 public slots:
  void slotFromMainWindow();
};
}  // namespace s21

#endif  // HSL_FILTERS_H
