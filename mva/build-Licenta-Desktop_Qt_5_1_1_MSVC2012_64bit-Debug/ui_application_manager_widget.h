/********************************************************************************
** Form generated from reading UI file 'application_manager_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPLICATION_MANAGER_WIDGET_H
#define UI_APPLICATION_MANAGER_WIDGET_H

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

class Ui_CApplicationManagerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayoutButtons;
    QPushButton *refreshButton;
    QPushButton *uninstallButton;
    QSpacerItem *hSpacer1;
    QTableView *tableView;

    void setupUi(QWidget *CApplicationManagerWidget)
    {
        if (CApplicationManagerWidget->objectName().isEmpty())
            CApplicationManagerWidget->setObjectName(QStringLiteral("CApplicationManagerWidget"));
        CApplicationManagerWidget->resize(703, 441);
        verticalLayout = new QVBoxLayout(CApplicationManagerWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QStringLiteral("hLayoutButtons"));
        refreshButton = new QPushButton(CApplicationManagerWidget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));

        hLayoutButtons->addWidget(refreshButton);

        uninstallButton = new QPushButton(CApplicationManagerWidget);
        uninstallButton->setObjectName(QStringLiteral("uninstallButton"));

        hLayoutButtons->addWidget(uninstallButton);

        hSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutButtons->addItem(hSpacer1);


        verticalLayout->addLayout(hLayoutButtons);

        tableView = new QTableView(CApplicationManagerWidget);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout->addWidget(tableView);


        retranslateUi(CApplicationManagerWidget);

        QMetaObject::connectSlotsByName(CApplicationManagerWidget);
    } // setupUi

    void retranslateUi(QWidget *CApplicationManagerWidget)
    {
        CApplicationManagerWidget->setWindowTitle(QApplication::translate("CApplicationManagerWidget", "Form", 0));
        refreshButton->setText(QApplication::translate("CApplicationManagerWidget", "Refresh", 0));
        uninstallButton->setText(QApplication::translate("CApplicationManagerWidget", "Uninstall", 0));
    } // retranslateUi

};

namespace Ui {
    class CApplicationManagerWidget: public Ui_CApplicationManagerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPLICATION_MANAGER_WIDGET_H
