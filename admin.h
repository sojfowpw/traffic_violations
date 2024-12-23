#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>

namespace Ui {
class admin;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void on_returnToMain_clicked();

    void on_infoViolations_clicked();

    void on_changeStatus_clicked();

signals:
    void hideButtonInAllViolations();

    void sendTableInfo(int idUser, const QList<QVariantList> &violationsList);

private:
    Ui::admin *ui;
};

#endif // ADMIN_H
