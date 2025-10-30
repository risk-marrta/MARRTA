#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <tuple>
#include <QString>
#include <QSqlQuery>
#include <QDateTime>

#include <QList>

#include "etapas.h"
#include "defensedefinition.h"

namespace Database {
    struct ConfiguracionModelo {
        QString version;
        QString nombre;
        QString tecnica;
        QString analistas;
        QString institucion;
        QString fecha;
        QString descripcion;
        QString observaciones;
    };

    struct IniciadoresXEtapa
    {
        int etapaId;
        int subetapaId;
        QString etapa;
        QString subetapa;
        int numero = 0;
    };

    struct OcurrenciasXEtapa {
        int etapaId;
        int subetapaId;
        QString etapa;
        QString subetapa;
        int o0 = 0;
        int o1 = 0;
        int o2 = 0;
        int o3 = 0;
    };

    struct DefensaXSucesos {
        QString nombre;
        QStringList sucesos;
        int numSucesos;
        QVector<QVarLengthArray<int,3>> fcr;
    };

    class DbManager
    {
    private:
        constexpr static int DbVersion = 1;

    public:
        DbManager();

        /**
         * @brief Removes a list of elements from the table
         *
         * @param const QList<T>& List of elements to remove (only the id field is required)
         * @param std::string Name of the table to remove
         */
        template <class T> void static elementsRemove(const QList<T>&, std::string);

        /**
         * @brief checkDatabase check if the currently selected database is correct
         *
         * @param bool If this value is true,
         * @return std::tuple<int, std::string> Version of the database and title.
         *
         * @throws When the version of the database is not allowed
         */
        ConfiguracionModelo static checkDatabase(bool);

        /**
         * @brief saveConfiguration persists the configuration of the model to the database
         *
         * @param const ConfiguracionModelo & Information to save to the model
         */
        void static saveConfiguration(const ConfiguracionModelo &);

        /**
         * @brief stageList returns the list of stages of the database
         *
         * @params QList<EtapaDefinition>& Where the readed stages must be saved
         *
         * @return QList<IniciadoresXEtapa>
         *
         * @throws If an error has been found executing the sentence
         */
        void static stageList(QList<EtapaDefinition>&);

        void static stageUpdate(QList<EtapaDefinition>&);

        QList<IniciadoresXEtapa> iniciadoresXEtapa();

        /**
         * @brief frecuenciasXEtapa devuelve el número de frecuencias en eventos en cada etapa/subetapa
         * @return QList<OcurrenciasXEtapa>
         */
        QList<OcurrenciasXEtapa> frecuenciasXEtapa();

        /**
         * @brief consecuenciasXEtapa devuelve el número de consecuencias en eventos en cada etapa/subetapa
         * @return QList<OcurrenciasXEtapa>
         */
        QList<OcurrenciasXEtapa> consecuenciasXEtapa();

        /**
         * @brief riesgosXEtapa devuelve el número de riesgos en eventos en cada etapa/subetapa
         * @return QList<OcurrenciasXEtapa>
         */
        QList<OcurrenciasXEtapa> riesgosXEtapa();

        /**
         * @brief barrerasEnIniciadores lista el número de ocurrencias de cada barrera
         * @return QList<QPair<QString, int>>
         */
        QList<QPair<QString, int>> barrerasEnIniciadores();

        /**
         * @brief numeroBarreras devuelve el número de barreras en el modelo
         * @return
         */
        int static numeroBarreras();

        /**
         * @brief SucesosXBarreras devuelve las barreras
         * junto con los sucesos en los que se encuentra presente y sus detalles.
         * @return QVector<DefensaXSucesos>
         */
        std::tuple<int, QVector<DefensaXSucesos>> static sucesosXBarreras();

        /**
         * @brief Devuelve un listado de defensas únicas. Solo la información descriptiva
         * @return QList<DefenseDescription> Lista de defensas únicas
         * @date 2020-08-27
         * @see defensedefinition.h
         */
        QList<DefenseDescription> static listUniqueDefenses();

        /**
         * @brief Devuelve un listado de defensas. Solo la información descriptiva
         * @return QList<DefenseDescription> Lista de defensas únicas
         * @date 2020-08-27
         * @see defensedefinition.h
         */
        QList<DefenseDefinition> static listDefensesByCodeName(int);
    };
}

#endif // DBMANAGER_H
