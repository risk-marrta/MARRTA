#include "dbelementstage.h"

#include <QSqlDriver>
#include <QDebug>

namespace Database {
    const std::vector<QString> DbElementStage::HeaderTitles  = {
        "Etapa",
        "Riesgo Muy Alto (RMA)",
        "Riesgo Alto (RA)",
        "Riesgo Medio (RM)",
        "Riesgo Bajo (RB)",
        "Total"
    };

    DbElementStage::DbElementStage(): DbElement<EtapaDefinition>("etapas")
    {
    }

    void DbElementStage::list(QList<EtapaDefinition>& elementsList, bool todasLasEtapas)
    {
        elementsList.clear();

        QString sql;
        if (todasLasEtapas) {
            sql = "SELECT id,nombre,descripcion,orden,codigo,(SELECT group_concat(nombre, \"|\") FROM subetapas WHERE etapa_id=e.id) FROM etapas e ORDER BY orden";
        } else {
            sql = "SELECT id,nombre,descripcion,orden,codigo,(SELECT group_concat(nombre, \"|\") FROM subetapas WHERE etapa_id=e.id) FROM etapas e WHERE id>0 ORDER BY orden";
        }

        QSqlQuery qry;
        if (!qry.exec(sql))
        {
            qDebug() << "(" << qry.lastError().nativeErrorCode() << ")" << qry.lastError().text();
            DbException exception("Error leyendo las etapas");
            exception.raise();
            return;
        }

        QStringList etapas;

        int orden = 0;
        while(qry.next()) {
            EtapaDefinition dbModel;
            dbModel.id = qry.value(0).toInt();
            dbModel.nombre = qry.value(1).toString();
            dbModel.descripcion = qry.value(2).toString();
            dbModel.bdOrden = qry.value(3).toInt();
            dbModel.orden = orden++;
            dbModel.modificado = false;
            dbModel.creado = false;
            dbModel.codigo = qry.value(4).toString();
            QString subetapas = qry.value(5).toString();
            if (!subetapas.isEmpty()) {
                dbModel.subetapas = subetapas.split('|');
            }

            elementsList << dbModel;
        }
    }

    /**
     * @brief DbElementStage::listNamesCodes list all the stages.
     * It returns a map with the id as index and a pair that represent the name and code.
     * @return
     */
    std::map<int, Database::Orders> DbElementStage::listNamesCodes()
    {
        std::map<int, Database::Orders> elements;
        auto qry = new QSqlQuery();
        if (qry->exec(R"""(SELECT e.id,
                  e.nombre,
                  e.codigo,
                  e.orden,
                  (
                      SELECT COALESCE(MAX(orden), 0)
                        FROM defensa
                       WHERE etapa_orden_id = e.id
                  ) maxOrden
             FROM etapas e
                  LEFT JOIN
                  defensa ON defensa.etapa_orden_id = e.id
            GROUP BY e.id
            ORDER BY e.orden)""")
        ) {
            while (qry->next()) {
                elements[qry->value(0).toInt()] = {
                        qry->value(1).toString().toStdString(),
                        qry->value(2).toString().toStdString(),
                        qry->value(3).toInt(),
                        qry->value(4).toInt()
                };
            }
        }

        return elements;
    }

    int DbElementStage::getNextAvailableDefenseCode(QString subcode)
    {
        auto qry = new QSqlQuery();
        if (qry->exec("SELECT CAST(MAX(distinct(SUBSTR(codigo, -3))) AS INTEGER) + 1 FROM defensa WHERE codigo LIKE '" + subcode + "%'")) {
            qry->next();
            return qry->value(0).toInt();
        }
        return 1;
    }

    /**
     * @brief ordenCodigoEtapa devuelve el próximo orden y código de una etapa
     * @return
     */
    QPair<int, QString> DbElementStage::ordenCodigoEtapa(int etapaId)
    {
        QSqlQuery qry;
        if (qry.exec(R"""(SELECT
                  e.codigo,
                  (
                      SELECT COALESCE(MAX(orden), 0)
                        FROM defensa
                       WHERE etapa_orden_id = e.id
                  ) maxOrden
             FROM etapas e
                  LEFT JOIN
                  defensa ON defensa.etapa_orden_id = e.id
            WHERE e.id=:idEtapa)""")
        ) {
            qry.bindValue(":idEtapa", etapaId);
            if (qry.exec()) {
                qry.next();
                return QPair<int, QString> {
                  qry.value(0).toInt(),
                  qry.value(1).toString()
                };
            }
        }

        return {0, QString {}};
    }

    QString DbElementStage::getNextAvailableCode(int stageId)
    {
           auto qry = new QSqlQuery();
           qry->prepare("SELECT codigo FROM etapas WHERE id=:stageId");
           qry->bindValue(":stageId", stageId);
           if (qry->exec()){
               qry->next();
               QString codigo = qry->value(0).toString();
               QString l = QString::number(codigo.size());

               auto qryCode = new QSqlQuery();
               QString sentence = QString("SELECT CAST(substr(codigo, %1) AS int) nn FROM suceso_iniciador WHERE codigo LIKE '%2-%%'").arg(l, codigo);
               if (qryCode->exec(sentence)) {
                   qryCode->next();
                   int currentCode = qryCode->value(0).toInt();
                   delete qryCode;
                   return codigo + '-' + QStringLiteral("%1").arg(currentCode + 1, 2, 10, QLatin1Char('0'));
               }
               delete qryCode;

               return codigo + "-01";
           }

           delete qry;

           return "";
    }

    void DbElementStage::update(QList<EtapaDefinition>& elementsList, bool updateAll)
    {
        QSqlQuery *insertQuery = new QSqlQuery();
        insertQuery->prepare("INSERT INTO etapas (nombre, descripcion,orden,codigo) VALUES (:nombre,:descripcion,:orden,:codigo)");
        QSqlQuery *updateQuery = new QSqlQuery();
        updateQuery->prepare("UPDATE etapas SET nombre=:nombre,descripcion=:descripcion,orden=:orden,codigo=:codigo WHERE id=:id");
        for(auto etapa : elementsList) {
            if (etapa.creado == true) {
                insertQuery->bindValue(":nombre", etapa.nombre);
                insertQuery->bindValue(":descripcion", etapa.descripcion);
                insertQuery->bindValue(":orden", etapa.orden);
                insertQuery->bindValue(":codigo", etapa.codigo);
                if (!insertQuery->exec()) {
                    throw insertQuery->lastError().databaseText();
                }
            } else if ((etapa.modificado == true) || (updateAll == true)) {
                updateQuery->bindValue(":nombre", etapa.nombre);
                updateQuery->bindValue(":descripcion", etapa.descripcion);
                updateQuery->bindValue(":orden", etapa.orden);
                updateQuery->bindValue(":codigo", etapa.codigo);
                updateQuery->bindValue(":id", etapa.id);
                if (!updateQuery->exec()) {
                    throw "UPDATE: " + updateQuery->lastError().databaseText();
                }
            }
        }
        delete insertQuery;
        delete updateQuery;
    }

    /**
     * @brief DbElementStage::summary
     * Get a summary of the risks of the model.
     * The summary get the saved information. The information that hasn't been saved is not checked.
     *
     * @return
     */
    std::vector<Summary> DbElementStage::summary(std::unordered_map<std::string, std::vector<CompleteEventDefinition> > *)
    {
        std::vector<Summary> vSummaries;
        QSqlQuery *namesQuery = new QSqlQuery();
        if (namesQuery->exec(R"""(select e.nombre,e.orden,riesgo_3.riesgo_3,riesgo_2.riesgo_2,riesgo_1.riesgo_1,riesgo_0.riesgo_0
             from etapas as e
             LEFT JOIN (
                 SELECT etapa_id, count(riesgo) riesgo_0 FROM suceso_iniciador WHERE riesgo=0 AND completado=1 group by etapa_id
             ) AS riesgo_0  ON riesgo_0.etapa_id=e.id
             LEFT JOIN (
                 SELECT etapa_id, count(riesgo) riesgo_1 FROM suceso_iniciador WHERE riesgo=1 AND completado=1  group by etapa_id
             ) AS riesgo_1  ON riesgo_1.etapa_id=e.id
             LEFT JOIN (
                 SELECT etapa_id, count(riesgo) riesgo_2 FROM suceso_iniciador WHERE riesgo=2 AND completado=1  group by etapa_id
             ) AS riesgo_2  ON riesgo_2.etapa_id=e.id
             LEFT JOIN (
                 SELECT etapa_id, count(riesgo) riesgo_3 FROM suceso_iniciador WHERE riesgo=3  AND completado=1  group by etapa_id
             ) AS riesgo_3  ON riesgo_3.etapa_id=e.id
             where e.id > 0
             ORDER BY e.orden)""")) {

            while(namesQuery->next()) {
                Summary summaryElement = {
                    namesQuery->value(0).toString(),
                    namesQuery->value(1).toInt() - 1,
                    namesQuery->value(2).toInt(),
                    namesQuery->value(3).toInt(),
                    namesQuery->value(4).toInt(),
                    namesQuery->value(5).toInt()
                };
                vSummaries.push_back(summaryElement);
            }
        } else {
            qDebug() << namesQuery->lastError().text();
        }

        delete namesQuery;
        return vSummaries;
    }
}
