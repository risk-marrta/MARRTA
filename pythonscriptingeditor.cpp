#include "pythonscriptingeditor.h"
#include "ui_pythonscriptingeditor.h"
#include <QClipboard>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "python_highlighter.h"

PythonScriptingEditor::PythonScriptingEditor(QWidget *parent, PythonScriptingEngine *scriptingEngine) :
    QDialog(parent),
    ui(new Ui::PythonScriptingEditor)
{
    ui->setupUi(this);

    this->scriptingEngine = scriptingEngine;
    if (this->scriptingEngine != nullptr) {
        this->scriptingEngine->setConsoleDestiny(ui->result);
    }
    
    if (this->scriptingEngine == nullptr || !this->scriptingEngine->isInitialized()) {
        ui->btExecute->setEnabled(false);
        ui->editor->setReadOnly(true);
        ui->result->appendPlainText(tr("Entorno Python no habilitado"));
        
        // Show detailed error message with installation instructions
        QString errorMsg = tr("Python No Disponible");
        QString detailedMsg;
        
        if (this->scriptingEngine != nullptr) {
            QString initError = this->scriptingEngine->pythonInitializationError();
            detailedMsg = tr("El intérprete de Python no pudo ser inicializado.\n\n"
                           "Para utilizar el editor de scripts de Python, necesita:\n\n"
                           "1. Instalar Python 3.12 (versión compatible)\n"
                           "   - Descargue desde: https://www.python.org/downloads/\n"
                           "   - Durante la instalación, marque la opción 'Add Python to PATH'\n\n"
                           "2. Asegurarse de que python312.dll está disponible en:\n"
                           "   - La carpeta de instalación de Python\n"
                           "   - O junto al ejecutable de MARRTA\n\n"
                           "3. Verificar que la carpeta 'Lib' de Python está presente\n\n"
                           "Error técnico: %1").arg(initError.isEmpty() ? tr("Desconocido") : initError);
        } else {
            detailedMsg = tr("El motor de scripting de Python no está disponible.\n\n"
                           "Python 3.12 es requerido para esta funcionalidad.\n"
                           "Por favor, instale Python desde https://www.python.org/downloads/");
        }
        
        QMessageBox::warning(this, errorMsg, detailedMsg);
    }

    // Apply Python syntax highlighting
    new PythonHighlighter(ui->editor->document());
    
    // Set monospaced font for editor and console
    QFont monoFont("Courier New", 10);
    monoFont.setStyleHint(QFont::Monospace);
    ui->editor->setFont(monoFont);
    ui->result->setFont(monoFont);
    
    // Connect cursor position change for brace matching
    connect(ui->editor, &QPlainTextEdit::cursorPositionChanged, 
            this, &PythonScriptingEditor::onCursorPositionChanged);
}

PythonScriptingEditor::~PythonScriptingEditor()
{
    delete ui;
}

void PythonScriptingEditor::on_btExecute_clicked()
{
    QString code = ui->editor->toPlainText();
    if (code.size() > 0) {
        ui->result->appendPlainText(this->scriptingEngine->execute(code));
    }
}

void PythonScriptingEditor::on_btClearEditor_clicked()
{
    ui->editor->clear();
}

void PythonScriptingEditor::on_btCopyConsole_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->result->toPlainText());
}

void PythonScriptingEditor::on_btClearConsole_clicked()
{
    ui->result->clear();
}

void PythonScriptingEditor::on_btDocumentation_clicked()
{
    PythonScriptingDocumentation *doc = new PythonScriptingDocumentation(this);
    connect(doc, &PythonScriptingDocumentation::insertExample, this, &PythonScriptingEditor::onInsertExample);
    doc->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed
    doc->show(); // Non-blocking
}

void PythonScriptingEditor::onInsertExample(const QString &example)
{
    ui->editor->insertPlainText(example);
}

void PythonScriptingEditor::on_btSaveScript_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Guardar Script"),
                                                    "",
                                                    tr("Python Files (*.py);;All Files (*)"));
    
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

void PythonScriptingEditor::on_btLoadScript_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Cargar Script"),
                                                    "",
                                                    tr("Python Files (*.py);;All Files (*)"));
    
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

void PythonScriptingEditor::on_btFind_clicked()
{
    findNext();
}

void PythonScriptingEditor::on_btReplace_clicked()
{
    replaceNext();
}

void PythonScriptingEditor::on_btReplaceAll_clicked()
{
    replaceAll();
}

void PythonScriptingEditor::findNext()
{
    QString searchText = ui->edFind->text();
    if (searchText.isEmpty()) {
        return;
    }
    
    QTextDocument::FindFlags flags;
    if (ui->cbCaseSensitive->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (ui->cbWholeWords->isChecked()) {
        flags |= QTextDocument::FindWholeWords;
    }
    
    bool found = ui->editor->find(searchText, flags);
    
    if (!found) {
        // Wrap around to beginning
        QTextCursor cursor = ui->editor->textCursor();
        cursor.movePosition(QTextCursor::Start);
        ui->editor->setTextCursor(cursor);
        found = ui->editor->find(searchText, flags);
        
        if (!found) {
            ui->result->appendPlainText(tr("No se encontró: %1").arg(searchText));
        }
    }
}

void PythonScriptingEditor::replaceNext()
{
    QString searchText = ui->edFind->text();
    QString replaceText = ui->edReplace->text();
    
    if (searchText.isEmpty()) {
        return;
    }
    
    QTextCursor cursor = ui->editor->textCursor();
    
    // Check if current selection matches the search text
    QString selectedText = cursor.selectedText();
    bool matches = (ui->cbCaseSensitive->isChecked()) 
                   ? (selectedText == searchText)
                   : (selectedText.toLower() == searchText.toLower());
    
    if (cursor.hasSelection() && matches) {
        // Replace the current selection
        cursor.insertText(replaceText);
    }
    
    // Find next occurrence
    findNext();
}

void PythonScriptingEditor::replaceAll()
{
    QString searchText = ui->edFind->text();
    QString replaceText = ui->edReplace->text();
    
    if (searchText.isEmpty()) {
        return;
    }
    
    // Move to beginning
    QTextCursor cursor = ui->editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->editor->setTextCursor(cursor);
    
    int count = 0;
    QTextDocument::FindFlags flags;
    if (ui->cbCaseSensitive->isChecked()) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (ui->cbWholeWords->isChecked()) {
        flags |= QTextDocument::FindWholeWords;
    }
    
    // Replace all occurrences
    while (ui->editor->find(searchText, flags)) {
        QTextCursor replaceCursor = ui->editor->textCursor();
        replaceCursor.insertText(replaceText);
        count++;
    }
    
    ui->result->appendPlainText(tr("Reemplazadas %1 ocurrencias").arg(count));
}

void PythonScriptingEditor::onCursorPositionChanged()
{
    highlightMatchingBraces();
}

void PythonScriptingEditor::highlightMatchingBraces()
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

int PythonScriptingEditor::findMatchingBrace(const QString &text, int pos, QChar currentChar, QChar matchChar, bool forward)
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
