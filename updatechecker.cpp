#include "updatechecker.h"
#include "mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QSslError>

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
    , silentVersionCheck(false)
    , silentModelsCheck(false)
    , settings(nullptr)
{
    // Extract version number from APP_VERSION
    currentVersion = APP_VERSION;
    
    connect(networkManager, &QNetworkAccessManager::finished,
            this, [this](QNetworkReply *reply) {
        // Determine which check this reply is for based on the URL
        QString url = reply->request().url().toString();
        if (url.contains(GITHUB_MARRTA_RELEASES_PATH)) {
            onVersionCheckFinished(reply);
        } else if (url.contains(GITHUB_MODELS_RELEASES_PATH)) {
            onModelsCheckFinished(reply);
        }
    });
    
    // Handle SSL errors (log but don't block - for self-signed certificates in development)
    connect(networkManager, &QNetworkAccessManager::sslErrors,
            this, [](QNetworkReply *reply, const QList<QSslError> &errors) {
        qWarning() << "SSL errors encountered:";
        for (const QSslError &error : errors) {
            qWarning() << "  -" << error.errorString();
        }
        // Don't ignore SSL errors by default - security first
        // If needed in development, user can temporarily uncomment the line below
        // reply->ignoreSslErrors();
    });
}

UpdateChecker::~UpdateChecker()
{
    // networkManager is deleted automatically by parent
}

void UpdateChecker::checkForUpdates(bool silent)
{
    silentVersionCheck = silent;
    
    // GitHub API endpoint for releases
    QUrl url(GITHUB_API_MARRTA_RELEASES);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "MARRTA-UpdateChecker");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");
    
    qDebug() << "Checking for updates..." << (silent ? "(silent)" : "(user requested)");
    networkManager->get(request);
}

void UpdateChecker::checkForNewModels(bool silent)
{
    silentModelsCheck = silent;
    
    // GitHub API endpoint for releases in models repository
    QUrl url(GITHUB_API_MODELS_RELEASES);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "MARRTA-UpdateChecker");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");
    
    qDebug() << "Checking for new models..." << (silent ? "(silent)" : "(user requested)");
    networkManager->get(request);
}

bool UpdateChecker::shouldAutoCheck(QSettings *settings)
{
    this->settings = settings;
    
    // Check if 2 weeks (14 days) have passed since last check
    int daysSinceVersion = daysSinceLastCheck("lastVersionCheck");
    int daysSinceModels = daysSinceLastCheck("lastModelsCheck");
    
    // Return true if either check is due (>= 14 days or never checked)
    return (daysSinceVersion < 0 || daysSinceVersion >= 14) ||
           (daysSinceModels < 0 || daysSinceModels >= 14);
}

void UpdateChecker::autoCheckOnStartup(QSettings *settings)
{
    this->settings = settings;
    
    int daysSinceVersion = daysSinceLastCheck("lastVersionCheck");
    int daysSinceModels = daysSinceLastCheck("lastModelsCheck");
    
    qDebug() << "Auto-check on startup: days since version check:" << daysSinceVersion
             << "days since models check:" << daysSinceModels;
    
    // Check for version updates if 14 days have passed or never checked
    if (daysSinceVersion < 0 || daysSinceVersion >= 14) {
        checkForUpdates(true); // Silent check
    }
    
    // Check for new models if 14 days have passed or never checked
    if (daysSinceModels < 0 || daysSinceModels >= 14) {
        checkForNewModels(true); // Silent check
    }
}

void UpdateChecker::onVersionCheckFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        QString errorDetails = reply->errorString();
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QByteArray responseData = reply->readAll();
        
        qWarning() << "Error checking for updates:" << errorDetails;
        qWarning() << "HTTP Status Code:" << httpStatus;
        qWarning() << "Response:" << responseData;
        
        if (!silentVersionCheck) {
            QString userMessage = tr("No se pudo comprobar actualizaciones: %1").arg(errorDetails);
            if (httpStatus > 0) {
                userMessage += tr("\n\nCódigo HTTP: %1").arg(httpStatus);
            }
            emit checkError(userMessage);
        }
        return;
    }
    
    // Update last check date
    if (settings) {
        updateLastCheckDate("lastVersionCheck");
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    
    if (!jsonDoc.isObject()) {
        qWarning() << "Invalid JSON response from GitHub API";
        if (!silentVersionCheck) {
            emit checkError(tr("Respuesta inválida del servidor"));
        }
        return;
    }
    
    QJsonObject jsonObj = jsonDoc.object();
    QString latestVersion = jsonObj["tag_name"].toString();
    QString releaseUrl = jsonObj["html_url"].toString();
    QString releaseName = jsonObj["name"].toString();
    QString publishedAt = jsonObj["published_at"].toString();
    
    qDebug() << "Latest version from GitHub:" << latestVersion << "Current:" << currentVersion;
    
    if (latestVersion.isEmpty()) {
        qWarning() << "Could not parse version from response";
        if (!silentVersionCheck) {
            emit checkError(tr("No se pudo obtener información de versión"));
        }
        return;
    }
    
    // Remove 'v' prefix if present
    if (latestVersion.startsWith('v') || latestVersion.startsWith('V')) {
        latestVersion = latestVersion.mid(1);
    }
    
    // Extract and format date from published_at (ISO format: "2025-01-15T10:30:45Z")
    QString formattedDate;
    if (!publishedAt.isEmpty()) {
        QDateTime releaseDate = QDateTime::fromString(publishedAt, Qt::ISODate);
        if (releaseDate.isValid()) {
            formattedDate = releaseDate.toString("yyyy-MM-dd");
        }
    }
    
    if (isNewerVersion(currentVersion, latestVersion)) {
        qDebug() << "New version available:" << latestVersion;
        
        // Show message box with update information
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Nueva versión disponible   "));
        
        // Main message
        msgBox.setText(tr("Hay una nueva versión de MARRTA disponible          "));
        
        // Format version with date on same line, like current version format
        QString newVersionLine = latestVersion;
        if (!formattedDate.isEmpty()) {
            newVersionLine += " (" + formattedDate + ")";
        }
        
        QString detailedInfo = tr("Nueva versión: %1").arg(newVersionLine);
        detailedInfo += "\n" + tr("Versión actual: %1").arg(currentVersion);
        detailedInfo += "\n\n\n" + tr("¿Desea visitar la página de descarga?");
        msgBox.setInformativeText(detailedInfo);
        
        // Spanish buttons
        QPushButton *yesButton = msgBox.addButton(tr("Sí"), QMessageBox::YesRole);
        msgBox.addButton(tr("No"), QMessageBox::NoRole);
        msgBox.setDefaultButton(yesButton);
        
        msgBox.exec();
        if (msgBox.clickedButton() == yesButton) {
            QDesktopServices::openUrl(QUrl(releaseUrl));
        }
        
        emit newVersionAvailable(latestVersion, releaseUrl);
    } else {
        qDebug() << "No new version available";
        if (!silentVersionCheck) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Sin actualizaciones"));
            msgBox.setText(tr("No hay nuevas versiones disponibles desde la última comprobación.     "));
            msgBox.setInformativeText(tr("Versión actual: %1").arg(currentVersion));
            msgBox.addButton(tr("Aceptar"), QMessageBox::AcceptRole);
            msgBox.exec();
            emit noUpdatesAvailable();
        }
    }
}

void UpdateChecker::onModelsCheckFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        QString errorDetails = reply->errorString();
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QByteArray responseData = reply->readAll();
        
        qWarning() << "Error checking for new models:" << errorDetails;
        qWarning() << "HTTP Status Code:" << httpStatus;
        qWarning() << "Response:" << responseData;
        
        if (!silentModelsCheck) {
            QString userMessage = tr("No se pudo comprobar nuevos modelos: %1").arg(errorDetails);
            if (httpStatus > 0) {
                userMessage += tr("\n\nCódigo HTTP: %1").arg(httpStatus);
            }
            emit checkError(userMessage);
        }
        return;
    }
    
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    
    if (!jsonDoc.isArray()) {
        qWarning() << "Invalid JSON response from GitHub API (expected array)";
        if (!silentModelsCheck) {
            emit checkError(tr("Respuesta inválida del servidor"));
        }
        return;
    }
    
    QJsonArray releases = jsonDoc.array();
    
    // Get the date of the last check (or use a very old date if never checked)
    QDateTime lastCheck;
    if (settings && settings->contains("lastModelsCheck")) {
        lastCheck = settings->value("lastModelsCheck").toDateTime();
    } else {
        lastCheck = QDateTime::fromString("2000-01-01", Qt::ISODate);
    }
    
    // Update last check date
    if (settings) {
        updateLastCheckDate("lastModelsCheck");
    }
    
    // Count releases published after last check
    int newModelsCount = 0;
    QStringList newModelsList;
    
    for (const QJsonValue &value : releases) {
        QJsonObject release = value.toObject();
        QString publishedAt = release["published_at"].toString();
        QString name = release["name"].toString();
        QString tagName = release["tag_name"].toString();
        
        QDateTime releaseDate = QDateTime::fromString(publishedAt, Qt::ISODate);
        
        if (releaseDate > lastCheck) {
            newModelsCount++;
            newModelsList.append(name.isEmpty() ? tagName : name);
        }
    }
    
    qDebug() << "New models found:" << newModelsCount;
    
    if (newModelsCount > 0) {
        QString repoUrl = GITHUB_MODELS_RELEASES_URL;
        
        // Show message box with new models information
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Nuevos modelos disponibles"));
        
        QString message;
        if (newModelsCount == 1) {
            message = tr("Hay un nuevo modelo disponible en el repositorio:");
        } else {
            message = tr("Hay %1 nuevos modelos disponibles en el repositorio:").arg(newModelsCount);
        }
        
        message += "\n\n";
        for (int i = 0; i < qMin(5, newModelsList.size()); i++) {
            message += "• " + newModelsList[i] + "\n";
        }
        
        if (newModelsList.size() > 5) {
            message += tr("... y %1 más\n").arg(newModelsList.size() - 5);
        }
        
        message += tr("\n¿Desea visitar la página de descargas?");
        
        msgBox.setText(message);
        
        // Spanish buttons
        QPushButton *yesButton = msgBox.addButton(tr("Sí"), QMessageBox::YesRole);
        msgBox.addButton(tr("No"), QMessageBox::NoRole);
        msgBox.setDefaultButton(yesButton);
        
        msgBox.exec();
        if (msgBox.clickedButton() == yesButton) {
            QDesktopServices::openUrl(QUrl(repoUrl));
        }
        
        emit newModelsAvailable(newModelsCount, repoUrl);
    } else {
        qDebug() << "No new models available";
        if (!silentModelsCheck) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Sin nuevos modelos"));
            msgBox.setText(tr("No hay nuevos modelos disponibles desde la última comprobación.       "));
            msgBox.addButton(tr("Aceptar"), QMessageBox::AcceptRole);
            msgBox.exec();
            emit noNewModels();
        }
    }
}

QList<int> UpdateChecker::parseVersion(const QString &versionStr)
{
    QList<int> components;
    
    // Extract just the version number (e.g., "1.4.4" from "1.4.4 (2025-10-07)")
    QString version = versionStr;
    int parenIndex = version.indexOf('(');
    if (parenIndex > 0) {
        version = version.left(parenIndex).trimmed();
    }
    
    // Remove 'v' prefix if present
    if (version.startsWith('v') || version.startsWith('V')) {
        version = version.mid(1);
    }
    
    QStringList parts = version.split('.');
    for (const QString &part : parts) {
        bool ok;
        int num = part.toInt(&ok);
        if (ok) {
            components.append(num);
        } else {
            components.append(0);
        }
    }
    
    // Ensure we have at least 3 components
    while (components.size() < 3) {
        components.append(0);
    }
    
    return components;
}

bool UpdateChecker::isNewerVersion(const QString &current, const QString &latest)
{
    QList<int> currentVer = parseVersion(current);
    QList<int> latestVer = parseVersion(latest);
    
    // Compare major.minor.patch
    for (int i = 0; i < 3; i++) {
        if (latestVer[i] > currentVer[i]) {
            return true;
        } else if (latestVer[i] < currentVer[i]) {
            return false;
        }
    }
    
    // Versions are equal
    return false;
}

void UpdateChecker::updateLastCheckDate(const QString &key)
{
    if (settings) {
        settings->setValue(key, QDateTime::currentDateTime());
        settings->sync();
        qDebug() << "Updated" << key << "to" << QDateTime::currentDateTime();
    }
}

int UpdateChecker::daysSinceLastCheck(const QString &key)
{
    if (!settings || !settings->contains(key)) {
        return -1; // Never checked
    }
    
    QDateTime lastCheck = settings->value(key).toDateTime();
    QDateTime now = QDateTime::currentDateTime();
    
    return lastCheck.daysTo(now);
}
