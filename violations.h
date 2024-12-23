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
    void setInfo(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status, int fine_amount);

    void setInfo2(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status, int fine_amount);

private slots:
    void on_returnToMain_clicked();

    void on_back_clicked();

private:
    Ui::violations *ui;
};

#endif // VIOLATIONS_H
