#ifndef DLGCONFIGINFORME_H
#define DLGCONFIGINFORME_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "dbelementstage.h"
#include "dbelementsubstage.h"
#include "completeeventdefinition.h"

class QProgressDialog;

namespace Ui {
class DlgConfigInforme;
}

struct HtmlReportConfig {
    bool includeSecuenciasRiesgo;
    bool includeRMA;
    bool includeRA;
    bool includeRM;
    bool includeRB;
    bool soloResumen;  // true = solo tabla resumen, false = resumen y detallado
    bool includeModelInfo;  // Información del modelo
    bool includeListadoCompleto;
    bool includeIndice;  // Índice / Table of Contents
    bool includeIniciadoresNoAplicables;  // Listado de iniciadores no aplicables
    bool includeDefensasNoAplicables;  // Defensas no aplicables al modelo
    bool includeListadoIniciadores;  // Listado de iniciadores
    bool includeListadoDefensas;  // Listado de defensas
    int orientacion;  // 0 = landscape (apaisado), 1 = portrait (vertical)
    QList<int> selectedStages;      // IDs of selected stages
    QList<int> selectedSubstages;   // IDs of selected substages
    QList<CompleteEventDefinition> selectedEvents;  // Events to include in report
};

class DlgConfigInforme : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConfigInforme(const QList<CompleteEventDefinition>& allEvents, QWidget *parent = nullptr);
    ~DlgConfigInforme();

    HtmlReportConfig getConfiguration() const;

private slots:
    void on_checkSecuenciasRiesgo_stateChanged(int state);
    void on_checkListadoCompleto_stateChanged(int state);
    void on_checkInfoModelo_stateChanged(int state);
    void on_treeEtapas_itemChanged(QTreeWidgetItem *item, int column);
    void on_btnGenerarInforme_clicked();
    void on_btnVistaPrevia_clicked();
    void on_btnCancelar_clicked();

private:
    Ui::DlgConfigInforme *ui;
    HtmlReportConfig config;
    QList<CompleteEventDefinition> allEvents;
    
    void loadStagesTree();
    void updateParentCheckbox(QTreeWidgetItem *item);
    void collectSelectedEvents();
    bool generateReport(const QString& fileName, QProgressDialog* progress);
    QString generateFullHtml(QProgressDialog* progress = nullptr);
    QString generateCoverPage();
    QString generateIndice();
    QString generateEventPage(const CompleteEventDefinition& event, const QString& sectionName, int pageNumber);
    QString generateSummaryTable(int riskLevel, const QString& sectionName, int& pageNumber);
    QString generateSectionTitlePage(const QString& sectionName);
    QString generateModelInformationSection(int& pageNumber);
    QString generateDefensasNoAplicables(int& pageNumber);
    QString generateListadoIniciadores(int& pageNumber);
    QString generateListadoDefensas(int& pageNumber);
    
    // CSS management functions
    QString loadCSS();
    QString getDefaultCSS();
    QString getUserCSSPath();
    QString getLogoAsBase64();
    bool saveUserCSS(const QString& css);
};

#endif // DLGCONFIGINFORME_H
