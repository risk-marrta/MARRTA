#ifndef DEFENSEDEFINITION_H
#define DEFENSEDEFINITION_H

#include "orderable.h"

enum class DefenseType {
    FrecuencyReducer,
    Barrier,
    ConsequenceReducer
};

/**
  * @brief Guarda la definición de una defensa cuando solo se necesita lo básico.
  * @date 2020-08-27 Para DbDefense::listUnique
 */
struct DefenseDescription {
    QString code;
    QString name;
    QString definition;
    int hardiness;
    int orderStageId;
    bool active;
    DefenseType type;
};

class DefenseDefinition: public OrderableDefinition {
public:
    int event;
    QString code;
    int hardiness;
    DefenseType type;
    bool active;
    bool changed;
    int orderStageId;
    QString orderStage;

    DefenseDefinition();
    DefenseDefinition(DefenseDescription, int);

    DefenseDefinition* setName(const QString &_name);
    DefenseDefinition* setHardiness(int _hardiness);

    void changeState();

    friend QDataStream & operator<< (QDataStream &stream, const DefenseDefinition &dd);
    friend QDataStream & operator>> (QDataStream &stream, DefenseDefinition &dd);
};

#endif // DEFENSEDEFINITION_H
