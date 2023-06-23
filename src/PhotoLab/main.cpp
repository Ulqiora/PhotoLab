#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Photo p;
  s21::Controller c(&p);
  s21::MainWindow w(&c);

  w.show();
  return a.exec();
}
