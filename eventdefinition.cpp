#include "eventdefinition.h"

QDataStream & operator<< (QDataStream &stream, const EventDefinition &ed)
{
    stream << ed.codigo
           << ed.nombre
           << ed.descripcion
           << ed.frecuencia
           << ed.consecuencia
           << ed.errorHumano;
    return stream;
}

QDataStream & operator>> (QDataStream &stream, EventDefinition &ed)
{
    stream >> ed.codigo >> ed.nombre >> ed.descripcion >> ed.frecuencia >> ed.consecuencia >> ed.errorHumano;
    return stream;
}
