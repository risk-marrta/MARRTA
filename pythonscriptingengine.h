#ifndef PYTHONSCRIPTINGENGINE_H
#define PYTHONSCRIPTINGENGINE_H

// When Python is enabled, we must include Python.h before any Qt headers
// to avoid the Qt 'slots' macro conflicting with Python's object.h
#ifdef PYTHON_ENABLED
    // Temporarily undefine slots if it was defined
    #ifdef slots
        #define SLOTS_DEFINED_BEFORE_PYTHON
        #undef slots
    #endif
    
    #include <Python.h>
    
    // Restore slots macro for Qt if it was defined before
    #ifdef SLOTS_DEFINED_BEFORE_PYTHON
        #define slots Q_SLOTS
        #undef SLOTS_DEFINED_BEFORE_PYTHON
    #endif
#endif

#include <QString>
#include <QPlainTextEdit>

class MainWindow;

/**
 * @brief Python scripting engine for MARRTA
 * 
 * Provides Python scripting capabilities with access to MARRTA objects:
 * - Marrta: Base object for accessing main application features
 * - Database: Direct SQL query access
 * - Etapas: Access to stages with Etapa.Imprimir() method
 * - Sucesos: Access to events (initiating events)
 * - Defensas: Access to defenses (barriers, frequency and consequence reducers)
 */
class PythonScriptingEngine
{
private:
    MainWindow *mainWindow;
    QPlainTextEdit *consoleOutput;
#ifdef PYTHON_ENABLED
    PyObject *mainModule;
    PyObject *mainDict;
#endif
    bool initialized;
    QString pythonInitError_;

    // Initialize Python interpreter
    bool initializePython();
    
    // Register MARRTA objects in Python
    void registerMarrtaObject();
    void registerDatabaseObject();
    void registerEtapasObject();
    void registerSucesosObject();
    void registerDefensasObject();
    
    // Redirect Python stdout/stderr to console
    void redirectPythonOutput();

public:
    explicit PythonScriptingEngine(MainWindow *mainWindow);
    ~PythonScriptingEngine();

    /**
     * @brief Execute Python code
     * @param code Python code to execute
     * @return Result message or error
     */
    QString execute(const QString &code);

    /**
     * @brief Set the console output destination
     * @param destiny QPlainTextEdit widget for console output
     */
    void setConsoleDestiny(QPlainTextEdit *destiny);
    
    /**
     * @brief Check if Python engine is initialized properly
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const { return initialized; }
    
    /**
     * @brief Check if Python is available and initialized
     * @return true if Python is available and initialized, false otherwise
     */
    bool isPythonAvailable() const { return initialized; }
    
    /**
     * @brief Get the last Python initialization error message
     * @return Error message from last initialization attempt, or empty string if successful
     */
    QString pythonInitializationError() const;
};

#endif // PYTHONSCRIPTINGENGINE_H
