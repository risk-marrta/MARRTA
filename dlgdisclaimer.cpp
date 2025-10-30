#include "dlgdisclaimer.h"
#include "ui_dlgdisclaimer.h"

DlgDisclaimer::DlgDisclaimer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDisclaimer)
{
    ui->setupUi(this);
    setWindowTitle(tr("Descargo de Responsabilidad / Disclaimer"));
    
    // Set the disclaimer text
    QString disclaimerText = tr(
        "<h2>DESCARGO DE RESPONSABILIDAD / DISCLAIMER</h2>"
        "<h3>ADVERTENCIA IMPORTANTE: ESTE SOFTWARE NO ES UN DISPOSITIVO MÉDICO</h3>"
        "<p>Este software de análisis de riesgo (en adelante, \"el Software\") está diseñado exclusivamente con fines informativos, educativos y de análisis administrativo de riesgos en el ámbito hospitalario.</p>"
        "<p><strong>El Software NO está autorizado, certificado ni regulado como producto médico o dispositivo médico según las normativas aplicables (FDA, Reglamento MDR 2017/745 de la UE, o autoridades sanitarias correspondientes).</strong></p>"
        "<h3>Limitaciones de Uso:</h3>"
        "<ul>"
        "<li>NO ESTÁ DESTINADO al diagnóstico, tratamiento, curación, mitigación o prevención de enfermedades o condiciones médicas.</li>"
        "<li>NO DEBE UTILIZARSE para tomar decisiones clínicas directas sobre pacientes individuales.</li>"
        "<li>NO SUSTITUYE el juicio clínico profesional, el criterio médico cualificado, ni la consulta con profesionales sanitarios autorizados.</li>"
        "<li>NO REEMPLAZA los protocolos médicos establecidos, las guías clínicas aprobadas ni los sistemas de soporte a decisiones clínicas validados.</li>"
        "</ul>"
        "<h3>Propósito del Software:</h3>"
        "<p>Este Software proporciona análisis de riesgos a nivel institucional, organizativo o poblacional con el fin de:</p>"
        "<ul>"
        "<li>Identificar patrones y tendencias generales</li>"
        "<li>Apoyar la gestión administrativa y operativa hospitalaria</li>"
        "<li>Facilitar la planificación de recursos</li>"
        "<li>Generar informes estadísticos agregados</li>"
        "</ul>"
        "<h3>Responsabilidad del Usuario:</h3>"
        "<p>Los profesionales sanitarios y administradores hospitalarios que utilicen este Software:</p>"
        "<ul>"
        "<li>Son los únicos responsables de verificar independientemente la información generada</li>"
        "<li>Deben basar sus decisiones clínicas exclusivamente en su experiencia profesional, formación médica y las herramientas clínicas apropiadas y autorizadas</li>"
        "<li>Deben consultar con profesionales cualificados para cualquier decisión que afecte a la atención al paciente</li>"
        "</ul>"
        "<h3>Limitación de Responsabilidad:</h3>"
        "<p>Los profesionales que han trabajado en el software y modelos dentro del proyecto MARRTA, así como los Organismos, Fundaciones, Hospitales, Departamentos y Sociedades Profesionales que han apoyado su desarrollo, no asumen responsabilidad alguna por:</p>"
        "<ul>"
        "<li>Decisiones tomadas basándose en la información proporcionada por el Software</li>"
        "<li>Daños, pérdidas o perjuicios derivados del uso o mal uso del Software</li>"
        "<li>La exactitud, completitud o actualidad de los resultados generados</li>"
        "</ul>"
        "<p><strong>EN CASO DE EMERGENCIA MÉDICA, CONTACTE INMEDIATAMENTE CON LOS SERVICIOS DE URGENCIAS O CON UN PROFESIONAL MÉDICO CUALIFICADO.</strong></p>"
        "<p><em>El uso de este Software implica la aceptación expresa de los términos de este descargo de responsabilidad.</em></p>"
    );
    
    ui->textBrowser->setHtml(disclaimerText);
}

DlgDisclaimer::~DlgDisclaimer()
{
    delete ui;
}
