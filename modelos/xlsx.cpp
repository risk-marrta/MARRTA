#include "xlsx.h"

#include <QCoreApplication>
#include <QProgressDialog>

#include <climits>

#include "matrizriesgo.h"

using namespace QXlsx;

namespace Modelos {

const int xlsx::VERSION = 1;

xlsx::xlsx()
{

}

bool xlsx::createTables(QString path, QString title, std::unordered_map<std::string, std::vector<CompleteEventDefinition> > &events)
{
    QXlsx::Document xlsx;
    QString srow;
    QXlsx::Format formatHeader;
    formatHeader.setFontBold(true);
    formatHeader.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    // Detalles del documento
    xlsx.setDocumentProperty("creator", "Proyecto MARRTA");
    xlsx.setDocumentProperty("title", title);
    xlsx.setDocumentProperty("subject", "Análisis de Riesgo");

    // Escritura de la pestaña etapas
    xlsx.addSheet("Etapas");
    xlsx.selectSheet("Etapas");
    xlsx.write("A1", "Id", formatHeader);
    xlsx.write("B1", "Nombre", formatHeader);
    xlsx.write("C1", "Descripción", formatHeader);
    xlsx.write("D1", "Orden", formatHeader);
    xlsx.write("E1", "Código", formatHeader);

    auto stages = Database::DbElementStage();
    QList<EtapaDefinition> etapasDb;
    try {
        stages.list(etapasDb, true);
    } catch(QString msg) {
        QMessageBox msgbox;
        msgbox.setText(msg);
        msgbox.exec();
        return false;
    }

    int row = 2;
    for (auto stage : etapasDb) {
        srow = QString::number(row);
        xlsx.write("A" + srow, stage.id);
        xlsx.write("B" + srow, stage.nombre);
        xlsx.write("C" + srow, stage.descripcion);
        xlsx.write("D" + srow, stage.orden);
        xlsx.write("E" + srow, stage.codigo);

        row++;
    }


    // Escritura de la pantalla de subetapas
    xlsx.addSheet("Subetapas");
    xlsx.selectSheet("Subetapas");

    xlsx.write("A1", "Id", formatHeader);
    xlsx.write("B1", "Etapa", formatHeader);
    xlsx.write("C1", "Nombre", formatHeader);
    xlsx.write("D1", "Descripción", formatHeader);
    xlsx.write("E1", "Orden", formatHeader);

    QList<SubetapaDefinition> substages;
    try {
        Database::DbElementSubstage::listAll(&substages);
    } catch(QString msg) {
        QMessageBox msgbox;
        msgbox.setText(msg);
        msgbox.exec();
        return false;
    }

    row = 2;
    for (auto substage : substages) {
        srow = QString::number(row);
        xlsx.write("A" + srow, substage.id);
        xlsx.write("B" + srow, substage.etapaId);
        xlsx.write("C" + srow, substage.nombre);
        xlsx.write("D" + srow, substage.descripcion);
        xlsx.write("E" + srow, substage.orden);

        row++;
    }

    xlsx.addSheet("Sucesos");
    xlsx.selectSheet("Sucesos");

    xlsx.write("A1", "Id", formatHeader);
    xlsx.write("B1", "Etapa", formatHeader);
    xlsx.write("C1", "Subetapa", formatHeader);
    xlsx.write("D1", "Nombre", formatHeader);
    xlsx.write("E1", "Código", formatHeader);
    xlsx.write("F1", "Descripción", formatHeader);
    xlsx.write("G1", "Error Humano", formatHeader);
    xlsx.write("H1", "Frecuencia", formatHeader);
    xlsx.write("I1", "Consecuencia", formatHeader);
    xlsx.write("J1", "Riesgo", formatHeader);
    xlsx.write("K1", "Completado", formatHeader);
    xlsx.addSheet("Reductores de frecuencia");
    xlsx.selectSheet("Reductores de frecuencia");
    xlsx.write("A1", "Id", formatHeader);
    xlsx.write("B1", "Suceso", formatHeader);
    xlsx.write("C1", "Código", formatHeader);
    xlsx.write("D1", "Nombre", formatHeader);
    xlsx.write("E1", "Descripción", formatHeader);
    xlsx.write("F1", "Robustez", formatHeader);
    xlsx.write("G1", "Marcado", formatHeader);
    xlsx.write("H1", "Orden", formatHeader);
    xlsx.addSheet("Barreras");
    xlsx.selectSheet("Barreras");
    xlsx.write("A1", "Id", formatHeader);
    xlsx.write("B1", "Suceso", formatHeader);
    xlsx.write("C1", "Código", formatHeader);
    xlsx.write("D1", "Nombre", formatHeader);
    xlsx.write("E1", "Descripción", formatHeader);
    xlsx.write("F1", "Robustez", formatHeader);
    xlsx.write("G1", "Marcado", formatHeader);
    xlsx.write("H1", "Orden", formatHeader);
    xlsx.addSheet("Reductores de consecuencia");
    xlsx.selectSheet("Reductores de consecuencia");
    xlsx.write("A1", "Id", formatHeader);
    xlsx.write("B1", "Suceso", formatHeader);
    xlsx.write("C1", "Código", formatHeader);
    xlsx.write("D1", "Nombre", formatHeader);
    xlsx.write("E1", "Descripción", formatHeader);
    xlsx.write("F1", "Robustez", formatHeader);
    xlsx.write("G1", "Marcado", formatHeader);
    xlsx.write("H1", "Orden", formatHeader);

    // Escritura de la pestaña de eventos
    if (events.size() > 0) {
        row = 2;
        int rfrow = 2;
        QString srfrow;
        int brow = 2;
        QString sbrow;
        int rcrow = 2;
        QString srcrow;

        for(auto eventsInStage : events) {

            for (auto event : eventsInStage.second) {
                xlsx.selectSheet("Sucesos");

                srow = QString::number(row);
                xlsx.write("A" + srow, event.id);
                xlsx.write("B" + srow, event.etapa);
                xlsx.write("C" + srow, event.subetapa);
                xlsx.write("D" + srow, event.nombre);
                xlsx.write("E" + srow, event.codigo);
                xlsx.write("F" + srow, event.descripcion);
                xlsx.write("G" + srow, event.errorHumano);
                xlsx.write("H" + srow, MatrizRiesgo::Frecuencias.at(event.frecuencia));
                xlsx.write("I" + srow, MatrizRiesgo::Consecuencias.at(event.consecuencia));
                xlsx.write("J" + srow, MatrizRiesgo::Riesgos.at(event.riesgo));
                xlsx.write("K" + srow, event.completado ? "Sí": "No");
                row++;

                if (event.reductoresFrecuencias.size() > 0) {
                    xlsx.selectSheet("Reductores de frecuencia");
                    for(auto defense : event.reductoresFrecuencias) {
                        srfrow = QString::number(rfrow);
                        xlsx.write("A" + srfrow, defense.id);
                        xlsx.write("B" + srfrow, defense.event);
                        xlsx.write("C" + srfrow, defense.code);
                        xlsx.write("D" + srfrow, defense.nombre);
                        xlsx.write("E" + srfrow, defense.descripcion);
                        xlsx.write("F" + srfrow, MatrizRiesgo::Durezas.at(defense.hardiness));
                        xlsx.write("G" + srfrow, defense.active);
                        xlsx.write("H" + srfrow, defense.orderStage);
                        rfrow++;
                    }
                }

                if (event.barreras.size() > 0) {
                    xlsx.selectSheet("Barreras");
                    for(auto defense : event.barreras) {
                        sbrow = QString::number(brow);
                        xlsx.write("A" + sbrow, defense.id);
                        xlsx.write("B" + sbrow, defense.event);
                        xlsx.write("C" + sbrow, defense.code);
                        xlsx.write("D" + sbrow, defense.nombre);
                        xlsx.write("E" + sbrow, defense.descripcion);
                        xlsx.write("F" + sbrow, MatrizRiesgo::Durezas.at(defense.hardiness));
                        xlsx.write("G" + sbrow, defense.active);
                        xlsx.write("H" + sbrow, defense.orderStage);
                        brow++;
                    }
                }

                if (event.reductoresConsecuencias.size() > 0) {
                    xlsx.selectSheet("Reductores de consecuencia");
                    for(auto defense : event.reductoresConsecuencias) {
                        srcrow = QString::number(rcrow);
                        xlsx.write("A" + srcrow, defense.id);
                        xlsx.write("B" + srcrow, defense.event);
                        xlsx.write("C" + srcrow, defense.code);
                        xlsx.write("D" + srcrow, defense.nombre);
                        xlsx.write("E" + srcrow, defense.descripcion);
                        xlsx.write("F" + srcrow, MatrizRiesgo::Durezas.at(defense.hardiness));
                        xlsx.write("G" + srcrow, defense.active);
                        xlsx.write("H" + srcrow, defense.orderStage);
                        rcrow++;
                    }
                }
            }
        }
    }

    xlsx.selectSheet("Etapas");
    xlsx.saveAs(path);

    return true;
}

bool xlsx::importTables(QString origin, QString destiny, QString _title)
{
    title = _title;
    dlg = new QProgressDialog("Localización de la base de datos para el modelo Marr", "Cancelar", 0, 100, nullptr);
    dlg->setWindowModality(Qt::WindowModal);
    dlg->setWindowModality(Qt::WindowModal);
    dlg->open();

    initiate(destiny);

    try {
        QCoreApplication::processEvents();

        if (dlg->wasCanceled()) return false;
        dlg->setLabelText("Creando estructura del modelo");
        createBase();

        QCoreApplication::processEvents();
        if (dlg->wasCanceled()) {
            delete dlg;
            return false;
        }
        dlg->setValue(10);
        dlg->setLabelText("Insertando datos de configuración");
        insertInConfiguration();

        xlsxDoc = new Document(origin);

        QStringList sheetNames = xlsxDoc->sheetNames();
        if (sheetNames.contains("Etapas")
                && sheetNames.contains("Subetapas")
                && sheetNames.contains("Sucesos")
                && sheetNames.contains("Reductores de frecuencia")
                && sheetNames.contains("Barreras")
                && sheetNames.contains("Reductores de consecuencia")
        ) {
            if (dlg->wasCanceled()) {
                delete dlg;
                return false;
            }
            dlg->setValue(15);
            dlg->setLabelText("Insertando etapas");
            QCoreApplication::processEvents();
            insertInStages();

            if (dlg->wasCanceled()) {
                delete dlg;
                return false;
            }
            dlg->setValue(20);
            dlg->setLabelText("Insertando subetapas");
            QCoreApplication::processEvents();
            insertInSubstages();

            if (dlg->wasCanceled()) {
                delete dlg;
                return false;
            }
            dlg->setValue(30);
            dlg->setLabelText("Insertando eventos");
            QCoreApplication::processEvents();
            insertInEvents();

            if (dlg->wasCanceled()) {
                delete dlg;
                return false;
            }
            dlg->setValue(60);
            dlg->setLabelText("Insertando defensas");
            QCoreApplication::processEvents();
            insertInDefenses();

            dlg->close();
            delete dlg;

            return true;
        }
    } catch (...) { return false; }

    return false;
}

void xlsx::insertInConfiguration()
{
    auto conf = new QSqlQuery(database);
    conf->prepare("INSERT INTO configuracion (version, nombre) VALUES (:version, :nombre)");
    conf->bindValue(":version", VERSION);
    conf->bindValue(":nombre", title);
    conf->exec();
}

void xlsx::insertInStages()
{
    xlsxDoc->selectSheet("Etapas");
    auto stagesTable = new QSqlQuery(database);
    stagesTable->prepare("INSERT INTO etapas (id, nombre, descripcion, orden, activo, codigo) VALUES(?,?,?,?,1,?)");

    etapasIdOrden.clear();
    validStageIds.clear();

    int lastId = INT_MIN;
    int row = 1;
    QString srow = QString::number(row);
    int stageId;
    QString codigoOrden;
    QString nombre;
    QString descripcion;
    int orden;
    while ((xlsxDoc->read("A" + QString::number(row + 1)).toString()) != "") {
        row++;
        srow = QString::number(row);
        stageId = xlsxDoc->read("A" + srow).toInt();
        if (lastId < stageId) {
            lastId = stageId;
        }

        nombre = xlsxDoc->read("B" + srow).toString();
        descripcion = xlsxDoc->read("C" + srow).toString();
        codigoOrden = xlsxDoc->read("E" + srow).toString();
        orden = xlsxDoc->read("D" + srow).toInt();
        etapasIdOrden.insert(codigoOrden, stageId);
        validStageIds.insert(stageId);

        stagesTable->bindValue(0, stageId);
        stagesTable->bindValue(1, nombre); // Nombre
        stagesTable->bindValue(2, descripcion); // Descripción
        stagesTable->bindValue(3, orden); // Orden
        stagesTable->bindValue(4, codigoOrden); // Código
        stagesTable->exec();

    }

    stagesTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='etapas'");
    stagesTable->bindValue(0, lastId + 1);
    stagesTable->exec();

    delete stagesTable;
}

void xlsx::insertInSubstages()
{
    xlsxDoc->selectSheet("Subetapas");
    auto substagesTable = new QSqlQuery(database);
    substagesTable->prepare("INSERT INTO subetapas (id,nombre,descripcion,orden,activo,etapa_id) VALUES (?,?,?,?,1,?)");
    
    validSubstageIds.clear();
    
    int lastId = 1;
    int row = 1;
    QString srow = QString::number(row);
    int substageId;
    int etapaId;
    QString nombre;
    
    while ((xlsxDoc->read("A" + QString::number(row + 1)).toString()) != "") {
        row++;
        srow = QString::number(row);
        substageId = xlsxDoc->read("A" + srow).toInt();
        etapaId = xlsxDoc->read("B" + srow).toInt();
        nombre = xlsxDoc->read("C" + srow).toString();
        
        if (lastId < substageId) {
            lastId = substageId;
        }

        // Validate that the stage exists
        if (!validStageIds.contains(etapaId)) {
            QString cellRef = "Fila " + srow;
            QString errorMsg = "La etapa " + QString::number(etapaId) + " no existe en la pestaña 'Etapas'";
            
            errorList.append(ErrorLocation{
                "Subetapas",
                cellRef,
                "Etapa inexistente",
                QString::number(etapaId),
                errorMsg,
                QVector<QString>()
            });
            
            // Skip this substage
            continue;
        }

        substagesTable->bindValue(0, substageId); // Id
        substagesTable->bindValue(4, etapaId); // Etapa Id
        substagesTable->bindValue(1, nombre); // Nombre
        substagesTable->bindValue(2, xlsxDoc->read("D" + srow).toString()); // Descripción
        substagesTable->bindValue(3, xlsxDoc->read("E" + srow).toInt()); // Orden
        substagesTable->exec();
        
        // Track valid substages
        QString substageKey = QString::number(etapaId) + "-" + QString::number(substageId);
        validSubstageIds.insert(substageKey);
    }

    substagesTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='subetapas'");
    substagesTable->bindValue(0, lastId + 1);
    substagesTable->exec();

    delete substagesTable;
}

void xlsx::insertInEvents()
{
    const std::map<QString, int> InversoRiesgos{
        {"RB", 0},
        {"RM", 1},
        {"RA", 2},
        {"RMA", 3}
    };
    const std::map<QString, int> InversoFrecuencias{
        {"FMB", 0},
        {"FB", 1},
        {"FM", 2},
        {"FA", 3}
    };
    const std::map<QString, int> InversoConsecuencias{
        {"CMB", -1},
        {"CB", 0},
        {"CM", 1},
        {"CA", 2},
        {"CMA", 3}
    };

    xlsxDoc->selectSheet("Sucesos");
    auto eventsTable = new QSqlQuery(database);
    eventsTable->prepare("INSERT INTO suceso_iniciador (id,nombre,descripcion,codigo,frecuencia,consecuencia,etapa_id,subetapa_id,error_humano,riesgo,base,completado) VALUES (?,?,?,?,?,?,?,?,?,?,1,?)");
    int lastId = 0;
    int row = 1;
    QString srow, eventColumn;
    QString columnaFrecuencia;
    QString columnaConsecuencia;
    QString columnaRiesgo;
    QString columnaCompletado;
    int eventId;
    int etapaId;
    int subetapaId;
    
    while (row++, srow = QString::number(row), ((eventColumn = xlsxDoc->read("A" + srow).toString())) != "") {
        eventId = eventColumn.toInt();
        if (lastId < eventId) {
            lastId = eventId;
        }
        
        etapaId = xlsxDoc->read("B" + srow).toInt();
        subetapaId = xlsxDoc->read("C" + srow).toInt();
        QString nombre = xlsxDoc->read("D" + srow).toString();
        
        // Validate that the stage exists
        if (!validStageIds.contains(etapaId)) {
            QString cellRef = "Fila " + srow;
            QString errorMsg = "La etapa " + QString::number(etapaId) + " no existe en la pestaña 'Etapas'";
            
            errorList.append(ErrorLocation{
                "Sucesos",
                cellRef,
                "Etapa inexistente",
                QString::number(etapaId),
                errorMsg,
                QVector<QString>()
            });
            
            // Skip this event
            continue;
        }
        
        // Validate that the substage exists (if specified, subetapaId = 0 means no substage)
        if (subetapaId != 0) {
            QString substageKey = QString::number(etapaId) + "-" + QString::number(subetapaId);
            if (!validSubstageIds.contains(substageKey)) {
                QString cellRef = "Fila " + srow;
                QString errorMsg = "La subetapa " + QString::number(subetapaId) + 
                                   " no existe en la etapa " + QString::number(etapaId) + 
                                   " en la pestaña 'Subetapas'";
                
                errorList.append(ErrorLocation{
                    "Sucesos",
                    cellRef,
                    "Subetapa inexistente",
                    QString::number(subetapaId),
                    errorMsg,
                    QVector<QString>()
                });
                
                // Skip this event
                continue;
            }
        }

        eventsTable->bindValue(0, eventId);
        eventsTable->bindValue(1, nombre); // Nombre
        eventsTable->bindValue(2, xlsxDoc->read("F" + srow).toString()); // Descripción
        eventsTable->bindValue(3, xlsxDoc->read("E" + srow).toString()); // Código
        columnaFrecuencia = normalizeString(xlsxDoc->read("H" + srow).toString());
        if (InversoFrecuencias.find(columnaFrecuencia) != InversoFrecuencias.end() ) {
            eventsTable->bindValue(4, InversoFrecuencias.at(columnaFrecuencia)); // Frecuencia
        } else {
            eventsTable->bindValue(4, 0);
        }
        columnaConsecuencia = normalizeString(xlsxDoc->read("I" + srow).toString());
        if (InversoConsecuencias.find(columnaConsecuencia) != InversoConsecuencias.end() ) {
            eventsTable->bindValue(5, InversoConsecuencias.at(columnaConsecuencia)); // Consecuencia
        } else {
            eventsTable->bindValue(5, 0);
        }

        eventsTable->bindValue(6, etapaId); // Etapa id
        eventsTable->bindValue(7, subetapaId); // Subetapa
        eventsTable->bindValue(8, xlsxDoc->read("G" + srow).toBool()); // Error humano
        columnaRiesgo = normalizeString(xlsxDoc->read("J" + srow).toString());
        if (InversoRiesgos.find(columnaRiesgo) != InversoRiesgos.end() ) {
            eventsTable->bindValue(9, InversoRiesgos.at(columnaRiesgo)); // Riesgo
        } else {
            eventsTable->bindValue(9, 0);
        }
        // Evento completado?
        columnaCompletado = normalizeString(xlsxDoc->read("K" + srow).toString());
        if (columnaCompletado == "SI" || columnaCompletado == "TRUE" || 
            columnaCompletado == "VERDADERO" || columnaCompletado == "1"){
            eventsTable->bindValue(10, 1);
        } else {
            eventsTable->bindValue(10, 0);
        }
        eventsTable->exec();
        //qDebug() << eventsTable->lastError();
    }

    eventsTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='suceso_iniciador'");
    eventsTable->bindValue(0, lastId + 1);
    eventsTable->exec();

    delete eventsTable;
}

void xlsx::insertInDefenses()
{
    auto defensesTable = new QSqlQuery(database);
    defensesTable->prepare("INSERT INTO defensa (suceso_iniciador_id,codigo,nombre,descripcion,robustez,tipo,activo,etapa_orden_id) VALUES (?,?,?,?,?,?,?,?)");

    int lastId = 1;
    if (dlg->wasCanceled()) {
        delete dlg;
        delete defensesTable;
        return;
    }
    dlg->setValue(70);
    dlg->setLabelText("Insertando Reductores de frecuencia");
    QCoreApplication::processEvents();
    lastId = insertInDefensesByType(lastId, "Reductores de frecuencia", defensesTable, 0);
    if (dlg->wasCanceled()) {
        delete dlg;
        delete defensesTable;
        return;
    }
    dlg->setValue(80);
    dlg->setLabelText("Insertando Barreras");
    QCoreApplication::processEvents();
    lastId = insertInDefensesByType(lastId, "Barreras", defensesTable, 1);
    if (dlg->wasCanceled()) {
        delete dlg;
        delete defensesTable;
        return;
    }
    dlg->setValue(90);
    dlg->setLabelText("Insertando Reductores de consecuencia");
    QCoreApplication::processEvents();
    lastId = insertInDefensesByType(lastId, "Reductores de consecuencia", defensesTable, 2);

    delete defensesTable;
}

int xlsx::insertInDefensesByType(int lastId, QString sheetName, QSqlQuery *defensesTable, int tipoDefensa)
{
    xlsxDoc->selectSheet(sheetName);
    int row = 1;
    QString srow, columnDefenseId;
    int defenseId;
    QString codigoEtapaOrden;
    QString columnaDureza;

    while (row++,
           srow = QString::number(row),
           ((columnDefenseId = xlsxDoc->read("A" + srow).toString())) != ""
    ) {
        defenseId = columnDefenseId.toInt();
        //std::cout << row << ": " << columnDefenseId.toStdString() << "->" << xlsxDoc->read("C" + srow).toString().toStdString() << std::endl;
        if (lastId < defenseId) {
            lastId = defenseId;
        }

        defensesTable->bindValue(0, xlsxDoc->read("B" + srow).toInt()); // Suceso Id
        defensesTable->bindValue(1, xlsxDoc->read("C" + srow).toString()); // Código
        defensesTable->bindValue(2, xlsxDoc->read("D" + srow).toString()); // Nombre
        defensesTable->bindValue(3, xlsxDoc->read("E" + srow).toString()); // Descripción
        columnaDureza = normalizeString(xlsxDoc->read("F" + srow).toString());
        if (InversoDurezas.find(columnaDureza) != InversoDurezas.end()) {
            defensesTable->bindValue(4, InversoDurezas.at(columnaDureza)); // Robustez
        } else {
            defensesTable->bindValue(4, "");
        }
        defensesTable->bindValue(5, tipoDefensa);
        defensesTable->bindValue(6, xlsxDoc->read("G" + srow).toBool()); // Marcado/Activo
        codigoEtapaOrden = xlsxDoc->read("H" + srow).toString();
        if ((codigoEtapaOrden != "") && (etapasIdOrden.contains(codigoEtapaOrden)) ) {
            defensesTable->bindValue(7, etapasIdOrden.find(codigoEtapaOrden).value()); // Etapa orden
        } else {
            //defensesTable->
            //defensesTable->bindValue(7, QVariant(QMetaType::QString));
        }
        defensesTable->exec();
        auto error = defensesTable->lastError();
        if (error.isValid()) {
            QVector<QString> rowData(3);
            rowData[0] = xlsxDoc->read("B" + srow).toString();
            rowData[1] = xlsxDoc->read("C" + srow).toString();
            rowData[2] = xlsxDoc->read("D" + srow).toString();
            
            QString cellRef = "Fila " + srow;
            QString errorMsg = error.text();
            QString cellValue = rowData[1]; // Código
            
            errorList.append(ErrorLocation{
                sheetName,
                cellRef,
                "Error de base de datos",
                cellValue,
                errorMsg,
                rowData
            });
            qDebug() << defensesTable->lastError();
        }
    }

    return lastId;
}

QString xlsx::normalizeString(const QString &str)
{
    QString normalized = str.toUpper();
    
    // Eliminar acentos - mayúsculas
    normalized.replace("Á", "A");
    normalized.replace("É", "E");
    normalized.replace("Í", "I");
    normalized.replace("Ó", "O");
    normalized.replace("Ú", "U");
    normalized.replace("Ñ", "N");
    normalized.replace("Ü", "U");
    
    
    return normalized;
}

}
