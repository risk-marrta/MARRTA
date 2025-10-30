#include "dbmanager.h"

#include <QVariant>
#include <QSqlError>
#include <QDebug>

#include <iostream>

using namespace std;

namespace Database {
    DbManager::DbManager()
    {
    
    }

    ConfiguracionModelo DbManager::checkDatabase(bool verbose)
    {
        QString query = "SELECT version,nombre,tecnica,analistas,institucion,fecha,descripcion,observaciones FROM configuracion DESC LIMIT 1;";
        QSqlQuery *qry = new QSqlQuery();
        if (!qry->exec(query) || !qry->first())
        {
            if (verbose) {
                cout << "El último modelo de riesgo usado no se encuentra disponible" << endl;
            }
            delete qry;
            throw "El último modelo de riesgo usado no se encuentra disponible";
        }

        int databaseVersion = qry->value(0).toInt();
        if (databaseVersion != DbVersion) {
            delete qry;
            throw "La versión de este fichero de modelo no está soportado.";
        }

        ConfiguracionModelo configuracion;
        configuracion.nombre = qry->value(1).toString();
        configuracion.tecnica = qry->value(2).toString();
        configuracion.analistas = qry->value(3).toString();
        configuracion.institucion = qry->value(4).toString();
        QVariant fecha = qry->value(5);
        if (!fecha.isNull()) {
            configuracion.fecha = fecha.toString();
        } else {
            QSqlQuery *updateDate = new QSqlQuery();
            updateDate->exec("UPDATE configuracion SET fecha=date('now')");
            delete updateDate;
        }
        configuracion.descripcion = qry->value(6).toString();
        configuracion.observaciones = qry->value(7).toString();

        delete qry;

        return configuracion;
    }

    template <class T> void DbManager::elementsRemove(const QList<T>& listToRemove, std::string tableName)
    {
        Q_UNUSED(tableName);
        cout << "Antes de borrar" << endl;
        QSqlQuery *deleteQuery = new QSqlQuery();
        deleteQuery->prepare("DELETE FROM etapas WHERE id=:id");
        for (auto etapa : listToRemove) {
            deleteQuery->bindValue(":id", etapa.id);
            if (!deleteQuery->exec()) {
                delete deleteQuery;
                throw deleteQuery->lastError().driverText();
            }
        }
        delete deleteQuery;
    }

    void DbManager::saveConfiguration(const ConfiguracionModelo &conf)
    {
        QSqlQuery *qry = new QSqlQuery();
        qry->prepare("UPDATE configuracion SET nombre=:nombre, fecha=:fecha, tecnica=:tecnica,analistas=:analistas,institucion=:institucion,descripcion=:descripcion,observaciones=:observaciones");
        qry->bindValue(":nombre", conf.nombre);
        qry->bindValue(":tecnica", conf.tecnica);
        qry->bindValue(":fecha", conf.fecha);
        qry->bindValue(":analistas", conf.analistas);
        qry->bindValue(":institucion", conf.institucion);
        qry->bindValue(":descripcion", conf.descripcion);
        qry->bindValue(":observaciones", conf.observaciones);
        if (!qry->exec()) {
            QMessageBox msgBox;
            msgBox.setText("No se han podido guardar las modificaciones del modelo" + qry->lastError().text());
            msgBox.exec();
        }

        delete qry;
    }

    QList<IniciadoresXEtapa> DbManager::iniciadoresXEtapa()
    {
        auto qry = new QSqlQuery();
        if (!qry->exec(R"""(SELECT count(*),si.etapa_id,si.subetapa_id,etapas.nombre,subetapas.nombre
          FROM suceso_iniciador AS si
               LEFT JOIN etapas ON etapas.id = si.etapa_id
               LEFT JOIN subetapas ON subetapas.id = si.subetapa_id
         GROUP BY si.etapa_id, si.subetapa_id)""")
        ) {
            qDebug() << qry->lastError().text();
            throw "Error obteniendo la lista de iniciadores";
        }

        QList<IniciadoresXEtapa> iniciadores;
        while(qry->next()) {
            IniciadoresXEtapa iniciador;
            iniciador.numero = qry->value(0).toInt();
            iniciador.etapaId = qry->value(1).toInt();
            iniciador.subetapaId = qry->value(2).toInt();
            iniciador.etapa = qry->value(3).toString();
            iniciador.subetapa = qry->value(4).toString();

            iniciadores << iniciador;
        }

        return iniciadores;
    }

    QList<OcurrenciasXEtapa> DbManager::frecuenciasXEtapa()
    {
        auto qry = new QSqlQuery();
        if (!qry->exec(R"""(
               SELECT
                   fmb.fmb, fb.fb,fm.fm,fa.fa,fmb.etapa_id,fmb.subetapa_id,fmb.et,fmb.se
               FROM
                   (SELECT count( * ) fmb,si.etapa_id,si.subetapa_id,etapas.nombre et,subetapas.nombre se FROM suceso_iniciador AS si LEFT JOIN etapas ON etapas.id = si.etapa_id LEFT JOIN subetapas ON subetapas.id = si.subetapa_id WHERE si.frecuencia=0 GROUP BY si.etapa_id, si.subetapa_id) fmb
               LEFT JOIN
                   (SELECT count( * ) fb,si.etapa_id,si.subetapa_id FROM suceso_iniciador AS si WHERE si.frecuencia=1 GROUP BY si.etapa_id, si.subetapa_id) fb
                   ON fmb.etapa_id=fb.etapa_id AND fmb.subetapa_id=fb.subetapa_id
               LEFT JOIN
                   (SELECT count( * ) fm,si.etapa_id,si.subetapa_id FROM suceso_iniciador AS si WHERE si.frecuencia=2 GROUP BY si.etapa_id, si.subetapa_id) fm
                   ON fmb.etapa_id=fm.etapa_id AND fmb.subetapa_id=fm.subetapa_id
               LEFT JOIN
                   (SELECT count( * ) fa,si.etapa_id,si.subetapa_id FROM suceso_iniciador AS si WHERE si.frecuencia=3 GROUP BY si.etapa_id, si.subetapa_id) fa
                   ON fmb.etapa_id=fa.etapa_id AND fmb.subetapa_id=fa.subetapa_id
        )""")) {
            qDebug() << qry->lastError().text();
            throw "Error obteniendo la lista de iniciadores";
        }

        QList<OcurrenciasXEtapa> frecuencias;
        while(qry->next()) {
            OcurrenciasXEtapa frecuencia;
            frecuencia.etapaId = qry->value(4).toInt();
            frecuencia.subetapaId = qry->value(5).toInt();
            frecuencia.etapa = qry->value(6).toString();
            frecuencia.subetapa = qry->value(7).toString();
            frecuencia.o0 = qry->value(0).toInt();
            frecuencia.o1 = qry->value(1).toInt();
            frecuencia.o2 = qry->value(2).toInt();
            frecuencia.o3 = qry->value(3).toInt();

            frecuencias << frecuencia;
        }

        return frecuencias;
    }

    QList<OcurrenciasXEtapa> DbManager::consecuenciasXEtapa()
    {
        auto qry = new QSqlQuery();
        if (!qry->exec(R"""(
                       WITH
                         cmb AS (
                           SELECT
                             si.etapa_id,
                             si.subetapa_id,
                             etapas.nombre et,
                             subetapas.nombre se
                           FROM suceso_iniciador AS si
                           LEFT JOIN etapas ON etapas.id  =  si.etapa_id
                           LEFT JOIN subetapas ON subetapas.id  =  si.subetapa_id
                           GROUP BY si.etapa_id, si.subetapa_id
                         ),
                         si AS (
                           SELECT
                             count( * ) FILTER (WHERE si.consecuencia = 1) AS cmb,
                             count( * ) FILTER (WHERE si.consecuencia = 1) AS cb,
                             count( * ) FILTER (WHERE si.consecuencia = 2) AS cm,
                             count( * ) FILTER (WHERE si.consecuencia = 3) AS ca,
                             si.etapa_id,
                             si.subetapa_id
                           FROM suceso_iniciador AS si
                           GROUP BY si.etapa_id, si.subetapa_id
                         )
                       SELECT
                         coalesce(si.cmb, 0),
                         coalesce(si.cb, 0),
                         coalesce(si.cm, 0),
                         coalesce(si.ca, 0),
                         cmb.etapa_id,
                         cmb.subetapa_id,
                         cmb.et,
                         cmb.se
                       FROM cmb
                       LEFT JOIN si ON cmb.etapa_id = si.etapa_id AND cmb.subetapa_id = si.subetapa_id
        )""")) {
            qDebug() << qry->lastError().text();
            throw "Error obteniendo la lista de iniciadores";
        }

        QList<OcurrenciasXEtapa> ocurrencias;
        while(qry->next()) {
            OcurrenciasXEtapa ocurrencia;
            ocurrencia.etapaId = qry->value(4).toInt();
            ocurrencia.subetapaId = qry->value(5).toInt();
            ocurrencia.etapa = qry->value(6).toString();
            ocurrencia.subetapa = qry->value(7).toString();
            ocurrencia.o0 = qry->value(0).toInt();
            ocurrencia.o1 = qry->value(1).toInt();
            ocurrencia.o2 = qry->value(2).toInt();
            ocurrencia.o3 = qry->value(3).toInt();

            ocurrencias << ocurrencia;
        }

        return ocurrencias;
    }

    QList<OcurrenciasXEtapa> DbManager::riesgosXEtapa()
    {
        auto qry = new QSqlQuery();
        if (!qry->exec(R"""(
               SELECT
                     coalesce(count( * ) FILTER (WHERE si.consecuencia = 1), 0) AS cmb,
                     coalesce(count( * ) FILTER (WHERE si.consecuencia = 1), 0) AS cb,
                     coalesce(count( * ) FILTER (WHERE si.consecuencia = 2), 0) AS cm,
                     coalesce(count( * ) FILTER (WHERE si.consecuencia = 3), 0) AS ca,
                     si.etapa_id,
                     si.subetapa_id,
                     etapas.nombre et,
                     subetapas.nombre se
                   FROM suceso_iniciador AS si
                   LEFT JOIN etapas ON etapas.id  =  si.etapa_id
                   LEFT JOIN subetapas ON subetapas.id  =  si.subetapa_id
                   GROUP BY si.etapa_id, si.subetapa_id
        )""")) {
            qDebug() << qry->lastError().text();
            throw "Error obteniendo la lista de iniciadores";
        }

        QList<OcurrenciasXEtapa> riesgos;
        while(qry->next()) {
            OcurrenciasXEtapa riesgo;
            riesgo.etapaId = qry->value(4).toInt();
            riesgo.subetapaId = qry->value(5).toInt();
            riesgo.etapa = qry->value(6).toString();
            riesgo.subetapa = qry->value(7).toString();
            riesgo.o0 = qry->value(0).toInt();
            riesgo.o1 = qry->value(1).toInt();
            riesgo.o2 = qry->value(2).toInt();
            riesgo.o3 = qry->value(3).toInt();

            riesgos << riesgo;
        }

        return riesgos;
    }

    QList<QPair<QString, int>> DbManager::barrerasEnIniciadores()
    {
        auto qry = new QSqlQuery();
        if (!qry->exec(R"""(
               SELECT nombre, COUNT(*) FROM defensa
               GROUP BY nombre
           )""")
         ) {
            qDebug() << qry->lastError().text();
            throw "Error obteniendo la lista de iniciadores";
         }

        QList<QPair<QString, int>> barreras;
        while(qry->next()) {
            barreras << QPair<QString,int>(qry->value(0).toString(), qry->value(1).toInt());
        }

        return barreras;
    }

    void DbManager::stageList(QList<EtapaDefinition>& stageList)
    {
        auto qry = new QSqlQuery();
        if (!qry->exec("SELECT id,nombre,descripcion,orden FROM etapas ORDER BY orden"))
        {
            qDebug() << qry->lastError().text();
            throw "Error leyendo las etapas";
        }

        QStringList etapas;
        stageList.clear();

        int orden = 0;
        while(qry->next()) {
            EtapaDefinition dbModel;
            dbModel.id = qry->value(0).toInt();
            dbModel.nombre = qry->value(1).toString();
            dbModel.descripcion = qry->value(2).toString();
            dbModel.bdOrden = qry->value(3).toInt();
            dbModel.orden = orden++;
            dbModel.modificado = false;
            dbModel.creado = false;

            stageList << dbModel;

            //cout << "Etapa: " << dbModel.nombre.toStdString() << endl;
        }
        delete qry;
    }

    void DbManager::stageUpdate(QList<EtapaDefinition>& stageList)
    {
        Q_UNUSED(stageList);
    }


    int DbManager::numeroBarreras()
    {
        QString query = "SELECT COUNT(DISTINCT(nombre)) FROM defensa";
        auto qry = new QSqlQuery();
        if (qry->exec(query))
        {
            qry->next();
            return qry->value(0).toInt();
        } else {
            qDebug() << qry->lastError().text();
        }

        return 0;
    }

    std::tuple<int, QVector<DefensaXSucesos>> DbManager::sucesosXBarreras()
    {
        QVector<DefensaXSucesos> defensas;
        QStringList fcr;
        QVarLengthArray<int,3> fcrArray(3);
        int totalSucesos = 0;
        QString query = R"""(SELECT
                DISTINCT(defensa.nombre),
                COUNT(defensa.nombre),
                GROUP_CONCAT('(' || si.codigo || ') ' || si.nombre, ';') AS sucesos,
                GROUP_CONCAT(si.frecuencia || si.consecuencia || si.riesgo, ';') AS fcr
            FROM defensa
            INNER JOIN suceso_iniciador si ON si.id=defensa.suceso_iniciador_id
            GROUP BY defensa.nombre)""";
        QSqlQuery qry(query);
        qry.exec();

        while(qry.next()) {
            DefensaXSucesos defensa;
            defensa.nombre = qry.value(0).toString();
            defensa.numSucesos = qry.value(1).toInt();
            totalSucesos += defensa.numSucesos;
            defensa.sucesos = qry.value(2).toString().split(';');
            fcr = qry.value(3).toString().split(';');
            for(auto elements : fcr){
                fcrArray[0] = elements.at(0).digitValue();
                fcrArray[1] = elements.at(1).digitValue();
                fcrArray[2] = elements.at(2).digitValue();
                defensa.fcr.append(fcrArray);
            }
            defensas << defensa;
        }

        return {totalSucesos, defensas};
    }

    QList<DefenseDescription> DbManager::listUniqueDefenses()
    {
        QSqlQuery listQuery(R"""(SELECT
                codigo,
                nombre,
                descripcion,
                robustez,
                activo,
                tipo
            FROM defensa
            GROUP BY nombre
            ORDER BY nombre)""");
        listQuery.exec();
        QList<DefenseDescription> defenseList;
        while(listQuery.next()){
            DefenseDescription defense;
            defense.code = listQuery.value(0).toString();
            defense.name = listQuery.value(1).toString();
            defense.definition = listQuery.value(2).toString();
            defense.hardiness = listQuery.value(3).toInt();
            defense.active = true;
            defense.type = DefenseType(listQuery.value(5).toInt()); // Fixed: tipo is at index 5, not 6

            defenseList.append(defense);
        }

        return defenseList;
    }

    QList<DefenseDefinition> DbManager::listDefensesByCodeName(int defenseType)
    {
        QSqlQuery listQuery;
        listQuery.prepare(R"""(SELECT
                codigo,
                nombre,
                descripcion,
                robustez,
                activo,
                etapa_orden_id
            FROM defensa
            WHERE tipo=:type
            GROUP BY codigo,nombre
            ORDER BY etapa_orden_id, codigo)""");
        listQuery.bindValue(":type", defenseType);
        listQuery.exec();
        //qDebug() << listQuery.lastError();
        QList<DefenseDefinition> defenseList;
        auto defenseToSave = DefenseType(defenseType);
        while(listQuery.next()){
            DefenseDefinition defense;
            defense.code = listQuery.value(0).toString();
            defense.nombre = listQuery.value(1).toString();
            defense.descripcion = listQuery.value(2).toString();
            defense.hardiness = listQuery.value(3).toInt();
            defense.active = listQuery.value(4).toBool();
            defense.orderStageId = listQuery.value(5).toInt();
            defense.type = defenseToSave;

            defenseList.append(defense);
        }

        return defenseList;
    }
}
