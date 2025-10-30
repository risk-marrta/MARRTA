# Python Scripting Engine Implementation Summary

## Overview

This document describes the implementation of the Python scripting engine for MARRTA, which provides Python 3.12 scripting capabilities alongside the existing JavaScript engine.

## Problem Statement

The issue requested integration of a Python scripting engine similar to the existing JavaScript engine with the following requirements:

1. Access from menu **Modelo/Scripting** with a new **"Python"** entry alongside the existing "Editor JS"
2. Launch a non-modal editor/viewer similar to the JavaScript editor with same options and buttons
3. Access to a non-modal help window with API documentation
4. Python syntax highlighting with find/replace functionality
5. Expose the following objects to Python:
   - **Marrta**: Base object (similar to JavaScript)
   - **Database**: Direct SQL query access
   - **Etapas**: Complex object containing all stages, with:
     - **Etapa.Imprimir()**: Returns tuple (ID, código, nombre, descripción, (substage_IDs))

## Implementation Details

### 1. Python Engine Integration

#### PythonScriptingEngine Class
**Files**: `pythonscriptingengine.h`, `pythonscriptingengine.cpp`

The engine uses Python 3.12's C API to embed Python in the Qt application:

- **Initialization**: `Py_Initialize()` initializes Python interpreter
- **Module Access**: Uses `__main__` module for script execution
- **Output Redirection**: Captures Python stdout/stderr to console widget
- **Memory Management**: Proper reference counting with `Py_INCREF`/`Py_DECREF`

```cpp
// Engine initialization
bool PythonScriptingEngine::initializePython() {
    Py_Initialize();
    mainModule = PyImport_AddModule("__main__");
    mainDict = PyModule_GetDict(mainModule);
    return true;
}
```

#### Object Exposure

Three main objects are exposed to Python:

**1. Marrta Object**
```python
# Get MARRTA version
version = Marrta.get_version()
```

**2. Database Object**
```python
# Execute SQL queries
results = Database.query("SELECT * FROM Etapa ORDER BY orden")
for row in results:
    print(row['nombre'])
```

**3. Etapas Object**
```python
# Get all stages as Etapa objects
etapas = Etapas.get_all()

# Use Etapa.imprimir() method
for etapa in etapas:
    datos = etapa.imprimir()
    id_etapa, codigo, nombre, desc, subetapas = datos
    print(f"{codigo}: {nombre} ({len(subetapas)} subetapas)")
```

### 2. Python Editor Dialog

#### PythonScriptingEditor Class
**Files**: `pythonscriptingeditor.h`, `pythonscriptingeditor.cpp`, `pythonscriptingeditor.ui`

Features implemented:
- **Execute**: Runs Python code via `PythonScriptingEngine::execute()`
- **Save/Load**: Handles `.py` files with proper error checking
- **Clear**: Clears editor and console
- **Documentation**: Opens non-modal `PythonScriptingDocumentation` window
- **Find/Replace**: Full-featured search with:
  - Case-sensitive option
  - Whole words option
  - Find next with wrap-around
  - Replace single occurrence
  - Replace all occurrences
- **Brace Matching**: Highlights matching (), [], {} pairs

```cpp
void PythonScriptingEditor::on_btExecute_clicked() {
    QString code = ui->editor->toPlainText();
    if (code.size() > 0) {
        ui->result->appendPlainText(this->scriptingEngine->execute(code));
    }
}
```

### 3. Syntax Highlighting

#### PythonHighlighter Class
**Files**: `python_highlighter.h`, `python_highlighter.cpp`

Extends `QSyntaxHighlighter` with Python-specific rules:

- **Keywords**: if, for, while, def, class, import, etc. (blue, bold)
- **Built-ins**: print, len, range, type, etc. (yellow)
- **Strings**: Single, double, and triple-quoted strings (orange-brown)
- **Comments**: Lines starting with # (green, italic)
- **Numbers**: Integer and floating-point literals (light green)
- **Operators**: +, -, *, /, =, etc. (light grey)
- **Decorators**: @decorator syntax (yellow, italic)

### 4. Documentation Window

#### PythonScriptingDocumentation Class
**Files**: `pythonscriptingdocumentation.h`, `pythonscriptingdocumentation.cpp`

Interactive documentation with:
- **Tree Navigation**: Organized API structure
- **HTML Documentation**: Detailed explanations with examples
- **Insert Example**: Click-to-insert code examples into editor
- **Sections**:
  - Marrta object and methods
  - Database object and query() method
  - Etapas object and get_all() method
  - Etapa class and imprimir() method
  - Practical examples

### 5. UI Integration

#### Menu Integration
**Files Modified**: `mainwindow.ui`, `mainwindow.h`, `mainwindow.cpp`

Added to **Modelo/Scripting** menu:
- Menu item: "Python"
- Action: `actionEditorPython`
- Handler: `MainWindow::on_actionEditorPython_triggered()`

```cpp
void MainWindow::on_actionEditorPython_triggered() {
    PythonScriptingEditor *pythonEditor = 
        new PythonScriptingEditor(this, pythonScriptingEngine);
    pythonEditor->setAttribute(Qt::WA_DeleteOnClose);
    pythonEditor->show();
}
```

### 6. Build Configuration

#### marrta.pro Updates

Added Python 3.12 support:

```qmake
# Python integration
unix {
    INCLUDEPATH += /usr/include/python3.12
    LIBS += -lpython3.12
}

win32 {
    INCLUDEPATH += "C:/Python312/include"
    LIBS += -L"C:/Python312/libs" -lpython312
}
```

Added new source files:
- `python_highlighter.cpp/h`
- `pythonscriptingengine.cpp/h`
- `pythonscriptingeditor.cpp/h/ui`
- `pythonscriptingdocumentation.cpp/h`

## API Reference

### Marrta Object

```python
# Get MARRTA version
version = Marrta.get_version()
# Returns: String with MARRTA version
```

### Database Object

```python
# Execute SQL query
results = Database.query(sql_string)
# Parameters: sql_string (str) - SQL query to execute
# Returns: List of dictionaries, one per row
# Each dictionary has column names as keys

# Example
results = Database.query("SELECT * FROM Etapa WHERE orden < 3")
for row in results:
    print(f"Stage: {row['nombre']}, Code: {row['codigo']}")
```

### Etapas Object

```python
# Get all stages
etapas = Etapas.get_all()
# Returns: List of Etapa objects

# Each Etapa object has properties:
# - id: Stage ID (int)
# - codigo: Stage code (string)
# - nombre: Stage name (string)
# - descripcion: Stage description (string)
# - orden: Display order (int)

# Etapa.imprimir() method
datos = etapa.imprimir()
# Returns: Tuple with 5 elements:
#   0: ID (int)
#   1: código (string)
#   2: nombre (string)
#   3: descripción (string)
#   4: (substage_IDs) (tuple of ints, empty if no substages)

# Example
etapas = Etapas.get_all()
for etapa in etapas:
    id_etapa, codigo, nombre, desc, subetapas = etapa.imprimir()
    print(f"Stage {id_etapa}: {codigo} - {nombre}")
    print(f"  Substages: {len(subetapas)}")
```

## Testing

Test scripts are located in `test_scripts/` directory:

1. **test_python_marrta_api.py**: Tests Marrta object
2. **test_python_database_api.py**: Tests Database.query() with various SQL queries
3. **test_python_etapas_api.py**: Tests Etapas.get_all() and Etapa.imprimir()
4. **test_python_complete.py**: Complete integration test of all APIs

### Running Tests

1. Open MARRTA application
2. Open a model
3. Navigate to **Modelo > Scripting > Python**
4. In the Python editor, click **Cargar** (Load)
5. Select a test script from `test_scripts/`
6. Click **Ejecutar** (Execute)
7. Review results in console pane

## Technical Decisions

### 1. Python Version
- **Chosen**: Python 3.12
- **Rationale**: Modern Python version with good C API stability

### 2. Etapa.imprimir() Implementation
- **Approach**: Returns tuple instead of dictionary
- **Rationale**: Matches requirement specification exactly
- **Format**: `(ID, código, nombre, descripción, (substage_IDs))`

### 3. Database Access
- **Direct SQL**: Allows flexible queries
- **Safety**: User responsibility (same as JavaScript engine)
- **Return Type**: List of dictionaries for ease of use

### 4. Memory Management
- **Python Objects**: Proper reference counting throughout
- **Qt Integration**: No memory leaks with proper cleanup in destructor
- **String Conversion**: Helper functions for QString ↔ PyObject conversion

## Known Limitations

1. **Python Version**: Requires Python 3.12 installed on system
2. **Concurrency**: Single-threaded execution (same as JavaScript engine)
3. **Security**: No sandboxing (trusted user environment)
4. **Error Handling**: Errors displayed in console, no line number highlighting

## Future Enhancements

Potential improvements for future versions:

1. **More API Objects**: Expose Events, Defenses similar to JavaScript API
2. **Enhanced Error Reporting**: Line numbers and better error messages
3. **Code Completion**: Auto-complete for API methods
4. **Debugging**: Step-through debugging capabilities
5. **Import Support**: Allow importing Python modules
6. **Async Execution**: Support for long-running scripts

## Comparison with JavaScript Engine

| Feature | JavaScript | Python |
|---------|-----------|--------|
| Engine | QJSEngine | Python C API |
| Syntax Highlighting | ✓ | ✓ |
| Find/Replace | ✗ | ✓ |
| Documentation Window | ✓ | ✓ |
| Marrta Object | ✓ | ✓ |
| Database Access | ✗ | ✓ |
| Etapas with imprimir() | ✗ | ✓ |
| Events API | ✓ | ✗ (future) |
| Defenses API | ✓ | ✗ (future) |
| Charts API | ✓ | ✗ (future) |

## Files Modified

### New Files Created
- `python_highlighter.h/cpp` - Python syntax highlighter
- `pythonscriptingengine.h/cpp` - Python interpreter integration
- `pythonscriptingeditor.h/cpp/ui` - Python editor dialog
- `pythonscriptingdocumentation.h/cpp` - Documentation window
- `test_scripts/test_python_*.py` - Test scripts (4 files)

### Files Modified
- `mainwindow.h` - Added Python engine declaration and slot
- `mainwindow.cpp` - Added Python engine initialization and menu handler
- `mainwindow.ui` - Added Python menu entry and action
- `marrta.pro` - Added Python files and library dependencies
- `RELEASE_INFO.md` - Documented changes

## Conclusion

The Python scripting engine provides a powerful alternative to JavaScript for MARRTA automation. The implementation follows the existing JavaScript engine patterns while adding requested features like find/replace and the specific Etapa.imprimir() method. The API is well-documented, tested, and ready for use.

Key achievements:
- ✓ Full Python 3.12 integration
- ✓ Complete Etapa.imprimir() implementation as specified
- ✓ Direct database access via SQL queries
- ✓ Enhanced editor with find/replace
- ✓ Comprehensive documentation with examples
- ✓ Test suite for verification
- ✓ Non-blocking UI integration
