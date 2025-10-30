#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

#include <QStandardItemModel>

#include <QJSEngine>

#include "adapters/consoleadapter.h"
#include "adapters/eventadapter.h"
#include "adapters/defenseadapter.h"
#include "adapters/chartadapter.h"
#include "adapters/stageadapter.h"

class MainWindow;

class ScriptingEngine
{
private:
    QJSEngine jsEngine;
    MainWindow *mainWindow;

    // Objects to interact with
    ConsoleAdapter *consoleAdapter;
    EventAdapter *eventAdapter;
    DefenseAdapter *defenseAdapter;
    ChartAdapter *chartAdapter;
    StageAdapter *stageAdapter;

    // Global objects with data
    QStandardItemModel *currentDb;

public:
    ScriptingEngine(MainWindow *);

    QString execute(QString code);

    void setCurrentDb(QStandardItemModel *db);
    void setConsoleDestiny(QPlainTextEdit *destiny);
};

#endif // SCRIPTINGENGINE_H
