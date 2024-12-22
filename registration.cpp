#include "registration.h"
#include "ui_registration.h"
#include "mainwindow.h"
#include "infotransport.h"
#include "login.h"

registration::registration(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registration)
{
    ui->setupUi(this);
    setWindowTitle("Регистрация");

    ui->pass1Line->setEchoMode(QLineEdit::Password); // отображение пароля звездочками
    ui->pass2Line->setEchoMode(QLineEdit::Password);

    nameLineEdit = ui->nameLine; // переменные для названия полей для ввода
    surnameLineEdit = ui->surnameLine;
    phoneLineEdit = ui->phoneLine;
    transportLineEdit = ui->transportLine;
    pass1LineEdit = ui->pass1Line;
    pass2LineEdit = ui->pass2Line;
}

registration::~registration()
{
    delete ui;
}

void registration::on_returnToMain_clicked() // кнопка вернуться в главное окно
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void registration::on_infoTransport_clicked() // справка о номере ТС
{
    infoTransport infoTS;
    infoTS.setModal(true);
    infoTS.exec();
}


void registration::on_continueReg_clicked() // регистрация пользователя
{
    registrUser();
}

void registration::registrUser() { // функционал регистрации
    QString name = nameLineEdit->text(); // строковые переменные - имена полей
    QString surname = surnameLineEdit->text();
    QString phone = phoneLineEdit->text();
    QString transport = transportLineEdit->text();
    QString pass1 = pass1LineEdit->text();
    QString pass2 = pass2LineEdit->text();
    // проверка фамилии
    if(surname.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Фамилия\" не может быть пустым</FONT>");
        return;
    }
    if (!surname.contains(QRegularExpression("^[А-Яа-я]+$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Фамилия\" должно содержать только буквы русского алфавита</FONT>");
        return;
    }
    // проверка имени
    if(name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Имя\" не может быть пустым</FONT>");
        return;
    }
    if (!name.contains(QRegularExpression("^[А-Яа-я]+$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Имя\" должно содержать только буквы русского алфавита</FONT>");
        return;
    }
    // проверка телефона
    if(phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер телефона\" не может быть пустым</FONT>");
        return;
    }
    if (!phone.contains(QRegularExpression("^\\+7\\d{10}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер телефона\" заполнено некорректно</FONT>");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT phone FROM owners WHERE phone = :phone");
    query.bindValue(":phone", phone);
    query.exec();
    if (query.next()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Такой номер телефона уже зарегистрирован</FONT>");
        return;
    }
    // проверка номера ТС
    if(transport.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер ТС\" не может быть пустым</FONT>");
        return;
    }
    if (!transport.contains(QRegularExpression("^[АВЕКМНОРСТУХ]\\d{3}[АВЕКМНОРСТУХ]{2}\\d{2,3}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Номер ТС\" заполнено некорректно</FONT>");
        return;
    }
    query.prepare("SELECT license_plate FROM vehicles WHERE license_plate = :license_plate");
    query.bindValue(":license_plate", transport);
    query.exec();
    if (query.next()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Такой номер ТС уже зарегистрирован</FONT>");
        return;
    }
    // проверка первого пароля
    if(pass1.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Пароль\" не может быть пустым</FONT>");
        return;
    }
    if (pass1.contains(QRegularExpression("['\"\\-\\-;\\*\\/]"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Пароль\" не должно содержать запрещённые символы: ' \" - ; / *</FONT>");
        return;
    }
    // проверка второго пароля
    if(pass2.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Подтверждение пароля\" не может быть пустым</FONT>");
        return;
    }
    if (pass2 != pass1) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Пароли не совпадают</FONT>");
        return;
    }

    // создание запроса на добавление номера ТС
    query.prepare("INSERT INTO vehicles (license_plate) VALUES (:license_plate)");
    query.bindValue(":license_plate", transport);
    query.exec();
    QVariant lastInsertId = query.lastInsertId(); // id транспорта

    // создание запроса на добавление владельца
    query.prepare("INSERT INTO owners (name, surname, phone, amount_fines, pass, id_vehicle) VALUES (:name, :surname, :phone, :amount_fines,"
                  " :pass, :id_vehicle)");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":phone", phone);
    int amount = QRandomGenerator::global()->bounded(1, 3); // рандомное количество штрафов
    query.bindValue(":amount_fines", amount);
    QByteArray hashedPassword = QCryptographicHash::hash(pass1.toUtf8(), QCryptographicHash::Sha256);
    query.bindValue(":pass", hashedPassword.toHex()); // пароль захеширован с помощью SHA-256
    query.bindValue(":id_vehicle", lastInsertId);
    query.exec();
    lastInsertId = query.lastInsertId(); // id владельца

    // создание запроса на добавление самих нарушений
    QDate startDay(2024, 11, 1); // диапазон дат
    QDate endDay(2024, 12, 22);
    int daysRange = startDay.daysTo(endDay); // количество дней в диапазоне
    for (int i = 0; i < amount; i++) {
        query.prepare("INSERT INTO violations (id_violation, violation_date, location, id_owner, id_camera, status, fine_amount) VALUES"
                      "(:id_violation, :violation_date, :location, :id_owner, :id_camera, :status, :fine_amount)");
        int violation = QRandomGenerator::global()->bounded(1, 11);
        query.bindValue(":id_violation", violation); // получение типа нарушения и штрафа за него
        QSqlQuery queryViolation;
        queryViolation.prepare("SELECT monetary_fine FROM violation_types WHERE id = :id");
        queryViolation.bindValue(":id", violation);
        queryViolation.exec();
        queryViolation.next();
        int fine_amount = queryViolation.value("monetary_fine").toInt();
        query.bindValue(":fine_amount", fine_amount);

        int randomDays = QRandomGenerator::global()->bounded(daysRange + 1); // количество дней
        QDate randomDate = startDay.addDays(randomDays); // рандомная дата
        QString violation_date = randomDate.toString("dd.MM.yyyy"); // перевод в строку
        query.bindValue(":violation_date", violation_date);
        query.bindValue(":id_owner", lastInsertId);

        int randomCamera = QRandomGenerator::global()->bounded(1, 11); // случаный номер камеры
        query.bindValue(":id_camera", randomCamera);
        QSqlQuery queryCamera;
        queryCamera.prepare("SELECT location FROM cameras WHERE id = :id");
        queryCamera.bindValue(":id", randomCamera);
        queryCamera.exec();
        queryCamera.next();
        QString location = queryCamera.value("location").toString(); // адрес камеры
        query.bindValue(":location", location);
        query.bindValue(":status", "Не оплачен");
        query.exec();
    }

    //переход к окну входа
    hide();
    login *log = new login(this);
    log->show();
}
