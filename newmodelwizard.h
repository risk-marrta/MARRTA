#ifndef NEWMODELWIZARD_H
#define NEWMODELWIZARD_H

#include <QDialog>
#include <QString>
#include <QTreeWidgetItem>
#include <QList>

namespace Ui {
class NewModelWizard;
}

struct SubstageInfo {
    QString nombre;
    QString descripcion;
};

struct StageInfo {
    QString nombre;
    QString codigo;
    QString descripcion;
    QList<SubstageInfo> substages;
};

class NewModelWizard : public QDialog
{
    Q_OBJECT

public:
    explicit NewModelWizard(QWidget *parent = nullptr);
    ~NewModelWizard();

    QString getPdbFile() const { return pdbFile; }
    bool isSuccessful() const { return creationSuccessful; }

private slots:
    void on_btnBrowsePdb_clicked();
    void on_btnNext_clicked();
    void on_btnBack_clicked();
    void on_btnAddStage_clicked();
    void on_btnRemoveStage_clicked();
    void on_btnMoveUp_clicked();
    void on_btnMoveDown_clicked();
    void on_cancelButtonClicked();

private:
    Ui::NewModelWizard *ui;
    
    QString pdbFile;
    QString nombre;
    QString fecha;
    QString tecnica;
    QString analistas;
    QString institucion;
    QString descripcion;
    QString observaciones;
    
    QList<StageInfo> stages;
    
    int currentStep;
    bool creationSuccessful;
    
    QPushButton *nextButton;
    QPushButton *backButton;

    void updateButtons();
    void startCreation();
    bool validateCurrentStep();
    void showError(const QString &message);
    void addStageDialog();
    void addSubstageDialog(QTreeWidgetItem *stageItem);
    bool createModelDatabase();
};

#endif // NEWMODELWIZARD_H
