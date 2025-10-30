#include "defensecalc.h"

#include <qdebug.h>
#include <math.h>

int DefenseCalc::getFrecuencyReducer(std::vector<std::pair<int, int> > elements)
{
    int frecuencyReducer = 1;
    for (auto reducer : elements) {
        //qDebug() << "Reductor frecuencia " << reducer.first << "-" << reducer.second;
        switch(reducer.first){
            case 0:
                frecuencyReducer *= pow(4, reducer.second);
                break;
            case 1:
                frecuencyReducer *= pow(8, reducer.second);
                break;
            case 2:
                frecuencyReducer *= pow(16, reducer.second);
                break;
            case 3:
                frecuencyReducer *= pow(32, reducer.second);
                break;
        }
    }

    return frecuencyReducer;
}

int DefenseCalc::getBarriers(std::vector<std::pair<int, int> > elements)
{
    int barrier = 1;
    for (auto reducer : elements) {
        //qDebug() << "Barrera " << reducer.first << "-" << reducer.second;
        switch(reducer.first){
            case 0:
                barrier *= pow(4, reducer.second);
                break;
            case 1:
                barrier *= pow(8, reducer.second);
                break;
            case 2:
                barrier *= pow(16, reducer.second);
                break;
            case 3:
                barrier *= pow(32, reducer.second);
                break;
        }
    }
    //qDebug() << "Para devolver: " << barrier;
    return barrier;
}

int DefenseCalc::getConsequencyReducer(std::vector<std::pair<int, int> > elements)
{
    int consecuenceReducer = 1;
    for (auto reducer : elements) {
        //qDebug() << "Reductor consecuencia " << reducer.first << "-" << reducer.second << "-" << pow(4,reducer.second);
        switch(reducer.first){
            case 0:
                consecuenceReducer *= pow(4, reducer.second);
                break;
            case 1:
                consecuenceReducer *= pow(8, reducer.second);
                break;
            case 2:
                consecuenceReducer *= pow(16, reducer.second);
                break;
            case 3:
                consecuenceReducer *= pow(32, reducer.second);
                break;
        }
    }

    return consecuenceReducer;
}

int DefenseCalc::calcFrecuencyLevel(const EventDefinition &event, int reducer)
{
    return calcFrecuencyLevel(event.frecuencia, event.errorHumano, reducer);
}

int DefenseCalc::calcFrecuencyLevel(int startPoint, bool humanError, int reducer)
{
    switch (startPoint)
    {
        case 1: // FB
            if ((humanError == false)
                    && (reducer >= 32)
            ) {
                return 0; // FMB
            }
            break;
        case 2: // FM
            if ((humanError == false)
                    && (reducer > 64)
            ) {
                return 0; // FMB
            } else if (reducer >= 32) {
                return 1; // FB
            }
            break;
        case 3: // FA
            if (reducer > 64) {
                return 1; // FB
            } else if (reducer >= 32) {
                return 2; // FM
            }
            break;
    }

    return startPoint;
}
int DefenseCalc::calcProbabilityLevel(const EventDefinition &event, int reducer)
{
    return calcProbabilityLevel(reducer);
}

int DefenseCalc::calcProbabilityLevel(int reducer)
{
    //qDebug() << "Probabilidad: " << reducer;
    if (reducer <= 1) { // Just in case
        return 3;
    } else if (reducer < 32) {
        return 2;
    } else if (reducer <= 64) {
        return 1;
    } else {
        return 0;
    }
    return 3;
 }

int DefenseCalc::calcConsequencyLevel(const EventDefinition &event, int reducer)
{
    return calcConsequencyLevel(event.consecuencia, reducer);
}

int DefenseCalc::calcConsequencyLevel(int startPoint, int reducer) {
    switch (startPoint) {
        case 2:
            if (reducer > 64) {
                return 1;
            }
            break;
        case 3:
            if (reducer > 64) {
                return 2;
            }
    }

    return startPoint;
}
