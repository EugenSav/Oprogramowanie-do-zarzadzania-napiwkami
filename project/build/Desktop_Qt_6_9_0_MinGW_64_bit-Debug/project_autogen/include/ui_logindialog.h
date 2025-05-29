/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QLabel *login;
    QLineEdit *loginEdit;
    QLabel *haslo;
    QLineEdit *passwordEdit;
    QLabel *errorLabel;
    QPushButton *loginButton;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(450, 291);
        login = new QLabel(LoginDialog);
        login->setObjectName("login");
        login->setGeometry(QRect(120, 90, 58, 16));
        loginEdit = new QLineEdit(LoginDialog);
        loginEdit->setObjectName("loginEdit");
        loginEdit->setGeometry(QRect(180, 90, 113, 21));
        haslo = new QLabel(LoginDialog);
        haslo->setObjectName("haslo");
        haslo->setGeometry(QRect(120, 130, 41, 16));
        passwordEdit = new QLineEdit(LoginDialog);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(180, 130, 113, 21));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
        errorLabel = new QLabel(LoginDialog);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setGeometry(QRect(110, 240, 191, 21));
        loginButton = new QPushButton(LoginDialog);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(180, 170, 101, 32));

        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Dialog", nullptr));
        login->setText(QCoreApplication::translate("LoginDialog", "Login:", nullptr));
        haslo->setText(QCoreApplication::translate("LoginDialog", "Haslo:", nullptr));
        errorLabel->setText(QString());
        loginButton->setText(QCoreApplication::translate("LoginDialog", "Zaloguj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
