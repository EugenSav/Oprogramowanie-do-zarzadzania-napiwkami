#include "UserManagementDialog.h"
#include "ui_UserManagementDialog.h"
#include <QMessageBox>
#include <QCryptographicHash>

UserManagementDialog::UserManagementDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::UserManagementDialog)
{
    ui->setupUi(this);
}

UserManagementDialog::~UserManagementDialog()
{
    delete ui;
}

void UserManagementDialog::on_btnAddUser_clicked()
{
    QString login = ui->inputUsername->text().trimmed();
    QString password = ui->inputPassword->text().trimmed();
    QString role = ui->inputRole->currentText();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Login i hasło nie mogą być puste.");
        return;
    }

    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashHex = hash.toHex();

    if (repository.insertUser(login, hashHex, role)) {
        QMessageBox::information(this, "Sukces", "Dodano użytkownika.");
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się dodać użytkownika.");
    }
}

void UserManagementDialog::on_btnDeleteUser_clicked()
{
    int id = ui->inputDeleteId->value();
    if (repository.deleteUser(id)) {
        QMessageBox::information(this, "Sukces", "Użytkownik usunięty.");
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć użytkownika.");
    }
}
