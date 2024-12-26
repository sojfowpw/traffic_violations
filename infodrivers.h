#ifndef INFODRIVERS_H
#define INFODRIVERS_H

#include <QDialog>

namespace Ui {
class infoDrivers;
}

class infoDrivers : public QDialog
{
    Q_OBJECT

public:
    explicit infoDrivers(QWidget *parent = nullptr);
    ~infoDrivers();

public slots:
    void setInfo(const QList<QVariantList> &violationsList);

private slots:
    void on_returnToMain_clicked();

    void on_back_clicked();

private:
    Ui::infoDrivers *ui;
};

#endif // INFODRIVERS_H
