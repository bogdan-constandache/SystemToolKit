#include "../headers/process_wrapper.h"

QProcessWrapper::QProcessWrapper(QObject *parent) :
    QObject(parent), m_pProcess(NULL), m_bAlreadyRunning(false)
{
    m_pProcess = new QProcess;

    bool bResult = false;
    bResult = connect(m_pProcess, SIGNAL(finished(int)), this, SIGNAL(OnProcessFinishedSignal(int)), Qt::QueuedConnection);
    bResult = connect(m_pProcess, SIGNAL(finished(int)), this, SLOT(OnProcessFinishedSlot(int)), Qt::QueuedConnection);
    bResult = connect(m_pProcess, SIGNAL(started()), this, SIGNAL(OnProcessStartedSignal()), Qt::QueuedConnection);
}

QProcessWrapper::~QProcessWrapper()
{
}

void QProcessWrapper::StartProcess(QString qzCommand)
{
    if( m_bAlreadyRunning || m_pProcess->state() == QProcess::Running )
    {
        emit OnErrorSignal("Process is already running"); return;
    }

    m_pProcess->start(qzCommand);

    m_bAlreadyRunning = m_pProcess->waitForStarted();

    if( m_bAlreadyRunning )
        emit OnProcessStartedSignal();
}

void QProcessWrapper::Stop()
{
    if( !m_bAlreadyRunning )
    {
        emit OnErrorSignal("Process is not started"); return;
    }

    m_pProcess->kill();
    m_bAlreadyRunning = false;

    emit OnProcessStoppedSignal();
}

void QProcessWrapper::WaitForFinished()
{
    if( !m_bAlreadyRunning )
    {
        emit OnErrorSignal("Process is not started"); return;
    }

    bool bResult = m_pProcess->waitForFinished();

    if( !bResult )
    {
        emit OnErrorSignal("Process timeout"); return;
    }

    m_bAlreadyRunning = false;

    emit OnProcessFinishedSignal(0);
}

void QProcessWrapper::OnProcessFinishedSlot(int)
{
    m_bAlreadyRunning = false;
}
