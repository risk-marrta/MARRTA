#include "scriptingeditor.h"
#include "ui_scriptingeditor.h"
#include <QClipboard>
#include <QApplication>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "javascript_highlighter.h"

ScriptingEditor::ScriptingEditor(QWidget *parent, ScriptingEngine *scriptingEngine) :
    QDialog(parent),
    ui(new Ui::ScriptingEditor)
{
    ui->setupUi(this);

    this->scriptingEngine = scriptingEngine;
    this->scriptingEngine->setConsoleDestiny(ui->result);
    if (this->scriptingEngine == nullptr) {
        ui->btExecute->setEnabled(false);
        ui->editor->setReadOnly(true);
        ui->result->appendPlainText("Entorno Javascript no habilitado");
    }

    // Apply JavaScript syntax highlighting
    new JavaScriptHighlighter(ui->editor->document());
    
    // Set monospaced font for editor and console
    QFont monoFont("Courier New", 10);
    monoFont.setStyleHint(QFont::Monospace);
    ui->editor->setFont(monoFont);
    ui->result->setFont(monoFont);
    
    // Connect cursor position change for brace matching
    connect(ui->editor, &QPlainTextEdit::cursorPositionChanged, 
            this, &ScriptingEditor::onCursorPositionChanged);
}

ScriptingEditor::~ScriptingEditor()
{
    delete ui;
}

void ScriptingEditor::on_btExecute_clicked()
{
    QString code = ui->editor->toPlainText();
    if (code.size() > 0) {
        ui->result->appendPlainText(this->scriptingEngine->execute(code));
    }
}

void ScriptingEditor::on_btClearEditor_clicked()
{
    ui->editor->clear();
}

void ScriptingEditor::on_btCopyConsole_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->result->toPlainText());
}

void ScriptingEditor::on_btClearConsole_clicked()
{
    ui->result->clear();
}

void ScriptingEditor::on_btDocumentation_clicked()
{
    ScriptingDocumentation *doc = new ScriptingDocumentation(this);
    connect(doc, &ScriptingDocumentation::insertExample, this, &ScriptingEditor::onInsertExample);
    doc->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed
    doc->show(); // Non-blocking
}

void ScriptingEditor::onInsertExample(const QString &example)
{
    ui->editor->insertPlainText(example);
}

void ScriptingEditor::on_btSaveScript_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Guardar Script"),
                                                    "",
                                                    tr("JavaScript Files (*.js);;All Files (*)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), 
                           tr("No se pudo guardar el archivo:\n%1").arg(file.errorString()));
        return;
    }
    
    QTextStream out(&file);
    out << ui->editor->toPlainText();
    file.close();
    
    ui->result->appendPlainText(tr("Script guardado en: %1").arg(fileName));
}

void ScriptingEditor::on_btLoadScript_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Cargar Script"),
                                                    "",
                                                    tr("JavaScript Files (*.js);;All Files (*)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), 
                           tr("No se pudo cargar el archivo:\n%1").arg(file.errorString()));
        return;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    ui->editor->setPlainText(content);
    ui->result->appendPlainText(tr("Script cargado desde: %1").arg(fileName));
}

void ScriptingEditor::onCursorPositionChanged()
{
    highlightMatchingBraces();
}

void ScriptingEditor::highlightMatchingBraces()
{
    QList<QTextEdit::ExtraSelection> selections;
    QTextCursor cursor = ui->editor->textCursor();
    
    if (!cursor.hasSelection()) {
        int pos = cursor.position();
        QString text = ui->editor->toPlainText();
        
        // Try to match character before cursor or after cursor
        int charPos = -1;
        QChar currentChar;
        QChar matchChar;
        bool forward = false;
        
        // Check character before cursor
        if (pos > 0) {
            QChar charBefore = text[pos - 1];
            if (charBefore == '{' || charBefore == '}' || charBefore == '[' || 
                charBefore == ']' || charBefore == '(' || charBefore == ')') {
                charPos = pos - 1;
                currentChar = charBefore;
            }
        }
        
        // If no bracket before cursor, check character after cursor
        if (charPos == -1 && pos < text.length()) {
            QChar charAfter = text[pos];
            if (charAfter == '{' || charAfter == '}' || charAfter == '[' || 
                charAfter == ']' || charAfter == '(' || charAfter == ')') {
                charPos = pos;
                currentChar = charAfter;
            }
        }
        
        // If we found a bracket, determine its match
        if (charPos != -1) {
            if (currentChar == '{') {
                matchChar = '}';
                forward = true;
            } else if (currentChar == '}') {
                matchChar = '{';
                forward = false;
            } else if (currentChar == '[') {
                matchChar = ']';
                forward = true;
            } else if (currentChar == ']') {
                matchChar = '[';
                forward = false;
            } else if (currentChar == '(') {
                matchChar = ')';
                forward = true;
            } else if (currentChar == ')') {
                matchChar = '(';
                forward = false;
            }
            
            int matchPos = findMatchingBrace(text, charPos, currentChar, matchChar, forward);
            
            // Highlight matching braces if found
            if (matchPos != -1) {
                QTextEdit::ExtraSelection selection1;
                QTextCursor cursor1 = ui->editor->textCursor();
                cursor1.setPosition(charPos);
                cursor1.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                selection1.cursor = cursor1;
                selection1.format.setBackground(QColor(220, 220, 220)); // Light grey
                selection1.format.setForeground(Qt::black);
                selections.append(selection1);
                
                QTextEdit::ExtraSelection selection2;
                QTextCursor cursor2 = ui->editor->textCursor();
                cursor2.setPosition(matchPos);
                cursor2.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
                selection2.cursor = cursor2;
                selection2.format.setBackground(QColor(220, 220, 220)); // Light grey
                selection2.format.setForeground(Qt::black);
                selections.append(selection2);
            }
        }
    }
    
    ui->editor->setExtraSelections(selections);
}

int ScriptingEditor::findMatchingBrace(const QString &text, int pos, QChar currentChar, QChar matchChar, bool forward)
{
    int depth = 1;
    int step = forward ? 1 : -1;
    int i = pos + step;
    
    while (i >= 0 && i < text.length()) {
        QChar c = text[i];
        
        // If we find the same character as we started with, increase depth
        if (c == currentChar) {
            depth++;
        } 
        // If we find the matching character, decrease depth
        else if (c == matchChar) {
            depth--;
            if (depth == 0) {
                return i;
            }
        }
        
        i += step;
    }
    
    return -1; // No match found
}

