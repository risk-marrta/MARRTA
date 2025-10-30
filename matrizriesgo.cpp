#include "matrizriesgo.h"

#include <QDebug>

MatrizRiesgo::MatrizRiesgo()
{
    inicializarMatrizRiesgo();
}

const std::map<int, QString> MatrizRiesgo::Riesgos {
    {0, "RB"},
    {1, "RM"},
    {2, "RA"},
    {3, "RMA"}
};
const std::map<int, QString> MatrizRiesgo::Frecuencias {
    {0, "FMB"},
    {1, "FB"},
    {2, "FM"},
    {3, "FA"}
};
const std::map<int, QString> MatrizRiesgo::Probabilidades {
    {0, "PMB"},
    {1, "PB"},
    {2, "PM"},
    {3, "PA"}
};
const std::map<int, QString> MatrizRiesgo::Consecuencias {
    {0, "CB"},
    {1, "CM"},
    {2, "CA"},
    {3, "CMA"}
};
const std::map<int, QString> MatrizRiesgo::Durezas {
    {-1, "Muy blanda"},
    {0, "Blanda"},
    {1, "Normal"},
    {2, "Robusta"},
    {3, "Muy robusta"}
};

const std::map<QString, int> MatrizRiesgo::InversoRiesgos{
    {"RB", 0},
    {"RM", 1},
    {"RA", 2},
    {"RMA", 3}
};
const std::map<QString, int> MatrizRiesgo::InversoFrecuencias{
    {"FMB", 0},
    {"FB", 1},
    {"FM", 2},
    {"FA", 3}
};
const std::map<QString, int> MatrizRiesgo::InversoProbabilidades{
    {"PMB", 0},
    {"PB", 1},
    {"PM", 2},
    {"PA", 3}
};
const std::map<QString, int> MatrizRiesgo::InversoConsecuencias{
    {"CB", 0},
    {"CM", 1},
    {"CA", 2},
    {"CMA", 3}
};
const std::map<QString, int> MatrizRiesgo::InversoDurezas{
    {"Muy blanda", -1},
    {"Blanda", 0},
    {"Normal", 1},
    {"Robusta", 2},
    {"Muy robusta", 3}
};

/**
 * @brief MatrizRiesgo::inicializarMatrizRiesgo
 *
 * Esta montado así para simplificar la visualización.
 */
void MatrizRiesgo::inicializarMatrizRiesgo()
{
    // Probabilidad Muy Baja
    std::array<std::array<int, 4>, 4> pmb;
        // Frecuencia muy Baja
        std::array<int, 4> pmb_fmb = {0,0,0,1};
        pmb[0] = pmb_fmb;
        // Frecuencia baja
        std::array<int, 4> pmb_fb = {0,0,0,1};
        pmb[1] = pmb_fb;
        // Frecuencia media
        std::array<int, 4> pmb_fm = {0,1,1,1};
        pmb[2] = pmb_fm;
        // Frecuencia alta
        std::array<int, 4> pmb_fa = {0,1,1,2};
        pmb[3] = pmb_fa;
    Matriz[0] = pmb;

    // Probabilidad Baja
    std::array<std::array<int, 4>, 4> pb;
        // Frecuencia muy baja
        std::array<int, 4> pb_fmb = {0,1,1,1};
        pb[0] = pb_fmb;
        // Frecuencia baja
        std::array<int, 4> pb_fb = {0,1,1,1};
        pb[1] = pb_fb;
        // Frecuencia media
        std::array<int, 4> pb_fm = {0,1,2,2};
        pb[2] = pb_fm;
        // Frecuencia alta
        std::array<int, 4> pb_fa = {0,1,2,2};
        pb[3] = pb_fa;
    Matriz[1] = pb;

    // Probabilidad Media
    std::array<std::array<int, 4>, 4> pm;
        // Frecuencia muy baja
        std::array<int, 4> pm_fmb = {0,1,1,2};
        pm[0] = pm_fmb;
        // Frecuencia baja
        std::array<int, 4> pm_fb = {0,1,2,2};
        pm[1] = pm_fb;
        // Frecuencia media
        std::array<int, 4> pm_fm = {1,1,2,2};
        pm[2] = pm_fm;
        // Frecuencia alta
        std::array<int, 4> pm_fa = {1,2,2,3};
        pm[3] = pm_fa;
    Matriz[2] = pm;

    // Probabilidad Alta
    std::array<std::array<int, 4>, 4> pa;
        // Frecuencia muy baja
        std::array<int, 4> pa_fmb = {1,1,2,2};
        pa[0] = pa_fmb;
        // Frecuencia baja
        std::array<int, 4> pa_fb = {1,1,2,2};
        pa[1] = pa_fb;
        // Frecuencia media
        std::array<int, 4> pa_fm = {1,2,2,3};
        pa[2] = pa_fm;
        // Frecuencia alta
        std::array<int, 4> pa_fa = {1,2,3,3};
        pa[3] = pa_fa;
    Matriz[3] = pa;
}

int MatrizRiesgo::riesgo(std::size_t probabilidad, std::size_t frecuencia, std::size_t consecuencia) const
{
    //qDebug() << probabilidad << "." << frecuencia << "." << consecuencia << " = " << Matriz[probabilidad][frecuencia][consecuencia];

    return Matriz[probabilidad][frecuencia][consecuencia];
}

int MatrizRiesgo::riesgoReferencia(std::size_t frecuencia, std::size_t consecuencia) const
{
    return Matriz[3][frecuencia][consecuencia];
}
