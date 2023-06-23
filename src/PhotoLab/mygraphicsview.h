#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

#include "../Controller/controller.h"

class MyGraphicsView : public QGraphicsView {
  Q_OBJECT
 public:
  explicit MyGraphicsView(QWidget *parent = nullptr);
  ~MyGraphicsView() {}

  void setController(s21::Controller *c) { controller = c; }
  QPixmap setPicture();
  void update(QPixmap pixmap);

 signals:
  void pictureChanged();

 private:
  s21::Controller *controller;
  QPixmap picture;
  QGraphicsScene *scene;

 protected:
};

#endif  // MYGRAPHICSVIEW_H
