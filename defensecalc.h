#ifndef DEFENSECALC_H
#define DEFENSECALC_H

#include <vector>
#include <utility>
#include "eventdefinition.h"

/**
 * @brief The DefenseCalc class
 *
 * Cálculos con las defensas.
 *
 * Although the calculation is the same in all three cases,
 * they've been differentiated in case changes are added in the future
 */
class DefenseCalc
{
public:
    DefenseCalc(const DefenseCalc &) = delete;
    DefenseCalc & operator = (const DefenseCalc &) = delete;

protected:
    DefenseCalc () = default;
    ~DefenseCalc () = default;

public:
    static int getFrecuencyReducer(std::vector<std::pair<int, int>>);
    /**
     * @brief getBarriers
     * @param std::vector<std::pair<int, int>> Conjunto de barreras aplicables al evento.
     * Consta de un par de valores que representan el tipo y la dureza de la barrera.
     * @return
     */
    static int getBarriers(std::vector<std::pair<int, int>>);
    static int getConsequencyReducer(std::vector<std::pair<int, int>>);

    static int calcFrecuencyLevel(const EventDefinition &event, int reducer);
    static int calcFrecuencyLevel(int startPoint, bool humanError, int reducer);
    static int calcProbabilityLevel(const EventDefinition &event, int reducer);
    static int calcProbabilityLevel(int reducer);
    static int calcConsequencyLevel(const EventDefinition &event, int reducer);
    static int calcConsequencyLevel(int startPoint, int reducer);
};

#endif // DEFENSECALC_H
