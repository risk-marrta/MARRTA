#ifndef STYLES_H
#define STYLES_H

#include <QString>
#include <QColor>

namespace Styles {
    const QString WhiteLabel = "QLabel {background-color: white; color: black;}";
    const QString GreenLabel = "QLabel {background-color: #cce9ad; color: 006100;}";
    const QString YellowLabel = "QLabel {background-color: #ffeb9c; color: #e36b08;}";
    const QString RedLabel = "QLabel {background-color: #ffc7ce; color: #9c0006;}";

    const QColor WhiteBackgroundColor = {200, 200, 200, 255};
    const QColor GreenBackgroundColor = {204, 233, 173, 255};
    const QColor YellowBackgroundColor = {255, 235, 156, 255};
    const QColor RedBackgroundColor = {255, 199, 206, 255};

    // apm - colores para las gráficas
    const QColor RiesgoMuyAlto = {255,0,0};
    const QColor RiesgoAlto = {255,217,102};
    const QColor RiesgoMedio = {"#00B050"}; // {0,176,80}
    const QColor RiesgoBajo = {"#A6A6A6"}; // {166,166,166}
    // apm
}

#endif // STYLES_H
