#include "scriptingengine.h"

#include <iostream>
#include "mainwindow.h"
#include "adapters/eventadapter.h"
#include "adapters/defenseadapter.h"
#include "adapters/chartadapter.h"
#include "adapters/stageadapter.h"

ScriptingEngine::ScriptingEngine(MainWindow *mainWindow)
{
    jsEngine.installExtensions(QJSEngine::ConsoleExtension);
    jsEngine.globalObject().setProperty("db", jsEngine.newObject());

    consoleAdapter = nullptr;
    eventAdapter = nullptr;
    defenseAdapter = nullptr;
    chartAdapter = nullptr;
    stageAdapter = nullptr;

    this->mainWindow = mainWindow;

    jsEngine.globalObject().setProperty("Marrta", jsEngine.newQObject(this->mainWindow));
    
    // Initialize new adapters
    eventAdapter = new EventAdapter(this->mainWindow);
    defenseAdapter = new DefenseAdapter(this->mainWindow);
    chartAdapter = new ChartAdapter(this->mainWindow);
    stageAdapter = new StageAdapter(this->mainWindow);
    
    // Register adapters in JavaScript
    jsEngine.globalObject().setProperty("Events", jsEngine.newQObject(eventAdapter));
    jsEngine.globalObject().setProperty("Defenses", jsEngine.newQObject(defenseAdapter));
    jsEngine.globalObject().setProperty("Charts", jsEngine.newQObject(chartAdapter));
    jsEngine.globalObject().setProperty("Stages", jsEngine.newQObject(stageAdapter));
}

QString ScriptingEngine::execute(QString code)
{
    QJSValue result = jsEngine.evaluate(code);
    return result.toString();
}

void ScriptingEngine::setCurrentDb(QStandardItemModel *db)
{
    currentDb = db;

    jsEngine.globalObject().setProperty("db", jsEngine.newQObject(db));
}


void ScriptingEngine::setConsoleDestiny(QPlainTextEdit *destiny) {
    if (consoleAdapter != nullptr) {
        delete consoleAdapter;
    }

    consoleAdapter = new ConsoleAdapter(destiny);
    jsEngine.globalObject().setProperty("console", jsEngine.newQObject(consoleAdapter));
}
