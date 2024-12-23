#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "account.h"

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle("Вход");

    ui->passLine->setEchoMode(QLineEdit::Password);

    phoneLineEdit = ui->phoneLine; // переменные для названия полей для ввода
    passLineEdit = ui->passLine;
}

login::~login()
{
    delete ui;
}

void login::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void login::on_continueReg_clicked()
{
    logUser();
}

void login::logUser() {
    QString phone = phoneLineEdit->text(); // строковые переменные - имена полей
    QString pass = passLineEdit->text();

    // проверка телефона
    if(phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер телефона\" не может быть пустым</FONT>");
        return;
    }
    if (!phone.contains(QRegularExpression("^\\+7\\d{10}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер телефона\" заполнено некорректно</FONT>");
        return;
    }
    // проверка пароля
    if(pass.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Пароль\" не может быть пустым</FONT>");
        return;
    }
    // if (pass.contains(QRegularExpression("['\"\\-\\-;\\*\\/]"))) {
    //     QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Пароль\" не должно содержать запрещённые символы: ' \" - ; / *</FONT>");
    //     return;
    // }

    QSqlQuery query;
    query.prepare("SELECT phone FROM owners WHERE phone = :phone"); // проверка на существование номера телефона
    query.bindValue(":phone", phone);
    query.exec();
    if (!query.next()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Такой пользователь не зарегистрирован</FONT>");
        return;
    }

    QByteArray hashedPassword = QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha256); // хэширование пароля
    query.prepare("SELECT id, pass FROM owners WHERE phone = :phone");
    query.bindValue(":phone", phone);
    query.exec();
    query.next();
    int idUser = query.value("id").toInt(); // получаем айди пользователя
    QString storedPasswordHash = query.value("pass").toString(); // получаем пароль пользователя
    QByteArray storedHashBytes = QByteArray::fromHex(storedPasswordHash.toUtf8());
    if (storedHashBytes != hashedPassword.toHex()) { // сравниваем пароли
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Неверный пароль</FONT>");
        return;
    }

    query.prepare("SELECT name, surname, amount_fines, id_vehicle FROM owners WHERE id = :id");
    query.bindValue(":id", idUser); // получаем данные пользователя по айди
    query.exec();
    query.next();
    QString name = query.value("name").toString();
    QString surname = query.value("surname").toString();
    int amount_fines = query.value("amount_fines").toInt();
    int id_vehicle = query.value("id_vehicle").toInt();
    query.prepare("SELECT license_plate FROM vehicles WHERE id = :id"); // получаем номер ТС
    query.bindValue(":id", id_vehicle);
    query.exec();
    query.next();
    QString transport = query.value("license_plate").toString();

    hide();
    account *acc = new account(this); // переходим в личный аккаунт
    acc->show();
    QObject::connect(this, &login::sendUserInfo, acc, &account::setInfo); // связываем классы для передачи информации
    emit sendUserInfo(name, surname, transport, amount_fines, phone);
}

