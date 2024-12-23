#include "account.h"
#include "ui_account.h"
#include "mainwindow.h"
#include "violations.h"

account::account(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::account)
{
    ui->setupUi(this);
    setWindowTitle("Профиль");

    ui->oldPassLine->setEchoMode(QLineEdit::Password); // отображение пароля звездочками
    ui->newPassLine->setEchoMode(QLineEdit::Password);

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
    ui->back->setVisible(false);

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


void account::on_changePass_clicked() // кнопка для вывода интерфейса смены пароля
{
    ui->newTS->setVisible(false);
    ui->newTSLine->setVisible(false);
    ui->changeTS_2->setVisible(false);
    ui->newPhone->setVisible(false);
    ui->newPhoneLine->setVisible(false);
    ui->changePhone_2->setVisible(false);
    ui->goViolations->setVisible(false);

    ui->changeTS->setVisible(false);
    ui->changePhone->setVisible(false);
    ui->oldPassLabel->setVisible(!ui->oldPassLabel->isVisible());
    ui->oldPassLine->setVisible(!ui->oldPassLine->isVisible());
    ui->newPassLabel->setVisible(!ui->newPassLabel->isVisible());
    ui->newPassLine->setVisible(!ui->newPassLine->isVisible());
    ui->changePass_2->setVisible(!ui->changePass_2->isVisible());
    ui->back->setVisible(!ui->back->isVisible());
}


void account::on_back_clicked() // кнопка отменить смену пароля
{
    ui->oldPassLabel->setVisible(false);
    ui->oldPassLine->setVisible(false);
    ui->newPassLabel->setVisible(false);
    ui->newPassLine->setVisible(false);
    ui->changePass_2->setVisible(false);
    ui->back->setVisible(false);
    ui->changeTS->setVisible(!ui->changeTS->isVisible());
    ui->changePhone->setVisible(!ui->changePhone->isVisible());
    ui->goViolations->setVisible(!ui->goViolations->isVisible());
}


void account::on_changePass_2_clicked() // сама смена пароля
{
    QString oldPass = oldPassLineEdit->text();
    QString newPass = newPassLineEdit->text();

    if(oldPass.isEmpty()) { // проверка на правильность ввода
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Старый пароль\" не может быть пустым</FONT>");
        return;
    }
    if (oldPass.contains(QRegularExpression("['\"\\-\\-;\\*\\/]"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Старый пароль\" не должно содержать запрещённые символы: ' \" - ; / *</FONT>");
        return;
    }
    if(newPass.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Новый пароль\" не может быть пустым</FONT>");
        return;
    }
    if (newPass.contains(QRegularExpression("['\"\\-\\-;\\*\\/]"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Новый пароль\" не должно содержать запрещённые символы: ' \" - ; / *</FONT>");
        return;
    }

    QByteArray hashedPassword = QCryptographicHash::hash(oldPass.toUtf8(), QCryptographicHash::Sha256); // хэширование старого пароля
    QSqlQuery query;
    query.prepare("SELECT pass FROM owners WHERE phone = :phone");
    query.bindValue(":phone", currentPhone);
    query.exec();
    query.next();
    QString storedPasswordHash = query.value("pass").toString(); // получаем пароль пользователя
    QByteArray storedHashBytes = QByteArray::fromHex(storedPasswordHash.toUtf8());
    if (storedHashBytes != hashedPassword.toHex()) { // сравниваем пароли
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Старый пароль введён неверно</FONT>");
        return;
    }

    hashedPassword = QCryptographicHash::hash(newPass.toUtf8(), QCryptographicHash::Sha256); // хэшируем новый пароль для записи
    query.prepare("UPDATE owners SET pass = :newPass WHERE phone = :phone");
    query.bindValue(":newPass", hashedPassword.toHex());
    query.bindValue(":phone", currentPhone);
    query.exec();
}


void account::on_goViolations_clicked() // информация о нарушениях
{
    QSqlQuery query; // вытаскиваем айди пользователя
    query.prepare("SELECT id FROM owners WHERE phone = :phone");
    query.bindValue(":phone", currentPhone);
    query.exec();
    query.next();
    int id_owner = query.value("id").toInt();
    query.prepare("SELECT id_violation, violation_date, location, status, fine_amount FROM violations WHERE id_owner = :id_owner");
    query.bindValue(":id_owner", id_owner); // вытаскиваем информацию о нарушениях
    query.exec();
    query.next();
    int id_violation = query.value("id_violation").toInt();
    QString violation_date = query.value("violation_date").toString();
    QString location = query.value("location").toString();
    QString status = query.value("status").toString();
    int fine_amount = query.value("fine_amount").toInt();
    QSqlQuery queryType;
    queryType.prepare("SELECT violation_type FROM violation_types WHERE id = :id"); // вытаскиваем описание нарушения
    queryType.bindValue(":id", id_violation);
    queryType.exec();
    queryType.next();
    QString violation_type = queryType.value("violation_type").toString();


    QString violation_date2; // вытаскиваем второе нарушение
    QString location2;
    QString status2;
    int fine_amount2;
    QString violation_type2;

    if (query.next()) {
        int id_violation2 = query.value("id_violation").toInt();
        violation_date2 = query.value("violation_date").toString();
        location2 = query.value("location").toString();
        status2 = query.value("status").toString();
        fine_amount2 = query.value("fine_amount").toInt();
        queryType.prepare("SELECT violation_type FROM violation_types WHERE id = :id"); // вытаскиваем описание нарушения
        queryType.bindValue(":id", id_violation2);
        queryType.exec();
        queryType.next();
        violation_type2 = queryType.value("violation_type").toString();
    }

    hide();
    violations *infoViolation = new violations(this);
    infoViolation->show();
    QObject::connect(this, &account::sendUserInfo, infoViolation, &violations::setInfo); // связываем классы для передачи информации
    emit sendUserInfo(id_owner, violation_type, violation_date, location, status, fine_amount);
    QObject::connect(this, &account::sendUserInfo2, infoViolation, &violations::setInfo2); // связываем классы для передачи информации
    emit sendUserInfo2(violation_type2, violation_date2, location2, status2, fine_amount2);
}

