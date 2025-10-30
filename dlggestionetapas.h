#ifndef DLGGESTIONETAPAS_H
#define DLGGESTIONETAPAS_H

#include <QDialog>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QList>

#include "dbelementstage.h"
#include "dbelementsubstage.h"

namespace Ui {
class DlgGestionEtapas;
}

class DlgGestionEtapas : public QDialog
{
    Q_OBJECT

public:
    explicit DlgGestionEtapas(QWidget *parent = nullptr);
    ~DlgGestionEtapas();

private slots:
    // Stage management slots
    void on_btnNuevaEtapa_clicked();
    void on_btnEliminarEtapa_clicked();
    void on_btnSubirEtapa_clicked();
    void on_btnBajarEtapa_clicked();
    
    // Substage management slots
    void on_btnNuevaSubetapa_clicked();
    void on_btnEliminarSubetapa_clicked();
    void on_btnSubirSubetapa_clicked();
    void on_btnBajarSubetapa_clicked();
    
    // Stage editing slots
    void on_edNombreEtapa_textChanged(const QString &text);
    void on_edDescripcionEtapa_textChanged();
    void on_edCodigoEtapa_textChanged(const QString &text);
    
    // Substage editing slots
    void on_edNombreSubetapa_textChanged(const QString &text);
    void on_edDescripcionSubetapa_textChanged();
    void on_cbActivaSubetapa_stateChanged(int state);
    
    // Selection handling
    void onStageSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onSubstageSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    
    void on_btnCerrar_clicked();

private:
    Ui::DlgGestionEtapas *ui;
    
    QStandardItemModel *stagesModel;
    QStandardItemModel *substagesModel;
    
    QList<EtapaDefinition> stages;
    QList<SubetapaDefinition> substages;
    
    int selectedStageIndex;
    int selectedSubstageIndex;
    
    bool updatingUI; // Flag to prevent recursive updates
    
    // Helper methods
    void loadStages();
    void loadSubstages(int stageId);
    void refreshStagesView();
    void refreshSubstagesView();
    void showStageDetails();
    void showSubstageDetails();
    void clearStageDetails();
    void clearSubstageDetails();
    
    // Database operations
    bool saveStage(EtapaDefinition &stage);
    bool deleteStage(int stageId);
    bool updateStageOrder(int stageId, int newOrder);
    
    bool saveSubstage(SubetapaDefinition &substage);
    bool deleteSubstage(int substageId);
    bool updateSubstageOrder(int substageId, int newOrder);
    
    // Validation
    bool validateStage();
    bool validateSubstage();
};

#endif // DLGGESTIONETAPAS_H
