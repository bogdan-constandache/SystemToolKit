#include "view_adapter.h"

ViewAdapter::ViewAdapter(QObject *parent) :
    QObject(parent),
    m_pController(NULL),
    m_pMainWindow(NULL)
{
}

void ViewAdapter::SetController(AbstractController *pController)
{
    Q_ASSERT(pController);

    m_pController = pController;

    connect(m_pController, SIGNAL(OnCreateMainWindowSignal()), this, SLOT(OnCreateMainWindowSlot()), Qt::BlockingQueuedConnection);
    connect(m_pController, SIGNAL(OnShowMainWindowSignal()), this, SLOT(OnShowMainWindowSlot()), Qt::BlockingQueuedConnection);
}

void ViewAdapter::Initialize()
{
    connect(this, SIGNAL(OnInitializeSignal()), this, SLOT(OnInitializeSlot()), Qt::QueuedConnection);
    connect(this, SIGNAL(OnDestroySignal()), this, SLOT(OnDestroySlot()), Qt::QueuedConnection);

    emit OnInitializeSignal();
}

void ViewAdapter::Destroy()
{
    emit OnDestroySignal();
}

void ViewAdapter::OnInitializeSlot()
{

}

void ViewAdapter::OnDestroySlot()
{
    if (m_pMainWindow)
    {
        delete m_pMainWindow;
        m_pMainWindow = 0;
    }
}

void ViewAdapter::OnCreateMainWindowSlot()
{
    m_pMainWindow = new MainWindow(0, m_pController);
}

void ViewAdapter::OnShowMainWindowSlot()
{
    m_pMainWindow->show();
}
