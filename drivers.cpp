#include "drivers.h"
#include "ui_drivers.h"
#include "mainwindow.h"
#include "admin.h"

drivers::drivers(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::drivers)
{
    ui->setupUi(this);
    setWindowTitle("Водители");

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Имя", "Фамилия", "Количество нарушений", "Номер ТС"});
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");
}

drivers::~drivers()
{
    delete ui;
}

void drivers::setInfo(const QList<QVariantList> &violationsList) {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setColumnWidth(2, 400);
    ui->tableWidget->setColumnWidth(3, 200);
    ui->tableWidget->setStyleSheet("QTableWidget {font-size: 18pt; color: black;}");

    for (const QVariantList &violationData : violationsList) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(violationData[0].toString())); // имя
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(violationData[1].toString())); // фамилия
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(violationData[2].toString())); // количество нарушений
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(violationData[3].toString())); // номер тс
    }
}

void drivers::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void drivers::on_back_clicked()
{
    hide();
    admin *adminWindow = new admin(this);
    adminWindow->show();
}

