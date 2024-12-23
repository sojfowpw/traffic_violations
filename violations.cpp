#include "violations.h"
#include "ui_violations.h"
#include "mainwindow.h"
#include "account.h"
#include "allviolations.h"

violations::violations(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::violations)
{
    ui->setupUi(this);
    setWindowTitle("Штрафы");

    ui->groupBox_2->setVisible(false);
}

violations::~violations()
{
    delete ui;
}


void violations::setInfo(int idUser, const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status,
                         int fine_amount, int id2) {
    userId = idUser;
    violationId2 = id2;
    if (status == "В обработке" || status == "Оплачен") {
        ui->changeStatus1->setVisible(false);
    }

    ui->violationLabel->setText(violation_type);
    ui->violationLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->dateLabel->setText(violation_date);
    ui->dateLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->locationLabel->setText(violation_loc);
    ui->locationLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->statusLabel->setText(status);
    ui->statusLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->amountLabel->setText(QString::number(fine_amount));
    ui->amountLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
}

void violations::setInfo2(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status,
                          int fine_amount, int id1) {
    if (violation_type.isEmpty()) {
        return;
    }
    violationId1 = id1;
    if (status == "В обработке" || status == "Оплачен") {
        ui->changeStatus2->setVisible(false);
    }

    ui->groupBox_2->setVisible(!ui->groupBox_2->isVisible());
    ui->violationLabel_2->setText(violation_type);
    ui->violationLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->dateLabel_2->setText(violation_date);
    ui->dateLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->locationLabel_2->setText(violation_loc);
    ui->locationLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->statusLabel_2->setText(status);
    ui->statusLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->amountLabel_2->setText(QString::number(fine_amount));
    ui->amountLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
}


void violations::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void violations::on_back_clicked() // вернуться к профилю
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
    QObject::connect(this, &violations::sendUserInfo, acc, &account::setInfo); // связываем классы для передачи информации
    emit sendUserInfo(name, surname, transport, amount_fines, phone);
}


void violations::on_infoViolation_clicked()
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
    allViolations *viol = new allViolations(this);
    viol->show();
    QObject::connect(this, &violations::sendTableInfo, viol, &allViolations::setInfo);
    emit sendTableInfo(userId, violationsList);
    connect(this, &violations::hideButtonInAllViolations, viol, &allViolations::hideButton2);
    emit hideButtonInAllViolations();
}


void violations::on_changeStatus1_clicked()
{
    QSqlQuery query;
    query.prepare("UPDATE violations SET status = :status WHERE id = :id");
    query.bindValue(":status", "В обработке");
    query.bindValue(":id", violationId2);
    if (query.exec()) {
        ui->statusLabel->setText("В обработке");
        ui->statusLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
        ui->changeStatus1->setVisible(false);
    }
}


void violations::on_changeStatus2_clicked()
{
    QSqlQuery query;
    query.prepare("UPDATE violations SET status = :status WHERE id = :id");
    query.bindValue(":status", "В обработке");
    query.bindValue(":id", violationId1);
    if (query.exec()) {
        ui->statusLabel_2->setText("В обработке");
        ui->statusLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
        ui->changeStatus2->setVisible(false);
    }
}

