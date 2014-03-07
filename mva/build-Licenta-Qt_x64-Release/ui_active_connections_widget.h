/********************************************************************************
** Form generated from reading UI file 'active_connections_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACTIVE_CONNECTIONS_WIDGET_H
#define UI_ACTIVE_CONNECTIONS_WIDGET_H

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

class Ui_CActiveConnectionsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayoutButtons;
    QPushButton *refreshButton;
    QSpacerItem *hSpacer1;
    QTableView *tableView;

    void setupUi(QWidget *CActiveConnectionsWidget)
    {
        if (CActiveConnectionsWidget->objectName().isEmpty())
            CActiveConnectionsWidget->setObjectName(QStringLiteral("CActiveConnectionsWidget"));
        CActiveConnectionsWidget->resize(813, 432);
        verticalLayout = new QVBoxLayout(CActiveConnectionsWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QStringLiteral("hLayoutButtons"));
        refreshButton = new QPushButton(CActiveConnectionsWidget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));

        hLayoutButtons->addWidget(refreshButton);

        hSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutButtons->addItem(hSpacer1);


        verticalLayout->addLayout(hLayoutButtons);

        tableView = new QTableView(CActiveConnectionsWidget);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout->addWidget(tableView);


        retranslateUi(CActiveConnectionsWidget);

        QMetaObject::connectSlotsByName(CActiveConnectionsWidget);
    } // setupUi

    void retranslateUi(QWidget *CActiveConnectionsWidget)
    {
        CActiveConnectionsWidget->setWindowTitle(QApplication::translate("CActiveConnectionsWidget", "Form", 0));
        refreshButton->setText(QApplication::translate("CActiveConnectionsWidget", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class CActiveConnectionsWidget: public Ui_CActiveConnectionsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACTIVE_CONNECTIONS_WIDGET_H
