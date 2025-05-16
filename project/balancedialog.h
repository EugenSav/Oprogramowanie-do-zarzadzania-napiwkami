#ifndef BALANCEDIALOG_H
#define BALANCEDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class BalanceDialog;
}

class BalanceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BalanceDialog(int userId, const QString& role, QWidget *parent = nullptr);
    ~BalanceDialog();

private:
    Ui::BalanceDialog *ui;
    int m_userId;
    QString m_role;

    void loadBalance(int targetWaiterId);

private slots:
    void on_btnLoadBalance_clicked();
};

#endif // BALANCEDIALOG_H
