#ifndef ALLVIOLATIONS_H
#define ALLVIOLATIONS_H

#include <QDialog>

namespace Ui {
class allViolations;
}

class allViolations : public QDialog
{
    Q_OBJECT

public:
    explicit allViolations(QWidget *parent = nullptr);
    ~allViolations();

public slots:
    void setInfo(int idUser, const QList<QVariantList> &violationsList);

    void hideButton();

    void hideButton2();

private slots:
    void on_returnToMain_clicked();

    void on_back_clicked();

    void on_backAdmin_clicked();

signals:
    void sendUserInfo(const QString &name, const QString &surname, const QString &transport, int violations, const QString &phone);

private:
    Ui::allViolations *ui;

    int userId;
};

#endif // ALLVIOLATIONS_H
