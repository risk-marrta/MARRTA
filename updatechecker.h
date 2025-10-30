#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDateTime>
#include <QString>
#include <QSettings>
#include <QList>

// GitHub API URLs for update checking
#define GITHUB_API_MARRTA_RELEASES "https://api.github.com/repos/risk-marrta/MARRTA/releases/latest"
#define GITHUB_API_MODELS_RELEASES "https://api.github.com/repos/risk-marrta/marrta-modelos-de-riesgo/releases"
#define GITHUB_MARRTA_RELEASES_PATH "risk-marrta/MARRTA/releases"
#define GITHUB_MODELS_RELEASES_PATH "risk-marrta/marrta-modelos-de-riesgo/releases"
#define GITHUB_MODELS_RELEASES_URL "https://github.com/risk-marrta/marrta-modelos-de-riesgo/releases"

/**
 * @brief The UpdateChecker class handles version and model checking from GitHub
 * 
 * This class checks for:
 * - New application releases in theotocopulitos/MARRTA
 * - New models in theotocopulitos/marrta-modelos-de-riesgo
 * 
 * Checks can be triggered:
 * - Manually by user request
 * - Automatically every 2 weeks on startup
 */
class UpdateChecker : public QObject
{
    Q_OBJECT

public:
    explicit UpdateChecker(QObject *parent = nullptr);
    ~UpdateChecker();

    /**
     * @brief Check for new application version
     * @param silent If true, only show notification if update is available
     */
    void checkForUpdates(bool silent = false);

    /**
     * @brief Check for new models in the models repository
     * @param silent If true, only show notification if new models are available
     */
    void checkForNewModels(bool silent = false);

    /**
     * @brief Check if automatic check should be performed (every 2 weeks)
     * @param settings Application settings
     * @return true if check should be performed
     */
    bool shouldAutoCheck(QSettings *settings);

    /**
     * @brief Perform automatic checks on startup if needed
     * @param settings Application settings
     */
    void autoCheckOnStartup(QSettings *settings);

signals:
    /**
     * @brief Emitted when a new version is available
     * @param version The new version string
     * @param releaseUrl URL to the release page
     */
    void newVersionAvailable(const QString &version, const QString &releaseUrl);

    /**
     * @brief Emitted when new models are available
     * @param count Number of new models
     * @param repoUrl URL to the models repository
     */
    void newModelsAvailable(int count, const QString &repoUrl);

    /**
     * @brief Emitted when no updates are found (only in non-silent mode)
     */
    void noUpdatesAvailable();

    /**
     * @brief Emitted when no new models are found (only in non-silent mode)
     */
    void noNewModels();

    /**
     * @brief Emitted when an error occurs during checking
     * @param message Error message
     */
    void checkError(const QString &message);

private slots:
    void onVersionCheckFinished(QNetworkReply *reply);
    void onModelsCheckFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    bool silentVersionCheck;
    bool silentModelsCheck;

    QString currentVersion;
    QSettings *settings;

    /**
     * @brief Extract version number from version string
     * @param versionStr Version string like "1.4.4 (2025-10-07)"
     * @return Version components as list [major, minor, patch]
     */
    QList<int> parseVersion(const QString &versionStr);

    /**
     * @brief Compare two version numbers
     * @param current Current version
     * @param latest Latest version from GitHub
     * @return true if latest is newer than current
     */
    bool isNewerVersion(const QString &current, const QString &latest);

    /**
     * @brief Update last check date in settings
     * @param key Settings key
     */
    void updateLastCheckDate(const QString &key);

    /**
     * @brief Get days since last check
     * @param key Settings key
     * @return Number of days since last check, or -1 if never checked
     */
    int daysSinceLastCheck(const QString &key);
};

#endif // UPDATECHECKER_H
