#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "login.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("АВТОШтраф");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_registration_clicked()
{
    hide();
    reg = new registration(this);
    reg->show();
}


void MainWindow::on_Button_login_clicked()
{
    hide();
    login *log = new login(this);
    log->show();
}

