#include "defensedefinition.h"

#include <QDataStream>

DefenseDefinition::DefenseDefinition(): changed{false} {

}

DefenseDefinition::DefenseDefinition(DefenseDescription dd, int event)
{
    nombre = dd.name;
    this->descripcion = dd.definition;
    this->type = dd.type;
    this->hardiness = dd.hardiness;
    this->event = event;
    this->active = true;
    this->changed = true;

}

DefenseDefinition* DefenseDefinition::setName(const QString &_name)
{
    this->nombre = _name;
    this->changed = true;
    return this;
}
DefenseDefinition* DefenseDefinition::setHardiness(int _hardiness)
{
    this->hardiness = _hardiness;
    this->changed = true;
    return this;
}

void DefenseDefinition::changeState() {
    active = !active;
    changed = true;
}

QDataStream &operator<<(QDataStream &stream, const DefenseDefinition &dd)
{
    stream << dd.nombre << dd.descripcion << dd.hardiness << static_cast<int>(dd.type) << dd.orden;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DefenseDefinition &dd)
{
    int tipo;
    stream >> dd.nombre >> dd.descripcion >> dd.hardiness >> tipo >> dd.orden;
    switch (tipo) {
        case 0:
            dd.type = DefenseType::FrecuencyReducer;
            break;
        case 1:
            dd.type = DefenseType::Barrier;
            break;
        case 2:
            dd.type = DefenseType::ConsequenceReducer;
            break;
    }
    return stream;
}
