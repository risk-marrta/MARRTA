#include "importsevrra.h"

#include <QProgressDialog>
#include <QMessageBox>
#include <QHash>
#include <QCoreApplication>

namespace Modelos {

const int ImportSevrra::VERSION = 1;
const QString ImportSevrra::NAME = "Acelerador Lineal";

using namespace std;

const QHash<QString, int> frecuenceCodes{
    {"FMB", 0},
    {"FB", 1},
    {"FA", 2},
    {"FMA", 3}
};

const QHash<QString, int> consecuenceCodes{
    {"CMB", 0},
    {"CB", 1},
    {"CA", 2},
    {"CMA", 3}
};

const QHash<int, int> hardiness {
    {1, -1},
    {4, 0},
    {8, 1},
    {16, 2},
    {32, 3}
};

const QHash<int, int> defenseType {
    {1, 1},
    {2, 0},
    {3, 2}
};

ImportSevrra::ImportSevrra(const QSqlDatabase *_sevrra, int _praxisNum)
{
    sevrra = _sevrra;
    praxisNum = _praxisNum;
    userEventsError = false;
}

ImportSevrra::~ImportSevrra() { }

bool ImportSevrra::checkDetails()
{
    return true;
}

bool ImportSevrra::createTables(QString path)
{
    QProgressDialog dlg("Localización de la base de datos para el modelo Marr", "Cancelar", 0, 100, nullptr);
    dlg.setWindowModality(Qt::WindowModal);
    dlg.open();

    initiate(path);

    try {
        QCoreApplication::processEvents();
        if (dlg.wasCanceled()) return false;
        dlg.setLabelText("Creando estructura del modelo");
        createBase();

        QCoreApplication::processEvents();
        if (dlg.wasCanceled()) return false;
        dlg.setValue(10);
        dlg.setLabelText("Insertando datos de configuración");
        insertInConfiguration();

        QCoreApplication::processEvents();
        if (dlg.wasCanceled()) return false;
        dlg.setValue(25);
        dlg.setLabelText("Insertando datos de etapas");
        insertInStages();

        QCoreApplication::processEvents();
        if (dlg.wasCanceled()) return false;
        dlg.setValue(30);
        dlg.setLabelText("Insertando datos de etapas");
        insertInSubstages();

        QCoreApplication::processEvents();
        if (dlg.wasCanceled()) return false;
        dlg.setValue(50);
        dlg.setLabelText("Insertando datos de sucesos iniciadores");
        insertInEvents();

        QCoreApplication::processEvents();
        if (dlg.wasCanceled()) return false;
        dlg.setValue(70);
        dlg.setLabelText("Insertando datos de defensas");
        insertInDefenses();
    } catch (QString msg) { }

    finish();

    dlg.close();

    return true;
}

void ImportSevrra::insertInConfiguration()
{
    auto conf = new QSqlQuery(database);
    conf->prepare("INSERT INTO configuracion (version, nombre) VALUES (:version, :nombre)");
    conf->bindValue(":version", VERSION);
    conf->bindValue(":nombre", NAME);
    conf->exec();
}

void ImportSevrra::insertInStages()
{
    auto readStages = new QSqlQuery(*sevrra);
    readStages->prepare("SELECT idetapa,desc_etapa,ayuda,orden,activo FROM cat_etapa WHERE idpractica=:idpractica");
    readStages->bindValue(":idpractica", praxisNum);
    if (!readStages->exec()) {
        throw QString("Fallo en la lectura de la base de datos");
    }

    auto stagesTable = new QSqlQuery(database);
    stagesTable->prepare("INSERT INTO etapas (id, nombre, descripcion, orden, activo) VALUES(?,?,?,?,?)");
    int lastId = 1;

    while(readStages->next())  {
        lastId = readStages->value(0).toInt();
        stagesTable->bindValue(0, lastId);
        stagesTable->bindValue(1, readStages->value(1).toString());
        stagesTable->bindValue(2, readStages->value(2).toString());
        stagesTable->bindValue(3, readStages->value(3).toInt());
        stagesTable->bindValue(4, readStages->value(4).toBool());
        stagesTable->exec();

    }

    stagesTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='etapas'");
    stagesTable->bindValue(0, lastId + 1);
    stagesTable->exec();

    delete stagesTable;
}

void ImportSevrra::insertInSubstages()
{
    auto readSubstages = new QSqlQuery(*sevrra);
    readSubstages->prepare("SELECT idsubetapa,idetapa,desc_subetapa,ayuda,orden,activo FROM cat_subetapa WHERE idpractica=:idpractica");
    readSubstages->bindValue(":idpractica", praxisNum);
    if (!readSubstages->exec()) {
        throw QString("Fallo en la lectura de la base de datos");
    }

    auto substagesTable = new QSqlQuery(database);
    substagesTable->prepare("INSERT INTO subetapas (id,nombre,descripcion,orden,activo,etapa_id) VALUES (?,?,?,?,?,?)");
    int lastId = 1;
    while(readSubstages->next()) {
        lastId = readSubstages->value(0).toInt();
        substagesTable->bindValue(0, lastId);
        substagesTable->bindValue(1, readSubstages->value(2).toString());
        substagesTable->bindValue(2, readSubstages->value(3).toString());
        substagesTable->bindValue(3, readSubstages->value(4).toInt());
        substagesTable->bindValue(4, readSubstages->value(5).toBool());
        substagesTable->bindValue(5, readSubstages->value(1).toInt());
        substagesTable->exec();
    }

    substagesTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='subetapas'");
    substagesTable->bindValue(0, lastId + 1);
    substagesTable->exec();

    delete substagesTable;
}

void ImportSevrra::insertInEvents()
{
    auto readEvents = new QSqlQuery(*sevrra);
    readEvents->prepare("SELECT idsuceso,idetapa,idsubetapa,denominacion,ayuda,codigo,frecuencia,consecuencia,orden,iderror,true as base FROM cat_sucesoi WHERE idpractica=:idpractica");
    readEvents->bindValue(":idpractica", praxisNum);
    if (!readEvents->exec()) {
        throw QString("Fallo en la lectura de la base de datos");
    }

    int lastId = 1;
    lastId = insertEventsPart(readEvents, lastId);

    readEvents->prepare("SELECT idsuceso_usr,idetapa,idsubetapa,denominacion_usr,ayuda_usr,codigo_usr,frecuencia_usr,consecuencia_usr,orden_usr,iderror_usr,false as base FROM usr_sucesosi WHERE idpractica=:idpractica");
    readEvents->bindValue(":idpractica", praxisNum);
    if (!readEvents->exec()) {
        QMessageBox msgBox;
        msgBox.setText("Error al leer los eventos creados por el usuario. Se continuará únicamente con los eventos base");
        msgBox.exec();
        userEventsError = true;
    } else {
        lastId = insertEventsPart(readEvents, lastId);
    }

    auto eventsTable = new QSqlQuery(database);
    eventsTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='suceso_iniciador'");
    eventsTable->bindValue(0, lastId + 1);
    eventsTable->exec();

    delete readEvents;
    delete eventsTable;
}

int ImportSevrra::insertEventsPart(QSqlQuery *readedEvents, int lastId)
{
    auto eventsTable = new QSqlQuery(database);
    eventsTable->prepare("INSERT INTO suceso_iniciador (id,etapa_id,subetapa_id,nombre,descripcion,codigo,frecuencia,consecuencia,orden,error_humano,base) VALUES (?,?,?,?,?,?,?,?,?,?,?)");

    while (readedEvents->next()) {
        lastId = readedEvents->value(0).toInt();
        eventsTable->bindValue(0, lastId);
        eventsTable->bindValue(1, readedEvents->value(1).toInt());
        int substage = readedEvents->value(2).toInt();
        if (substage == 0) {
            eventsTable->bindValue(2, QVariant::fromValue(nullptr));
        } else {
            eventsTable->bindValue(2, substage);
        }
        eventsTable->bindValue(3, readedEvents->value(3).toString()); // Nombre
        eventsTable->bindValue(4, readedEvents->value(4).toString()); // Descripción
        eventsTable->bindValue(5, readedEvents->value(5).toString()); // Código
        eventsTable->bindValue(6, frecuenceCodes[readedEvents->value(6).toString()]); // Frecuencia
        eventsTable->bindValue(7, consecuenceCodes[readedEvents->value(7).toString()]); // Consecuencia
        eventsTable->bindValue(8, readedEvents->value(8).toInt()); // Orden
        eventsTable->bindValue(9, readedEvents->value(9).toBool()); // Error humano
        eventsTable->bindValue(10, readedEvents->value(10).toBool()); // Base
        eventsTable->exec();
    }

    delete eventsTable;

    return lastId;
}

void ImportSevrra::insertInDefenses()
{
    auto readDefenses = new QSqlQuery(*sevrra);
    readDefenses->prepare("SELECT idsuceso, descripcion, ayuda,idclas, ponderacion, true as base FROM cat_analisis ca LEFT JOIN rel_suceso_analisis AS rsa ON rsa.idanalisis = ca.idanalisis WHERE idpractica=:idpractica");
    readDefenses->bindValue(":idpractica", praxisNum);
    if (!readDefenses->exec()) {
        throw QString("Fallo en la lectura de la base de datos");
    }
    insertDefensesPart(readDefenses);

    if (userEventsError == false) {
        readDefenses->prepare("SELECT ca.idsuceso, descripcion_usr, ayuda_usr,idclass_usr, ponderacion_usr, false as base FROM usr_analisis ca LEFT JOIN rel_suceso_analisis AS rsa ON rsa.idanalisis = ca.idanalisis_usr WHERE idpractica=:idpractica");
        if (!readDefenses->exec()) {
            QMessageBox msgBox;
            msgBox.setText("Error al leer las defensas creadas por el usuario. Se continuará únicamente con las definidas en el modelo base");
            msgBox.exec();
        } else {
            insertDefensesPart(readDefenses);
        }
    }

    delete readDefenses;
}

void ImportSevrra::insertDefensesPart(QSqlQuery *readedDefenses)
{
    auto defensesTable = new QSqlQuery(database);
    defensesTable->prepare("INSERT INTO defensa (suceso_iniciador_id,nombre,descripcion,robustez,tipo,base,orden,activo) VALUES (?,?,?,?,?,?,1,1)");
    while (readedDefenses->next()) {
        defensesTable->bindValue(0, readedDefenses->value(0).toInt());
        defensesTable->bindValue(1, readedDefenses->value(1).toString());
        defensesTable->bindValue(2, readedDefenses->value(2).toString());
        defensesTable->bindValue(3, hardiness[readedDefenses->value(4).toInt()]);
        defensesTable->bindValue(4, defenseType[readedDefenses->value(3).toInt()]);
        defensesTable->bindValue(5, readedDefenses->value(5).toBool());
        defensesTable->exec();
    }

    delete defensesTable;
}

}
