#include "AddTipDialog.h"
#include "ui_AddTipDialog.h"

AddTipDialog::AddTipDialog(int waiterId, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AddTipDialog),
    m_waiterId(waiterId)
{
    ui->setupUi(this);
    ui->secondWaiterIdSpinBox->setMinimum(0); // 0 = brak drugiego kelnera
}

AddTipDialog::~AddTipDialog()
{
    delete ui;
}

double AddTipDialog::amount() const
{
    return ui->amountSpinBox->value();
}

int AddTipDialog::secondWaiterId() const
{
    return ui->secondWaiterIdSpinBox->value();
}
