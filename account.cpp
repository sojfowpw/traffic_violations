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
    ui->oldTS->setVisible(false);
    ui->oldTSLine->setVisible(false);
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

    ui->addTSLabel->setVisible(false);
    ui->addTSLine->setVisible(false);
    ui->addTS_2->setVisible(false);
    ui->returnAdd->setVisible(false);

    newTSLineEdit = ui->newTSLine;
    oldTSLineEdit = ui->oldTSLine;
    newPhoneLineEdit = ui->newPhoneLine;
    oldPassLineEdit = ui->oldPassLine;
    newPassLineEdit = ui->newPassLine;
    addTSLineEdit = ui->addTSLine;
    newPhoneLineEdit->setInputMask("+7 (000) 000-00-00");
    newPhoneLineEdit->setPlaceholderText("+7 (___) ___-__-__");
}

account::~account()
{
    delete ui;
}

void account::setInfo(int idUser, const QString &name, const QString &surname, const QList<QVariantList> &licenseList, int violations, const QString &phone) {
    ui->nameLabel->setText(name); // информация о пользователе
    ui->nameLabel->setStyleSheet("font-family: 'Cantarell Regular'; font-weight: bold; color: rgb(246, 97, 81); font-size: 26pt;");
    ui->surnameLabel->setText(surname);
    ui->surnameLabel->setStyleSheet("font-family: 'Cantarell Regular'; font-weight: bold; color: rgb(246, 97, 81); font-size: 26pt;");

    QStringList licenses;
    for (const QVariantList &license : licenseList) {
        if (!license.isEmpty()) {
            licenses.append(license[0].toString());
        }
    }
    QString transport = licenses.join(", ");

    ui->transportLabel->setText(transport);
    ui->transportLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->violationsLabel->setText(QString::number(violations));
    ui->violationsLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->phoneLabel->setText(phone);
    ui->phoneLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");

    currentTransport = transport; // сохраняем текущие данные
    currentPhone = phone;
    userId = idUser;
}

void account::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void account::on_changeTS_clicked() // появление кнопки изменить
{
    ui->newPhone->setVisible(false);
    ui->newPhoneLine->setVisible(false);
    ui->changePhone_2->setVisible(false);
    ui->newTS->setVisible(!ui->newTS->isVisible());
    ui->newTSLine->setVisible(!ui->newTSLine->isVisible());
    ui->oldTS->setVisible(!ui->oldTS->isVisible());
    ui->oldTSLine->setVisible(!ui->oldTSLine->isVisible());
    ui->changeTS_2->setVisible(!ui->changeTS_2->isVisible());
}


void account::on_changeTS_2_clicked() // изменяем ТС
{
    QString newTransport = newTSLineEdit->text();
    QString oldTransport = oldTSLineEdit->text();
    if(oldTransport.isEmpty()) { // проверка ввода
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Старый номер\" не может быть пустым</FONT>");
        return;
    }
    if (!oldTransport.contains(QRegularExpression("^[АВЕКМНОРСТУХ]\\d{3}[АВЕКМНОРСТУХ]{2}\\d{2,3}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Старый номер\" заполнено некорректно</FONT>");
        return;
    }
    if(newTransport.isEmpty()) { // проверка ввода
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Новый номер\" не может быть пустым</FONT>");
        return;
    }
    if (!newTransport.contains(QRegularExpression("^[АВЕКМНОРСТУХ]\\d{3}[АВЕКМНОРСТУХ]{2}\\d{2,3}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Новый номер\" заполнено некорректно</FONT>");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT license_plate FROM vehicles WHERE license_plate = :license_plate AND id_owner = :id_owner");
    query.bindValue(":license_plate", oldTransport);
    query.bindValue(":id_owner", userId);
    query.exec();
    if (!query.next()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Такой номер ТС не зарегистрирован</FONT>");
        return;
    }

    query.prepare("SELECT license_plate FROM vehicles WHERE license_plate = :license_plate");
    query.bindValue(":license_plate", newTransport);
    query.exec();
    if (query.next()) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Такой номер ТС уже зарегистрирован</FONT>");
        return;
    }

    query.prepare("UPDATE vehicles SET license_plate = :newlicense WHERE license_plate = :transport AND id_owner = :id_owner");
    query.bindValue(":transport", currentTransport); // обновление данных
    query.bindValue(":newlicense", newTransport);
    query.bindValue(":id_owner", userId);
    if (query.exec()) {
        currentTransport = newTransport; // обновление интерфейса
        ui->transportLabel->setText(currentTransport);
        ui->transportLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
        ui->newTS->setVisible(false);
        ui->newTSLine->setVisible(false);
        ui->oldTS->setVisible(false);
        ui->oldTSLine->setVisible(false);
        ui->changeTS_2->setVisible(false);
        qDebug() << "Update выполнен.";
    }
}


void account::on_changePhone_clicked() // появление кнопки изменить
{
    ui->newTS->setVisible(false);
    ui->newTSLine->setVisible(false);
    ui->changeTS_2->setVisible(false);
    ui->oldTS->setVisible(false);
    ui->oldTSLine->setVisible(false);
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
    if (!newPhone.contains(QRegularExpression("^\\+7 \\([0-9]{3}\\) [0-9]{3}-[0-9]{2}-[0-9]{2}$"))) {
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

    query.prepare("UPDATE owners SET phone = :newphone WHERE phone = :phone AND id = :id_owner");
    query.bindValue(":phone", currentPhone);
    query.bindValue(":newphone", newPhone);
    query.bindValue(":id_owner", userId);
    if (query.exec()) {
        currentPhone = newPhone; // обновление интерфейса
        ui->phoneLabel->setText(currentPhone);
        ui->phoneLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
        ui->newPhone->setVisible(false);
        ui->newPhoneLine->setVisible(false);
        ui->changePhone_2->setVisible(false);
        qDebug() << "Update выполнен.";
    }
}


void account::on_changePass_clicked() // кнопка для вывода интерфейса смены пароля
{
    ui->newTS->setVisible(false);
    ui->newTSLine->setVisible(false);
    ui->oldTS->setVisible(false);
    ui->oldTSLine->setVisible(false);
    ui->changeTS_2->setVisible(false);
    ui->newPhone->setVisible(false);
    ui->newPhoneLine->setVisible(false);
    ui->changePhone_2->setVisible(false);
    ui->goViolations->setVisible(false);
    ui->addTSLabel->setVisible(false);
    ui->addTSLine->setVisible(false);
    ui->addTS_2->setVisible(false);
    ui->addTS->setVisible(false);
    ui->changeTS->setVisible(false);
    ui->changePhone->setVisible(false);
    ui->returnAdd->setVisible(false);
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
    ui->addTS->setVisible(!ui->addTS->isVisible());
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
    qDebug() << "Update выполнен.";
}


void account::on_goViolations_clicked() // информация о нарушениях
{
    QSqlQuery query; // вытаскиваем айди пользователя
    query.prepare("SELECT id FROM owners WHERE phone = :phone");
    query.bindValue(":phone", currentPhone);
    query.exec();
    query.next();
    int id_owner = query.value("id").toInt();
    query.prepare("SELECT id, id_violation, violation_date, location, status, fine_amount FROM violations WHERE id_owner = :id_owner");
    query.bindValue(":id_owner", id_owner); // вытаскиваем информацию о нарушениях
    query.exec();
    query.next();
    int id1 = query.value("id").toInt();
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
    int id2;

    if (query.next()) {
        id2 = query.value("id").toInt();
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
    emit sendUserInfo(id_owner, violation_type, violation_date, location, status, fine_amount, id1);
    QObject::connect(this, &account::sendUserInfo2, infoViolation, &violations::setInfo2); // связываем классы для передачи информации
    emit sendUserInfo2(violation_type2, violation_date2, location2, status2, fine_amount2, id2);
}


void account::on_addTS_clicked() // добавить тс
{
    ui->newTS->setVisible(false);
    ui->newTSLine->setVisible(false);
    ui->oldTS->setVisible(false);
    ui->oldTSLine->setVisible(false);
    ui->changeTS_2->setVisible(false);
    ui->newPhone->setVisible(false);
    ui->newPhoneLine->setVisible(false);
    ui->changePhone_2->setVisible(false);
    ui->goViolations->setVisible(false);
    ui->changeTS->setVisible(false);
    ui->changePhone->setVisible(false);

    ui->addTSLabel->setVisible(!ui->addTSLabel->isVisible());
    ui->addTSLine->setVisible(!ui->addTSLine->isVisible());
    ui->addTS_2->setVisible(!ui->addTS_2->isVisible());
    ui->returnAdd->setVisible(!ui->returnAdd->isVisible());
}


void account::on_returnAdd_clicked()
{
    ui->addTSLabel->setVisible(false);
    ui->addTSLine->setVisible(false);
    ui->addTS_2->setVisible(false);
    ui->returnAdd->setVisible(false);
    ui->changeTS->setVisible(!ui->changeTS->isVisible());
    ui->changePhone->setVisible(!ui->changePhone->isVisible());
    ui->goViolations->setVisible(!ui->goViolations->isVisible());
}


void account::on_addTS_2_clicked()
{
    QString newTransport = addTSLineEdit->text();
    if(newTransport.isEmpty()) { // проверка ввода
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Новое ТС\" не может быть пустым</FONT>");
        return;
    }
    if (!newTransport.contains(QRegularExpression("^[АВЕКМНОРСТУХ]\\d{3}[АВЕКМНОРСТУХ]{2}\\d{2,3}$"))) {
        QMessageBox::warning(this, "Ошибка", "<FONT COLOR='#000000'>Поле \"Новое ТС\" заполнено некорректно</FONT>");
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

    query.prepare("INSERT INTO vehicles (license_plate, id_owner) VALUES (:license_plate, :id_owner)");
    query.bindValue(":license_plate", newTransport);
    query.bindValue(":id_owner", userId);
    if (query.exec()) {
        qDebug() << "Insert выполнен.";
    }
    QVariant lastInsertId = query.lastInsertId();

    QList<QVariantList> licenseList;
    query.prepare("SELECT license_plate FROM vehicles WHERE id_owner = :id_owner"); // получаем номер ТС
    query.bindValue(":id_owner", userId);
    query.exec();
    while (query.next()) {
        QString transport = query.value("license_plate").toString();
        QVariantList licenseData;
        licenseData << transport;
        licenseList.append(licenseData);
    }

    QStringList licenses;
    for (const QVariantList &license : licenseList) {
        if (!license.isEmpty()) {
            licenses.append(license[0].toString());
        }
    }
    QString transport = licenses.join(", ");
    ui->transportLabel->setText(transport);
    ui->transportLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");

    query.prepare("SELECT amount_fines FROM owners WHERE id = :id");
    query.bindValue(":id", userId);
    query.exec();
    query.next();
    int currentAmount = query.value("amount_fines").toInt();

    if (currentAmount < 2) {
    QDate startDay(2024, 11, 1); // диапазон дат
    QDate endDay(2024, 12, 22);
    int daysRange = startDay.daysTo(endDay); // количество дней в диапазоне
    query.prepare("INSERT INTO violations (id_violation, violation_date, location, id_owner, id_camera, id_vehicle, status, fine_amount) VALUES"
                  "(:id_violation, :violation_date, :location, :id_owner, :id_camera, :id_vehicle, :status, :fine_amount)");
    int violation = QRandomGenerator::global()->bounded(1, 11);
    query.bindValue(":id_violation", violation); // получение типа нарушения и штрафа за него
    QSqlQuery queryViolation;
    queryViolation.prepare("SELECT monetary_fine FROM violation_types WHERE id = :id");
    queryViolation.bindValue(":id", violation);
    queryViolation.exec();
    qDebug() << "Insert выполнен.";
    queryViolation.next();
    int fine_amount = queryViolation.value("monetary_fine").toInt();
    query.bindValue(":fine_amount", fine_amount);

    int randomDays = QRandomGenerator::global()->bounded(daysRange + 1); // количество дней
    QDate randomDate = startDay.addDays(randomDays); // рандомная дата
    QString violation_date = randomDate.toString("dd.MM.yyyy"); // перевод в строку
    query.bindValue(":violation_date", violation_date);
    query.bindValue(":id_owner", userId);

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
    query.bindValue(":id_vehicle", lastInsertId);
    query.exec();

    query.prepare("UPDATE owners SET amount_fines = :amount WHERE id = :id");
    query.bindValue(":amount", currentAmount + 1);
    query.bindValue(":id", userId);
    query.exec();
    query.next();
    qDebug() << "Update выполнен.";
    ui->violationsLabel->setText(QString::number(currentAmount + 1));
    ui->violationsLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    }

    ui->addTSLabel->setVisible(false);
    ui->addTSLine->setVisible(false);
    ui->addTS_2->setVisible(false);
    ui->returnAdd->setVisible(false);
    ui->changeTS->setVisible(!ui->changeTS->isVisible());
    ui->changePhone->setVisible(!ui->changePhone->isVisible());
    ui->goViolations->setVisible(!ui->goViolations->isVisible());
}

