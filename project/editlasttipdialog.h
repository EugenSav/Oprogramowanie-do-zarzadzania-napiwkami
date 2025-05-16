#ifndef EDITLASTTIPDIALOG_H
#define EDITLASTTIPDIALOG_H

#include <QDialog>

namespace Ui {
class EditLastTipDialog;
}

class EditLastTipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditLastTipDialog(QWidget *parent = nullptr);
    ~EditLastTipDialog();

    double newAmount() const;

private:
    Ui::EditLastTipDialog *ui;
};

#endif // EDITLASTTIPDIALOG_H
