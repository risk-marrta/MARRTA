#ifndef MODELO_H
#define MODELO_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Modelos {

class Modelo
{
protected:
    QSqlDatabase database;

protected:
    void initiate(QString filepath);
    void createBase();
    void finish();

    /**
     * @brief update This method is empty. It's a stub for a future update algorithm.
     * @param from
     * @param to
     */
    void update(int from, int to);

    virtual void insertInConfiguration() = 0;
    virtual void insertInStages() = 0;
    virtual void insertInSubstages() = 0;
    virtual void insertInEvents() = 0;
    virtual void insertInDefenses() = 0;

public:
    virtual ~Modelo() = 0;

    enum class Tables {Configuracion,Etapas,Subetapas,SucesoIniciador,Defensas};

    virtual bool checkDetails() = 0;
    virtual bool createTables(QString path) = 0;

    void insertData(Tables table);
};

}

#endif // MODELO_H
