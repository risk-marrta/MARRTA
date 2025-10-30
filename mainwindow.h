#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <unordered_map>
#include <QSettings>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDir>
#include <QMenu>
#include <QAction>

#include <QPropertyAnimation>

#include <QMainWindow>
#include <QVariantList>
#include <QVariantMap>

#include "widget/orientablepushbutton.h"

#include "dbmanager.h"
#include "etapas.h"
#include "dlggestionetapas.h"
#include "sucesos.h"
#include "about.h"
#include "eventdefinition.h"
#include "completeeventdefinition.h"
#include "defenselistmodel.h"
#include "matrizriesgo.h"
#include "settings.h"

#include "scriptingengine.h"
#include "pythonscriptingengine.h"
#include "updatechecker.h"


#define APP_VERSION "2.0.0 (2025-10-24)"

namespace Ui {
class MainWindow;
}

enum TipoDefensa {ReductoresFrecuencia, Barreras, ReductoresConsecuencia};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool openPraxisFile(QString filename);

    /**
     * Initialize the settings.
     * Returns the last praxis readed by the user.
     *
     * @brief loadSettings
     * @return
     */
    void loadSettings(QSettings *settings = nullptr);
    /**
     * Save the configuration
     *
     * @brief saveSettings
     */
    void saveSettings();

    Settings settings;

    static const int RoleRisk = 0x1001;
    static const int RoleStage = 0x1100;
    static const int RoleSubstage = 0x1101;
    static const int RoleName = 0x1200;


    /**
     * @brief Toggle the active state of the currently selected event
     * 
     * This method is accessible from the scripting engine as Marrta.toggleActualEvent()
     */
    Q_INVOKABLE void toggleActualEvent();
    
    /**
     * @brief Enable or disable all defenses of a specific type for the current event
     * 
     * @param tipoDefensa The type of defense (0=ReductoresFrecuencia, 1=Barreras, 2=ReductoresConsecuencia)
     * @param habilitar true to enable, false to disable
     * 
     * This method is accessible from the scripting engine as Marrta.alternarDefensasEventoActual(tipo, habilitar)
     */
    Q_INVOKABLE void alternarDefensasEventoActual(TipoDefensa tipoDefensa, bool habilitar);
    
    /**
     * @brief Enable or disable a specific defense across the entire model
     * 
     * @param tipoDefensa The type of defense (0=ReductoresFrecuencia, 1=Barreras, 2=ReductoresConsecuencia)
     * @param habilitar true to enable, false to disable
     * @param pos The position in the defense list
     * 
     * This method is accessible from the scripting engine as Marrta.alternarDefensaModelo(tipo, habilitar, pos)
     */
    Q_INVOKABLE void alternarDefensaModelo(TipoDefensa tipoDefensa, bool habilitar, const QPoint &pos);
    Q_INVOKABLE void refreshMainScreen();
    Q_INVOKABLE void saveModelChanges();
    
    /**
     * @brief Get all stages including virtual stages (id < 0)
     * 
     * Returns a list of all stages defined in the current PRAXIS file, including both
     * regular stages (id >= 0) and virtual stages (id < 0).
     * 
     * @return QVariantList containing stage objects with properties:
     *         - id (int): Stage ID (can be negative for virtual stages)
     *         - nombre (QString): Stage name
     *         - descripcion (QString): Stage description
     *         - orden (int): Display order
     *         - codigo (QString): Stage code
     *         - subetapas (QStringList): Array of substage names
     * 
     * This method is accessible from the scripting engine as Marrta.getAllStages()
     * 
     * Example usage in JavaScript:
     * @code
     * var stages = Marrta.getAllStages();
     * for (var i = 0; i < stages.length; i++) {
     *     console.log("Stage: " + stages[i].nombre + " (ID: " + stages[i].id + ")");
     * }
     * @endcode
     * 
     * @since 1.4.2
     */
    Q_INVOKABLE QVariantList getAllStages();

private slots:
    void on_mnuOpen_triggered();

    void on_mnuClose_triggered();

    void on_actionEtapas_triggered();

    void on_mnuSucesosIniciadores_triggered();

    void on_mnuAcercaDe_triggered();

    void on_actionSalir_triggered();

    void on_actionEmpty_triggered();

    void on_actionMARR_triggered();


    void on_actionResumen_triggered();

    void on_actionInforme_triggered();

    void on_actionGuardarModelo_triggered();

    void on_actionMarcarTodasDefensas_triggered();
    void on_actionMarcarReductoresFrecuencia_triggered();
    void on_actionMarcarBarreras_triggered();
    void on_actionMarcarReductoresConsecuencia_triggered();

    void on_actionDesmarcarTodasDefensas_triggered();
    void on_actionDesmarcarReductoresFrecuencia_triggered();
    void on_actionDesmarcarBarreras_triggered();
    void on_actionDesmarcarReductoresConsecuencia_triggered();

    void on_actionMarcarIniciadoresAplicables_triggered();

    void on_actionMarcarIniciadoresNoAplicables_triggered();

    void on_actionCSV_triggered();

    void on_actionImportarExcel_triggered();

    void openRecentFile();  // Slot for recent file actions

    void on_lReductoresFrecuencia_customContextMenuRequested(const QPoint &pos);

    void on_lBarreras_customContextMenuRequested(const QPoint &pos);

    void on_lReductoresConsecuencia_customContextMenuRequested(const QPoint &pos);

    void on_actionInformacion_triggered();

    void on_actionXmlIniciadoresXEtapa_triggered();

    void on_actionFrecuenciaIniciadoresXEtapa_triggered();

    void on_actionConsecuenciasIniciadoresXEtapa_triggered();

    void on_actionDistribucionRiesgoEventos_triggered();

    void on_actionDistribucionBarrerasIniciadores_triggered();

    void on_actionMostrarEtapas_triggered();

    void on_eventos_customContextMenuRequested(const QPoint &pos);

    void on_lReductoresFrecuencia_doubleClicked(const QModelIndex &index);

    void on_lBarreras_doubleClicked(const QModelIndex &index);

    void on_lReductoresConsecuencia_doubleClicked(const QModelIndex &index);

    void on_btTodasEtapas_clicked();

    void on_edFiltroNombre_textChanged(const QString &arg1);

    void on_edFiltroDescripcion_textChanged(const QString &arg1);

    void on_cbFiltroFrecuencia_currentIndexChanged(int index);

    void on_cbFiltroConsecuencia_currentIndexChanged(int index);

    void on_cbFiltroRiesgo_currentIndexChanged(int index);

    void on_eventos_activated(const QModelIndex &index);

    void on_eventos_clicked(const QModelIndex &index);

    void on_eventos_doubleClicked(const QModelIndex &index);

    void closeEvent(QCloseEvent *event);

    void on_actionEditor_triggered();

    void on_actionEditorPython_triggered();

    void on_actionGuardar_triggered();

    void on_actionGuardar_como_triggered();

    void on_cbAplicables_currentIndexChanged(int index);

    void on_cbErroresHumanos_currentIndexChanged(int index);

    void on_actionGenerarInformeHTML_triggered();

    void on_actionGenerarFlujograma_triggered();

    void on_actionComprobarActualizaciones_triggered();

    void on_actionComprobarNuevosModelos_triggered();

    void on_actionEnviarComentario_triggered();

    void on_actionEnviarModeloRiesgo_triggered();

    void on_actionAyuda_triggered();

    void on_actionAvisoLegal_triggered();

    void on_actionLicenciaUso_triggered();

    void on_actionComparadorModelos_triggered();

    void on_actionListaBarreras_triggered();

public slots:

    void slot_stages_selection_changed(const QModelIndex &, const QModelIndex &);

private:
    Ui::MainWindow *ui;

    ScriptingEngine *scriptingEngine;
    PythonScriptingEngine *pythonScriptingEngine;
    UpdateChecker *updateChecker;

    OrientablePushButton *botonEtapas;
    QPropertyAnimation *animacionEtapas;

    QSqlDatabase praxisDb;
    Database::ConfiguracionModelo configuracionModelo;
    QSettings *qSettings;
    bool verbose = false;

    QStandardItemModel* model;
    QStandardItemModel *eventosModel;
    QStandardItem *actualItem = nullptr;
    // Etapa y subetapa seleccionada
    std::pair<int, int> actualElementId;
    // Este string incluye la etapa y la subetapa separados por un guión
    // Es para acelerar el acceso a los eventos.
    std::string actualStageIndex;
    QModelIndex actualEvent;

    DefenseListModel *reductoresFrecuencia;
    DefenseListModel *barreras;
    DefenseListModel *reductoresConsecuencias;
    MatrizRiesgo matrizRiesgo;

    // Guarda la definición de la etapa y subetapa seleccionados
    //std::pair<ElementType, std::pair<int, int>> selectedFS;

    // Comprobar esto para buscar evento actual al hacer una modificación
    QString etapaSubetapaSeleccionada;
    int eventoActualId;

    // Puntero a los datos del evento seleccionado por el usuario
    CompleteEventDefinition* actualCompleteEvent;

    std::unordered_map<std::string, std::vector<CompleteEventDefinition> > events;
    bool modificado = false;


    /**
     * @brief eventoAModelo Add the event to the model
     * @param model
     * @param event
     */
    void eventoAModelo(QStandardItemModel* model, CompleteEventDefinition event);

    CompleteEventDefinition *buscarEventoPorId(int id);

    void limpiarFiltros();
    bool comprobarUsarFiltro();
    bool comprobarCumpleFiltro(const CompleteEventDefinition &evento);

    void limpiarDetalles();

    void loadModel();

    /**
     * @brief mostrarEventosSeleccion
     * Show the events according to the selection made
     */
    void mostrarEventosSeleccion();

    /**
     * @brief MainWindow::mostrarEvento
     * Display the information of an event
     */
    void mostrarEvento();

    /**
     * @brief mostrarFrecuencia pinta la frecuencia del evento indicado.
     * Recibe el evento porque en los bucles que hacen el cálculo
     * no se usa el evento actual.
     * @param evento
     */
    void mostrarFrecuencia(CompleteEventDefinition &evento);
    /**
     * @brief mostrarConsecuencia pinta la consecuencia del evento indicado.
     * @param evento
     */
    void mostrarConsecuencia(CompleteEventDefinition &evento);
    /**
     * @brief mostrarProbabilidad pinta la probabilidad del evento indicado.
     * @param evento
     */
    void mostrarProbabilidad(CompleteEventDefinition &evento);
    void mostrarRiesgo();

    void mostrarReductoresFrecuencia();
    void mostrarBarreras();
    void mostrarReductoresConsecuencia();
    
    /**
     * @brief Update the defense count labels for all defense types
     * Displays [n/m] where n is active defenses and m is total defenses
     */
    void actualizarContadoresDefensas();
    
    /**
     * @brief Update the event counter in the stage title
     * Displays [n/m] where n is applicable events (completado=1) and m is total events
     */
    void actualizarContadorEventos();
    
    /**
     * @brief Refresh event list while preserving current selection
     * Updates the risk color indicators for all events
     */
    void refrescarListaEventos();

    void mostrarInfoBarrera(const std::vector<DefenseDefinition> &defensas, const QString &nombre);

    void saveModel();
    void setupScripting();
    
    /**
     * @brief showNoModelLoadedWarning
     * Shows a warning message when user tries to access a function that requires a loaded model
     */
    void showNoModelLoadedWarning();
    
    /**
     * @brief markAllInitiators
     * Marks all initiators in the model with the specified completado state
     * @param completado true to mark as applicable, false to mark as not applicable
     */
    void markAllInitiators(bool completado);
    
    /**
     * @brief markAllDefenses
     * Marks or unmarks all defenses of a specific type in the model
     * @param tipoDefensa The type of defense to mark/unmark (-1 for all types)
     * @param marcar true to mark (activate), false to unmark (deactivate)
     */
    void markAllDefenses(bool marcar); // Marks/unmarks all defenses
    void markAllDefenses(TipoDefensa tipo, bool marcar); // Marks/unmarks defenses of a specific type
    
    /**
     * @brief Get set of visible event IDs from current event model
     * 
     * Builds a set of event IDs that are currently visible in the event list,
     * respecting filters and stage selection.
     * 
     * @return std::set<int> Set of visible event IDs
     */
    std::set<int> getVisibleEventIds() const;
    
    /**
     * @brief Extract the defense name from display text format
     * 
     * Extracts the defense name from the display format "[code] Name [Hardness]"
     * by removing the code prefix and hardiness suffix.
     * 
     * @param displayText The formatted display text (e.g., "[FMB-001] Sistema de detección [Robusta]")
     * @return QString The extracted defense name (e.g., "Sistema de detección")
     */
    QString extractDefenseNameFromDisplay(const QString &displayText) const;
    
    // Recent files management
    void updateRecentFiles(const QString &fileName);
    void setupRecentFilesMenu();
    void updateRecentFilesMenu();
    void clearRecentFiles();
    
    // Maximum number of recent files to display in the menu. 
    // This value was chosen as a reasonable limit to avoid cluttering the UI.
    static const int MaxRecentFiles = 5;
    QAction *recentFileActions[MaxRecentFiles];
    QMenu *recentFilesMenu;
};

#endif // MAINWINDOW_H
