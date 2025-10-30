#ifndef EMPTY_H
#define EMPTY_H

#include "modelo.h"

namespace Modelos {

class Empty : public Modelo
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

public:
    Empty();
    ~Empty();

    bool checkDetails();
    bool createTables(QString path);
};

}

#endif // EMPTY_H
