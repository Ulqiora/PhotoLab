#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPropertyAnimation>

#include "../Controller/controller.h"
#include "Filters/bright_contrast.h"
#include "Filters/hsl_filters.h"
#include "Filters/hsv_filters.h"
#include "Kernel/user_kernel.h"
#include "mygraphicsview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(Controller *c, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:

  void update_pixmap();

  void on_Open_File_triggered();

  void on_Close_program_triggered();

  void on_Save_File_triggered();

  void on_Toning_triggered();

  void on_menu_clicked();

  void on_close_clicked();

  void on_openFile_clicked();

  void on_saveFile_clicked();

  void on_filters_clicked();

  void on_cancelChanges_clicked();

  void on_emboss_clicked();

  void on_box_blur_clicked();

  void on_sharpen_clicked();

  void on_gaussian_clicked();

  void on_laplacian_clicked();

  void on_sobelLeft_clicked();

  void on_sobelRight_clicked();

  void on_discolor_clicked();

  void on_negative_clicked();

  void on_toning_clicked();

  void on_user_kernel_clicked();

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

 private:
  Ui::MainWindow *ui;
  Controller *controller;
  User_kernel user_kernel;
  HSL_filters *hslFilterWindow;
  HSV_filters *hsvFilterWindow;
  Bright_Contrast *brightContrast;

  QString filename;
  QPixmap pixmap_before;
  QPixmap pixmap_after;

  int hue_ = 0, saturation_ = 0, value_ = 0;
  int HSLhue_ = 0, HSLsaturation_ = 100, HSLlightness_ = 0;

 public slots:
  void slotFromHSL(int hue, int sat);
  void slotFromHSL_Light(int light);
  void slotFromHSL_Apply();
  void slotFromHSV(int hue, int saturation, int value);
  void slotFromHSV_Apply();
  void slotFromBright(int bright);
  void slotFromContrast(int contrast);
  void slotFromBright_Apply();

 signals:
  void signalToCancel();
};
}  // namespace s21

#endif  // MAINWINDOW_H
