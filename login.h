#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_returnToMain_clicked();

    void on_continueReg_clicked();

signals:
    void sendUserInfo(const QString &name, const QString &surname, const QString &transport, int violations, const QString &phone);

private:
    Ui::login *ui;
    QLineEdit *phoneLineEdit; // переменные для названия полей для ввода
    QLineEdit *passLineEdit;

    void logUser();
};

#endif // LOGIN_H
