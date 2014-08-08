#include "../headers/about_dialog.h"
#include "ui_about_dialog.h"
#include "../../proto-buffers/configuration_data.pb.h"

#include <QFile>
#include <QShowEvent>

CAboutDialog::CAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAboutDialog)
{
    ui->setupUi(this);

    setWindowTitle("System Monitoring - About");
    setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);

    ui->titleLabel->clear();
    ui->iconLabel->clear();

    // LOGO
    ui->iconLabel->setFixedSize(48, 48);
    ui->iconLabel->setPixmap(QPixmap(":/img/logo.png"));
    ui->titleLabel->setFixedHeight(48);
    ui->titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QFile qFile(":/style/qss/about_dialog.qss");
    if( !qFile.open(QIODevice::ReadOnly) )
        return;

    QString qzStyleSheet = QLatin1String(qFile.readAll());

    setStyleSheet(qzStyleSheet);
    ensurePolished();

    connect(this, SIGNAL(OnReadConfigurationFileSignal()),
            this, SLOT(OnReadConfigurationFileSlot()), Qt::QueuedConnection);
}

CAboutDialog::~CAboutDialog()
{
    delete ui;
}

void CAboutDialog::showEvent(QShowEvent *pEvent)
{
    emit OnReadConfigurationFileSignal();

    QDialog::showEvent(pEvent);
}

void CAboutDialog::OnReadConfigurationFileSlot()
{
    QString qzConfigName = CONFIGURATION_FILE;

    QFile qFile(qzConfigName);
    if( !qFile.open(QIODevice::ReadOnly) )
    {
        ui->aboutContentTBrowser->clear();
        ui->aboutContentTBrowser->setText("There was a problem reading informations from configuration file!!!");
        return;
    }

    QByteArray msgBuffer = qFile.readAll();

    qFile.close();

    AboutData msgAbout;

    if( !msgAbout.ParseFromArray(msgBuffer.data(), msgBuffer.size()) )
    {
        ui->aboutContentTBrowser->clear();
        ui->aboutContentTBrowser->setText("There was a problem reading informations from configuration file!!!");
        return;
    }

    ui->titleLabel->setText(msgAbout.applicationname().c_str());

    QString qzAboutContent;

    qzAboutContent = "<p>$1</p><p>$2</p><p>$3</p><p>$4</p><p>$5</p>";

    QString qzApplicationName = "<font family=\"Segoe Ui\" size=\"14\" color=\"white\">$1,</font>";
    qzApplicationName.replace("$1", msgAbout.applicationname().c_str());
    QString qzCompany = "<font family=\"Segoe Ui\" size=\"14\" color=\"white\">$1,</font>";
    qzCompany.replace("$1", msgAbout.companyname().c_str());
    QString qzAuthor = "<font family=\"Segoe Ui\" size=\"14\" color=\"white\">$1</font>";
    qzAuthor.replace("$1", msgAbout.author().c_str());
    QString qzOS = "<font family=\"Segoe Ui\" size=\"14\" color=\"white\">$1</font>";
    qzOS.replace("$1", msgAbout.os().c_str());
    QString qzPlatform = "<font family=\"Segoe Ui\" size=\"14\" color=\"white\">$1</font>";
    qzPlatform.replace("$1", msgAbout.platform().c_str());
    QString qzVersion = "<font family=\"Segoe Ui\" size=\"14\" color=\"white\">Version: $1</font>";
    qzVersion.replace("$1", msgAbout.version().c_str());

    qzAboutContent.replace("$1", qzApplicationName);
    qzAboutContent.replace("$2", qzCompany);
    qzAboutContent.replace("$3", qzAuthor);
    qzAboutContent.replace("$4", qzOS + QString(" ") + qzPlatform);
    qzAboutContent.replace("$5", qzVersion);

    ui->aboutContentTBrowser->clear();
    ui->aboutContentTBrowser->setHtml(qzAboutContent);
}

