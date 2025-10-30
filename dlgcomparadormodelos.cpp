#include "dlgcomparadormodelos.h"
#include "ui_dlgcomparadormodelos.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QStandardPaths>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts>

#include "dbevent.h"
#include "dbdefense.h"
#include "dbelementstage.h"
#include "dbelementsubstage.h"

DlgComparadorModelos::DlgComparadorModelos(
    const QString &currentModelPath,
    Database::ConfiguracionModelo currentConfig,
    std::unordered_map<std::string, std::vector<CompleteEventDefinition>> *currentEvents,
    QWidget *parent
) :
    QDialog(parent),
    ui(new Ui::DlgComparadorModelos),
    modeloActualPath(currentModelPath),
    modeloActualConfig(currentConfig),
    eventosActuales(currentEvents),
    modeloComparacionCargado(false)
{
    ui->setupUi(this);
    
    // Set current model info
    ui->edModeloActual->setText(modeloActualConfig.nombre + " (" + modeloActualPath + ")");
    
    // Configure tables
    ui->tblEtapas->horizontalHeader()->setStretchLastSection(true);
    ui->tblIniciadores->horizontalHeader()->setStretchLastSection(true);
    ui->tblDefensas->horizontalHeader()->setStretchLastSection(true);
    ui->tblAsociaciones->horizontalHeader()->setStretchLastSection(true);
}

DlgComparadorModelos::~DlgComparadorModelos()
{
    if (dbComparacion.isOpen()) {
        dbComparacion.close();
    }
    delete ui;
}

void DlgComparadorModelos::on_btnSeleccionarModelo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Seleccionar modelo para comparar"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        tr("Archivos PRAXIS (*.pdb)")
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Check if it's the same file
    if (fileName == modeloActualPath) {
        QMessageBox::warning(
            this,
            tr("Error"),
            tr("No puede comparar el modelo consigo mismo. Por favor, seleccione un modelo diferente.")
        );
        return;
    }
    
    modeloComparacionPath = fileName;
    cargarModeloComparacion();
}

void DlgComparadorModelos::cargarModeloComparacion()
{
    try {
        // Close previous connection if any
        if (dbComparacion.isOpen()) {
            QString connectionName = dbComparacion.connectionName();
            dbComparacion.close();
            QSqlDatabase::removeDatabase(connectionName);
        }
        
        // Create a unique connection name
        QString connectionName = "comparacion_" + QString::number(QDateTime::currentMSecsSinceEpoch());
        dbComparacion = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        dbComparacion.setDatabaseName(modeloComparacionPath);
        
        if (!dbComparacion.open()) {
            QMessageBox::critical(
                this,
                tr("Error"),
                tr("No se puede abrir el archivo de base de datos:\n%1").arg(modeloComparacionPath)
            );
            return;
        }
        
        // Check database version
        QSqlQuery query(dbComparacion);
        if (!query.exec("SELECT version, nombre FROM configuracion LIMIT 1") || !query.first()) {
            QMessageBox::critical(
                this,
                tr("Error"),
                tr("El archivo seleccionado no es un modelo PRAXIS válido.")
            );
            dbComparacion.close();
            return;
        }
        
        modeloComparacionConfig.version = query.value(0).toString();
        modeloComparacionConfig.nombre = query.value(1).toString();
        
        // Load events from comparison database
        eventosComparacion.clear();
        if (!cargarEventosDeDB(dbComparacion, eventosComparacion)) {
            QMessageBox::critical(
                this,
                tr("Error"),
                tr("Error al cargar los eventos del modelo de comparación.")
            );
            dbComparacion.close();
            return;
        }
        
        ui->edModeloComparacion->setText(modeloComparacionConfig.nombre + " (" + modeloComparacionPath + ")");
        ui->btnComparar->setEnabled(true);
        modeloComparacionCargado = true;
        
        QMessageBox::information(
            this,
            tr("Modelo Cargado"),
            tr("El modelo '%1' se ha cargado correctamente.\nAhora puede realizar la comparación.")
                .arg(modeloComparacionConfig.nombre)
        );
        
    } catch (const std::exception &e) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Error al cargar el modelo: %1").arg(e.what())
        );
    }
}

bool DlgComparadorModelos::cargarEventosDeDB(
    QSqlDatabase &db,
    std::unordered_map<std::string, std::vector<CompleteEventDefinition>> &events
)
{
    QSqlQuery query(db);
    
    QString sql = R"""(
        SELECT 
            si.id, si.nombre, si.descripcion, si.codigo,
            si.etapa_id, si.subetapa_id,
            si.frecuencia, si.consecuencia, si.base,
            si.completado, si.error_humano, si.riesgo
        FROM suceso_iniciador AS si
        ORDER BY si.etapa_id, si.subetapa_id, si.id
    )""";
    
    if (!query.exec(sql)) {
        qDebug() << "Error loading events:" << query.lastError().text();
        return false;
    }
    
    while (query.next()) {
        CompleteEventDefinition event;
        event.id = query.value(0).toInt();
        event.nombre = query.value(1).toString();
        event.descripcion = query.value(2).toString();
        event.codigo = query.value(3).toString();
        event.etapa = query.value(4).toInt();
        event.subetapa = query.value(5).toInt();
        event.orden = 0; // Not used for comparison, set to default
        event.frecuencia = query.value(6).toInt();
        event.consecuencia = query.value(7).toInt();
        event.base = query.value(8).toBool();
        event.completado = query.value(9).toBool();
        event.errorHumano = query.value(10).toBool();
        event.riesgo = query.value(11).toInt();
        
        // Load defenses for this event
        QSqlQuery defQuery(db);
        defQuery.prepare(R"""(
            SELECT id, nombre, codigo, robustez, activo, tipo_defensa
            FROM defensas
            WHERE suceso_iniciador_id = :eventId
        )""");
        defQuery.bindValue(":eventId", event.id);
        
        if (defQuery.exec()) {
            while (defQuery.next()) {
                DefenseDefinition defense;
                defense.id = defQuery.value(0).toInt();
                defense.nombre = defQuery.value(1).toString();
                defense.code = defQuery.value(2).toString();
                defense.hardiness = defQuery.value(3).toInt();
                defense.active = defQuery.value(4).toBool();
                
                int tipoDefensa = defQuery.value(5).toInt();
                if (tipoDefensa == 0) {
                    event.reductoresFrecuencias.push_back(defense);
                } else if (tipoDefensa == 1) {
                    event.barreras.push_back(defense);
                } else if (tipoDefensa == 2) {
                    event.reductoresConsecuencias.push_back(defense);
                }
            }
        }
        
        std::string key = std::to_string(event.etapa) + "-" + std::to_string(event.subetapa);
        events[key].push_back(event);
    }
    
    return true;
}

void DlgComparadorModelos::on_btnComparar_clicked()
{
    if (!modeloComparacionCargado) {
        QMessageBox::warning(
            this,
            tr("Advertencia"),
            tr("Primero debe cargar un modelo para comparar.")
        );
        return;
    }
    
    limpiarResultados();
    realizarComparacion();
    
    ui->tabComparison->setEnabled(true);
    ui->btnExportar->setEnabled(true);
    
    QMessageBox::information(
        this,
        tr("Comparación Completada"),
        tr("La comparación se ha realizado correctamente.\nPuede revisar los resultados en las diferentes pestañas.")
    );
}

void DlgComparadorModelos::limpiarResultados()
{
    ui->tblEtapas->setRowCount(0);
    ui->tblIniciadores->setRowCount(0);
    ui->tblDefensas->setRowCount(0);
    ui->tblAsociaciones->setRowCount(0);
    ui->txtResumen->clear();
}

void DlgComparadorModelos::realizarComparacion()
{
    compararEtapas();
    compararIniciadores();
    compararDefensas();
    compararAsociacionesDefensas();
    generarGraficas();
    
    // Generate summary
    QString resumenHTML = generarReporteHTML();
    ui->txtResumen->setHtml(resumenHTML);
}

void DlgComparadorModelos::compararEtapas()
{
    // Get stages from both models
    QSqlQuery queryActual;
    queryActual.exec(R"""(
        SELECT e.id, e.nombre, COUNT(s.id) as num_subetapas
        FROM etapas e
        LEFT JOIN subetapas s ON s.etapa_id = e.id
        GROUP BY e.id, e.nombre
        ORDER BY e.orden
    )""");
    
    std::map<QString, int> etapasActuales;
    while (queryActual.next()) {
        QString nombre = queryActual.value(1).toString();
        int numSubetapas = queryActual.value(2).toInt();
        etapasActuales[nombre] = numSubetapas;
    }
    
    QSqlQuery queryComp(dbComparacion);
    queryComp.exec(R"""(
        SELECT e.id, e.nombre, COUNT(s.id) as num_subetapas
        FROM etapas e
        LEFT JOIN subetapas s ON s.etapa_id = e.id
        GROUP BY e.id, e.nombre
        ORDER BY e.orden
    )""");
    
    std::map<QString, int> etapasComparacion;
    while (queryComp.next()) {
        QString nombre = queryComp.value(1).toString();
        int numSubetapas = queryComp.value(2).toInt();
        etapasComparacion[nombre] = numSubetapas;
    }
    
    // Compare and populate table
    std::set<QString> todasEtapas;
    for (const auto &pair : etapasActuales) {
        todasEtapas.insert(pair.first);
    }
    for (const auto &pair : etapasComparacion) {
        todasEtapas.insert(pair.first);
    }
    
    int row = 0;
    for (const QString &etapa : todasEtapas) {
        ui->tblEtapas->insertRow(row);
        
        int subActual = etapasActuales.count(etapa) ? etapasActuales[etapa] : 0;
        int subComp = etapasComparacion.count(etapa) ? etapasComparacion[etapa] : 0;
        
        ui->tblEtapas->setItem(row, 0, new QTableWidgetItem(etapa));
        ui->tblEtapas->setItem(row, 1, new QTableWidgetItem(QString::number(subActual)));
        ui->tblEtapas->setItem(row, 2, new QTableWidgetItem(QString::number(subComp)));
        
        QString estado;
        QColor color;
        if (subActual == 0 && subComp > 0) {
            estado = tr("Solo en externo");
            color = QColor(255, 200, 200);
        } else if (subActual > 0 && subComp == 0) {
            estado = tr("Solo en actual");
            color = QColor(200, 200, 255);
        } else if (subActual != subComp) {
            estado = tr("Diferente");
            color = QColor(255, 255, 200);
        } else {
            estado = tr("Igual");
            color = QColor(200, 255, 200);
        }
        
        QTableWidgetItem *estadoItem = new QTableWidgetItem(estado);
        estadoItem->setBackground(color);
        ui->tblEtapas->setItem(row, 3, estadoItem);
        
        row++;
    }
}

void DlgComparadorModelos::compararIniciadores()
{
    // Create maps for comparison
    std::map<QString, CompleteEventDefinition*> iniciadoresActuales;
    for (auto &pair : *eventosActuales) {
        for (auto &event : pair.second) {
            iniciadoresActuales[event.codigo] = &event;
        }
    }
    
    std::map<QString, CompleteEventDefinition*> iniciadoresComp;
    for (auto &pair : eventosComparacion) {
        for (auto &event : pair.second) {
            iniciadoresComp[event.codigo] = &event;
        }
    }
    
    // Get all unique codes
    std::set<QString> todosCodigos;
    for (const auto &pair : iniciadoresActuales) {
        todosCodigos.insert(pair.first);
    }
    for (const auto &pair : iniciadoresComp) {
        todosCodigos.insert(pair.first);
    }
    
    // Populate table
    int row = 0;
    for (const QString &codigo : todosCodigos) {
        ui->tblIniciadores->insertRow(row);
        
        QString nombreActual = iniciadoresActuales.count(codigo) ? 
            iniciadoresActuales[codigo]->nombre : tr("(No existe)");
        QString nombreExterno = iniciadoresComp.count(codigo) ? 
            iniciadoresComp[codigo]->nombre : tr("(No existe)");
        
        int fActual = iniciadoresActuales.count(codigo) ? 
            iniciadoresActuales[codigo]->frecuencia : -1;
        int fComp = iniciadoresComp.count(codigo) ? 
            iniciadoresComp[codigo]->frecuencia : -1;
        
        int cActual = iniciadoresActuales.count(codigo) ? 
            iniciadoresActuales[codigo]->consecuencia : -1;
        int cComp = iniciadoresComp.count(codigo) ? 
            iniciadoresComp[codigo]->consecuencia : -1;
        
        QString etapa = "";
        if (iniciadoresActuales.count(codigo)) {
            QSqlQuery query;
            query.prepare("SELECT nombre FROM etapas WHERE id = :id");
            query.bindValue(":id", iniciadoresActuales[codigo]->etapa);
            if (query.exec() && query.first()) {
                etapa = query.value(0).toString();
            }
        }
        
        ui->tblIniciadores->setItem(row, 0, new QTableWidgetItem(codigo));
        ui->tblIniciadores->setItem(row, 1, new QTableWidgetItem(nombreActual));
        ui->tblIniciadores->setItem(row, 2, new QTableWidgetItem(nombreExterno));
        ui->tblIniciadores->setItem(row, 3, new QTableWidgetItem(fActual >= 0 ? QString::number(fActual) : "-"));
        ui->tblIniciadores->setItem(row, 4, new QTableWidgetItem(fComp >= 0 ? QString::number(fComp) : "-"));
        ui->tblIniciadores->setItem(row, 5, new QTableWidgetItem(cActual >= 0 ? QString::number(cActual) : "-"));
        ui->tblIniciadores->setItem(row, 6, new QTableWidgetItem(cComp >= 0 ? QString::number(cComp) : "-"));
        ui->tblIniciadores->setItem(row, 7, new QTableWidgetItem(etapa));
        
        QStringList diferencias;
        if (nombreActual == tr("(No existe)")) {
            diferencias << tr("Nuevo en externo");
        } else if (nombreExterno == tr("(No existe)")) {
            diferencias << tr("Eliminado en externo");
        } else {
            if (nombreActual != nombreExterno) {
                diferencias << tr("Nombre diferente");
            }
            if (fActual != fComp) {
                diferencias << tr("Frecuencia diferente");
            }
            if (cActual != cComp) {
                diferencias << tr("Consecuencia diferente");
            }
            if (diferencias.isEmpty()) {
                diferencias << tr("Sin cambios");
            }
        }
        
        QTableWidgetItem *difItem = new QTableWidgetItem(diferencias.join(", "));
        if (!diferencias.contains(tr("Sin cambios"))) {
            difItem->setBackground(QColor(255, 255, 200));
        }
        ui->tblIniciadores->setItem(row, 8, difItem);
        
        row++;
    }
}

void DlgComparadorModelos::compararDefensas()
{
    // Collect all unique defenses from both models
    std::map<QString, DefenseDefinition> defensasActuales;
    for (auto &pair : *eventosActuales) {
        for (auto &event : pair.second) {
            for (auto &def : event.reductoresFrecuencias) {
                if (defensasActuales.count(def.code) == 0) {
                    defensasActuales[def.code] = def;
                }
            }
            for (auto &def : event.barreras) {
                if (defensasActuales.count(def.code) == 0) {
                    defensasActuales[def.code] = def;
                }
            }
            for (auto &def : event.reductoresConsecuencias) {
                if (defensasActuales.count(def.code) == 0) {
                    defensasActuales[def.code] = def;
                }
            }
        }
    }
    
    std::map<QString, DefenseDefinition> defensasComp;
    for (auto &pair : eventosComparacion) {
        for (auto &event : pair.second) {
            for (auto &def : event.reductoresFrecuencias) {
                if (defensasComp.count(def.code) == 0) {
                    defensasComp[def.code] = def;
                }
            }
            for (auto &def : event.barreras) {
                if (defensasComp.count(def.code) == 0) {
                    defensasComp[def.code] = def;
                }
            }
            for (auto &def : event.reductoresConsecuencias) {
                if (defensasComp.count(def.code) == 0) {
                    defensasComp[def.code] = def;
                }
            }
        }
    }
    
    // Get all unique codes
    std::set<QString> todosCodigos;
    for (const auto &pair : defensasActuales) {
        todosCodigos.insert(pair.first);
    }
    for (const auto &pair : defensasComp) {
        todosCodigos.insert(pair.first);
    }
    
    // Populate table
    int row = 0;
    for (const QString &codigo : todosCodigos) {
        ui->tblDefensas->insertRow(row);
        
        QString nombreActual = defensasActuales.count(codigo) ? 
            defensasActuales[codigo].nombre : tr("(No existe)");
        QString nombreExterno = defensasComp.count(codigo) ? 
            defensasComp[codigo].nombre : tr("(No existe)");
        
        int robActual = defensasActuales.count(codigo) ? 
            defensasActuales[codigo].hardiness : -1;
        int robComp = defensasComp.count(codigo) ? 
            defensasComp[codigo].hardiness : -1;
        
        ui->tblDefensas->setItem(row, 0, new QTableWidgetItem(codigo));
        ui->tblDefensas->setItem(row, 1, new QTableWidgetItem(nombreActual));
        ui->tblDefensas->setItem(row, 2, new QTableWidgetItem(nombreExterno));
        ui->tblDefensas->setItem(row, 3, new QTableWidgetItem(robActual >= 0 ? QString::number(robActual) : "-"));
        ui->tblDefensas->setItem(row, 4, new QTableWidgetItem(robComp >= 0 ? QString::number(robComp) : "-"));
        
        QStringList diferencias;
        if (nombreActual == tr("(No existe)")) {
            diferencias << tr("Nueva en externo");
        } else if (nombreExterno == tr("(No existe)")) {
            diferencias << tr("Eliminada en externo");
        } else {
            if (nombreActual != nombreExterno) {
                diferencias << tr("Nombre diferente");
            }
            if (robActual != robComp) {
                diferencias << tr("Robustez diferente");
            }
            if (diferencias.isEmpty()) {
                diferencias << tr("Sin cambios");
            }
        }
        
        QTableWidgetItem *difItem = new QTableWidgetItem(diferencias.join(", "));
        if (!diferencias.contains(tr("Sin cambios"))) {
            difItem->setBackground(QColor(255, 255, 200));
        }
        ui->tblDefensas->setItem(row, 5, difItem);
        
        row++;
    }
}

void DlgComparadorModelos::compararAsociacionesDefensas()
{
    // This compares which defenses are associated (established and applied) with which initiators
    std::map<QString, std::set<QString>> asocActuales; // initiator -> set of defense codes
    std::map<QString, std::set<QString>> asocAplicadasActuales;
    
    for (auto &pair : *eventosActuales) {
        for (auto &event : pair.second) {
            // All defenses in the event are "established"
            for (auto &def : event.reductoresFrecuencias) {
                asocActuales[event.codigo].insert(def.code);
                if (def.active) {
                    asocAplicadasActuales[event.codigo].insert(def.code);
                }
            }
            for (auto &def : event.barreras) {
                asocActuales[event.codigo].insert(def.code);
                if (def.active) {
                    asocAplicadasActuales[event.codigo].insert(def.code);
                }
            }
            for (auto &def : event.reductoresConsecuencias) {
                asocActuales[event.codigo].insert(def.code);
                if (def.active) {
                    asocAplicadasActuales[event.codigo].insert(def.code);
                }
            }
        }
    }
    
    std::map<QString, std::set<QString>> asocComp;
    std::map<QString, std::set<QString>> asocAplicadasComp;
    
    for (auto &pair : eventosComparacion) {
        for (auto &event : pair.second) {
            for (auto &def : event.reductoresFrecuencias) {
                asocComp[event.codigo].insert(def.code);
                if (def.active) {
                    asocAplicadasComp[event.codigo].insert(def.code);
                }
            }
            for (auto &def : event.barreras) {
                asocComp[event.codigo].insert(def.code);
                if (def.active) {
                    asocAplicadasComp[event.codigo].insert(def.code);
                }
            }
            for (auto &def : event.reductoresConsecuencias) {
                asocComp[event.codigo].insert(def.code);
                if (def.active) {
                    asocAplicadasComp[event.codigo].insert(def.code);
                }
            }
        }
    }
    
    // Get all initiator codes
    std::set<QString> todosIniciadores;
    for (const auto &pair : asocActuales) {
        todosIniciadores.insert(pair.first);
    }
    for (const auto &pair : asocComp) {
        todosIniciadores.insert(pair.first);
    }
    
    // Compare associations
    int row = 0;
    for (const QString &iniciador : todosIniciadores) {
        const auto &defensasActual = asocActuales[iniciador];
        const auto &defensasComp = asocComp[iniciador];
        const auto &defensasAplActual = asocAplicadasActuales[iniciador];
        const auto &defensasAplComp = asocAplicadasComp[iniciador];
        
        // Get all defense codes for this initiator
        std::set<QString> todasDefensas;
        todasDefensas.insert(defensasActual.begin(), defensasActual.end());
        todasDefensas.insert(defensasComp.begin(), defensasComp.end());
        
        for (const QString &defensa : todasDefensas) {
            ui->tblAsociaciones->insertRow(row);
            
            bool estActual = defensasActual.count(defensa) > 0;
            bool estComp = defensasComp.count(defensa) > 0;
            bool aplActual = defensasAplActual.count(defensa) > 0;
            bool aplComp = defensasAplComp.count(defensa) > 0;
            
            ui->tblAsociaciones->setItem(row, 0, new QTableWidgetItem(iniciador));
            ui->tblAsociaciones->setItem(row, 1, new QTableWidgetItem(defensa));
            ui->tblAsociaciones->setItem(row, 2, new QTableWidgetItem(estActual ? tr("Sí") : tr("No")));
            ui->tblAsociaciones->setItem(row, 3, new QTableWidgetItem(estComp ? tr("Sí") : tr("No")));
            ui->tblAsociaciones->setItem(row, 4, new QTableWidgetItem(aplActual ? tr("Sí") : tr("No")));
            ui->tblAsociaciones->setItem(row, 5, new QTableWidgetItem(aplComp ? tr("Sí") : tr("No")));
            
            QString estado;
            QColor color;
            if (estActual != estComp) {
                estado = tr("Establecimiento diferente");
                color = QColor(255, 200, 200);
            } else if (aplActual != aplComp) {
                estado = tr("Aplicación diferente");
                color = QColor(255, 255, 200);
            } else {
                estado = tr("Sin cambios");
                color = QColor(200, 255, 200);
            }
            
            QTableWidgetItem *estadoItem = new QTableWidgetItem(estado);
            estadoItem->setBackground(color);
            ui->tblAsociaciones->setItem(row, 6, estadoItem);
            
            row++;
        }
    }
}

void DlgComparadorModelos::generarGraficas()
{
    // Generate comparison charts
    
    // Clear existing charts
    QLayout *layout = ui->scrollAreaWidgetContents->layout();
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    
    // Count risk levels in both models
    std::map<int, int> riesgosActuales;
    std::map<int, int> riesgosComp;
    
    for (auto &pair : *eventosActuales) {
        for (auto &event : pair.second) {
            if (event.completado) { // Only count applicable events
                riesgosActuales[event.riesgo]++;
            }
        }
    }
    
    for (auto &pair : eventosComparacion) {
        for (auto &event : pair.second) {
            if (event.completado) {
                riesgosComp[event.riesgo]++;
            }
        }
    }
    
    // Create bar chart for risk comparison
    QBarSet *setActual = new QBarSet(tr("Modelo Actual"));
    QBarSet *setComp = new QBarSet(tr("Modelo Externo"));
    
    *setActual << riesgosActuales[3] << riesgosActuales[2] << riesgosActuales[1] << riesgosActuales[0];
    *setComp << riesgosComp[3] << riesgosComp[2] << riesgosComp[1] << riesgosComp[0];
    
    QBarSeries *series = new QBarSeries();
    series->append(setActual);
    series->append(setComp);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Perfil de Riesgo - Comparación"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    QStringList categories;
    categories << tr("Riesgo Muy Alto") << tr("Riesgo Alto") << tr("Riesgo Medio") << tr("Riesgo Bajo");
    
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText(tr("Número de Iniciadores"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(400);
    
    layout->addWidget(chartView);
    
    // Add a label with summary statistics
    int totalActual = 0, totalComp = 0;
    for (const auto &pair : riesgosActuales) {
        totalActual += pair.second;
    }
    for (const auto &pair : riesgosComp) {
        totalComp += pair.second;
    }
    
    QLabel *lblStats = new QLabel();
    lblStats->setText(tr("<b>Estadísticas:</b><br>"
                        "Total iniciadores aplicables (Actual): %1<br>"
                        "Total iniciadores aplicables (Externo): %2")
                        .arg(totalActual).arg(totalComp));
    lblStats->setMargin(10);
    layout->addWidget(lblStats);
}

QString DlgComparadorModelos::generarReporteHTML()
{
    QString html;
    html += "<html><head><style>";
    html += "body { font-family: Arial, sans-serif; }";
    html += "h1 { color: #0066cc; }";
    html += "h2 { color: #0088cc; margin-top: 20px; }";
    html += "table { border-collapse: collapse; width: 100%; margin: 10px 0; }";
    html += "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }";
    html += "th { background-color: #0066cc; color: white; }";
    html += ".igual { background-color: #c8ffc8; }";
    html += ".diferente { background-color: #ffffc8; }";
    html += ".nuevo { background-color: #ffc8c8; }";
    html += "</style></head><body>";
    
    html += "<h1>" + tr("Comparación de Modelos de Riesgo") + "</h1>";
    html += "<p><b>" + tr("Modelo Actual:") + "</b> " + modeloActualConfig.nombre + "</p>";
    html += "<p><b>" + tr("Modelo Externo:") + "</b> " + modeloComparacionConfig.nombre + "</p>";
    html += "<p><b>" + tr("Fecha de comparación:") + "</b> " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + "</p>";
    
    // Summary of stages
    html += "<h2>" + tr("Resumen de Etapas y Subetapas") + "</h2>";
    int totalEtapasActual = ui->tblEtapas->rowCount();
    int etapasIguales = 0, etapasDiferentes = 0;
    for (int i = 0; i < ui->tblEtapas->rowCount(); i++) {
        QString estado = ui->tblEtapas->item(i, 3)->text();
        if (estado == tr("Igual")) etapasIguales++;
        else etapasDiferentes++;
    }
    html += "<p>" + tr("Total de etapas: %1").arg(totalEtapasActual) + "</p>";
    html += "<p>" + tr("Etapas idénticas: %1").arg(etapasIguales) + "</p>";
    html += "<p>" + tr("Etapas con diferencias: %1").arg(etapasDiferentes) + "</p>";
    
    // Summary of initiators
    html += "<h2>" + tr("Resumen de Iniciadores") + "</h2>";
    int totalIniciadores = ui->tblIniciadores->rowCount();
    int iniciadoresIguales = 0, iniciadoresdif = 0;
    for (int i = 0; i < ui->tblIniciadores->rowCount(); i++) {
        QString dif = ui->tblIniciadores->item(i, 8)->text();
        if (dif == tr("Sin cambios")) iniciadoresIguales++;
        else iniciadoresdif++;
    }
    html += "<p>" + tr("Total de iniciadores: %1").arg(totalIniciadores) + "</p>";
    html += "<p>" + tr("Iniciadores sin cambios: %1").arg(iniciadoresIguales) + "</p>";
    html += "<p>" + tr("Iniciadores con diferencias: %1").arg(iniciadoresdif) + "</p>";
    
    // Summary of defenses
    html += "<h2>" + tr("Resumen de Defensas") + "</h2>";
    int totalDefensas = ui->tblDefensas->rowCount();
    int defensasIguales = 0, defensasDif = 0;
    for (int i = 0; i < ui->tblDefensas->rowCount(); i++) {
        QString dif = ui->tblDefensas->item(i, 5)->text();
        if (dif == tr("Sin cambios")) defensasIguales++;
        else defensasDif++;
    }
    html += "<p>" + tr("Total de defensas: %1").arg(totalDefensas) + "</p>";
    html += "<p>" + tr("Defensas sin cambios: %1").arg(defensasIguales) + "</p>";
    html += "<p>" + tr("Defensas con diferencias: %1").arg(defensasDif) + "</p>";
    
    // Summary of associations
    html += "<h2>" + tr("Resumen de Asociaciones") + "</h2>";
    int totalAsoc = ui->tblAsociaciones->rowCount();
    int asocIguales = 0, asocDif = 0;
    for (int i = 0; i < ui->tblAsociaciones->rowCount(); i++) {
        QString estado = ui->tblAsociaciones->item(i, 6)->text();
        if (estado == tr("Sin cambios")) asocIguales++;
        else asocDif++;
    }
    html += "<p>" + tr("Total de asociaciones: %1").arg(totalAsoc) + "</p>";
    html += "<p>" + tr("Asociaciones sin cambios: %1").arg(asocIguales) + "</p>";
    html += "<p>" + tr("Asociaciones con diferencias: %1").arg(asocDif) + "</p>";
    
    html += "</body></html>";
    
    return html;
}

void DlgComparadorModelos::on_btnExportar_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Exportar Reporte de Comparación"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + 
            "/comparacion_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".html",
        tr("Archivos HTML (*.html)")
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("No se puede crear el archivo:\n%1").arg(fileName)
        );
        return;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << generarReporteHTML();
    file.close();
    
    QMessageBox::information(
        this,
        tr("Exportación Completada"),
        tr("El reporte se ha exportado correctamente a:\n%1").arg(fileName)
    );
}

void DlgComparadorModelos::on_tabComparison_currentChanged(int index)
{
    Q_UNUSED(index);
    // Could be used to lazy-load content for performance
}
