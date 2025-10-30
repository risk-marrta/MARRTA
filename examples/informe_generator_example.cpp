/**
 * Example: Using the HTML Report Generator (InformeGenerator)
 * 
 * This example demonstrates how to use the InformeGenerator class
 * to create professional PDF reports from event data.
 * 
 * Note: This is a code example for documentation purposes.
 * In practice, data would come from the MARRTA database.
 */

#include "informegenerator.h"
#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Example 1: Simple report with basic data
    {
        DatosInforme datos;
        
        // Basic event information
        datos.etapa = "Etapa de Producción";
        datos.iniciador = "Fallo en el sistema de control automático";
        datos.descripcionError = "El sistema de control automático presenta fallos "
                                 "intermitentes durante las operaciones de producción, "
                                 "lo que puede llevar a paradas no planificadas.";
        datos.frecuencia = "Media";
        datos.consecuencias = "Alta";
        datos.contexto = "Este evento puede ocurrir durante operaciones normales "
                        "cuando el sistema está bajo carga elevada. Los operadores "
                        "deben estar preparados para intervenir manualmente si es necesario.";
        
        // Generate PDF
        QString outputPath = "ejemplo_informe_simple.pdf";
        if (InformeGenerator::generarPDF(datos, outputPath)) {
            qDebug() << "✓ Informe simple generado:" << outputPath;
        } else {
            qDebug() << "✗ Error al generar informe simple";
        }
    }

    // Example 2: Complete report with barriers and reducers
    {
        DatosInforme datos;
        
        // Event information
        datos.etapa = "Etapa de Almacenamiento";
        datos.iniciador = "Derrame de material peligroso";
        datos.descripcionError = "Posible derrame de material peligroso debido a "
                                 "deterioro del contenedor de almacenamiento o "
                                 "manipulación incorrecta durante el transporte interno.";
        datos.frecuencia = "Baja";
        datos.consecuencias = "Muy Alta";
        datos.contexto = "El almacenamiento de materiales peligrosos requiere "
                        "condiciones específicas y protocolos estrictos. Un derrame "
                        "podría tener consecuencias graves para la salud y el medio ambiente.";
        
        // Add barriers (defenses that prevent the event)
        Barrera barrera1;
        barrera1.codigo = "B001";
        barrera1.descripcion = "Contenedores de doble pared certificados";
        barrera1.implementacion = "Implementado: Todos los contenedores cumplen con "
                                 "normativa internacional y son inspeccionados mensualmente.";
        barrera1.robustez = "Alta";
        datos.barreras.append(barrera1);
        
        Barrera barrera2;
        barrera2.codigo = "B002";
        barrera2.descripcion = "Sistema de detección de fugas automático";
        barrera2.implementacion = "Implementado: Sensores instalados en toda el área "
                                 "de almacenamiento con alarma en sala de control.";
        barrera2.robustez = "Media";
        datos.barreras.append(barrera2);
        
        Barrera barrera3;
        barrera3.codigo = "B003";
        barrera3.descripcion = "Procedimiento de manipulación segura";
        barrera3.implementacion = "Implementado: Todo el personal está capacitado "
                                 "y certificado en manipulación de materiales peligrosos.";
        barrera3.robustez = "Alta";
        datos.barreras.append(barrera3);
        
        // Add frequency reducers (measures that reduce the likelihood)
        Reductor reductor1;
        reductor1.codigo = "RF001";
        reductor1.robustez = "Alta";
        reductor1.descripcion = "Inspección visual diaria de contenedores";
        datos.reductoresFrecuencia.append(reductor1);
        
        Reductor reductor2;
        reductor2.codigo = "RF002";
        reductor2.robustez = "Media";
        reductor2.descripcion = "Mantenimiento preventivo trimestral de equipos";
        datos.reductoresFrecuencia.append(reductor2);
        
        Reductor reductor3;
        reductor3.codigo = "RF003";
        reductor3.robustez = "Alta";
        reductor3.descripcion = "Renovación programada de contenedores cada 5 años";
        datos.reductoresFrecuencia.append(reductor3);
        
        // Add consequence reducers (measures that reduce the impact if it happens)
        Reductor reductor4;
        reductor4.codigo = "RC001";
        reductor4.robustez = "Alta";
        reductor4.descripcion = "Sistema de contención y drenaje en área de almacenamiento";
        datos.reductoresConsecuencias.append(reductor4);
        
        Reductor reductor5;
        reductor5.codigo = "RC002";
        reductor5.robustez = "Media";
        reductor5.descripcion = "Kit de emergencia para derrames disponible 24/7";
        datos.reductoresConsecuencias.append(reductor5);
        
        Reductor reductor6;
        reductor6.codigo = "RC003";
        reductor6.robustez = "Alta";
        reductor6.descripcion = "Protocolo de evacuación y respuesta a emergencias";
        datos.reductoresConsecuencias.append(reductor6);
        
        // Generate PDF
        QString outputPath = "ejemplo_informe_completo.pdf";
        if (InformeGenerator::generarPDF(datos, outputPath)) {
            qDebug() << "✓ Informe completo generado:" << outputPath;
        } else {
            qDebug() << "✗ Error al generar informe completo";
        }
    }

    // Example 3: Report with only barriers (no reducers)
    {
        DatosInforme datos;
        
        datos.etapa = "Etapa de Transporte";
        datos.iniciador = "Accidente de tráfico en transporte de mercancías";
        datos.descripcionError = "Riesgo de accidente vehicular durante el transporte "
                                 "de mercancías entre instalaciones.";
        datos.frecuencia = "Baja";
        datos.consecuencias = "Media";
        datos.contexto = "El transporte de mercancías se realiza por carretera entre "
                        "las instalaciones principales y los centros de distribución.";
        
        Barrera barrera;
        barrera.id = 1;
        barrera.descripcion = "Conductores certificados y con experiencia";
        barrera.implementacion = "Todos los conductores tienen licencia profesional "
                                "y experiencia mínima de 5 años.";
        datos.barreras.append(barrera);
        
        QString outputPath = "ejemplo_informe_barreras.pdf";
        if (InformeGenerator::generarPDF(datos, outputPath)) {
            qDebug() << "✓ Informe con barreras generado:" << outputPath;
        } else {
            qDebug() << "✗ Error al generar informe con barreras";
        }
    }

    // Example 4: Handling special characters
    {
        DatosInforme datos;
        
        datos.etapa = "Prueba de caracteres especiales: <>&\"'";
        datos.iniciador = "Test con símbolos: €, £, ¥, ©, ®, ™";
        datos.descripcionError = "Este es un test de HTML escaping:\n"
                                 "- Símbolos: < > & \" '\n"
                                 "- Acentos: á é í ó ú ñ\n"
                                 "- Unicode: 你好 مرحبا שלום";
        datos.frecuencia = "Test";
        datos.consecuencias = "Test";
        datos.contexto = "El sistema debe escapar correctamente todos los "
                        "caracteres especiales HTML para prevenir problemas de formato.";
        
        QString outputPath = "ejemplo_caracteres_especiales.pdf";
        if (InformeGenerator::generarPDF(datos, outputPath)) {
            qDebug() << "✓ Informe con caracteres especiales generado:" << outputPath;
        } else {
            qDebug() << "✗ Error al generar informe con caracteres especiales";
        }
    }

    qDebug() << "\n=== Ejemplos completados ===";
    qDebug() << "Revise los archivos PDF generados para ver los resultados.";
    
    return 0;
}
