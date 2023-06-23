#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(Controller *c, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller(c) {
  ui->setupUi(this);
  ui->graphicsView->setController(c);
  ui->graphicsView_2->setController(c);
  hslFilterWindow = new HSL_filters();
  hsvFilterWindow = new HSV_filters();
  brightContrast = new Bright_Contrast();

  connect(hslFilterWindow, &HSL_filters::signalToMainWindow, this,
          &MainWindow::slotFromHSL);
  connect(hslFilterWindow, &HSL_filters::signalToMainWindowLight, this,
          &MainWindow::slotFromHSL_Light);
  connect(hslFilterWindow, &HSL_filters::signalToMainWindowApply, this,
          &MainWindow::slotFromHSL_Apply);
  connect(this, &MainWindow::signalToCancel, hslFilterWindow,
          &HSL_filters::slotFromMainWindow);

  connect(hsvFilterWindow, &HSV_filters::signalToMainWindow, this,
          &MainWindow::slotFromHSV);
  connect(hsvFilterWindow, &HSV_filters::signalToMainWindowApply, this,
          &MainWindow::slotFromHSV_Apply);
  connect(this, &MainWindow::signalToCancel, hsvFilterWindow,
          &HSV_filters::slotFromMainWindow);

  connect(brightContrast, &Bright_Contrast::signalToMainWindowBright, this,
          &MainWindow::slotFromBright);
  connect(brightContrast, &Bright_Contrast::signalToMainWindowContrast, this,
          &MainWindow::slotFromContrast);
  connect(brightContrast, &Bright_Contrast::signalToMainWindowApply, this,
          &MainWindow::slotFromBright_Apply);
  connect(this, &MainWindow::signalToCancel, brightContrast,
          &Bright_Contrast::slotFromMainWindow);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::update_pixmap() {
  pixmap_after = ui->graphicsView_2->setPicture();
  ui->graphicsView_2->update(pixmap_after);
}

void MainWindow::on_Open_File_triggered() { on_openFile_clicked(); }

void MainWindow::on_Close_program_triggered() { on_close_clicked(); }

void MainWindow::on_Save_File_triggered() { on_saveFile_clicked(); }

void MainWindow::on_menu_clicked() {
  int x = ui->mainArea->geometry().x();
  int newX;
  if (x == 100) {
    newX = 190;
  } else {
    newX = 100;
  }
  QPropertyAnimation *anim = new QPropertyAnimation(ui->mainArea, "pos");
  anim->setDuration(250);
  anim->setStartValue(QPoint(x, 10));
  anim->setEndValue(QPoint(newX, 10));
  anim->setEasingCurve(QEasingCurve::InOutQuad);
  anim->start();
}

void MainWindow::on_close_clicked() { QApplication::quit(); }

void MainWindow::on_openFile_clicked() {
  QString path;
  path = QFileDialog::getOpenFileName(
      this, "Open File", HOME_DIR "/CPP8_PhotoLab_v1.0-1/misc/images/",
      "*.bmp");
  if (path != nullptr) {
    std::string filePath = path.toStdString();
    try {
      controller->openFile(filePath);
      pixmap_before = ui->graphicsView->setPicture();
      ui->graphicsView->update(pixmap_before);
      pixmap_after = ui->graphicsView_2->setPicture();
      ui->graphicsView_2->update(pixmap_after);
      QFileInfo fileinfo(path);
      filename = fileinfo.fileName();
      ui->fileName->setText(filename);
      ui->fileName_2->setText(filename);
    } catch (std::exception &exceptionText) {
      QMessageBox mb;
      mb.setText(exceptionText.what());
      QPixmap exportSuccess(":/icons/img/error.png");
      mb.setIconPixmap(exportSuccess);
      mb.exec();
    }
  }
}

void MainWindow::on_saveFile_clicked() {
  if (pixmap_after.isNull()) {
    QMessageBox mb;
    mb.setText("Please upload an image first!");
    QPixmap exportSuccess(":/icons/img/error.png");
    mb.setIconPixmap(exportSuccess);
    mb.exec();
  } else {
    QString fileName = QFileDialog::getSaveFileName(
        this, "Save txt", HOME_DIR "/Applications/PhotoLab/Save/" + filename,
        "*.bmp");
    if (fileName != nullptr) pixmap_after.save(fileName);
  }
}

void MainWindow::on_Toning_triggered() { on_toning_clicked(); }

void MainWindow::on_filters_clicked() { on_menu_clicked(); }

void MainWindow::on_cancelChanges_clicked() {
  if (!pixmap_after.isNull()) {
    controller->cancelFiltration();
    update_pixmap();
  }
  emit signalToCancel();
}

void MainWindow::on_emboss_clicked() {
  if (!pixmap_after.isNull()) {
    controller->kernelFiltration(EMBOSS);
    update_pixmap();
  }
}

void MainWindow::on_box_blur_clicked() {
  if (!pixmap_after.isNull()) {
    controller->kernelFiltration(BOX_BLUR);
    update_pixmap();
  }
}

void MainWindow::on_sharpen_clicked() {
  if (!pixmap_after.isNull()) {
    controller->kernelFiltration(SHARPEN);
    update_pixmap();
  }
}

void MainWindow::on_gaussian_clicked() {
  if (!pixmap_after.isNull()) {
    controller->kernelFiltration(GAUSSIAN);
    update_pixmap();
  }
}

void MainWindow::on_laplacian_clicked() {
  if (!pixmap_after.isNull()) {
    controller->kernelFiltration(LAPLACIAN);
    update_pixmap();
  }
}

void MainWindow::on_sobelLeft_clicked() {
  if (!pixmap_after.isNull()) {
    controller->kernelFiltration(SOBEL_LEFT);
    update_pixmap();
  }
}

void MainWindow::on_sobelRight_clicked() {
  if (!pixmap_after.isNull()) {
    controller->kernelFiltration(SOBEL_RIGHT);
    update_pixmap();
  }
}

void MainWindow::on_discolor_clicked() {
  if (!pixmap_after.isNull()) {
    controller->simpleFiltration(DISCOLOR);
    update_pixmap();
  }
}

void MainWindow::on_negative_clicked() {
  if (!pixmap_after.isNull()) {
    controller->simpleFiltration(NEGATIVE);
    update_pixmap();
  }
}

void MainWindow::on_toning_clicked() {
  if (!pixmap_after.isNull()) {
    QColorDialog dialog(this);
    QColor color = dialog.getColor();
    controller->toningSimpleFiltration(color.red(), color.green(),
                                       color.blue());
    update_pixmap();
  }
}

void MainWindow::on_user_kernel_clicked() {
  if (!pixmap_after.isNull()) {
    user_kernel.setModal(true);
    user_kernel.exec();
    std::vector<float> kernel1 = user_kernel.getVector();
    if (!kernel1.empty()) {
      Matrix<float> kernel;
      kernel.fill_matrix(kernel1);
      controller->kernelFiltration(kernel);
      update_pixmap();
    }
  }
}

void MainWindow::on_pushButton_clicked() {
  hslFilterWindow->setModal(true);
  hslFilterWindow->show();
}

void MainWindow::slotFromHSL(int hue, int sat) {
  if (!pixmap_after.isNull()) {
    controller->HSL(hue, sat);
    update_pixmap();
  }
}

void MainWindow::slotFromHSL_Light(int light) {
  if (!pixmap_after.isNull()) {
    controller->brightness(light / 100.0);
    update_pixmap();
  }
}

void MainWindow::slotFromHSL_Apply() { controller->applyChanges(); }

void MainWindow::on_pushButton_2_clicked() {
  hsvFilterWindow->setModal(true);
  hsvFilterWindow->show();
}

void MainWindow::slotFromHSV(int hue, int saturation, int value) {
  if (!pixmap_after.isNull()) {
    controller->HSV(hue, saturation, value);
    update_pixmap();
  }
}

void MainWindow::slotFromHSV_Apply() { controller->applyChanges(); }

void MainWindow::slotFromBright(int bright) {
  if (!pixmap_after.isNull()) {
    controller->brightness(bright / 100.0);
    update_pixmap();
  }
}

void MainWindow::slotFromContrast(int contrast) {
  if (!pixmap_after.isNull()) {
    float contrastFactor = (100.0 + contrast) / 100.0;
    contrastFactor *= contrastFactor;
    controller->contrast(contrastFactor);
    update_pixmap();
  }
}

void MainWindow::slotFromBright_Apply() { controller->applyChanges(); }

void MainWindow::on_pushButton_3_clicked() {
  brightContrast->setModal(true);
  brightContrast->show();
}

}  // namespace s21
