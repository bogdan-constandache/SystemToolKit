/********************************************************************************
** Form generated from reading UI file 'network_devices_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORK_DEVICES_WIDGET_H
#define UI_NETWORK_DEVICES_WIDGET_H

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

class Ui_CNetworkDevicesWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayoutButtons;
    QPushButton *refreshBtn;
    QSpacerItem *hSpacerH1;
    QTreeView *networkDevNames;
    QTreeView *networkDevInfos;

    void setupUi(QWidget *CNetworkDevicesWidget)
    {
        if (CNetworkDevicesWidget->objectName().isEmpty())
            CNetworkDevicesWidget->setObjectName(QStringLiteral("CNetworkDevicesWidget"));
        CNetworkDevicesWidget->resize(923, 510);
        verticalLayout = new QVBoxLayout(CNetworkDevicesWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QStringLiteral("hLayoutButtons"));
        refreshBtn = new QPushButton(CNetworkDevicesWidget);
        refreshBtn->setObjectName(QStringLiteral("refreshBtn"));

        hLayoutButtons->addWidget(refreshBtn);

        hSpacerH1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutButtons->addItem(hSpacerH1);


        verticalLayout->addLayout(hLayoutButtons);

        networkDevNames = new QTreeView(CNetworkDevicesWidget);
        networkDevNames->setObjectName(QStringLiteral("networkDevNames"));
        networkDevNames->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(networkDevNames);

        networkDevInfos = new QTreeView(CNetworkDevicesWidget);
        networkDevInfos->setObjectName(QStringLiteral("networkDevInfos"));

        verticalLayout->addWidget(networkDevInfos);


        retranslateUi(CNetworkDevicesWidget);

        QMetaObject::connectSlotsByName(CNetworkDevicesWidget);
    } // setupUi

    void retranslateUi(QWidget *CNetworkDevicesWidget)
    {
        CNetworkDevicesWidget->setWindowTitle(QApplication::translate("CNetworkDevicesWidget", "Form", 0));
        refreshBtn->setText(QApplication::translate("CNetworkDevicesWidget", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class CNetworkDevicesWidget: public Ui_CNetworkDevicesWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORK_DEVICES_WIDGET_H
