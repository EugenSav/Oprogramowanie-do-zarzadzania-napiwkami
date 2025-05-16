#include <QApplication>
#include <QSqlDatabase>
#include "LoginDialog.h"
#include "MainWindow.h"
#include "DatabaseService.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString dbPath = "/Users/maciejbajer/project/tips.db";
    if (!DatabaseService::connect(dbPath)) {
        qDebug() << "Błąd połączenia z bazą danych:" << dbPath;
        return 1;
    }
    qDebug() << "Połączono z bazą danych:" << dbPath;

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w(login.userId, login.role);
        w.show();
        return a.exec();
    }

    return 0;
}
