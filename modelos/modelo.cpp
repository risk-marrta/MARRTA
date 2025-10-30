#include "modelo.h"

namespace Modelos {

Modelo::~Modelo(){}

void Modelo::initiate(QString filepath)
{
    const QString DRIVER("QSQLITE");
    if(!QSqlDatabase::isDriverAvailable(DRIVER))
    {
        throw QString("Error en el conector de la base de datos");
    }
    database = QSqlDatabase::addDatabase(DRIVER, "new_database");
    database.setDatabaseName(filepath);
}

void Modelo::createBase()
{
    if (!database.open())
    {
        throw QString("La base de datos no puede crearse");
    }

    QSqlDatabase::database().transaction();

    QSqlQuery query(database);

    if (!query.exec("CREATE TABLE configuracion (version INTEGER, nombre TEXT, tecnica TEXT, analistas TEXT, institucion TEXT, fecha VARCHAR(10), descripcion TEXT, observaciones TEXT);")) {
        database.close();
        throw QString("No se ha podido crear la tabla de configuración");
    }

    if (!query.exec(R"""(CREATE TABLE etapas (
                    id          INTEGER      NOT NULL
                                             PRIMARY KEY AUTOINCREMENT,
                    nombre      VARCHAR (50) NOT NULL
                                             UNIQUE,
                    descripcion TEXT,
                    orden       INTEGER      NOT NULL
                                             DEFAULT 1,
                    activo      BOOLEAN      NOT NULL
                                             DEFAULT (1),
                    codigo      CHAR (3)
                );)""")
    ) {
        throw QString("No se ha podido crear la tabla de etapas");
    }

    if (!query.exec(R"""(CREATE TABLE subetapas (
                    id          INTEGER NOT NULL
                                        PRIMARY KEY AUTOINCREMENT,
                    nombre      TEXT    NOT NULL,
                    descripcion TEXT,
                    orden       INTEGER,
                    activo      INTEGER NOT NULL
                                        DEFAULT 1,
                    etapa_id    INTEGER NOT NULL,
                    FOREIGN KEY (
                        etapa_id
                    )
                    REFERENCES etapas (id) ON DELETE CASCADE
                                           ON UPDATE CASCADE
                );)""")) {
        throw QString("No se ha podido crear la tabla de subetapas");
    }

    if (!query.exec(R"""(CREATE TABLE suceso_iniciador (
                    id           INTEGER     PRIMARY KEY AUTOINCREMENT,
                    nombre       TEXT        NOT NULL,
                    descripcion  TEXT,
                    codigo       TEXT        NOT NULL,
                    frecuencia   VARCHAR (3) NOT NULL,
                    consecuencia VARCHAR (3) NOT NULL,
                    etapa_id     INTEGER     NOT NULL,
                    subetapa_id  INTEGER,
                    error_humano BOOLEAN     NOT NULL
                                             DEFAULT false,
                    riesgo       VARCHAR (3),
                    base         BOOLEAN     NOT NULL
                                             DEFAULT (0),
                    activo       BOOLEAN     NOT NULL
                                             DEFAULT (1),
                    completado   BOOLEAN     DEFAULT (false),
                    CONSTRAINT unique_id UNIQUE (id)
                );)""")) {
        throw QString("No se ha podido crear la tabla de sucesos iniciadores");


    }

    if (!query.exec(R"""(CREATE TABLE defensa (
                    id                  INTEGER       PRIMARY KEY AUTOINCREMENT,
                    suceso_iniciador_id INTEGER       REFERENCES suceso_iniciador (id) ON DELETE CASCADE
                                                                                       ON UPDATE CASCADE,
                    "codigo"            VARCHAR (25),
                    nombre              VARCHAR (250) NOT NULL,
                    descripcion         TEXT,
                    robustez            INT           NOT NULL
                                                      DEFAULT (0)
                                                      CHECK (robustez >= -1 AND
                                                             robustez < 4),
                    tipo                INTEGER       CHECK (tipo >= 0 AND
                                                             tipo < 3)
                                                      NOT NULL
                                                      DEFAULT (0),
                    activo              BOOLEAN       DEFAULT (0),
                    etapa_orden_id      INTEGER       REFERENCES etapas (id) ON DELETE SET NULL
                                                                             ON UPDATE CASCADE
                );)""")) {       
        throw QString("No se ha podido crear la tabla de defensas");
    }
}

void Modelo::finish()
{
    QSqlDatabase::database().commit();

    database.close();
}

void Modelo::insertData(Tables table)
{
    switch (table) {
        case Tables::Configuracion:
            insertInConfiguration();
            break;
        case Tables::Etapas:
            insertInStages();
            break;
        case Tables::Subetapas:
            insertInSubstages();
            break;
        case Tables::SucesoIniciador:
            insertInEvents();
            break;
        case Tables::Defensas:
            insertInDefenses();
            break;
    }
}

}
