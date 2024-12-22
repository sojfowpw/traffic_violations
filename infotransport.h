#ifndef INFOTRANSPORT_H
#define INFOTRANSPORT_H

#include <QDialog>

namespace Ui {
class infoTransport;
}

class infoTransport : public QDialog
{
    Q_OBJECT

public:
    explicit infoTransport(QWidget *parent = nullptr);
    ~infoTransport();

private slots:
    void on_pushButton_clicked();

private:
    Ui::infoTransport *ui;
};

#endif // INFOTRANSPORT_H
