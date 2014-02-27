/********************************************************************************
** Form generated from reading UI file 'smart_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMART_WIDGET_H
#define UI_SMART_WIDGET_H

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

class Ui_CSmartWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayoutButtons;
    QPushButton *refreshButton;
    QSpacerItem *hSpacerH1;
    QTreeView *hddTreeView;
    QTreeView *propertiesTreeView;

    void setupUi(QWidget *CSmartWidget)
    {
        if (CSmartWidget->objectName().isEmpty())
            CSmartWidget->setObjectName(QStringLiteral("CSmartWidget"));
        CSmartWidget->resize(627, 430);
        verticalLayout = new QVBoxLayout(CSmartWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QStringLiteral("hLayoutButtons"));
        refreshButton = new QPushButton(CSmartWidget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));

        hLayoutButtons->addWidget(refreshButton);

        hSpacerH1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutButtons->addItem(hSpacerH1);


        verticalLayout->addLayout(hLayoutButtons);

        hddTreeView = new QTreeView(CSmartWidget);
        hddTreeView->setObjectName(QStringLiteral("hddTreeView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(hddTreeView->sizePolicy().hasHeightForWidth());
        hddTreeView->setSizePolicy(sizePolicy);
        hddTreeView->setMinimumSize(QSize(0, 70));
        hddTreeView->setMaximumSize(QSize(16777215, 70));

        verticalLayout->addWidget(hddTreeView);

        propertiesTreeView = new QTreeView(CSmartWidget);
        propertiesTreeView->setObjectName(QStringLiteral("propertiesTreeView"));

        verticalLayout->addWidget(propertiesTreeView);


        retranslateUi(CSmartWidget);

        QMetaObject::connectSlotsByName(CSmartWidget);
    } // setupUi

    void retranslateUi(QWidget *CSmartWidget)
    {
        CSmartWidget->setWindowTitle(QApplication::translate("CSmartWidget", "Form", 0));
        refreshButton->setText(QApplication::translate("CSmartWidget", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class CSmartWidget: public Ui_CSmartWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMART_WIDGET_H
