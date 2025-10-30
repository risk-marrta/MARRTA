# MARRTA - GitHub Copilot Instructions

## ⚠️ CRITICAL REQUIREMENTS

### 1. Qt Translation Compatibility
**ALL text shown on screen (except from database) MUST use Qt translation standards:**
- Wrap every user-facing string with `tr()` - no exceptions
- Never hardcode Spanish text directly in code
- This ensures future internationalization support

### 2. Documentation File Placement
**ALL new .md documentation files MUST be created in `Documentation/Support/` folder:**
- Only `README.md` and `RELEASE_INFO.md` are allowed in root
- Implementation summaries, guides, and all other .md files belong in `Documentation/Support/`
- This keeps the root folder clean and organized

### 3. Release Information Updates
**EVERY PR must update `RELEASE_INFO.md` at the top with changes included:**
- Add a new entry at the top of the file documenting the PR's modifications
- Include what was changed, added, or fixed
- This maintains accurate release notes for future releases
- Follow the existing format in the file

---

## Project Overview

MARRTA (Metodología de Análisis de Riesgos y Rendimiento) is a comprehensive risk analysis and management tool built with Qt/C++17. The application provides stage management, event management, defense management, risk analysis, reporting, and a JavaScript scripting engine for automation.

## Technology Stack

- **Language**: C++17
- **Framework**: Qt 6.10 (core, gui, qml, sql, widgets, charts, printsupport, network)
- **Build System**: qmake
- **Database**: SQLite (via Qt SQL module)
- **Scripting**: JavaScript (Qt's QJSEngine)
- **Reporting**: QtWebEngine
- **Excel Integration**: QXlsx library
- **UI**: Qt Designer (.ui files)

## Code Standards and Guidelines

### C++ Coding Standards

1. **Language Version**: Use C++17 features where appropriate
2. **Qt Conventions**: Follow Qt naming conventions:
   - Use camelCase for methods and variables
   - Use PascalCase for class names
   - Qt slots use `on_<objectName>_<signalName>()` pattern
   - Member variables can use m_ prefix or no prefix (follow existing patterns in the file)

3. **Memory Management**:
   - Prefer Qt's parent-child ownership for QObject-derived classes
   - Use smart pointers (QScopedPointer, QSharedPointer) when appropriate
   - Always check for nullptr before dereferencing pointers

4. **Includes**:
   - Group includes: Qt headers, then project headers
   - Use forward declarations in headers when possible
   - Use `#include <QtModule/QClass>` format for Qt includes

### Database Guidelines

1. **Database Access**:
   - Use the `DbManager` class for database operations
   - Always use prepared statements with parameter binding (`:paramName`)
   - Check query execution success with `query.exec()` return value
   - Use transactions for multiple related operations

2. **Database Classes**:
   - `DbDefense`, `DbEvent`, `DbElement`, `DbElementStage`, `DbElementSubstage` for data access
   - Always retrieve the `activo` (active) field when querying defenses/events
   - Use `DbException` for database-related error handling

3. **SQL Patterns**:
   ```cpp
   QString sql = R"""(
       SELECT field1, field2, activo
       FROM table_name
       WHERE condition = :param
   )""";
   query.prepare(sql);
   query.bindValue(":param", value);
   if (!query.exec()) {
       // Handle error
   }
   ```

### UI Development

**⚠️ CRITICAL REQUIREMENT: ALL TEXT SHOWN ON SCREEN (except from database) MUST USE Qt TRANSLATION STANDARDS**
- Every user-facing string must be wrapped with `tr()` - no exceptions
- This applies to ALL UI elements: labels, buttons, tooltips, message boxes, window titles, status messages
- Never hardcode Spanish (or any language) text directly - always use `tr("text")`
- See "Translation and Internationalization" subsection below for complete guidelines

1. **Qt Designer Files (.ui)**:
   - Use Qt Designer for UI layouts when possible
   - Follow existing naming patterns for UI elements
   - Spanish is used for UI labels and user-facing text - **BUT MUST BE WRAPPED WITH tr()**
   - Use appropriate layouts (QFormLayout, QVBoxLayout, QHBoxLayout)

2. **Dialogs**:
   - Validate input before accepting dialogs
   - Use `QMessageBox` for confirmations and errors
   - Follow the pattern in `newmodelwizard.cpp` for wizard implementations
   - Use QFormLayout for input dialogs with proper field validation

3. **Widget Patterns**:
   - Custom widgets like `Switch`, `SwitchButton`, `WaitingSpinnerWidget` are available
   - Use `OrderableWidget` for drag-and-drop reordering

4. **Translation and Internationalization (i18n)**:
   - **CRITICAL**: ALL user-facing text MUST be wrapped with `tr()` for Qt translation compatibility
   - Never use hardcoded strings for UI text; always use `tr("text")` or `QObject::tr("text")`
   - Translation files: `marrta_es.ts` and `marrta_en.ts` (update with `lupdate`)
   - Apply `tr()` to: QMessageBox messages, setText(), setToolTip(), setWindowTitle(), button text, labels, etc.
   
   **Correct patterns:**
   ```cpp
   // QMessageBox - ALWAYS use tr()
   QMessageBox::warning(this, tr("Error"), tr("El campo es obligatorio"));
   msgBox.setText(tr("¿Quieres guardar los cambios?"));
   msgBox.setInformativeText(tr("Si no los guardas puedes perder los cambios"));
   
   // Dynamic text in widgets - use tr()
   ui->label->setText(tr("Ver renderizado"));
   ui->button->setToolTip(tr("Riesgo Bajo - Nivel de riesgo actual"));
   setWindowTitle(tr("Gestión de defensas"));
   
   // Text with placeholders - use arg()
   QString msg = tr("Error al cargar archivo: %1").arg(filename);
   ui->label->setText(tr("Total: %1 elementos").arg(count));
   ```
   
   **WRONG patterns (never do this):**
   ```cpp
   // ❌ Hardcoded Spanish text
   QMessageBox::warning(this, "Error", "El campo es obligatorio");
   msgBox.setText("¿Quieres guardar los cambios?");
   ui->label->setText("Ver renderizado");
   
   // ❌ Direct string concatenation for dynamic text
   ui->label->setText("Total: " + QString::number(count) + " elementos");
   ```
   
   **Special cases:**
   - Internal identifiers, database field names, and debug messages don't need `tr()`
   - Format specifiers like "FMB", "RA", "RMA" are data codes, not UI text (no `tr()` needed)
   - Markdown/HTML content that's pure markup doesn't need `tr()`, but text within it does
   - Property names and technical keys don't need translation

### Scripting Engine

1. **Architecture**:
   - Scripting engine based on Qt's QJSEngine
   - Global objects: `Marrta`, `console`, `Events`, `Defenses`, `Stages`, `Charts`
   - Adapters located in `adapters/` directory provide JavaScript APIs

2. **Adapter Development**:
   - Inherit from QObject and use Q_INVOKABLE for JavaScript-accessible methods
   - Document all methods with comprehensive JSDoc-style comments
   - Include @brief, @param, @return, and @example tags
   - Return QVariant or QVariantList for complex objects
   - Register adapters in `scriptingengine.cpp`

3. **API Documentation**:
   - Update `scriptingdocumentation.cpp` when adding new APIs
   - Provide code examples in documentation
   - Keep `SCRIPTING_API.md` and `SCRIPTING_API_REFERENCE.md` synchronized

### Wizards and Import/Export

1. **Wizard Pattern**:
   - Use `QWizard` for multi-step processes
   - Implement `initializePage()` and `validatePage()` for each page
   - Follow patterns in `newmodelwizard.cpp` and `importwizard.cpp`
   - Save/restore state with `QSettings` when appropriate

2. **Database Operations**:
   - Save database before destructive operations (imports, major changes)
   - Use `DbManager::saveDatabaseToFile()` and `DbManager::loadDatabaseFromFile()`
   - Provide rollback capability when possible

### Documentation Standards

1. **Code Documentation**:
   - Use comprehensive inline comments for complex logic
   - Document class purposes and responsibilities in header files
   - Include usage examples for public APIs
   - Keep existing documentation style (mix of English/Spanish)

2. **Implementation Summaries**:
   - Create detailed implementation summaries for significant changes
   - Include: Overview, Problem Statement, Solutions, Files Modified, Testing Instructions
   - Follow format in `Documentation/Support/ISSUE_3_IMPLEMENTATION_SUMMARY.md`
   - Use markdown with clear sections and code examples

3. **Visual Documentation**:
   - Use ASCII art for UI mockups (see `Documentation/Support/UI_DESCRIPTION.md`)
   - Create flow diagrams for complex processes
   - Provide before/after comparisons for UI changes

4. **Documentation File Placement**:
   - **CRITICAL**: ALL new .md documentation files MUST be created in `Documentation/Support/` folder
   - **NEVER** create .md files in the root folder except:
     - `README.md` - Main project documentation
     - `RELEASE_INFO.md` - Release information
   - Implementation summaries, guides, visual documentation, and all other .md files belong in `Documentation/Support/`
   - Update references in code/docs when creating new documentation files

### Testing and Quality

1. **Testing**:
   - Test scripts located in `test_scripts/` directory
   - Test JavaScript APIs with dedicated test files
   - Compile with Qt before committing (`qmake marrta.pro && make`)
   - Test UI changes manually in the Qt environment

2. **Build Process**:
   ```bash
   qmake marrta.pro
   make
   ```

3. **Error Handling**:
   - Use `QMessageBox` for user-facing errors
   - Log errors appropriately
   - Provide meaningful error messages
   - Use `DbException` for database errors

4. **Translation Workflow**:
   ```bash
   # After adding/modifying tr() strings, update translation files
   lupdate marrta.pro
   
   # Edit translations with Qt Linguist (optional)
   linguist marrta_es.ts
   linguist marrta_en.ts
   
   # Compile translations
   lrelease marrta.pro
   
   # Translation files will be embedded in the application via translations.qrc
   ```
   
   **Important**: Always use `tr()` for user-facing strings to ensure they appear in translation files

### Language Considerations

- **Code**: English (variables, functions, comments in code)
- **UI Labels**: Spanish (user interface text) - **MUST use tr() for translation compatibility**
- **Documentation**: Mix of English and Spanish (follow existing patterns)
- **Git Commits**: Can be either English or Spanish
- **Translation**:
  - Default language is Spanish, but all UI text must be translatable
  - Use `tr()` wrapper for all user-visible strings
  - Translation files located at root: `marrta_es.ts`, `marrta_en.ts`
  - Update translations with: `lupdate marrta.pro`
  - Compile translations with: `lrelease marrta.pro`
  - Never hardcode UI text - always use `tr()` even for Spanish default text

### File Organization

- **Core Application**: Root directory (main.cpp, mainwindow.cpp, etc.)
- **Dialogs**: Root directory (dlg*.cpp, dlg*.h, dlg*.ui)
- **Database Models**: Root directory (db*.cpp, db*.h)
- **Scripting Adapters**: `adapters/` directory
- **Data Models**: `modelos/` directory
- **Custom Widgets**: `widget/` directory
- **Test Scripts**: `test_scripts/` directory
- **Examples**: `examples/scripting/` directory
- **Documentation**: `Documentation/Support/` folder (*.md files, except README.md and RELEASE_INFO.md in root)
- **Resources**: `resources/` directory
- **Libraries**: `QXlsx/`, `LimeReport/`, `qdarkstyle/`

### Key Classes and Patterns

1. **Main Application**:
   - `MainWindow`: Main application window and coordinator
   - `DbManager`: Database management singleton

2. **Data Models**:
   - `DbEvent`, `DbDefense`, `DbElementStage`, `DbElementSubstage`
   - Follow repository pattern with static list() and CRUD methods

3. **Wizards**:
   - `NewModelWizard`: Create new models
   - `ImportWizard`: Import from Excel

4. **Dialogs**:
   - Use QFormLayout for input forms
   - Validate before accept
   - Return data via public getter methods or dialog result

5. **Scripting**:
   - `ScriptingEngine`: JavaScript engine initialization
   - `ScriptingEditor`: Script editor UI
   - Adapters: EventAdapter, DefenseAdapter, StageAdapter, ChartAdapter, ConsoleAdapter

### Common Patterns

1. **Iterating Query Results**:
   ```cpp
   while (query.next()) {
       QVariantMap item;
       item["id"] = query.value("id");
       item["nombre"] = query.value("nombre");
       result.append(item);
   }
   ```

2. **Dialog Input Validation (with proper translation)**:
   ```cpp
   if (lineEdit->text().isEmpty()) {
       QMessageBox::warning(this, tr("Error"), tr("El campo es obligatorio"));
       return;
   }
   ```

3. **Adapter Method Pattern**:
   ```cpp
   Q_INVOKABLE QVariantList getItems() {
       QVariantList result;
       // Populate result from database
       return result;
   }
   ```

4. **Translatable User Messages**:
   ```cpp
   // Simple message
   QMessageBox::information(this, tr("Información"), tr("Operación completada exitosamente"));
   
   // Message with dynamic content
   QString msg = tr("Se encontraron %1 elementos").arg(count);
   QMessageBox::information(this, tr("Resultados"), msg);
   
   // Setting widget text
   ui->statusLabel->setText(tr("Cargando..."));
   ui->actionButton->setToolTip(tr("Hacer clic para abrir"));
   ```

### Minimal Changes Philosophy

- Make **surgical, minimal changes** to existing code
- Don't refactor working code unless necessary
- Preserve existing patterns and styles
- Update only files directly related to the feature/fix
- Don't remove or modify unrelated code

### When Adding New Features

1. Study existing similar implementations first
2. Follow established patterns in the codebase
3. Update relevant documentation files
4. Add test scripts if adding JavaScript APIs
5. Create implementation summary documents
6. **Update RELEASE_INFO.md at the top with PR changes for future release notes**
7. Ensure backward compatibility

### Important Notes

- The project uses both English and Spanish - respect existing patterns
- Qt 6.10 compatibility is crucial (don't use Qt 6+ specific features)
- Database schema changes require migration strategy
- UI changes should maintain consistency with existing design
- JavaScript APIs must be thoroughly documented with examples

### Translation Best Practices

1. **Always Use tr() for UI Text**:
   - Every string that appears in the user interface MUST be wrapped with `tr()`
   - This includes: window titles, labels, buttons, tooltips, message boxes, status messages
   - Example: `setText(tr("Texto"))` instead of `setText("Texto")`

2. **Dynamic Text with Placeholders**:
   - Use `%1`, `%2`, etc. for dynamic values with `.arg()`
   - Example: `tr("Archivo: %1").arg(filename)`
   - Example: `tr("Total: %1 de %2").arg(current).arg(total)`
   - Never concatenate strings: ❌ `tr("Total: ") + QString::number(n)`

3. **Context for Translators**:
   - Use `tr("text", "context")` when the same word has different meanings
   - Example: `tr("Open", "verb")` vs `tr("Open", "adjective")`

4. **What NOT to Translate**:
   - Database field names and SQL queries
   - Internal identifiers and constants
   - Debug/log messages (optional, can be in English)
   - Format codes like "FMB", "RA", "RMA" (these are data, not UI)
   - File paths and system commands

5. **QMessageBox Translation Pattern**:
   ```cpp
   // Correct
   QMessageBox::warning(this, tr("Advertencia"), tr("Mensaje de advertencia"));
   
   QMessageBox msgBox;
   msgBox.setWindowTitle(tr("Confirmación"));
   msgBox.setText(tr("¿Desea continuar?"));
   msgBox.setInformativeText(tr("Esta acción no se puede deshacer"));
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   ```

6. **Button Text Translation**:
   ```cpp
   // For custom buttons
   QPushButton *btn = new QPushButton(tr("Aceptar"));
   
   // For dialog buttons  
   ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
   ```

7. **Updating Translations**:
   - Run `lupdate marrta.pro` after adding new `tr()` calls
   - Edit `.ts` files with Qt Linguist if needed
   - Run `lrelease marrta.pro` to compile translations to `.qm` files
   - Commit both source changes and updated `.ts` files

## Build and Test

```bash
# Build the project
cd /home/runner/work/MARRTA/MARRTA
qmake marrta.pro
make

# Run the application
./marrta

# Test JavaScript APIs
# Load test scripts from test_scripts/ directory in the Script Editor
```

## References

- Qt Documentation: https://doc.qt.io/qt-5/
- Project README: README.md
- Scripting API: Documentation/Support/SCRIPTING_API.md, Documentation/Support/SCRIPTING_API_REFERENCE.md
- Implementation examples: Documentation/Support/ISSUE_3_IMPLEMENTATION_SUMMARY.md, Documentation/Support/FEEDBACK_CHANGES.md
