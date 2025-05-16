/********************************************************************************
** Form generated from reading UI file 'BalanceDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BALANCEDIALOG_H
#define UI_BALANCEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BalanceDialog
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *labelDailyBalance;
    QLabel *labelMonthlyBalance;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QSpinBox *waiterIdSpinBox;
    QLabel *labelWaiterId;
    QPushButton *btnLoadBalance;

    void setupUi(QDialog *BalanceDialog)
    {
        if (BalanceDialog->objectName().isEmpty())
            BalanceDialog->setObjectName("BalanceDialog");
        BalanceDialog->resize(400, 359);
        formLayoutWidget = new QWidget(BalanceDialog);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(70, 150, 271, 71));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        labelDailyBalance = new QLabel(formLayoutWidget);
        labelDailyBalance->setObjectName("labelDailyBalance");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelDailyBalance);

        labelMonthlyBalance = new QLabel(formLayoutWidget);
        labelMonthlyBalance->setObjectName("labelMonthlyBalance");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelMonthlyBalance);

        formLayoutWidget_2 = new QWidget(BalanceDialog);
        formLayoutWidget_2->setObjectName("formLayoutWidget_2");
        formLayoutWidget_2->setGeometry(QRect(70, 110, 271, 31));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName("formLayout_2");
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        waiterIdSpinBox = new QSpinBox(formLayoutWidget_2);
        waiterIdSpinBox->setObjectName("waiterIdSpinBox");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, waiterIdSpinBox);

        labelWaiterId = new QLabel(formLayoutWidget_2);
        labelWaiterId->setObjectName("labelWaiterId");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, labelWaiterId);

        btnLoadBalance = new QPushButton(BalanceDialog);
        btnLoadBalance->setObjectName("btnLoadBalance");
        btnLoadBalance->setGeometry(QRect(140, 230, 101, 32));

        retranslateUi(BalanceDialog);

        QMetaObject::connectSlotsByName(BalanceDialog);
    } // setupUi

    void retranslateUi(QDialog *BalanceDialog)
    {
        BalanceDialog->setWindowTitle(QCoreApplication::translate("BalanceDialog", "Dialog", nullptr));
        labelDailyBalance->setText(QCoreApplication::translate("BalanceDialog", "Saldo dzienne:      ", nullptr));
        labelMonthlyBalance->setText(QCoreApplication::translate("BalanceDialog", "Saldo miesi\304\231czne: ", nullptr));
        labelWaiterId->setText(QCoreApplication::translate("BalanceDialog", "ID kelnera: ", nullptr));
        btnLoadBalance->setText(QCoreApplication::translate("BalanceDialog", "Poka\305\274 saldo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BalanceDialog: public Ui_BalanceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BALANCEDIALOG_H
