#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent, QString version) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    QString strEtVersion = "";
    if (version != "") {
        strEtVersion = "Versión " + version;
        // Add build number (git commit hash) if available
        // If GIT_COMMIT is not defined, the version string will not include the build number.
        #ifdef GIT_COMMIT
            strEtVersion += " [" + QString(GIT_COMMIT) + "]";
        #endif
    }
    ui->lVersion->setText(strEtVersion);
    
    // Make version label clickable and open hyperlink
    ui->lVersion->setTextFormat(Qt::RichText);
    ui->lVersion->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->lVersion->setOpenExternalLinks(true);
    QString versionWithLink = QString("<a href=\"https://github.com/risk-marrta/MARRTA/releases\" style=\"color: black; text-decoration: none;\">%1</a>").arg(strEtVersion);
    ui->lVersion->setText(versionWithLink);
}

About::~About()
{
    delete ui;
}
