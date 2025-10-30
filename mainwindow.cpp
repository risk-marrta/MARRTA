#include "mainwindow.h"
const int MainWindow::MaxRecentFiles;
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardPaths>
#include <QXmlStreamWriter>
#include <QLayout>
#include <QCloseEvent>   //apm
#include <QFile>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QRegularExpression>

#include <iostream>
#include <bitset>
#include <set>

#include "waitingspinnerwidget.h"

#include "styles.h"

#include "resumen.h"
#include "calculodefensa.h"

#include "dlgerror.h"
#include "dlgconfiginforme.h"
#include "dlgsendfeedback.h"
#include "dlgdisclaimer.h"
#include "dlglicense.h"
#include "dlgcomparadormodelos.h"

#include "dbevent.h"
#include "dbdefense.h"
#include "dbelementstage.h"

#include "scriptingeditor.h"
#include "pythonscriptingeditor.h"
#include "listabarreras.h"
#include "usermanualdocumentation.h"

#include "widget/completedelegate.h"
#include "defenseitemdelegate.h"

#include "modelos/empty.h"
#include "modelos/marr.h"
#include "modelos/xlsx.h"

#include "informacionmodelo.h"
#include "importwizard.h"
#include "newmodelwizard.h"
#include "flujogramagenerator.h"

using namespace Modelos;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    actualCompleteEvent(nullptr),
    eventosModel(nullptr)
{
    ui->setupUi(this);

    // Apply minimal stylesheet for QListView with softer alternating colors
    // Using system colors for selection/hover, only customizing alternating rows
    QString listViewStyle = R"(
        QListView {
            alternate-background-color: #f5f5f5;
        }
    )";
    
    ui->lReductoresFrecuencia->setStyleSheet(listViewStyle);
    ui->lBarreras->setStyleSheet(listViewStyle);
    ui->lReductoresConsecuencia->setStyleSheet(listViewStyle);
    ui->eventos->setStyleSheet(listViewStyle);
    ui->stages->setStyleSheet(listViewStyle);

    // Set custom delegate for defense lists to properly align hardiness on the right
    DefenseItemDelegate *defenseDelegate = new DefenseItemDelegate(this);
    ui->lReductoresFrecuencia->setItemDelegate(defenseDelegate);
    ui->lBarreras->setItemDelegate(defenseDelegate);
    ui->lReductoresConsecuencia->setItemDelegate(defenseDelegate);

    #ifdef Q_OS_MACOS

    ui->actionSalir->setVisible(false);

    #endif

    botonEtapas = new OrientablePushButton("Etapas");
    botonEtapas->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    botonEtapas->setOrientation(OrientablePushButton::Orientation::VerticalTopToBottom);
    botonEtapas->resize(20, 80);
    botonEtapas->move(0, 80);
    botonEtapas->setStyleSheet("background-color: #0077ff;color:#fdfdfd;");
    layout()->addWidget(botonEtapas);

    /*ui->lateralFrame->setGeometry(0, 0, 0, ui->lateralFrame->height());
    ui->lateralFrame->setMinimumWidth(0);
    ui->lateralFrame->setMaximumWidth(0);*/
    animacionEtapas = new QPropertyAnimation(ui->lateralFrame, "size");
    animacionEtapas->setDuration(300);
    connect(botonEtapas, &QPushButton::clicked, [=](){
        if (ui->lateralFrame->width() != 400) {
            animacionEtapas->setStartValue(QSize(0, ui->lateralFrame->height()));
            animacionEtapas->setEndValue(QSize(400, ui->lateralFrame->height()));
            ui->lateralFrame->setMaximumWidth(400);
            ui->lateralFrame->setMinimumWidth(400);
        } else {
            animacionEtapas->setStartValue(QSize(400, ui->lateralFrame->height()));
            animacionEtapas->setEndValue(QSize(0, ui->lateralFrame->height()));
            ui->lateralFrame->setMinimumWidth(0);
            ui->lateralFrame->setMaximumWidth(0);
        }

        animacionEtapas->start();
    });

    ui->cbFiltroFrecuencia->addItem("*", QVariant::fromValue(-1));
    for(auto e : MatrizRiesgo::Frecuencias) {
        ui->cbFiltroFrecuencia->addItem(e.second, QVariant::fromValue(e.first));
    }
    ui->cbFiltroConsecuencia->addItem("*", QVariant::fromValue(-1));
    for(auto e: MatrizRiesgo::Consecuencias) {
        ui->cbFiltroConsecuencia->addItem(e.second, QVariant::fromValue(e.first));
    }
    ui->cbFiltroRiesgo->addItem("*", QVariant::fromValue(-1));
    for(auto e : MatrizRiesgo::Riesgos) {
        ui->cbFiltroRiesgo->addItem(e.second, QVariant::fromValue(e.first));
    }

    ui->statusBar->hide();

    const QString DRIVER("QSQLITE");
    if(!QSqlDatabase::isDriverAvailable(DRIVER))
    {
        if (settings.debugMode) {
            std::cout << "Driver de la Base de Datos no válido" << std::endl;
            exit(-1);
        }
    }
    this->praxisDb = QSqlDatabase::addDatabase(DRIVER);
    setupScripting();
    setupRecentFilesMenu();
    
    // Initialize update checker
    updateChecker = new UpdateChecker(this);
}

MainWindow::~MainWindow()
{
    if (settings.debugMode) {
        std::cout << "Cerrando la aplicación" << std::endl;
    }

    // @moises: animacionEtapas es un smart pointer
    //delete animacionEtapas;

    if (praxisDb.isOpen()){
        praxisDb.close();
    }
    // @moises: actualItem es un smart pointer
    //if (actualItem != nullptr) {
    //    delete actualItem;
    //}

    if (actualCompleteEvent != nullptr) {
        delete actualCompleteEvent;
    }

    saveSettings();

    delete ui;
}

/**
 * Inicializa el entorno de script
 *
 * @brief MainWindow::setupScripting
 */
void MainWindow::setupScripting()
{
    scriptingEngine = new ScriptingEngine(this);
    pythonScriptingEngine = new PythonScriptingEngine(this);
}

/**
 * Check for application updates (manual check triggered by user)
 *
 * @brief MainWindow::on_actionComprobarActualizaciones_triggered
 */
void MainWindow::on_actionComprobarActualizaciones_triggered()
{
    updateChecker->checkForUpdates(false);
}

/**
 * Check for new models (manual check triggered by user)
 *
 * @brief MainWindow::on_actionComprobarNuevosModelos_triggered
 */
void MainWindow::on_actionComprobarNuevosModelos_triggered()
{
    updateChecker->checkForNewModels(false);
}

/**
 * Open dialog to send feedback/bug report to GitHub
 *
 * @brief MainWindow::on_actionEnviarComentario_triggered
 */
void MainWindow::on_actionEnviarComentario_triggered()
{
    DlgSendFeedback dialog(this);
    dialog.exec();
}

/**
 * Open dialog to send risk model to MARRTA team
 * (Placeholder for future implementation)
 *
 * @brief MainWindow::on_actionEnviarModeloRiesgo_triggered
 */
void MainWindow::on_actionEnviarModeloRiesgo_triggered()
{
    QMessageBox::information(
        this,
        tr("Funcionalidad en desarrollo"),
        tr("La funcionalidad de enviar modelos de riesgo estará disponible en una próxima versión.\n\n"
           "Por favor, contacte directamente con el equipo de MARRTA si desea compartir su modelo.\n\n"
           "Email de contacto: matrices.marrta@gmail.com")
    );
}

/**
 * Open help documentation
 *
 * @brief MainWindow::on_actionAyuda_triggered
 */
void MainWindow::on_actionAyuda_triggered()
{
    UserManualDocumentation *helpDialog = new UserManualDocumentation(this);
    helpDialog->exec();
    delete helpDialog;
}

/**
 * Show legal notice (disclaimer)
 *
 * @brief MainWindow::on_actionAvisoLegal_triggered
 */
void MainWindow::on_actionAvisoLegal_triggered()
{
    DlgDisclaimer disclaimerDlg(this);
    disclaimerDlg.exec();
}

/**
 * Show usage license (GPLv3)
 *
 * @brief MainWindow::on_actionLicenciaUso_triggered
 */
void MainWindow::on_actionLicenciaUso_triggered()
{
    DlgLicense licenseDlg(this);
    licenseDlg.exec();
}

void MainWindow::on_actionListaBarreras_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    ListaBarreras *listaBarreras = new ListaBarreras(this);
    
    // Conectar señal para refrescar pantalla cuando cambien defensas
    connect(listaBarreras, &ListaBarreras::defensesChanged, this, &MainWindow::refreshMainScreen);
    
    listaBarreras->exec();
    delete listaBarreras;
}

void MainWindow::on_actionComparadorModelos_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    DlgComparadorModelos *comparador = new DlgComparadorModelos(
        settings.lastFile,
        configuracionModelo,
        &events,
        this
    );
    
    comparador->exec();
    delete comparador;
}

/**
 * Carga la configuración a partir de las especificaciones creadas en el archivo main.cpp
 *
 * @brief MainWindow::loadSettings
 * @param s
 */
void MainWindow::loadSettings(QSettings *s)
{
    if (s != nullptr) {
        this->qSettings = s;
    } else {
        this->qSettings = new QSettings("VORTICE", "MARRTA");
    }

    settings.debugMode = this->qSettings->value("debugMode", false).toBool();
    settings.createBackupOnOpen = this->qSettings->value("createBackup", false).toBool();
    settings.disclaimerAccepted = this->qSettings->value("disclaimerAccepted", false).toBool();
    settings.lastFile = this->qSettings->value("lastPraxis", "").toString();
    
    // Load recent files list
    settings.recentFiles = this->qSettings->value("recentFiles", QStringList()).toStringList();
    // Remove any non-existent files from the recent list
    QStringList validRecentFiles;
    for (const QString &file : settings.recentFiles) {
        if (QFile::exists(file)) {
            validRecentFiles.append(file);
        }
    }
    settings.recentFiles = validRecentFiles;

    QString databaseFolder = this->qSettings->value("databaseFolder", "").toString();
    // Comprobar que databaseFolder es válido
    if (databaseFolder.isEmpty()) {
        #ifdef Q_OS_MACOS
            settings.databaseFolder = QCoreApplication::applicationDirPath() + "/../../..";
        #else
            settings.databaseFolder = QCoreApplication::applicationDirPath();
        #endif
    } else {
        settings.databaseFolder = databaseFolder;
    }
    QDir dir(settings.databaseFolder);

    if (!dir.isReadable()) {
        QMessageBox msgBox;
        msgBox.setText("No se puede localizar la carpeta de modelos. Compruebe que existe la carpeta.");
        msgBox.exec();
        exit(-1);
    }

    QString reportFolder = this->qSettings->value("reportFolder", "").toString();

    // Comprobar que reportFolder es válido
    if (reportFolder.isEmpty()) {
        #ifdef Q_OS_MACOS
            settings.reportFolder = QCoreApplication::applicationDirPath() + "/../../../reports";
        #else
            settings.reportFolder = QCoreApplication::applicationDirPath() + "/reports";
        #endif
    } else {
        settings.reportFolder = reportFolder;
    }
    dir.setPath(settings.reportFolder);

        if (!dir.exists()) {
        if (!dir.mkpath(".")) { // Try to create the folder
            QMessageBox msgBox;
            msgBox.setText("No se puede crear la carpeta de informes. Compruebe permisos de escritura.");
            msgBox.exec();
            exit(-1);
        }
    };

    if (!dir.isReadable()) {
        QMessageBox msgBox;
        msgBox.setText("No se puede localizar la carpeta de informes. Compruebe que existe la carpeta 'reports'.");
        msgBox.exec();
        exit(-1);
    }
    
    // Perform automatic update checks if needed (every 2 weeks)
    if (updateChecker) {
        updateChecker->autoCheckOnStartup(this->qSettings);
    }
}

void MainWindow::saveSettings()
{
    //if (settings.lastFile != "") {
        qSettings->setValue("debugMode", settings.debugMode);
        qSettings->setValue("lastPraxis", settings.lastFile);
        qSettings->setValue("createBackup", settings.createBackupOnOpen);
        qSettings->setValue("disclaimerAccepted", settings.disclaimerAccepted);
        qSettings->setValue("databaseFolder", settings.databaseFolder);
        qSettings->setValue("reportFolder", settings.reportFolder);
        qSettings->setValue("recentFiles", settings.recentFiles);
        qSettings->sync();
    //}
}

/**
 * @brief MainWindow::on_mnuOpen_triggered
 *
 * 2022-09-12 Added parameter to configure if the backup file should be done
 */
void MainWindow::on_mnuOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir modelo"), settings.databaseFolder, "Modelo Marrta (*.pdb)");
    openPraxisFile(fileName);
}

bool MainWindow::openPraxisFile(QString filename)
{
    if (filename.isEmpty()) {
        return false;
    }

    if (settings.createBackupOnOpen) {
        QString fecha = QDateTime::currentDateTime().toString("yyyy.MM.dd - hh.mm.ss");
        QString backupfilename = filename.left(filename.indexOf(".")) + " - (" + fecha + ").pdb";

        if (!QFile::copy(filename, backupfilename)) {
            return false;
        }
    }

    if (settings.debugMode) {
        std::cout << "Abriendo modelo en " << filename.toStdString() << std::endl;
    }

    //qDebug() << filename;
    this->praxisDb.setDatabaseName(filename);
    if (!this->praxisDb.open())
    {
        if (settings.debugMode) {
            qDebug() << "No se puede localizar el archivo o no es una base de datos SQLite";
        }
    }

    try {
        configuracionModelo = Database::DbManager::checkDatabase(settings.debugMode);
        setWindowTitle(configuracionModelo.nombre + " - " + filename);
    } catch (const char* msg) {
        QMessageBox msgBox;
        msgBox.setText(msg);
        msgBox.exec();
        return false;
    }

    limpiarFiltros();
    events.clear();
    loadModel();

    findChild<QAction *>("mnuClose")->setDisabled(false);

    // After opening the file, its name is saved for the next session
    settings.lastFile = filename;
    
    // Add to recent files and update the menu
    updateRecentFiles(filename);

    return true;
}

CompleteEventDefinition * MainWindow::buscarEventoPorId(int id)
{
    // If all the stages is shown, all the events have to be checked
    if (ui->btTodasEtapas->isChecked() || comprobarUsarFiltro()) {
        for(auto &l: events) {
            for (auto &e: l.second) {
                if (e.id == id) {
                    return &e;
                }
            }
        }
    } else {
        std::string index = std::to_string(actualElementId.first) + "-" + std::to_string(actualElementId.second);
        if (actualStageIndex == index) {
            //qDebug() << (*std::next(events.begin(), id)).first.c_str();
            for (auto &e : events[actualStageIndex]) {
                if (e.id == id) {
                    return &e;
                }
            }
            //return events.at(id);
        } else {
            for (int i = 0; i <= actualElementId.second; i++){
                for (auto &e : events[std::to_string(actualElementId.first) + "-" + std::to_string(i)]) {
                    if (e.id == id) {
                        return &e;
                    }
                }
            }
        }
    }

    return nullptr;
}

void MainWindow::limpiarFiltros()
{
    ui->edFiltroNombre->setText("");
    ui->edFiltroDescripcion->setText("");
    ui->cbFiltroFrecuencia->setCurrentIndex(0);
    ui->cbFiltroConsecuencia->setCurrentIndex(0);
    ui->cbFiltroRiesgo->setCurrentIndex(0);
    ui->cbAplicables->setCurrentIndex(0);
    ui->cbErroresHumanos->setCurrentIndex(0);
}

bool MainWindow::comprobarUsarFiltro()
{
    return ((ui->edFiltroNombre->text() != "")
            || (ui->edFiltroDescripcion->text() != "")
            || (ui->cbFiltroFrecuencia->currentData().toInt() >= 0)
            || (ui->cbFiltroConsecuencia->currentData().toInt() >= 0)
            || (ui->cbFiltroRiesgo->currentData().toInt() >= 0)
            || (ui->cbAplicables->currentIndex() > 0)
            || (ui->cbErroresHumanos->currentIndex() > 0)
    );
}

bool MainWindow::comprobarCumpleFiltro(const CompleteEventDefinition &evento)
{
    std::bitset<7> filters;

    if (!ui->edFiltroNombre->text().isEmpty()) {
        if (evento.nombre.contains(ui->edFiltroNombre->text(), Qt::CaseInsensitive)) {
            filters.set(0, true);
        }
    } else {
        filters.set(0, true);
    }
    if (!ui->edFiltroDescripcion->text().isEmpty()) {
        if (evento.descripcion.contains(ui->edFiltroDescripcion->text(), Qt::CaseInsensitive)) {
            filters.set(1, true);
        }
    } else {
        filters.set(1, true);
    }
    if (ui->cbFiltroFrecuencia->currentData().toInt() >= 0) {
        if (evento.frecuenciaCalculada == ui->cbFiltroFrecuencia->currentData().toInt()) {
            filters.set(2, true);
        }
    } else {
        filters.set(2, true);
    }
    if (ui->cbFiltroConsecuencia->currentData().toInt() >= 0) {
        if (evento.consecuenciaCalculada == ui->cbFiltroConsecuencia->currentData().toInt()) {
            filters.set(3, true);
        }
    } else {
        filters.set(3, true);
    }
    if (ui->cbFiltroRiesgo->currentData().toInt() >= 0) {
        if (evento.riesgo == ui->cbFiltroRiesgo->currentData().toInt()) {
            filters.set(4, true);
        }
    } else {
        filters.set(4, true);
    }

    if ((ui->cbAplicables->currentIndex() == 0)
            || (ui->cbAplicables->currentIndex() == 1 && evento.completado)
            || (ui->cbAplicables->currentIndex() == 2 && !evento.completado)
    ) {
        filters.set(5, true);
    }

    if ((ui->cbErroresHumanos->currentIndex() == 0)
            || (ui->cbErroresHumanos->currentIndex() == 1 && evento.errorHumano)
            || (ui->cbErroresHumanos->currentIndex() == 2 && !evento.errorHumano)
    ) {
        filters.set(6, true);
    }

    return filters.all();
}

void MainWindow::limpiarDetalles()
{
    ui->nombreEvento->setText("");
    ui->descripcionEvento->setText("");
    ui->etRiesgoMaximo->setText("");
    ui->etRiesgoMaximo->setStyleSheet("");
    ui->etRiesgo->setText("");
    ui->etRiesgo->setStyleSheet("");
    ui->etFrecuenciaBase->setText("");
    ui->etFrecuenciaBase->setStyleSheet("");
    ui->etFrecuencia->setText("");
    ui->etFrecuencia->setStyleSheet("");
    ui->etProbabilidad->setText("");
    ui->etProbabilidad->setStyleSheet("");
    ui->etConsecuenciaBase->setText("");
    ui->etConsecuenciaBase->setStyleSheet("");
    ui->etConsecuencia->setText("");
    ui->etConsecuencia->setStyleSheet("");
    ui->lReductoresFrecuencia->setModel(nullptr);
    ui->lReductoresConsecuencia->setModel(nullptr);
    ui->lBarreras->setModel(nullptr);
    
    // Clear defense counters
    ui->lblCountReductoresFrecuencia->setText("[0/0]");
    ui->lblCountBarreras->setText("[0/0]");
    ui->lblCountReductoresConsecuencia->setText("[0/0]");
}

void MainWindow::loadModel()
{
    QList<EtapaDefinition> etapasDb;
    QList<SubetapaDefinition> subetapasDb;
    model = new QStandardItemModel();

    auto eventObject = Database::DbEvent();
    try {
        eventObject.getAllInfo(matrizRiesgo, events);
    } catch (...) {
            QMessageBox msgBox;
            msgBox.setText("Error al obtener la información de la base de datos. Puede ser que esté corrupta.");
            msgBox.exec();
            return;
    }

    auto stages = Database::DbElementStage();
    try {
        stages.list(etapasDb);
    } catch (const Database::DbException& exception) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.setText(exception.what());
        msgBox.exec();

        return;
    }
    if (etapasDb.count() > 0) {
        for (auto stage : etapasDb) {
            QStandardItem *item = new QStandardItem("[" + stage.codigo + "] " + stage.nombre);
            ElementIndexType elementId(ElementType::Scene, std::make_pair(stage.id, 0));
            item->setData(QVariant::fromValue(elementId));
            item->setData(QVariant::fromValue(stage.nombre), Qt::ToolTipRole);

            // Check if stage has any events
            std::string stageIndex = std::to_string(stage.id) + "-0";
            bool hasEvents = events.find(stageIndex) != events.end() && !events[stageIndex].empty();

            auto substages = Database::DbElementSubstage(stage.id);
            substages.list(subetapasDb);

            // Also check substages for events
            bool hasSubstageEvents = false;
            for (auto sub : subetapasDb) {
                std::string substageIndex = std::to_string(stage.id) + "-" + std::to_string(sub.id);
                if (events.find(substageIndex) != events.end() && !events[substageIndex].empty()) {
                    hasSubstageEvents = true;
                    break;
                }
            }

            // If neither stage nor its substages have events, set gray color
            if (!hasEvents && !hasSubstageEvents) {
                item->setForeground(QBrush(QColor(128, 128, 128))); // Gray color
            }

            model->appendRow(item);

            for (auto sub : subetapasDb) {
                QStandardItem *subitem = new QStandardItem(sub.nombre);
                subitem->setData(QVariant::fromValue(sub.nombre), Qt::ToolTipRole);
                ElementIndexType subelementId(ElementType::Subscene, std::make_pair(stage.id, sub.id));
                subitem->setData(QVariant::fromValue(subelementId), Qt::UserRole + 1);

                // Check if substage has any events
                std::string substageIndex = std::to_string(stage.id) + "-" + std::to_string(sub.id);
                if (events.find(substageIndex) == events.end() || events[substageIndex].empty()) {
                    subitem->setForeground(QBrush(QColor(128, 128, 128))); // Gray color
                }

                item->appendRow(subitem);
            }
        }
    }

    mostrarEventosSeleccion();
    eventosModel->clear();
    ui->etEtapaActual->setText("");
    ui->stages->setModel(model);
    auto selection_model = ui->stages->selectionModel();
    connect(selection_model, &QItemSelectionModel::currentChanged, this, &MainWindow::slot_stages_selection_changed);

    scriptingEngine->setCurrentDb(model);

    eventosModel = nullptr;
    actualStageIndex = "";

    reductoresFrecuencia = nullptr;
    barreras = nullptr;
    reductoresConsecuencias = nullptr;
}

void MainWindow::eventoAModelo(QStandardItemModel *model, CompleteEventDefinition event)
{
    Q_UNUSED(model);
    auto item = new QStandardItem(tr("[%1] %2").arg(event.codigo, event.nombre));
    item->setData(QVariant::fromValue(event.id), Qt::UserRole);
    
    // ** apm ** Formateo del tooltip para que el texto se divida en líneas de aproximadamente 100 caracteres,
    // rompiendo por palabras completas. Esto evita que el tooltip se expanda horizontalmente más allá
    // de lo deseado y mejora su legibilidad, especialmente en configuraciones multi-monitor.
    QString tooltipText = event.descripcion;
    QString formattedTooltip;
    int lineLength = 0;
    QStringList words = tooltipText.split(' ');
    
    for (const QString& word : words) {
        if (lineLength + word.length() > 100) {
            formattedTooltip += "\n";
            lineLength = 0;
        }
        formattedTooltip += word + " ";
        lineLength += word.length() + 1;
    }
    
    item->setToolTip(formattedTooltip.trimmed());

    // Check if the event has been marked as finished
    if (event.completado) {
        item->setData(QVariant::fromValue(event.riesgo), RoleRisk);
    } else {
        item->setData(QVariant::fromValue(-1), RoleRisk);
    }
    item->setData(QVariant::fromValue(event.etapa), RoleStage);
    item->setData(QVariant::fromValue(event.subetapa), RoleSubstage);
    item->setData(QVariant::fromValue(event.nombre), RoleName);
    eventosModel->appendRow(item);
}

void MainWindow::mostrarEventosSeleccion()
{
    if (eventosModel != nullptr) {
        delete eventosModel;
    }
    eventosModel = new QStandardItemModel(this);

    bool filtrar = comprobarUsarFiltro();
    if (ui->btTodasEtapas->isChecked()) {
        // Show all events from all stages
        std::map<QString, CompleteEventDefinition> todosEventos;

        for(auto l: events) {
            for (auto e: l.second) {
                if (filtrar) {
                    if (comprobarCumpleFiltro(e)) {
                        todosEventos[e.codigo] = e;
                    }
                } else {
                    todosEventos[e.codigo] = e;
                }
            }
        }

        // apm - Ordenamos por "etapa" los sucesos (issue #62):
        std::vector<std::pair<QString, CompleteEventDefinition>> todosEventosOrdenado(todosEventos.begin(), todosEventos.end());
        std::sort(todosEventosOrdenado.begin(), todosEventosOrdenado.end(),
                  [](const std::pair<QString, CompleteEventDefinition>& a, const std::pair<QString, CompleteEventDefinition>& b) {
            if (a.second.etapa != b.second.etapa)
                return a.second.etapa < b.second.etapa;
            if (a.second.subetapa != b.second.subetapa)
                return a.second.subetapa < b.second.subetapa;
            return a.second.id < b.second.id;
                  });
        for(auto e : todosEventosOrdenado) {
            eventoAModelo(eventosModel, e.second);
        }
    } else {
        // Show events from selected stage/substage, with optional filtering
        // Vamos a comprobar si es una etapa con subetapas
        if (!actualStageIndex.empty() && (actualElementId.second == 0)) {
            // Primero cargamos los SI de la etapa
            for (auto event : events[actualStageIndex]) {
                if (!filtrar || comprobarCumpleFiltro(event)) {
                    eventoAModelo(eventosModel, event);
                }
            }

            // Y si tiene subetapas, cargamos los de cada una de ellas
            if (actualItem->hasChildren()) {
                for (int i = 0; i < actualItem->rowCount(); i++) {
                    ElementIndexType element = actualItem->child(i)->data().value<ElementIndexType>();

                    for (auto event : events[std::to_string(element.second.first) + "-" + std::to_string(element.second.second)]) {
                        if (!filtrar || comprobarCumpleFiltro(event)) {
                            eventoAModelo(eventosModel, event);
                        }
                    }
                }
            }
        } else {
            for (auto event : events[actualStageIndex]) {
                if (!filtrar || comprobarCumpleFiltro(event)) {
                    eventoAModelo(eventosModel, event);
                }
            }
        }
    }

    ui->eventos->setItemDelegate(new CompleteDelegate);
    ui->eventos->setModel(eventosModel);

    limpiarDetalles();

    // Auto-select first item if there are any events
    if (eventosModel->rowCount() > 0) {
        QModelIndex firstIndex = eventosModel->index(0, 0);
        ui->eventos->setCurrentIndex(firstIndex);
        
        // Trigger the event selection logic
        etapaSubetapaSeleccionada = firstIndex.data(RoleStage).toString() + "-" + firstIndex.data(RoleSubstage).toString();
        QString nombre = firstIndex.data(RoleName).toString();
        
        auto listaEventos = events.find(etapaSubetapaSeleccionada.toStdString())->second;
        for (auto evento : listaEventos) {
            if (evento.nombre == nombre) {
                eventoActualId = evento.id;
                if (actualCompleteEvent != nullptr) {
                    delete actualCompleteEvent;
                }
                actualCompleteEvent = new CompleteEventDefinition(&evento);
                mostrarEvento();
                break;
            }
        }
    }
}

/**
 * @brief MainWindow::on_stages_selection_changed
 * Llamado cuando se pulsa en una etapa o subetapa.
 *
 * Cambia la etiqueta de la etapa/subetapa actual
 *
 * @param index
 * @param previous
 */
void MainWindow::slot_stages_selection_changed(const QModelIndex &index, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    actualItem = model->itemFromIndex(index);

    ElementIndexType elementId = actualItem->data().value<ElementIndexType>();
    actualElementId = elementId.second;
    actualStageIndex = std::to_string(actualElementId.first) + "-" + std::to_string(actualElementId.second);

    QString ruta;
    // Tiene padre
    auto parent = actualItem->parent();
    if (parent != nullptr) {
        ruta = parent->data(Qt::DisplayRole).toString() + " / ";
    }
    ruta += actualItem->data(Qt::DisplayRole).toString();
    ui->etEtapaActual->setText(ruta);

    mostrarEventosSeleccion();
    
    // Update event counter after displaying events
    actualizarContadorEventos();
}

/**
 * @brief MainWindow::mostrarEvento
 * Mostrar los detalles de un evento. Calcula las barreras y muestra los resultados.
 */
void MainWindow::mostrarEvento()
{
    if (actualCompleteEvent != nullptr && actualCompleteEvent->id > 0) {
        ui->descripcionEvento->setText(actualCompleteEvent->descripcion);
        ui->nombreEvento->setText("[" + actualCompleteEvent->codigo + "] " + actualCompleteEvent->nombre);
        //ui->codigoEvento->setText(actualCompleteEvent->codigo);

        actualCompleteEvent->calcularReductores();
        mostrarFrecuencia(*actualCompleteEvent);
        mostrarProbabilidad(*actualCompleteEvent);
        mostrarConsecuencia(*actualCompleteEvent);
        mostrarRiesgo();

        mostrarReductoresFrecuencia();
        mostrarBarreras();
        mostrarReductoresConsecuencia();
        
        actualizarContadoresDefensas();
    }
}

void MainWindow::mostrarFrecuencia(CompleteEventDefinition &evento)
{
    switch (evento.frecuenciaCalculada) {
        case 0:
            ui->etFrecuencia->setText("FMB");
            ui->etFrecuencia->setStyleSheet(Styles::WhiteLabel);
            break;
        case 1:
            ui->etFrecuencia->setText("FB");
            ui->etFrecuencia->setStyleSheet(Styles::GreenLabel);
            break;
        case 2:
            ui->etFrecuencia->setText("FM");
            ui->etFrecuencia->setStyleSheet(Styles::YellowLabel);
            break;
        case 3:
            ui->etFrecuencia->setText("FA");
            ui->etFrecuencia->setStyleSheet(Styles::RedLabel);
            break;
    }
    switch (evento.frecuencia) {
        case 0:
            ui->etFrecuenciaBase->setText("FMB");
            ui->etFrecuenciaBase->setStyleSheet(Styles::WhiteLabel);
            break;
        case 1:
            ui->etFrecuenciaBase->setText("FB");
            ui->etFrecuenciaBase->setStyleSheet(Styles::GreenLabel);
            break;
        case 2:
            ui->etFrecuenciaBase->setText("FM");
            ui->etFrecuenciaBase->setStyleSheet(Styles::YellowLabel);
            break;
        case 3:
            ui->etFrecuenciaBase->setText("FA");
            ui->etFrecuenciaBase->setStyleSheet(Styles::RedLabel);
            break;
    }
}

void MainWindow::mostrarConsecuencia(CompleteEventDefinition &evento)
{
    switch (evento.consecuenciaCalculada) {
        case 0:
            ui->etConsecuencia->setText("CB");
            ui->etConsecuencia->setStyleSheet(Styles::WhiteLabel);
            break;
        case 1:
            ui->etConsecuencia->setText("CM");
            ui->etConsecuencia->setStyleSheet(Styles::GreenLabel);
            break;
        case 2:
            ui->etConsecuencia->setText("CA");
            ui->etConsecuencia->setStyleSheet(Styles::YellowLabel);
            break;
        case 3:
            ui->etConsecuencia->setText("CMA");
            ui->etConsecuencia->setStyleSheet(Styles::RedLabel);
            break;
    }
    switch (evento.consecuencia) {
        case 0:
            ui->etConsecuenciaBase->setText("CB");
            ui->etConsecuenciaBase->setStyleSheet(Styles::WhiteLabel);
            break;
        case 1:
            ui->etConsecuenciaBase->setText("CM");
            ui->etConsecuenciaBase->setStyleSheet(Styles::GreenLabel);
            break;
        case 2:
            ui->etConsecuenciaBase->setText("CA");
            ui->etConsecuenciaBase->setStyleSheet(Styles::YellowLabel);
            break;
        case 3:
            ui->etConsecuenciaBase->setText("CMA");
            ui->etConsecuenciaBase->setStyleSheet(Styles::RedLabel);
            break;
    }
}

void MainWindow::mostrarProbabilidad(CompleteEventDefinition &evento)
{
    switch (evento.probabilidadCalculada) {
        case 0:
            ui->etProbabilidad->setText("PMB");
            ui->etProbabilidad->setStyleSheet(Styles::WhiteLabel);
            break;
        case 1:
            ui->etProbabilidad->setText("PB");
            ui->etProbabilidad->setStyleSheet(Styles::GreenLabel);
            break;
        case 2:
            ui->etProbabilidad->setText("PM");
            ui->etProbabilidad->setStyleSheet(Styles::YellowLabel);
            break;
        case 3:
            ui->etProbabilidad->setText("PA");
            ui->etProbabilidad->setStyleSheet(Styles::RedLabel);
            break;
    }
}

void MainWindow::mostrarRiesgo()
{
    actualCompleteEvent->calcularRiesgo(matrizRiesgo);
    actualCompleteEvent->calcularRiesgoMaximo(matrizRiesgo);
    //qDebug() << "Riesgo: " << actualCompleteEvent->riesgo;
    switch (actualCompleteEvent->riesgo) {
        case 0:
            ui->etRiesgo->setText("RB");
            ui->etRiesgo->setStyleSheet(Styles::WhiteLabel);
            ui->etRiesgo->setToolTip("Riesgo Bajo - Nivel de riesgo actual del suceso con las defensas aplicadas");
            break;
        case 1:
            ui->etRiesgo->setText("RM");
            ui->etRiesgo->setStyleSheet(Styles::GreenLabel);
            ui->etRiesgo->setToolTip("Riesgo Medio - Nivel de riesgo actual del suceso con las defensas aplicadas");
            break;
        case 2:
            ui->etRiesgo->setText("RA");
            ui->etRiesgo->setStyleSheet(Styles::YellowLabel);
            ui->etRiesgo->setToolTip("Riesgo Alto - Nivel de riesgo actual del suceso con las defensas aplicadas");
            break;
        case 3:
            ui->etRiesgo->setText("RMA");
            ui->etRiesgo->setStyleSheet(Styles::RedLabel);
            ui->etRiesgo->setToolTip("Riesgo Muy Alto - Nivel de riesgo actual del suceso con las defensas aplicadas");
            break;
    }
    switch (actualCompleteEvent->riesgoMaximo) {
    case 0:
        ui->etRiesgoMaximo->setText("RB");
        ui->etRiesgoMaximo->setStyleSheet(Styles::WhiteLabel);
        ui->etRiesgoMaximo->setToolTip("Riesgo Bajo - Nivel de riesgo máximo que puede alcanzar el suceso si no se aplica ninguna defensa");
        break;
    case 1:
        ui->etRiesgoMaximo->setText("RM");
        ui->etRiesgoMaximo->setStyleSheet(Styles::GreenLabel);
        ui->etRiesgoMaximo->setToolTip("Riesgo Medio - Nivel de riesgo máximo que puede alcanzar el suceso si no se aplica ninguna defensa");
        break;
    case 2:
        ui->etRiesgoMaximo->setText("RA");
        ui->etRiesgoMaximo->setStyleSheet(Styles::YellowLabel);
        ui->etRiesgoMaximo->setToolTip("Riesgo Alto - Nivel de riesgo máximo que puede alcanzar el suceso si no se aplica ninguna defensa");
        break;
    case 3:
        ui->etRiesgoMaximo->setText("RMA");
        ui->etRiesgoMaximo->setStyleSheet(Styles::RedLabel);
        ui->etRiesgoMaximo->setToolTip("Riesgo Muy Alto - Nivel de riesgo máximo que puede alcanzar el suceso si no se aplica ninguna defensa");
        break;
    }
}

void MainWindow::mostrarReductoresFrecuencia()
{
    if (reductoresFrecuencia != nullptr) {
        delete reductoresFrecuencia;
    }

    reductoresFrecuencia = new DefenseListModel;
    QStringList lstLineas;
    QStringList lstSeleccionados;
    for (auto linea : actualCompleteEvent->reductoresFrecuencias) {
        QString hardnessText = MatrizRiesgo::Durezas.at(linea.hardiness);
        QString leftPart = QString("[%1] %2").arg(linea.code, linea.nombre);
        QString rightPart = QString("[%1]").arg(hardnessText);
        // Store both parts separated by space - delegate will handle the display
        QString displayText = leftPart + " " + rightPart;
        lstLineas << displayText;

        if (linea.active) {
            lstSeleccionados << displayText;
        }
    }
    reductoresFrecuencia->setStringList(lstLineas);
    reductoresFrecuencia->setChecked(lstSeleccionados);
    ui->lReductoresFrecuencia->setModel(reductoresFrecuencia);

    // Para responder a cuando un usuario cambia el estado del reductor
    connect(reductoresFrecuencia, &DefenseListModel::dataChanged, [=](const QModelIndex &a){
        auto displayText = a.data(Qt::DisplayRole).toString();
        auto nameOfChanged = extractDefenseNameFromDisplay(displayText);

        for(auto &listaEventos : events){
            for (auto &evento : listaEventos.second) {
                if (evento.id == eventoActualId){
                    for(auto &barrera : evento.reductoresFrecuencias){
                        if (barrera.nombre == nameOfChanged) {
                            barrera.changeState();

                            evento.calcularReductoresFrecuencia();
                            evento.calcularRiesgo(matrizRiesgo);

                            // actualCompleteEvent already points to this event, no need to recreate
                            actualCompleteEvent->calcularReductoresFrecuencia();
                            actualCompleteEvent->calcularRiesgo(matrizRiesgo);

                            mostrarFrecuencia(evento);
                            mostrarRiesgo();
                            
                            actualizarContadoresDefensas();
                            refrescarListaEventos();

                            break;
                        }
                    }
                }
            }
        }
    });
}

void MainWindow::mostrarBarreras()
{
    if (barreras != nullptr) {
        delete barreras;
    }

    barreras = new DefenseListModel;
    QStringList lstLineas;
    QStringList lstSeleccionados;
    for (auto linea : actualCompleteEvent->barreras) {
        QString hardnessText = MatrizRiesgo::Durezas.at(linea.hardiness);
        QString leftPart = QString("[%1] %2").arg(linea.code, linea.nombre);
        QString rightPart = QString("[%1]").arg(hardnessText);
        // Store both parts separated by space - delegate will handle the display
        QString displayText = leftPart + " " + rightPart;
        lstLineas << displayText;

        if (linea.active) {
            lstSeleccionados << displayText;
        }
    }
    barreras->setStringList(lstLineas);
    barreras->setChecked(lstSeleccionados);
    ui->lBarreras->setModel(barreras);

    connect(barreras, &DefenseListModel::dataChanged, [=](const QModelIndex &a){
        auto displayText = a.data(Qt::DisplayRole).toString();
        auto nameOfChanged = extractDefenseNameFromDisplay(displayText);

        for(auto &listaEventos : events){
            for (auto &evento : listaEventos.second) {
                if (evento.id == eventoActualId){
                    for(auto &barrera : evento.barreras){
                        if (barrera.nombre == nameOfChanged) {
                            barrera.changeState();

                            evento.calcularBarreras();
                            evento.calcularRiesgo(matrizRiesgo);

                            // actualCompleteEvent already points to this event, no need to recreate
                            actualCompleteEvent->calcularBarreras();
                            actualCompleteEvent->calcularRiesgo(matrizRiesgo);

                            mostrarProbabilidad(evento);
                            mostrarRiesgo();
                            
                            actualizarContadoresDefensas();
                            refrescarListaEventos();

                            break;
                        }
                    }
                }
            }
        }
    });
}

void MainWindow::mostrarReductoresConsecuencia()
{
    if (reductoresConsecuencias != nullptr) {
        delete reductoresConsecuencias;
    }

    reductoresConsecuencias = new DefenseListModel;
    QStringList lstLineas;
    QStringList lstSeleccionados;
    for (auto linea : actualCompleteEvent->reductoresConsecuencias) {
        QString hardnessText = MatrizRiesgo::Durezas.at(linea.hardiness);
        QString leftPart = QString("[%1] %2").arg(linea.code, linea.nombre);
        QString rightPart = QString("[%1]").arg(hardnessText);
        // Store both parts separated by space - delegate will handle the display
        QString displayText = leftPart + " " + rightPart;
        lstLineas << displayText;

        if (linea.active) {
            lstSeleccionados << displayText;
        }
    }
    reductoresConsecuencias->setStringList(lstLineas);
    reductoresConsecuencias->setChecked(lstSeleccionados);
    ui->lReductoresConsecuencia->setModel(reductoresConsecuencias);

    connect(reductoresConsecuencias, &DefenseListModel::dataChanged, [=](const QModelIndex &a){
        auto displayText = a.data(Qt::DisplayRole).toString();
        auto nameOfChanged = extractDefenseNameFromDisplay(displayText);

        for(auto &listaEventos : events){
            for (auto &evento : listaEventos.second) {
                if (evento.id == eventoActualId){
                    for(auto &barrera : evento.reductoresConsecuencias){
                        if (barrera.nombre == nameOfChanged) {
                            barrera.changeState();

                            evento.calcularReductoresConsecuencia();
                            evento.calcularRiesgo(matrizRiesgo);

                            // actualCompleteEvent already points to this event, no need to recreate
                            actualCompleteEvent->calcularReductoresConsecuencia();
                            actualCompleteEvent->calcularRiesgo(matrizRiesgo);

                            mostrarConsecuencia(evento);
                            mostrarRiesgo();
                            
                            actualizarContadoresDefensas();
                            refrescarListaEventos();

                            break;
                        }
                    }
                }
            }
        }
    });
}

void MainWindow::actualizarContadoresDefensas()
{
    if (actualCompleteEvent != nullptr && actualCompleteEvent->id > 0) {
        // Count active and total defenses for each type
        
        // Reductores de Frecuencia
        int activeFrecuencia = 0;
        int totalFrecuencia = actualCompleteEvent->reductoresFrecuencias.size();
        for (const auto &defensa : actualCompleteEvent->reductoresFrecuencias) {
            if (defensa.active) {
                activeFrecuencia++;
            }
        }
        ui->lblCountReductoresFrecuencia->setText(QString("[%1/%2]").arg(activeFrecuencia).arg(totalFrecuencia));
        
        // Barreras
        int activeBarreras = 0;
        int totalBarreras = actualCompleteEvent->barreras.size();
        for (const auto &defensa : actualCompleteEvent->barreras) {
            if (defensa.active) {
                activeBarreras++;
            }
        }
        ui->lblCountBarreras->setText(QString("[%1/%2]").arg(activeBarreras).arg(totalBarreras));
        
        // Reductores de Consecuencia
        int activeConsecuencia = 0;
        int totalConsecuencia = actualCompleteEvent->reductoresConsecuencias.size();
        for (const auto &defensa : actualCompleteEvent->reductoresConsecuencias) {
            if (defensa.active) {
                activeConsecuencia++;
            }
        }
        ui->lblCountReductoresConsecuencia->setText(QString("[%1/%2]").arg(activeConsecuencia).arg(totalConsecuencia));
    } else {
        // Reset counters when no event is selected
        ui->lblCountReductoresFrecuencia->setText("[0/0]");
        ui->lblCountBarreras->setText("[0/0]");
        ui->lblCountReductoresConsecuencia->setText("[0/0]");
    }
}

void MainWindow::actualizarContadorEventos()
{
    // Count total and applicable events in the current event list
    int totalEventos = eventosModel ? eventosModel->rowCount() : 0;
    int eventosAplicables = 0;
    
    if (eventosModel) {
        for (int i = 0; i < eventosModel->rowCount(); i++) {
            QModelIndex index = eventosModel->index(i, 0);
            QString etapaSubetapa = index.data(RoleStage).toString() + "-" + index.data(RoleSubstage).toString();
            QString nombre = index.data(RoleName).toString();
            
            // Find the event and check if it's completado
            auto it = events.find(etapaSubetapa.toStdString());
            if (it != events.end()) {
                for (const auto &evento : it->second) {
                    if (evento.nombre == nombre && evento.completado) {
                        eventosAplicables++;
                        break;
                    }
                }
            }
        }
    }
    
    QString texto;
    if (ui->btTodasEtapas->isChecked()) {
        // Show counter even in "All Stages" mode
        texto = QString(tr("**** Todas las etapas ****") + "   [%1/%2]").arg(eventosAplicables).arg(totalEventos);
    } else if (actualItem != nullptr) {
        QString ruta;
        auto parent = actualItem->parent();
        if (parent != nullptr) {
            ruta = parent->data(Qt::DisplayRole).toString() + " / ";
        }
        ruta += actualItem->data(Qt::DisplayRole).toString();
        
        // Append the counter after the stage name
        texto = ruta + QString("   [%1/%2]").arg(eventosAplicables).arg(totalEventos);
    }
    
    if (!texto.isEmpty()) {
        ui->etEtapaActual->setText(texto);
    }
}

void MainWindow::refrescarListaEventos()
{
    // Save current selection
    const QModelIndex currentIndex = ui->eventos->currentIndex();
    QVariant m_previouslySelected = (currentIndex.isValid())? currentIndex.data(Qt::DisplayRole): QVariant();
    
    // Refresh event list to update risk colors
    mostrarEventosSeleccion();
    
    // Update event counter
    actualizarContadorEventos();
    
    // Restore selection
    if (!m_previouslySelected.isNull()) {
        const QAbstractItemModel *model = ui->eventos->model();
        const QModelIndexList indexes = model->match(
              model->index(0,0),
              Qt::DisplayRole,
              m_previouslySelected,
              1, // first hit only
              Qt::MatchExactly
            );
        if (indexes.size() > 0) {
              ui->eventos->setCurrentIndex(indexes.at(0));
        }
    }
}

void MainWindow::mostrarInfoBarrera(const std::vector<DefenseDefinition> &defensas, const QString &nombre)
{
    for (auto linea : defensas) {
        if (linea.nombre == nombre) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(linea.code);
            msgBox.setIcon(QMessageBox::Icon::Information);
            QString tipo;
            switch (linea.type) {
                case DefenseType::FrecuencyReducer:
                    tipo = "Reductor de frecuencia";
                    break;
                case DefenseType::Barrier:
                    tipo = "Barrera";
                    break;
                case DefenseType::ConsequenceReducer:
                    tipo = "Reductor de consecuencia";
                    break;
            }

            msgBox.setText("<p>" + linea.code + "  -  " + tipo +  "</p><p>" + linea.nombre + "</p><p>Descripción: " + linea.descripcion + "</p><p><strong>Robustez</strong> -> " + MatrizRiesgo::Durezas.at(linea.hardiness) + " (" + QString::number(linea.hardiness) + ")</p>");
            msgBox.exec();
            return;
        }
    }
}

QString MainWindow::extractDefenseNameFromDisplay(const QString &displayText) const
{
    // Extract the name from "[code] Name [Hardness]" format
    QString temp = displayText.section("] ", 1); // Remove "[code] " part
    // Remove the trailing "[Hardness]" part by finding the last '['
    int lastBracket = temp.lastIndexOf('[');
    if (lastBracket > 0) {
        temp = temp.left(lastBracket).trimmed(); // Remove "[Hardness]" and trim spaces
    }
    return temp;
}

void MainWindow::on_mnuClose_triggered()
{
    if (settings.debugMode) {
        qDebug() << "Cerrando modelo ";
    }

    praxisDb.close();
    eventosModel->clear();
    model->clear();


    actualStageIndex = "";
    reductoresFrecuencia = nullptr;
    barreras = nullptr;
    reductoresConsecuencias = nullptr;

    setWindowTitle("Análisis de matrices de riesgo");
    settings.lastFile = "";
    findChild<QAction *>("mnuClose")->setDisabled(true);
}

void MainWindow::on_actionEtapas_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    DlgGestionEtapas *dlgEtapas = new DlgGestionEtapas(this);
    QObject::connect(dlgEtapas, &QDialog::finished, [=](){
        this->loadModel();
    });
    dlgEtapas->exec();
    delete dlgEtapas;
}


void MainWindow::on_mnuSucesosIniciadores_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    sucesos *wndSucesos = new sucesos(this);
    QObject::connect(wndSucesos, &QDialog::finished, [=](){
        events.clear();
        loadModel();
    });
    wndSucesos->exec();
}

void MainWindow::on_mnuAcercaDe_triggered()
{
    About acercade(this, APP_VERSION);
    acercade.exec();
}

void MainWindow::on_actionSalir_triggered()
{
	// apm comprueba si hay cambios en el modelo 
	// 			para guardarlo antes de cerrarlo
	//			extraído en parte de "dbEvent.checkAndUpdate(events)"
    bool modelchanges = false;

    for(auto stage : events) {
        auto eventos = stage.second;
        for (auto event : eventos){
            for (auto defensa : event.reductoresFrecuencias) {
                if (defensa.changed) {
                modelchanges=true;
                }
            }
            for (auto defensa : event.barreras) {
                if (defensa.changed) {
                modelchanges=true;
                }
            }
            for (auto defensa : event.reductoresConsecuencias) {
                if (defensa.changed) {
                modelchanges=true;
                }
            }
         }
     }

    if (modelchanges == true) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmación", "Has modificado los datos del modelo. ¿Quieres guardar los cambios antes de cerrar?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            on_actionGuardarModelo_triggered();
        }
    }
    //apm
    
    this->close();
}

void MainWindow::on_actionEmpty_triggered()
{
    // Check if there's a database open
    if (praxisDb.isOpen() && !settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Base de datos abierta"));
        msgBox.setText(tr("Hay una base de datos abierta actualmente."));
        msgBox.setInformativeText(tr("¿Desea guardar los cambios antes de crear un nuevo modelo?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        
        int ret = msgBox.exec();
        if (ret == QMessageBox::Save) {
            saveModel();
        } else if (ret == QMessageBox::Cancel) {
            return;
        }
        // If Discard, continue without saving
    }
    
    NewModelWizard wizard(this);
    if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
        // Load the newly created model
        openPraxisFile(wizard.getPdbFile());
    }
}

void MainWindow::on_actionMARR_triggered()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("MARRTA (*.pdb)");
    if (dialog.exec()) {
        QStringList filenames = dialog.selectedFiles();
        if (filenames.count() > 0) { // Siempre debe ser cierto
            QString filename = filenames.first();

            if (!filename.endsWith(".pdb")) {
                filename += ".pdb";
            }

            if (QFile::exists(filename)) {
                QFile *exists = new QFile(filename);
                if (!exists->remove()) {
                    QMessageBox msgBox;
                    msgBox.setText("No se ha podido borrar el archivo elegido");
                    msgBox.exec();

                    return;
                }
                delete exists;
            }

            try {
                Marr marrModel;
                if (marrModel.createTables(filename)) {
                    QMessageBox msgBox;
                    msgBox.setText("El Modelo basado en MARR ha sido creado correctamente");
                    msgBox.exec();
                }
            } catch (QString msg) {
                QMessageBox msgBox;
                msgBox.setText(msg);
                msgBox.exec();
            }
        }

    }
}

void MainWindow::on_actionResumen_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    Resumen resumen(&events);
    resumen.exec();
}

void MainWindow::on_actionInforme_triggered()
{
    // Redirect to generate HTML report
    on_actionGenerarInformeHTML_triggered();
}

/**
 * @brief MainWindow::on_actionGenerarInformeHTML_triggered
 * Opens the HTML report generator dialog to create PDF reports from event data
 */
void MainWindow::on_actionGenerarInformeHTML_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    // Prepare list of all events
    QList<CompleteEventDefinition> allEvents;
    
    for (const auto& pair : events) {
        for (const auto& event : pair.second) {
            allEvents.append(event);
        }
    }
    
    if (allEvents.isEmpty()) {
        QMessageBox::information(this, tr("Sin eventos"), 
            tr("No hay eventos disponibles para generar informes."));
        return;
    }
    
    // Show configuration dialog which now handles everything
    DlgConfigInforme configDialog(allEvents, this);
    configDialog.exec();
}

/**
 * @brief MainWindow::on_actionGenerarFlujograma_triggered
 * Generates a vertical flowchart HTML file of the risk model
 */
void MainWindow::on_actionGenerarFlujograma_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    // Get model configuration to use the model name
    Database::ConfiguracionModelo config = Database::DbManager::checkDatabase(false);
    
    // Generate default filename: "flujograma-" + model name + date (YY-MM-DD)
    QString currentDate = QDateTime::currentDateTime().toString("yy-MM-dd");
    // Use the actual model name from the database configuration table
    QString modelName = config.nombre;
    // Sanitize model name for filename (remove invalid characters)
    QString sanitizedModelName = modelName;
    sanitizedModelName.replace(QRegularExpression("[^a-zA-Z0-9_\\-]"), "_");
    QString defaultFileName = QString("flujograma-%1-%2.html")
        .arg(sanitizedModelName)
        .arg(currentDate);
    
    // Ask user for file location
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Guardar flujograma"),
        QDir::homePath() + "/" + defaultFileName,
        tr("Archivos HTML (*.html *.htm)")
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Generate the flowchart
    if (FlujogramaGenerator::generarHTML(fileName)) {
        QMessageBox::information(
            this,
            tr("Flujograma generado"),
            tr("El flujograma se ha generado correctamente en:\n%1").arg(fileName)
        );
        
        // Ask if user wants to open it with translatable buttons
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Abrir flujograma"));
        msgBox.setText(tr("¿Desea abrir el flujograma generado en el navegador?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.button(QMessageBox::Yes)->setText(tr("Sí"));
        msgBox.button(QMessageBox::No)->setText(tr("No"));
        
        if (msgBox.exec() == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
        }
    } else {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("No se pudo generar el flujograma. Verifique que hay datos en el modelo.")
        );
    }
}

/**
 * @brief MainWindow::on_actionGuardarModelo_triggered
 * Recorre el estado actual del modelo buscando defensas que se han modificado
 * y guarda esa información en la BD.
 * También guarda el riesgo asociado de todos los eventos de nuevo.
 */
/**
 * @brief MainWindow::showNoModelLoadedWarning
 * Shows a warning message when user tries to access a function that requires a loaded model
 */
void MainWindow::showNoModelLoadedWarning()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("No hay modelo cargado"));
    msgBox.setText(tr("No hay ningún modelo de riesgo cargado.\nCargue un modelo para poder editarlo."));
    msgBox.exec();
}

/**
 * @brief MainWindow::on_actionGuardarModelo_triggered
 * Recorre el estado actual del modelo buscando defensas que se han modificado
 * y guarda esa información en la BD.
 * También guarda el riesgo asociado de todos los eventos de nuevo.
 */
void MainWindow::saveModel()
{
    QMessageBox msgBox;
    msgBox.setText(tr("¿Quieres guardar los cambios?"));
    msgBox.setInformativeText(tr("Si no los guardas puedes perder los cambios que has realizado"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        WaitingSpinnerWidget* spinner = new WaitingSpinnerWidget(this, true, true);
        spinner->start();
        QCoreApplication::processEvents();

        Database::DbEvent dbEvent;
        dbEvent.checkAndUpdate(events);

        spinner->stop();
        delete spinner;

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Los cambios del modelo se han guardado."));
        msgBox.exec();

        // apm - refrescamos la pantalla - referencia: https://www.qtcentre.org/threads/63990-Select-specific-item-in-QListView
        //
        //      primero guardamos el ínidce de listView eventos, luego mostramos todos los eventos, y luego restauramos el indice y mostramos el evento seleccionado

        const QModelIndex currentIndex = ui->eventos->currentIndex();  // may be an invalid index
        QVariant m_previouslySelected = (currentIndex.isValid())? currentIndex.data(Qt::DisplayRole): QVariant();

        mostrarEventosSeleccion();

        if (!m_previouslySelected.isNull()) {
            const QAbstractItemModel *model = ui->eventos->model();
            const QModelIndexList indexes = model->match(
                  model->index(0,0),
                  Qt::DisplayRole,
                  m_previouslySelected,
                  1, // first hit only
                  Qt::MatchExactly
                );
            if (indexes.size() > 0) {  // found a match
                  ui->eventos->setCurrentIndex(indexes.at(0));
                  // Trigger the proper event selection logic to update actualCompleteEvent
                  on_eventos_clicked(indexes.at(0));
              }
        }
        // apm
    }
}

void MainWindow::on_actionGuardar_triggered()
{
    saveModel();
}

void MainWindow::on_actionGuardar_como_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }

    // First, save any pending changes to the current file
    Database::DbEvent dbEvent;
    dbEvent.checkAndUpdate(events);

    // Prompt user for new file location
    QString newFileName = QFileDialog::getSaveFileName(
        this,
        tr("Guardar modelo como"),
        settings.databaseFolder,
        tr("Modelo MARRTA (*.pdb)")
    );

    if (newFileName.isEmpty()) {
        return; // User cancelled
    }

    // Ensure .pdb extension
    if (!newFileName.endsWith(".pdb")) {
        newFileName += ".pdb";
    }

    // If file exists, it needs to be removed before copying
    // (native dialog already handled overwrite confirmation)
    if (QFile::exists(newFileName)) {
        if (!QFile::remove(newFileName)) {
            QMessageBox::critical(
                this,
                tr("Error"),
                tr("No se pudo eliminar el archivo existente")
            );
            return;
        }
    }

    // Copy current database to new location
    if (QFile::copy(settings.lastFile, newFileName)) {
        QMessageBox::information(
            this,
            tr("Guardado completado"),
            tr("El modelo se ha guardado correctamente en:\n%1").arg(newFileName)
        );
    } else {
        QMessageBox::critical(
            this,
            tr("Error al guardar"),
            tr("No se pudo guardar el archivo en la ubicación especificada")
        );
    }
}

void MainWindow::on_actionGuardarModelo_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    saveModel();
}

/**
 * @brief getVisibleEventIds
 * Helper method to get set of event IDs currently visible in event list
 * @return std::set<int> Set of visible event IDs
 */
std::set<int> MainWindow::getVisibleEventIds() const
{
    std::set<int> visibleEventIds;
    if (eventosModel) {
        for (int i = 0; i < eventosModel->rowCount(); i++) {
            QModelIndex index = eventosModel->index(i, 0);
            QString etapaSubetapa = index.data(RoleStage).toString() + "-" + index.data(RoleSubstage).toString();
            QString nombre = index.data(RoleName).toString();
            
            // Find the event ID
            auto it = events.find(etapaSubetapa.toStdString());
            if (it != events.end()) {
                for (const auto &evento : it->second) {
                    if (evento.nombre == nombre) {
                        visibleEventIds.insert(evento.id);
                        break;
                    }
                }
            }
        }
    }
    return visibleEventIds;
}

/**
 * @brief markAllDefenses
 * Helper method to mark or unmark all defenses in visible (filtered) events
 * @param marcar true to mark (activate), false to unmark (deactivate)
 */
void MainWindow::markAllDefenses(bool marcar)
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    // Build a set of visible event IDs from the current event model
    std::set<int> visibleEventIds = getVisibleEventIds();
    
    // Begin transaction for batch update
    QSqlDatabase::database().transaction();
    
    try {
        // Update database for visible defenses that need changing
        for(auto &eventsInStages : events) {
            auto &eventsInStage = eventsInStages.second;

            for (auto &event : eventsInStage) {
                // Skip events that are not visible (filtered out)
                if (visibleEventIds.find(event.id) == visibleEventIds.end()) {
                    continue;
                }
                
                // Process all defense types
                for (auto &defensa : event.reductoresFrecuencias) {
                    if (defensa.active != marcar) {
                        // Update database within transaction
                        Database::DbDefense::setActive(defensa.id, marcar);
                    }
                }
                
                for (auto &defensa : event.barreras) {
                    if (defensa.active != marcar) {
                        // Update database within transaction
                        Database::DbDefense::setActive(defensa.id, marcar);
                    }
                }
                
                for (auto &defensa : event.reductoresConsecuencias) {
                    if (defensa.active != marcar) {
                        // Update database within transaction
                        Database::DbDefense::setActive(defensa.id, marcar);
                    }
                }
            }
        }
        
        // Commit transaction if all updates succeeded
        QSqlDatabase::database().commit();
        
        // Only update in-memory state after successful commit
        for(auto &eventsInStages : events) {
            auto &eventsInStage = eventsInStages.second;

            for (auto &event : eventsInStage) {
                // Skip events that are not visible (filtered out)
                if (visibleEventIds.find(event.id) == visibleEventIds.end()) {
                    continue;
                }
                
                bool changed = false;
                
                // Update in-memory state for all defense types
                for (auto &defensa : event.reductoresFrecuencias) {
                    if (defensa.active != marcar) {
                        defensa.active = marcar;
                        changed = true;
                    }
                }
                
                for (auto &defensa : event.barreras) {
                    if (defensa.active != marcar) {
                        defensa.active = marcar;
                        changed = true;
                    }
                }
                
                for (auto &defensa : event.reductoresConsecuencias) {
                    if (defensa.active != marcar) {
                        defensa.active = marcar;
                        changed = true;
                    }
                }
                
                // Recalculate risk if any defense changed
                if (changed) {
                    event.calcularReductores();
                    event.calcularRiesgo(matrizRiesgo);
                    // Update risk in database
                    Database::DbEvent::updateRisk(event.id, event.riesgo);
                }
            }
        }
        
    } catch (const QString& msg) {
        // Rollback transaction on error
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al actualizar las defensas: %1").arg(msg));
        throw; // Re-throw to signal error to caller
    }
}

/**
 * @brief markAllDefenses
 * Helper method to mark or unmark all defenses of a specific type in visible (filtered) events
 * @param tipo The type of defense to mark/unmark
 * @param marcar true to mark (activate), false to unmark (deactivate)
 */
void MainWindow::markAllDefenses(TipoDefensa tipo, bool marcar)
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    // Build a set of visible event IDs from the current event model
    std::set<int> visibleEventIds = getVisibleEventIds();
    
    // Begin transaction for batch update
    QSqlDatabase::database().transaction();
    
    try {
        // Update database for visible defenses of the specific type that need changing
        for(auto &eventsInStages : events) {
            auto &eventsInStage = eventsInStages.second;

            for (auto &event : eventsInStage) {
                // Skip events that are not visible (filtered out)
                if (visibleEventIds.find(event.id) == visibleEventIds.end()) {
                    continue;
                }
                
                // Process specific defense type
                if (tipo == ReductoresFrecuencia) {
                    for (auto &defensa : event.reductoresFrecuencias) {
                        if (defensa.active != marcar) {
                            // Update database within transaction
                            Database::DbDefense::setActive(defensa.id, marcar);
                        }
                    }
                } else if (tipo == Barreras) {
                    for (auto &defensa : event.barreras) {
                        if (defensa.active != marcar) {
                            // Update database within transaction
                            Database::DbDefense::setActive(defensa.id, marcar);
                        }
                    }
                } else if (tipo == ReductoresConsecuencia) {
                    for (auto &defensa : event.reductoresConsecuencias) {
                        if (defensa.active != marcar) {
                            // Update database within transaction
                            Database::DbDefense::setActive(defensa.id, marcar);
                        }
                    }
                }
            }
        }
        
        // Commit transaction if all updates succeeded
        QSqlDatabase::database().commit();
        
        // Only update in-memory state after successful commit
        for(auto &eventsInStages : events) {
            auto &eventsInStage = eventsInStages.second;

            for (auto &event : eventsInStage) {
                // Skip events that are not visible (filtered out)
                if (visibleEventIds.find(event.id) == visibleEventIds.end()) {
                    continue;
                }
                
                bool changed = false;
                
                // Update in-memory state for specific defense type
                if (tipo == ReductoresFrecuencia) {
                    for (auto &defensa : event.reductoresFrecuencias) {
                        if (defensa.active != marcar) {
                            defensa.active = marcar;
                            changed = true;
                        }
                    }
                } else if (tipo == Barreras) {
                    for (auto &defensa : event.barreras) {
                        if (defensa.active != marcar) {
                            defensa.active = marcar;
                            changed = true;
                        }
                    }
                } else if (tipo == ReductoresConsecuencia) {
                    for (auto &defensa : event.reductoresConsecuencias) {
                        if (defensa.active != marcar) {
                            defensa.active = marcar;
                            changed = true;
                        }
                    }
                }
                
                // Recalculate risk if any defense changed
                if (changed) {
                    event.calcularReductores();
                    event.calcularRiesgo(matrizRiesgo);
                    // Update risk in database
                    Database::DbEvent::updateRisk(event.id, event.riesgo);
                }
            }
        }
        
    } catch (const QString& msg) {
        // Rollback transaction on error
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al actualizar las defensas: %1").arg(msg));
        throw; // Re-throw to signal error to caller
    }
}

/**
 * Marca todas las defensas del modelo actual
 */
void MainWindow::on_actionMarcarTodasDefensas_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(true);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Marca todos los reductores de frecuencia del modelo actual
 */
void MainWindow::on_actionMarcarReductoresFrecuencia_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(ReductoresFrecuencia, true);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Marca todas las barreras del modelo actual
 */
void MainWindow::on_actionMarcarBarreras_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(Barreras, true);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Marca todos los reductores de consecuencia del modelo actual
 */
void MainWindow::on_actionMarcarReductoresConsecuencia_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(ReductoresConsecuencia, true);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Desmarca todas las defensas del modelo actual
 */
void MainWindow::on_actionDesmarcarTodasDefensas_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(false);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Desmarca todos los reductores de frecuencia del modelo actual
 */
void MainWindow::on_actionDesmarcarReductoresFrecuencia_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(ReductoresFrecuencia, false);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Desmarca todas las barreras del modelo actual
 */
void MainWindow::on_actionDesmarcarBarreras_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(Barreras, false);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Desmarca todos los reductores de consecuencia del modelo actual
 */
void MainWindow::on_actionDesmarcarReductoresConsecuencia_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllDefenses(ReductoresConsecuencia, false);
    } catch (const QString&) {
        // Error already handled in markAllDefenses
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * @brief markAllInitiators
 * Helper method to mark visible (filtered) initiators with the specified completado state
 * @param completado true to mark as applicable, false to mark as not applicable
 */
void MainWindow::markAllInitiators(bool completado)
{
    // Build a set of visible event IDs from the current event model
    std::set<int> visibleEventIds = getVisibleEventIds();
    
    // Begin transaction for batch update
    QSqlDatabase::database().transaction();
    
    try {
        // Update database for visible initiators that need changing
        for(auto &eventsInStages : events) {
            auto &eventsInStage = eventsInStages.second;

            for (auto &event : eventsInStage) {
                // Skip events that are not visible (filtered out)
                if (visibleEventIds.find(event.id) == visibleEventIds.end()) {
                    continue;
                }
                
                if (event.completado != completado) {
                    // Update database within transaction
                    Database::DbEvent::setCompleted(event.id, completado, event.riesgo);
                }
            }
        }
        
        // Commit transaction if all updates succeeded
        QSqlDatabase::database().commit();
        
        // Only update in-memory state after successful commit
        for(auto &eventsInStages : events) {
            auto &eventsInStage = eventsInStages.second;

            for (auto &event : eventsInStage) {
                // Skip events that are not visible (filtered out)
                if (visibleEventIds.find(event.id) == visibleEventIds.end()) {
                    continue;
                }
                
                if (event.completado != completado) {
                    event.completado = completado;
                }
            }
        }
        
    } catch (const QString& msg) {
        // Rollback transaction on error
        QSqlDatabase::database().rollback();
        QMessageBox::critical(this, tr("Error"), 
            tr("Error al actualizar los iniciadores: %1").arg(msg));
        throw; // Re-throw to signal error to caller
    }
}

/**
 * Marca todos los iniciadores del modelo actual como aplicables
 *
 * @brief MainWindow::on_actionMarcarIniciadoresAplicables_triggered
 */
void MainWindow::on_actionMarcarIniciadoresAplicables_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllInitiators(true);
    } catch (const QString&) {
        // Error already handled in markAllInitiators
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

/**
 * Marca todos los iniciadores del modelo actual como NO aplicables
 *
 * @brief MainWindow::on_actionMarcarIniciadoresNoAplicables_triggered
 */
void MainWindow::on_actionMarcarIniciadoresNoAplicables_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    try {
        markAllInitiators(false);
    } catch (const QString&) {
        // Error already handled in markAllInitiators
        return;
    }

    // Refresh the main window without changing focus
    refreshMainScreen();
}

void MainWindow::toggleActualEvent()
{
    actualCompleteEvent->toggleActive();
    mostrarEvento();
    
    // Update event counter when toggling event status
    actualizarContadorEventos();
}

void MainWindow::refreshMainScreen()
{
    // Save the current event ID before refreshing
    int savedEventoId = eventoActualId;
    QString savedNombre;
    if (actualCompleteEvent != nullptr) {
        savedNombre = actualCompleteEvent->nombre;
    }
    
    // Refresh the event list
    mostrarEventosSeleccion();
    
    // Update event counter after refreshing
    actualizarContadorEventos();
    
    // Restore the selection if an event was selected
    if (savedEventoId > 0 && !savedNombre.isEmpty()) {
        // Find the event in the model and select it
        for (int i = 0; i < eventosModel->rowCount(); ++i) {
            QModelIndex idx = eventosModel->index(i, 0);
            QString nombre = idx.data(RoleName).toString();
            if (nombre == savedNombre) {
                ui->eventos->setCurrentIndex(idx);
                // Refresh the event details
                if (actualCompleteEvent != nullptr) {
                    delete actualCompleteEvent;
                }
                etapaSubetapaSeleccionada = idx.data(RoleStage).toString() + "-" + idx.data(RoleSubstage).toString();
                auto listaEventos = events.find(etapaSubetapaSeleccionada.toStdString())->second;
                for (auto evento : listaEventos) {
                    if (evento.nombre == nombre) {
                        eventoActualId = evento.id;
                        actualCompleteEvent = new CompleteEventDefinition(&evento);
                        mostrarEvento();
                        break;
                    }
                }
                break;
            }
        }
    } else if (actualCompleteEvent != nullptr) {
        // If no saved event, just refresh the current one
        mostrarEvento();
    }
}

void MainWindow::saveModelChanges()
{
    Database::DbEvent dbEvent;
    dbEvent.checkAndUpdate(events);
}

void MainWindow::on_actionCSV_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Archivo donde exportar el modelo actual"),
                QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).last(),
                tr("Archivos Excel (*.xlsx)")
    );
    if (filename != "") {
        if (!filename.endsWith(".xlsx")) {
            filename += ".xlsx";
        }

        if (QFile::exists(filename)) {
            QFile *exists = new QFile(filename);
            exists->remove();
            delete exists;
        }

        xlsx xlsx;
        xlsx.createTables(filename, configuracionModelo.nombre, events);

        QMessageBox msgBox;
        msgBox.setText("Exportación finalizada correctamente");
        msgBox.exec();
    }
}

void MainWindow::on_actionImportarExcel_triggered()
{
    // Check if there's a database open
    if (praxisDb.isOpen() && !settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Base de datos abierta"));
        msgBox.setText(tr("Hay una base de datos abierta actualmente."));
        msgBox.setInformativeText(tr("¿Desea guardar los cambios antes de importar desde Excel?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        
        int ret = msgBox.exec();
        if (ret == QMessageBox::Save) {
            saveModel();
        } else if (ret == QMessageBox::Cancel) {
            return;
        }
        // If Discard, continue without saving
    }
    
    ImportWizard wizard(this);
    if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
        // Load the newly imported model
        openPraxisFile(wizard.getPdbFile());
    }
}

void MainWindow::on_actionInformacion_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    InformacionModelo informacionModelo(configuracionModelo);
    informacionModelo.exec();

    if (informacionModelo.modelChanged()) {
        configuracionModelo = informacionModelo.configuracion();

        Database::DbManager::saveConfiguration(configuracionModelo);
    }
}

/**
 * EVENTOS AL HACER DOBLE CLIC EN DEFENSAS. MUESTRA LA INFORMACIÓN DE LA MISMA
 */

void MainWindow::on_lReductoresFrecuencia_doubleClicked(const QModelIndex &index)
{
    QString displayText = index.data().toString();
    QString nombre = extractDefenseNameFromDisplay(displayText);
    mostrarInfoBarrera (actualCompleteEvent->reductoresFrecuencias, nombre);
}

void MainWindow::on_lBarreras_doubleClicked(const QModelIndex &index)
{
    QString displayText = index.data().toString();
    QString nombre = extractDefenseNameFromDisplay(displayText);
    mostrarInfoBarrera (actualCompleteEvent->barreras, nombre);
}

void MainWindow::on_lReductoresConsecuencia_doubleClicked(const QModelIndex &index)
{
    QString displayText = index.data().toString();
    QString nombre = extractDefenseNameFromDisplay(displayText);
    mostrarInfoBarrera (actualCompleteEvent->reductoresConsecuencias, nombre);
}

/** EVENTOS LANZADOS POR LOS FILTROS LATERALES */
void MainWindow::on_btTodasEtapas_clicked()
{
    if (ui->btTodasEtapas->isChecked()) {
        ui->stages->setEnabled(false);
    } else {
        ui->stages->setEnabled(true);
    }

    mostrarEventosSeleccion();
    
    // Update event counter (works for both all stages and individual stage)
    actualizarContadorEventos();
}

void MainWindow::on_edFiltroNombre_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mostrarEventosSeleccion();
    actualizarContadorEventos();
}

void MainWindow::on_edFiltroDescripcion_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    mostrarEventosSeleccion();
    actualizarContadorEventos();
}

void MainWindow::on_cbFiltroFrecuencia_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mostrarEventosSeleccion();
    actualizarContadorEventos();
}

void MainWindow::on_cbFiltroConsecuencia_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mostrarEventosSeleccion();
    actualizarContadorEventos();
}

void MainWindow::on_cbFiltroRiesgo_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    mostrarEventosSeleccion();
    actualizarContadorEventos();
}
void MainWindow::on_cbAplicables_currentIndexChanged(int )
{
    mostrarEventosSeleccion();
    actualizarContadorEventos();
}
void MainWindow::on_cbErroresHumanos_currentIndexChanged(int )
{
    mostrarEventosSeleccion();
    actualizarContadorEventos();
}
/** FIN EVENTOS LANZADOS POR LOS FILTROS LATERALES */

///////////////////////////////////////////////////
/// Menus contextuales


void MainWindow::on_lReductoresFrecuencia_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->lReductoresFrecuencia->indexAt(pos);
    if (item.isValid()) {
        QString displayText = item.data().toString();
        QString seleccionado = extractDefenseNameFromDisplay(displayText);
        QMenu menu(this);
        menu.addAction(
            QIcon(":/icons/resources/icons/checked.svg"),
            "Marcar todos en este iniciador",
            [=]{
                alternarDefensasEventoActual(ReductoresFrecuencia, true);
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/stop.svg"),
            "Desmarcar todos en este iniciador",
            [=]{
                alternarDefensasEventoActual(ReductoresFrecuencia, false);
            }
        );
        menu.addSeparator();
        menu.addAction(
            QIcon(":/icons/resources/icons/checked.svg"),
            "Marcar esta defensa en todos los iniciadores",
            [=]{
                alternarDefensaModelo(ReductoresFrecuencia, true, pos);
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/stop.svg"),
            "Desmarcar esta defensa en todos los iniciadores",
            [=]{
                alternarDefensaModelo(ReductoresFrecuencia, false, pos);
            }
        );
        menu.addSeparator();
        menu.addAction(
            QIcon(":/icons/resources/icons/no-entry.svg"),
            "Analizar fortaleza",
            [=]{
                CalculoDefensa calculoDefensa;
                calculoDefensa.comprobarCalculo(events, matrizRiesgo, seleccionado, 0);
                calculoDefensa.exec();
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/trending-down.svg"),
            "Mostrar descripción",
            [=]{
                mostrarInfoBarrera (actualCompleteEvent->reductoresFrecuencias, seleccionado);
            }
        );

        menu.exec(ui->lReductoresFrecuencia->mapToGlobal(pos));
    }
}

void MainWindow::on_lBarreras_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->lBarreras->indexAt(pos);
    if (item.isValid()) {
        QString displayText = item.data().toString();
        QString seleccionado = extractDefenseNameFromDisplay(displayText);
        QMenu menu(this);

        menu.addAction(
            QIcon(":/icons/resources/icons/checked.svg"),
            "Marcar todos en este iniciador",
            [=]{
                alternarDefensasEventoActual(Barreras, true);
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/stop.svg"),
            "Desmarcar todos en este iniciador",
            [=]{
                alternarDefensasEventoActual(Barreras, false);
            }
        );
        menu.addSeparator();
        menu.addAction(
            QIcon(":/icons/resources/icons/checked.svg"),
            "Marcar esta defensa en todos los iniciadores",
            [=]{
                alternarDefensaModelo(Barreras, true, pos);
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/stop.svg"),
            "Desmarcar esta defensa en todos los iniciadores",
            [=]{
                alternarDefensaModelo(Barreras, false, pos);
            }
        );
        menu.addSeparator();
        menu.addAction(
            QIcon(":/icons/resources/icons/no-entry.svg"),
            "Analizar fortaleza",
            [=]{
                CalculoDefensa calculoDefensa;
                calculoDefensa.comprobarCalculo(events, matrizRiesgo, seleccionado, 1);
                calculoDefensa.exec();
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/box.svg"),
            "Mostrar descripción",
            [=]{
                mostrarInfoBarrera (actualCompleteEvent->barreras, seleccionado);
            }
        );

        menu.exec(ui->lBarreras->mapToGlobal(pos));
    }
}

void MainWindow::on_lReductoresConsecuencia_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->lReductoresConsecuencia->indexAt(pos);
    if (item.isValid()) {
        QString displayText = item.data().toString();
        QString seleccionado = extractDefenseNameFromDisplay(displayText);
        QMenu menu(this);

        menu.addAction(
            QIcon(":/icons/resources/icons/checked.svg"),
            "Marcar todos en este iniciador",
            [=]{
                alternarDefensasEventoActual(ReductoresConsecuencia, true);
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/stop.svg"),
            "Desmarcar todos en este iniciador",
            [=]{
                alternarDefensasEventoActual(ReductoresConsecuencia, false);
            }
        );
        menu.addSeparator();
        menu.addAction(
            QIcon(":/icons/resources/icons/checked.svg"),
            "Marcar esta defensa en todos los iniciadores",
            [=]{
                alternarDefensaModelo(ReductoresConsecuencia, true, pos);
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/stop.svg"),
            "Desmarcar esta defensa en todos los iniciadores",
            [=]{
                alternarDefensaModelo(ReductoresConsecuencia, false, pos);
            }
        );
        menu.addSeparator();
        menu.addAction(
            QIcon(":/icons/resources/icons/no-entry.svg"),
            "Analizar fortaleza",
            [=]{
                CalculoDefensa calculoDefensa;
                calculoDefensa.comprobarCalculo(events, matrizRiesgo, seleccionado, 2);
                calculoDefensa.exec();
            }
        );
        menu.addAction(
            QIcon(":/icons/resources/icons/activity.svg"),
            "Mostrar descripción",
            [=]{
                mostrarInfoBarrera (actualCompleteEvent->reductoresConsecuencias, seleccionado);
            }
        );

        menu.exec(ui->lReductoresConsecuencia->mapToGlobal(pos));
    }
}

void MainWindow::on_eventos_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->eventos->indexAt(pos);
    if (item.isValid()) {
        QMenu menu(this);
        if (actualCompleteEvent) {
            if (actualCompleteEvent->completado) {
                menu.addAction("Desmarcar como APLICABLE a mi servicio",
                   [=]{
                       actualCompleteEvent->setCompletado(false);
                       eventosModel->setData(item, QVariant::fromValue(-1), RoleRisk);
                       ui->eventos->repaint();

                       for(auto &listaEventos : events){
                           for (auto &evento : listaEventos.second) {
                               if (evento.id == eventoActualId){
                                   evento.completado = false;
                               }
                           }
                       }
                       
                       // Update event counter after marking/unmarking
                       actualizarContadorEventos();
                   }
               );
            } else {
                menu.addAction("Marcar como APLICABLE a mi servicio",
                    [=]{
                        actualCompleteEvent->setCompletado(true);
                        eventosModel->setData(item, QVariant::fromValue(actualCompleteEvent->riesgo), RoleRisk);
                        ui->eventos->repaint();

                        for(auto &listaEventos : events){
                            for (auto &evento : listaEventos.second) {
                                if (evento.id == eventoActualId){
                                    evento.completado = true;
                                }
                            }
                        }
                        
                        // Update event counter after marking/unmarking
                        actualizarContadorEventos();
                    }
                );
            }

            menu.addAction("Editar",
                [=]{
                    sucesos wndSucesos(this);
                    wndSucesos.establecerSucesoActivo(actualCompleteEvent);

                    int returnSucesos = wndSucesos.exec();
                    if (returnSucesos == QDialog::Accepted) {
                        // Store current state before reloading
                        QStandardItem *prevItem = this->actualItem;
                        std::pair<int, int> prevElementId = this->actualElementId;
                        std::string prevStageIndex = this->actualStageIndex;
                        QString prevEtapaSubetapa = this->etapaSubetapaSeleccionada;
                        int prevEventoId = this->eventoActualId;
                        QString prevEventName = actualCompleteEvent->nombre;
                        
                        // Reload the model
                        loadModel();
                        
                        // Restore stage selection
                        this->actualItem = prevItem;
                        this->actualElementId = prevElementId;
                        this->actualStageIndex = prevStageIndex;
                        this->etapaSubetapaSeleccionada = prevEtapaSubetapa;
                        this->eventoActualId = prevEventoId;
                        
                        // Restore stage/substage UI state
                        if (ui->btTodasEtapas->isChecked()) {
                            ui->etEtapaActual->setText("**** Todas las etapas ****");
                        } else {
                            QString ruta;
                            auto parent = actualItem->parent();
                            if (parent != nullptr) {
                                ruta = parent->data(Qt::DisplayRole).toString() + " / ";
                            }
                            ruta += actualItem->data(Qt::DisplayRole).toString();
                            ui->etEtapaActual->setText(ruta);
                        }

                        // Reload events for the current stage
                        mostrarEventosSeleccion();
                        actualizarContadorEventos();
                        
                        // Find and select the previously selected event
                        for (int i = 0; i < eventosModel->rowCount(); i++) {
                            QModelIndex idx = eventosModel->index(i, 0);
                            if (idx.data(RoleName).toString() == prevEventName) {
                                ui->eventos->setCurrentIndex(idx);
                                on_eventos_clicked(idx);
                                break;
                            }
                        }
                    }
                }
            );
        }

        menu.exec(ui->eventos->mapToGlobal(pos));
    }
}

void MainWindow::alternarDefensasEventoActual(TipoDefensa tipoDefensa, bool habilitar)
{
    for(auto &listaEventos : events){
        for (auto &evento : listaEventos.second) {
            if (evento.id == eventoActualId) {
                std::vector<DefenseDefinition> *defensas;
                switch (tipoDefensa) {
                case ReductoresFrecuencia:
                    defensas = &evento.reductoresFrecuencias;
                    break;
                case ReductoresConsecuencia:
                    defensas = &evento.reductoresConsecuencias;
                    break;
                case Barreras:
                    defensas = &evento.barreras;
                    break;
                }

                for (auto &defense : *defensas) {
                    if (defense.active != habilitar) {
                        defense.active = habilitar;
                        defense.changed = true;
                    }
                }

                // Recalculate risk for the event in the events map
                evento.calcularReductores();
                evento.calcularRiesgo(matrizRiesgo);

                if (actualCompleteEvent != nullptr) {
                    delete actualCompleteEvent;
                }
                actualCompleteEvent = new CompleteEventDefinition(&evento);
                mostrarEvento();
                refrescarListaEventos();
            }
        }
    }
}

void MainWindow::alternarDefensaModelo(TipoDefensa tipoDefensa, bool habilitar, const QPoint &pos)
{
    QModelIndex item;
    switch (tipoDefensa) {
    case ReductoresFrecuencia:
        item = ui->lReductoresFrecuencia->indexAt(pos);
        break;
    case ReductoresConsecuencia:
        item = ui->lReductoresConsecuencia->indexAt(pos);
        break;
    case Barreras:
        item = ui->lBarreras->indexAt(pos);
        break;
    }

    if (item.isValid()) {
        QString displayText = item.data().toString();
        QString seleccionado = extractDefenseNameFromDisplay(displayText);

        for(auto &listaEventos : events){
            for (auto &evento : listaEventos.second) {
                std::vector<DefenseDefinition> *defensas;
                switch (tipoDefensa) {
                case ReductoresFrecuencia:
                    defensas = &evento.reductoresFrecuencias;
                    break;
                case ReductoresConsecuencia:
                    defensas = &evento.reductoresConsecuencias;
                    break;
                case Barreras:
                    defensas = &evento.barreras;
                    break;
                }
                for(auto &reductor : *defensas){
                    if (reductor.nombre == seleccionado){
                        if (reductor.active != habilitar) {
                            reductor.active = habilitar;
                            reductor.changed = true;
                        }
                    }
                }

                // Recalculate risk for all events that have this defense
                evento.calcularReductores();
                evento.calcularRiesgo(matrizRiesgo);

                if (evento.id == eventoActualId) {
                    if (actualCompleteEvent != nullptr) {
                        delete actualCompleteEvent;
                    }
                    actualCompleteEvent = new CompleteEventDefinition(&evento);
                }
            }
        }
        mostrarEvento();
        refrescarListaEventos();
    }
}

/**
 * @brief MainWindow::getAllStages
 * 
 * Retrieves all stages from the database, including virtual stages (id < 0).
 * This method is exposed to the scripting engine and can be called from JavaScript
 * as Marrta.getAllStages().
 * 
 * Virtual stages are special stages used in the MARRTA workflow that have negative IDs.
 * Regular stages have non-negative IDs.
 * 
 * @return QVariantList A list of stage objects, each containing:
 *         - id: Stage identifier (can be negative)
 *         - nombre: Stage name
 *         - descripcion: Stage description
 *         - orden: Display order
 *         - codigo: Stage code
 *         - subetapas: List of substage names
 * 
 * @see DbElementStage::list()
 */
QVariantList MainWindow::getAllStages()
{
    QVariantList result;
    QList<EtapaDefinition> etapas;
    Database::DbElementStage dbElementStage;
    
    // Call list with todasLasEtapas = true to include stages with id < 0 (virtual stages)
    dbElementStage.list(etapas, true);
    
    // Convert each EtapaDefinition to a QVariantMap for JavaScript access
    for (const auto& etapa : etapas) {
        QVariantMap stageMap;
        stageMap["id"] = etapa.id;
        stageMap["nombre"] = etapa.nombre;
        stageMap["descripcion"] = etapa.descripcion;
        stageMap["orden"] = etapa.orden;
        stageMap["codigo"] = etapa.codigo;
        stageMap["subetapas"] = etapa.subetapas;
        result.append(stageMap);
    }
    
    return result;
}

///////////////////////////////////////////////////
// Generación de XMLs

/**
 * @brief MainWindow::on_actionXmlIniciadoresXEtapa_triggered
 * Genera un XML con la información de las etapas y los eventos que tiene.
 */
void MainWindow::on_actionXmlIniciadoresXEtapa_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    Database::DbManager db;
    auto iniciadores = db.iniciadoresXEtapa();
    if (iniciadores.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("No hay definidos eventos para generar el archivo");
        msgBox.exec();
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar archivo",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        ("Archivo XML (*.xml)"));
    if (!fileName.isEmpty()) {
        QFile xml(fileName);
        if (xml.open(QIODevice::WriteOnly)) {
            QXmlStreamWriter xmlStream(&xml);
            xmlStream.setAutoFormatting(true);
            xmlStream.writeStartDocument();

            xmlStream.writeStartElement("elementos");
            for (auto iniciador : iniciadores) {
                xmlStream.writeStartElement("suceso_iniciador");
                xmlStream.writeTextElement("etapa", iniciador.etapa);
                xmlStream.writeTextElement("subetapa", iniciador.subetapa);
                xmlStream.writeTextElement("eventos", QString::number(iniciador.numero));
                xmlStream.writeEndElement();
            }
            xmlStream.writeEndElement();
            xmlStream.writeEndDocument();
        }
    }
}

void MainWindow::on_actionFrecuenciaIniciadoresXEtapa_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    Database::DbManager db;
    auto iniciadores = db.frecuenciasXEtapa();
    if (iniciadores.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("No hay definidos eventos para generar el archivo");
        msgBox.exec();
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar archivo",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        ("Archivo XML (*.xml)"));

    if (!fileName.isEmpty()) {
        QFile xml(fileName);
        if (xml.open(QIODevice::WriteOnly)) {
            QXmlStreamWriter xmlStream(&xml);
            xmlStream.setAutoFormatting(true);
            xmlStream.writeStartDocument();

            xmlStream.writeStartElement("elementos");
            xmlStream.writeComment("Listado de frecuencias de los iniciadores en cada etapa/subetapa");
            for (auto iniciador : iniciadores) {
                xmlStream.writeStartElement("frecuencia");
                xmlStream.writeTextElement("etapa", iniciador.etapa);
                xmlStream.writeTextElement("subetapa", iniciador.subetapa);
                xmlStream.writeTextElement("fmb", QString::number(iniciador.o0));
                xmlStream.writeTextElement("fb", QString::number(iniciador.o1));
                xmlStream.writeTextElement("fm", QString::number(iniciador.o2));
                xmlStream.writeTextElement("fa", QString::number(iniciador.o3));
                xmlStream.writeEndElement();
            }
            xmlStream.writeEndElement();
            xmlStream.writeEndDocument();
        }
    }
}

void MainWindow::on_actionConsecuenciasIniciadoresXEtapa_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    Database::DbManager db;
    auto iniciadores = db.consecuenciasXEtapa();
    if (iniciadores.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("No hay definidos eventos para generar el archivo");
        msgBox.exec();
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar archivo",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        ("Archivo XML (*.xml)"));

    if (!fileName.isEmpty()) {
        QFile xml(fileName);
        if (xml.open(QIODevice::WriteOnly)) {
            QXmlStreamWriter xmlStream(&xml);
            xmlStream.setAutoFormatting(true);
            xmlStream.writeStartDocument();

            xmlStream.writeStartElement("elementos");
            xmlStream.writeComment("Listado de consecuencias de los iniciadores en cada etapa/subetapa");
            for (auto iniciador : iniciadores) {
                xmlStream.writeStartElement("consecuencia");
                xmlStream.writeTextElement("etapa", "<!CDATA[" + iniciador.etapa + "]]>");
                xmlStream.writeTextElement("subetapa", iniciador.subetapa);
                xmlStream.writeTextElement("fmb", QString::number(iniciador.o0));
                xmlStream.writeTextElement("fb", QString::number(iniciador.o1));
                xmlStream.writeTextElement("fm", QString::number(iniciador.o2));
                xmlStream.writeTextElement("fa", QString::number(iniciador.o3));
                xmlStream.writeEndElement();
            }
            xmlStream.writeEndElement();
            xmlStream.writeEndDocument();
        }
    }
}

void MainWindow::on_actionDistribucionRiesgoEventos_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    Database::DbManager db;
    auto iniciadores = db.consecuenciasXEtapa();
    if (iniciadores.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("No hay definidos eventos para generar el archivo");
        msgBox.exec();
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar archivo",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        ("Archivo XML (*.xml)"));

    if (!fileName.isEmpty()) {
        QFile xml(fileName);
        if (xml.open(QIODevice::WriteOnly)) {
            QXmlStreamWriter xmlStream(&xml);
            xmlStream.setAutoFormatting(true);
            xmlStream.writeStartDocument();

            xmlStream.writeStartElement("elementos");
            xmlStream.writeComment("Perfil de riesgo");
            for (auto iniciador : iniciadores) {
                xmlStream.writeStartElement("consecuencia");
                xmlStream.writeTextElement("etapa", iniciador.etapa);
                xmlStream.writeTextElement("subetapa", iniciador.subetapa);
                xmlStream.writeTextElement("rmb", QString::number(iniciador.o0));
                xmlStream.writeTextElement("rb", QString::number(iniciador.o1));
                xmlStream.writeTextElement("ra", QString::number(iniciador.o2));
                xmlStream.writeTextElement("rma", QString::number(iniciador.o3));
                xmlStream.writeEndElement();
            }
            xmlStream.writeEndElement();
            xmlStream.writeEndDocument();
        }
    }
}

void MainWindow::on_actionDistribucionBarrerasIniciadores_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    Database::DbManager db;
    auto iniciadores = db.barrerasEnIniciadores();
    if (iniciadores.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("No hay definidos eventos para generar el archivo");
        msgBox.exec();
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Guardar archivo",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        ("Archivo XML (*.xml)"));

    if (!fileName.isEmpty()) {
        QFile xml(fileName);
        if (xml.open(QIODevice::WriteOnly)) {
            QXmlStreamWriter xmlStream(&xml);
            xmlStream.setAutoFormatting(true);
            xmlStream.writeStartDocument();

            xmlStream.writeStartElement("elementos");
            xmlStream.writeComment("Distribución de barreras en iniciadores");
            for (auto iniciador : iniciadores) {
                xmlStream.writeStartElement("barrera");
                xmlStream.writeTextElement("nombre", "<!CDATA[" + iniciador.first + "]]>");
                xmlStream.writeTextElement("veces", QString::number(iniciador.second));
                xmlStream.writeEndElement();
            }
            xmlStream.writeEndElement();
            xmlStream.writeEndDocument();
        }
    }
}

void MainWindow::on_actionMostrarEtapas_triggered()
{
    /*if (ui->stages->width() == 0) {
        ui->stages->
    }*/
}

void MainWindow::on_eventos_activated(const QModelIndex &index)
{
    Q_UNUSED(index);
    //qDebug() << index.data();
}

void MainWindow::on_eventos_clicked(const QModelIndex &index)
{
    etapaSubetapaSeleccionada = index.data(RoleStage).toString() + "-" + index.data(RoleSubstage).toString();

    QString nombre = index.data(RoleName).toString();
    auto listaEventos = events.find(etapaSubetapaSeleccionada.toStdString())->second;
    for (auto evento : listaEventos) {
        if (evento.nombre == nombre) {
            eventoActualId = evento.id;
            if (actualCompleteEvent != nullptr) {
                delete actualCompleteEvent;
            }
            actualCompleteEvent = new CompleteEventDefinition(&evento);
            mostrarEvento();
            break;
        }
    }
}

void MainWindow::on_eventos_doubleClicked(const QModelIndex &index)
{
    if (actualCompleteEvent) {
        sucesos wndSucesos(this);
        wndSucesos.establecerSucesoActivo(actualCompleteEvent);

        int returnSucesos = wndSucesos.exec();
        if (returnSucesos == QDialog::Accepted) {
            // Store current state before reloading
            QStandardItem *prevItem = this->actualItem;
            std::pair<int, int> prevElementId = this->actualElementId;
            std::string prevStageIndex = this->actualStageIndex;
            QString prevEtapaSubetapa = this->etapaSubetapaSeleccionada;
            int prevEventoId = this->eventoActualId;
            QString prevEventName = actualCompleteEvent->nombre;
            
            // Reload the model
            loadModel();
            
            // Restore stage selection
            this->actualItem = prevItem;
            this->actualElementId = prevElementId;
            this->actualStageIndex = prevStageIndex;
            this->etapaSubetapaSeleccionada = prevEtapaSubetapa;
            this->eventoActualId = prevEventoId;
            
            // Restore stage/substage UI state
            if (ui->btTodasEtapas->isChecked()) {
                ui->etEtapaActual->setText("**** Todas las etapas ****");
            } else {
                QString ruta;
                auto parent = actualItem->parent();
                if (parent != nullptr) {
                    ruta = parent->data(Qt::DisplayRole).toString() + " / ";
                }
                ruta += actualItem->data(Qt::DisplayRole).toString();
                ui->etEtapaActual->setText(ruta);
            }

            // Reload events for the current stage
            mostrarEventosSeleccion();
            actualizarContadorEventos();
            
            // Find and select the previously selected event
            for (int i = 0; i < eventosModel->rowCount(); i++) {
                QModelIndex idx = eventosModel->index(i, 0);
                if (idx.data(RoleName).toString() == prevEventName) {
                    ui->eventos->setCurrentIndex(idx);
                    on_eventos_clicked(idx);
                    break;
                }
            }
        }
    }
}

// apm - gestionar que el usuario cierre la ventana de golpe con la "X"
void MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    on_actionSalir_triggered();
}
// apm

void MainWindow::on_actionEditor_triggered()
{
    // Check if there's a model loaded
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    ScriptingEditor *scriptingEditor = new ScriptingEditor(this, scriptingEngine);
    scriptingEditor->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed
    scriptingEditor->show(); // Non-blocking - allows access to main window
}

void MainWindow::on_actionEditorPython_triggered()
{
    // Check if there's a model loaded
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    PythonScriptingEditor *pythonEditor = new PythonScriptingEditor(this, pythonScriptingEngine);
    pythonEditor->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed
    pythonEditor->show(); // Non-blocking - allows access to main window
}

// Recent files management methods
void MainWindow::setupRecentFilesMenu()
{
    recentFilesMenu = findChild<QMenu *>("menuRecientes");
    
    // Create recent file actions
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], &QAction::triggered, this, &MainWindow::openRecentFile);
        recentFilesMenu->addAction(recentFileActions[i]);
    }
    
    // Add separator and clear action
    recentFilesMenu->addSeparator();
    QAction *clearAction = new QAction(tr("Limpiar lista"), this);
    connect(clearAction, &QAction::triggered, this, &MainWindow::clearRecentFiles);
    recentFilesMenu->addAction(clearAction);
    
    // Update the menu with current recent files
    updateRecentFilesMenu();
}

void MainWindow::updateRecentFiles(const QString &fileName)
{
    // Remove if already in list
    settings.recentFiles.removeAll(fileName);
    
    // Add to front
    settings.recentFiles.prepend(fileName);
    
    // Limit the size
    while (settings.recentFiles.size() > MaxRecentFiles) {
        settings.recentFiles.removeLast();
    }
    
    // Update the menu
    updateRecentFilesMenu();
    
    // Save settings
    saveSettings();
}

void MainWindow::updateRecentFilesMenu()
{
    int numRecentFiles = qMin(settings.recentFiles.size(), MaxRecentFiles);
    
    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1 %2").arg(i + 1).arg(QFileInfo(settings.recentFiles[i]).fileName());
        recentFileActions[i]->setText(text);
        recentFileActions[i]->setData(settings.recentFiles[i]);
        recentFileActions[i]->setVisible(true);
    }
    
    for (int i = numRecentFiles; i < MaxRecentFiles; ++i) {
        recentFileActions[i]->setVisible(false);
    }
    
    // Enable/disable the menu based on whether there are recent files
    recentFilesMenu->setEnabled(numRecentFiles > 0);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString fileName = action->data().toString();
        if (QFile::exists(fileName)) {
            openPraxisFile(fileName);
        } else {
            QMessageBox::warning(this, tr("Archivo no encontrado"), 
                               tr("El archivo %1 no existe.").arg(fileName));
            // Remove from recent files
            settings.recentFiles.removeAll(fileName);
            updateRecentFilesMenu();
            saveSettings();
        }
    }
}

void MainWindow::clearRecentFiles()
{
    settings.recentFiles.clear();
    updateRecentFilesMenu();
    saveSettings();
}

