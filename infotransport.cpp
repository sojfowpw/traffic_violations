#include "infotransport.h"
#include "ui_infotransport.h"

infoTransport::infoTransport(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::infoTransport)
{
    ui->setupUi(this);
    setWindowTitle("Справка");
}

infoTransport::~infoTransport()
{
    delete ui;
}

void infoTransport::on_pushButton_clicked()
{
    hide();
}
