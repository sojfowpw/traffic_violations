#include "admin.h"
#include "ui_admin.h"
#include "mainwindow.h"
#include "allviolations.h"
#include "changestatusadmin.h"
#include "drivers.h"
#include "infodrivers.h"

admin::admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin)
{
    ui->setupUi(this);
    setWindowTitle("Администратор");
}

admin::~admin()
{
    delete ui;
}

void admin::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void admin::on_infoViolations_clicked()
{
    QList<QVariantList> violationsList;
    QSqlQuery query;
    query.prepare("SELECT violation_type, monetary_fine FROM violation_types");
    query.exec();
    while (query.next()) {
        QString violation_type = query.value("violation_type").toString();
        int monetary_fine = query.value("monetary_fine").toInt();
        QVariantList violationData;
        violationData << violation_type << monetary_fine;
        violationsList.append(violationData);
    }

    hide();
    allViolations *allviol = new allViolations(this);
    allviol->show();
    connect(this, &admin::hideButtonInAllViolations, allviol, &allViolations::hideButton);
    emit hideButtonInAllViolations();
    QObject::connect(this, &admin::sendTableInfo, allviol, &allViolations::setInfo);
    int userId = 1;
    emit sendTableInfo(userId, violationsList);
}


void admin::on_changeStatus_clicked()
{
    QList<QVariantList> violationsList;
    QSqlQuery query;
    query.prepare("SELECT id_violation, id_owner, status FROM violations WHERE status = :status");
    query.bindValue(":status", "В обработке");
    query.exec();
    while (query.next()) {
        int id_violation = query.value("id_violation").toInt();
        QSqlQuery queryInfo;
        queryInfo.prepare("SELECT violation_type FROM violation_types WHERE id = :id");
        queryInfo.bindValue(":id", id_violation);
        queryInfo.exec();
        queryInfo.next();
        QString violation_type = queryInfo.value("violation_type").toString(); // получаем тип нарушения

        int id_owner = query.value("id_owner").toInt();
        queryInfo.prepare("SELECT name, surname FROM owners WHERE id = :id");
        queryInfo.bindValue(":id", id_owner);
        queryInfo.exec();
        queryInfo.next();
        QString name = queryInfo.value("name").toString(); // имя
        QString surname = queryInfo.value("surname").toString(); // фамилия
        QString status = query.value("status").toString();

        QVariantList violationData;
        violationData << name << surname << violation_type << status;
        violationsList.append(violationData);
    }

    hide();
    changeStatusAdmin *newStatus = new changeStatusAdmin(this);
    newStatus->show();
    QObject::connect(this, &admin::sendTableInfoViol, newStatus, &changeStatusAdmin::setInfo);
    emit sendTableInfoViol(violationsList);
}


void admin::on_infoUsers_clicked()
{
    QList<QVariantList> violationsList;
    QSqlQuery query;
    query.prepare("SELECT id, name, surname FROM owners WHERE phone != :phone");
    query.bindValue(":phone", "+7 (913) 012-34-56");
    query.exec();
    while (query.next()) {
        QString name = query.value("name").toString();
        QString surname = query.value("surname").toString();
        int userId = query.value("id").toInt();

        QSqlQuery queryInfo;
        queryInfo.prepare("SELECT id, license_plate FROM vehicles WHERE id_owner = :id_owner");
        queryInfo.bindValue(":id_owner", userId);
        queryInfo.exec();
        while (queryInfo.next()) {
            QString license_plate = queryInfo.value("license_plate").toString();
            QVariantList violationData;
            violationData << name << surname << license_plate;
            violationsList.append(violationData);
        }
    }

    hide();
    drivers *driver = new drivers(this);
    driver->show();
    QObject::connect(this, &admin::sendDrivers, driver, &drivers::setInfo);
    emit sendDrivers(violationsList);
}


void admin::on_infoDrivers_clicked()
{
    QList<QVariantList> violationsList;
    QSqlQuery query;
    query.prepare("SELECT id, name, surname FROM owners WHERE phone != :phone");
    query.bindValue(":phone", "+7 (913) 012-34-56");
    query.exec();
    while (query.next()) {
        QString name = query.value("name").toString();
        QString surname = query.value("surname").toString();
        int userId = query.value("id").toInt();

        QSqlQuery queryInfo;
        queryInfo.prepare("SELECT id_violation, status FROM violations WHERE id_owner = :id_owner");
        queryInfo.bindValue(":id_owner", userId);
        queryInfo.exec();
        while (queryInfo.next()) {
            QString status = queryInfo.value("status").toString();
            int id_violation = queryInfo.value("id_violation").toInt();
            QSqlQuery queryViol;
            queryViol.prepare("SELECT violation_type FROM violation_types WHERE id = :id");
            queryViol.bindValue(":id", id_violation);
            queryViol.exec();
            queryViol.next();
            QString violation = queryViol.value("violation_type").toString();
            QVariantList violationData;
            violationData << name << surname << violation << status;
            violationsList.append(violationData);
        }
    }


    hide();
    infoDrivers *info = new infoDrivers(this);
    info->show();
    QObject::connect(this, &admin::sendDriversInfo, info, &infoDrivers::setInfo);
    emit sendDriversInfo(violationsList);
}

