#ifndef CHANGESTATUSADMIN_H
#define CHANGESTATUSADMIN_H

#include <QDialog>

namespace Ui {
class changeStatusAdmin;
}

class changeStatusAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit changeStatusAdmin(QWidget *parent = nullptr);
    ~changeStatusAdmin();

private slots:
    void on_returnToMain_clicked();

    void on_back_clicked();

private:
    Ui::changeStatusAdmin *ui;
};

#endif // CHANGESTATUSADMIN_H
