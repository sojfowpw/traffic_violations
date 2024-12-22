#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QDate>

namespace Ui {
class registration;
}

class registration : public QDialog
{
    Q_OBJECT

public:
    explicit registration(QWidget *parent = nullptr);
    ~registration();

private slots:
    void on_returnToMain_clicked(); // вернуться в главное окно

    void on_infoTransport_clicked(); // справка о номере ТС

    void on_continueReg_clicked(); // регистрация пользователя

private:
    Ui::registration *ui;
    QLineEdit *nameLineEdit; // переменные для названия полей для ввода
    QLineEdit *surnameLineEdit;
    QLineEdit *phoneLineEdit;
    QLineEdit *transportLineEdit;
    QLineEdit *pass1LineEdit;
    QLineEdit *pass2LineEdit;

    void registrUser(); // функционал регистрации
};

#endif // REGISTRATION_H
