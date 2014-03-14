/********************************************************************************
** Form generated from reading UI file 'cpuid_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPUID_WIDGET_H
#define UI_CPUID_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCPUIDWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;

    void setupUi(QWidget *CCPUIDWidget)
    {
        if (CCPUIDWidget->objectName().isEmpty())
            CCPUIDWidget->setObjectName(QStringLiteral("CCPUIDWidget"));
        CCPUIDWidget->resize(1058, 684);
        verticalLayout = new QVBoxLayout(CCPUIDWidget);
        verticalLayout->setSpacing(5);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeView = new QTreeView(CCPUIDWidget);
        treeView->setObjectName(QStringLiteral("treeView"));

        verticalLayout->addWidget(treeView);


        retranslateUi(CCPUIDWidget);

        QMetaObject::connectSlotsByName(CCPUIDWidget);
    } // setupUi

    void retranslateUi(QWidget *CCPUIDWidget)
    {
        CCPUIDWidget->setWindowTitle(QApplication::translate("CCPUIDWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class CCPUIDWidget: public Ui_CCPUIDWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPUID_WIDGET_H
