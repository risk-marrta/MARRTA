#include "dbelementsubstage.h"

#include <QDebug>

namespace Database {
    void DbElementSubstage::list(QList<SubetapaDefinition>& elementsList)
    {
        auto qry = new QSqlQuery();
        qry->prepare("SELECT id,nombre,descripcion,orden,activo FROM subetapas WHERE etapa_id=:etapa ORDER BY orden");
        qry->bindValue(":etapa", _parentId);
        if (!qry->exec()) {
            throw "Error leyendo las subetapas";
        }

        QStringList etapas;
        elementsList.clear();

        int orden = 0;
        while(qry->next()) {
            SubetapaDefinition dbModel;
            dbModel.id = qry->value(0).toInt();
            dbModel.nombre = qry->value(1).toString();
            dbModel.descripcion = qry->value(2).toString();
            dbModel.bdOrden = qry->value(3).toInt();
            dbModel.activo = qry->value(4).toBool();
            dbModel.orden = orden++;
            dbModel.modificado = false;
            dbModel.creado = false;
            dbModel.etapaId = _parentId;

            elementsList << dbModel;
        }
        delete qry;
    }

    void DbElementSubstage::listAll(QList<SubetapaDefinition> *elementsList)
    {
        auto qry = new QSqlQuery();
        qry->prepare("SELECT id,nombre,descripcion,orden,activo,etapa_id FROM subetapas ORDER BY orden");
        if (!qry->exec()) {
            throw QString("Error leyendo las subetapas");
        }

        QStringList etapas;
        elementsList->clear();

        int orden = 0;
        while(qry->next()) {
            SubetapaDefinition dbModel;
            dbModel.id = qry->value(0).toInt();
            dbModel.nombre = qry->value(1).toString();
            dbModel.descripcion = qry->value(2).toString();
            dbModel.bdOrden = qry->value(3).toInt();
            dbModel.activo = qry->value(4).toBool();
            dbModel.orden = orden++;
            dbModel.etapaId = qry->value(5).toInt();
            dbModel.modificado = false;
            dbModel.creado = false;

            elementsList->append(dbModel);
        }
        delete qry;
    }

    void DbElementSubstage::update(QList<SubetapaDefinition>& elementsList, bool updateAll)
    {
        QSqlQuery *insertQuery = new QSqlQuery();
        insertQuery->prepare("INSERT INTO subetapas (etapa_id,nombre, descripcion,orden,activo) VALUES (:etapaId,:nombre, :descripcion, :orden, :activo)");
        QSqlQuery *updateQuery = new QSqlQuery();
        updateQuery->prepare("UPDATE subetapas SET nombre=:nombre, descripcion=:descripcion,orden=:orden,activo=:activo WHERE id=:id");
        for(auto element : elementsList) {

            if (element.creado == true) {
                insertQuery->bindValue(":etapaId", _parentId);
                insertQuery->bindValue(":nombre", element.nombre);
                insertQuery->bindValue(":descripcion", element.descripcion);
                insertQuery->bindValue(":orden", element.orden);
                insertQuery->bindValue(":activo", element.activo);
                if (!insertQuery->exec()) {
                    throw insertQuery->lastError().databaseText();
                }
            } else if ((element.modificado == true) || (updateAll == true)) {
                updateQuery->bindValue(":nombre", element.nombre);
                updateQuery->bindValue(":descripcion", element.descripcion);
                updateQuery->bindValue(":orden", element.orden);
                updateQuery->bindValue(":activo", element.activo);
                updateQuery->bindValue(":id", element.id);
                if (!updateQuery->exec()) {
                    qDebug() << "UPDATE: "
                             << updateQuery->lastError().text()
                             << "-" << element.nombre
                             << "-" << element.descripcion
                             << "-" << element.orden
                             << "-" << element.activo
                             << "-" << element.id;
                    throw QString("Error actualizando las subetapas");
                }
            }
        }
        delete insertQuery;
        delete updateQuery;
    }
}
