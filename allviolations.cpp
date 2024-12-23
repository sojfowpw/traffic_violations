#include "allviolations.h"
#include "ui_allviolations.h"
#include "mainwindow.h"
#include "account.h"
#include "admin.h"

allViolations::allViolations(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::allViolations)
{
    ui->setupUi(this);
    setWindowTitle("Штрафы");

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"Тип нарушения", "Сумма штрафа"});
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");
}

allViolations::~allViolations()
{
    delete ui;
}

void allViolations::setInfo(int idUser, const QList<QVariantList> &violationsList) {
    userId = idUser;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0, 600);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setStyleSheet("QTableWidget {font-size: 18pt; color: black;}");

    for (const QVariantList &violationData : violationsList) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(violationData[0].toString())); // тип нарушения
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(violationData[1].toString())); // сумма
    }
}

void allViolations::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void allViolations::on_back_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT name, surname, phone, amount_fines, id_vehicle FROM owners WHERE id = :id");
    query.bindValue(":id", userId); // получаем данные пользователя по айди
    query.exec();
    query.next();
    QString name = query.value("name").toString();
    QString surname = query.value("surname").toString();
    QString phone = query.value("phone").toString();
    int amount_fines = query.value("amount_fines").toInt();
    int id_vehicle = query.value("id_vehicle").toInt();
    query.prepare("SELECT license_plate FROM vehicles WHERE id = :id"); // получаем номер ТС
    query.bindValue(":id", id_vehicle);
    query.exec();
    query.next();
    QString transport = query.value("license_plate").toString();

    hide();
    account *acc = new account(this);
    acc->show();
    QObject::connect(this, &allViolations::sendUserInfo, acc, &account::setInfo); // связываем классы для передачи информации
    emit sendUserInfo(name, surname, transport, amount_fines, phone);
}

void allViolations::hideButton() {
    ui->back->setVisible(false);
}

void allViolations::hideButton2() {
    ui->backAdmin->setVisible(false);
}


void allViolations::on_backAdmin_clicked()
{
    hide();
    admin *adminWindow = new admin(this);
    adminWindow->show();
}

