#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>

#define CONFIGURATION_FILE QString("configuration.cfg")

namespace Ui {
class CAboutDialog;
}

class CAboutDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::CAboutDialog *ui;

public:
    explicit CAboutDialog(QWidget *parent = 0);
    ~CAboutDialog();

protected:
    void showEvent(QShowEvent *);

private slots:
    void OnReadConfigurationFileSlot();

signals:
    void OnReadConfigurationFileSignal();
};

#endif // ABOUT_DIALOG_H
