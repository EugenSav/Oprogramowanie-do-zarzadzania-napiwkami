/********************************************************************************
** Form generated from reading UI file 'metricsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METRICSDIALOG_H
#define UI_METRICSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MetricsDialog
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *labelWaiterId;
    QSpinBox *waiterIdSpinBox;
    QPushButton *btnLoadMetrics;
    QTableWidget *tableMetrics;

    void setupUi(QDialog *MetricsDialog)
    {
        if (MetricsDialog->objectName().isEmpty())
            MetricsDialog->setObjectName("MetricsDialog");
        MetricsDialog->resize(553, 260);
        formLayoutWidget = new QWidget(MetricsDialog);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(50, 20, 471, 221));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        labelWaiterId = new QLabel(formLayoutWidget);
        labelWaiterId->setObjectName("labelWaiterId");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelWaiterId);

        waiterIdSpinBox = new QSpinBox(formLayoutWidget);
        waiterIdSpinBox->setObjectName("waiterIdSpinBox");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, waiterIdSpinBox);

        btnLoadMetrics = new QPushButton(formLayoutWidget);
        btnLoadMetrics->setObjectName("btnLoadMetrics");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, btnLoadMetrics);

        tableMetrics = new QTableWidget(formLayoutWidget);
        if (tableMetrics->columnCount() < 2)
            tableMetrics->setColumnCount(2);
        tableMetrics->setObjectName("tableMetrics");
        tableMetrics->setColumnCount(2);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, tableMetrics);


        retranslateUi(MetricsDialog);

        QMetaObject::connectSlotsByName(MetricsDialog);
    } // setupUi

    void retranslateUi(QDialog *MetricsDialog)
    {
        MetricsDialog->setWindowTitle(QCoreApplication::translate("MetricsDialog", "Dialog", nullptr));
        labelWaiterId->setText(QCoreApplication::translate("MetricsDialog", "ID kelnera:", nullptr));
        btnLoadMetrics->setText(QCoreApplication::translate("MetricsDialog", "Poka\305\274 metryki", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MetricsDialog: public Ui_MetricsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METRICSDIALOG_H
