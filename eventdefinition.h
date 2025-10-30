#ifndef EVENT_H
#define EVENT_H

#include "orderable.h"
#include "etapas.h"
#include "subetapas.h"
#include "defensedefinition.h"

class EventDefinition: public OrderableDefinition {
public:
    QString codigo;
    EtapaDefinition etapa;
    SubetapaDefinition subetapa;
    int frecuencia;
    int probabilidad;
    int consecuencia;
    bool errorHumano;
    bool base;

    EventDefinition() = default;
    ~EventDefinition() = default;
    EventDefinition(const EventDefinition &) = default;
    EventDefinition &operator=(const EventDefinition &) = default;

    friend QDataStream & operator<< (QDataStream &stream, const EventDefinition &ed);
    friend QDataStream & operator>> (QDataStream &stream, EventDefinition &ed);
};

Q_DECLARE_METATYPE(EventDefinition);

#endif // EVENT_H
