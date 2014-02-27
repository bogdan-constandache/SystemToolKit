/********************************************************************************
** Form generated from reading UI file 'battery_status_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BATTERY_STATUS_WIDGET_H
#define UI_BATTERY_STATUS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CBatteryStatusWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayoutButtons;
    QPushButton *refreshButton;
    QSpacerItem *hSpacer1;
    QTreeView *treeView;

    void setupUi(QWidget *CBatteryStatusWidget)
    {
        if (CBatteryStatusWidget->objectName().isEmpty())
            CBatteryStatusWidget->setObjectName(QStringLiteral("CBatteryStatusWidget"));
        CBatteryStatusWidget->resize(620, 369);
        verticalLayout = new QVBoxLayout(CBatteryStatusWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QStringLiteral("hLayoutButtons"));
        refreshButton = new QPushButton(CBatteryStatusWidget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));

        hLayoutButtons->addWidget(refreshButton);

        hSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutButtons->addItem(hSpacer1);


        verticalLayout->addLayout(hLayoutButtons);

        treeView = new QTreeView(CBatteryStatusWidget);
        treeView->setObjectName(QStringLiteral("treeView"));

        verticalLayout->addWidget(treeView);


        retranslateUi(CBatteryStatusWidget);

        QMetaObject::connectSlotsByName(CBatteryStatusWidget);
    } // setupUi

    void retranslateUi(QWidget *CBatteryStatusWidget)
    {
        CBatteryStatusWidget->setWindowTitle(QApplication::translate("CBatteryStatusWidget", "Form", 0));
        refreshButton->setText(QApplication::translate("CBatteryStatusWidget", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class CBatteryStatusWidget: public Ui_CBatteryStatusWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATTERY_STATUS_WIDGET_H
