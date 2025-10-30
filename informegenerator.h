#ifndef INFORMEGENERATOR_H
#define INFORMEGENERATOR_H

#include <QString>
#include <QVector>
#include <QPrinter>
#include <QTextDocument>
#include <QPageLayout>

struct Barrera {
    QString codigo;
    QString descripcion;
    QString implementacion;
    QString robustez;
};

struct Reductor {
    QString codigo;
    QString descripcion;
    QString robustez;
};

struct DatosInforme {
    QString etapa;
    QString subetapa;
    QString codigo;
    QString iniciador;
    QString descripcionError;
    QString frecuencia;
    QString consecuencias;
    QString frecuenciaCalculada;
    QString probabilidadCalculada;
    QString consecuenciasCalculadas;
    QString riesgoCalculado;
    QString contexto;
    QVector<Barrera> barreras;
    QVector<Reductor> reductoresFrecuencia;
    QVector<Reductor> reductoresConsecuencias;
};

class InformeGenerator {
public:
    static bool generarPDF(const DatosInforme& datos, const QString& rutaSalida);
    static QString generarHTML(const DatosInforme& datos);
    
private:
    static QString obtenerEstilosCSS();
    static QString generarTablaBarreras(const QVector<Barrera>& barreras);
    static QString generarTablaReductores(const QVector<Reductor>& reductores);
};

#endif // INFORMEGENERATOR_H
