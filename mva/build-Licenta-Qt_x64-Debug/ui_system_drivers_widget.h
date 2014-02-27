/********************************************************************************
** Form generated from reading UI file 'system_drivers_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEM_DRIVERS_WIDGET_H
#define UI_SYSTEM_DRIVERS_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CSystemDriversWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayoutButtons;
    QPushButton *refreshButton;
    QSpacerItem *hSpacerH1;
    QTableView *tableView;

    void setupUi(QWidget *CSystemDriversWidget)
    {
        if (CSystemDriversWidget->objectName().isEmpty())
            CSystemDriversWidget->setObjectName(QStringLiteral("CSystemDriversWidget"));
        CSystemDriversWidget->resize(1063, 568);
        verticalLayout = new QVBoxLayout(CSystemDriversWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QStringLiteral("hLayoutButtons"));
        refreshButton = new QPushButton(CSystemDriversWidget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));

        hLayoutButtons->addWidget(refreshButton);

        hSpacerH1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutButtons->addItem(hSpacerH1);


        verticalLayout->addLayout(hLayoutButtons);

        tableView = new QTableView(CSystemDriversWidget);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout->addWidget(tableView);


        retranslateUi(CSystemDriversWidget);

        QMetaObject::connectSlotsByName(CSystemDriversWidget);
    } // setupUi

    void retranslateUi(QWidget *CSystemDriversWidget)
    {
        CSystemDriversWidget->setWindowTitle(QApplication::translate("CSystemDriversWidget", "Form", 0));
        refreshButton->setText(QApplication::translate("CSystemDriversWidget", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class CSystemDriversWidget: public Ui_CSystemDriversWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEM_DRIVERS_WIDGET_H
