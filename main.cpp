#include "mainwindow.h"
#include "dlgdisclaimer.h"
#include <QApplication>
#include <QStyleFactory>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>

#include <QCommandLineParser>
#include <QLibrary>

#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("VórticeSoft");
    QCoreApplication::setOrganizationDomain("vorticesoft.com");
    QApplication::setApplicationName("MARRTA");
    QApplication::setApplicationVersion(APP_VERSION);

    // Use Windows Vista style on Windows for blue selection colors
    // On other platforms, use native style
    #ifdef Q_OS_WIN
        QApplication::setStyle(QStyleFactory::create("WindowsVista"));
    #endif

    #ifdef Q_OS_MACOS
        QString settingsFilename = QCoreApplication::applicationDirPath() + "/../../../marrta.ini";
    #else
        QString settingsFilename = QCoreApplication::applicationDirPath() + "/marrta.ini";
    #endif

    QSettings *qsettings = new QSettings(settingsFilename, QSettings::IniFormat);
    QString language = qsettings->value("language", "es").toString();

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QObject::tr("modelo"), QObject::tr("Ruta al archivo de modelo con el que trabajar"));

    QCommandLineOption verboseOption(QObject::tr("verboso"), QObject::tr("Mostrar información adicional durante la ejecución."));
    parser.addOption(verboseOption);

    parser.process(a);

    const QStringList args = parser.positionalArguments();
    const bool verbose = parser.isSet(verboseOption);

    if (verbose) {
        std::cout << "Inicializando MARRTA versión " << APP_VERSION << std::endl;
    }

    // Check if this is the first run and show disclaimer
    bool disclaimerAccepted = qsettings->value("disclaimerAccepted", false).toBool();
    if (!disclaimerAccepted) {
        DlgDisclaimer disclaimerDlg(nullptr);
        
        // Customize button labels for first run
        QDialogButtonBox *buttonBox = disclaimerDlg.findChild<QDialogButtonBox*>("buttonBox");
        if (buttonBox) {
            QPushButton *acceptButton = buttonBox->button(QDialogButtonBox::Ok);
            QPushButton *rejectButton = buttonBox->button(QDialogButtonBox::Cancel);
            if (acceptButton) {
                acceptButton->setText(QObject::tr("Acepto"));
            }
            if (rejectButton) {
                rejectButton->setText(QObject::tr("No acepto"));
            }
        }
        
        int result = disclaimerDlg.exec();
        
        if (result == QDialog::Rejected) {
            // User rejected the disclaimer, exit the application
            return 0;
        }
        
        // User accepted, save this to settings
        qsettings->setValue("disclaimerAccepted", true);
        qsettings->sync();
    }

    MainWindow w;
    w.loadSettings(qsettings);
    // If verbose mode is set in command line, this is saved for the next session
    if (verbose) {
        w.settings.debugMode = true;
    }

    QString praxisToOpen = "";

    // Only open file if provided via command line argument
    // Don't automatically open the last used file to avoid issues with
    // multiple instances or moved files
    if (args.size() == 1) {
        praxisToOpen = args.at(0);
    }

    if (praxisToOpen != "") {
        w.openPraxisFile(praxisToOpen);
    }
    w.show();

    return a.exec();
}
