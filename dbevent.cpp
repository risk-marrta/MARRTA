#include "dbevent.h"

#include "etapas.h"
#include "subetapas.h"

namespace Database {

    void DbEvent::getAllInfo(const MatrizRiesgo &matriz, std::unordered_map<std::string, std::vector<CompleteEventDefinition> > &events)
    {
        // Primero guardamos las defensas en estructuras temporales separadas por el evento al que están asociadas.
        std::unordered_map<int, std::vector<DefenseDefinition>> tmpReductoresFrecuencias;
        std::unordered_map<int, std::vector<DefenseDefinition>> tmpBarreras;
        std::unordered_map<int, std::vector<DefenseDefinition>> tmpReductoresConsecuencias;

        auto qryDef = new QSqlQuery(R"""(SELECT d.id,
                d.suceso_iniciador_id,
                d.nombre,
                d.robustez,
                d.tipo,
                d.activo,
                d.descripcion,
                etapas.codigo ce,
                etapas.orden,
                d.codigo
           FROM defensa d
           LEFT JOIN etapas ON etapas.id=d.etapa_orden_id
           ORDER BY etapas.orden)"""
        );
        if (!qryDef->exec()) {
            qDebug() << qryDef->lastError().text();
            delete qryDef;
            throw QString("Error leyendo los eventos");
        }
        while(qryDef->next()) {
            DefenseDefinition definition;
            definition.id = qryDef->value(0).toInt();
            definition.event = qryDef->value(1).toInt();
            definition.code = qryDef->value(9).toString();
            definition.nombre = qryDef->value(2).toString();
            definition.hardiness = qryDef->value(3).toInt();
            definition.active = qryDef->value(5).toBool();
            definition.descripcion = qryDef->value(6).toString();
            definition.orderStage = qryDef->value(7).toString();

            int suceso = qryDef->value(1).toInt();
            int tipo = qryDef->value(4).toInt();
            if (tipo == static_cast<int>(DefenseType::FrecuencyReducer)) {
                definition.type = DefenseType::FrecuencyReducer;
                tmpReductoresFrecuencias[suceso].push_back(definition);
            } else if (tipo == static_cast<int>(DefenseType::Barrier)) {
                definition.type = DefenseType::Barrier;
                tmpBarreras[suceso].push_back(definition);
            } else if (tipo == static_cast<int>(DefenseType::ConsequenceReducer)) {
                definition.type = DefenseType::ConsequenceReducer;
                tmpReductoresConsecuencias[suceso].push_back(definition);
            }
        }

        delete qryDef;

        // / Obtenemos los eventos
        auto qry = new QSqlQuery(R"""(SELECT
                id,
                nombre,
                descripcion,
                codigo,
                frecuencia,
                consecuencia,
                error_humano,
                etapa_id,
                subetapa_id,
                base,
                completado
            FROM suceso_iniciador si
            ORDER BY id
        )""");
        if (!qry->exec()) {
            delete qry;
            qDebug() << qry->lastError().text();
            throw QString("Error leyendo los eventos");
        }

        events.clear();

        while(qry->next()) {
            CompleteEventDefinition dbModel;

            dbModel.id = qry->value(0).toInt();
            dbModel.nombre = qry->value(1).toString();
            dbModel.descripcion = qry->value(2).toString();
            dbModel.codigo = qry->value(3).toString();
            dbModel.frecuencia = qry->value(4).toInt();
            dbModel.consecuencia = qry->value(5).toInt();
            dbModel.errorHumano = qry->value(6).toBool();
            dbModel.etapa = qry->value(7).toInt();
            dbModel.subetapa = qry->value(8).toInt();
            dbModel.base = qry->value(9).toBool();
            dbModel.completado = qry->value(10).toBool();
            dbModel.reductoresFrecuencias = tmpReductoresFrecuencias[dbModel.id];
            dbModel.barreras = tmpBarreras[dbModel.id];
            dbModel.reductoresConsecuencias = tmpReductoresConsecuencias[dbModel.id];

            std::string index = std::to_string(dbModel.etapa) + "-" + std::to_string(dbModel.subetapa);

            dbModel.calcularReductores();
            dbModel.calcularRiesgo(matriz);

            events[index].push_back(dbModel);
        }

        delete qry;
    }

    void DbEvent::list(QList<EventDefinition>& elementsList, int stage, int substage)
    {
        auto qry = new QSqlQuery();
        QString sql = R"""(SELECT
                si.id,
                si.nombre,
                si.descripcion,
                si.codigo,
                si.frecuencia,
                si.consecuencia,
                e.nombre etapa_nombre,
                s.nombre subetapa_nombre,
                si.error_humano,
                si.base,
                si.etapa_id,
                si.subetapa_id
            FROM suceso_iniciador si
            LEFT JOIN etapas e
                ON e.id=si.etapa_id
            LEFT JOIN subetapas s
                ON s.id=si.subetapa_id)""";
        
        // Handle different filtering cases
        if (stage == -1) {
            // Get all events - no WHERE clause needed
            sql += " ORDER BY e.orden";
        } else {
            sql += " WHERE ";
            if (substage != 0) {
                sql += " si.subetapa_id=:subetapa";
            } else {
                sql += " si.etapa_id=:etapa AND (si.subetapa_id IS NULL OR si.subetapa_id=0)";
            }
            sql += " ORDER BY e.orden";
        }
        
        qry->prepare(sql);
        qDebug() << "DbEvent::list() - SQL:" << sql;

        if (stage != -1) {
            if (substage != 0) {
                qry->bindValue(":subetapa", substage);
                qDebug() << "DbEvent::list() - Filtering by substage:" << substage;
            } else {
                qry->bindValue(":etapa", stage);
                qDebug() << "DbEvent::list() - Filtering by stage:" << stage;
                //qDebug() << "Etapa: " << stage;
            }
        }

        if (!qry->exec()) {
            qDebug() << "Error leyendo los eventos: " << qry->lastError();
            delete qry;
            throw QString("Error leyendo los eventos");
        }

        //QStringList etapas;
        elementsList.clear();
        
        int count = 0;
        while(qry->next()) {
            count++;
            EventDefinition dbModel;
            dbModel.id = qry->value(0).toInt(); //qDebug() << dbModel.id;
            dbModel.nombre = qry->value(1).toString();
            dbModel.descripcion = qry->value(2).toString();
            dbModel.codigo = qry->value(3).toString();
            dbModel.frecuencia = qry->value(4).toInt();
            dbModel.consecuencia = qry->value(5).toInt();
            dbModel.modificado = false;
            dbModel.creado = false;
            dbModel.errorHumano = (qry->value(8).toInt() == 1);
            dbModel.base = qry->value(9).toBool();

            EtapaDefinition etapaModel;
            // For all events query, get the actual stage ID from the result
            etapaModel.id = (stage == -1) ? qry->value(10).toInt() : stage;
            etapaModel.nombre = qry->value(6).toString();
            dbModel.etapa = etapaModel;

            // Get substage info from query result for all events
            if (stage == -1 || substage != 0) {
                SubetapaDefinition subetapaModel;
                subetapaModel.id = qry->value(11).toInt();
                subetapaModel.nombre = qry->value(7).toString();
                dbModel.subetapa = subetapaModel;
            }

            elementsList << dbModel;
        }
        
        qDebug() << "DbEvent::list() - Retrieved" << count << "events";
        delete qry;
    }

    void DbEvent::listAllForStage(QList<EventDefinition>& elementsList, int stage)
    {
        auto qry = new QSqlQuery();
        QString sql = R"""(SELECT
                si.id,
                si.nombre,
                si.descripcion,
                si.codigo,
                si.frecuencia,
                si.consecuencia,
                e.nombre etapa_nombre,
                s.nombre subetapa_nombre,
                si.error_humano,
                si.base,
                si.etapa_id,
                si.subetapa_id
            FROM suceso_iniciador si
            LEFT JOIN etapas e
                ON e.id=si.etapa_id
            LEFT JOIN subetapas s
                ON s.id=si.subetapa_id
            WHERE si.etapa_id=:etapa
            ORDER BY e.orden)""";
        
        qry->prepare(sql);
        qry->bindValue(":etapa", stage);

        if (!qry->exec()) {
            qDebug() << "Error leyendo los eventos: " << qry->lastError();
            delete qry;
            throw QString("Error leyendo los eventos");
        }

        elementsList.clear();
        
        while(qry->next()) {
            EventDefinition dbModel;
            dbModel.id = qry->value(0).toInt();
            dbModel.nombre = qry->value(1).toString();
            dbModel.descripcion = qry->value(2).toString();
            dbModel.codigo = qry->value(3).toString();
            dbModel.frecuencia = qry->value(4).toInt();
            dbModel.consecuencia = qry->value(5).toInt();
            dbModel.modificado = false;
            dbModel.creado = false;
            dbModel.errorHumano = (qry->value(8).toInt() == 1);
            dbModel.base = qry->value(9).toBool();

            EtapaDefinition etapaModel;
            etapaModel.id = qry->value(10).toInt();
            etapaModel.nombre = qry->value(6).toString();
            dbModel.etapa = etapaModel;

            SubetapaDefinition subetapaModel;
            subetapaModel.id = qry->value(11).toInt();
            subetapaModel.nombre = qry->value(7).toString();
            dbModel.subetapa = subetapaModel;

            elementsList << dbModel;
        }
        
        delete qry;
    }

    QStringList DbEvent::listCodesFromEventsWithStage(int stage)
    {
        QStringList codes;
        auto qry = new QSqlQuery();
        qry->prepare(R"""(SELECT
                     codigo
                 FROM suceso_iniciador
                 WHERE etapa_id=:etapa)""");
        qry->bindValue(":etapa", stage);
        if (qry->exec()) {
            while(qry->next()) {
                //qDebug() << "Código encontrado: " << qry->value(0).toString();
                codes << qry->value(0).toString();
            }
        } else {
            qDebug() << "Error leyendo los eventos: " << qry->lastError().text();
        }

        delete qry;

        return codes;
    }

    void DbEvent::setCompleted(int eventId, bool completed, int riesgo)
    {
        QSqlQuery *updateQuery = new QSqlQuery();
        updateQuery->prepare("UPDATE suceso_iniciador SET completado=:completado,riesgo=:riesgo WHERE id=:id");
        updateQuery->bindValue(":completado", completed);
        updateQuery->bindValue(":riesgo", riesgo);
        updateQuery->bindValue(":id", eventId);
        if (!updateQuery->exec()) {
            delete updateQuery;
            throw "UPDATE: " + updateQuery->lastError().databaseText();
        }
        delete updateQuery;
    }

    void DbEvent::updateRisk(int eventId, int riesgo)
    {
        QSqlQuery *updateQuery = new QSqlQuery();
        updateQuery->prepare("UPDATE suceso_iniciador SET riesgo=:riesgo WHERE id=:id");
        updateQuery->bindValue(":riesgo", riesgo);
        updateQuery->bindValue(":id", eventId);
        if (!updateQuery->exec()) {
            delete updateQuery;
            throw "UPDATE: " + updateQuery->lastError().databaseText();
        }
        delete updateQuery;
    }

    bool DbEvent::remove(EventDefinition &event)
    {
        QSqlQuery *deleteQuery = new QSqlQuery();
        deleteQuery->prepare("DELETE FROM suceso_iniciador WHERE id=:id");
        deleteQuery->bindValue(":id", event.id);
        if (!deleteQuery->exec()) {
            qDebug() << deleteQuery->lastError();
            delete deleteQuery;
            return false;
        }

        delete deleteQuery;
        return true;
    }

    void DbEvent::update(QList<EventDefinition>& elementsList, bool updateAll)
    {
        Q_UNUSED(updateAll);
        QSqlQuery *updateQuery = new QSqlQuery();
        updateQuery->prepare("UPDATE suceso_iniciador SET nombre=:nombre, codigo=:codigo,descripcion=:descripcion,frecuencia=:frecuencia,consecuencia=:consecuencia,error_humano=:error_humano,base=:base WHERE id=:id");
        for(auto event : elementsList) {
            if (event.errorHumano && (event.frecuencia == 0)) {
                event.frecuencia = 1;
            }
            if (event.base){
                event.base = false;
            }
            updateQuery->bindValue(":nombre", event.nombre);
            updateQuery->bindValue(":codigo", event.codigo);
            updateQuery->bindValue(":descripcion", event.descripcion);
            updateQuery->bindValue(":frecuencia", event.frecuencia);
            updateQuery->bindValue(":consecuencia", event.consecuencia);
            updateQuery->bindValue(":error_humano", event.errorHumano);
            updateQuery->bindValue(":base", event.base);
            updateQuery->bindValue(":id", event.id);
            if (!updateQuery->exec()) {
                delete updateQuery;
                throw "UPDATE: " + updateQuery->lastError().databaseText();
            }
        }
        delete updateQuery;
    }

    void DbEvent::updateElement(EventDefinition& event)
    {
        QSqlQuery *updateQuery = new QSqlQuery();
        updateQuery->prepare("UPDATE suceso_iniciador SET nombre=:nombre, codigo=:codigo,descripcion=:descripcion,frecuencia=:frecuencia,consecuencia=:consecuencia,error_humano=:error_humano,base=:base WHERE id=:id");

        if (event.errorHumano && (event.frecuencia == 0)) {
            event.frecuencia = 1;
        }
        if (event.base){
            event.base = false;
        }
        updateQuery->bindValue(":nombre", event.nombre);
        updateQuery->bindValue(":codigo", event.codigo);
        updateQuery->bindValue(":descripcion", event.descripcion);
        updateQuery->bindValue(":frecuencia", event.frecuencia);
        updateQuery->bindValue(":consecuencia", event.consecuencia);
        updateQuery->bindValue(":error_humano", event.errorHumano);
        updateQuery->bindValue(":base", event.base);
        updateQuery->bindValue(":id", event.id);
        if (!updateQuery->exec()) {
            delete updateQuery;
            throw "UPDATE: " + updateQuery->lastError().databaseText();
        }
        delete updateQuery;
    }

    QVariant DbEvent::create(EventDefinition &event)
    {
        if (event.errorHumano && (event.frecuencia == 0)) {
            event.frecuencia = 1;
        }
        QSqlQuery *createQuery = new QSqlQuery();
        createQuery->prepare(
            "INSERT INTO suceso_iniciador\
                (nombre,descripcion,codigo,frecuencia,consecuencia,etapa_id,subetapa_id,error_humano) \
                VALUES (:nombre,:descripcion,:codigo,:frecuencia,:consecuencia,:etapa_id,:subetapa_id,:error_humano)");
        createQuery->bindValue(":nombre", event.nombre);
        createQuery->bindValue(":descripcion", event.descripcion);
        createQuery->bindValue(":codigo", event.codigo);
        createQuery->bindValue(":frecuencia", event.frecuencia);
        createQuery->bindValue(":consecuencia", event.consecuencia);
        createQuery->bindValue(":etapa_id", event.etapa.id);
        createQuery->bindValue(":subetapa_id", (event.subetapa.id == -1) ? QVariant::fromValue(nullptr) : event.subetapa.id);
        createQuery->bindValue(":error_humano", event.errorHumano);
        if (!createQuery->exec()) {
            throw "CREATING: " + createQuery->lastError().databaseText();
        }
        auto idQ = createQuery->lastInsertId();

        delete createQuery;

        return idQ;
    }

    void DbEvent::checkAndUpdate(const std::unordered_map<std::string, std::vector<CompleteEventDefinition> > &stages)
    {
        QSqlQuery *updateEvent = new QSqlQuery();
        updateEvent->prepare(
                    "UPDATE suceso_iniciador SET riesgo=:riesgo WHERE id=:id"
        );
        QSqlQuery *updateDefense = new QSqlQuery();
        updateDefense->prepare(
                    "UPDATE defensa SET activo=:activo WHERE id=:id"
        );

        for(auto stage : stages) {
            auto events = stage.second;

            for (auto event : events) {
                updateEvent->bindValue(":riesgo", event.riesgo);
                updateEvent->bindValue(":id", event.id);
                updateEvent->exec();


                for (auto defensa : event.reductoresFrecuencias) {
                    if (defensa.changed) {
                        updateDefense->bindValue(":activo", defensa.active);
                        updateDefense->bindValue(":id", defensa.id);
                        updateDefense->exec();
                    }
                }
                for (auto defensa : event.barreras) {
                    if (defensa.changed) {
                        updateDefense->bindValue(":activo", defensa.active);
                        updateDefense->bindValue(":id", defensa.id);
                        updateDefense->exec();
                    }
                }
                for (auto defensa : event.reductoresConsecuencias) {
                    if (defensa.changed) {
                        updateDefense->bindValue(":activo", defensa.active);
                        updateDefense->bindValue(":id", defensa.id);
                        updateDefense->exec();
                    }
                }
            }
        }
    }
}
