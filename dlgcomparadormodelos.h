#ifndef DLGCOMPARADORMODELOS_H
#define DLGCOMPARADORMODELOS_H

#include <QDialog>
#include <QSqlDatabase>
#include <unordered_map>
#include <vector>

#include "completeeventdefinition.h"
#include "dbmanager.h"

namespace Ui {
class DlgComparadorModelos;
}

/**
 * @brief The DlgComparadorModelos class
 * 
 * Dialog to compare two risk models, showing differences in:
 * - Stages and substages
 * - Initiators (total, by stage, and applicable)
 * - Initiator codes/names and F/C values
 * - Defenses codes/names and robustness
 * - Defense associations to initiators
 * - Risk profile charts
 */
class DlgComparadorModelos : public QDialog
{
    Q_OBJECT

public:
    explicit DlgComparadorModelos(
        const QString &currentModelPath,
        Database::ConfiguracionModelo currentConfig,
        std::unordered_map<std::string, std::vector<CompleteEventDefinition>> *currentEvents,
        QWidget *parent = nullptr
    );
    ~DlgComparadorModelos();

private slots:
    void on_btnSeleccionarModelo_clicked();
    void on_btnComparar_clicked();
    void on_btnExportar_clicked();
    void on_tabComparison_currentChanged(int index);

private:
    Ui::DlgComparadorModelos *ui;
    
    QString modeloActualPath;
    Database::ConfiguracionModelo modeloActualConfig;
    std::unordered_map<std::string, std::vector<CompleteEventDefinition>> *eventosActuales;
    
    QString modeloComparacionPath;
    Database::ConfiguracionModelo modeloComparacionConfig;
    std::unordered_map<std::string, std::vector<CompleteEventDefinition>> eventosComparacion;
    QSqlDatabase dbComparacion;
    
    bool modeloComparacionCargado;
    
    // Comparison structures
    struct StageComparison {
        QString nombre;
        int substagesActual;
        int substagesComparacion;
        bool equal;
    };
    
    struct InitiatorComparison {
        QString codigo;
        QString nombreActual;
        QString nombreComparacion;
        int frecuenciaActual;
        int frecuenciaComparacion;
        int consecuenciaActual;
        int consecuenciaComparacion;
        QString etapa;
        bool codeOrNameDifferent;
        bool valuesDifferent;
    };
    
    struct DefenseComparison {
        QString codigo;
        QString nombreActual;
        QString nombreComparacion;
        int robustezActual;
        int robustezComparacion;
        bool codeOrNameDifferent;
        bool robustnessDifferent;
    };
    
    struct DefenseAssociationComparison {
        QString iniciadorCodigo;
        QString defensaCodigo;
        bool establecidaActual;
        bool establecidaComparacion;
        bool aplicadaActual;
        bool aplicadaComparacion;
    };
    
    // Helper functions
    void cargarModeloComparacion();
    void realizarComparacion();
    void compararEtapas();
    void compararIniciadores();
    void compararDefensas();
    void compararAsociacionesDefensas();
    void generarGraficas();
    void limpiarResultados();
    QString generarReporteHTML();
    
    // Data loading helpers
    bool cargarEventosDeDB(QSqlDatabase &db, 
                          std::unordered_map<std::string, std::vector<CompleteEventDefinition>> &events);
};

#endif // DLGCOMPARADORMODELOS_H
