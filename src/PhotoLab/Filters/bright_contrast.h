#ifndef BRIGHT_CONTRAST_H
#define BRIGHT_CONTRAST_H

#include <QDialog>
#include <QIntValidator>

namespace Ui {
class Bright_Contrast;
}

namespace s21 {
class Bright_Contrast : public QDialog {
  Q_OBJECT

 public:
  explicit Bright_Contrast(QWidget *parent = nullptr);
  ~Bright_Contrast();

 private slots:
  void on_Brightness_slider_valueChanged(int value);

  void on_Contrast_slider_valueChanged(int value);

  void on_pushButton_clicked();

  void on_lineEdit_2_textChanged(const QString &arg1);

  void on_lineEdit_textChanged(const QString &arg1);

  void on_close_clicked();

 private:
  Ui::Bright_Contrast *ui;
  struct BCvalues {
    int bright_ = 100;
    int contrast_ = 0;
  };

  BCvalues bcTemp;
  BCvalues bcWork;

 signals:
  void signalToMainWindowBright(int bright);
  void signalToMainWindowContrast(int contrast);
  void signalToMainWindowApply();

 public slots:
  void slotFromMainWindow();
};
}  // namespace s21
#endif  // BRIGHT_CONTRAST_H
