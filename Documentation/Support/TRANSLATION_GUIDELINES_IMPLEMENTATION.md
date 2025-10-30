# Translation Guidelines - Implementation Summary

## Overview

This document summarizes the addition of comprehensive translation guidelines to the GitHub Copilot instructions for the MARRTA project. The guidelines ensure that all user-facing text is compatible with Qt's translation system, enabling future internationalization.

## Problem Statement

The user requested that Copilot instructions ensure **ALL texts displayed on screen are prepared to be compatible with Qt's translation system to other languages**. 

### Current State Analysis

The project already has translation infrastructure:
- Translation files: `marrta_es.ts`, `marrta_en.ts`
- Compiled translation: `marrta_en.qm`
- Translation resources: `translations.qrc`
- Some files use `tr()` correctly (e.g., `importwizard.cpp`)

However, many files contain hardcoded Spanish text without `tr()`:
- `mainwindow.cpp`: Hardcoded QMessageBox messages and setText() calls
- `resumen.cpp`: Hardcoded button text
- `defensas.cpp`: Hardcoded warning messages
- And many others

## Solution Implemented

### 1. Updated `.github/copilot-instructions.md`

Added comprehensive translation guidelines in multiple strategic locations:

#### A. UI Development Section (New Subsection 4)

Added **"Translation and Internationalization (i18n)"** subsection with:
- **CRITICAL** requirement: ALL user-facing text MUST use `tr()`
- Correct patterns with code examples
- Wrong patterns marked with ❌ 
- Special cases that don't need translation

**Key Points:**
```cpp
// ✅ Correct
QMessageBox::warning(this, tr("Error"), tr("El campo es obligatorio"));
ui->label->setText(tr("Ver renderizado"));

// ❌ Wrong
QMessageBox::warning(this, "Error", "El campo es obligatorio");
ui->label->setText("Ver renderizado");
```

#### B. Language Considerations Section (Enhanced)

Expanded to emphasize translation requirements:
- Default language is Spanish, but ALL UI text must be translatable
- Use `tr()` wrapper for all user-visible strings
- Translation workflow: `lupdate marrta.pro` → edit → `lrelease marrta.pro`
- **Never hardcode UI text** - always use `tr()` even for Spanish default text

#### C. Testing and Quality Section (New Subsection 4)

Added **"Translation Workflow"** with complete commands:
```bash
# Update translation files
lupdate marrta.pro

# Edit translations (optional)
linguist marrta_es.ts
linguist marrta_en.ts

# Compile translations
lrelease marrta.pro
```

#### D. Common Patterns Section (Updated)

Changed example from:
```cpp
// Old - hardcoded
QMessageBox::warning(this, "Error", "El campo es obligatorio");
```

To:
```cpp
// New - translatable
QMessageBox::warning(this, tr("Error"), tr("El campo es obligatorio"));
```

Added new pattern (4) for "Translatable User Messages" with multiple examples.

#### E. New Section: Translation Best Practices

Added comprehensive standalone section after "Important Notes" with 7 detailed guidelines:

1. **Always Use tr() for UI Text**
   - Lists all cases that need `tr()`
   - Clear example: `setText(tr("Texto"))` vs `setText("Texto")`

2. **Dynamic Text with Placeholders**
   - Use `%1`, `%2` with `.arg()`
   - Example: `tr("Archivo: %1").arg(filename)`
   - Never concatenate: ❌ `tr("Total: ") + QString::number(n)`

3. **Context for Translators**
   - Use `tr("text", "context")` for disambiguation
   - Example: `tr("Open", "verb")` vs `tr("Open", "adjective")`

4. **What NOT to Translate**
   - Database field names
   - Internal identifiers
   - Debug/log messages
   - Format codes like "FMB", "RA", "RMA"
   - File paths and system commands

5. **QMessageBox Translation Pattern**
   ```cpp
   QMessageBox::warning(this, tr("Advertencia"), tr("Mensaje"));
   
   msgBox.setWindowTitle(tr("Confirmación"));
   msgBox.setText(tr("¿Desea continuar?"));
   msgBox.setInformativeText(tr("Esta acción no se puede deshacer"));
   ```

6. **Button Text Translation**
   ```cpp
   QPushButton *btn = new QPushButton(tr("Aceptar"));
   ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
   ```

7. **Updating Translations**
   - Step-by-step workflow
   - Reminder to commit both source and `.ts` files

## Files Modified

1. **`.github/copilot-instructions.md`**
   - Added Translation and Internationalization subsection to UI Development
   - Enhanced Language Considerations section with translation details
   - Added Translation Workflow subsection to Testing and Quality
   - Updated Common Patterns with correct tr() usage
   - Added comprehensive Translation Best Practices section

2. **`TRANSLATION_GUIDELINES_IMPLEMENTATION.md`** (this file)
   - Complete documentation of the changes

## Technical Details

### What Requires tr()

All user-facing strings must use `tr()`:
- `QMessageBox` messages (title, text, informativeText)
- `setText()` calls on UI elements
- `setToolTip()` calls
- `setWindowTitle()` calls
- Button text and labels
- Status messages and notifications
- Error messages shown to users

### What Does NOT Require tr()

The following should NOT be translated:
- Database field names and SQL queries
- Internal identifiers and constants
- Debug/log messages (optional)
- Data format codes (FMB, RA, RMA, etc.)
- File paths and system commands
- Property names and technical keys

### Translation Workflow

```bash
# 1. Write code with tr() for all UI text
ui->label->setText(tr("Mi texto"));

# 2. Update translation files
lupdate marrta.pro

# 3. (Optional) Edit translations with Qt Linguist
linguist marrta_es.ts
linguist marrta_en.ts

# 4. Compile translations
lrelease marrta.pro

# 5. Commit changes
git add .
git commit -m "Add translatable strings"
```

## Examples from Existing Code

### Good Example (importwizard.cpp)
```cpp
nextButton->setText(tr("Siguiente"));
cancelButton->setText(tr("Terminar"));
QMessageBox::warning(this, tr("Error de validación"), message);
ui->lblConversionStatus->setText(tr("Iniciando conversión..."));
```

### Bad Examples (to be fixed)

**mainwindow.cpp:**
```cpp
// ❌ Should be tr()
msgBox.setText("No se puede localizar la carpeta de modelos...");
ui->etRiesgo->setToolTip("Riesgo Bajo - Nivel de riesgo actual...");
```

**resumen.cpp:**
```cpp
// ❌ Should be tr()
ui->btRiskSourceText->setText("Ver renderizado");
ui->btRiskSourceText->setText("Ver Markdown");
```

**defensas.cpp:**
```cpp
// ❌ Should be tr()
QMessageBox::critical(this, "MARRTA", "Este nombre ya está siendo utilizado.");
msgBox.setText("¿Quieres guardar los cambios?");
ui->etGraficoRobustez->setText("Muy blanda");
```

## Impact

### For Developers
- Clear guidelines on when and how to use `tr()`
- Examples of correct and incorrect patterns
- Complete workflow documentation

### For Copilot
- Will now suggest `tr()` for all user-facing strings
- Will follow correct translation patterns
- Will avoid hardcoded UI text

### For the Project
- All new code will be translation-ready
- Consistent approach to internationalization
- Better preparation for multi-language support

## Next Steps

1. ✅ **Completed**: Updated Copilot instructions with translation guidelines
2. **Recommended**: Gradually update existing code to use `tr()` where missing
3. **Recommended**: Run `lupdate marrta.pro` to update translation files
4. **Recommended**: Review and complete translations in `.ts` files
5. **Recommended**: Test application with different languages

## Testing the Guidelines

To verify Copilot follows these guidelines:
1. Ask Copilot to add a new dialog with user messages
2. Verify it uses `tr()` for all text
3. Ask Copilot to add a QMessageBox
4. Verify it uses `tr()` for title and message
5. Ask Copilot to set widget text dynamically
6. Verify it uses `tr("Text %1").arg(value)` pattern

## Translation Resources

- **Qt Linguist**: GUI tool for editing translations
- **lupdate**: Extracts translatable strings from source code
- **lrelease**: Compiles `.ts` files to binary `.qm` files
- **Translation Files**: `marrta_es.ts`, `marrta_en.ts`
- **Resource File**: `translations.qrc`

## References

- Qt Internationalization: https://doc.qt.io/qt-5/internationalization.html
- Qt Linguist Manual: https://doc.qt.io/qt-5/qtlinguist-index.html
- Copilot Instructions: `.github/copilot-instructions.md`
- Best Practices: https://gh.io/copilot-coding-agent-tips

## Keywords

Qt Translation, Internationalization, i18n, tr(), lupdate, lrelease, Qt Linguist, GitHub Copilot, MARRTA, Multilingual Support
