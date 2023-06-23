#include "user_kernel.h"

#include "ui_user_kernel.h"

User_kernel::User_kernel(QWidget *parent)
    : QDialog(parent), ui(new Ui::User_kernel) {
  ui->setupUi(this);
  size = 0;
  kernel.clear();
}

User_kernel::~User_kernel() { delete ui; }

void User_kernel::on_buttonBox_accepted() {
  kernel.clear();
  float num;
  bool check = true;
  for (int i = 0; i < size && check; i++) {
    for (int j = 0; j < size; j++) {
      QString x = ui->tableWidget->item(i, j)->text();
      num = x.toFloat(&check);
      if (check == false) {
        QMessageBox mb;
        mb.setText("Incorrect values ​​entered");
        QPixmap exportSuccess(":/icons/img/error.png");
        mb.setIconPixmap(exportSuccess);
        mb.exec();
        kernel.clear();
        break;
      } else
        kernel.push_back(num);
    }
  }
  if (check) close();
}

void User_kernel::on_pushButton_clicked() {
  int box = ui->comboBox->currentIndex();
  switch (box) {
    case 0:
      size = 3;
      break;
    case 1:
      size = 5;
      break;
    case 2:
      size = 7;
      break;
    case 3:
      size = 9;
      break;
    case 4:
      size = 11;
      break;
    case 5:
      size = 13;
      break;
    case 6:
      size = 15;
      break;
    default:
      size = 3;
      break;
  }
  ui->tableWidget->setColumnCount(size);
  ui->tableWidget->setRowCount(size);

  int x = 400 / size;
  int y = 400 - (x * size);
  for (int i = 0; i < size; i++) {
    if (i < y) {
      ui->tableWidget->setColumnWidth(i, x + 1);
      ui->tableWidget->setRowHeight(i, x + 1);
    } else {
      ui->tableWidget->setColumnWidth(i, x);
      ui->tableWidget->setRowHeight(i, x);
    }
  }
  ui->tableWidget->verticalHeader()->hide();
  ui->tableWidget->horizontalHeader()->hide();
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) {
      ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(0)));
      ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
    }
}

void User_kernel::on_buttonBox_rejected() { close(); }
