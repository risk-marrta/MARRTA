#include "completeeventdefinition.h"

#include <utility>

#include <QDebug>

#include "dbevent.h"

CompleteEventDefinition::CompleteEventDefinition()
{

}

CompleteEventDefinition::CompleteEventDefinition(CompleteEventDefinition *toCopy)
{
    id = toCopy->id;
    orden = toCopy->orden;
    barreras = toCopy->barreras;
    reductoresConsecuencias = toCopy->reductoresConsecuencias;
    reductoresFrecuencias = toCopy->reductoresFrecuencias;
    base = toCopy->base;
    codigo = toCopy->codigo;
    completado = toCopy->completado;
    consecuencia = toCopy->consecuencia;
    consecuenciaMinima = toCopy->consecuenciaMinima;
    consecuenciaCalculada = toCopy->consecuenciaCalculada;
    frecuencia = toCopy->frecuencia;
    frecuenciaMinima = toCopy->frecuenciaMinima;
    frecuenciaCalculada = toCopy->frecuenciaCalculada;
    probabilidadMinima = toCopy->probabilidadMinima;
    probabilidadCalculada = toCopy->probabilidadCalculada;
    nombre = toCopy->nombre;
    descripcion = toCopy->descripcion;
    etapa = toCopy->etapa;
    subetapa = toCopy->subetapa;
    errorHumano = toCopy->errorHumano;
    riesgo = toCopy->riesgo;
    riesgoMinimo = toCopy->riesgoMinimo;
    riesgoMaximo = toCopy->riesgoMaximo;
}

void CompleteEventDefinition::calcularReductores()
{
    calcularReductoresFrecuencia();
    calcularBarreras();
    calcularReductoresConsecuencia();
}

void CompleteEventDefinition::calcularReductoresFrecuencia()
{
    int ra[4] = {0,0,0,0};
    for(const auto & r : reductoresFrecuencias) {
        if (r.active && (r.hardiness >= 0) && (r.hardiness <= 3)) {
            ra[r.hardiness]++;
        }
    }

    std::vector<std::pair<int, int>> infoReductores;
    for(int i = 0; i < 4; i++){
        infoReductores.push_back(std::make_pair(i, ra[i]));
    }

    int reductor = DefenseCalc::getFrecuencyReducer(infoReductores);
    frecuenciaCalculada = DefenseCalc::calcFrecuencyLevel(frecuencia, errorHumano, reductor);
}

void CompleteEventDefinition::calcularBarreras()
{
    std::vector<std::pair<int, int>> infoBarreras;

    int ra[4] = {0,0,0,0};
    for(const auto & b : barreras) {
        if (b.active && (b.hardiness >= 0) && (b.hardiness <= 3)) {
            ra[b.hardiness]++;
        }
    }
    for (int i = 0; i < 4; i++) {
        infoBarreras.push_back(std::make_pair(i, ra[i]));
    }

    int reductor = DefenseCalc::getBarriers(infoBarreras);
    probabilidadCalculada = DefenseCalc::calcProbabilityLevel(reductor);
}

void CompleteEventDefinition::calcularReductoresConsecuencia()
{
    int ra[4] = {0,0,0,0};
    for(const auto & r : reductoresConsecuencias) {
        if (r.active && (r.hardiness >= 0) && (r.hardiness <= 3)) {
            ra[r.hardiness]++;
        }
    }

    std::vector<std::pair<int, int>> infoReductores;
    for(int i = 0; i < 4; i++){
        infoReductores.push_back(std::make_pair(i, ra[i]));
    }

    int reductor = DefenseCalc::getConsequencyReducer(infoReductores);
    consecuenciaCalculada = DefenseCalc::calcConsequencyLevel(consecuencia, reductor);
}

void CompleteEventDefinition::calcularRiesgo(const MatrizRiesgo &matrizRiesgo)
{
    riesgo = matrizRiesgo.riesgo(
                static_cast <std::size_t>(probabilidadCalculada),
                static_cast <std::size_t>(frecuenciaCalculada),
                static_cast <std::size_t>(consecuenciaCalculada)
    );
}

void CompleteEventDefinition::calcularReductoresFrecuenciaMinimo()
{
    int ra[4] = {0,0,0,0};
    for(const auto & r : reductoresFrecuencias) {
        if ((r.hardiness >= 0) && (r.hardiness <= 3)) {
            ra[r.hardiness]++;
        }
    }

    std::vector<std::pair<int, int>> infoReductores;
    for(int i = 0; i < 4; i++){
        infoReductores.push_back(std::make_pair(i, ra[i]));
    }

    int reductor = DefenseCalc::getFrecuencyReducer(infoReductores);
    frecuenciaMinima = DefenseCalc::calcFrecuencyLevel(frecuencia, errorHumano, reductor);
}

void CompleteEventDefinition::calcularBarrerasMinimo()
{
    std::vector<std::pair<int, int>> infoBarreras;

    int ra[4] = {0,0,0,0};
    for(const auto & b : barreras) {
        if ((b.hardiness >= 0) && (b.hardiness <= 3)) {
            ra[b.hardiness]++;
        }
    }
    for (int i = 0; i < 4; i++) {
        infoBarreras.push_back(std::make_pair(i, ra[i]));
    }

    int reductor = DefenseCalc::getBarriers(infoBarreras);
    probabilidadMinima = DefenseCalc::calcProbabilityLevel(reductor);
}

void CompleteEventDefinition::calcularReductoresConsecuenciaMinimo()
{
    int ra[4] = {0,0,0,0};
    for(const auto & r : reductoresConsecuencias) {
        if ((r.hardiness >= 0) && (r.hardiness <= 3)) {
            ra[r.hardiness]++;
        }
    }

    std::vector<std::pair<int, int>> infoReductores;
    for(int i = 0; i < 4; i++){
        infoReductores.push_back(std::make_pair(i, ra[i]));
    }

    int reductor = DefenseCalc::getConsequencyReducer(infoReductores);
    consecuenciaMinima = DefenseCalc::calcConsequencyLevel(consecuencia, reductor);
}

void CompleteEventDefinition::calcularRiesgoMinimo(const MatrizRiesgo &matrizRiesgo)
{
    calcularReductoresFrecuenciaMinimo();
    calcularBarrerasMinimo();
    calcularReductoresConsecuenciaMinimo();

    riesgoMinimo = matrizRiesgo.riesgo(
                static_cast <std::size_t>(probabilidadMinima),
                static_cast <std::size_t>(frecuenciaMinima),
                static_cast <std::size_t>(consecuenciaMinima)
    );
}

void CompleteEventDefinition::calcularRiesgoMaximo(const MatrizRiesgo &matrizRiesgo)
{
    riesgoMaximo = matrizRiesgo.riesgo(
                static_cast<std::size_t>(3),
                static_cast<std::size_t>(frecuencia),
                static_cast<std::size_t>(consecuencia)
    );
}

void CompleteEventDefinition::setCompletado(bool _completado)
{
    completado = _completado;
    Database::DbEvent::setCompleted(id, completado, riesgo);
}

void CompleteEventDefinition::toggleActive()
{
    for(auto &frecuencia: reductoresFrecuencias) {
        frecuencia.changeState();
    }
    for(auto &barrera: barreras) {
        barrera.changeState();
    }
    for(auto &consecuencia: reductoresConsecuencias) {
        consecuencia.changeState();
    }
}

