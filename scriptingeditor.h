#ifndef SCRIPTINGEDITOR_H
#define SCRIPTINGEDITOR_H

#include <QDialog>
#include <QTextCursor>
#include <QList>
#include <QTextEdit>

#include "scriptingengine.h"
#include "scriptingdocumentation.h"

namespace Ui {
class ScriptingEditor;
}

class ScriptingEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptingEditor(QWidget *parent = nullptr, ScriptingEngine *scriptingEngine = nullptr);
    ~ScriptingEditor();

private slots:
    void on_btExecute_clicked();
    void on_btClearEditor_clicked();
    void on_btCopyConsole_clicked();
    void on_btClearConsole_clicked();
    void on_btDocumentation_clicked();
    void on_btSaveScript_clicked();
    void on_btLoadScript_clicked();
    void onInsertExample(const QString &example);
    void onCursorPositionChanged();

private:
    Ui::ScriptingEditor *ui;
    ScriptingEngine *scriptingEngine;
    
    // Brace matching
    void highlightMatchingBraces();
    int findMatchingBrace(const QString &text, int pos, QChar currentChar, QChar matchChar, bool forward);
};

#endif // SCRIPTINGEDITOR_H
