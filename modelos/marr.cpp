#include "marr.h"

#include <QProgressDialog>
#include <QMessageBox>
#include <QDebug>

namespace Modelos {

const int Marr::VERSION = 2;
const QString Marr::NAME = "Acelerador Lineal";

struct Stage {
    int id;
    int order;
    QString name;
    QString description;
    bool active;
    QString code;
};

struct Substage {
    int id;
    int order;
    int stage_id;
    QString name;
    QString description;
    bool active;
};

struct Event {
    int id;
    QString name;
    QString description;
    QString code;
    int frecuency;
    int consecuence;
    int order;
    int stage_id;
    int substage_id;
    bool humanError;
    int risk;
    bool base;
    bool active;
};


struct Defense {
    int event_id;
    int hardiness;
    int type;
    QString name;
    QString description;
};

Marr::Marr()
{
    library = new QLibrary(QApplication::applicationDirPath() + QDir::separator() + "databases" + QDir::separator() + "marrDatabase");
    //qDebug() << "Ruta librería: " << QApplication::applicationDirPath() + QDir::separator() + "databases" + QDir::separator() + "marrDatabase";
    if (!library->load()) {
        QMessageBox msgBox;
        msgBox.setText("No se ha podido cargar la librería con el modelo MARR");
        msgBox.exec();

        delete library;
        library = nullptr;
    }
}

Marr::~Marr()
{
    delete library;
}

bool Marr::checkDetails()
{
    return true;
}

bool Marr::createTables(QString path)
{
    if (library == nullptr) {
        return false;
    }

    QProgressDialog dlg("Creación de nuevo base de datos para el modelo Marr", "Cancelar", 0, 100, nullptr);
    //dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowCloseButtonHint);
    dlg.setWindowModality(Qt::WindowModal);
    dlg.open();

    initiate(path);

    try {
        dlg.setLabelText("Creando estructura del modelo");
        createBase();

        dlg.setValue(10);
        dlg.setLabelText("Insertando datos de configuración");
        insertInConfiguration();

        dlg.setValue(25);
        dlg.setLabelText("Insertando datos de etapas");
        insertInStages();

        if (errorEnProceso) {
            QMessageBox msgBox;
            msgBox.setText("Error obteniendo la información del modelo");
            msgBox.exec();
            return false;
        }

        dlg.setValue(30);
        dlg.setLabelText("Insertando datos de etapas");
        insertInSubstages();

        dlg.setValue(50);
        dlg.setLabelText("Insertando datos de sucesos iniciadores");
        insertInEvents();

        dlg.setValue(70);
        dlg.setLabelText("Insertando datos de defensas");
        insertInDefenses();
    } catch (QString msg) { }

    finish();

    dlg.close();

    return true;
}

void Marr::insertInConfiguration()
{
    auto conf = new QSqlQuery(database);
    conf->prepare("INSERT INTO configuracion (version, nombre) VALUES (:version, :nombre)");
    conf->bindValue(":version", VERSION);
    conf->bindValue(":nombre", NAME);
    conf->exec();
}

void Marr::insertInStages()
{
    if (library != nullptr) {
         typedef QList<Stage> (*GetStages)();
         GetStages getStages = (GetStages)library->resolve("stages");
         if (getStages) {
             auto stages = getStages();
             //qDebug() << "Etapas " << stages.size();

             if (stages.size() > 0) {
                 auto stagesTable = new QSqlQuery(database);
                 stagesTable->prepare("INSERT INTO etapas (id, nombre, descripcion, orden, activo,codigo) VALUES(?,?,?,?,?,?)");
                 int lastId = 1;
                 for (auto stage : stages) {
                     stagesTable->bindValue(0, stage.id);
                     stagesTable->bindValue(1, stage.name);
                     stagesTable->bindValue(2, stage.description);
                     stagesTable->bindValue(3, stage.order);
                     stagesTable->bindValue(4, stage.active);
                     stagesTable->bindValue(5, stage.code);
                     stagesTable->exec();

                     lastId = stage.id;
                 }

                 stagesTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='etapas'");
                 stagesTable->bindValue(0, lastId + 1);
                 stagesTable->exec();

                 delete stagesTable;
             } else {
                 errorEnProceso = true;
             }
         } else {
            qDebug() << "No se ha encontrado la función";
         }
    }
}

void Marr::insertInSubstages()
{
    try {
        if (library != nullptr) {
            typedef QList<Substage> (*GetSubstages)();
            GetSubstages getSubStages = (GetSubstages)library->resolve("substages");
            if (getSubStages) {
                auto substages = getSubStages(); //qDebug() << "Num substages " << substages.size();

                auto substagesTable = new QSqlQuery(database);
                substagesTable->prepare("INSERT INTO subetapas (id,nombre,descripcion,orden,activo,etapa_id) VALUES (?,?,?,?,?,?)");
                int lastId = 1;
                for (auto substage : substages) {
                    substagesTable->bindValue(0, substage.id);
                    substagesTable->bindValue(1, substage.name);
                    substagesTable->bindValue(2, substage.description);
                    substagesTable->bindValue(3, substage.order);
                    substagesTable->bindValue(4, substage.active);
                    substagesTable->bindValue(5, substage.stage_id);
                    substagesTable->exec();

                    lastId = substage.id;
                }

                substagesTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='subetapas'");
                substagesTable->bindValue(0, lastId + 1);
                substagesTable->exec();

                delete substagesTable;
            } else { qDebug() << "Error en función substages"; }
        }
    }catch (std::bad_alloc bad) {
        qDebug() << bad.what();
    }
}

void Marr::insertInEvents()
{
    try {
        if (library != nullptr) {
            typedef QList<Event> (*GetEvents)();
            GetEvents getEvents = (GetEvents)library->resolve("events");
            if (getEvents) {
                QList<Event> events = getEvents(); //qDebug() << "Num events " << events.size();

                auto eventsTable = new QSqlQuery(database);
                eventsTable->prepare("INSERT INTO suceso_iniciador (id,nombre,descripcion,codigo,frecuencia,consecuencia,orden,etapa_id,subetapa_id,error_humano,riesgo,base,activo) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");
                int lastId = 1;
                for (auto event : events) {
                    eventsTable->bindValue(0, event.id);
                    eventsTable->bindValue(1, event.name);
                    eventsTable->bindValue(2, event.description);
                    eventsTable->bindValue(3, event.code);
                    eventsTable->bindValue(4, event.frecuency);
                    eventsTable->bindValue(5, event.consecuence);
                    eventsTable->bindValue(6, event.order);
                    eventsTable->bindValue(7, event.stage_id);
                    if (event.substage_id == 0) {
                        eventsTable->bindValue(8, QVariant::fromValue(nullptr));
                    } else {
                        eventsTable->bindValue(8, event.substage_id);
                    }
                    eventsTable->bindValue(9, event.humanError);
                    eventsTable->bindValue(10, event.risk);
                    eventsTable->bindValue(11, event.base);
                    eventsTable->bindValue(12, event.active);
                    eventsTable->exec();

                    lastId = event.id;
                }

                eventsTable->prepare("UPDATE SQLITE_SEQUENCE SET seq=? WHERE name='suceso_iniciador'");
                eventsTable->bindValue(0, lastId + 1);
                eventsTable->exec();

                delete eventsTable;
            } else { qDebug() << "Error en función events"; }
        }
    }catch (std::bad_alloc bad) {
        qDebug() << bad.what();
    }
}

void Marr::insertInDefenses()
{
    try {
        if (library != nullptr) {
            typedef QList<Defense> (*GetDefenses)();
            GetDefenses getDefenses = (GetDefenses)library->resolve("defenses");
            if (getDefenses) {
                auto defenses = getDefenses(); //qDebug() << "Num defenses " << defenses.size();

                auto defensesTable = new QSqlQuery(database);
                defensesTable->prepare("INSERT INTO defensa (suceso_iniciador_id,nombre,descripcion,robustez,tipo,orden,base,activo) VALUES (?,?,?,?,?,1,1,1)");
                //int lastId = 1;
                for (auto defense : defenses) {
                    defensesTable->bindValue(0, defense.event_id);
                    defensesTable->bindValue(1, defense.name);
                    defensesTable->bindValue(2, defense.description);
                    defensesTable->bindValue(3, defense.hardiness);
                    defensesTable->bindValue(4, defense.type);
                    defensesTable->exec();
                }

                delete defensesTable;
            }
        } else { qDebug() << "Error en función defenses"; }
    }catch (std::bad_alloc bad) {
        qDebug() << bad.what();
    }
}

}
