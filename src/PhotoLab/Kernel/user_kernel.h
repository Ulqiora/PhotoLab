#ifndef USER_KERNEL_H
#define USER_KERNEL_H

#include <QAbstractButton>
#include <QDialog>
#include <QMessageBox>
#include <vector>

namespace Ui {
class User_kernel;
}

class User_kernel : public QDialog {
  Q_OBJECT

 public:
  explicit User_kernel(QWidget *parent = nullptr);
  ~User_kernel();

  std::vector<float> getVector() { return kernel; }

 private slots:

  void on_buttonBox_accepted();

  void on_pushButton_clicked();

  void on_buttonBox_rejected();

 private:
  Ui::User_kernel *ui;
  int size;
  std::vector<float> kernel;
};

#endif  // USER_KERNEL_H
