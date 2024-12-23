#ifndef DRIVERS_H
#define DRIVERS_H

#include <QDialog>

namespace Ui {
class drivers;
}

class drivers : public QDialog
{
    Q_OBJECT

public:
    explicit drivers(QWidget *parent = nullptr);
    ~drivers();

public slots:
    void setInfo(const QList<QVariantList> &violationsList);

private slots:
    void on_returnToMain_clicked();

    void on_back_clicked();

private:
    Ui::drivers *ui;
};

#endif // DRIVERS_H
