#ifndef DBELEMENT_H
#define DBELEMENT_H

#include <string>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>

#include "orderable.h"

namespace Database {
    template<class T> class DbElement
    {
    private:
        QString _tableName;

    public:
        DbElement() {}
        DbElement(QString tableName): _tableName(tableName) {}

        void list(QList<T>&);

        void remove(const QList<T>& listToRemove){
            //qDebug() << "Dentro de borrar ->" << listToRemove.count();
            QSqlQuery *deleteQuery = new QSqlQuery();
            deleteQuery->prepare("DELETE FROM " + _tableName + " WHERE id=:id");

            for (auto element : listToRemove) {
                //qDebug() << "DELETE FROM " << _tableName << " WHERE id=:id [" << element.id << "]";
                deleteQuery->bindValue(":id", element.id);
                if (!deleteQuery->exec()) {
                    qDebug() << deleteQuery->lastError();
                    throw deleteQuery->lastError().driverText();
                }
            }
            delete deleteQuery;
        }

        void update(QList<T>&, bool);
    };
}

#endif // DBELEMENT_H
