/********************************************************************************
** Form generated from reading UI file 'UserManagementDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERMANAGEMENTDIALOG_H
#define UI_USERMANAGEMENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_UserManagementDialog
{
public:
    QLabel *label;
    QPushButton *btnAddUser;
    QLabel *label_2;
    QSpinBox *inputDeleteId;
    QPushButton *btnDeleteUser;
    QComboBox *inputRole;
    QLineEdit *inputPassword;
    QLineEdit *inputUsername;

    void setupUi(QDialog *UserManagementDialog)
    {
        if (UserManagementDialog->objectName().isEmpty())
            UserManagementDialog->setObjectName("UserManagementDialog");
        UserManagementDialog->resize(449, 323);
        label = new QLabel(UserManagementDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 50, 191, 20));
        btnAddUser = new QPushButton(UserManagementDialog);
        btnAddUser->setObjectName("btnAddUser");
        btnAddUser->setGeometry(QRect(150, 110, 101, 32));
        label_2 = new QLabel(UserManagementDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 180, 191, 16));
        inputDeleteId = new QSpinBox(UserManagementDialog);
        inputDeleteId->setObjectName("inputDeleteId");
        inputDeleteId->setGeometry(QRect(30, 210, 141, 21));
        btnDeleteUser = new QPushButton(UserManagementDialog);
        btnDeleteUser->setObjectName("btnDeleteUser");
        btnDeleteUser->setGeometry(QRect(150, 240, 101, 32));
        inputRole = new QComboBox(UserManagementDialog);
        inputRole->addItem(QString());
        inputRole->addItem(QString());
        inputRole->addItem(QString());
        inputRole->setObjectName("inputRole");
        inputRole->setGeometry(QRect(300, 70, 111, 31));
        inputPassword = new QLineEdit(UserManagementDialog);
        inputPassword->setObjectName("inputPassword");
        inputPassword->setGeometry(QRect(164, 75, 131, 21));
        inputUsername = new QLineEdit(UserManagementDialog);
        inputUsername->setObjectName("inputUsername");
        inputUsername->setGeometry(QRect(31, 75, 121, 21));

        retranslateUi(UserManagementDialog);

        QMetaObject::connectSlotsByName(UserManagementDialog);
    } // setupUi

    void retranslateUi(QDialog *UserManagementDialog)
    {
        UserManagementDialog->setWindowTitle(QCoreApplication::translate("UserManagementDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("UserManagementDialog", "Dodaj nowego u\305\274ytkownika:", nullptr));
        btnAddUser->setText(QCoreApplication::translate("UserManagementDialog", "Dodaj", nullptr));
        label_2->setText(QCoreApplication::translate("UserManagementDialog", "Usu\305\204 u\305\274ytkownika po ID:", nullptr));
        btnDeleteUser->setText(QCoreApplication::translate("UserManagementDialog", "Usu\305\204", nullptr));
        inputRole->setItemText(0, QCoreApplication::translate("UserManagementDialog", "waiter", nullptr));
        inputRole->setItemText(1, QCoreApplication::translate("UserManagementDialog", "manager", nullptr));
        inputRole->setItemText(2, QCoreApplication::translate("UserManagementDialog", "admin", nullptr));

        inputPassword->setText(QCoreApplication::translate("UserManagementDialog", "password", nullptr));
        inputUsername->setInputMask(QString());
        inputUsername->setText(QCoreApplication::translate("UserManagementDialog", "username", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserManagementDialog: public Ui_UserManagementDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERMANAGEMENTDIALOG_H
