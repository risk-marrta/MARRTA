#ifndef ORDERABLE_H
#define ORDERABLE_H

#include <QString>

struct OrderableDefinition {
    int id;
    QString nombre;
    QString descripcion;
    int orden;

    int bdOrden;
    bool modificado; // Usado para cuando se guarda el modelo en la base de datos
    bool creado;

    bool operator<(const OrderableDefinition& b){ return this->orden < b.orden; }
};

#endif // ORDERABLE_H
