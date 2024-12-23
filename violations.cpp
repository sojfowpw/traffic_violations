#include "violations.h"
#include "ui_violations.h"
#include "mainwindow.h"
#include "account.h"

violations::violations(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::violations)
{
    ui->setupUi(this);
    setWindowTitle("Штрафы");

    ui->groupBox_2->setVisible(false);
}

violations::~violations()
{
    delete ui;
}


void violations::setInfo(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status, int fine_amount) {
    ui->violationLabel->setText(violation_type);
    ui->violationLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->dateLabel->setText(violation_date);
    ui->dateLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->locationLabel->setText(violation_loc);
    ui->locationLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->statusLabel->setText(status);
    ui->statusLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->amountLabel->setText(QString::number(fine_amount));
    ui->amountLabel->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
}

void violations::setInfo2(const QString &violation_type, const QString &violation_date, const QString &violation_loc, const QString &status, int fine_amount) {
    if (violation_type.isEmpty()) {
        return;
    }

    ui->groupBox_2->setVisible(!ui->groupBox_2->isVisible());
    ui->violationLabel_2->setText(violation_type);
    ui->violationLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->dateLabel_2->setText(violation_date);
    ui->dateLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->locationLabel_2->setText(violation_loc);
    ui->locationLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->statusLabel_2->setText(status);
    ui->statusLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
    ui->amountLabel_2->setText(QString::number(fine_amount));
    ui->amountLabel_2->setStyleSheet("font-family: 'Cantarell Regular'; color: black; font-size: 16pt;");
}


void violations::on_returnToMain_clicked()
{
    hide();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->show();
}


void violations::on_back_clicked() // вернуться к профилю
{
    hide();
    account *acc = new account(this);
    acc->show();
}

