#include "dlggestionetapas.h"
#include "ui_dlggestionetapas.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DlgGestionEtapas::DlgGestionEtapas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgGestionEtapas),
    stagesModel(nullptr),
    substagesModel(nullptr),
    selectedStageIndex(-1),
    selectedSubstageIndex(-1),
    updatingUI(false)
{
    ui->setupUi(this);
    setWindowTitle(tr("Gestión de Etapas y Subetapas"));
    
    // Initialize models
    stagesModel = new QStandardItemModel(this);
    substagesModel = new QStandardItemModel(this);
    
    ui->listEtapas->setModel(stagesModel);
    ui->listSubetapas->setModel(substagesModel);
    
    // Connect selection signals
    connect(ui->listEtapas->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &DlgGestionEtapas::onStageSelectionChanged);
    connect(ui->listSubetapas->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &DlgGestionEtapas::onSubstageSelectionChanged);
    
    // Load data
    loadStages();
    
    // Initially disable substage controls
    ui->groupSubetapas->setEnabled(false);
    clearStageDetails();
    clearSubstageDetails();
}

DlgGestionEtapas::~DlgGestionEtapas()
{
    delete stagesModel;
    delete substagesModel;
    delete ui;
}

void DlgGestionEtapas::loadStages()
{
    try {
        Database::DbElementStage dbStages;
        stages.clear();
        dbStages.list(stages, false);
        refreshStagesView();
        
        if (!stages.isEmpty()) {
            ui->listEtapas->setCurrentIndex(stagesModel->index(0, 0));
        }
    } catch (Database::DbException &e) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al cargar las etapas: %1").arg(e.message()));
    }
}

void DlgGestionEtapas::loadSubstages(int stageId)
{
    try {
        Database::DbElementSubstage dbSubstages(stageId);
        substages.clear();
        dbSubstages.list(substages);
        refreshSubstagesView();
        
        if (!substages.isEmpty()) {
            ui->listSubetapas->setCurrentIndex(substagesModel->index(0, 0));
        } else {
            clearSubstageDetails();
        }
    } catch (const char* msg) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al cargar las subetapas: %1").arg(msg));
    }
}

void DlgGestionEtapas::refreshStagesView()
{
    stagesModel->clear();
    
    for (int i = 0; i < stages.size(); i++) {
        QStandardItem *item = new QStandardItem(stages[i].nombre);
        item->setData(i, Qt::UserRole); // Store index
        stagesModel->appendRow(item);
    }
}

void DlgGestionEtapas::refreshSubstagesView()
{
    substagesModel->clear();
    
    for (int i = 0; i < substages.size(); i++) {
        QString text = substages[i].nombre;
        if (!substages[i].activo) {
            text += tr(" (Inactiva)");
        }
        QStandardItem *item = new QStandardItem(text);
        item->setData(i, Qt::UserRole); // Store index
        substagesModel->appendRow(item);
    }
}

void DlgGestionEtapas::showStageDetails()
{
    if (selectedStageIndex < 0 || selectedStageIndex >= stages.size()) {
        clearStageDetails();
        return;
    }
    
    updatingUI = true;
    
    const EtapaDefinition &stage = stages[selectedStageIndex];
    ui->edNombreEtapa->setText(stage.nombre);
    ui->edDescripcionEtapa->setPlainText(stage.descripcion);
    ui->edCodigoEtapa->setText(stage.codigo);
    
    // Enable/disable ordering buttons
    ui->btnSubirEtapa->setEnabled(selectedStageIndex > 0);
    ui->btnBajarEtapa->setEnabled(selectedStageIndex < stages.size() - 1);
    
    // Enable substage management
    ui->groupSubetapas->setEnabled(true);
    
    // Load substages for this stage
    loadSubstages(stage.id);
    
    updatingUI = false;
}

void DlgGestionEtapas::showSubstageDetails()
{
    if (selectedSubstageIndex < 0 || selectedSubstageIndex >= substages.size()) {
        clearSubstageDetails();
        return;
    }
    
    updatingUI = true;
    
    const SubetapaDefinition &substage = substages[selectedSubstageIndex];
    ui->edNombreSubetapa->setText(substage.nombre);
    ui->edDescripcionSubetapa->setPlainText(substage.descripcion);
    ui->cbActivaSubetapa->setChecked(substage.activo);
    
    // Enable/disable ordering buttons
    ui->btnSubirSubetapa->setEnabled(selectedSubstageIndex > 0);
    ui->btnBajarSubetapa->setEnabled(selectedSubstageIndex < substages.size() - 1);
    
    updatingUI = false;
}

void DlgGestionEtapas::clearStageDetails()
{
    updatingUI = true;
    ui->edNombreEtapa->clear();
    ui->edDescripcionEtapa->clear();
    ui->edCodigoEtapa->clear();
    ui->btnSubirEtapa->setEnabled(false);
    ui->btnBajarEtapa->setEnabled(false);
    ui->groupSubetapas->setEnabled(false);
    updatingUI = false;
}

void DlgGestionEtapas::clearSubstageDetails()
{
    updatingUI = true;
    ui->edNombreSubetapa->clear();
    ui->edDescripcionSubetapa->clear();
    ui->cbActivaSubetapa->setChecked(true);
    ui->btnSubirSubetapa->setEnabled(false);
    ui->btnBajarSubetapa->setEnabled(false);
    updatingUI = false;
}

// Stage management slots
void DlgGestionEtapas::on_btnNuevaEtapa_clicked()
{
    bool ok;
    QString nombre = QInputDialog::getText(this, tr("Nueva Etapa"), 
        tr("Nombre de la etapa:"), QLineEdit::Normal, "", &ok);
    
    if (!ok || nombre.trimmed().isEmpty()) {
        return;
    }
    
    QString codigo = QInputDialog::getText(this, tr("Nueva Etapa"), 
        tr("Código de la etapa (máximo 3 caracteres):"), QLineEdit::Normal, "", &ok);
    
    if (!ok || codigo.trimmed().isEmpty()) {
        return;
    }
    
    if (codigo.length() > 3) {
        QMessageBox::warning(this, tr("Error"), 
            tr("El código no puede tener más de 3 caracteres"));
        return;
    }
    
    // Create new stage
    EtapaDefinition newStage;
    newStage.nombre = nombre.trimmed();
    newStage.codigo = codigo.trimmed();
    newStage.descripcion = "";
    newStage.orden = stages.isEmpty() ? 0 : stages.last().orden + 1;
    newStage.bdOrden = newStage.orden;
    newStage.creado = true;
    newStage.modificado = false;
    
    // Save to database
    if (saveStage(newStage)) {
        // Reload stages from database to get the new ID
        loadStages();
        
        // Select the new stage (should be last)
        if (!stages.isEmpty()) {
            ui->listEtapas->setCurrentIndex(stagesModel->index(stages.size() - 1, 0));
        }
    }
}

void DlgGestionEtapas::on_btnEliminarEtapa_clicked()
{
    if (selectedStageIndex < 0 || selectedStageIndex >= stages.size()) {
        return;
    }
    
    const EtapaDefinition &stage = stages[selectedStageIndex];
    
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Confirmar eliminación"));
    msgBox.setText(tr("¿Está seguro de que desea eliminar la etapa '%1'?").arg(stage.nombre));
    msgBox.setInformativeText(tr("Esta acción eliminará también todas sus subetapas y no se puede deshacer."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Warning);
    
    if (msgBox.exec() == QMessageBox::Yes) {
        if (deleteStage(stage.id)) {
            loadStages();
        }
    }
}

void DlgGestionEtapas::on_btnSubirEtapa_clicked()
{
    if (selectedStageIndex <= 0 || selectedStageIndex >= stages.size()) {
        return;
    }
    
    // Swap orders
    int currentOrder = stages[selectedStageIndex].orden;
    int previousOrder = stages[selectedStageIndex - 1].orden;
    
    stages[selectedStageIndex].orden = previousOrder;
    stages[selectedStageIndex - 1].orden = currentOrder;
    
    // Save both stages
    QSqlDatabase::database().transaction();
    
    bool success = true;
    success = success && updateStageOrder(stages[selectedStageIndex].id, previousOrder);
    success = success && updateStageOrder(stages[selectedStageIndex - 1].id, currentOrder);
    
    if (success) {
        QSqlDatabase::database().commit();
        int newIndex = selectedStageIndex - 1;
        loadStages();
        ui->listEtapas->setCurrentIndex(stagesModel->index(newIndex, 0));
    } else {
        QSqlDatabase::database().rollback();
    }
}

void DlgGestionEtapas::on_btnBajarEtapa_clicked()
{
    if (selectedStageIndex < 0 || selectedStageIndex >= stages.size() - 1) {
        return;
    }
    
    // Swap orders
    int currentOrder = stages[selectedStageIndex].orden;
    int nextOrder = stages[selectedStageIndex + 1].orden;
    
    stages[selectedStageIndex].orden = nextOrder;
    stages[selectedStageIndex + 1].orden = currentOrder;
    
    // Save both stages
    QSqlDatabase::database().transaction();
    
    bool success = true;
    success = success && updateStageOrder(stages[selectedStageIndex].id, nextOrder);
    success = success && updateStageOrder(stages[selectedStageIndex + 1].id, currentOrder);
    
    if (success) {
        QSqlDatabase::database().commit();
        int newIndex = selectedStageIndex + 1;
        loadStages();
        ui->listEtapas->setCurrentIndex(stagesModel->index(newIndex, 0));
    } else {
        QSqlDatabase::database().rollback();
    }
}

// Substage management slots
void DlgGestionEtapas::on_btnNuevaSubetapa_clicked()
{
    if (selectedStageIndex < 0 || selectedStageIndex >= stages.size()) {
        return;
    }
    
    bool ok;
    QString nombre = QInputDialog::getText(this, tr("Nueva Subetapa"), 
        tr("Nombre de la subetapa:"), QLineEdit::Normal, "", &ok);
    
    if (!ok || nombre.trimmed().isEmpty()) {
        return;
    }
    
    // Create new substage
    SubetapaDefinition newSubstage;
    newSubstage.nombre = nombre.trimmed();
    newSubstage.descripcion = "";
    newSubstage.activo = true;
    newSubstage.etapaId = stages[selectedStageIndex].id;
    newSubstage.orden = substages.isEmpty() ? 0 : substages.last().orden + 1;
    newSubstage.bdOrden = newSubstage.orden;
    newSubstage.creado = true;
    newSubstage.modificado = false;
    
    // Save to database
    if (saveSubstage(newSubstage)) {
        // Reload substages from database to get the new ID
        loadSubstages(stages[selectedStageIndex].id);
        
        // Select the new substage (should be last)
        if (!substages.isEmpty()) {
            ui->listSubetapas->setCurrentIndex(substagesModel->index(substages.size() - 1, 0));
        }
    }
}

void DlgGestionEtapas::on_btnEliminarSubetapa_clicked()
{
    if (selectedSubstageIndex < 0 || selectedSubstageIndex >= substages.size()) {
        return;
    }
    
    const SubetapaDefinition &substage = substages[selectedSubstageIndex];
    
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Confirmar eliminación"));
    msgBox.setText(tr("¿Está seguro de que desea eliminar la subetapa '%1'?").arg(substage.nombre));
    msgBox.setInformativeText(tr("Esta acción no se puede deshacer."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Warning);
    
    if (msgBox.exec() == QMessageBox::Yes) {
        if (deleteSubstage(substage.id)) {
            loadSubstages(stages[selectedStageIndex].id);
        }
    }
}

void DlgGestionEtapas::on_btnSubirSubetapa_clicked()
{
    if (selectedSubstageIndex <= 0 || selectedSubstageIndex >= substages.size()) {
        return;
    }
    
    // Swap orders
    int currentOrder = substages[selectedSubstageIndex].orden;
    int previousOrder = substages[selectedSubstageIndex - 1].orden;
    
    substages[selectedSubstageIndex].orden = previousOrder;
    substages[selectedSubstageIndex - 1].orden = currentOrder;
    
    // Save both substages
    QSqlDatabase::database().transaction();
    
    bool success = true;
    success = success && updateSubstageOrder(substages[selectedSubstageIndex].id, previousOrder);
    success = success && updateSubstageOrder(substages[selectedSubstageIndex - 1].id, currentOrder);
    
    if (success) {
        QSqlDatabase::database().commit();
        int newIndex = selectedSubstageIndex - 1;
        loadSubstages(stages[selectedStageIndex].id);
        ui->listSubetapas->setCurrentIndex(substagesModel->index(newIndex, 0));
    } else {
        QSqlDatabase::database().rollback();
    }
}

void DlgGestionEtapas::on_btnBajarSubetapa_clicked()
{
    if (selectedSubstageIndex < 0 || selectedSubstageIndex >= substages.size() - 1) {
        return;
    }
    
    // Swap orders
    int currentOrder = substages[selectedSubstageIndex].orden;
    int nextOrder = substages[selectedSubstageIndex + 1].orden;
    
    substages[selectedSubstageIndex].orden = nextOrder;
    substages[selectedSubstageIndex + 1].orden = currentOrder;
    
    // Save both substages
    QSqlDatabase::database().transaction();
    
    bool success = true;
    success = success && updateSubstageOrder(substages[selectedSubstageIndex].id, nextOrder);
    success = success && updateSubstageOrder(substages[selectedSubstageIndex + 1].id, currentOrder);
    
    if (success) {
        QSqlDatabase::database().commit();
        int newIndex = selectedSubstageIndex + 1;
        loadSubstages(stages[selectedStageIndex].id);
        ui->listSubetapas->setCurrentIndex(substagesModel->index(newIndex, 0));
    } else {
        QSqlDatabase::database().rollback();
    }
}

// Stage editing slots
void DlgGestionEtapas::on_edNombreEtapa_textChanged(const QString &text)
{
    if (updatingUI || selectedStageIndex < 0 || selectedStageIndex >= stages.size()) {
        return;
    }
    
    stages[selectedStageIndex].nombre = text;
    stages[selectedStageIndex].modificado = true;
    
    // Update the list view
    QStandardItem *item = stagesModel->item(selectedStageIndex);
    if (item) {
        item->setText(text);
    }
    
    // Save to database
    saveStage(stages[selectedStageIndex]);
}

void DlgGestionEtapas::on_edDescripcionEtapa_textChanged()
{
    if (updatingUI || selectedStageIndex < 0 || selectedStageIndex >= stages.size()) {
        return;
    }
    
    stages[selectedStageIndex].descripcion = ui->edDescripcionEtapa->toPlainText();
    stages[selectedStageIndex].modificado = true;
    
    // Save to database
    saveStage(stages[selectedStageIndex]);
}

void DlgGestionEtapas::on_edCodigoEtapa_textChanged(const QString &text)
{
    if (updatingUI || selectedStageIndex < 0 || selectedStageIndex >= stages.size()) {
        return;
    }
    
    
    stages[selectedStageIndex].codigo = text;
    stages[selectedStageIndex].modificado = true;
    
    // Save to database
    saveStage(stages[selectedStageIndex]);
}

// Substage editing slots
void DlgGestionEtapas::on_edNombreSubetapa_textChanged(const QString &text)
{
    if (updatingUI || selectedSubstageIndex < 0 || selectedSubstageIndex >= substages.size()) {
        return;
    }
    
    substages[selectedSubstageIndex].nombre = text;
    substages[selectedSubstageIndex].modificado = true;
    
    // Update the list view
    QString displayText = text;
    if (!substages[selectedSubstageIndex].activo) {
        displayText += tr(" (Inactiva)");
    }
    QStandardItem *item = substagesModel->item(selectedSubstageIndex);
    if (item) {
        item->setText(displayText);
    }
    
    // Save to database
    saveSubstage(substages[selectedSubstageIndex]);
}

void DlgGestionEtapas::on_edDescripcionSubetapa_textChanged()
{
    if (updatingUI || selectedSubstageIndex < 0 || selectedSubstageIndex >= substages.size()) {
        return;
    }
    
    substages[selectedSubstageIndex].descripcion = ui->edDescripcionSubetapa->toPlainText();
    substages[selectedSubstageIndex].modificado = true;
    
    // Save to database
    saveSubstage(substages[selectedSubstageIndex]);
}

void DlgGestionEtapas::on_cbActivaSubetapa_stateChanged(int state)
{
    if (updatingUI || selectedSubstageIndex < 0 || selectedSubstageIndex >= substages.size()) {
        return;
    }
    
    substages[selectedSubstageIndex].activo = (state == Qt::Checked);
    substages[selectedSubstageIndex].modificado = true;
    
    // Update the list view
    QString displayText = substages[selectedSubstageIndex].nombre;
    if (!substages[selectedSubstageIndex].activo) {
        displayText += tr(" (Inactiva)");
    }
    QStandardItem *item = substagesModel->item(selectedSubstageIndex);
    if (item) {
        item->setText(displayText);
    }
    
    // Save to database
    saveSubstage(substages[selectedSubstageIndex]);
}

// Selection handling
void DlgGestionEtapas::onStageSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    
    if (selected.indexes().isEmpty()) {
        selectedStageIndex = -1;
        clearStageDetails();
        clearSubstageDetails();
        ui->groupSubetapas->setEnabled(false);
        return;
    }
    
    QModelIndex index = selected.indexes().first();
    selectedStageIndex = index.data(Qt::UserRole).toInt();
    showStageDetails();
}

void DlgGestionEtapas::onSubstageSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    
    if (selected.indexes().isEmpty()) {
        selectedSubstageIndex = -1;
        clearSubstageDetails();
        return;
    }
    
    QModelIndex index = selected.indexes().first();
    selectedSubstageIndex = index.data(Qt::UserRole).toInt();
    showSubstageDetails();
}

void DlgGestionEtapas::on_btnCerrar_clicked()
{
    accept();
}

// Database operations
bool DlgGestionEtapas::saveStage(EtapaDefinition &stage)
{
    QSqlQuery query;
    
    if (stage.creado) {
        // Insert new stage
        query.prepare(R"(
            INSERT INTO etapas (nombre, descripcion, codigo, orden)
            VALUES (:nombre, :descripcion, :codigo, :orden)
        )");
        query.bindValue(":nombre", stage.nombre);
        query.bindValue(":descripcion", stage.descripcion);
        query.bindValue(":codigo", stage.codigo);
        query.bindValue(":orden", stage.orden);
        
        if (!query.exec()) {
            QMessageBox::critical(this, tr("Error"), 
                tr("Error al crear la etapa: %1").arg(query.lastError().text()));
            return false;
        }
        
        stage.id = query.lastInsertId().toInt();
        stage.creado = false;
        stage.modificado = false;
    } else if (stage.modificado) {
        // Update existing stage
        query.prepare(R"(
            UPDATE etapas 
            SET nombre = :nombre, descripcion = :descripcion, codigo = :codigo
            WHERE id = :id
        )");
        query.bindValue(":nombre", stage.nombre);
        query.bindValue(":descripcion", stage.descripcion);
        query.bindValue(":codigo", stage.codigo);
        query.bindValue(":id", stage.id);
        
        if (!query.exec()) {
            QMessageBox::critical(this, tr("Error"), 
                tr("Error al actualizar la etapa: %1").arg(query.lastError().text()));
            return false;
        }
        
        stage.modificado = false;
    }
    
    return true;
}

bool DlgGestionEtapas::deleteStage(int stageId)
{
    QSqlQuery query;
    
    // Delete substages first
    query.prepare("DELETE FROM subetapas WHERE etapa_id = :stageId");
    query.bindValue(":stageId", stageId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al eliminar las subetapas: %1").arg(query.lastError().text()));
        return false;
    }
    
    // Delete stage
    query.prepare("DELETE FROM etapas WHERE id = :id");
    query.bindValue(":id", stageId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al eliminar la etapa: %1").arg(query.lastError().text()));
        return false;
    }
    
    return true;
}

bool DlgGestionEtapas::updateStageOrder(int stageId, int newOrder)
{
    QSqlQuery query;
    query.prepare("UPDATE etapas SET orden = :orden WHERE id = :id");
    query.bindValue(":orden", newOrder);
    query.bindValue(":id", stageId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al actualizar el orden de la etapa: %1").arg(query.lastError().text()));
        return false;
    }
    
    return true;
}

bool DlgGestionEtapas::saveSubstage(SubetapaDefinition &substage)
{
    QSqlQuery query;
    
    if (substage.creado) {
        // Insert new substage
        query.prepare(R"(
            INSERT INTO subetapas (etapa_id, nombre, descripcion, orden, activo)
            VALUES (:etapa_id, :nombre, :descripcion, :orden, :activo)
        )");
        query.bindValue(":etapa_id", substage.etapaId);
        query.bindValue(":nombre", substage.nombre);
        query.bindValue(":descripcion", substage.descripcion);
        query.bindValue(":orden", substage.orden);
        query.bindValue(":activo", substage.activo);
        
        if (!query.exec()) {
            QMessageBox::critical(this, tr("Error"), 
                tr("Error al crear la subetapa: %1").arg(query.lastError().text()));
            return false;
        }
        
        substage.id = query.lastInsertId().toInt();
        substage.creado = false;
        substage.modificado = false;
    } else if (substage.modificado) {
        // Update existing substage
        query.prepare(R"(
            UPDATE subetapas 
            SET nombre = :nombre, descripcion = :descripcion, activo = :activo
            WHERE id = :id
        )");
        query.bindValue(":nombre", substage.nombre);
        query.bindValue(":descripcion", substage.descripcion);
        query.bindValue(":activo", substage.activo);
        query.bindValue(":id", substage.id);
        
        if (!query.exec()) {
            QMessageBox::critical(this, tr("Error"), 
                tr("Error al actualizar la subetapa: %1").arg(query.lastError().text()));
            return false;
        }
        
        substage.modificado = false;
    }
    
    return true;
}

bool DlgGestionEtapas::deleteSubstage(int substageId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM subetapas WHERE id = :id");
    query.bindValue(":id", substageId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al eliminar la subetapa: %1").arg(query.lastError().text()));
        return false;
    }
    
    return true;
}

bool DlgGestionEtapas::updateSubstageOrder(int substageId, int newOrder)
{
    QSqlQuery query;
    query.prepare("UPDATE subetapas SET orden = :orden WHERE id = :id");
    query.bindValue(":orden", newOrder);
    query.bindValue(":id", substageId);
    
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al actualizar el orden de la subetapa: %1").arg(query.lastError().text()));
        return false;
    }
    
    return true;
}

bool DlgGestionEtapas::validateStage()
{
    if (selectedStageIndex < 0 || selectedStageIndex >= stages.size()) {
        return false;
    }
    
    if (stages[selectedStageIndex].nombre.trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validación"), 
            tr("El nombre de la etapa es obligatorio"));
        return false;
    }
    
    if (stages[selectedStageIndex].codigo.trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validación"), 
            tr("El código de la etapa es obligatorio"));
        return false;
    }
    
    return true;
}

bool DlgGestionEtapas::validateSubstage()
{
    if (selectedSubstageIndex < 0 || selectedSubstageIndex >= substages.size()) {
        return false;
    }
    
    if (substages[selectedSubstageIndex].nombre.trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validación"), 
            tr("El nombre de la subetapa es obligatorio"));
        return false;
    }
    
    return true;
}
