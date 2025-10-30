#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H

#include <QDialog>
#include <QString>

namespace Ui {
class ImportWizard;
}

class ImportWizard : public QDialog
{
    Q_OBJECT

public:
    explicit ImportWizard(QWidget *parent = nullptr);
    ~ImportWizard();

    QString getExcelFile() const { return excelFile; }
    QString getPdbFile() const { return pdbFile; }
    QString getPracticeName() const { return practiceName; }
    bool isSuccessful() const { return conversionSuccessful; }

private slots:
    void on_btnBrowseExcel_clicked();
    void on_btnBrowsePdb_clicked();
    void on_btnNext_clicked();
    void on_btnBack_clicked();
    void on_rbOther_toggled(bool checked);
    void on_cancelButtonClicked();

private:
    Ui::ImportWizard *ui;
    
    QString excelFile;
    QString pdbFile;
    QString practiceName;
    int currentStep;
    bool conversionSuccessful;
    
    QPushButton *nextButton;
    QPushButton *backButton;

    void updateButtons();
    void startConversion();
    bool validateCurrentStep();
    void showError(const QString &message);
};

#endif // IMPORTWIZARD_H
