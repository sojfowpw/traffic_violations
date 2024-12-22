#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName("localhost");
    database.setDatabaseName("traffic");
    database.setUserName("postgres");
    database.setPassword("password");
    database.setPort(5432);

    if (database.open()) {
        qDebug() << "Успешно.";
    }
    else {
        qDebug() << "Отклонено.";
    }
    return a.exec();
}
