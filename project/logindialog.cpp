#include "logindialog.h"
#include "ui_logindialog.h"
#include "UserManager.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::attemptLogin() {
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();

    UserManager um;
    if (um.login(login, password)) {
        role = um.getRole(login);
        userId = um.getUserId(login);
        accept(); // zamyka dialog i przechodzi do MainWindow
    } else {
        ui->errorLabel->setText("Niepoprawny login lub hasło");
    }
}
