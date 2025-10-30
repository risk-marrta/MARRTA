#ifndef PYTHONSCRIPTINGEDITOR_H
#define PYTHONSCRIPTINGEDITOR_H

#include <QDialog>
#include <QTextCursor>
#include <QList>
#include <QTextEdit>

#include "pythonscriptingengine.h"
#include "pythonscriptingdocumentation.h"

namespace Ui {
class PythonScriptingEditor;
}

class PythonScriptingEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PythonScriptingEditor(QWidget *parent = nullptr, PythonScriptingEngine *scriptingEngine = nullptr);
    ~PythonScriptingEditor();

private slots:
    void on_btExecute_clicked();
    void on_btClearEditor_clicked();
    void on_btCopyConsole_clicked();
    void on_btClearConsole_clicked();
    void on_btDocumentation_clicked();
    void on_btSaveScript_clicked();
    void on_btLoadScript_clicked();
    void on_btFind_clicked();
    void on_btReplace_clicked();
    void on_btReplaceAll_clicked();
    void onInsertExample(const QString &example);
    void onCursorPositionChanged();

private:
    Ui::PythonScriptingEditor *ui;
    PythonScriptingEngine *scriptingEngine;
    
    // Brace matching
    void highlightMatchingBraces();
    int findMatchingBrace(const QString &text, int pos, QChar currentChar, QChar matchChar, bool forward);
    
    // Find/Replace functionality
    void findNext();
    void replaceNext();
    void replaceAll();
};

#endif // PYTHONSCRIPTINGEDITOR_H
