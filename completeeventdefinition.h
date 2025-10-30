#ifndef COMPLETEEVENTDEFINITION_H
#define COMPLETEEVENTDEFINITION_H

#include <vector>

#include "defensedefinition.h"
#include "defensecalc.h"
#include "matrizriesgo.h"

/**
 * @brief The CompleteEventDefinition class
 * This struct is used in the MainWindow to read all the information about the model.
 */
class CompleteEventDefinition {

public:
    int id;
    QString nombre;
    QString descripcion;
    QString codigo;
    int etapa;
    int subetapa;
    int orden;
    int frecuencia;
    int consecuencia;
    bool base;
    bool completado;

    int frecuenciaCalculada;
    int probabilidadCalculada;
    int consecuenciaCalculada;

    bool errorHumano;

    int riesgo;
    int riesgoMinimo;
    int riesgoMaximo;

    std::vector<DefenseDefinition> reductoresFrecuencias;
    std::vector<DefenseDefinition> barreras;
    std::vector<DefenseDefinition> reductoresConsecuencias;

    CompleteEventDefinition();
    CompleteEventDefinition(CompleteEventDefinition *);

    bool operator==(int _id) const
    {
        return id == _id;
    }

    void setCompletado(bool _completado);

    void calcularReductores();

    void calcularReductoresFrecuencia();
    void calcularBarreras();
    void calcularReductoresConsecuencia();

    void calcularRiesgo(const MatrizRiesgo &);
    void calcularRiesgoMinimo(const MatrizRiesgo &);
    void calcularRiesgoMaximo(const MatrizRiesgo &);

    void toggleActive();
private:
    /*
     * Esto se usa para mostrar los informes y mostrar el riesgo cuando
     * se usan todas las barreras.
     */
    int frecuenciaMinima;
    int probabilidadMinima;
    int consecuenciaMinima;

    void calcularReductoresFrecuenciaMinimo();
    void calcularBarrerasMinimo();
    void calcularReductoresConsecuenciaMinimo();
};

#endif // COMPLETEEVENTDEFINITION_H
