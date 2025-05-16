/********************************************************************************
** Form generated from reading UI file 'EditLastTipDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITLASTTIPDIALOG_H
#define UI_EDITLASTTIPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditLastTipDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QDoubleSpinBox *newAmountSpinBox;
    QLabel *label;

    void setupUi(QDialog *EditLastTipDialog)
    {
        if (EditLastTipDialog->objectName().isEmpty())
            EditLastTipDialog->setObjectName("EditLastTipDialog");
        EditLastTipDialog->resize(407, 245);
        buttonBox = new QDialogButtonBox(EditLastTipDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(160, 130, 81, 241));
        buttonBox->setOrientation(Qt::Orientation::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        formLayoutWidget = new QWidget(EditLastTipDialog);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(50, 80, 311, 41));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        newAmountSpinBox = new QDoubleSpinBox(formLayoutWidget);
        newAmountSpinBox->setObjectName("newAmountSpinBox");
        newAmountSpinBox->setMinimum(0.000000000000000);
        newAmountSpinBox->setMaximum(100000.000000000000000);
        newAmountSpinBox->setValue(0.000000000000000);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, newAmountSpinBox);

        label = new QLabel(formLayoutWidget);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);


        retranslateUi(EditLastTipDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, EditLastTipDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, EditLastTipDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(EditLastTipDialog);
    } // setupUi

    void retranslateUi(QDialog *EditLastTipDialog)
    {
        EditLastTipDialog->setWindowTitle(QCoreApplication::translate("EditLastTipDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("EditLastTipDialog", "Nowa kwota napiwku", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditLastTipDialog: public Ui_EditLastTipDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITLASTTIPDIALOG_H
