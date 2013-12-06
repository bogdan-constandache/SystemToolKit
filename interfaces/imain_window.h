#ifndef IMAIN_WINDOW_H
#define IMAIN_WINDOW_H

#include <QMainWindow>

#include "module.h"

class IMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    virtual ~IMainWindow(){}
public slots:
    virtual void CreateWidgetSlot(ModuleType mType, IModule *pModule) = 0;
    virtual void SendDataToGUISlot(ModuleType, QWidget*, QStandardItemModel*) = 0;
};

#endif // IMAIN_WINDOW_H
