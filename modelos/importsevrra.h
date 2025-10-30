#ifndef IMPORTSEVRRA_H
#define IMPORTSEVRRA_H

#include <QVariant>

#include "modelo.h"

namespace Modelos {

class ImportSevrra : public Modelo
{
public:
    static const int VERSION;
    static const QString NAME;

private:
    const QSqlDatabase *sevrra;
    int praxisNum;
    bool userEventsError;

private:
    void insertInConfiguration();
    void insertInStages();
    void insertInSubstages();
    void insertInEvents();
    /**
     * @brief insertEventsPart the previous method read the base and the user
     * events and call this to insert in the database.
     *
     * @param QSqlQuery *readedEvents   Result of retrieve the data
     * @param int lastId                Last readed id
     */
    int insertEventsPart(QSqlQuery *readedEvents, int lastId);
    void insertInDefenses();
    void insertDefensesPart(QSqlQuery *readedDefenses);


public:
    /**
     * @brief ImportSevrra
     * Create the instance using a pointer to the MySQL connector and the number of the praxis to import.
     */
    ImportSevrra(const QSqlDatabase *, int);
    ~ImportSevrra();

    bool checkDetails();
    bool createTables(QString path);
};

}

#endif // IMPORTSEVRRA_H
