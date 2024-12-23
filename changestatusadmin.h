#ifndef CHANGESTATUSADMIN_H
#define CHANGESTATUSADMIN_H

#include <QDialog>
#include <QDebug>
#include <QString>

namespace Ui {
class changeStatusAdmin;
}

class changeStatusAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit changeStatusAdmin(QWidget *parent = nullptr);
    ~changeStatusAdmin();

public slots:
    void setInfo(const QList<QVariantList> &violationsList);

private slots:
    void on_returnToMain_clicked();

    void on_back_clicked();

    void on_acceptChange_clicked();

    void on_save_clicked();

private:
    Ui::changeStatusAdmin *ui;
};

#endif // CHANGESTATUSADMIN_H
