#ifndef PROCESS_WRAPPER_H
#define PROCESS_WRAPPER_H

#include <QObject>
#include <QProcess>

class QProcessWrapper : public QObject
{
    Q_OBJECT

private:
    QProcess *m_pProcess;
    bool m_bAlreadyRunning;

private slots:
    void OnProcessFinishedSlot(int);

public:
    explicit QProcessWrapper(QObject *parent = 0);
    ~QProcessWrapper();

    void StartProcess(QString qzCommand);
    void Stop();
    void WaitForFinished();

signals:
    void OnProcessStartedSignal();
    void OnProcessFinishedSignal(int);
    void OnProcessStoppedSignal();
    void OnErrorSignal(QString);

public slots:
};

#endif // PROCESS_WRAPPER_H
