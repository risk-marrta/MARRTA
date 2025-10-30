#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QStringList>

struct Settings {
    bool    debugMode = false;
    bool    createBackupOnOpen = false;
    bool    disclaimerAccepted = false;  // Track if user accepted disclaimer on first run
    QString lastFile = "";  // Keep for backward compatibility
    QStringList recentFiles;  // List of recently opened files
    QString reportFolder = "";
    QString databaseFolder = "";
};

#endif // SETTINGS_H
