#include "empty.h"

#include <QVariant>

namespace Modelos {

const int Empty::VERSION = 1;
const QString Empty::NAME = "Acelerador Lineal";

Empty::Empty()
{

}

Empty::~Empty()
{

}

bool Empty::checkDetails()
{
    return true;
}

bool Empty::createTables(QString path)
{
    initiate(path);
    createBase();

    insertInConfiguration();
    insertInStages();

    finish();

    return true;
}

void Empty::insertInConfiguration()
{
    auto conf = new QSqlQuery(database);
    conf->prepare("INSERT INTO configuracion (version, nombre) VALUES (:version, :nombre)");
    conf->bindValue(":version", VERSION);
    conf->bindValue(":nombre", NAME);
    conf->exec();
}

void Empty::insertInStages()
{
    database.transaction();
    auto stages = new QSqlQuery(database);
    stages->prepare("INSERT INTO etapas (id, nombre, orden, activo,codigo) VALUES(?,?,?,1,?)");

    stages->bindValue(0, -1);
    stages->bindValue(1, "Control de calidad");
    stages->bindValue(2, 0);
    stages->bindValue(3, "CON");
    stages->exec();
    stages->bindValue(0, -2);
    stages->bindValue(1, "Previas al proceso");
    stages->bindValue(2, -1);
    stages->bindValue(3, "PRE");
    stages->exec();
    stages->bindValue(0, -3);
    stages->bindValue(1, "Defensas de sistemas o enclavamientos");
    stages->bindValue(2, -2);
    stages->bindValue(3, "DEF");
    stages->exec();
    stages->bindValue(0, -4);
    stages->bindValue(1, "Identificación paciente");
    stages->bindValue(2, -3);
    stages->bindValue(3, "IDE");
    stages->exec();
    stages->bindValue(0, -5);
    stages->bindValue(1, "Defensas varias");
    stages->bindValue(2, -4);
    stages->bindValue(3, "DEV");
    stages->exec();

    database.commit();
}

void Empty::insertInSubstages()
{

}

void Empty::insertInEvents()
{

}

void Empty::insertInDefenses()
{

}

}
