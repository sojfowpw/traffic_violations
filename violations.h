#ifndef VIOLATIONS_H
#define VIOLATIONS_H

#include <QDialog>

namespace Ui {
class violations;
}

class violations : public QDialog
{
    Q_OBJECT

public:
    explicit violations(QWidget *parent = nullptr);
    ~violations();

public slots:
    void setInfo(int idUser, const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status,
                 int fine_amount, int id1);

    void setInfo2(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status,
                  int fine_amount, int id2);

private slots:
    void on_returnToMain_clicked();

    void on_back_clicked();

    void on_infoViolation_clicked();

    void on_changeStatus1_clicked();

    void on_changeStatus2_clicked();

signals:
    void sendUserInfo(int idUser, const QString &name, const QString &surname, const QList<QVariantList> &licenseList, int violations, const QString &phone);

    void sendTableInfo(int idUser, const QList<QVariantList> &violationsList);

    void hideButtonInAllViolations();

private:
    Ui::violations *ui;

    int userId;
    int violationId1;
    int violationId2;
};

#endif // VIOLATIONS_H
