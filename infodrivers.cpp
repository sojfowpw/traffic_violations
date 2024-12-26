#include "infodrivers.h"
#include "ui_infodrivers.h"
#include "mainwindow.h"
#include "admin.h"

infoDrivers::infoDrivers(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::infoDrivers)
{
    ui->setupUi(this);
    setWindowTitle("Водители");
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Имя", "Фамилия", "Тип нарушения", "Статус"});
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");
}

infoDrivers::~infoDrivers()
{
    delete ui;
}

void infoDrivers::setInfo(const QList<QVariantList> &violationsList) {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setColumnWidth(2, 500);
    ui->tableWidget->setColumnWidth(3, 200);
    ui->tableWidget->setStyleSheet("QTableWidget {font-size: 18pt; color: black;}");

    for (const QVariantList &violationData : violationsList) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(violationData[0].toString())); // имя
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(violationData[1].toString())); // фамилия
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(violationData[2].toString())); // нарушение
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(violationData[3].toString())); // статус
    }
}

void infoDrivers::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void infoDrivers::on_back_clicked()
{
    hide();
    admin *adm = new admin(this);
    adm->show();
}

