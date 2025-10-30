#ifndef MARR_H
#define MARR_H

#include <QVariant>
#include <QLibrary>
#include <QApplication>
#include <QDir>
#include <QDebug>

#include "modelo.h"

namespace Modelos {

class Marr : public Modelo
{
public:
    static const int VERSION;
    static const QString NAME;

private:
    void insertInConfiguration();
    void insertInStages();
    void insertInSubstages();
    void insertInEvents();
    void insertInDefenses();

    QLibrary *library;

    bool errorEnProceso = false;

public:
    Marr();
    ~Marr();

    bool checkDetails();
    bool createTables(QString path);
};

}

#endif // MARR_H
