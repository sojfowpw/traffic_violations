#include "changestatusadmin.h"
#include "ui_changestatusadmin.h"
#include "mainwindow.h"
#include "admin.h"

changeStatusAdmin::changeStatusAdmin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::changeStatusAdmin)
{
    ui->setupUi(this);
    setWindowTitle("Оплата");

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Имя", "Фамилия", "Тип нарушения", "Статус"});
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section {font-size: 18pt; background-color: rgb(246, 97, 81); color: black;}");

    ui->changeLabel->setVisible(false);
    ui->changeLine->setVisible(false);
    ui->save->setVisible(false);
}

changeStatusAdmin::~changeStatusAdmin()
{
    delete ui;
}

void changeStatusAdmin::setInfo(const QList<QVariantList> &violationsList) {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setColumnWidth(2, 600);
    ui->tableWidget->setColumnWidth(3, 200);
    ui->tableWidget->setStyleSheet("QTableWidget {font-size: 18pt; color: black;}");

    for (const QVariantList &violationData : violationsList) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(violationData[0].toString())); // имя
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(violationData[1].toString())); // фамилия
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(violationData[2].toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(violationData[3].toString()));
    }
}

void changeStatusAdmin::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void changeStatusAdmin::on_back_clicked()
{
    hide();
    admin *adminWindow = new admin(this);
    adminWindow->show();
}


void changeStatusAdmin::on_acceptChange_clicked()
{
    ui->changeLabel->setVisible(!ui->changeLabel->isVisible());
    ui->changeLine->setVisible(!ui->changeLine->isVisible());
    ui->save->setVisible(!ui->save->isVisible());
}


void changeStatusAdmin::on_save_clicked()
{
    QString rowStr = ui->changeLine->text();
    if (rowStr.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле не должно быть пустым</FONT>");
        return;
    }
    int row = rowStr.toInt();
    if (row < 1 || row > ui->tableWidget->rowCount()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Некорректный ввод</FONT>");
        return;
    }
    QString name = ui->tableWidget->item(row - 1, 0)->text();
    QString surname = ui->tableWidget->item(row - 1, 1)->text();
    QString violationType = ui->tableWidget->item(row - 1, 2)->text();
    QSqlQuery query;
    query.prepare("SELECT id FROM owners WHERE name = :name AND surname = :surname");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.exec();
    query.next();
    int ownerId = query.value("id").toInt();
    query.prepare("SELECT id FROM violation_types WHERE violation_type = :violation_type");
    query.bindValue(":violation_type", violationType);
    query.exec();
    query.next();
    int violationId = query.value("id").toInt();
    query.prepare("UPDATE violations SET status = :status WHERE id_owner = :id_owner AND id_violation = :id_violation");
    query.bindValue(":status", "Оплачен");
    query.bindValue(":id_owner", ownerId);
    query.bindValue(":id_violation", violationId);
    query.exec();
}

