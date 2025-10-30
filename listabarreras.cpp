#include "listabarreras.h"
#include "ui_listabarreras.h"
#include "dbmanager.h"
#include "dbdefense.h"
#include "matrizriesgo.h"
#include "defensecalc.h"
#include "dlgadddefensetoevents.h"
#include "styles.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QInputDialog>
#include <QTreeView>
#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QTextCursor>
#include <QBrush>

// Implementación del delegate para robustez
QWidget *RobustezDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    
    QComboBox *combo = new QComboBox(parent);
    // Agregar opciones de robustez en orden
    combo->addItem("Muy blanda");  // -1
    combo->addItem("Blanda");      // 0
    combo->addItem("Normal");      // 1
    combo->addItem("Robusta");     // 2
    combo->addItem("Muy robusta"); // 3
    return combo;
}

void RobustezDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    if (combo) {
        QString currentText = index.data(Qt::DisplayRole).toString();
        int idx = combo->findText(currentText);
        if (idx >= 0) {
            combo->setCurrentIndex(idx);
        }
    }
}

void RobustezDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    if (combo) {
        model->setData(index, combo->currentText(), Qt::EditRole);
    }
}

ListaBarreras::ListaBarreras(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListaBarreras)
{
    ui->setupUi(this);
    
    // Configurar modelo para Reductores de Frecuencia
    modelReductoresFrecuencia = new QStandardItemModel(this);
    modelReductoresFrecuencia->setHorizontalHeaderLabels(QStringList() << "Código / Nombre" << "Robustez" << "Riesgo\nBase" << "Riesgo\nCalculado" << "Nº\nSucesos" << "Nº\nAplicable");
    
    // Crear proxy model para ordenación numérica
    proxyReductoresFrecuencia = new NumericSortProxyModel(this);
    proxyReductoresFrecuencia->setSourceModel(modelReductoresFrecuencia);
    
    ui->treeReductoresFrecuencia->setModel(proxyReductoresFrecuencia);
    ui->treeReductoresFrecuencia->setColumnWidth(0, 300);
    ui->treeReductoresFrecuencia->setColumnWidth(1, 120);
    ui->treeReductoresFrecuencia->setColumnWidth(2, 70);
    ui->treeReductoresFrecuencia->setColumnWidth(3, 70);
    ui->treeReductoresFrecuencia->setColumnWidth(4, 70);
    ui->treeReductoresFrecuencia->setColumnWidth(5, 70);
    // Hacer que solo la primera columna se estire al redimensionar
    ui->treeReductoresFrecuencia->header()->setStretchLastSection(false);
    ui->treeReductoresFrecuencia->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeReductoresFrecuencia->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->treeReductoresFrecuencia->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->treeReductoresFrecuencia->header()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->treeReductoresFrecuencia->header()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->treeReductoresFrecuencia->header()->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->treeReductoresFrecuencia->setItemDelegateForColumn(1, new RobustezDelegate(this));
    ui->treeReductoresFrecuencia->setSortingEnabled(true);
    ui->treeReductoresFrecuencia->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(modelReductoresFrecuencia, &QStandardItemModel::itemChanged, this, &ListaBarreras::onItemChanged);
    connect(ui->treeReductoresFrecuencia, &QTreeView::doubleClicked, this, &ListaBarreras::onItemDoubleClicked);
    connect(ui->treeReductoresFrecuencia, &QTreeView::customContextMenuRequested, this, &ListaBarreras::showContextMenu);
    
    // Configurar modelo para Barreras
    modelBarreras = new QStandardItemModel(this);
    modelBarreras->setHorizontalHeaderLabels(QStringList() << "Código / Nombre" << "Robustez" << "Riesgo\nBase" << "Riesgo\nCalculado" << "Nº\nSucesos" << "Nº\nAplicable");
    
    // Crear proxy model para ordenación numérica
    proxyBarreras = new NumericSortProxyModel(this);
    proxyBarreras->setSourceModel(modelBarreras);
    
    ui->treeBarreras->setModel(proxyBarreras);
    ui->treeBarreras->setColumnWidth(0, 300);
    ui->treeBarreras->setColumnWidth(1, 120);
    ui->treeBarreras->setColumnWidth(2, 70);
    ui->treeBarreras->setColumnWidth(3, 70);
    ui->treeBarreras->setColumnWidth(4, 70);
    ui->treeBarreras->setColumnWidth(5, 70);
    // Hacer que solo la primera columna se estire al redimensionar
    ui->treeBarreras->header()->setStretchLastSection(false);
    ui->treeBarreras->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeBarreras->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->treeBarreras->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->treeBarreras->header()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->treeBarreras->header()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->treeBarreras->header()->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->treeBarreras->setItemDelegateForColumn(1, new RobustezDelegate(this));
    ui->treeBarreras->setSortingEnabled(true);
    ui->treeBarreras->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(modelBarreras, &QStandardItemModel::itemChanged, this, &ListaBarreras::onItemChanged);
    connect(ui->treeBarreras, &QTreeView::doubleClicked, this, &ListaBarreras::onItemDoubleClicked);
    connect(ui->treeBarreras, &QTreeView::customContextMenuRequested, this, &ListaBarreras::showContextMenu);
    
    // Configurar modelo para Reductores de Consecuencia
    modelReductoresConsecuencia = new QStandardItemModel(this);
    modelReductoresConsecuencia->setHorizontalHeaderLabels(QStringList() << "Código / Nombre" << "Robustez" << "Riesgo\nBase" << "Riesgo\nCalculado" << "Nº\nSucesos" << "Nº\nAplicable");
    
    // Crear proxy model para ordenación numérica
    proxyReductoresConsecuencia = new NumericSortProxyModel(this);
    proxyReductoresConsecuencia->setSourceModel(modelReductoresConsecuencia);
    
    ui->treeReductoresConsecuencia->setModel(proxyReductoresConsecuencia);
    ui->treeReductoresConsecuencia->setColumnWidth(0, 300);
    ui->treeReductoresConsecuencia->setColumnWidth(1, 120);
    ui->treeReductoresConsecuencia->setColumnWidth(2, 70);
    ui->treeReductoresConsecuencia->setColumnWidth(3, 70);
    ui->treeReductoresConsecuencia->setColumnWidth(4, 70);
    ui->treeReductoresConsecuencia->setColumnWidth(5, 70);
    // Hacer que solo la primera columna se estire al redimensionar
    ui->treeReductoresConsecuencia->header()->setStretchLastSection(false);
    ui->treeReductoresConsecuencia->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeReductoresConsecuencia->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->treeReductoresConsecuencia->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->treeReductoresConsecuencia->header()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->treeReductoresConsecuencia->header()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->treeReductoresConsecuencia->header()->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->treeReductoresConsecuencia->setItemDelegateForColumn(1, new RobustezDelegate(this));
    ui->treeReductoresConsecuencia->setSortingEnabled(true);
    ui->treeReductoresConsecuencia->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(modelReductoresConsecuencia, &QStandardItemModel::itemChanged, this, &ListaBarreras::onItemChanged);
    connect(ui->treeReductoresConsecuencia, &QTreeView::doubleClicked, this, &ListaBarreras::onItemDoubleClicked);
    connect(ui->treeReductoresConsecuencia, &QTreeView::customContextMenuRequested, this, &ListaBarreras::showContextMenu);
    
    cargarDefensas();
}

ListaBarreras::~ListaBarreras()
{
    delete ui;
}

void ListaBarreras::cargarDefensas()
{
    defensasUnicas = Database::DbManager::listUniqueDefenses();
    
    modelReductoresFrecuencia->removeRows(0, modelReductoresFrecuencia->rowCount());
    modelBarreras->removeRows(0, modelBarreras->rowCount());
    modelReductoresConsecuencia->removeRows(0, modelReductoresConsecuencia->rowCount());
    
    // Cargar cada tipo de defensa con sus eventos
    cargarDefensasConEventos(modelReductoresFrecuencia, DefenseType::FrecuencyReducer);
    cargarDefensasConEventos(modelBarreras, DefenseType::Barrier);
    cargarDefensasConEventos(modelReductoresConsecuencia, DefenseType::ConsequenceReducer);
    
    // Actualizar contadores en las pestañas
    ui->tabWidget->setTabText(0, QString("Reductores de Frecuencia (%1)").arg(modelReductoresFrecuencia->rowCount()));
    ui->tabWidget->setTabText(1, QString("Barreras (%1)").arg(modelBarreras->rowCount()));
    ui->tabWidget->setTabText(2, QString("Reductores de Consecuencia (%1)").arg(modelReductoresConsecuencia->rowCount()));
}

void ListaBarreras::cargarDefensasConEventos(QStandardItemModel *model, DefenseType tipo)
{
    for (const auto &defensa : defensasUnicas) {
        if (defensa.type != tipo) {
            continue;
        }
        
        // Crear fila padre para la defensa
        QList<QStandardItem*> row;
        
        QStandardItem *itemNombre = new QStandardItem(QString("[%1] %2").arg(defensa.code).arg(defensa.name));
        itemNombre->setEditable(false);
        // Tooltip para la defensa (doble de largo ~1350px, fuente pequeña, interlineado simple)
        QString tooltipDefensa = QString("<table style='width: 1350px; font-size: 9pt;'><tr><td>"
                                        "<p style='margin: 0 0 6px 0; line-height: 1.2;'><b>%1</b></p>"
                                        "<p style='margin: 0 0 8px 0; line-height: 1.2;'>%2</p>"
                                        "<p style='margin: 0; font-style: italic; color: #666; line-height: 1.2;'>%3</p>"
                                        "</td></tr></table>")
            .arg(defensa.name)
            .arg(defensa.definition.isEmpty() ? "Sin descripción" : defensa.definition)
            .arg(obtenerTextoTipo(tipo));
        itemNombre->setToolTip(tooltipDefensa);
        row.append(itemNombre);
        
        QStandardItem *itemRobustez = new QStandardItem(obtenerTextoRobustez(defensa.hardiness));
        itemRobustez->setEditable(true); // Hacer editable para dropdown
        itemRobustez->setToolTip(tooltipDefensa);
        itemRobustez->setData(defensa.hardiness, Qt::UserRole); // Guardar valor numérico
        itemRobustez->setData(QString::fromStdString(defensa.name.toStdString()), Qt::UserRole + 1); // Guardar nombre defensa
        itemRobustez->setData(static_cast<int>(tipo), Qt::UserRole + 2); // Guardar tipo defensa
        row.append(itemRobustez);
        
        // Columnas vacías para riesgo en el nivel padre
        QStandardItem *itemRiesgoBase = new QStandardItem("");
        itemRiesgoBase->setEditable(false);
        itemRiesgoBase->setToolTip(tooltipDefensa);
        itemRiesgoBase->setTextAlignment(Qt::AlignCenter);
        row.append(itemRiesgoBase);
        
        QStandardItem *itemRiesgoCalc = new QStandardItem("");
        itemRiesgoCalc->setEditable(false);
        itemRiesgoCalc->setToolTip(tooltipDefensa);
        itemRiesgoCalc->setTextAlignment(Qt::AlignCenter);
        row.append(itemRiesgoCalc);
        
        // Obtener eventos que usan esta defensa
        QList<EventoInfo> eventos = obtenerEventosPorDefensa(defensa.name, tipo);
        
        // Contar cuántos están seleccionados (activos)
        int eventosActivos = 0;
        for (const auto &evento : eventos) {
            if (evento.activa) {
                eventosActivos++;
            }
        }
        
        // Columna de número total de sucesos
        QStandardItem *itemNumTotal = new QStandardItem(QString::number(eventos.size()));
        itemNumTotal->setEditable(false);
        itemNumTotal->setToolTip(tooltipDefensa);
        itemNumTotal->setTextAlignment(Qt::AlignCenter);
        itemNumTotal->setData(eventos.size(), Qt::UserRole); // Para ordenar numéricamente
        row.append(itemNumTotal);
        
        // Columna de número de sucesos aplicables (activos)
        QStandardItem *itemNumAplicable = new QStandardItem(QString::number(eventosActivos));
        itemNumAplicable->setEditable(false);
        itemNumAplicable->setToolTip(tooltipDefensa);
        itemNumAplicable->setTextAlignment(Qt::AlignCenter);
        itemNumAplicable->setData(eventosActivos, Qt::UserRole); // Para ordenar numéricamente
        row.append(itemNumAplicable);
        
        // Agregar filas hijas para cada evento
        for (const auto &evento : eventos) {
            QList<QStandardItem*> childRow;
            
            // Usar checkbox en vez de flecha
            QStandardItem *childNombre = new QStandardItem(QString("[%1] %2").arg(evento.codigo).arg(evento.nombre));
            childNombre->setCheckable(true);
            childNombre->setCheckState(evento.activa ? Qt::Checked : Qt::Unchecked);
            childNombre->setEditable(false);
            
            // Guardar IDs en el item para uso posterior
            childNombre->setData(evento.eventoId, Qt::UserRole);
            childNombre->setData(evento.defensaId, Qt::UserRole + 1);
            
            // Tooltip para el evento (doble de largo ~1350px, fuente pequeña, interlineado simple, tabla centrada)
            QString tooltipEvento = QString(
                "<table style='width: 1350px; font-size: 9pt;'><tr><td>"
                "<p style='margin: 0 0 6px 0; line-height: 1.2;'><b>[%1] %2</b></p>"
                "<p style='margin: 0 0 10px 0; line-height: 1.2;'>%3</p>"
                "</td></tr><tr><td align='center'>"
                "<table style='border-collapse: collapse; font-size: 9pt;'>"
                "<tr style='background-color: #f5f5f5;'>"
                "  <td style='padding: 3px 8px; font-weight: bold;'>Parámetro</td>"
                "  <td style='padding: 3px 8px; text-align: center; font-weight: bold;'>Base</td>"
                "  <td style='padding: 3px 8px; text-align: center; font-weight: bold;'></td>"
                "  <td style='padding: 3px 8px; text-align: center; font-weight: bold;'>Calculado</td>"
                "</tr>"
                "<tr>"
                "  <td style='padding: 3px 8px;'>Frecuencia</td>"
                "  <td style='padding: 3px 8px; text-align: center;'>%4</td>"
                "  <td style='padding: 3px 8px; text-align: center;'>→</td>"
                "  <td style='padding: 3px 8px; text-align: center;'>%5</td>"
                "</tr>"
                "<tr style='background-color: #f5f5f5;'>"
                "  <td style='padding: 3px 8px;'>Consecuencia</td>"
                "  <td style='padding: 3px 8px; text-align: center;'>%6</td>"
                "  <td style='padding: 3px 8px; text-align: center;'>→</td>"
                "  <td style='padding: 3px 8px; text-align: center;'>%7</td>"
                "</tr>"
                "<tr>"
                "  <td style='padding: 3px 8px; font-weight: bold;'>Riesgo</td>"
                "  <td style='padding: 3px 8px; text-align: center; font-weight: bold;'>%8</td>"
                "  <td style='padding: 3px 8px; text-align: center;'>→</td>"
                "  <td style='padding: 3px 8px; text-align: center; font-weight: bold;'>%9</td>"
                "</tr>"
                "</table>"
                "</td></tr></table>")
                .arg(evento.codigo)
                .arg(evento.nombre)
                .arg(evento.descripcion.isEmpty() ? "Sin descripción" : evento.descripcion)
                .arg(obtenerTextoFrecuencia(evento.frecuenciaBase))
                .arg(obtenerTextoFrecuencia(evento.frecuencia))
                .arg(obtenerTextoConsecuencia(evento.consecuenciaBase))
                .arg(obtenerTextoConsecuencia(evento.consecuencia))
                .arg(obtenerTextoRiesgo(evento.riesgoBase))
                .arg(obtenerTextoRiesgo(evento.riesgo));
            childNombre->setToolTip(tooltipEvento);
            childRow.append(childNombre);
            
            QStandardItem *childRobustez = new QStandardItem("");
            childRobustez->setEditable(false);
            childRobustez->setToolTip(tooltipEvento);
            childRow.append(childRobustez);
            
            QStandardItem *childRiesgoBase = new QStandardItem(obtenerTextoRiesgo(evento.riesgoBase));
            childRiesgoBase->setEditable(false);
            childRiesgoBase->setToolTip(tooltipEvento);
            childRiesgoBase->setTextAlignment(Qt::AlignCenter);
            childRiesgoBase->setData(evento.riesgoBase, Qt::UserRole); // Para ordenar numéricamente
            childRiesgoBase->setBackground(QBrush(obtenerColorRiesgo(evento.riesgoBase)));
            childRow.append(childRiesgoBase);
            
            QStandardItem *childRiesgoCalc = new QStandardItem(obtenerTextoRiesgo(evento.riesgo));
            childRiesgoCalc->setEditable(false);
            childRiesgoCalc->setToolTip(tooltipEvento);
            childRiesgoCalc->setTextAlignment(Qt::AlignCenter);
            childRiesgoCalc->setData(evento.riesgo, Qt::UserRole); // Para ordenar numéricamente
            childRiesgoCalc->setBackground(QBrush(obtenerColorRiesgo(evento.riesgo)));
            childRow.append(childRiesgoCalc);
            
            // Columnas vacías para número de sucesos en hijos
            QStandardItem *childNumTotal = new QStandardItem("");
            childNumTotal->setEditable(false);
            childNumTotal->setToolTip(tooltipEvento);
            childRow.append(childNumTotal);
            
            QStandardItem *childNumAplicable = new QStandardItem("");
            childNumAplicable->setEditable(false);
            childNumAplicable->setToolTip(tooltipEvento);
            childRow.append(childNumAplicable);
            
            itemNombre->appendRow(childRow);
        }
        
        model->appendRow(row);
    }
}

QList<EventoInfo> ListaBarreras::obtenerEventosPorDefensa(const QString &nombreDefensa, DefenseType tipo)
{
    QList<EventoInfo> eventos;
    
    // La columna 'frecuencia' y 'consecuencia' son los valores BASE
    // El 'riesgo' es el valor calculado después de aplicar defensas
    QString escapedNombre = nombreDefensa;
    escapedNombre.replace("'", "''"); // Escapar comillas simples
    
    QString query = "SELECT DISTINCT "
                    "si.id, "
                    "d.id, "
                    "si.codigo, "
                    "si.nombre, "
                    "si.descripcion, "
                    "si.frecuencia, "
                    "si.consecuencia, "
                    "si.riesgo, "
                    "d.activo, "
                    "si.error_humano "
                    "FROM defensa d "
                    "INNER JOIN suceso_iniciador si ON si.id = d.suceso_iniciador_id "
                    "WHERE d.nombre = '" + escapedNombre + "' AND d.tipo = " + QString::number(static_cast<int>(tipo)) + " "
                    "ORDER BY si.codigo";
    
    QSqlQuery qry;
    
    if (qry.exec(query)) {
        MatrizRiesgo matriz;
        while (qry.next()) {
            EventoInfo evento;
            evento.eventoId = qry.value(0).toInt();
            evento.defensaId = qry.value(1).toInt();
            evento.codigo = qry.value(2).toString();
            evento.nombre = qry.value(3).toString();
            evento.descripcion = qry.value(4).toString();
            evento.frecuenciaBase = qry.value(5).toInt();
            evento.consecuenciaBase = qry.value(6).toInt();
            evento.riesgo = qry.value(7).toInt();
            evento.activa = qry.value(8).toBool();
            bool errorHumano = qry.value(9).toBool();
            
            // Calcular riesgo base
            evento.riesgoBase = matriz.riesgoReferencia(evento.frecuenciaBase, evento.consecuenciaBase);
            
            // Calcular frecuencia y consecuencia con todas las defensas activas usando DefenseCalc
            // Obtener todas las defensas activas para este evento agrupadas por tipo y robustez
            QString queryDefensas = "SELECT tipo, robustez FROM defensa WHERE suceso_iniciador_id = " + QString::number(evento.eventoId) + " AND activo = 1";
            QSqlQuery qryDef;
            
            int reductoresFrecuencia[4] = {0, 0, 0, 0};
            int barreras[4] = {0, 0, 0, 0};
            int reductoresConsecuencia[4] = {0, 0, 0, 0};
            
            if (qryDef.exec(queryDefensas)) {
                while (qryDef.next()) {
                    int tipoDefensa = qryDef.value(0).toInt();
                    int robustez = qryDef.value(1).toInt();
                    
                    if (robustez >= 0 && robustez <= 3) {
                        if (tipoDefensa == 0) { // Reductor de frecuencia
                            reductoresFrecuencia[robustez]++;
                        } else if (tipoDefensa == 1) { // Barrera
                            barreras[robustez]++;
                        } else if (tipoDefensa == 2) { // Reductor de consecuencia
                            reductoresConsecuencia[robustez]++;
                        }
                    }
                }
            }
            
            // Preparar datos para DefenseCalc
            std::vector<std::pair<int, int>> infoReductoresFrecuencia;
            for (int i = 0; i < 4; i++) {
                infoReductoresFrecuencia.push_back(std::make_pair(i, reductoresFrecuencia[i]));
            }
            
            // Calcular frecuencia usando DefenseCalc
            int reductorFrecuencia = DefenseCalc::getFrecuencyReducer(infoReductoresFrecuencia);
            evento.frecuencia = DefenseCalc::calcFrecuencyLevel(evento.frecuenciaBase, errorHumano, reductorFrecuencia);
            
            // Preparar datos para consecuencia
            std::vector<std::pair<int, int>> infoReductoresConsecuencia;
            for (int i = 0; i < 4; i++) {
                infoReductoresConsecuencia.push_back(std::make_pair(i, reductoresConsecuencia[i]));
            }
            
            // Calcular consecuencia usando DefenseCalc
            int reductorConsecuencia = DefenseCalc::getConsequencyReducer(infoReductoresConsecuencia);
            evento.consecuencia = DefenseCalc::calcConsequencyLevel(evento.consecuenciaBase, reductorConsecuencia);
            
            eventos.append(evento);
        }
    } else {
        qDebug() << "Error al obtener eventos por defensa:" << qry.lastError().text();
    }
    
    return eventos;
}

void ListaBarreras::aplicarFiltro()
{
    QString filtro = ui->edFiltro->text().toLower();
    
    // Aplicar filtro a Reductores de Frecuencia
    for (int i = 0; i < modelReductoresFrecuencia->rowCount(); ++i) {
        bool mostrar = filtro.isEmpty();
        if (!mostrar) {
            QString texto = modelReductoresFrecuencia->item(i, 0)->text().toLower();
            mostrar = texto.contains(filtro);
        }
        ui->treeReductoresFrecuencia->setRowHidden(i, QModelIndex(), !mostrar);
    }
    
    // Aplicar filtro a Barreras
    for (int i = 0; i < modelBarreras->rowCount(); ++i) {
        bool mostrar = filtro.isEmpty();
        if (!mostrar) {
            QString texto = modelBarreras->item(i, 0)->text().toLower();
            mostrar = texto.contains(filtro);
        }
        ui->treeBarreras->setRowHidden(i, QModelIndex(), !mostrar);
    }
    
    // Aplicar filtro a Reductores de Consecuencia
    for (int i = 0; i < modelReductoresConsecuencia->rowCount(); ++i) {
        bool mostrar = filtro.isEmpty();
        if (!mostrar) {
            QString texto = modelReductoresConsecuencia->item(i, 0)->text().toLower();
            mostrar = texto.contains(filtro);
        }
        ui->treeReductoresConsecuencia->setRowHidden(i, QModelIndex(), !mostrar);
    }
}



QString ListaBarreras::obtenerTextoRobustez(int hardiness)
{
    // Usar la nomenclatura estándar de MatrizRiesgo::Durezas
    if (MatrizRiesgo::Durezas.find(hardiness) != MatrizRiesgo::Durezas.end()) {
        return MatrizRiesgo::Durezas.at(hardiness);
    }
    return "Desconocida";
}

QString ListaBarreras::obtenerTextoTipo(DefenseType tipo)
{
    switch (tipo) {
        case DefenseType::FrecuencyReducer:
            return "Reductor de Frecuencia";
        case DefenseType::Barrier:
            return "Barrera";
        case DefenseType::ConsequenceReducer:
            return "Reductor de Consecuencia";
        default:
            return "Desconocido";
    }
}

QString ListaBarreras::obtenerTextoRiesgo(int riesgo)
{
    if (MatrizRiesgo::Riesgos.find(riesgo) != MatrizRiesgo::Riesgos.end()) {
        return MatrizRiesgo::Riesgos.at(riesgo);
    }
    return QString::number(riesgo);
}

QString ListaBarreras::obtenerTextoFrecuencia(int frecuencia)
{
    if (MatrizRiesgo::Frecuencias.find(frecuencia) != MatrizRiesgo::Frecuencias.end()) {
        return MatrizRiesgo::Frecuencias.at(frecuencia);
    }
    return QString::number(frecuencia);
}

QString ListaBarreras::obtenerTextoConsecuencia(int consecuencia)
{
    if (MatrizRiesgo::Consecuencias.find(consecuencia) != MatrizRiesgo::Consecuencias.end()) {
        return MatrizRiesgo::Consecuencias.at(consecuencia);
    }
    return QString::number(consecuencia);
}

QColor ListaBarreras::obtenerColorRiesgo(int riesgo)
{
    switch (riesgo) {
        case 0:
            return Styles::WhiteBackgroundColor;
        case 1:
            return Styles::GreenBackgroundColor;
        case 2:
            return Styles::YellowBackgroundColor;
        case 3:
            return Styles::RedBackgroundColor;
        default:
            return Qt::white;
    }
}

void ListaBarreras::toggleDefenseForEvent(int defensaId, int eventoId, bool activar)
{
    QString query = "UPDATE defensa SET activo = " + QString::number(activar ? 1 : 0) + " WHERE id = " + QString::number(defensaId);
    QSqlQuery qry;
    
    if (!qry.exec(query)) {
        qDebug() << "Error al actualizar defensa:" << qry.lastError().text();
    }
}

void ListaBarreras::recalcularYActualizarRiesgo(QStandardItem *childItem, int eventoId)
{
    Q_UNUSED(childItem); // No longer needed since we refresh the entire window
    
    // Primero obtener datos base del evento
    QString queryEvento = "SELECT frecuencia, consecuencia, error_humano FROM suceso_iniciador WHERE id = " + QString::number(eventoId);
    QSqlQuery qryEvento;
    
    if (!qryEvento.exec(queryEvento) || !qryEvento.next()) {
        qDebug() << "Error al obtener evento:" << qryEvento.lastError().text();
        return;
    }
    
    int frecuenciaBase = qryEvento.value(0).toInt();
    int consecuenciaBase = qryEvento.value(1).toInt();
    bool errorHumano = qryEvento.value(2).toBool();
    qryEvento.finish(); // Liberar recursos del query
    
    // Ahora obtener todas las defensas del evento agrupadas por tipo y robustez
    QString query = "SELECT d.tipo, d.robustez FROM defensa d WHERE d.suceso_iniciador_id = " + QString::number(eventoId) + " AND d.activo = 1 ORDER BY d.tipo";
    QSqlQuery qry;
    
    if (!qry.exec(query)) {
        qDebug() << "Error al obtener defensas:" << qry.lastError().text();
        return;
    }
    
    // Contar defensas por tipo y robustez (igual que CompleteEventDefinition)
    int reductoresFrecuencia[4] = {0, 0, 0, 0}; // hardiness 0-3
    int barreras[4] = {0, 0, 0, 0};
    int reductoresConsecuencia[4] = {0, 0, 0, 0};
    
    while (qry.next()) {
        int tipo = qry.value(0).toInt();
        int robustez = qry.value(1).toInt();
        
        if (robustez >= 0 && robustez <= 3) {
            if (tipo == 0) { // Reductor de frecuencia
                reductoresFrecuencia[robustez]++;
            } else if (tipo == 1) { // Barrera
                barreras[robustez]++;
            } else if (tipo == 2) { // Reductor de consecuencia
                reductoresConsecuencia[robustez]++;
            }
        }
    }
    qry.finish(); // Liberar recursos del query
    
    // Preparar datos para DefenseCalc (igual que en CompleteEventDefinition::calcularReductoresFrecuencia)
    std::vector<std::pair<int, int>> infoReductoresFrecuencia;
    for (int i = 0; i < 4; i++) {
        infoReductoresFrecuencia.push_back(std::make_pair(i, reductoresFrecuencia[i]));
    }
    
    std::vector<std::pair<int, int>> infoBarreras;
    for (int i = 0; i < 4; i++) {
        infoBarreras.push_back(std::make_pair(i, barreras[i]));
    }
    
    std::vector<std::pair<int, int>> infoReductoresConsecuencia;
    for (int i = 0; i < 4; i++) {
        infoReductoresConsecuencia.push_back(std::make_pair(i, reductoresConsecuencia[i]));
    }
    
    // Calcular usando DefenseCalc (igual que CompleteEventDefinition)
    int reductorFrecuencia = DefenseCalc::getFrecuencyReducer(infoReductoresFrecuencia);
    int frecuenciaCalc = DefenseCalc::calcFrecuencyLevel(frecuenciaBase, errorHumano, reductorFrecuencia);
    
    int reductorBarreras = DefenseCalc::getBarriers(infoBarreras);
    int probabilidadCalc = DefenseCalc::calcProbabilityLevel(reductorBarreras);
    
    int reductorConsecuencia = DefenseCalc::getConsequencyReducer(infoReductoresConsecuencia);
    int consecuenciaCalc = DefenseCalc::calcConsequencyLevel(consecuenciaBase, reductorConsecuencia);
    
    // Calcular riesgo con MatrizRiesgo usando los tres valores calculados (igual que CompleteEventDefinition::calcularRiesgo)
    MatrizRiesgo matriz;
    int riesgoCalc = matriz.riesgo(
        static_cast<std::size_t>(probabilidadCalc),
        static_cast<std::size_t>(frecuenciaCalc),
        static_cast<std::size_t>(consecuenciaCalc)
    );
    
    qDebug() << "Recálculo para evento" << eventoId << ": Frec" << frecuenciaBase << "->" << frecuenciaCalc 
             << "Cons" << consecuenciaBase << "->" << consecuenciaCalc 
             << "Prob" << probabilidadCalc << "-> Riesgo" << riesgoCalc;
    
    // Actualizar en la base de datos
    QString updateQuery = "UPDATE suceso_iniciador SET riesgo = " + QString::number(riesgoCalc) + " WHERE id = " + QString::number(eventoId);
    QSqlQuery qryUpdate;
    
    if (!qryUpdate.exec(updateQuery)) {
        qDebug() << "Error al actualizar riesgo:" << qryUpdate.lastError().text();
        return;
    }
    
    // Actualizar el riesgo en la vista sin refrescar toda la ventana
    actualizarRiesgoEnVista(eventoId, riesgoCalc);
}

void ListaBarreras::actualizarRiesgoEnVista(int eventoId, int nuevoRiesgo)
{
    // Actualizar el riesgo en todos los modelos donde aparezca este evento
    actualizarRiesgoEnModelo(modelReductoresFrecuencia, eventoId, nuevoRiesgo);
    actualizarRiesgoEnModelo(modelBarreras, eventoId, nuevoRiesgo);
    actualizarRiesgoEnModelo(modelReductoresConsecuencia, eventoId, nuevoRiesgo);
}

void ListaBarreras::actualizarRiesgoEnModelo(QStandardItemModel *model, int eventoId, int nuevoRiesgo)
{
    // Recorrer todos los items padre (defensas)
    for (int i = 0; i < model->rowCount(); ++i) {
        QStandardItem *parentItem = model->item(i, 0);
        if (!parentItem) continue;
        
        // Recorrer todos los items hijos (eventos)
        for (int j = 0; j < parentItem->rowCount(); ++j) {
            QStandardItem *childItem = parentItem->child(j, 0);
            if (!childItem) continue;
            
            // Verificar si este es el evento que buscamos
            int itemEventoId = childItem->data(Qt::UserRole).toInt();
            if (itemEventoId == eventoId) {
                // Actualizar el riesgo calculado en la columna 3
                QStandardItem *riesgoItem = parentItem->child(j, 3);
                if (riesgoItem) {
                    riesgoItem->setText(obtenerTextoRiesgo(nuevoRiesgo));
                    riesgoItem->setData(nuevoRiesgo, Qt::UserRole); // Para ordenar numéricamente
                    riesgoItem->setBackground(QBrush(obtenerColorRiesgo(nuevoRiesgo)));
                }
            }
        }
    }
}

void ListaBarreras::actualizarRobustezEnVista(const QString &nombreDefensa, int tipoDefensa, int nuevaRobustez)
{
    // Determinar qué modelo actualizar según el tipo
    QStandardItemModel *model = nullptr;
    if (tipoDefensa == 0) { // Reductor de frecuencia
        model = modelReductoresFrecuencia;
    } else if (tipoDefensa == 1) { // Barrera
        model = modelBarreras;
    } else if (tipoDefensa == 2) { // Reductor de consecuencia
        model = modelReductoresConsecuencia;
    }
    
    if (model) {
        actualizarRobustezEnModelo(model, nombreDefensa, tipoDefensa, nuevaRobustez);
    }
}

void ListaBarreras::actualizarRobustezEnModelo(QStandardItemModel *model, const QString &nombreDefensa, int tipoDefensa, int nuevaRobustez)
{
    // Recorrer todos los items padre (defensas)
    for (int i = 0; i < model->rowCount(); ++i) {
        QStandardItem *robustezItem = model->item(i, 1); // Columna 1 es robustez
        if (!robustezItem) continue;
        
        // Verificar si esta es la defensa que buscamos
        QString itemNombreDefensa = robustezItem->data(Qt::UserRole + 1).toString();
        int itemTipoDefensa = robustezItem->data(Qt::UserRole + 2).toInt();
        
        if (itemNombreDefensa == nombreDefensa && itemTipoDefensa == tipoDefensa) {
            // Actualizar el texto y el dato
            robustezItem->setText(obtenerTextoRobustez(nuevaRobustez));
            robustezItem->setData(nuevaRobustez, Qt::UserRole);
        }
    }
}

void ListaBarreras::actualizarContadorSucesosEnVista(const QString &nombreDefensa, int tipoDefensa)
{
    // Determinar qué modelo actualizar según el tipo
    QStandardItemModel *model = nullptr;
    if (tipoDefensa == 0) { // Reductor de frecuencia
        model = modelReductoresFrecuencia;
    } else if (tipoDefensa == 1) { // Barrera
        model = modelBarreras;
    } else if (tipoDefensa == 2) { // Reductor de consecuencia
        model = modelReductoresConsecuencia;
    }
    
    if (!model) return;
    
    // Recorrer todos los items padre (defensas)
    for (int i = 0; i < model->rowCount(); ++i) {
        QStandardItem *robustezItem = model->item(i, 1); // Columna 1 es robustez
        if (!robustezItem) continue;
        
        // Verificar si esta es la defensa que buscamos
        QString itemNombreDefensa = robustezItem->data(Qt::UserRole + 1).toString();
        int itemTipoDefensa = robustezItem->data(Qt::UserRole + 2).toInt();
        
        if (itemNombreDefensa == nombreDefensa && itemTipoDefensa == tipoDefensa) {
            // Obtener el item padre (nombre) para contar sus hijos
            QStandardItem *nombreItem = model->item(i, 0);
            if (!nombreItem) continue;
            
            // Contar sucesos totales y activos
            int totalSucesos = nombreItem->rowCount();
            int sucesosActivos = 0;
            
            for (int j = 0; j < nombreItem->rowCount(); ++j) {
                QStandardItem *childItem = nombreItem->child(j, 0);
                if (childItem && childItem->checkState() == Qt::Checked) {
                    sucesosActivos++;
                }
            }
            
            // Actualizar el contador total en columna 4
            QStandardItem *contadorTotalItem = model->item(i, 4);
            if (contadorTotalItem) {
                contadorTotalItem->setText(QString::number(totalSucesos));
                contadorTotalItem->setData(totalSucesos, Qt::UserRole);
            }
            
            // Actualizar el contador de aplicables en columna 5
            QStandardItem *contadorAplicableItem = model->item(i, 5);
            if (contadorAplicableItem) {
                contadorAplicableItem->setText(QString::number(sucesosActivos));
                contadorAplicableItem->setData(sucesosActivos, Qt::UserRole);
            }
        }
    }
}

void ListaBarreras::actualizarCheckboxesEnVista(const QString &nombreDefensa, int tipoDefensa, bool activar)
{
    // Determinar qué modelo actualizar según el tipo
    QStandardItemModel *model = nullptr;
    if (tipoDefensa == 0) { // Reductor de frecuencia
        model = modelReductoresFrecuencia;
    } else if (tipoDefensa == 1) { // Barrera
        model = modelBarreras;
    } else if (tipoDefensa == 2) { // Reductor de consecuencia
        model = modelReductoresConsecuencia;
    }
    
    if (!model) return;
    
    // Recorrer todos los items padre (defensas)
    for (int i = 0; i < model->rowCount(); ++i) {
        QStandardItem *robustezItem = model->item(i, 1); // Columna 1 es robustez
        if (!robustezItem) continue;
        
        // Verificar si esta es la defensa que buscamos
        QString itemNombreDefensa = robustezItem->data(Qt::UserRole + 1).toString();
        int itemTipoDefensa = robustezItem->data(Qt::UserRole + 2).toInt();
        
        if (itemNombreDefensa == nombreDefensa && itemTipoDefensa == tipoDefensa) {
            // Obtener el item padre (nombre)
            QStandardItem *nombreItem = model->item(i, 0);
            if (!nombreItem) continue;
            
            // Actualizar checkboxes de todos los hijos
            for (int j = 0; j < nombreItem->rowCount(); ++j) {
                QStandardItem *childItem = nombreItem->child(j, 0);
                if (childItem && childItem->isCheckable()) {
                    // Bloquear señales para evitar recálculos múltiples
                    model->blockSignals(true);
                    childItem->setCheckState(activar ? Qt::Checked : Qt::Unchecked);
                    model->blockSignals(false);
                }
            }
        }
    }
}

void ListaBarreras::onItemChanged(QStandardItem *item)
{
    // Manejar checkboxes (iniciadores)
    if (item->isCheckable()) {
        // Obtener IDs guardados en el item
        int eventoId = item->data(Qt::UserRole).toInt();
        int defensaId = item->data(Qt::UserRole + 1).toInt();
        
        if (eventoId > 0 && defensaId > 0) {
            bool activar = (item->checkState() == Qt::Checked);
            
            // Actualizar en la base de datos
            toggleDefenseForEvent(defensaId, eventoId, activar);
            
            // Recalcular el riesgo y actualizar en la vista
            recalcularYActualizarRiesgo(item, eventoId);
            
            // Notificar que hubo cambios
            emit defensesChanged();
        }
        return;
    }
    
    // Manejar cambios en robustez (columna 1 de defensas)
    if (item->column() == 1 && item->parent() == nullptr) {
        // Es una celda de robustez de defensa padre
        QString nuevoTexto = item->text();
        QString nombreDefensa = item->data(Qt::UserRole + 1).toString();
        int tipoDefensa = item->data(Qt::UserRole + 2).toInt();
        
        // Convertir texto a valor numérico de robustez
        int nuevaRobustez = -1;
        for (const auto& pair : MatrizRiesgo::Durezas) {
            if (pair.second == nuevoTexto) {
                nuevaRobustez = pair.first;
                break;
            }
        }
        
        if (nuevaRobustez >= -1 && nuevaRobustez <= 3 && !nombreDefensa.isEmpty()) {
            // Actualizar robustez en la base de datos para todas las defensas con ese nombre y tipo
            QString escapedNombre = nombreDefensa;
            escapedNombre.replace("'", "''"); // Escapar comillas simples
            QString query = "UPDATE defensa SET robustez = " + QString::number(nuevaRobustez) + " WHERE nombre = '" + escapedNombre + "' AND tipo = " + QString::number(tipoDefensa);
            QSqlQuery qry;
            
            if (qry.exec(query)) {
                // Actualizar la robustez en todas las instancias visuales de esta defensa
                actualizarRobustezEnVista(nombreDefensa, tipoDefensa, nuevaRobustez);
                
                // Obtener todos los eventos afectados y recalcular sus riesgos
                QString queryEventos = "SELECT DISTINCT suceso_iniciador_id FROM defensa WHERE nombre = '" + escapedNombre + "' AND tipo = " + QString::number(tipoDefensa);
                QSqlQuery qryEventos;
                
                if (qryEventos.exec(queryEventos)) {
                    // Recalcular riesgo para cada evento afectado y actualizar en la vista
                    while (qryEventos.next()) {
                        int eventoId = qryEventos.value(0).toInt();
                        if (eventoId > 0) {
                            recalcularYActualizarRiesgo(nullptr, eventoId);
                        }
                    }
                }
                
                // Notificar que hubo cambios
                emit defensesChanged();
            } else {
                qDebug() << "Error al actualizar robustez:" << qry.lastError().text();
                // Revertir el cambio en la UI
                item->setText(obtenerTextoRobustez(item->data(Qt::UserRole).toInt()));
            }
        } else {
            // Valor inválido, revertir
            item->setText(obtenerTextoRobustez(item->data(Qt::UserRole).toInt()));
        }
    }
}

void ListaBarreras::on_btCerrar_clicked()
{
    accept();
}

void ListaBarreras::onItemDoubleClicked(const QModelIndex &index)
{
    // Solo permitir edición en items padre (defensas), columna 0
    if (!index.isValid() || index.parent().isValid()) {
        return; // Es un hijo (evento), no permitir edición
    }
    
    if (index.column() == 0) {
        // Es la columna de nombre/código de una defensa - permitir editar nombre y descripción
        QStandardItem *item = nullptr;
        QStandardItemModel *model = nullptr;
        NumericSortProxyModel *proxy = nullptr;
        QModelIndex sourceIndex;
        
        // Determinar de qué modelo es el index por el sender
        QTreeView *treeView = qobject_cast<QTreeView*>(sender());
        if (!treeView) return;
        
        if (treeView == ui->treeReductoresFrecuencia) {
            model = modelReductoresFrecuencia;
            proxy = proxyReductoresFrecuencia;
        } else if (treeView == ui->treeBarreras) {
            model = modelBarreras;
            proxy = proxyBarreras;
        } else if (treeView == ui->treeReductoresConsecuencia) {
            model = modelReductoresConsecuencia;
            proxy = proxyReductoresConsecuencia;
        }
        
        if (!model || !proxy) return;
        
        // Mapear el índice del proxy al modelo fuente
        sourceIndex = proxy->mapToSource(index);
        item = model->itemFromIndex(sourceIndex);
        if (!item) return;
        
        // Obtener robustez item para extraer datos
        QStandardItem *robustezItem = model->item(item->row(), 1);
        if (!robustezItem) return;
        
        QString nombreDefensa = robustezItem->data(Qt::UserRole + 1).toString();
        int tipoDefensa = robustezItem->data(Qt::UserRole + 2).toInt();
        
        // Obtener nombre y descripción actuales de la base de datos
        QString escapedNombre = nombreDefensa;
        escapedNombre.replace("'", "''");
        QString query = QString("SELECT nombre, descripcion FROM defensa WHERE nombre = '%1' AND tipo = %2 LIMIT 1")
            .arg(escapedNombre)
            .arg(tipoDefensa);
        
        QSqlQuery qry;
        QString nombreActual = nombreDefensa;
        QString descripcionActual;
        if (qry.exec(query) && qry.next()) {
            nombreActual = qry.value(0).toString();
            descripcionActual = qry.value(1).toString();
        }
        
        // Crear un diálogo personalizado para editar nombre y descripción
        QDialog dialogo(this);
        dialogo.setWindowTitle("Editar Defensa");
        dialogo.resize(594, 240); // 66% de 900 = 594, 40% de 600 = 240
        
        QVBoxLayout *layout = new QVBoxLayout(&dialogo);
        
        // Campo para el nombre
        QLabel *labelNombre = new QLabel("Nombre:", &dialogo);
        QTextEdit *editNombre = new QTextEdit(&dialogo);
        editNombre->setPlainText(nombreActual);
        editNombre->setMaximumHeight(60); // Permitir 2 líneas
        // Posicionar cursor al principio
        QTextCursor cursor = editNombre->textCursor();
        cursor.movePosition(QTextCursor::Start);
        editNombre->setTextCursor(cursor);
        layout->addWidget(labelNombre);
        layout->addWidget(editNombre);
        
        // Campo para la descripción
        QLabel *labelDescripcion = new QLabel("Descripción:", &dialogo);
        QTextEdit *editDescripcion = new QTextEdit(&dialogo);
        editDescripcion->setPlainText(descripcionActual);
        layout->addWidget(labelDescripcion);
        layout->addWidget(editDescripcion);
        
        // Botones
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialogo);
        connect(buttonBox, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialogo, &QDialog::reject);
        layout->addWidget(buttonBox);
        
        if (dialogo.exec() == QDialog::Accepted) {
            QString nuevoNombre = editNombre->toPlainText().trimmed();
            QString nuevaDescripcion = editDescripcion->toPlainText();
            
            if (nuevoNombre.isEmpty()) {
                QMessageBox::warning(this, "Error", "El nombre no puede estar vacío.");
                return;
            }
            
            // Actualizar en la base de datos
            QString escapedNuevoNombre = nuevoNombre;
            escapedNuevoNombre.replace("'", "''");
            QString escapedDescripcion = nuevaDescripcion;
            escapedDescripcion.replace("'", "''");
            
            QString updateQuery = QString("UPDATE defensa SET nombre = '%1', descripcion = '%2' WHERE nombre = '%3' AND tipo = %4")
                .arg(escapedNuevoNombre)
                .arg(escapedDescripcion)
                .arg(escapedNombre)
                .arg(tipoDefensa);
            
            QSqlQuery qryUpdate;
            if (qryUpdate.exec(updateQuery)) {
                // Actualizar solo el texto del item sin recargar toda la vista
                // Obtener el código de la defensa desde la DB
                QString queryCode = QString("SELECT codigo FROM defensa WHERE nombre = '%1' AND tipo = %2 LIMIT 1")
                    .arg(escapedNuevoNombre)
                    .arg(tipoDefensa);
                QSqlQuery qryCode;
                if (qryCode.exec(queryCode) && qryCode.next()) {
                    QString codigo = qryCode.value(0).toString();
                    item->setText(QString("[%1] %2").arg(codigo).arg(nuevoNombre));
                }
                
                // Actualizar el dato almacenado en robustez item
                robustezItem->setData(nuevoNombre, Qt::UserRole + 1);
            } else {
                QMessageBox::warning(this, "Error", "No se pudo actualizar la defensa: " + qryUpdate.lastError().text());
            }
        }
    }
}

void ListaBarreras::showContextMenu(const QPoint &pos)
{
    // Determinar qué tree view generó el evento
    QTreeView *treeView = qobject_cast<QTreeView*>(sender());
    if (!treeView) return;
    
    QModelIndex index = treeView->indexAt(pos);
    if (!index.isValid() || index.parent().isValid()) {
        return; // Solo mostrar menú para items padre (defensas)
    }
    
    QStandardItemModel *model = nullptr;
    NumericSortProxyModel *proxy = nullptr;
    
    if (treeView == ui->treeReductoresFrecuencia) {
        model = modelReductoresFrecuencia;
        proxy = proxyReductoresFrecuencia;
    } else if (treeView == ui->treeBarreras) {
        model = modelBarreras;
        proxy = proxyBarreras;
    } else if (treeView == ui->treeReductoresConsecuencia) {
        model = modelReductoresConsecuencia;
        proxy = proxyReductoresConsecuencia;
    }
    
    if (!model || !proxy) return;
    
    // Mapear el índice del proxy al modelo fuente
    QModelIndex sourceIndex = proxy->mapToSource(index);
    QStandardItem *item = model->itemFromIndex(sourceIndex);
    if (!item) return;
    
    // Obtener robustez item para extraer datos
    QStandardItem *robustezItem = model->item(item->row(), 1);
    if (!robustezItem) return;
    
    QString nombreDefensa = robustezItem->data(Qt::UserRole + 1).toString();
    int tipoDefensa = robustezItem->data(Qt::UserRole + 2).toInt();
    
    // Crear menú contextual
    QMenu contextMenu(this);
    QAction *activarAction = contextMenu.addAction("Activar en todos los sucesos");
    QAction *desactivarAction = contextMenu.addAction("Desactivar en todos los sucesos");
    
    // Añadir separador y nueva opción
    contextMenu.addSeparator();
    QAction *addToEventsAction = contextMenu.addAction("Añadir a nuevos sucesos");
    
    QAction *selectedAction = contextMenu.exec(treeView->viewport()->mapToGlobal(pos));
    
    if (selectedAction == activarAction) {
        activarDefensaEnTodosEventos(nombreDefensa, tipoDefensa);
    } else if (selectedAction == desactivarAction) {
        desactivarDefensaEnTodosEventos(nombreDefensa, tipoDefensa);
    } else if (selectedAction == addToEventsAction) {
        // Crear DefenseDefinition a partir de los datos únicos de la defensa
        // Primero obtener los datos completos de la defensa
        QString queryDef = QString("SELECT d.codigo, d.descripcion, d.robustez, d.etapa_orden_id "
                                   "FROM defensa d WHERE d.nombre = '%1' AND d.tipo = %2 LIMIT 1")
            .arg(nombreDefensa.replace("'", "''"))
            .arg(tipoDefensa);
        
        QSqlQuery qryDef;
        if (qryDef.exec(queryDef) && qryDef.next()) {
            DefenseDefinition defense;
            defense.nombre = nombreDefensa;
            defense.code = qryDef.value(0).toString();
            defense.descripcion = qryDef.value(1).toString();
            defense.hardiness = qryDef.value(2).toInt();
            defense.type = static_cast<DefenseType>(tipoDefensa);
            defense.orderStageId = qryDef.value(3).toInt();
            defense.event = -1; // No specific event
            defense.active = true; // Default
            
            // Abrir diálogo para seleccionar sucesos
            DlgAddDefenseToEvents dlg(defense, this);
            if (dlg.exec() == QDialog::Accepted) {
                QList<int> selectedEventIds = dlg.getSelectedEventIds();
                bool shouldApply = dlg.shouldApplyDefense();
                
                // Añadir la defensa a cada suceso seleccionado
                for (int eventId : selectedEventIds) {
                    DefenseDefinition newDefense;
                    newDefense.nombre = defense.nombre;
                    newDefense.descripcion = defense.descripcion;
                    newDefense.hardiness = defense.hardiness;
                    newDefense.orderStageId = defense.orderStageId;
                    newDefense.code = defense.code;
                    newDefense.type = defense.type;
                    newDefense.event = eventId;
                    newDefense.active = shouldApply;
                    
                    // Guardar en base de datos
                    Database::DbDefense dbDefense;
                    dbDefense.create(newDefense);
                    
                    // Recalcular riesgo del suceso
                    recalcularYActualizarRiesgo(nullptr, eventId);
                }
                
                // Recargar la ventana para mostrar los cambios
                cargarDefensas();
                
                QMessageBox::information(this, "Éxito", 
                    QString("La defensa se ha añadido a %1 suceso(s).").arg(selectedEventIds.count()));
            }
        }
    }
}

void ListaBarreras::activarDefensaEnTodosEventos(const QString &nombreDefensa, int tipoDefensa)
{
    aplicarCambioDefensaEnTodosEventos(nombreDefensa, tipoDefensa, true);
}

void ListaBarreras::desactivarDefensaEnTodosEventos(const QString &nombreDefensa, int tipoDefensa)
{
    aplicarCambioDefensaEnTodosEventos(nombreDefensa, tipoDefensa, false);
}

void ListaBarreras::aplicarCambioDefensaEnTodosEventos(const QString &nombreDefensa, int tipoDefensa, bool activar)
{
    QString accion = activar ? "activar" : "desactivar";
    QString accionCapital = activar ? "Activar" : "Desactivar";
    
    // Confirmar acción
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(QString("Confirmar %1").arg(accion));
    msgBox.setText(QString("¿Está seguro de que desea %1 la defensa '%2' en todos los sucesos donde aparece?").arg(accion).arg(nombreDefensa));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Sí");
    msgBox.setButtonText(QMessageBox::No, "No");
    msgBox.setDefaultButton(QMessageBox::No);
    
    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }
    
    // Obtener riesgos actuales antes de hacer cambios
    QString escapedNombre = nombreDefensa;
    escapedNombre.replace("'", "''");
    QString queryEventos = QString("SELECT DISTINCT si.id, si.riesgo FROM defensa d "
                                   "INNER JOIN suceso_iniciador si ON d.suceso_iniciador_id = si.id "
                                   "WHERE d.nombre = '%1' AND d.tipo = %2")
        .arg(escapedNombre)
        .arg(tipoDefensa);
    
    QSqlQuery qryEventos;
    QMap<int, int> riesgosAnteriores; // eventoId -> riesgo anterior
    
    if (qryEventos.exec(queryEventos)) {
        while (qryEventos.next()) {
            int eventoId = qryEventos.value(0).toInt();
            int riesgoActual = qryEventos.value(1).toInt();
            riesgosAnteriores[eventoId] = riesgoActual;
        }
    }
    
    // Activar/Desactivar en la base de datos
    QString query = QString("UPDATE defensa SET activo = %1 WHERE nombre = '%2' AND tipo = %3")
        .arg(activar ? 1 : 0)
        .arg(escapedNombre)
        .arg(tipoDefensa);
    
    QSqlQuery qry;
    if (qry.exec(query)) {
        // Recalcular riesgos y recopilar cambios
        QMap<QString, int> cambiosRiesgo; // "R1->R2" -> count
        
        for (auto it = riesgosAnteriores.begin(); it != riesgosAnteriores.end(); ++it) {
            int eventoId = it.key();
            int riesgoAnterior = it.value();
            
            // Recalcular el riesgo (esto actualiza la BD)
            recalcularYActualizarRiesgo(nullptr, eventoId);
            
            // Obtener el nuevo riesgo de la BD
            QString queryNuevoRiesgo = QString("SELECT riesgo FROM suceso_iniciador WHERE id = %1").arg(eventoId);
            QSqlQuery qryNuevo;
            int riesgoNuevo = riesgoAnterior;
            
            if (qryNuevo.exec(queryNuevoRiesgo) && qryNuevo.next()) {
                riesgoNuevo = qryNuevo.value(0).toInt();
            }
            
            // Registrar cambio si hubo diferencia
            if (riesgoAnterior != riesgoNuevo) {
                QString cambio = QString("%1 → %2")
                    .arg(obtenerTextoRiesgo(riesgoAnterior))
                    .arg(obtenerTextoRiesgo(riesgoNuevo));
                cambiosRiesgo[cambio] = cambiosRiesgo.value(cambio, 0) + 1;
            }
        }
        
        // Actualizar la vista sin recargar (preservar estado)
        // Primero actualizar checkboxes de todos los eventos afectados en la vista
        actualizarCheckboxesEnVista(nombreDefensa, tipoDefensa, activar);
        
        // Luego actualizar el número de sucesos activos en la columna (después de los checkboxes)
        actualizarContadorSucesosEnVista(nombreDefensa, tipoDefensa);
        
        // Notificar cambios
        emit defensesChanged();
        
        // Mostrar resumen de cambios
        QString mensaje;
        if (cambiosRiesgo.isEmpty()) {
            mensaje = "No ha habido cambios en el riesgo de ningún suceso.";
        } else {
            QStringList lineas;
            for (auto it = cambiosRiesgo.begin(); it != cambiosRiesgo.end(); ++it) {
                lineas << QString("%1 suceso(s) cambió/cambiaron de %2").arg(it.value()).arg(it.key());
            }
            mensaje = lineas.join("\n");
        }
        
        // Crear un QMessageBox personalizado con 10% más de ancho
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Completado");
        msgBox.setText(mensaje);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        
        // Ajustar el ancho del mensaje box (10% más ancho que el predeterminado)
        // El ancho predeterminado es aproximadamente 400px, así que usamos un 10% más
        constexpr int DEFAULT_MSGBOX_WIDTH = 400;
        int increasedWidth = int(DEFAULT_MSGBOX_WIDTH * 1.1); // 10% más ancho
        QSpacerItem* horizontalSpacer = new QSpacerItem(increasedWidth, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout* layout = (QGridLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        
        msgBox.exec();
    } else {
        QMessageBox::warning(this, "Error", QString("No se pudo %1 la defensa: %2").arg(accion).arg(qry.lastError().text()));
    }
}

void ListaBarreras::on_edFiltro_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    aplicarFiltro();
}
