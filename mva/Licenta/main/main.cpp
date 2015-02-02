#include <QApplication>
#include <QThread>
#include "../controller/controller.h"
#include "../gui/view_adapter.h"


int main(int argc, char *argv[])
{
    QApplication Application(argc, argv);
    QThread ControllerThread;
    ViewAdapter *pViewAdapter = new ViewAdapter();
    Controller *pController  = new Controller();

    qRegisterMetaType< DMIModuleType > ( "DMIModuleType" );
    qRegisterMetaType< std::string > ( "std::string" );
    qRegisterMetaType< HANDLE > ( "HANDLE" );
    qRegisterMetaType< Qt::Orientation > ( "Qt::Orientation" );
    qRegisterMetaType< QVector<int> > ( "QVector<int>" );
    qRegisterMetaType< QList<QPersistentModelIndex> > ( "QList<QPersistentModelIndex>" );
    qRegisterMetaType< QAbstractItemModel::LayoutChangeHint > ( "QAbstractItemModel::LayoutChangeHint" );

    pController->moveToThread(&ControllerThread);
    pViewAdapter->SetController(pController);

    QObject::connect(&ControllerThread, SIGNAL(started()), pController, SLOT(StartController()), Qt::QueuedConnection);
    QObject::connect(&Application, SIGNAL(aboutToQuit()), &ControllerThread, SLOT(quit()), Qt::DirectConnection);

    ControllerThread.start();

    int nStatus = Application.exec();

    ControllerThread.wait();

    delete pController;
    delete pViewAdapter;

    return nStatus;
}
