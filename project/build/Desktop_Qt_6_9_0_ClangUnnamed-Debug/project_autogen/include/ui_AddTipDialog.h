/********************************************************************************
** Form generated from reading UI file 'AddTipDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDTIPDIALOG_H
#define UI_ADDTIPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddTipDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QDoubleSpinBox *amountSpinBox;
    QLabel *label_2;
    QSpinBox *secondWaiterIdSpinBox;
    QLabel *label;

    void setupUi(QDialog *AddTipDialog)
    {
        if (AddTipDialog->objectName().isEmpty())
            AddTipDialog->setObjectName("AddTipDialog");
        AddTipDialog->resize(399, 405);
        buttonBox = new QDialogButtonBox(AddTipDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(150, 230, 81, 241));
        buttonBox->setOrientation(Qt::Orientation::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        formLayoutWidget = new QWidget(AddTipDialog);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(40, 139, 311, 101));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        amountSpinBox = new QDoubleSpinBox(formLayoutWidget);
        amountSpinBox->setObjectName("amountSpinBox");
        amountSpinBox->setMaximum(100000.000000000000000);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, amountSpinBox);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        secondWaiterIdSpinBox = new QSpinBox(formLayoutWidget);
        secondWaiterIdSpinBox->setObjectName("secondWaiterIdSpinBox");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, secondWaiterIdSpinBox);

        label = new QLabel(formLayoutWidget);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        amountSpinBox->raise();
        label_2->raise();
        secondWaiterIdSpinBox->raise();
        label->raise();

        retranslateUi(AddTipDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, AddTipDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, AddTipDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(AddTipDialog);
    } // setupUi

    void retranslateUi(QDialog *AddTipDialog)
    {
        AddTipDialog->setWindowTitle(QCoreApplication::translate("AddTipDialog", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("AddTipDialog", "ID drugiego kelnera", nullptr));
        label->setText(QCoreApplication::translate("AddTipDialog", "Kwota napiwku:      ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddTipDialog: public Ui_AddTipDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDTIPDIALOG_H
