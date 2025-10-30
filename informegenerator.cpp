#include "informegenerator.h"
#include <QCoreApplication>
#include <QDate>
#include <QObject>

bool InformeGenerator::generarPDF(const DatosInforme& datos, const QString& rutaSalida) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(rutaSalida);
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    
    QTextDocument document;
    document.setHtml(generarHTML(datos));
    document.print(&printer);
    
    return true;
}

QString InformeGenerator::obtenerEstilosCSS() {
    return R"(
        <style>
            @page {
                size: A4;
                margin: 15mm;
            }
            
            body {
                font-family: 'Calibri', 'Segoe UI', Arial, sans-serif;
                font-size: 10pt;
                color: #000000;
                line-height: 1.4;
                margin: 0;
                padding: 0;
            }
            
            .header-etapa {
                background-color: #4F81BD;
                color: white;
                padding: 8px 15px;
                font-weight: bold;
                font-size: 11pt;
                margin-bottom: 0;
                text-align: right;
                letter-spacing: 0.5px;
            }
            
            .seccion-iniciador {
                background-color: #D9D9D9;
                padding: 8px 15px;
                font-weight: bold;
                font-size: 10pt;
                margin-bottom: 0;
            }
            
            .recuadro-descripcion {
                border: 1px solid #000000;
                padding: 12px;
                margin-bottom: 10px;
                background-color: white;
                font-size: 10pt;
            }
            
            .barra-info {
                background-color: #4F81BD;
                color: white;
                padding: 8px 15px;
                font-weight: bold;
                font-size: 10pt;
                margin: 0;
            }
            
            .barra-info-calculada {
                background-color: #70AD47;
                color: white;
                padding: 8px 15px;
                font-weight: bold;
                font-size: 10pt;
                margin: 0;
                text-align: right;
            }
            
            .riesgo-rb { color: #FFFFFF; font-weight: bold; background-color: #808080; padding: 2px 6px; }  /* Gray for RB */
            .riesgo-rm { color: #FFFFFF; font-weight: bold; background-color: #70AD47; padding: 2px 6px; }  /* Green for RM */
            .riesgo-ra { color: #000000; font-weight: bold; background-color: #FFA500; padding: 2px 6px; }  /* Orange for RA */
            .riesgo-rma { color: #FFFFFF; font-weight: bold; background-color: #FF0000; padding: 2px 6px; } /* Red for RMA */
            
            .parrafo-contexto {
                margin: 15px 0;
                text-align: justify;
                font-size: 10pt;
            }
            
            .titulo-seccion {
                color: #4F81BD;
                font-weight: bold;
                font-size: 11pt;
                margin: 20px 0 10px 0;
                padding-bottom: 5px;
                border-bottom: 2px solid #4F81BD;
            }
            
            table {
                width: 100%;
                border-collapse: collapse;
                margin: 10px 0 20px 0;
                font-size: 10pt;
            }
            
            table.tabla-barreras td {
                border: 1px solid #000000;
                padding: 8px;
                vertical-align: top;
            }
            
            table.tabla-barreras td:first-child {
                width: 80px;
                text-align: center;
                font-weight: bold;
                background-color: #F2F2F2;
            }
            
            table.tabla-barreras td:nth-child(2) {
                width: 35%;
            }
            
            table.tabla-barreras td:nth-child(3) {
                width: 45%;
            }
            
            table.tabla-barreras td:nth-child(4) {
                width: 100px;
                text-align: center;
                background-color: #F2F2F2;
            }
            
            table.tabla-reductores {
                border: 1px solid #000000;
            }
            
            table.tabla-reductores td {
                border-bottom: 1px solid #D9D9D9;
                padding: 8px;
            }
            
            table.tabla-reductores td:first-child {
                width: 80px;
                text-align: center;
                font-weight: bold;
                border-right: 1px solid #000000;
                background-color: #F2F2F2;
            }
            
            table.tabla-reductores td:nth-child(3) {
                width: 100px;
                text-align: center;
                border-left: 1px solid #D9D9D9;
                background-color: #F2F2F2;
            }
            
            table.tabla-reductores tr:last-child td {
                border-bottom: none;
            }
            
            .page-break {
                page-break-after: always;
            }
        </style>
    )";
}

QString InformeGenerator::generarHTML(const DatosInforme& datos) {
    // Escape HTML special characters
    QString etapaEscaped = QString(datos.etapa).toHtmlEscaped();
    QString subetapaEscaped = QString(datos.subetapa).toHtmlEscaped();
    QString codigoEscaped = QString(datos.codigo).toHtmlEscaped();
    QString iniciadorEscaped = QString(datos.iniciador).toHtmlEscaped();
    QString descripcionEscaped = QString(datos.descripcionError).toHtmlEscaped();
    QString frecuenciaEscaped = QString(datos.frecuencia).toHtmlEscaped();
    QString consecuenciasEscaped = QString(datos.consecuencias).toHtmlEscaped();
    QString contextoEscaped = QString(datos.contexto).toHtmlEscaped();
    
    // Translatable labels
    QString labelEtapa = QCoreApplication::translate("InformeGenerator", "ETAPA:");
    QString labelSubetapa = QCoreApplication::translate("InformeGenerator", "SUBETAPA:");
    QString labelIniciador = QCoreApplication::translate("InformeGenerator", "INICIADOR:");
    QString labelCodigo = QCoreApplication::translate("InformeGenerator", "CÓDIGO:");
    QString labelFrecuencia = QCoreApplication::translate("InformeGenerator", "FRECUENCIA:");
    QString labelConsecuencias = QCoreApplication::translate("InformeGenerator", "CONSECUENCIAS:");
    QString labelFrecuenciaCalc = QCoreApplication::translate("InformeGenerator", "F. CALC:");
    QString labelProbabilidadCalc = QCoreApplication::translate("InformeGenerator", "P. CALC:");
    QString labelConsecuenciasCalc = QCoreApplication::translate("InformeGenerator", "C. CALC:");
    QString labelRiesgoCalc = QCoreApplication::translate("InformeGenerator", "RIESGO CALC:");
    QString labelBarreras = QCoreApplication::translate("InformeGenerator", "BARRERAS");
    QString labelReductoresFrecuencia = QCoreApplication::translate("InformeGenerator", "REDUCTORES DE FRECUENCIA");
    QString labelReductoresConsecuencias = QCoreApplication::translate("InformeGenerator", "REDUCTORES DE CONSECUENCIAS");
    
    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
)" + obtenerEstilosCSS() + R"(
</head>
<body>
    <div class="header-etapa" style="font-size: 12pt;">)" + etapaEscaped.toUpper();
    
    if (!subetapaEscaped.isEmpty()) {
        html += "<br>" + subetapaEscaped.toUpper();
    }
    
    html += R"(</div>
    
    <div class="seccion-iniciador" style="background-color: #dbe5f1; color: #000; font-size: 12pt;">)" + labelIniciador + " " + codigoEscaped + R"(</div>
    
    <div class="recuadro-descripcion" style="font-size: 10pt; font-weight: bold; margin-top: 0; padding-top: 8px; margin-bottom: 0;">
        )" + iniciadorEscaped + R"(
    </div>
    
    <div class="barra-info" style="display: grid; grid-template-columns: auto 1fr; gap: 0 20px; padding: 8px 15px; margin-top: 0;">
        <span style="grid-column: 1;">)" + QCoreApplication::translate("InformeGenerator", "VALORES BASE:") + R"(</span>
        <span style="grid-column: 2; text-align: right;">)" + labelFrecuencia + " " + frecuenciaEscaped + "    -    " + labelConsecuencias + " " + consecuenciasEscaped + R"(</span>
    </div>
)";

    // Add calculated values if available
    if (!datos.frecuenciaCalculada.isEmpty() || !datos.probabilidadCalculada.isEmpty() || 
        !datos.consecuenciasCalculadas.isEmpty() || !datos.riesgoCalculado.isEmpty()) {
        
        QString rightSide;
        if (!datos.frecuenciaCalculada.isEmpty()) {
            rightSide += labelFrecuencia + " " + QString(datos.frecuenciaCalculada).toHtmlEscaped();
        }
        if (!datos.probabilidadCalculada.isEmpty()) {
            if (!rightSide.isEmpty()) rightSide += "    -    ";
            rightSide += QCoreApplication::translate("InformeGenerator", "PROBABILIDAD:") + " " + QString(datos.probabilidadCalculada).toHtmlEscaped();
        }
        if (!datos.consecuenciasCalculadas.isEmpty()) {
            if (!rightSide.isEmpty()) rightSide += "    -    ";
            rightSide += labelConsecuencias + " " + QString(datos.consecuenciasCalculadas).toHtmlEscaped();
        }
        if (!datos.riesgoCalculado.isEmpty()) {
            if (!rightSide.isEmpty()) rightSide += "    -    ";
            
            // Determine color class based on risk level
            QString riesgoClass = "riesgo-rb";  // Default gray
            QString riesgo = datos.riesgoCalculado.toUpper();
            if (riesgo.contains("RMA") || riesgo.contains("MUY ALTO")) {
                riesgoClass = "riesgo-rma";  // Red
            } else if (riesgo.contains("RA") || riesgo.contains("ALTO")) {
                riesgoClass = "riesgo-ra";  // Orange
            } else if (riesgo.contains("RM") || riesgo.contains("MEDIO")) {
                riesgoClass = "riesgo-rm";  // Green
            }
            
            rightSide += QCoreApplication::translate("InformeGenerator", "RIESGO:") + " <span class=\"" + riesgoClass + "\">" + QString(datos.riesgoCalculado).toHtmlEscaped() + "</span>";
        }
        
        html += R"(
    <div class="barra-info" style="display: grid; grid-template-columns: auto 1fr; gap: 0 20px; padding: 8px 15px; margin-top: 0; background-color: #4F81BD;">
        <span style="grid-column: 1;">)" + QCoreApplication::translate("InformeGenerator", "VALORES CALCULADOS:") + R"(</span>
        <span style="grid-column: 2; text-align: right;">)" + rightSide + R"(</span>
    </div>
)";
    }
    
    html += R"(    
    <div class="parrafo-contexto" style="margin-top: 15px;">
        )" + contextoEscaped + R"(
    </div>
)";

    // Sección BARRERAS
    if (!datos.barreras.isEmpty()) {
        html += R"(
    <div class="titulo-seccion">)" + labelBarreras + R"(</div>
    )" + generarTablaBarreras(datos.barreras);
    }
    
    // Sección REDUCTORES DE FRECUENCIA
    if (!datos.reductoresFrecuencia.isEmpty()) {
        html += R"(
    <div class="titulo-seccion">)" + labelReductoresFrecuencia + R"(</div>
    )" + generarTablaReductores(datos.reductoresFrecuencia);
    }
    
    // Sección REDUCTORES DE CONSECUENCIAS
    if (!datos.reductoresConsecuencias.isEmpty()) {
        html += R"(
    <div class="titulo-seccion">)" + labelReductoresConsecuencias + R"(</div>
    )" + generarTablaReductores(datos.reductoresConsecuencias);
    }
    
    html += R"(
</body>
</html>
)";
    
    return html;
}

QString InformeGenerator::generarTablaBarreras(const QVector<Barrera>& barreras) {
    QString tabla = R"(<table style="width: 100%; border-collapse: collapse; margin-top: 10px; background-color: white;">)";
    
    for (const auto& barrera : barreras) {
        tabla += QString(R"(
        <tr style="font-size: 10pt; background-color: white;">
            <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap;">%1</td>
            <td style="border: 1px solid #000; padding: 8px; text-align: left;">%2</td>
            <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap;">%3</td>
        </tr>
        )").arg(QString(barrera.codigo).toHtmlEscaped())
           .arg(QString(barrera.descripcion).toHtmlEscaped())
           .arg(QString(barrera.robustez).toHtmlEscaped());
    }
    
    tabla += "</table>";
    return tabla;
}

QString InformeGenerator::generarTablaReductores(const QVector<Reductor>& reductores) {
    QString tabla = R"(<table style="width: 100%; border-collapse: collapse; margin-top: 10px; background-color: white;">)";
    
    for (const auto& reductor : reductores) {
        tabla += QString(R"(
        <tr style="font-size: 10pt; background-color: white;">
            <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap;">%1</td>
            <td style="border: 1px solid #000; padding: 8px; text-align: left;">%2</td>
            <td style="border: 1px solid #000; padding: 8px; width: 1%; white-space: nowrap;">%3</td>
        </tr>
        )").arg(QString(reductor.codigo).toHtmlEscaped())
           .arg(QString(reductor.descripcion).toHtmlEscaped())
           .arg(QString(reductor.robustez).toHtmlEscaped());
    }
    
    tabla += "</table>";
    return tabla;
}
