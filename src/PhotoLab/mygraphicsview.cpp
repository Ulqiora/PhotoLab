#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent), controller(nullptr) {
  connect(this, SIGNAL(pictureChanged()), SLOT(repaint()));
}

QPixmap MyGraphicsView::setPicture() {
  QImage *image =
      new QImage(controller->getPhotoWidth(), controller->getPhotoHeight(),
                 QImage::Format_RGB666);  //создаем изображение
  QColor pixel;
  for (int i = controller->getPhotoHeight() - 1; i >= 0; i--) {
    for (int j = 0; j < controller->getPhotoWidth(); j++) {
      pixel.setRed(controller->getRed(i, j));
      pixel.setGreen(controller->getGreen(i, j));
      pixel.setBlue(controller->getBlue(i, j));
      image->setPixel(j, controller->getPhotoHeight() - i - 1, pixel.rgb());
    }
  }
  return QPixmap::fromImage(*image);
}

void MyGraphicsView::update(QPixmap pixmap) {
  scene = new QGraphicsScene();
  scene->addPixmap(pixmap);
  this->setScene(scene);
}
