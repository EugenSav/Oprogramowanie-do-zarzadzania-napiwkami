#ifndef USERMANAGEMENTDIALOG_H
#define USERMANAGEMENTDIALOG_H

#include <QDialog>
#include "UserRepository.h"

namespace Ui {
class UserManagementDialog;
}

class UserManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagementDialog(QWidget *parent = nullptr);
    ~UserManagementDialog();

private slots:
    void on_btnAddUser_clicked();
    void on_btnDeleteUser_clicked();

private:
    Ui::UserManagementDialog *ui;
    UserRepository repository;
};

#endif // USERMANAGEMENTDIALOG_H
