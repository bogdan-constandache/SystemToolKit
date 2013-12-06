#include <QApplication>
#include <QThread>
#include "../gui/headers/main_window.h"
#include "../controller/headers/controller.h"

int main(int argc, char **argv)
{
    QApplication Application(argc, argv);
    CController Controller;
    MainWindow  View;
    QThread     ControllerThread;

    qRegisterMetaType< Qt::Orientation > ( "Qt::Orientation" );
    qRegisterMetaType< ModuleType > ( "ModuleType" );
    qRegisterMetaType< QVector<int> > ( "QVector<int>" );
    qRegisterMetaType< QList<QPersistentModelIndex> > ( "QList<QPersistentModelIndex>" );
    qRegisterMetaType< QAbstractItemModel::LayoutChangeHint > ( "QAbstractItemModel::LayoutChangeHint" );

    Controller.SetView(&View);
    View.SetController(&Controller);

    Controller.moveToThread(&ControllerThread);
    QObject::connect(&ControllerThread, SIGNAL(started()),
                     &Controller, SLOT(StartControllerSlot()),
                     Qt::QueuedConnection);

    QObject::connect(&Application, SIGNAL(aboutToQuit()),
                     &ControllerThread, SLOT(quit()),
                     Qt::DirectConnection);

    ControllerThread.start();

    Application.exec();

    ControllerThread.wait();

    return 0;
}
