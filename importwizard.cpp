#include "importwizard.h"
#include "ui_importwizard.h"
#include "modelos/xlsx.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>
#include <QPushButton>
#include <QAbstractButton>
#include <QFile>
#include <QCoreApplication>

using namespace Modelos;

ImportWizard::ImportWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportWizard),
    currentStep(0),
    conversionSuccessful(false),
    nextButton(nullptr),
    backButton(nullptr)
{
    ui->setupUi(this);
    
    // Add custom buttons to the button box
    nextButton = ui->buttonBox->addButton(tr("Siguiente"), QDialogButtonBox::ActionRole);
    backButton = ui->buttonBox->addButton(tr("Atrás"), QDialogButtonBox::ActionRole);
    
    connect(nextButton, &QPushButton::clicked, this, &ImportWizard::on_btnNext_clicked);
    connect(backButton, &QPushButton::clicked, this, &ImportWizard::on_btnBack_clicked);
    
    // Connect the Cancel/Terminar button to our custom handler
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ImportWizard::on_cancelButtonClicked);

    // Set initial state
    ui->stackedWidget->setCurrentIndex(0);
    updateButtons();
}

ImportWizard::~ImportWizard()
{
    delete ui;
}

void ImportWizard::updateButtons()
{
    QPushButton *cancelButton = nullptr;
    
    // Find the cancel button
    foreach (QAbstractButton *button, ui->buttonBox->buttons()) {
        if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Cancel) {
            cancelButton = qobject_cast<QPushButton*>(button);
            break;
        }
    }
    
    currentStep = ui->stackedWidget->currentIndex();
    
    // Update button visibility and text
    if (backButton) {
        backButton->setVisible(currentStep > 0 && currentStep < 3);
        backButton->setEnabled(currentStep > 0 && currentStep < 3);
    }
    
    if (nextButton) {
        if (currentStep == 3) {
            // On conversion page
            nextButton->setVisible(!conversionSuccessful);
            nextButton->setEnabled(false);
        } else {
            nextButton->setVisible(true);
            nextButton->setEnabled(true);
            if (currentStep == 2) {
                nextButton->setText(tr("Iniciar conversión"));
            } else {
                nextButton->setText(tr("Siguiente"));
            }
        }
    }
    
    if (cancelButton) {
        if (currentStep == 3 && conversionSuccessful) {
            cancelButton->setText(tr("Terminar"));
        } else {
            cancelButton->setText(tr("Cancelar"));
        }
    }
}

void ImportWizard::on_btnBrowseExcel_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Seleccionar archivo Excel"),
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).last(),
        tr("Archivos Excel (*.xlsx)")
    );
    
    if (!filename.isEmpty()) {
        excelFile = filename;
        ui->edExcelFile->setText(filename);
    }
}

void ImportWizard::on_btnBrowsePdb_clicked()
{
    // Start from the Excel file's directory if it exists, otherwise use Documents
    QString initialDir;
    if (!excelFile.isEmpty() && QFile::exists(excelFile)) {
        QFileInfo fileInfo(excelFile);
        initialDir = fileInfo.absolutePath();
    } else {
        initialDir = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).last();
    }
    
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Guardar modelo MARRTA"),
        initialDir,
        tr("Archivo de modelo (*.pdb)")
    );
    
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".pdb")) {
            filename += ".pdb";
        }
        pdbFile = filename;
        ui->edPdbFile->setText(filename);
    }
}

void ImportWizard::on_rbOther_toggled(bool checked)
{
    ui->edOtherName->setEnabled(checked);
    if (checked) {
        ui->edOtherName->setFocus();
    }
}

bool ImportWizard::validateCurrentStep()
{
    switch (currentStep) {
        case 0: // Step 1: Select Excel file
            if (excelFile.isEmpty()) {
                showError(tr("Por favor, seleccione un archivo Excel."));
                return false;
            }
            if (!QFile::exists(excelFile)) {
                showError(tr("El archivo seleccionado no existe."));
                return false;
            }
            return true;
            
        case 1: // Step 2: Select practice type
            if (ui->rbMARR->isChecked()) {
                practiceName = "MARR";
            } else if (ui->rbMARRTA->isChecked()) {
                practiceName = "MARR + Técnicas Avanzadas";
            } else if (ui->rbOther->isChecked()) {
                practiceName = ui->edOtherName->text().trimmed();
                if (practiceName.isEmpty()) {
                    showError(tr("Por favor, ingrese el nombre de la nueva práctica."));
                    return false;
                }
            }
            return true;
            
        case 2: // Step 3: Select destination file
            if (pdbFile.isEmpty()) {
                showError(tr("Por favor, seleccione la ubicación del archivo .pdb."));
                return false;
            }
            return true;
            
        default:
            return true;
    }
}

void ImportWizard::showError(const QString &message)
{
    QMessageBox::warning(this, tr("Error de validación"), message);
}

void ImportWizard::on_btnNext_clicked()
{
    if (!validateCurrentStep()) {
        return;
    }
    
    if (currentStep < 3) {
        currentStep++;
        ui->stackedWidget->setCurrentIndex(currentStep);
        
        if (currentStep == 3) {
            // Start conversion immediately when reaching step 4
            startConversion();
        }
        
        updateButtons();
    }
}

void ImportWizard::on_btnBack_clicked()
{
    if (currentStep > 0 && currentStep < 3) {
        currentStep--;
        ui->stackedWidget->setCurrentIndex(currentStep);
        updateButtons();
    }
}

void ImportWizard::startConversion()
{
    ui->lblConversionStatus->setText(tr("Iniciando conversión..."));
    ui->progressBar->setValue(0);
    QCoreApplication::processEvents();
    
    // Check if file exists and remove it
    if (QFile::exists(pdbFile)) {
        QFile *exists = new QFile(pdbFile);
        if (!exists->remove()) {
            ui->lblConversionStatus->setText(tr("Error: No se pudo eliminar el archivo existente."));
            conversionSuccessful = false;
            updateButtons();
            delete exists;
            return;
        }
        delete exists;
    }
    
    // Perform the conversion
    xlsx xlsxImporter;
    
    ui->lblConversionStatus->setText(tr("Convirtiendo archivo Excel..."));
    ui->progressBar->setValue(10);
    QCoreApplication::processEvents();
    
    bool result = xlsxImporter.importTables(excelFile, pdbFile, practiceName);
    
    ui->progressBar->setValue(100);
    QCoreApplication::processEvents();
    
    // Check for errors
    QVector<ErrorLocation> errors = xlsxImporter.errors();
    
    if (errors.size() == 0 && result) {
        ui->lblConversionStatus->setText(tr("¡Conversión completada correctamente!"));
        conversionSuccessful = true;
    } else {
        ui->lblConversionStatus->setText(tr("Conversión completada con errores."));
        conversionSuccessful = false;
        
        // Show errors
        ui->groupBoxErrors->setVisible(true);
        QString errorText = "<html><body>";
        errorText += "<h3>Se encontraron los siguientes errores:</h3>";
        errorText += "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse;'>";
        errorText += "<tr style='background-color: #f0f0f0; font-weight: bold;'>";
        errorText += "<th>Pestaña</th><th>Ubicación</th><th>Tipo de Error</th><th>Valor en Celda</th><th>Detalles</th>";
        errorText += "</tr>";
        
        for (auto &error : errors) {
            errorText += "<tr>";
            errorText += "<td>" + error.tab.toHtmlEscaped() + "</td>";
            errorText += "<td>" + error.line.toHtmlEscaped() + "</td>";
            
            // Use new error structure if available, otherwise fall back to old format
            if (!error.errorType.isEmpty()) {
                errorText += "<td>" + error.errorType.toHtmlEscaped() + "</td>";
                errorText += "<td>" + error.cellValue.toHtmlEscaped() + "</td>";
                errorText += "<td>" + error.errorDetails.toHtmlEscaped() + "</td>";
            } else {
                // Backward compatibility with old error format
                QString errorDesc = "";
                QString details = "";
                if (error.lineDetails.size() > 0) {
                    errorDesc = error.lineDetails[0];
                }
                if (error.lineDetails.size() > 1) {
                    for (int i = 1; i < error.lineDetails.size(); i++) {
                        if (!details.isEmpty()) details += ", ";
                        details += error.lineDetails[i];
                    }
                }
                
                errorText += "<td>Error de datos</td>";
                errorText += "<td>" + errorDesc.toHtmlEscaped() + "</td>";
                errorText += "<td>" + details.toHtmlEscaped() + "</td>";
            }
            
            errorText += "</tr>";
        }
        
        errorText += "</table>";
        errorText += "</body></html>";
        
        ui->textErrors->setHtml(errorText);
    }
    
    updateButtons();
}

void ImportWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && conversionSuccessful) {
        accept();  // Accept the dialog when conversion is successful
    } else {
        reject();  // Reject in case of cancellation
    }
}
