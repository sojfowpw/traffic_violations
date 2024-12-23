#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QDialog>
#include <QLineEdit>
#include <QCryptographicHash>
#include <QDebug>

namespace Ui {
class account;
}

class account : public QDialog
{
    Q_OBJECT

public:
    explicit account(QWidget *parent = nullptr);
    ~account();

public slots:
    void setInfo(const QString &name, const QString &surname, const QString &transport, int violations, const QString &phone);

private slots:
    void on_returnToMain_clicked();

    void on_changeTS_clicked();

    void on_changeTS_2_clicked();

    void on_changePhone_clicked();

    void on_changePhone_2_clicked();

    void on_changePass_clicked();

    void on_back_clicked();

    void on_changePass_2_clicked();

    void on_goViolations_clicked();

signals:
    void sendUserInfo(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status, int fine_amount);

    void sendUserInfo2(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status, int fine_amount);

private:
    Ui::account *ui;

    QString currentTransport;
    QString currentPhone;

    QLineEdit *newTSLineEdit;
    QLineEdit *newPhoneLineEdit;
    QLineEdit *oldPassLineEdit;
    QLineEdit *newPassLineEdit;
};

#endif // ACCOUNT_H
