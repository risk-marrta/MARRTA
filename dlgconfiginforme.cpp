#include "dlgconfiginforme.h"
#include "ui_dlgconfiginforme.h"
#include "informegenerator.h"
#include "matrizriesgo.h"
#include "dbmanager.h"
#include "htmlpreviewhelper.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QPageLayout>
#include <QStandardPaths>
#include <QBuffer>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlError>
#include <algorithm>

DlgConfigInforme::DlgConfigInforme(const QList<CompleteEventDefinition>& _allEvents, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfigInforme),
    allEvents(_allEvents)
{
    ui->setupUi(this);
    setWindowTitle(tr("Configurar informe"));
    
    // Initialize configuration with defaults
    config.includeSecuenciasRiesgo = false;
    config.includeRMA = false;
    config.includeRA = false;
    config.includeRM = false;
    config.includeRB = false;
    config.soloResumen = false;
    config.includeModelInfo = false;
    config.includeListadoCompleto = false;
    config.includeIndice = false;
    config.includeIniciadoresNoAplicables = false;
    config.includeDefensasNoAplicables = false;
    config.includeListadoIniciadores = false;
    config.includeListadoDefensas = false;
    config.orientacion = 1;  // Default to portrait (vertical)
    
    // Show all sections and enable them
    ui->checkPortada->setEnabled(true);  // Enable cover page
    ui->checkIndice->setEnabled(true);  // Enable table of contents
    ui->checkInfoModelo->setEnabled(true);  // Enable model info
    ui->checkAnalisisDefensas->setEnabled(true);  // Enable defense analysis
    ui->checkListadoIniciadores->setEnabled(true);  // Enable initiator list
    ui->checkListadoDefensas->setEnabled(true);  // Enable defense list
    ui->checkIniciadoresNoAplicables->setEnabled(true);  // Enable non-applicable initiators
    ui->checkDefensasNoAplicables->setEnabled(true);  // Enable non-applicable defenses
    
    // Enable tree initially
    ui->treeEtapas->setEnabled(true);
    
    // Load stages tree
    loadStagesTree();
}

DlgConfigInforme::~DlgConfigInforme()
{
    delete ui;
}

void DlgConfigInforme::loadStagesTree()
{
    ui->treeEtapas->clear();
    ui->treeEtapas->setHeaderLabel(tr("Etapas y Subetapas"));
    
    // Create "TODAS LAS ETAPAS" root item
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(ui->treeEtapas);
    rootItem->setText(0, tr("TODAS LAS ETAPAS"));
    rootItem->setFlags(rootItem->flags() | Qt::ItemIsUserCheckable);
    rootItem->setCheckState(0, Qt::Checked);  // Default to checked
    rootItem->setData(0, Qt::UserRole, -1); // Special ID for "all stages"
    
    // Load stages from database
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> etapas;
    dbStage.list(etapas);
    
    for (const auto& etapa : etapas) {
        QTreeWidgetItem *stageItem = new QTreeWidgetItem(rootItem);
        stageItem->setText(0, etapa.nombre);
        stageItem->setFlags(stageItem->flags() | Qt::ItemIsUserCheckable);
        stageItem->setCheckState(0, Qt::Checked);  // Default to checked
        stageItem->setData(0, Qt::UserRole, etapa.id);
        stageItem->setData(0, Qt::UserRole + 1, "stage"); // Type marker
        
        // Load substages for this stage
        Database::DbElementSubstage dbSubstage(etapa.id);
        QList<SubetapaDefinition> subetapas;
        dbSubstage.list(subetapas);
        
        for (const auto& subetapa : subetapas) {
            QTreeWidgetItem *substageItem = new QTreeWidgetItem(stageItem);
            substageItem->setText(0, subetapa.nombre);
            substageItem->setFlags(substageItem->flags() | Qt::ItemIsUserCheckable);
            substageItem->setCheckState(0, Qt::Checked);  // Default to checked
            substageItem->setData(0, Qt::UserRole, subetapa.id);
            substageItem->setData(0, Qt::UserRole + 1, "substage"); // Type marker
        }
        
        stageItem->setExpanded(false);
    }
    
    rootItem->setExpanded(true);
}

void DlgConfigInforme::on_checkSecuenciasRiesgo_stateChanged(int state)
{
    bool enabled = (state == Qt::Checked);
    ui->radioSoloResumen->setEnabled(enabled);
    ui->radioResumenDetallado->setEnabled(enabled);
    ui->checkRMA->setEnabled(enabled);
    ui->checkRA->setEnabled(enabled);
    ui->checkRM->setEnabled(enabled);
    ui->checkRB->setEnabled(enabled);
    
    if (!enabled) {
        ui->checkRMA->setChecked(false);
        ui->checkRA->setChecked(false);
        ui->checkRM->setChecked(false);
        ui->checkRB->setChecked(false);
    }
}

void DlgConfigInforme::on_checkListadoCompleto_stateChanged(int state)
{
    // No longer coupled to stage selection
    // User can independently select this option
}

void DlgConfigInforme::on_checkInfoModelo_stateChanged(int state)
{
    config.includeModelInfo = (state == Qt::Checked);
}

void DlgConfigInforme::on_treeEtapas_itemChanged(QTreeWidgetItem *item, int column)
{
    if (column != 0) return;
    
    // Block signals to avoid recursive calls
    ui->treeEtapas->blockSignals(true);
    
    // No longer auto-check "Listado completo de secuencias" when stages are selected
    // Users can independently select stages and sections
    
    // If this is the root "TODAS LAS ETAPAS", check/uncheck all stages
    if (item->data(0, Qt::UserRole).toInt() == -1) {
        Qt::CheckState state = item->checkState(0);
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem *stageItem = item->child(i);
            stageItem->setCheckState(0, state);
            // Also update substages
            for (int j = 0; j < stageItem->childCount(); ++j) {
                stageItem->child(j)->setCheckState(0, state);
            }
        }
    }
    // If this is a stage, update its substages
    else if (item->data(0, Qt::UserRole + 1).toString() == "stage") {
        Qt::CheckState state = item->checkState(0);
        for (int i = 0; i < item->childCount(); ++i) {
            item->child(i)->setCheckState(0, state);
        }
        updateParentCheckbox(item);
    }
    // If this is a substage, update parent stage checkbox
    else if (item->data(0, Qt::UserRole + 1).toString() == "substage") {
        updateParentCheckbox(item);
    }
    
    ui->treeEtapas->blockSignals(false);
}

void DlgConfigInforme::updateParentCheckbox(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if (!parent) return;
    
    // Check if all children are checked, unchecked, or mixed
    int checkedCount = 0;
    int totalCount = parent->childCount();
    
    for (int i = 0; i < totalCount; ++i) {
        if (parent->child(i)->checkState(0) == Qt::Checked) {
            checkedCount++;
        }
    }
    
    if (checkedCount == totalCount) {
        parent->setCheckState(0, Qt::Checked);
    } else if (checkedCount == 0) {
        parent->setCheckState(0, Qt::Unchecked);
    } else {
        parent->setCheckState(0, Qt::PartiallyChecked);
    }
    
    // If parent is a stage, also update the root "TODAS LAS ETAPAS"
    if (parent->data(0, Qt::UserRole + 1).toString() == "stage") {
        updateParentCheckbox(parent);
    }
}

void DlgConfigInforme::collectSelectedEvents()
{
    config.selectedStages.clear();
    config.selectedSubstages.clear();
    config.selectedEvents.clear();
    
    QTreeWidgetItem *rootItem = ui->treeEtapas->topLevelItem(0);
    if (!rootItem) return;
    
    // Check if "TODAS LAS ETAPAS" is selected
    bool allStagesSelected = (rootItem->checkState(0) == Qt::Checked);
    
    if (allStagesSelected) {
        // If "TODAS LAS ETAPAS" is checked, select all stages and substages
        Database::DbElementStage dbStage;
        QList<EtapaDefinition> etapas;
        dbStage.list(etapas);
        
        for (const auto& etapa : etapas) {
            config.selectedStages.append(etapa.id);
            
            Database::DbElementSubstage dbSubstage(etapa.id);
            QList<SubetapaDefinition> subetapas;
            dbSubstage.list(subetapas);
            
            for (const auto& subetapa : subetapas) {
                config.selectedSubstages.append(subetapa.id);
            }
        }
    } else {
        // Iterate through all stages
        for (int i = 0; i < rootItem->childCount(); ++i) {
            QTreeWidgetItem *stageItem = rootItem->child(i);
            int stageId = stageItem->data(0, Qt::UserRole).toInt();
            
            // Check if any substage is checked
            bool hasCheckedSubstage = false;
            for (int j = 0; j < stageItem->childCount(); ++j) {
                QTreeWidgetItem *substageItem = stageItem->child(j);
                if (substageItem->checkState(0) == Qt::Checked) {
                    int substageId = substageItem->data(0, Qt::UserRole).toInt();
                    config.selectedSubstages.append(substageId);
                    hasCheckedSubstage = true;
                }
            }
            
            // If any substage is checked, include the stage
            if (hasCheckedSubstage) {
                config.selectedStages.append(stageId);
            }
        }
    }
    
    // Now collect the actual events based on selection
    if (config.includeSecuenciasRiesgo) {
        // Collect events by risk level (only completed events)
        for (const auto& event : allEvents) {
            if (event.completado && 
                ((config.includeRMA && event.riesgo == 3) ||
                 (config.includeRA && event.riesgo == 2) ||
                 (config.includeRM && event.riesgo == 1) ||
                 (config.includeRB && event.riesgo == 0))) {
                config.selectedEvents.append(event);
            }
        }
    }
    
    if (config.includeListadoCompleto) {
        // Collect events from selected substages (only completed events)
        for (const auto& event : allEvents) {
            if (event.completado && config.selectedSubstages.contains(event.subetapa)) {
                // Avoid duplicates if also selected by risk
                bool alreadyAdded = false;
                for (const auto& existing : config.selectedEvents) {
                    if (existing.id == event.id) {
                        alreadyAdded = true;
                        break;
                    }
                }
                if (!alreadyAdded) {
                    config.selectedEvents.append(event);
                }
            }
        }
    }
}

void DlgConfigInforme::on_btnGenerarInforme_clicked()
{
    // Collect configuration from UI
    config.includeSecuenciasRiesgo = ui->checkSecuenciasRiesgo->isChecked();
    config.includeRMA = ui->checkRMA->isChecked();
    config.includeRA = ui->checkRA->isChecked();
    config.includeRM = ui->checkRM->isChecked();
    config.includeRB = ui->checkRB->isChecked();
    config.soloResumen = ui->radioSoloResumen->isChecked();
    config.includeModelInfo = ui->checkInfoModelo->isChecked();
    config.includeListadoCompleto = ui->checkListadoCompleto->isChecked();
    config.includeIndice = ui->checkIndice->isChecked();
    config.includeIniciadoresNoAplicables = ui->checkIniciadoresNoAplicables->isChecked();
    config.includeDefensasNoAplicables = ui->checkDefensasNoAplicables->isChecked();
    config.includeListadoIniciadores = ui->checkListadoIniciadores->isChecked();
    config.includeListadoDefensas = ui->checkListadoDefensas->isChecked();
    config.orientacion = ui->radioPortrait->isChecked() ? 1 : 0;  // 0=landscape, 1=portrait
    
    // Collect selected stages, substages and events
    collectSelectedEvents();
    
    // Validate: at least one option must be selected
    if (!config.includeSecuenciasRiesgo && !config.includeListadoCompleto && !config.includeModelInfo &&
        !config.includeIniciadoresNoAplicables && !config.includeDefensasNoAplicables &&
        !config.includeListadoIniciadores && !config.includeListadoDefensas) {
        QMessageBox::warning(this, tr("Sin selección"), 
            tr("Debe seleccionar al menos una sección para el informe."));
        return;
    }
    
    // Validate stage selection for stage-related sections
    bool needsStages = config.includeSecuenciasRiesgo || config.includeListadoCompleto || 
                       config.includeListadoIniciadores || config.includeListadoDefensas;
    
    if (needsStages && config.selectedStages.isEmpty()) {
        QMessageBox::warning(this, tr("Sin selección"), 
            tr("Debe seleccionar al menos una etapa en «Etapas que abarcará el informe»."));
        return;
    }
    
    // If "Secuencias de mayor riesgo" is selected, at least one risk level must be checked (for both options)
    if (config.includeSecuenciasRiesgo) {
        if (!config.includeRMA && !config.includeRA && !config.includeRM && !config.includeRB) {
            QMessageBox::warning(this, tr("Sin selección"), 
                tr("Debe seleccionar al menos un nivel de riesgo en el apartado \"Secuencias de mayor riesgo\"."));
            return;
        }
    }
    
    // Check if we have events to generate for "Listado detallado de secuencias"
    if (config.includeListadoCompleto) {
        if (config.selectedEvents.isEmpty()) {
            QMessageBox::warning(this, tr("Sin selección"), 
                tr("No hay eventos seleccionados para generar el informe."));
            return;
        }
    }
    
    // Check if we have events to generate (skip check if only model info is selected)
    if ((config.includeSecuenciasRiesgo || config.includeListadoCompleto) && config.selectedEvents.isEmpty()) {
        QMessageBox::warning(this, tr("Sin selección"), 
            tr("No hay eventos seleccionados para generar el informe."));
        return;
    }
    
    // Check if "Iniciadores no aplicables" or "Defensas no aplicables" is selected but there are none
    QStringList emptySections = QStringList();
    
    if (config.includeIniciadoresNoAplicables) {
        bool hasNoAplicables = false;
        for (const auto& event : allEvents) {
            if (!event.completado) {
                hasNoAplicables = true;
                break;
            }
        }
        if (!hasNoAplicables) {
            emptySections.append(tr("Listado de iniciadores no aplicables al servicio"));
        }
    }
    
    if (config.includeDefensasNoAplicables) {
        // Query for defenses where activo=0 (not marked in any event)
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM defensa WHERE activo = 0");
        if (query.exec() && query.next()) {
            int count = query.value(0).toInt();
            if (count == 0) {
                emptySections.append(tr("Defensas no aplicables al modelo"));
            }
        }
    }
    
    if (!emptySections.isEmpty()) {
        QString message;
        if (emptySections.count() == 1) {
            message = tr("No se incluirá la sección \"%1\" porque no hay %2.")
                .arg(emptySections.at(0))
                .arg(emptySections.at(0).contains(tr("iniciadores")) ? 
                     tr("iniciadores no aplicables") : tr("defensas no aplicables"));
        } else {
            message = tr("No se incluirán las siguientes secciones:\n");
            for (const QString& section : emptySections) {
                message += tr("- \"%1\" porque no hay %2\n")
                    .arg(section)
                    .arg(section.contains(tr("iniciadores")) ? 
                         tr("iniciadores no aplicables") : tr("defensas no aplicables"));
            }
        }
        QMessageBox::information(this, tr("Información"), message);
    }
    
    // Ask for file name
    QString fileName = QFileDialog::getSaveFileName(this, 
        tr("Guardar Informe PDF"), 
        QString(), 
        tr("Archivos PDF (*.pdf)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }
    
    // Show progress dialog
    QProgressDialog progress(tr("Generando informe, espere..."), tr("Cancelar"), 0, 100, this);
    progress.setMinimumDuration(0);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(10);
    
    // Generate the report
    if (generateReport(fileName, &progress)) {
        progress.setValue(100);
        QMessageBox::information(this, tr("Éxito"), 
            tr("El informe PDF se ha generado correctamente en:\n%1").arg(fileName));
        accept();
    } else {
        QMessageBox::critical(this, tr("Error"), 
            tr("No se pudo generar el informe PDF"));
    }
}

void DlgConfigInforme::on_btnCancelar_clicked()
{
    reject();
}

void DlgConfigInforme::on_btnVistaPrevia_clicked()
{
    // Collect configuration
    config.includeSecuenciasRiesgo = ui->checkSecuenciasRiesgo->isChecked();
    config.includeRMA = ui->checkRMA->isChecked();
    config.includeRA = ui->checkRA->isChecked();
    config.includeRM = ui->checkRM->isChecked();
    config.includeRB = ui->checkRB->isChecked();
    config.soloResumen = ui->radioSoloResumen->isChecked();
    config.includeModelInfo = ui->checkInfoModelo->isChecked();
    config.includeListadoCompleto = ui->checkListadoCompleto->isChecked();
    config.includeIndice = ui->checkIndice->isChecked();
    config.includeIniciadoresNoAplicables = ui->checkIniciadoresNoAplicables->isChecked();
    config.includeDefensasNoAplicables = ui->checkDefensasNoAplicables->isChecked();
    config.includeListadoIniciadores = ui->checkListadoIniciadores->isChecked();
    config.includeListadoDefensas = ui->checkListadoDefensas->isChecked();
    config.orientacion = ui->radioPortrait->isChecked() ? 1 : 0;
    
    // Collect selected events
    collectSelectedEvents();
    
    // Validate configuration (at least one section must be selected)
    if (!config.includeSecuenciasRiesgo && !config.includeListadoCompleto && !config.includeModelInfo &&
        !config.includeIniciadoresNoAplicables && !config.includeDefensasNoAplicables &&
        !config.includeListadoIniciadores && !config.includeListadoDefensas) {
        QMessageBox::warning(this, tr("Sin selección"), 
            tr("Debe seleccionar al menos una sección para previsualizar."));
        return;
    }
    
    // Check if "Secuencias de mayor riesgo" is selected but no risk level is chosen
    if (config.includeSecuenciasRiesgo) {
        if (!config.includeRMA && !config.includeRA && !config.includeRM && !config.includeRB) {
            QMessageBox::warning(this, tr("Sin selección"), 
                tr("En el apartado 'Secuencias de mayor riesgo' las opciones seleccionadas no tienen ningún suceso iniciador.\n\nDebe seleccionar al menos un nivel de riesgo."));
            return;
        }
    }
    
    // Check if we have events to generate for "Listado detallado de secuencias"
    if (config.includeListadoCompleto) {
        collectSelectedEvents();
        if (config.selectedEvents.isEmpty()) {
            QMessageBox::warning(this, tr("Sin selección"), 
                tr("No hay eventos seleccionados para previsualizar el informe."));
            return;
        }
    }
    
    // Check if we have events to generate (skip check if only model info is selected)
    if ((config.includeSecuenciasRiesgo || config.includeListadoCompleto) && config.selectedEvents.isEmpty()) {
        QMessageBox::warning(this, tr("Sin selección"), 
            tr("No hay eventos seleccionados para previsualizar el informe."));
        return;
    }
    
    // Generate the HTML
    QString html = generateFullHtml(nullptr);
    
    if (html.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), 
            tr("No se pudo generar la vista previa del informe"));
        return;
    }
    
    HtmlPreviewHelper::instance().showPreview(html, this);
}

HtmlReportConfig DlgConfigInforme::getConfiguration() const
{
    return config;
}

bool DlgConfigInforme::generateReport(const QString& fileName, QProgressDialog* progress)
{
    try {
        // Generate the full HTML content
        QString fullHtml = generateFullHtml(progress);
        
        if (fullHtml.isEmpty()) {
            return false;
        }
        
        // Prepare QPrinter with selected orientation
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        printer.setPageSize(QPageSize::A4);
        
        // Set orientation based on user selection
        if (config.orientacion == 1) {
            printer.setPageOrientation(QPageLayout::Portrait);
        } else {
            printer.setPageOrientation(QPageLayout::Landscape);
        }
        
        printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
        
        // Set the complete HTML to document and print
        QTextDocument document;
        document.setHtml(fullHtml);
        document.print(&printer);
        
        return true;
    } catch (...) {
        return false;
    }
}

QString DlgConfigInforme::generateFullHtml(QProgressDialog* progress)
{
    // Load CSS from external file (user's custom or default)
    QString cssContent = loadCSS();
    
    // Update @page orientation in CSS
    QString orientationStr = (config.orientacion == 1) ? "portrait" : "landscape";
    cssContent.replace(QRegularExpression("@page\\s*\\{[^}]*size:\\s*A4\\s+\\w+"), 
                      QString("@page { size: A4 %1; margin: 15mm; }").arg(orientationStr));
    
    QString fullHtml = QString(R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <style>
%1
    </style>
</head>
<body>)").arg(cssContent);
    
    int totalSteps = config.selectedEvents.size() + (config.includeModelInfo ? 1 : 0);
    int currentStep = 0;
    int pageNumber = 1;
    
    // Generate cover page if enabled
    if (ui->checkPortada->isChecked()) {
        fullHtml += generateCoverPage();
        fullHtml += R"(<div class="page-break"></div>)";
    }
    
    // Generate Table of Contents (Índice) if selected
    if (config.includeIndice) {
        fullHtml += generateIndice();
        fullHtml += R"(<div class="page-break"></div>)";
    }
    
    // Generate Model Information section if selected
    if (config.includeModelInfo) {
        fullHtml += R"(<div id="info-modelo" class="section-title">INFORMACIÓN DEL MODELO</div>)";
        fullHtml += R"(<div class="page-break"></div>)";
        fullHtml += generateModelInformationSection(pageNumber);
        if (progress) {
            progress->setValue(20 + (currentStep++ * 70 / totalSteps));
        }
    }
    
    // Generate sections based on configuration
    if (config.includeSecuenciasRiesgo) {
        // Section title with anchor
        fullHtml += R"(<div id="secuencias-riesgo" class="section-title">SECUENCIAS DE MAYOR RIESGO</div>)";
        fullHtml += R"(<div class="page-break"></div>)";
        
        // Group events by risk level
        QList<int> riskLevels;
        if (config.includeRMA) riskLevels.append(3);
        if (config.includeRA) riskLevels.append(2);
        if (config.includeRM) riskLevels.append(1);
        if (config.includeRB) riskLevels.append(0);
        
        for (int riskLevel : riskLevels) {
            // Section title page
            QString sectionName = MatrizRiesgo::Riesgos.at(riskLevel);
            fullHtml += R"(<div class="section-title">SECUENCIAS CON )" + sectionName.toUpper() + R"(</div>)";
            fullHtml += R"(<div class="page-break"></div>)";
            
            // Generate summary table
            fullHtml += generateSummaryTable(riskLevel, sectionName, pageNumber);
            
            // If "Resumen y listado detallado" is selected, generate detailed pages
            if (!config.soloResumen) {
                // Generate events for this risk level
                for (const auto& event : config.selectedEvents) {
                    if (event.riesgo == riskLevel) {
                        if (progress) {
                            progress->setValue(20 + (currentStep++ * 70 / totalSteps));
                            if (progress->wasCanceled()) return QString();
                        }
                        
                        fullHtml += R"(<div class="page-break"></div>)";
                        fullHtml += generateEventPage(event, sectionName, pageNumber++);
                    }
                }
            }
        }
    }
    
    if (config.includeListadoCompleto) {
        // Section title page
        fullHtml += R"(<div id="listado-completo" class="section-title">LISTADO COMPLETO DE SECUENCIAS</div>)";
        fullHtml += R"(<div class="page-break"></div>)";
        
        // Generate complete listing pages
        
        // Generate events from selected substages
        for (const auto& event : config.selectedEvents) {
            if (config.selectedSubstages.contains(event.subetapa)) {
                // Skip if already generated in risk section
                bool alreadyGenerated = false;
                if (config.includeSecuenciasRiesgo) {
                    if ((config.includeRMA && event.riesgo == 3) ||
                        (config.includeRA && event.riesgo == 2) ||
                        (config.includeRM && event.riesgo == 1) ||
                        (config.includeRB && event.riesgo == 0)) {
                        alreadyGenerated = true;
                    }
                }
                
                if (!alreadyGenerated) {
                    if (progress) {
                        progress->setValue(20 + (currentStep++ * 70 / totalSteps));
                        if (progress->wasCanceled()) return QString();
                    }
                    
                    fullHtml += R"(<div class="page-break"></div>)";
                    fullHtml += generateEventPage(event, tr("Listado Completo"), pageNumber++);
                }
            }
        }
    }
    
    // Add "Listado de iniciadores no aplicables al servicio" section (optional)
    if (config.includeIniciadoresNoAplicables) {
        QList<CompleteEventDefinition> noAplicables;
        for (const auto& event : allEvents) {
            if (!event.completado) {
                noAplicables.append(event);
            }
        }
        
        if (!noAplicables.isEmpty()) {
            // Section title page
            fullHtml += R"(<div id="iniciadores-no-aplicables" class="section-title">LISTADO DE INICIADORES NO APLICABLES AL SERVICIO</div>)";
            fullHtml += R"(<div class="page-break"></div>)";
            
            // Generate grouped tables by stage/substage for non-applicable events
            
            // Sort by stage and substage, then by ID
            std::sort(noAplicables.begin(), noAplicables.end(), 
                [](const CompleteEventDefinition& a, const CompleteEventDefinition& b) {
                    if (a.etapa != b.etapa) return a.etapa < b.etapa;
                    if (a.subetapa != b.subetapa) return a.subetapa < b.subetapa;
                    return a.id < b.id;
                });
            
            // Get stage/substage names for grouping
            Database::DbElementStage dbStage;
            QList<EtapaDefinition> etapas;
            dbStage.list(etapas);
            
            // Group by stage/substage and generate tables
            int currentEtapa = -9999;
            int currentSubetapa = -9999;
            bool tableOpen = false;
            
            for (const auto& event : noAplicables) {
                // Check if we need a new header for this stage/substage
                if (event.etapa != currentEtapa || event.subetapa != currentSubetapa) {
                    // Close previous table if open
                    if (tableOpen) {
                        fullHtml += R"(</table>)";
                    }
                    
                    // Find stage and substage names
                    QString etapaNombre;
                    QString subetapaNombre;
                    for (const auto& etapa : etapas) {
                        if (etapa.id == event.etapa) {
                            etapaNombre = etapa.nombre.toUpper();
                            Database::DbElementSubstage dbSubstage(etapa.id);
                            QList<SubetapaDefinition> subetapas;
                            dbSubstage.list(subetapas);
                            for (const auto& subetapa : subetapas) {
                                if (subetapa.id == event.subetapa) {
                                    subetapaNombre = subetapa.nombre.toUpper();
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    
                    // Add stage/substage header with blue background (same style as regular initiators list)
                    fullHtml += QString(R"(
<h3 style="background-color: #4F81BD; color: white; padding: 10px; text-align: right; font-size: 12pt; margin-top: 20px; margin-bottom: 0;">ETAPA: %1<br/>SUBETAPA: %2</h3>
<table style="width: 100%; border-collapse: collapse; margin-bottom: 20px;">
    <tr style="background-color: #4F81BD; color: white; font-weight: bold; font-size: 10pt;">
        <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap; text-align: center;">%3</td>
        <td style="border: 1px solid #000; padding: 8px;">%4</td>
    </tr>
)").arg(etapaNombre)
   .arg(subetapaNombre)
   .arg(tr("CÓDIGO"))
   .arg(tr("Definición"));
                    
                    tableOpen = true;
                    currentEtapa = event.etapa;
                    currentSubetapa = event.subetapa;
                }
                
                // Add row for this event
                fullHtml += QString(R"(
    <tr style="font-size: 10pt; background-color: white;">
        <td style="border: 1px solid #000; padding: 8px; text-align: center; white-space: nowrap;">%1</td>
        <td style="border: 1px solid #000; padding: 8px;">%2</td>
    </tr>
)").arg(QString(event.codigo).toHtmlEscaped())
   .arg(QString(event.nombre).toHtmlEscaped());
            }
            
            // Close last table if open
            if (tableOpen) {
                fullHtml += R"(</table>)";
            }
        }
    }
    
    // Add "Defensas no aplicables al modelo" section (optional)
    if (config.includeDefensasNoAplicables) {
        fullHtml += generateDefensasNoAplicables(pageNumber);
    }
    
    // Add "Listado de iniciadores" section (optional)
    if (config.includeListadoIniciadores) {
        fullHtml += generateListadoIniciadores(pageNumber);
    }
    
    // Add "Listado de defensas" section (optional)
    if (config.includeListadoDefensas) {
        fullHtml += generateListadoDefensas(pageNumber);
    }
    
    fullHtml += R"(
</body>
</html>)";
    
    return fullHtml;
}

QString DlgConfigInforme::generateEventPage(const CompleteEventDefinition& event, const QString& sectionName, int pageNumber)
{
    // Get stage name
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> etapas;
    dbStage.list(etapas);
    QString nombreEtapa;
    for (const auto& etapa : etapas) {
        if (etapa.id == event.etapa) {
            nombreEtapa = etapa.nombre;
            break;
        }
    }
    
    // Get substage name
    QList<SubetapaDefinition> subetapas;
    Database::DbElementSubstage::listAll(&subetapas);
    QString nombreSubetapa;
    for (const auto& subetapa : subetapas) {
        if (subetapa.id == event.subetapa) {
            nombreSubetapa = subetapa.nombre;
            break;
        }
    }
    
    // Prepare data
    DatosInforme datos;
    datos.etapa = nombreEtapa;
    datos.subetapa = nombreSubetapa;
    datos.codigo = event.codigo;
    datos.iniciador = event.nombre;
    datos.descripcionError = event.descripcion;
    
    // Set nominal and calculated values
    if (event.frecuencia >= 0 && event.frecuencia <= 3) {
        datos.frecuencia = MatrizRiesgo::Frecuencias.at(event.frecuencia);
    }
    if (event.consecuencia >= 0 && event.consecuencia <= 3) {
        datos.consecuencias = MatrizRiesgo::Consecuencias.at(event.consecuencia);
    }
    if (event.frecuenciaCalculada >= 0 && event.frecuenciaCalculada <= 3) {
        datos.frecuenciaCalculada = MatrizRiesgo::Frecuencias.at(event.frecuenciaCalculada);
    }
    if (event.probabilidadCalculada >= 0 && event.probabilidadCalculada <= 3) {
        datos.probabilidadCalculada = MatrizRiesgo::Probabilidades.at(event.probabilidadCalculada);
    }
    if (event.consecuenciaCalculada >= 0 && event.consecuenciaCalculada <= 3) {
        datos.consecuenciasCalculadas = MatrizRiesgo::Consecuencias.at(event.consecuenciaCalculada);
    }
    if (event.riesgo >= 0 && event.riesgo <= 3) {
        datos.riesgoCalculado = MatrizRiesgo::Riesgos.at(event.riesgo);
    }
    
    datos.contexto = event.descripcion;
    
    // Add defenses
    for (const auto& barrera : event.barreras) {
        if (barrera.active) {
            Barrera b;
            b.codigo = barrera.code;
            b.descripcion = barrera.nombre;
            b.implementacion = barrera.descripcion.isEmpty() ? tr("Sin descripción") : barrera.descripcion;
            b.robustez = MatrizRiesgo::Durezas.at(barrera.hardiness);
            datos.barreras.append(b);
        }
    }
    
    for (const auto& reductor : event.reductoresFrecuencias) {
        if (reductor.active) {
            Reductor r;
            r.codigo = reductor.code;
            r.descripcion = reductor.nombre;
            r.robustez = MatrizRiesgo::Durezas.at(reductor.hardiness);
            datos.reductoresFrecuencia.append(r);
        }
    }
    
    for (const auto& reductor : event.reductoresConsecuencias) {
        if (reductor.active) {
            Reductor r;
            r.codigo = reductor.code;
            r.descripcion = reductor.nombre;
            r.robustez = MatrizRiesgo::Durezas.at(reductor.hardiness);
            datos.reductoresConsecuencias.append(r);
        }
    }
    
    // Generate HTML for this event
    QString html = InformeGenerator::generarHTML(datos);
    
    // Add page number
    html += QString(R"(<div class="page-number">%1 - %2 %3</div>)")
        .arg(sectionName)
        .arg(tr("Página"))
        .arg(pageNumber);
    
    return html;
}

QString DlgConfigInforme::generateSummaryTable(int riskLevel, const QString& sectionName, int& pageNumber)
{
    QString html = R"(<div class="page-break"></div>)";
    
    // Title for this risk level
    QString riskName = MatrizRiesgo::Riesgos.at(riskLevel).toUpper();
    html += QString(R"(
<div style="text-align: center; font-size: 14pt; font-weight: bold; margin: 20px 0; color: #4F81BD;">
    %1
</div>
<table style="width: 100%; border-collapse: collapse; margin-bottom: 20px;">
    <tr style="background-color: #4F81BD; color: white; font-weight: bold; font-size: 10pt;">
        <td style="border: 1px solid #000; padding: 8px; width: 100px; text-align: center;">%2</td>
        <td style="border: 1px solid #000; padding: 8px;">%3</td>
    </tr>
)").arg(tr("INICIADORES CON RIESGO ") + riskName)
   .arg(tr("CÓDIGO"))
   .arg(tr("Definición"));
    
    // Collect events for this risk level sorted by stage and ID
    QList<CompleteEventDefinition> eventsForRisk;
    for (const auto& event : config.selectedEvents) {
        if (event.riesgo == riskLevel) {
            eventsForRisk.append(event);
        }
    }
    
    // Sort by etapa and then by id
    std::sort(eventsForRisk.begin(), eventsForRisk.end(), 
        [](const CompleteEventDefinition& a, const CompleteEventDefinition& b) {
            if (a.etapa != b.etapa) return a.etapa < b.etapa;
            return a.id < b.id;
        });
    
    // Add rows
    for (const auto& event : eventsForRisk) {
        html += QString(R"(
    <tr style="font-size: 10pt;">
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%1</td>
        <td style="border: 1px solid #000; padding: 8px;">%2</td>
    </tr>
)").arg(QString(event.codigo).toHtmlEscaped())
   .arg(QString(event.nombre).toHtmlEscaped());
    }
    
    html += R"(
</table>
<div class="page-number">)" + sectionName + " - " + tr("Página") + " " + QString::number(pageNumber++) + R"(</div>
)";
    
    return html;
}

QString DlgConfigInforme::generateCoverPage()
{
    // Get configuration from database
    Database::ConfiguracionModelo modelConfig = Database::DbManager::checkDatabase(false);
    
    // Get logo as base64 data URI
    QString logoDataUri = getLogoAsBase64();
    
    QString html = R"(
<div style="display: flex; flex-direction: column; justify-content: center; align-items: center; min-height: 80vh;">
    <div style="text-align: center; margin-bottom: 50px;">
        <img src="%4" alt="MARRTA Logo" style="max-width: 300px; height: auto;" />
    </div>
    <div class="cover-title">
        <h1>ANÁLISIS DE RIESGOS CON LA METODOLOGÍA DE MATRICES DE RIESGO</h1>
        <h2>Modelo %1 - %2</h2>
        <h2>%3</h2>
    </div>
</div>
)";
    
    return html.arg(modelConfig.tecnica.toHtmlEscaped())
               .arg(modelConfig.nombre.toHtmlEscaped())
               .arg(modelConfig.institucion.toHtmlEscaped())
               .arg(logoDataUri);
}

QString DlgConfigInforme::generateSectionTitlePage(const QString& sectionName)
{
    return QString(R"(
<div class="section-title">
    <div class="section-title-bar">
        %1
    </div>
</div>
)").arg(sectionName);
}

QString DlgConfigInforme::generateModelInformationSection(int& pageNumber)
{
    QString html;
    
    // Get configuration from database
    Database::ConfiguracionModelo modelConfig = Database::DbManager::checkDatabase(false);
    
    // Configuration table
    html += QString(R"(
<h3 class="section-header">DATOS DE CONFIGURACIÓN</h3>
<table class="centered-table" style="width: 80%; border-collapse: collapse; margin-bottom: 30px;">
    <tr>
        <td style="border: 1px solid #DDDDDD; padding: 8px; width: 30%; font-weight: bold; background-color: #F5F5F5;">%1</td>
        <td style="border: 1px solid #DDDDDD; padding: 8px;">%2</td>
    </tr>
    <tr>
        <td style="border: 1px solid #DDDDDD; padding: 8px; font-weight: bold; background-color: #F5F5F5;">%3</td>
        <td style="border: 1px solid #DDDDDD; padding: 8px;">%4</td>
    </tr>
    <tr>
        <td style="border: 1px solid #DDDDDD; padding: 8px; font-weight: bold; background-color: #F5F5F5;">%5</td>
        <td style="border: 1px solid #DDDDDD; padding: 8px;">%6</td>
    </tr>
    <tr>
        <td style="border: 1px solid #DDDDDD; padding: 8px; font-weight: bold; background-color: #F5F5F5;">%7</td>
        <td style="border: 1px solid #DDDDDD; padding: 8px;">%8</td>
    </tr>
    <tr>
        <td style="border: 1px solid #DDDDDD; padding: 8px; font-weight: bold; background-color: #F5F5F5;">%9</td>
        <td style="border: 1px solid #DDDDDD; padding: 8px;">%10</td>
    </tr>
</table>
)").arg(tr("Nombre")).arg(modelConfig.nombre.toHtmlEscaped())
   .arg(tr("Técnica")).arg(modelConfig.tecnica.toHtmlEscaped())
   .arg(tr("Analistas")).arg(modelConfig.analistas.toHtmlEscaped())
   .arg(tr("Institución")).arg(modelConfig.institucion.toHtmlEscaped())
   .arg(tr("Fecha")).arg(modelConfig.fecha.toHtmlEscaped());
    
    // Count events by base and calculated consequences
    int consecCMA_base = 0, consecCA_base = 0, consecCM_base = 0, consecCB_base = 0;
    int consecCMA_calc = 0, consecCA_calc = 0, consecCM_calc = 0, consecCB_calc = 0;
    int totalEvents = allEvents.size();
    
    for (const auto& event : allEvents) {
        // Base consequences
        switch (event.consecuencia) {
            case 3: consecCMA_base++; break;
            case 2: consecCA_base++; break;
            case 1: consecCM_base++; break;
            case 0: consecCB_base++; break;
        }
        // Calculated consequences
        switch (event.consecuenciaCalculada) {
            case 3: consecCMA_calc++; break;
            case 2: consecCA_calc++; break;
            case 1: consecCM_calc++; break;
            case 0: consecCB_calc++; break;
        }
    }
    
    // Count unique and total defenses by type
    // Use QMap instead of QSet for easier key comparison
    struct DefenseKey {
        QString codigo;
        QString nombre;
        int robustez;
        
        bool operator<(const DefenseKey& other) const {
            if (codigo != other.codigo) return codigo < other.codigo;
            if (nombre != other.nombre) return nombre < other.nombre;
            return robustez < other.robustez;
        }
    };
    
    QMap<DefenseKey, bool> uniqueBarriers, uniqueFreqReducers, uniqueConseqReducers;
    int totalBarriers = 0, totalFreqReducers = 0, totalConseqReducers = 0;
    
    for (const auto& event : allEvents) {
        for (const auto& b : event.barreras) {
            if (b.active) {
                DefenseKey key{b.code, b.nombre, b.hardiness};
                uniqueBarriers[key] = true;
                totalBarriers++;
            }
        }
        for (const auto& r : event.reductoresFrecuencias) {
            if (r.active) {
                DefenseKey key{r.code, r.nombre, r.hardiness};
                uniqueFreqReducers[key] = true;
                totalFreqReducers++;
            }
        }
        for (const auto& r : event.reductoresConsecuencias) {
            if (r.active) {
                DefenseKey key{r.code, r.nombre, r.hardiness};
                uniqueConseqReducers[key] = true;
                totalConseqReducers++;
            }
        }
    }
    
    // First table: Número de iniciadores
    html += QString(R"(
<h3 class="subsection-header">NÚMERO DE INICIADORES</h3>
<table class="centered-table" style="width: 80%; border-collapse: collapse; margin-bottom: 30px;">
    <tr style="background-color: #4A7EBB; color: white; font-weight: bold;">
        <td style="border: 1px solid #000; padding: 8px;" colspan="2">%1</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%2</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%</td>
    </tr>
    <tr>
        <td style="border: 1px solid #000; padding: 8px;" colspan="2">%3</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%4(%5)</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%6%</td>
    </tr>
    <tr>
        <td style="border: 1px solid #000; padding: 8px;" colspan="2">%7</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%8(%9)</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%10%</td>
    </tr>
    <tr>
        <td style="border: 1px solid #000; padding: 8px;" colspan="2">%11</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%12(%13)</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%14%</td>
    </tr>
    <tr>
        <td style="border: 1px solid #000; padding: 8px;" colspan="2">%15</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%16(%17)</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%18%</td>
    </tr>
    <tr style="background-color: #F5F5F5; font-weight: bold;">
        <td style="border: 1px solid #000; padding: 8px;" colspan="2">%19</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%20</td>
        <td style="border: 1px solid #000; padding: 8px;"></td>
    </tr>
</table>
)").arg(tr("Descripción")).arg(tr("Número"))
   .arg(tr("Con consecuencias muy altas")).arg(consecCMA_calc).arg(consecCMA_base).arg(totalEvents > 0 ? QString::number(consecCMA_calc * 100 / totalEvents) : "0")
   .arg(tr("Con consecuencias altas")).arg(consecCA_calc).arg(consecCA_base).arg(totalEvents > 0 ? QString::number(consecCA_calc * 100 / totalEvents) : "0")
   .arg(tr("Con consecuencias medias")).arg(consecCM_calc).arg(consecCM_base).arg(totalEvents > 0 ? QString::number(consecCM_calc * 100 / totalEvents) : "0")
   .arg(tr("Con consecuencias bajas")).arg(consecCB_calc).arg(consecCB_base).arg(totalEvents > 0 ? QString::number(consecCB_calc * 100 / totalEvents) : "0")
   .arg(tr("Total de iniciadores")).arg(totalEvents);
    
    // Second table: Número de defensas
    html += QString(R"(
<h3 class="subsection-header">NÚMERO DE DEFENSAS</h3>
<table class="centered-table" style="width: 80%; border-collapse: collapse; margin-bottom: 30px;">
    <tr style="background-color: #4A7EBB; color: white; font-weight: bold;">
        <td style="border: 1px solid #000; padding: 8px;">%1</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%2</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%3</td>
    </tr>
    <tr>
        <td style="border: 1px solid #000; padding: 8px;">%4</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%5</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%6</td>
    </tr>
    <tr>
        <td style="border: 1px solid #000; padding: 8px;">%7</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%8</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%9</td>
    </tr>
    <tr>
        <td style="border: 1px solid #000; padding: 8px;">%10</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%11</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%12</td>
    </tr>
</table>
)").arg(tr("Tipo")).arg(tr("Únicas")).arg(tr("Total"))
   .arg(tr("Barreras")).arg(uniqueBarriers.size()).arg(totalBarriers)
   .arg(tr("Reductores de frecuencia")).arg(uniqueFreqReducers.size()).arg(totalFreqReducers)
   .arg(tr("Reductores de consecuencia")).arg(uniqueConseqReducers.size()).arg(totalConseqReducers);
    
    // Risk profile table with colors
    int riesgoRMA = 0, riesgoRA = 0, riesgoRM = 0, riesgoRB = 0;
    for (const auto& event : allEvents) {
        switch (event.riesgo) {
            case 3: riesgoRMA++; break;
            case 2: riesgoRA++; break;
            case 1: riesgoRM++; break;
            case 0: riesgoRB++; break;
        }
    }
    
    QString rmaColor = riesgoRMA > 0 ? "background-color: #FFCCCC; color: #8B0000; font-weight: bold;" : "";
    QString raColor = riesgoRA > 0 ? "background-color: #FFE4CC; color: #CC5500; font-weight: bold;" : "";
    QString rmColor = riesgoRM > 0 ? "background-color: #CCFFCC; color: #006600; font-weight: bold;" : "";
    QString rbColor = riesgoRB > 0 ? "background-color: #F0F0F0; color: #808080; font-weight: bold;" : "";
    
    html += QString(R"(
<h3 class="subsection-header">PERFIL DE RIESGO</h3>
<table class="centered-table" style="width: 80%; border-collapse: collapse; margin-bottom: 30px;">
    <tr style="%1">
        <td style="border: 1px solid #000; padding: 8px; width: 60%; font-weight: bold;">%2</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%3</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%4%</td>
    </tr>
    <tr style="%5">
        <td style="border: 1px solid #000; padding: 8px; font-weight: bold;">%6</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%7</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%8%</td>
    </tr>
    <tr style="%9">
        <td style="border: 1px solid #000; padding: 8px; font-weight: bold;">%10</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%11</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%12%</td>
    </tr>
    <tr style="%13">
        <td style="border: 1px solid #000; padding: 8px; font-weight: bold;">%14</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%15</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center;">%16%</td>
    </tr>
</table>
)").arg(rmaColor).arg(tr("Secuencias con riesgo muy alto")).arg(riesgoRMA).arg(totalEvents > 0 ? QString::number(riesgoRMA * 100 / totalEvents) : "0")
   .arg(raColor).arg(tr("Secuencias con riesgo alto")).arg(riesgoRA).arg(totalEvents > 0 ? QString::number(riesgoRA * 100 / totalEvents) : "0")
   .arg(rmColor).arg(tr("Secuencias con riesgo medio")).arg(riesgoRM).arg(totalEvents > 0 ? QString::number(riesgoRM * 100 / totalEvents) : "0")
   .arg(rbColor).arg(tr("Secuencias con riesgo bajo")).arg(riesgoRB).arg(totalEvents > 0 ? QString::number(riesgoRB * 100 / totalEvents) : "0");
    
    // Footer with version and date
    QString version = "1.0"; // TODO: Get from app version
    QString buildNumber = "0001"; // TODO: Get from build
    QDate currentDate = QDate::currentDate();
    QString dateStr = currentDate.toString("dd/MM/yyyy");
    
    html += QString(R"(
<div style="margin-top: 40px; text-align: center; font-style: italic; color: #666666;">
    %1
</div>
<div style="text-align: center; margin-top: 10px; color: #666666;">
    %2
</div>
<div class="page-number">%3 - %4 %5</div>
)").arg(tr("Análisis e informe realizados con la metodología de Matrices de Riesgo usando el programa MARRTA (versión %1 [compilación %2])").arg(version).arg(buildNumber))
   .arg(dateStr)
   .arg(tr("Información del Modelo"))
   .arg(tr("Página"))
   .arg(pageNumber++);
    
    return html;
}

QString DlgConfigInforme::generateIndice()
{
    QString html = R"(
<div class="section-title">ÍNDICE</div>
<div class="page-break"></div>
<div style="padding: 40px;">
    <h2 style="color: #4A7EBB; margin-bottom: 30px;">CONTENIDO DEL INFORME</h2>
    <ul style="list-style: none; padding: 0;">
)";

    // Add links to each section that will be generated
    if (config.includeModelInfo) {
        html += R"(
        <li style="margin-bottom: 15px; font-size: 14pt;">
            <a href="#info-modelo" class="indice-entry">▶ INFORMACIÓN DEL MODELO</a>
        </li>
)";
    }
    
    if (config.includeSecuenciasRiesgo) {
        html += R"(
        <li style="margin-bottom: 15px; font-size: 14pt;">
            <a href="#secuencias-riesgo" class="indice-entry">▶ SECUENCIAS DE MAYOR RIESGO</a>
        </li>
)";
    }
    
    if (config.includeListadoCompleto) {
        html += R"(
        <li style="margin-bottom: 15px; font-size: 14pt;">
            <a href="#listado-completo" class="indice-entry">▶ LISTADO COMPLETO DE SECUENCIAS</a>
        </li>
)";
    }
    
    if (config.includeIniciadoresNoAplicables) {
        html += R"(
        <li style="margin-bottom: 15px; font-size: 14pt;">
            <a href="#iniciadores-no-aplicables" class="indice-entry">▶ LISTADO DE INICIADORES NO APLICABLES AL SERVICIO</a>
        </li>
)";
    }
    
    if (config.includeDefensasNoAplicables) {
        html += R"(
        <li style="margin-bottom: 15px; font-size: 14pt;">
            <a href="#defensas-no-aplicables" class="indice-entry">▶ DEFENSAS NO APLICABLES AL MODELO</a>
        </li>
)";
    }
    
    if (config.includeListadoIniciadores) {
        html += R"(
        <li style="margin-bottom: 15px; font-size: 14pt;">
            <a href="#listado-iniciadores" class="indice-entry">▶ LISTADO DE INICIADORES</a>
        </li>
)";
    }
    
    if (config.includeListadoDefensas) {
        html += R"(
        <li style="margin-bottom: 15px; font-size: 14pt;">
            <a href="#listado-defensas" class="indice-entry">▶ LISTADO DE DEFENSAS</a>
        </li>
)";
    }

    html += R"(
    </ul>
</div>
)";
    
    return html;
}

QString DlgConfigInforme::generateDefensasNoAplicables(int& pageNumber)
{
    // Get all unique defenses from the database that are NOT marked as active (activo=0)
    // This means defenses that exist but are not marked/used in any event
    QSqlQuery query;
    
    // Query for defenses where activo=0 (not marked in any event)
    query.prepare(R"""(
        SELECT DISTINCT codigo, nombre, tipo, descripcion
        FROM defensa
        WHERE activo = 0
        ORDER BY tipo, codigo
    )""");
    
    if (!query.exec()) {
        qWarning() << "Error querying non-applicable defenses:" << query.lastError().text();
        return QString();
    }
    
    // Separate defenses by type
    QList<DefenseDescription> inactiveFrecuency;
    QList<DefenseDescription> inactiveBarrier;
    QList<DefenseDescription> inactiveConsequence;
    
    while (query.next()) {
        DefenseDescription defense;
        defense.code = query.value("codigo").toString();
        defense.name = query.value("nombre").toString();
        defense.definition = query.value("descripcion").toString();
        int tipoInt = query.value("tipo").toInt();
        
        // Map tipo integer to DefenseType enum
        if (tipoInt == 0) {
            defense.type = DefenseType::FrecuencyReducer;
            inactiveFrecuency.append(defense);
        } else if (tipoInt == 1) {
            defense.type = DefenseType::Barrier;
            inactiveBarrier.append(defense);
        } else if (tipoInt == 2) {
            defense.type = DefenseType::ConsequenceReducer;
            inactiveConsequence.append(defense);
        }
    }
    
    // Check if there are any inactive defenses
    if (inactiveFrecuency.isEmpty() && inactiveBarrier.isEmpty() && inactiveConsequence.isEmpty()) {
        return QString(); // Return empty string if no inactive defenses
    }
    
    QString html = R"(<div id="defensas-no-aplicables" class="section-title">DEFENSAS NO APLICABLES AL MODELO</div>)";
    html += R"(<div class="page-break"></div>)";
    
    // Add explanatory text
    html += QString(R"(<p style="margin-bottom: 20px; font-style: italic;">%1</p>)")
        .arg(tr("La última columna muestra los iniciadores a los que está asociada la defensa, y entre paréntesis en cuántos se ha considerado aplicable"));
    
    // Helper lambda to create a table for a defense type
    auto generateDefenseTable = [&](const QString& title, const QList<DefenseDescription>& defenses) -> QString {
        if (defenses.isEmpty()) return QString();
        
        QString tableHtml = QString(R"(
<h3 class="subsection-header" style="font-size: 12pt;">%1</h3>
<table style="width: 100%; border-collapse: collapse; margin-bottom: 30px;">
    <tr style="background-color: #4F81BD; color: white; font-weight: bold; font-size: 10pt;">
        <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap; text-align: center;">%2</td>
        <td style="border: 1px solid #000; padding: 8px;">%3</td>
        <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap; text-align: center;">%4</td>
    </tr>
)").arg(title)
   .arg(tr("CÓDIGO"))
   .arg(tr("DESCRIPCIÓN"))
   .arg(tr("INICIADORES (ACTIVOS)"));
        
        // Sort by code
        QList<DefenseDescription> sortedDefenses = defenses;
        std::sort(sortedDefenses.begin(), sortedDefenses.end(), 
            [](const DefenseDescription& a, const DefenseDescription& b) {
                return a.code < b.code;
            });
        
        for (const auto& defense : sortedDefenses) {
            // Count how many initiators have this defense (should be 0 for non-applicable, but let's count to be sure)
            int totalCount = 0;
            int activeCount = 0;
            
            for (const auto& event : allEvents) {
                const std::vector<DefenseDefinition>* defenseList = nullptr;
                if (defense.type == DefenseType::FrecuencyReducer) {
                    defenseList = &event.reductoresFrecuencias;
                } else if (defense.type == DefenseType::Barrier) {
                    defenseList = &event.barreras;
                } else if (defense.type == DefenseType::ConsequenceReducer) {
                    defenseList = &event.reductoresConsecuencias;
                }
                
                if (defenseList) {
                    for (const auto& def : *defenseList) {
                        if (def.code == defense.code) {
                            totalCount++;
                            if (def.active) {
                                activeCount++;
                            }
                            break;
                        }
                    }
                }
            }
            
            QString countText = QString("%1 (%2)").arg(totalCount).arg(activeCount);
            
            tableHtml += QString(R"(
    <tr style="font-size: 10pt; background-color: white;">
        <td style="border: 1px solid #000; padding: 8px; text-align: center; white-space: nowrap;">%1</td>
        <td style="border: 1px solid #000; padding: 8px;">%2</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center; white-space: nowrap;">%3</td>
    </tr>
)").arg(QString(defense.code).toHtmlEscaped())
   .arg(QString(defense.name).toHtmlEscaped())
   .arg(countText);
        }
        
        tableHtml += R"(
</table>
)";
        return tableHtml;
    };
    
    // Generate tables for each type that has inactive defenses
    html += generateDefenseTable(tr("REDUCTORES DE FRECUENCIA NO APLICABLES"), inactiveFrecuency);
    html += generateDefenseTable(tr("BARRERAS NO APLICABLES"), inactiveBarrier);
    html += generateDefenseTable(tr("REDUCTORES DE CONSECUENCIA NO APLICABLES"), inactiveConsequence);
    
    html += R"(<div class="page-number">)" + tr("Defensas No Aplicables") + " - " + tr("Página") + " " + QString::number(pageNumber++) + R"(</div>
)";
    
    return html;
}

QString DlgConfigInforme::generateListadoIniciadores(int& pageNumber)
{
    QString html = R"(<div id="listado-iniciadores" class="section-title">LISTADO DE INICIADORES</div>)";
    html += R"(<div class="page-break"></div>)";
    
    // Get all stages and substages
    Database::DbElementStage dbStage;
    QList<EtapaDefinition> etapas;
    dbStage.list(etapas);
    
    for (const auto& etapa : etapas) {
        // Get substages for this stage
        Database::DbElementSubstage dbSubstage(etapa.id);
        QList<SubetapaDefinition> subetapas;
        dbSubstage.list(subetapas);
        
        for (const auto& subetapa : subetapas) {
            // Get events for this stage/substage
            QList<CompleteEventDefinition> stageEvents;
            for (const auto& event : allEvents) {
                if (event.etapa == etapa.id && event.subetapa == subetapa.id && event.completado) {
                    stageEvents.append(event);
                }
            }
            
            if (stageEvents.isEmpty()) continue;
            
            // Sort by event code
            std::sort(stageEvents.begin(), stageEvents.end(), 
                [](const CompleteEventDefinition& a, const CompleteEventDefinition& b) {
                    return a.codigo < b.codigo;
                });
            
            // Create table for this stage/substage
            html += QString(R"(
<h3 class="subsection-header" style="text-align: right;">ETAPA: %1<br/>SUBETAPA: %2</h3>
<table style="width: 100%; border-collapse: collapse; margin-bottom: 30px;">
    <tr style="background-color: #4F81BD; color: white; font-weight: bold; font-size: 10pt;">
        <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap; text-align: center;">%3</td>
        <td style="border: 1px solid #000; padding: 8px;">%4</td>
    </tr>
)").arg(etapa.nombre.toUpper())
   .arg(subetapa.nombre.toUpper())
   .arg(tr("CÓDIGO"))
   .arg(tr("DEFINICIÓN"));
            
            for (const auto& event : stageEvents) {
                html += QString(R"(
    <tr style="font-size: 10pt;">
        <td style="border: 1px solid #000; padding: 8px; text-align: center; white-space: nowrap;">%1</td>
        <td style="border: 1px solid #000; padding: 8px;">%2</td>
    </tr>
)").arg(QString(event.codigo).toHtmlEscaped())
   .arg(QString(event.nombre).toHtmlEscaped());
            }
            
            html += R"(
</table>
)";
        }
    }
    
    html += R"(<div class="page-number">)" + tr("Listado de Iniciadores") + " - " + tr("Página") + " " + QString::number(pageNumber++) + R"(</div>
)";
    
    return html;
}

QString DlgConfigInforme::generateListadoDefensas(int& pageNumber)
{
    QString html = R"(<div id="listado-defensas" class="section-title">LISTADO DE DEFENSAS</div>)";
    html += R"(<div class="page-break"></div>)";

    // Helper to generate a defense table for a specific type and stage
    auto generateDefenseTypeTable = [&](const QString& typeTitle, DefenseType defType) -> QString {
        QString tableHtml = QString(R"(<h2 style="margin-top: 30px; color: #4F81BD;">%1</h2>)").arg(typeTitle);

        // Add explanatory text
        tableHtml += QString(R"(<p style="margin-bottom: 20px; font-style: italic;">%1</p>)")
                         .arg(tr("La última columna muestra los iniciadores a los que está asociada la defensa, y entre paréntesis en cuántos se ha considerado aplicable"));

        // Get all stages including fictional stages (id<=0)
        QList<EtapaDefinition> etapas;
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isOpen()) {
            qWarning() << this->tr("Database not open for stage query");
            return QString();
        }
        
        QSqlQuery stageQuery(db);
        stageQuery.prepare("SELECT id, nombre, codigo FROM etapa_orden");
        if (!stageQuery.exec()) {
            qWarning() << "Error querying stages:" << stageQuery.lastError().text();
        } else {
            while (stageQuery.next()) {
                EtapaDefinition etapa;
                etapa.id = stageQuery.value(0).toInt();
                etapa.nombre = stageQuery.value(1).toString();
                etapa.codigo = stageQuery.value(2).toString();
                etapas.append(etapa);
            }
        }

        // Query defenses from database grouped by stage
        QSqlQuery query(db);
        query.prepare(R"""(
            SELECT DISTINCT d.codigo, d.nombre, d.etapa_orden_id, d.robustez
            FROM defensa d
            WHERE d.tipo = :tipo
            ORDER BY d.etapa_orden_id, d.codigo
        )""");

        // Map DefenseType enum to database integer
        int tipoInt = 0;
        if (defType == DefenseType::FrecuencyReducer) tipoInt = 0;
        else if (defType == DefenseType::Barrier) tipoInt = 1;
        else if (defType == DefenseType::ConsequenceReducer) tipoInt = 2;

        query.bindValue(":tipo", tipoInt);

        if (!query.exec()) {
            qWarning() << tr("Error querying defenses:") << query.lastError().text();
            return QString();
        }

        // Group defenses by stage
        struct DefenseInfo {
            QString code;
            QString nombre;
            int robustez;
        };
        QMap<int, QList<DefenseInfo>> defensesByStage; // stageId -> list of defense info

        while (query.next()) {
            DefenseInfo info;
            info.code = query.value("codigo").toString();
            info.nombre = query.value("nombre").toString();
            info.robustez = query.value("robustez").toInt();
            int stageId = query.value("etapa_orden_id").toInt();

            defensesByStage[stageId].append(info);
        }

        // Generate tables for each stage that has defenses
        for (auto it = defensesByStage.constBegin(); it != defensesByStage.constEnd(); ++it) {
            int stageId = it.key();
            const auto& defenseList = it.value();

            if (defenseList.isEmpty()) continue;

            // Get stage name (handle fictional stages with id<=0)
            QString stageName;
            bool found = false;
            for (const auto& etapa : etapas) {
                if (etapa.id == stageId) {
                    if (stageId <= 0) {
                        // Fictional stage: format as "NAME (CODE)"
                        stageName = etapa.nombre.toUpper() + " (" + etapa.codigo + ")";
                    } else {
                        // Regular stage: format as "ETAPA: NAME"
                        stageName = "ETAPA: " + etapa.nombre.toUpper();
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                stageName = tr("ETAPA DESCONOCIDA (ID: %1)").arg(stageId);
            }

            // Filter by selected stages if applicable
            if (!config.selectedStages.isEmpty() && stageId >= 0 && !config.selectedStages.contains(stageId)) {
                continue;
            }

            // Create table for this stage
            tableHtml += QString(R"(
<h3 class="subsection-header" style="text-align: right;">%1</h3>
<table style="width: 100%; border-collapse: collapse; margin-bottom: 30px; background-color: white;">
    <tr style="background-color: #4F81BD; color: white; font-weight: bold; font-size: 10pt;">
        <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap; text-align: center;">%2</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: left;">%3</td>
        <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap; text-align: center;">ROBUSTEZ</td>
        <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap; text-align: center;"></td>
    </tr>
)").arg(stageName)
                             .arg(tr("CÓDIGO"))
                             .arg(tr("NOMBRE"));

            // For each defense, count how many initiators it's associated with and how many are active
            for (const auto& defenseInfo : defenseList) {
                QString code = defenseInfo.code;
                QString nombre = defenseInfo.nombre;
                QString robustez = MatrizRiesgo::Durezas.at(defenseInfo.robustez);

                // Count total and active initiators for this defense
                int totalCount = 0;
                int activeCount = 0;

                for (const auto& event : allEvents) {
                    // Only count events in selected stages/substages
                    if (!config.selectedSubstages.isEmpty() && !config.selectedSubstages.contains(event.subetapa)) {
                        continue;
                    }

                    const std::vector<DefenseDefinition>* defenseList = nullptr;
                    if (defType == DefenseType::FrecuencyReducer) {
                        defenseList = &event.reductoresFrecuencias;
                    } else if (defType == DefenseType::Barrier) {
                        defenseList = &event.barreras;
                    } else if (defType == DefenseType::ConsequenceReducer) {
                        defenseList = &event.reductoresConsecuencias;
                    }

                    if (defenseList) {
                        for (const auto& def : *defenseList) {
                            if (def.code == code) {
                                totalCount++;
                                if (def.active) {
                                    activeCount++;
                                }
                                break; // Defense found in this event, move to next event
                            }
                        }
                    }
                }

                QString countText = QString("%1 (%2)").arg(totalCount).arg(activeCount);

                tableHtml += QString(R"(
    <tr style="font-size: 10pt; background-color: white;">
        <td style="border: 1px solid #000; padding: 8px; text-align: center; white-space: nowrap;">%1</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: left;">%2</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center; white-space: nowrap;">%3</td>
        <td style="border: 1px solid #000; padding: 8px; text-align: center; white-space: nowrap;">%4</td>
    </tr>
)").arg(code.toHtmlEscaped())
                                 .arg(nombre.toHtmlEscaped())
                                 .arg(robustez)
                                 .arg(countText);
            }

            tableHtml += R"(
</table>
)";
        }

        return tableHtml;
    };

    // Generate tables for each defense type
    html += generateDefenseTypeTable(tr("REDUCTORES DE FRECUENCIA"), DefenseType::FrecuencyReducer);
    html += generateDefenseTypeTable(tr("BARRERAS"), DefenseType::Barrier);
    html += generateDefenseTypeTable(tr("REDUCTORES DE CONSECUENCIA"), DefenseType::ConsequenceReducer);

    html += R"(<div class="page-number">)" + tr("Listado de Defensas") + " - " + tr("Página") + " " + QString::number(pageNumber++) + R"(</div>
)";

    return html;
}

QString DlgConfigInforme::getDefaultCSS()
{
    // Load CSS from Qt resource
    QFile file(":/report/resources/report_style.css");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        return in.readAll();
    }
    return QString();
}

QString DlgConfigInforme::getUserCSSPath()
{
    // Get user's application data directory
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(dataPath);
    }
    return dataPath + "/report_style.css";
}

QString DlgConfigInforme::loadCSS()
{
    // Try to load user's custom CSS first
    QString userCSSPath = getUserCSSPath();
    QFile userFile(userCSSPath);
    if (userFile.exists() && userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&userFile);
        QString css = in.readAll();
        userFile.close();
        if (!css.isEmpty()) {
            return css;
        }
    }
    
    // Fall back to default CSS
    return getDefaultCSS();
}

bool DlgConfigInforme::saveUserCSS(const QString& css)
{
    QString userCSSPath = getUserCSSPath();
    QFile file(userCSSPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << css;
        file.close();
        return true;
    }
    return false;
}

QString DlgConfigInforme::getLogoAsBase64()
{
    // Load logo from resources and convert to base64
    QFile file(":/logos/resources/logos/logo.png");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        QString base64 = QString::fromLatin1(data.toBase64());
        return QString("data:image/png;base64,%1").arg(base64);
    }
    return QString();
}


