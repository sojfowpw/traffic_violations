#include "admin.h"
#include "ui_admin.h"
#include "mainwindow.h"
#include "allviolations.h"
#include "changestatusadmin.h"

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
    hide();
    changeStatusAdmin *newStatus = new changeStatusAdmin(this);
    newStatus->show();
}

