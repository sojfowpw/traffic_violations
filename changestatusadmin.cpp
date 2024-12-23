#include "changestatusadmin.h"
#include "ui_changestatusadmin.h"
#include "mainwindow.h"
#include "admin.h"

changeStatusAdmin::changeStatusAdmin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::changeStatusAdmin)
{
    ui->setupUi(this);
    setWindowTitle("Оплата");
}

changeStatusAdmin::~changeStatusAdmin()
{
    delete ui;
}

void changeStatusAdmin::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void changeStatusAdmin::on_back_clicked()
{
    hide();
    admin *adminWindow = new admin(this);
    adminWindow->show();
}

