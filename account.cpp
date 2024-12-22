#include "account.h"
#include "ui_account.h"
#include "mainwindow.h"

account::account(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::account)
{
    ui->setupUi(this);
    setWindowTitle("Профиль");
    ui->newTS->setVisible(false); // скрываем кнопки для изменения информации
    ui->newTSLine->setVisible(false);
    ui->changeTS_2->setVisible(false);
    ui->newPhone->setVisible(false);
    ui->newPhoneLine->setVisible(false);
    ui->changePhone_2->setVisible(false);

    ui->oldPassLabel->setVisible(false);
    ui->oldPassLine->setVisible(false);
    ui->newPassLabel->setVisible(false);
    ui->newPassLine->setVisible(false);
    ui->changePass_2->setVisible(false);

    newTSLineEdit = ui->newTSLine;
    newPhoneLineEdit = ui->newPhoneLine;
    oldPassLineEdit = ui->oldPassLine;
    newPassLineEdit = ui->newPassLine;
}

account::~account()
{
    delete ui;
}

void account::setInfo(const QString &name, const QString &surname, const QString &transport, int violations, const QString &phone) {
    ui->nameLabel->setText(name); // информация о пользователе
    ui->nameLabel->setStyleSheet("font-family: 'Cantarell Regular'; font-weight: bold; color: rgb(246, 97, 81); font-size: 26pt;");
    ui->surnameLabel->setText(surname);
    ui->surnameLabel->setStyleSheet("font-family: 'Cantarell Regular'; font-weight: bold; color: rgb(246, 97, 81); font-size: 26pt;");
    ui->transportLabel->setText(transport);
    ui->transportLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->violationsLabel->setText(QString::number(violations));
    ui->violationsLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->phoneLabel->setText(phone);
    ui->phoneLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");

    currentTransport = transport; // сохраняем текущие данные
    currentPhone = phone;
}

void account::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void account::on_changeTS_clicked() // появление кнопки изменить
{
    ui->newTS->setVisible(!ui->newTS->isVisible());
    ui->newTSLine->setVisible(!ui->newTSLine->isVisible());
    ui->changeTS_2->setVisible(!ui->changeTS_2->isVisible());
}


void account::on_changeTS_2_clicked() // изменяем ТС
{
    QString newTransport = newTSLineEdit->text();
    if(newTransport.isEmpty()) { // проверка ввода
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер ТС\" не может быть пустым</FONT>");
        return;
    }
    if (!newTransport.contains(QRegularExpression("^[АВЕКМНОРСТУХ]\\d{3}[АВЕКМНОРСТУХ]{2}\\d{2,3}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер ТС\" заполнено некорректно</FONT>");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT license_plate FROM vehicles WHERE license_plate = :license_plate");
    query.bindValue(":license_plate", newTransport);
    query.exec();
    if (query.next()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Такой номер ТС уже зарегистрирован</FONT>");
        return;
    }

    query.prepare("UPDATE vehicles SET license_plate = :newlicense WHERE license_plate = :transport");
    query.bindValue(":transport", currentTransport); // обновление данных
    query.bindValue(":newlicense", newTransport);
    if (query.exec()) {
        currentTransport = newTransport; // обновление интерфейса
        ui->transportLabel->setText(currentTransport);
        ui->transportLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
        ui->newTS->setVisible(false);
        ui->newTSLine->setVisible(false);
        ui->changeTS_2->setVisible(false);
    }
}


void account::on_changePhone_clicked() // появление кнопки изменить
{
    ui->newPhone->setVisible(!ui->newPhone->isVisible());
    ui->newPhoneLine->setVisible(!ui->newPhoneLine->isVisible());
    ui->changePhone_2->setVisible(!ui->changePhone_2->isVisible());
}


void account::on_changePhone_2_clicked() // изменяем номер телефона
{
    QString newPhone = newPhoneLineEdit->text();
    if(newPhone.isEmpty()) { // проверка ввода
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер телефона\" не может быть пустым</FONT>");
        return;
    }
    if (!newPhone.contains(QRegularExpression("^\\+7\\d{10}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер телефона\" заполнено некорректно</FONT>");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT phone FROM owners WHERE phone = :phone");
    query.bindValue(":phone", newPhone);
    query.exec();
    if (query.next()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Такой номер телефона уже зарегистрирован</FONT>");
        return;
    }

    query.prepare("UPDATE owners SET phone = :newphone WHERE phone = :phone");
    query.bindValue(":phone", currentPhone);
    query.bindValue(":newphone", newPhone);
    if (query.exec()) {
        currentPhone = newPhone; // обновление интерфейса
        ui->phoneLabel->setText(currentPhone);
        ui->phoneLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
        ui->newPhone->setVisible(false);
        ui->newPhoneLine->setVisible(false);
        ui->changePhone_2->setVisible(false);
    }
}


void account::on_changePass_clicked()
{
    ui->changeTS->setVisible(false);
    ui->changePhone->setVisible(false);
    ui->oldPassLabel->setVisible(!ui->oldPassLabel->isVisible());
    ui->oldPassLine->setVisible(!ui->oldPassLine->isVisible());
    ui->newPassLabel->setVisible(!ui->newPassLabel->isVisible());
    ui->newPassLine->setVisible(!ui->newPassLine->isVisible());
    ui->changePass_2->setVisible(!ui->changePass_2->isVisible());

    QString oldPass = oldPassLineEdit->text();
    QString newPass = newPassLineEdit->text();

}

