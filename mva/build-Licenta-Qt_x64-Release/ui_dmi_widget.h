/********************************************************************************
** Form generated from reading UI file 'dmi_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DMI_WIDGET_H
#define UI_DMI_WIDGET_H

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

class Ui_CDMIWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayoutButtons;
    QPushButton *refreshButton;
    QSpacerItem *hSpacerH1;
    QTreeView *itemsTree;
    QTreeView *propertiesTree;

    void setupUi(QWidget *CDMIWidget)
    {
        if (CDMIWidget->objectName().isEmpty())
            CDMIWidget->setObjectName(QStringLiteral("CDMIWidget"));
        CDMIWidget->resize(627, 430);
        verticalLayout = new QVBoxLayout(CDMIWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        hLayoutButtons = new QHBoxLayout();
        hLayoutButtons->setObjectName(QStringLiteral("hLayoutButtons"));
        refreshButton = new QPushButton(CDMIWidget);
        refreshButton->setObjectName(QStringLiteral("refreshButton"));

        hLayoutButtons->addWidget(refreshButton);

        hSpacerH1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayoutButtons->addItem(hSpacerH1);


        verticalLayout->addLayout(hLayoutButtons);

        itemsTree = new QTreeView(CDMIWidget);
        itemsTree->setObjectName(QStringLiteral("itemsTree"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(itemsTree->sizePolicy().hasHeightForWidth());
        itemsTree->setSizePolicy(sizePolicy);
        itemsTree->setMinimumSize(QSize(0, 0));
        itemsTree->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout->addWidget(itemsTree);

        propertiesTree = new QTreeView(CDMIWidget);
        propertiesTree->setObjectName(QStringLiteral("propertiesTree"));

        verticalLayout->addWidget(propertiesTree);


        retranslateUi(CDMIWidget);

        QMetaObject::connectSlotsByName(CDMIWidget);
    } // setupUi

    void retranslateUi(QWidget *CDMIWidget)
    {
        CDMIWidget->setWindowTitle(QApplication::translate("CDMIWidget", "Form", 0));
        refreshButton->setText(QApplication::translate("CDMIWidget", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class CDMIWidget: public Ui_CDMIWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DMI_WIDGET_H
