#ifndef MATRIZRIESGO_H
#define MATRIZRIESGO_H

#include <array>
#include <map>
#include <QString>

class MatrizRiesgo
{
public:
    MatrizRiesgo();

    int riesgo(std::size_t probabilidad, std::size_t frecuencia, std::size_t consecuencia) const;
    int riesgoReferencia(std::size_t frecuencia, std::size_t consecuencia) const;

    static const std::map<int, QString> Riesgos;
    static const std::map<int, QString> Frecuencias;
    static const std::map<int, QString> Probabilidades;
    static const std::map<int, QString> Consecuencias;
    static const std::map<int, QString> Durezas;

    static const std::map<QString, int> InversoRiesgos;
    static const std::map<QString, int> InversoFrecuencias;
    static const std::map<QString, int> InversoProbabilidades;
    static const std::map<QString, int> InversoConsecuencias;
    static const std::map<QString, int> InversoDurezas;

private:
    std::array<std::array<std::array<int, 4>, 4>, 4>  Matriz;

private:
    void inicializarMatrizRiesgo();
};

#endif // MATRIZRIESGO_H
