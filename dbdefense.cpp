#include "dbdefense.h"

namespace Database {
void DbDefense::create(DefenseDefinition &defensa)
{
    QSqlQuery *insertQuery = new QSqlQuery();
    insertQuery->prepare(R"""(INSERT INTO defensa
            (suceso_iniciador_id,codigo,nombre,robustez,tipo,descripcion,etapa_orden_id,activo)
        VALUES
            (:evento,:codigo,:nombre,:robustez,:tipo,:descripcion,:etapa_orden_id,:activo))"""
    );

    insertQuery->bindValue(":evento", defensa.event);
    insertQuery->bindValue(":codigo", defensa.code);
    insertQuery->bindValue(":nombre", defensa.nombre);
    insertQuery->bindValue(":robustez", defensa.hardiness);
    insertQuery->bindValue(":tipo", static_cast<int>(defensa.type));
    insertQuery->bindValue(":descripcion", defensa.descripcion);
    insertQuery->bindValue(":etapa_orden_id", defensa.orderStageId);
    insertQuery->bindValue(":activo", defensa.active);
    if (!insertQuery->exec()) {
        qDebug() << "Error creando defensa" << insertQuery->lastError().text() << defensa.orderStageId << defensa.bdOrden;
        throw insertQuery->lastError().text();
    }
}

void DbDefense::list(QList<DefenseDefinition>& elementList, int _eventId, DefenseType _defenseType)
{
    elementList.clear();
    auto qry = new QSqlQuery();
    QString sql = R"""(SELECT defensa.id,
          defensa.codigo,
          defensa.nombre,
          robustez,
          defensa.descripcion,
          etapa_orden_id,
          etapas.orden,
          defensa.activo
       FROM defensa
       LEFT JOIN etapas ON etapas.id=defensa.etapa_orden_id
       WHERE suceso_iniciador_id=:eventId
         AND tipo=:defenseType
       ORDER BY etapas.orden
    )""";
    qry->prepare(sql);
    qry->bindValue(":eventId", _eventId);
    qry->bindValue(":defenseType", static_cast<int>(_defenseType));

    if (!qry->exec()) {
        qDebug() << "Error de la base de datos" << qry->lastError().nativeErrorCode();
        delete qry;
        throw QString("Error leyendo las defensas");
    }

    int orden = 0;
    while(qry->next()) {
        DefenseDefinition dbModel;
        dbModel.id = qry->value(0).toInt();
        dbModel.code = qry->value(1).toString();
        dbModel.nombre = qry->value(2).toString();
        dbModel.hardiness = qry->value(3).toInt();
        dbModel.descripcion = qry->value(4).toString();
        dbModel.event = _eventId;
        dbModel.type = _defenseType;
        dbModel.orderStageId = qry->value(5).toInt();

        dbModel.bdOrden = qry->value(6).toInt();
        dbModel.active = qry->value(7).toBool();
        // Esto se usa para identificar qué defensa se pulsa en las listas
        dbModel.orden = orden++;
        dbModel.modificado = false;
        dbModel.creado = false;

        //qDebug() << dbModel.nombre;

        elementList << dbModel;
    }
    delete qry;
}

std::vector<std::pair<int, int>> DbDefense::groupByHardiness(int _eventId, DefenseType _defenseType, bool activesOnly)
{
    QSqlQuery *listQuery = new QSqlQuery();
    if (activesOnly) {
        listQuery->prepare("SELECT robustez,count(robustez) FROM defensa WHERE suceso_iniciador_id=:eventId AND tipo=:type AND activo=true GROUP BY robustez");
    } else {
        listQuery->prepare("SELECT robustez,count(robustez) FROM defensa WHERE suceso_iniciador_id=:eventId AND tipo=:type GROUP BY robustez");
    }
    listQuery->bindValue(":eventId", _eventId);
    listQuery->bindValue(":type", static_cast<int>(_defenseType));

    if (!listQuery->exec()) {
        qDebug() << listQuery->lastError().text();
        throw QString("Error leyendo las defensas");
    }

    std::vector<std::pair<int, int>> groups;
    while(listQuery->next()) {
        groups.push_back(std::make_pair(listQuery->value(0).toInt(), listQuery->value(1).toInt()));
    }

    return groups;
}

int DbDefense::getLastDefenseCode(int etapaOrdenId)
{
    QSqlQuery qry;
    //if (qry.exec("SELECT DISTINCT(CAST(substr(codigo, 8) AS int )) AS c FROM defensa WHERE c IS NOT NULL AND c <> '' ORDER BY c DESC LIMIT 1")) {

    QString sql =
        R"(SELECT
                COALESCE(DISTINCT(CAST(substr(codigo, 8) AS int )), 1) AS c
            FROM defensa
            WHERE etapa_orden_id=:etapaOrdenId
            ORDER BY c
            DESC LIMIT 1)";
    qry.prepare(sql);
    qry.bindValue(":etapaOrdenId", etapaOrdenId);
    if (qry.exec()) {
        qry.next();

        return qry.value(0).toInt();
    }

    return 0;
}

DefenseDefinition DbDefense::searchByName(QString name)
{
    QSqlQuery qry;
    qry.prepare(R"""(SELECT id,suceso_iniciador_id,codigo,nombre,descripcion,robustez,tipo,activo,etapa_orden_id FROM defensa WHERE nombre=:nombre)""");
    qry.bindValue(":nombre", name);
    if (!qry.exec()) {
        qDebug() << qry.lastError();
        throw QString("Error buscando una defensa");
    }

    DefenseDefinition dbModel;
    qry.next();
    if (qry.isValid()) {
        dbModel.id = qry.value(0).toInt();
        dbModel.event = qry.value(1).toInt();
        dbModel.code = qry.value(2).toString();
        dbModel.nombre = qry.value(3).toString();
        dbModel.descripcion = qry.value(4).toString();
        dbModel.hardiness = qry.value(5).toInt();
        dbModel.type = DefenseType(qry.value(6).toInt());
        dbModel.active = qry.value(7).toBool();
        dbModel.orderStageId = qry.value(8).toInt();
    }

    dbModel.modificado = false;
    dbModel.creado = false;

    return dbModel;
}

void DbDefense::update(QList<DefenseDefinition>& elementsList, bool updateAll)
{
    QSqlQuery *insertQuery = new QSqlQuery();
    insertQuery->prepare(R"""(INSERT INTO defensa
            (suceso_iniciador_id,codigo,nombre,robustez,tipo,descripcion,etapa_orden_id)
        VALUES
            (:evento,:codigo,:nombre,:robustez,:tipo,:descripcion,:etapa_orden_id))"""
    );
    QSqlQuery *updateQuery = new QSqlQuery();
    updateQuery->prepare(R"""(UPDATE defensa
        SET
            codigo=:codigo,
            nombre=:nombre,
            robustez=:robustez,
            descripcion=:descripcion,
            etapa_orden_id=:etapa_orden_id
        WHERE id=:id)"""
    );
    for(auto defensa : elementsList) {

        if (defensa.creado == true) {
            insertQuery->bindValue(":evento", defensa.event);
            insertQuery->bindValue(":codigo", defensa.code);
            insertQuery->bindValue(":nombre", defensa.nombre);
            insertQuery->bindValue(":robustez", defensa.hardiness);
            insertQuery->bindValue(":tipo", static_cast<int>(defensa.type));
            insertQuery->bindValue(":descripcion", defensa.descripcion);
            insertQuery->bindValue(":etapa_orden_id", defensa.orderStageId);
            if (!insertQuery->exec()) {
                qDebug() << "Error creando defensa" << insertQuery->lastError().text() << defensa.orderStageId << defensa.bdOrden;
                throw insertQuery->lastError().text();
            }
        } else if ((defensa.modificado == true) || (updateAll == true)) {
            updateQuery->bindValue(":nombre", defensa.nombre);
            updateQuery->bindValue(":codigo", defensa.code);
            updateQuery->bindValue(":robustez", defensa.hardiness);
            updateQuery->bindValue(":id", defensa.id);
            updateQuery->bindValue(":descripcion", defensa.descripcion);
            updateQuery->bindValue(":etapa_orden_id", defensa.orderStageId);
            if (!updateQuery->exec()) {
                throw "UPDATE: " + updateQuery->lastError().text();
            }
        }
    }
    delete insertQuery;
    delete updateQuery;
}

QString DbDefense::updateElement(DefenseDefinition &defensa)
{
    QSqlQuery *updateQuery = new QSqlQuery();
    updateQuery->prepare(R"""(UPDATE defensa
        SET
            codigo=:codigo,
            nombre=:nombre,
            robustez=:robustez,
            descripcion=:descripcion,
            etapa_orden_id=:etapa_orden_id
        WHERE id=:id)"""
    );
    updateQuery->bindValue(":nombre", defensa.nombre);
    updateQuery->bindValue(":codigo", defensa.code);
    updateQuery->bindValue(":robustez", defensa.hardiness);
    updateQuery->bindValue(":id", defensa.id);
    updateQuery->bindValue(":descripcion", defensa.descripcion);
    updateQuery->bindValue(":etapa_orden_id", defensa.orderStageId);
    if (!updateQuery->exec()) {
        qDebug() << "UPDATE: " + updateQuery->lastError().text();
        return updateQuery->lastError().text();
    }

    return "";
}

void DbDefense::createForEvent(QList<DefenseDefinition>& elementsList, int event)
{
    QSqlQuery *insertQuery = new QSqlQuery();
    insertQuery->prepare("INSERT INTO defensa (suceso_iniciador_id, nombre,robustez,tipo,orden) VALUES (:evento,:nombre,:robustez,:tipo, :orden)");

    for(auto defensa : elementsList) {
        insertQuery->bindValue(":evento", event);
        insertQuery->bindValue(":nombre", defensa.nombre);
        insertQuery->bindValue(":robustez", defensa.hardiness);
        insertQuery->bindValue(":tipo", static_cast<int>(defensa.type));
        insertQuery->bindValue(":orden", defensa.bdOrden);
        if (!insertQuery->exec()) {
            throw insertQuery->lastError().databaseText();
        }
    }
}

bool DbDefense::remove(DefenseDefinition &defensa)
{
    QSqlQuery *deleteQuery = new QSqlQuery();
    deleteQuery->prepare(R"""(DELETE FROM defensa
        WHERE id=:id)"""
    );
    deleteQuery->bindValue(":id", defensa.id);
    if (!deleteQuery->exec()) {
        qDebug() << "UPDATE: " + deleteQuery->lastError().text();
        return false;
    }

    return true;
}

void DbDefense::setActive(int defenseId, bool active)
{
    QSqlQuery *updateQuery = new QSqlQuery();
    updateQuery->prepare("UPDATE defensa SET activo=:activo WHERE id=:id");
    updateQuery->bindValue(":activo", active);
    updateQuery->bindValue(":id", defenseId);
    if (!updateQuery->exec()) {
        delete updateQuery;
        throw "UPDATE: " + updateQuery->lastError().databaseText();
    }
    delete updateQuery;
}

}
