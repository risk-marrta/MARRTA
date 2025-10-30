#include "newmodelwizard.h"
#include "ui_newmodelwizard.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QPushButton>
#include <QAbstractButton>
#include <QFile>
#include <QCoreApplication>
#include <QInputDialog>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QLabel>

NewModelWizard::NewModelWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewModelWizard),
    currentStep(0),
    creationSuccessful(false),
    nextButton(nullptr),
    backButton(nullptr)
{
    ui->setupUi(this);
    
    // Set default date to current date
    ui->edFecha->setDate(QDate::currentDate());
    
    // Add custom buttons to the button box
    nextButton = ui->buttonBox->addButton(tr("Siguiente"), QDialogButtonBox::ActionRole);
    backButton = ui->buttonBox->addButton(tr("Atrás"), QDialogButtonBox::ActionRole);
    
    connect(nextButton, &QPushButton::clicked, this, &NewModelWizard::on_btnNext_clicked);
    connect(backButton, &QPushButton::clicked, this, &NewModelWizard::on_btnBack_clicked);
    
    // Connect the Cancel button - when creation is successful, it accepts; otherwise it rejects
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NewModelWizard::on_cancelButtonClicked);

    // Set initial state
    ui->stackedWidget->setCurrentIndex(0);
    updateButtons();
}

NewModelWizard::~NewModelWizard()
{
    delete ui;
}

void NewModelWizard::updateButtons()
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
            // On creation page
            nextButton->setVisible(!creationSuccessful);
            nextButton->setEnabled(false);
        } else {
            nextButton->setVisible(true);
            nextButton->setEnabled(true);
            if (currentStep == 2) {
                nextButton->setText(tr("Crear modelo"));
            } else {
                nextButton->setText(tr("Siguiente"));
            }
        }
    }
    
    if (cancelButton) {
        if (currentStep == 3 && creationSuccessful) {
            cancelButton->setText(tr("Terminar"));
        } else {
            cancelButton->setText(tr("Cancelar"));
        }
    }
}

void NewModelWizard::on_btnBrowsePdb_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Guardar modelo MARRTA"),
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).last(),
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

bool NewModelWizard::validateCurrentStep()
{
    switch (currentStep) {
        case 0: // Step 1: Select destination file
            if (pdbFile.isEmpty()) {
                showError(tr("Por favor, seleccione la ubicación del archivo .pdb."));
                return false;
            }
            return true;
            
        case 1: // Step 2: Basic information
            if (ui->edNombre->text().trimmed().isEmpty()) {
                showError(tr("Por favor, ingrese el nombre del modelo."));
                return false;
            }
            // Store the values
            nombre = ui->edNombre->text().trimmed();
            fecha = ui->edFecha->date().toString("yyyy-MM-dd");
            tecnica = ui->edTecnica->text().trimmed();
            analistas = ui->edAnalistas->toPlainText().trimmed();
            institucion = ui->edInstitucion->text().trimmed();
            descripcion = ui->edDescripcion->toPlainText().trimmed();
            observaciones = ui->edObservaciones->toPlainText().trimmed();
            return true;
            
        case 2: // Step 3: Stages (optional)
            // Collect stages from tree widget
            stages.clear();
            for (int i = 0; i < ui->treeStages->topLevelItemCount(); i++) {
                QTreeWidgetItem *item = ui->treeStages->topLevelItem(i);
                StageInfo stage;
                stage.nombre = item->text(0);
                stage.codigo = item->text(1);
                stage.descripcion = item->data(0, Qt::UserRole).toString();
                
                // Collect substages
                for (int j = 0; j < item->childCount(); j++) {
                    QTreeWidgetItem *subItem = item->child(j);
                    SubstageInfo substage;
                    substage.nombre = subItem->text(0);
                    substage.descripcion = subItem->data(0, Qt::UserRole).toString();
                    stage.substages.append(substage);
                }
                
                stages.append(stage);
            }
            return true;
            
        default:
            return true;
    }
}

void NewModelWizard::showError(const QString &message)
{
    QMessageBox::warning(this, tr("Error de validación"), message);
}

void NewModelWizard::on_btnNext_clicked()
{
    if (!validateCurrentStep()) {
        return;
    }
    
    if (currentStep < 3) {
        currentStep++;
        ui->stackedWidget->setCurrentIndex(currentStep);
        
        if (currentStep == 3) {
            // Start creation immediately when reaching step 4
            startCreation();
        }
        
        updateButtons();
    }
}

void NewModelWizard::on_btnBack_clicked()
{
    if (currentStep > 0 && currentStep < 3) {
        currentStep--;
        ui->stackedWidget->setCurrentIndex(currentStep);
        updateButtons();
    }
}

void NewModelWizard::on_btnAddStage_clicked()
{
    addStageDialog();
}

void NewModelWizard::on_btnRemoveStage_clicked()
{
    QTreeWidgetItem *currentItem = ui->treeStages->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, tr("Eliminar"), tr("Por favor, seleccione una etapa o subetapa para eliminar."));
        return;
    }
    
    // If it's a top-level item (stage), remove it entirely
    if (currentItem->parent() == nullptr) {
        int index = ui->treeStages->indexOfTopLevelItem(currentItem);
        ui->treeStages->takeTopLevelItem(index);
        delete currentItem;
    } else {
        // It's a substage, just remove it from its parent
        QTreeWidgetItem *parent = currentItem->parent();
        parent->removeChild(currentItem);
        delete currentItem;
    }
}

void NewModelWizard::addStageDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Añadir Etapa"));
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QFormLayout *formLayout = new QFormLayout();
    
    QLineEdit *nameEdit = new QLineEdit(&dialog);
    QLineEdit *codeEdit = new QLineEdit(&dialog);
    QPlainTextEdit *descEdit = new QPlainTextEdit(&dialog);
    descEdit->setMaximumHeight(80);
    
    // Create labels with bold text for mandatory fields
    QLabel *nameLabel = new QLabel(tr("<b>Nombre:</b>"), &dialog);
    QLabel *codeLabel = new QLabel(tr("<b>Código:</b>"), &dialog);
    QLabel *descLabel = new QLabel(tr("Descripción:"), &dialog);
    
    formLayout->addRow(nameLabel, nameEdit);
    formLayout->addRow(codeLabel, codeEdit);
    formLayout->addRow(descLabel, descEdit);
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    // Custom handling for OK button to validate before accepting
    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        QString stageName = nameEdit->text().trimmed();
        QString stageCode = codeEdit->text().trimmed();
        
        if (stageName.isEmpty() || stageCode.isEmpty()) {
            QMessageBox::warning(&dialog, tr("Error"), tr("El nombre y código son obligatorios."));
            return; // Don't close dialog
        }
        
        dialog.accept();
    });
    
    layout->addWidget(buttonBox);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString stageName = nameEdit->text().trimmed();
        QString stageCode = codeEdit->text().trimmed();
        QString stageDesc = descEdit->toPlainText().trimmed();
        
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, stageName);
        item->setText(1, stageCode);
        item->setData(0, Qt::UserRole, stageDesc);
        ui->treeStages->addTopLevelItem(item);
        
        // Ask if want to add substages
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Subetapas"),
                                                                  tr("¿Quiere añadir subetapas a esta etapa?"),
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            addSubstageDialog(item);
        }
    }
}

void NewModelWizard::addSubstageDialog(QTreeWidgetItem *stageItem)
{
    if (!stageItem) {
        return;
    }
    
    while (true) {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Añadir Subetapa"));
        
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        
        QFormLayout *formLayout = new QFormLayout();
        
        QLineEdit *nameEdit = new QLineEdit(&dialog);
        QPlainTextEdit *descEdit = new QPlainTextEdit(&dialog);
        descEdit->setMaximumHeight(80);
        
        // Create labels with bold text for mandatory fields
        QLabel *nameLabel = new QLabel(tr("<b>Nombre:</b>"), &dialog);
        QLabel *descLabel = new QLabel(tr("Descripción:"), &dialog);
        
        formLayout->addRow(nameLabel, nameEdit);
        formLayout->addRow(descLabel, descEdit);
        
        layout->addLayout(formLayout);
        
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        
        // Custom handling for OK button to validate before accepting
        connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
            QString substageName = nameEdit->text().trimmed();
            
            if (substageName.isEmpty()) {
                QMessageBox::warning(&dialog, tr("Error"), tr("El nombre es obligatorio."));
                return; // Don't close dialog
            }
            
            dialog.accept();
        });
        
        layout->addWidget(buttonBox);
        
        if (dialog.exec() != QDialog::Accepted) {
            break;
        }
        
        QString substageName = nameEdit->text().trimmed();
        QString substageDesc = descEdit->toPlainText().trimmed();
        
        QTreeWidgetItem *subItem = new QTreeWidgetItem();
        subItem->setText(0, substageName);
        subItem->setData(0, Qt::UserRole, substageDesc);
        stageItem->addChild(subItem);
        stageItem->setExpanded(true);
        
        // Ask if want to add more substages
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Más Subetapas"),
                                                                  tr("¿Quiere añadir más subetapas a esta etapa?"),
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            break;
        }
    }
}

void NewModelWizard::on_btnMoveUp_clicked()
{
    QTreeWidgetItem *currentItem = ui->treeStages->currentItem();
    if (!currentItem) {
        return;
    }
    
    if (currentItem->parent() == nullptr) {
        // It's a top-level stage
        int index = ui->treeStages->indexOfTopLevelItem(currentItem);
        if (index > 0) {
            QTreeWidgetItem *item = ui->treeStages->takeTopLevelItem(index);
            ui->treeStages->insertTopLevelItem(index - 1, item);
            ui->treeStages->setCurrentItem(item);
        }
    } else {
        // It's a substage
        QTreeWidgetItem *parent = currentItem->parent();
        int index = parent->indexOfChild(currentItem);
        if (index > 0) {
            QTreeWidgetItem *item = parent->takeChild(index);
            parent->insertChild(index - 1, item);
            ui->treeStages->setCurrentItem(item);
        }
    }
}

void NewModelWizard::on_btnMoveDown_clicked()
{
    QTreeWidgetItem *currentItem = ui->treeStages->currentItem();
    if (!currentItem) {
        return;
    }
    
    if (currentItem->parent() == nullptr) {
        // It's a top-level stage
        int index = ui->treeStages->indexOfTopLevelItem(currentItem);
        if (index < ui->treeStages->topLevelItemCount() - 1) {
            QTreeWidgetItem *item = ui->treeStages->takeTopLevelItem(index);
            ui->treeStages->insertTopLevelItem(index + 1, item);
            ui->treeStages->setCurrentItem(item);
        }
    } else {
        // It's a substage
        QTreeWidgetItem *parent = currentItem->parent();
        int index = parent->indexOfChild(currentItem);
        if (index < parent->childCount() - 1) {
            QTreeWidgetItem *item = parent->takeChild(index);
            parent->insertChild(index + 1, item);
            ui->treeStages->setCurrentItem(item);
        }
    }
}

void NewModelWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && creationSuccessful) {
        // When successful, accept the dialog so the model can be loaded
        accept();
    } else {
        // Otherwise, reject (cancel)
        reject();
    }
}

void NewModelWizard::startCreation()
{
    ui->lblCreationStatus->setText(tr("Iniciando creación del modelo..."));
    ui->progressBar->setValue(0);
    QCoreApplication::processEvents();
    
    // Check if file exists and remove it
    if (QFile::exists(pdbFile)) {
        QFile exists(pdbFile);
        if (!exists.remove()) {
            ui->lblCreationStatus->setText(tr("Error: No se pudo eliminar el archivo existente."));
            creationSuccessful = false;
            updateButtons();
            return;
        }
    }
    
    ui->lblCreationStatus->setText(tr("Creando base de datos..."));
    ui->progressBar->setValue(20);
    QCoreApplication::processEvents();
    
    // Create the database
    bool result = createModelDatabase();
    
    ui->progressBar->setValue(100);
    QCoreApplication::processEvents();
    
    if (result) {
        ui->lblCreationStatus->setText(tr("¡Modelo creado correctamente!"));
        creationSuccessful = true;
    } else {
        ui->lblCreationStatus->setText(tr("Error al crear el modelo."));
        creationSuccessful = false;
    }
    
    updateButtons();
}

bool NewModelWizard::createModelDatabase()
{
    const QString DRIVER("QSQLITE");
    if(!QSqlDatabase::isDriverAvailable(DRIVER)) {
        QMessageBox::critical(this, tr("Error"), tr("Error en el conector de la base de datos"));
        return false;
    }
    
    QSqlDatabase database = QSqlDatabase::addDatabase(DRIVER, "new_model_wizard_db");
    database.setDatabaseName(pdbFile);
    
    if (!database.open()) {
        QMessageBox::critical(this, tr("Error"), tr("La base de datos no puede crearse"));
        return false;
    }
    
    database.transaction();
    QSqlQuery query(database);
    
    // Create configuration table
    if (!query.exec(R"""(
        CREATE TABLE configuracion (
            version      INTEGER,
            nombre       TEXT,
            tecnica      TEXT,
            analistas    TEXT,
            institucion  TEXT,
            fecha        VARCHAR(10),
            descripcion  TEXT,
            observaciones TEXT
        );
    )""")) {
        QMessageBox::critical(this, tr("Error"), tr("No se ha podido crear la tabla de configuración"));
        database.close();
        QSqlDatabase::removeDatabase("new_model_wizard_db");
        return false;
    }
    
    // Create stages table
    if (!query.exec(R"""(CREATE TABLE etapas (
                    id          INTEGER      NOT NULL
                                             PRIMARY KEY AUTOINCREMENT,
                    nombre      VARCHAR (50) NOT NULL
                                             UNIQUE,
                    descripcion TEXT,
                    orden       INTEGER      NOT NULL
                                             DEFAULT 1,
                    activo      BOOLEAN      NOT NULL
                                             DEFAULT (1),
                    codigo      CHAR (3)
                );)""")) {
        QMessageBox::critical(this, tr("Error"), tr("No se ha podido crear la tabla de etapas"));
        database.close();
        QSqlDatabase::removeDatabase("new_model_wizard_db");
        return false;
    }
    
    // Create substages table
    if (!query.exec(R"""(CREATE TABLE subetapas (
                    id          INTEGER NOT NULL
                                        PRIMARY KEY AUTOINCREMENT,
                    nombre      TEXT    NOT NULL,
                    descripcion TEXT,
                    orden       INTEGER,
                    activo      INTEGER NOT NULL
                                        DEFAULT 1,
                    etapa_id    INTEGER NOT NULL,
                    FOREIGN KEY (
                        etapa_id
                    )
                    REFERENCES etapas (id) ON DELETE CASCADE
                                           ON UPDATE CASCADE
                );)""")) {
        QMessageBox::critical(this, tr("Error"), tr("No se ha podido crear la tabla de subetapas"));
        database.close();
        QSqlDatabase::removeDatabase("new_model_wizard_db");
        return false;
    }
    
    // Create events table
    if (!query.exec(R"""(CREATE TABLE suceso_iniciador (
                    id           INTEGER     PRIMARY KEY AUTOINCREMENT,
                    nombre       TEXT        NOT NULL,
                    descripcion  TEXT,
                    codigo       TEXT        NOT NULL,
                    frecuencia   VARCHAR (3) NOT NULL,
                    consecuencia VARCHAR (3) NOT NULL,
                    etapa_id     INTEGER     NOT NULL,
                    subetapa_id  INTEGER,
                    error_humano BOOLEAN     NOT NULL
                                             DEFAULT false,
                    riesgo       VARCHAR (3),
                    base         BOOLEAN     NOT NULL
                                             DEFAULT (0),
                    activo       BOOLEAN     NOT NULL
                                             DEFAULT (1),
                    completado   BOOLEAN     DEFAULT (false),
                    CONSTRAINT unique_id UNIQUE (id)
                );)""")) {
        QMessageBox::critical(this, tr("Error"), tr("No se ha podido crear la tabla de sucesos iniciadores"));
        database.close();
        QSqlDatabase::removeDatabase("new_model_wizard_db");
        return false;
    }
    
    // Create defenses table
    if (!query.exec(R"""(CREATE TABLE defensa (
                    id                  INTEGER       PRIMARY KEY AUTOINCREMENT,
                    suceso_iniciador_id INTEGER       REFERENCES suceso_iniciador (id) ON DELETE CASCADE
                                                                                       ON UPDATE CASCADE,
                    "codigo"            VARCHAR (25),
                    nombre              VARCHAR (250) NOT NULL,
                    descripcion         TEXT,
                    robustez            INT           NOT NULL
                                                      DEFAULT (0)
                                                      CHECK (robustez >= -1 AND
                                                             robustez < 4),
                    tipo                INTEGER       CHECK (tipo >= 0 AND
                                                             tipo < 3)
                                                      NOT NULL
                                                      DEFAULT (0),
                    activo              BOOLEAN       DEFAULT (0),
                    etapa_orden_id      INTEGER       REFERENCES etapas (id) ON DELETE SET NULL
                                                                             ON UPDATE CASCADE
                );)""")) {
        QMessageBox::critical(this, tr("Error"), tr("No se ha podido crear la tabla de defensas"));
        database.close();
        QSqlDatabase::removeDatabase("new_model_wizard_db");
        return false;
    }
    
    ui->progressBar->setValue(40);
    QCoreApplication::processEvents();
    
    // Insert configuration
    query.prepare("INSERT INTO configuracion (version, nombre, tecnica, analistas, institucion, fecha, descripcion, observaciones) VALUES (:version, :nombre, :tecnica, :analistas, :institucion, :fecha, :descripcion, :observaciones)");
    query.bindValue(":version", 1);
    query.bindValue(":nombre", nombre);
    query.bindValue(":tecnica", tecnica);
    query.bindValue(":analistas", analistas);
    query.bindValue(":institucion", institucion);
    query.bindValue(":fecha", fecha);
    query.bindValue(":descripcion", descripcion);
    query.bindValue(":observaciones", observaciones);
    
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Error"), tr("No se ha podido insertar la configuración: ") + query.lastError().text());
        database.close();
        QSqlDatabase::removeDatabase("new_model_wizard_db");
        return false;
    }
    
    ui->progressBar->setValue(60);
    QCoreApplication::processEvents();
    
    // Insert stages and substages
    if (!stages.isEmpty()) {
        query.prepare("INSERT INTO etapas (nombre, descripcion, orden, activo, codigo) VALUES(?, ?, ?, 1, ?)");
        
        for (int i = 0; i < stages.size(); i++) {
            const StageInfo &stage = stages[i];
            
            query.bindValue(0, stage.nombre);
            query.bindValue(1, stage.descripcion);
            query.bindValue(2, i);
            query.bindValue(3, stage.codigo);
            
            if (!query.exec()) {
                QMessageBox::critical(this, tr("Error"), tr("No se ha podido insertar la etapa: ") + query.lastError().text());
                database.close();
                QSqlDatabase::removeDatabase("new_model_wizard_db");
                return false;
            }
            
            // Get the last inserted stage ID
            int stageId = query.lastInsertId().toInt();
            
            // Insert substages
            if (!stage.substages.isEmpty()) {
                QSqlQuery substageQuery(database);
                substageQuery.prepare("INSERT INTO subetapas (nombre, descripcion, orden, activo, etapa_id) VALUES(?, ?, ?, 1, ?)");
                
                for (int j = 0; j < stage.substages.size(); j++) {
                    const SubstageInfo &substage = stage.substages[j];
                    
                    substageQuery.bindValue(0, substage.nombre);
                    substageQuery.bindValue(1, substage.descripcion);
                    substageQuery.bindValue(2, j);
                    substageQuery.bindValue(3, stageId);
                    
                    if (!substageQuery.exec()) {
                        QMessageBox::critical(this, tr("Error"), tr("No se ha podido insertar la subetapa: ") + substageQuery.lastError().text());
                        database.close();
                        QSqlDatabase::removeDatabase("new_model_wizard_db");
                        return false;
                    }
                }
            }
        }
    }
    
    ui->progressBar->setValue(80);
    QCoreApplication::processEvents();
    
    database.commit();
    database.close();
    QSqlDatabase::removeDatabase("new_model_wizard_db");
    
    return true;
}
