#ifndef ADDTIPDIALOG_H
#define ADDTIPDIALOG_H

#include <QDialog>

namespace Ui {
class AddTipDialog;
}

class AddTipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTipDialog(int waiterId, QWidget *parent = nullptr);
    ~AddTipDialog();

    double amount() const;
    int secondWaiterId() const;

private:
    Ui::AddTipDialog *ui;
    int m_waiterId;
};

#endif // ADDTIPDIALOG_H
