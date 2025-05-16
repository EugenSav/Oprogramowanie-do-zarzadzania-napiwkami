#include "EditLastTipDialog.h"
#include "ui_EditLastTipDialog.h"

EditLastTipDialog::EditLastTipDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::EditLastTipDialog)
{
    ui->setupUi(this);
}

EditLastTipDialog::~EditLastTipDialog()
{
    delete ui;
}

double EditLastTipDialog::newAmount() const
{
    return ui->newAmountSpinBox->value();
}
