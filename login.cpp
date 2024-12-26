#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "account.h"
#include "admin.h"

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle("Вход");

    ui->passLine->setEchoMode(QLineEdit::Password);

    phoneLineEdit = ui->phoneLine; // переменные для названия полей для ввода
    passLineEdit = ui->passLine;

    phoneLineEdit->setInputMask("+7 (000) 000-00-00");
    phoneLineEdit->setPlaceholderText("+7 (___) ___-__-__");
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
    if (!phone.contains(QRegularExpression("^\\+7 \\([0-9]{3}\\) [0-9]{3}-[0-9]{2}-[0-9]{2}$"))) {
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
    if (phone == "+7 (913) 012-34-56") {
        hide();
        admin *adminWimdow = new admin(this);
        adminWimdow->show();
        return;
    }

    query.prepare("SELECT name, surname, amount_fines FROM owners WHERE id = :id");
    query.bindValue(":id", idUser); // получаем данные пользователя по айди
    query.exec();
    query.next();
    QString name = query.value("name").toString();
    QString surname = query.value("surname").toString();
    int amount_fines = query.value("amount_fines").toInt();

    QList<QVariantList> licenseList;
    query.prepare("SELECT license_plate FROM vehicles WHERE id_owner = :id_owner"); // получаем номер ТС
    query.bindValue(":id_owner", idUser);
    query.exec();
    while (query.next()) {
        QString transport = query.value("license_plate").toString();
        QVariantList licenseData;
        licenseData << transport;
        licenseList.append(licenseData);
    }

    hide();
    account *acc = new account(this); // переходим в личный аккаунт
    acc->show();
    QObject::connect(this, &login::sendUserInfo, acc, &account::setInfo); // связываем классы для передачи информации
    emit sendUserInfo(idUser, name, surname, licenseList, amount_fines, phone);
}

