/********************************************************************************
** Form generated from reading UI file 'ata_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATA_WIDGET_H
#define UI_ATA_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CATAWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *hddTree;
    QTreeView *propertiesTree;

    void setupUi(QWidget *CATAWidget)
    {
        if (CATAWidget->objectName().isEmpty())
            CATAWidget->setObjectName(QStringLiteral("CATAWidget"));
        CATAWidget->resize(1102, 608);
        verticalLayout = new QVBoxLayout(CATAWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        hddTree = new QTreeView(CATAWidget);
        hddTree->setObjectName(QStringLiteral("hddTree"));
        hddTree->setMinimumSize(QSize(0, 100));
        hddTree->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(hddTree);

        propertiesTree = new QTreeView(CATAWidget);
        propertiesTree->setObjectName(QStringLiteral("propertiesTree"));

        verticalLayout->addWidget(propertiesTree);


        retranslateUi(CATAWidget);

        QMetaObject::connectSlotsByName(CATAWidget);
    } // setupUi

    void retranslateUi(QWidget *CATAWidget)
    {
        CATAWidget->setWindowTitle(QApplication::translate("CATAWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class CATAWidget: public Ui_CATAWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATA_WIDGET_H
