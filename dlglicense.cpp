#include "dlglicense.h"
#include "ui_dlglicense.h"
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>

DlgLicense::DlgLicense(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLicense)
{
    ui->setupUi(this);
    setWindowTitle(tr("Licencia de uso - GPLv3"));
    
    // Set the license summary text
    QString licenseText = tr(
        "<h2>Licencia GNU General Public License v3.0 (GPLv3)</h2>"
        "<p><strong>MARRTA</strong> se distribuye bajo los términos de la Licencia Pública General de GNU versión 3 (GPLv3).</p>"
        "<h3>Resumen de la licencia:</h3>"
        "<ul>"
        "<li><strong>Libertad de uso:</strong> Puede usar este software para cualquier propósito, sin restricciones.</li>"
        "<li><strong>Libertad de estudio:</strong> Tiene acceso al código fuente completo y puede estudiarlo y modificarlo según sus necesidades.</li>"
        "<li><strong>Libertad de distribución:</strong> Puede distribuir copias del software original a otras personas.</li>"
        "<li><strong>Libertad de mejora:</strong> Puede modificar el software y distribuir versiones mejoradas.</li>"
        "</ul>"
        "<h3>Condiciones importantes:</h3>"
        "<ul>"
        "<li><strong>Copyleft:</strong> Si distribuye este software o trabajos derivados del mismo, debe hacerlo bajo la misma licencia GPLv3.</li>"
        "<li><strong>Código fuente:</strong> Cualquier versión modificada o derivada que distribuya debe incluir el código fuente completo o proporcionar un medio para obtenerlo.</li>"
        "<li><strong>Aviso de licencia:</strong> Debe mantener todos los avisos de copyright y licencia en el código fuente.</li>"
        "<li><strong>Sin garantías:</strong> El software se proporciona \"tal cual\", sin garantías de ningún tipo.</li>"
        "</ul>"
        "<p>Haga clic en el botón inferior para acceder al texto completo de la licencia en su idioma preferido.</p>"
    );
    
    ui->textBrowser->setHtml(licenseText);
    
    // Update button text based on language
    QString language = getCurrentLanguage();
    ui->btnLicenseLink->setText(tr("Ver licencia completa en %1").arg(language.toUpper()));
}

DlgLicense::~DlgLicense()
{
    delete ui;
}

QString DlgLicense::getCurrentLanguage()
{
    #ifdef Q_OS_MACOS
        QString settingsFilename = QCoreApplication::applicationDirPath() + "/../../../marrta.ini";
    #else
        QString settingsFilename = QCoreApplication::applicationDirPath() + "/marrta.ini";
    #endif
    
    QSettings settings(settingsFilename, QSettings::IniFormat);
    QString language = settings.value("language", "es").toString();
    
    // Map to full language names for display
    if (language == "es") return "Español";
    if (language == "en") return "English";
    if (language == "fr") return "Français";
    if (language == "pt") return "Português";
    
    return "Español"; // Default
}

QString DlgLicense::getLicenseUrl(const QString& language)
{
    // Map language codes to GNU GPL license URLs
    if (language == "Español" || language == "es") {
        return "https://www.gnu.org/licenses/gpl-3.0.es.html";
    }
    if (language == "English" || language == "en") {
        return "https://www.gnu.org/licenses/gpl-3.0.en.html";
    }
    if (language == "Français" || language == "fr") {
        return "https://www.gnu.org/licenses/gpl-3.0.fr.html";
    }
    if (language == "Português" || language == "pt") {
        // GNU doesn't have official PT translation, use English
        return "https://www.gnu.org/licenses/gpl-3.0.en.html";
    }
    
    // Default to English
    return "https://www.gnu.org/licenses/gpl-3.0.en.html";
}

void DlgLicense::on_btnLicenseLink_clicked()
{
    QString language = getCurrentLanguage();
    QString url = getLicenseUrl(language);
    QDesktopServices::openUrl(QUrl(url));
}
