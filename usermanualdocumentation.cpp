#include "usermanualdocumentation.h"
#include <QTreeWidgetItem>
#include <QHeaderView>

UserManualDocumentation::UserManualDocumentation(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Manual de Usuario - MARRTA"));
    setMinimumSize(900, 650);
    resize(1100, 750);
    
    setupUI();
    populateDocumentation();
}

void UserManualDocumentation::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create splitter for two-panel layout
    splitter = new QSplitter(Qt::Horizontal, this);
    
    // Left panel: Section tree
    sectionTree = new QTreeWidget();
    sectionTree->setHeaderLabel(tr("Índice"));
    sectionTree->setMaximumWidth(350);
    sectionTree->setMinimumWidth(280);
    
    // Right panel: Content browser
    contentBrowser = new QTextBrowser();
    contentBrowser->setOpenExternalLinks(true);
    
    splitter->addWidget(sectionTree);
    splitter->addWidget(contentBrowser);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    
    // Bottom button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    closeButton = new QPushButton(tr("Cerrar"));
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addWidget(splitter);
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(sectionTree, &QTreeWidget::currentItemChanged, this, &UserManualDocumentation::onSectionChanged);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
}

void UserManualDocumentation::populateDocumentation()
{
    // Clear existing items
    sectionTree->clear();
    documentationItems.clear();
    
    // Section 1: Introduction
    QTreeWidgetItem *introItem = new QTreeWidgetItem(sectionTree);
    introItem->setText(0, tr("1. Introducción"));
    addDocumentationSection(tr("1. Introducción"), 
        tr("<h2>Introducción General</h2>"
        "<p><b>MARRTA</b> (Metodología de Análisis de Riesgos y Rendimiento en Radioterapia Avanzada) "
        "es una herramienta integral para el análisis y gestión de riesgos en servicios de radioterapia. "
        "Esta aplicación implementa una metodología sistemática para identificar, evaluar y mitigar riesgos "
        "en todos los procesos del tratamiento radioterápico, desde la prescripción hasta la administración "
        "del tratamiento.</p>"
        "<h3>¿Qué es MARRTA?</h3>"
        "<p>MARRTA es un sistema de análisis modal de fallos y efectos (AMFE) específicamente diseñado para "
        "radioterapia. Permite a los equipos clínicos realizar análisis prospectivos de riesgos, identificando "
        "puntos críticos en sus procesos y evaluando la efectividad de las barreras de seguridad implementadas.</p>"
        "<h3>Documentación y Recursos de Aprendizaje</h3>"
        "<p>Para más información sobre la metodología teórica y el modelo subyacente de MARRTA, "
        "consulte los siguientes recursos:</p>"
        "<p><b>Documentación teórica:</b><br/>"
        "<a href='https://github.com/risk-marrta/MARRTA/wiki/Descargas-Proyecto-MARRTA'>"
        "https://github.com/risk-marrta/MARRTA/wiki/Descargas-Proyecto-MARRTA</a><br/>"
        "Artículos, guías metodológicas, casos de estudio y material de formación.</p>"
        "<p><b>Video-tutoriales:</b><br/>"
        "<a href='https://www.youtube.com/channel/UC4l1EbqC4Tth4S-GqWXetvQ'>"
        "https://www.youtube.com/channel/UC4l1EbqC4Tth4S-GqWXetvQ</a><br/>"
        "Canal de YouTube con tutoriales en video sobre el uso de MARRTA.</p>"
        "<h3>Requisitos del Sistema</h3>"
        "<ul>"
        "<li><b>Sistema operativo:</b> Windows 10/11, Linux (Ubuntu 20.04+, Debian 11+, Fedora 35+), macOS 10.15+</li>"
        "<li><b>Framework Qt:</b> Qt 6.10 o superior (incluido en la instalación)</li>"
        "<li><b>Base de datos:</b> SQLite 3.x (incluido)</li>"
        "<li><b>Python:</b> Python 3.12 (opcional, solo necesario para el motor de scripting Python)</li>"
        "<li><b>Memoria RAM:</b> Mínimo 4 GB, recomendado 8 GB o más</li>"
        "<li><b>Espacio en disco:</b> Aproximadamente 100 MB para la aplicación + espacio para modelos</li>"
        "<li><b>Resolución de pantalla:</b> Mínimo 1280x720, recomendado 1920x1080 o superior</li>"
        "</ul>"
        "<h3>Posibilidades y Funcionalidades</h3>"
        "<p>MARRTA proporciona un conjunto completo de herramientas para el análisis de riesgos:</p>"
        "<ul>"
        "<li><b>Gestión de modelos de riesgo:</b> Crear modelos nuevos desde cero, abrir modelos existentes, "
        "guardar cambios, y exportar a diferentes formatos (Excel, CSV)</li>"
        "<li><b>Análisis de sucesos iniciadores:</b> Identificar y evaluar eventos potenciales que pueden "
        "desencadenar incidentes o errores en el proceso de radioterapia</li>"
        "<li><b>Gestión de defensas:</b> Configurar y activar barreras, reductores de frecuencia y "
        "reductores de consecuencia para cada evento</li>"
        "<li><b>Cálculo automático de riesgo:</b> Evaluación dinámica de frecuencia, probabilidad, consecuencia "
        "y nivel de riesgo basada en las defensas activas</li>"
        "<li><b>Matriz de riesgo configurable:</b> Personalizar umbrales y criterios de aceptabilidad del riesgo</li>"
        "<li><b>Informes detallados:</b> Generación de informes profesionales en HTML con gráficos, "
        "tablas y análisis estadísticos</li>"
        "<li><b>Flujogramas de proceso:</b> Visualización gráfica del proceso con eventos y defensas</li>"
        "<li><b>Importación/Exportación:</b> Soporte completo para archivos Excel (.xlsx) permitiendo "
        "importar modelos completos o exportar para análisis externo</li>"
        "<li><b>Motores de scripting:</b> Automatización avanzada mediante JavaScript y Python para "
        "análisis personalizados y procesamiento de datos</li>"
        "<li><b>Gestión de etapas:</b> Organización jerárquica de eventos por etapas y subetapas del proceso, "
        "facilitando el análisis por fases</li>"
        "<li><b>Filtrado avanzado:</b> Búsqueda y filtrado múltiple por nombre, descripción, etapa, "
        "frecuencia, consecuencia, riesgo y otros atributos</li>"
        "<li><b>Análisis comparativos:</b> Comparar escenarios con diferentes configuraciones de defensas</li>"
        "<li><b>Trabajo colaborativo:</b> Compartir modelos entre equipos y departamentos</li>"
        "</ul>"
        "<h3>Casos de Uso Típicos</h3>"
        "<ul>"
        "<li>Análisis inicial de riesgos para un nuevo servicio de radioterapia</li>"
        "<li>Evaluación periódica de riesgos (revisión anual o semestral)</li>"
        "<li>Análisis tras la implementación de nuevos equipos o técnicas</li>"
        "<li>Evaluación del impacto de cambios en procedimientos</li>"
        "<li>Documentación para acreditaciones y auditorías</li>"
        "<li>Formación de personal en seguridad del paciente</li>"
        "</ul>"));
    
    // Section 1.1: Main screen elements
    QTreeWidgetItem *mainScreenItem = new QTreeWidgetItem(introItem);
    mainScreenItem->setText(0, tr("1.1 Elementos de la pantalla principal"));
    addDocumentationSection(tr("1.1 Elementos de la pantalla principal"),
        tr("<h2>Descripción de los Elementos de la Pantalla Principal</h2>"
        "<p>La ventana principal de MARRTA se compone de varias áreas funcionales:</p>"
        "<h3>Barra de menú superior</h3>"
        "<p>Contiene los menús principales: Archivo, Modelo, Herramientas, Administración y Ayuda.</p>"
        "<h3>Panel izquierdo: Navegación de etapas</h3>"
        "<ul>"
        "<li><b>Árbol de etapas y subetapas:</b> Permite navegar por las diferentes etapas del proceso de radioterapia</li>"
        "<li>Al seleccionar una etapa, se muestran los eventos asociados en el panel central</li>"
        "<li>El botón \"Todas las etapas\" muestra todos los eventos sin filtrar por etapa</li>"
        "</ul>"
        "<h3>Panel central: Lista de eventos iniciadores</h3>"
        "<ul>"
        "<li><b>Lista de sucesos iniciadores:</b> Muestra los eventos de la etapa seleccionada</li>"
        "<li>Cada evento se muestra con un indicador de color según su nivel de riesgo:</li>"
        "<ul>"
        "<li style='color: #2e7d32;'><b>Verde:</b> Riesgo Bajo (RB)</li>"
        "<li style='color: #f57c00;'><b>Naranja:</b> Riesgo Medio (RM)</li>"
        "<li style='color: #d32f2f;'><b>Rojo:</b> Riesgo Alto (RA)</li>"
        "<li style='color: #7b1fa2;'><b>Púrpura:</b> Riesgo Muy Alto (RMA)</li>"
        "</ul>"
        "<li>Contador [n/m]: n = eventos aplicables, m = total de eventos</li>"
        "</ul>"
        "<h3>Filtros superiores</h3>"
        "<ul>"
        "<li><b>Filtro por nombre:</b> Buscar eventos por nombre</li>"
        "<li><b>Filtro por descripción:</b> Buscar eventos por descripción</li>"
        "<li><b>Filtros por nivel:</b> Frecuencia, Consecuencia, Riesgo</li>"
        "<li><b>Filtro de aplicables:</b> Mostrar solo eventos aplicables o no aplicables</li>"
        "<li><b>Filtro de errores humanos:</b> Mostrar solo eventos que involucran error humano</li>"
        "</ul>"
        "<h3>Panel derecho: Detalles del evento seleccionado</h3>"
        "<ul>"
        "<li><b>Información del evento:</b> Código, nombre, descripción, etapa/subetapa</li>"
        "<li><b>Indicadores de riesgo:</b> Visualización gráfica de Frecuencia (F), Probabilidad (P), Consecuencia (C) y Riesgo (R)</li>"
        "<li><b>Checkbox \"Aplicable\":</b> Indica si el evento es aplicable al departamento</li>"
        "<li><b>Checkbox \"Error Humano\":</b> Indica si el evento involucra error humano</li>"
        "</ul>"
        "<h3>Listas de defensas (panel derecho inferior)</h3>"
        "<ul>"
        "<li><b>Reductores de Frecuencia:</b> Defensas que reducen la frecuencia del evento</li>"
        "<li><b>Barreras:</b> Defensas que reducen la probabilidad de que el evento cause consecuencias</li>"
        "<li><b>Reductores de Consecuencia:</b> Defensas que mitigan las consecuencias del evento</li>"
        "<li>Cada lista muestra [n/m] donde n = defensas activas, m = total de defensas</li>"
        "<li>Doble clic en una defensa muestra información detallada</li>"
        "</ul>"
        "<h3>Barra de estado inferior</h3>"
        "<p>Muestra información sobre el modelo cargado y mensajes de estado.</p>"));
    
    // Section 2: Analysis process
    QTreeWidgetItem *analysisItem = new QTreeWidgetItem(sectionTree);
    analysisItem->setText(0, tr("2. Proceso de análisis"));
    addDocumentationSection(tr("2. Proceso de análisis"),
        tr("<h2>Proceso de Análisis de Riesgos</h2>"
        "<p>El análisis de riesgos en MARRTA sigue un proceso sistemático:</p>"
        "<h3>1. Cargar un modelo</h3>"
        "<p>Utilice <b>Archivo → Abrir</b> para cargar un archivo .praxis existente o "
        "<b>Archivo → Nuevo modelo</b> para crear uno desde cero.</p>"
        "<h3>2. Revisar y marcar sucesos iniciadores</h3>"
        "<p>Para cada suceso iniciador del modelo, debe decidir si es <b>aplicable a su servicio o departamento de radioterapia</b>:</p>"
        "<ul>"
        "<li>Navegue por las etapas en el panel izquierdo</li>"
        "<li>Seleccione cada evento y revise su descripción</li>"
        "<li>Marque el checkbox \"Aplicable\" si el evento es relevante para su departamento</li>"
        "<li>Desmarque el checkbox si el evento no aplica a su contexto</li>"
        "</ul>"
        "<p><b>Nota importante:</b> Un suceso iniciador representa un evento potencial que podría ocurrir. "
        "Marcarlo como \"aplicable\" no significa que haya ocurrido, sino que es un evento posible en su departamento "
        "y debe ser considerado en el análisis de riesgo.</p>"
        "<h3>3. Configurar defensas aplicables</h3>"
        "<p>Para cada evento aplicable, revise y active las defensas pertinentes:</p>"
        "<ul>"
        "<li><b>Reductores de Frecuencia:</b> Medidas que reducen la frecuencia de ocurrencia del evento (F)</li>"
        "<li><b>Barreras:</b> Medidas que reducen la probabilidad de que el evento cause consecuencias (P)</li>"
        "<li><b>Reductores de Consecuencia:</b> Medidas que mitigan las consecuencias si ocurre el evento (C)</li>"
        "</ul>"
        "<p>Active (marque con ✓) las defensas que estén implementadas en su departamento. "
        "Las defensas activas se utilizan para calcular el riesgo real de la secuencia.</p>"
        "<h3>4. Comprender la diferencia: Suceso iniciador vs. Secuencia</h3>"
        "<h4>Suceso Iniciador (evento base)</h4>"
        "<ul>"
        "<li>Es el evento potencial sin considerar defensas</li>"
        "<li>Tiene asociados: <b>Frecuencia Base (F)</b> y <b>Consecuencia Base (C)</b></li>"
        "<li>El riesgo base asume la probabilidad máxima (PA=3), por lo que: <b>Riesgo Base = función(F, PA=3, C)</b></li>"
        "<li>Representa el \"peor escenario\" sin mitigación</li>"
        "</ul>"
        "<h4>Secuencia (iniciador + defensas)</h4>"
        "<ul>"
        "<li>Es el análisis completo del evento <b>considerando las defensas activas</b></li>"
        "<li>Tiene asociados valores calculados: <b>Frecuencia (F), Probabilidad (P), Consecuencia (C)</b></li>"
        "<li>F: Calculada desde F base aplicando reductores de frecuencia activos</li>"
        "<li>P: Calculada desde las barreras activas (a más barreras robustas, menor probabilidad)</li>"
        "<li>C: Calculada desde C base aplicando reductores de consecuencia activos</li>"
        "<li>El <b>Riesgo final</b> se obtiene de la matriz de riesgo aplicada a los valores F, P, C calculados</li>"
        "<li>Representa el riesgo real considerando las medidas de seguridad implementadas</li>"
        "</ul>"
        "<p><b>Ejemplo:</b> Un evento podría tener F base = Alta y C base = Muy Alta (Riesgo Base = RMA). "
        "Sin embargo, con reductores de frecuencia y barreras implementadas, la secuencia resultante "
        "podría tener F = Media, P = Baja, C = Media, resultando en un Riesgo = RM (Medio).</p>"
        "<h3>5. Revisar resultados</h3>"
        "<p>Utilice las opciones del menú <b>Herramientas</b> para revisar el análisis:</p>"
        "<ul>"
        "<li><b>Resumen:</b> Vista tabular consolidada de todos los eventos aplicables con sus niveles de riesgo</li>"
        "<li><b>Informe detallado (HTML):</b> Informe completo con análisis detallado de cada evento, "
        "incluyendo defensas aplicadas, cálculos de riesgo, y gráficos de distribución</li>"
        "<li><b>Flujograma:</b> Representación gráfica del proceso con eventos y defensas</li>"
        "</ul>"
        "<h3>Diferencia entre Resumen e Informe Detallado</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse;'>"
        "<tr style='background-color: #f0f0f0;'><th>Aspecto</th><th>Resumen</th><th>Informe Detallado</th></tr>"
        "<tr><td><b>Formato</b></td><td>Tabla interactiva en la aplicación</td><td>Documento HTML exportable</td></tr>"
        "<tr><td><b>Contenido</b></td><td>Lista de eventos con valores F, P, C, R</td><td>Análisis completo con descripción, defensas, cálculos y gráficos</td></tr>"
        "<tr><td><b>Uso</b></td><td>Revisión rápida y búsqueda</td><td>Documentación formal y presentaciones</td></tr>"
        "<tr><td><b>Interactividad</b></td><td>Permite ordenar y filtrar</td><td>Documento estático para impresión/distribución</td></tr>"
        "<tr><td><b>Detalle</b></td><td>Vista compacta</td><td>Incluye metodología, matriz de riesgo, distribuciones, análisis por etapa</td></tr>"
        "</table>"));
    
    // Section 3: File menu
    QTreeWidgetItem *fileMenuSection = new QTreeWidgetItem(sectionTree);
    fileMenuSection->setText(0, tr("3. Menú Archivo"));
    
    // Section 3.1: New and Open
    QTreeWidgetItem *fileNewOpenItem = new QTreeWidgetItem(fileMenuSection);
    fileNewOpenItem->setText(0, tr("3.1 Nuevo modelo y Abrir"));
    addDocumentationSection(tr("3.1 Nuevo modelo y Abrir"),
        tr("<h2>Nuevo modelo y Abrir</h2>"
        "<h3>Nuevo modelo</h3>"
        "<p>Esta opción crea un nuevo modelo de análisis de riesgo desde cero utilizando un asistente guiado "
        "que facilita la configuración inicial. El asistente le guiará a través de los siguientes pasos:</p>"
        "<ol>"
        "<li><b>Información básica:</b> Ingrese el nombre del modelo (ej: \"MARRTA Hospital X 2025\") y "
        "una descripción detallada del alcance del análisis</li>"
        "<li><b>Configuración de la matriz de riesgo:</b> Defina los umbrales de aceptabilidad del riesgo "
        "según los criterios de su institución. Puede usar la matriz estándar o personalizarla</li>"
        "<li><b>Definición de etapas:</b> Estructure el proceso de radioterapia en etapas principales "
        "(ej: Prescripción, Simulación, Planificación, Verificación, Tratamiento)</li>"
        "<li><b>Definición de subetapas:</b> Para cada etapa, defina las subetapas o pasos específicos "
        "que la componen</li>"
        "<li><b>Confirmación:</b> Revise la configuración y cree el modelo</li>"
        "</ol>"
        "<p>Una vez creado, el modelo estará listo para agregar eventos iniciadores y defensas.</p>"
        "<h3>Abrir</h3>"
        "<p>Abre un archivo de modelo existente con extensión <b>.praxis</b>. Al seleccionar esta opción:</p>"
        "<ul>"
        "<li>Se muestra un diálogo de selección de archivo</li>"
        "<li>Navegue hasta la ubicación del archivo .praxis</li>"
        "<li>El modelo se carga completamente en la aplicación</li>"
        "<li>El archivo se agrega automáticamente a la lista de \"Archivos recientes\"</li>"
        "</ul>"
        "<p><b>Archivos recientes:</b> Los últimos 5 archivos abiertos aparecen en un submenú "
        "para acceso rápido. Simplemente haga clic en el nombre del archivo para abrirlo directamente.</p>"
        "<p><b>Tip:</b> Si trabaja con múltiples modelos (por ejemplo, uno para cada modalidad de tratamiento), "
        "utilice nombres descriptivos como \"MARRTA_IMRT_2025.praxis\" o \"MARRTA_Braquiterapia_2025.praxis\".</p>"));
    
    // Section 3.2: Save
    QTreeWidgetItem *fileSaveItem = new QTreeWidgetItem(fileMenuSection);
    fileSaveItem->setText(0, tr("3.2 Guardar y Guardar como"));
    addDocumentationSection(tr("3.2 Guardar y Guardar como"),
        tr("<h2>Guardar y Guardar como</h2>"
        "<h3>Guardar</h3>"
        "<p>Guarda todos los cambios realizados en el modelo actual:</p>"
        "<ul>"
        "<li>Si el modelo ya tiene un nombre de archivo asociado, los cambios se guardan directamente</li>"
        "<li>Si el modelo es nuevo (nunca guardado), se comporta como \"Guardar como...\" y solicita un nombre</li>"
        "<li>Los cambios guardados incluyen: eventos modificados, defensas activadas/desactivadas, "
        "configuración de aplicabilidad, etc.</li>"
        "</ul>"
        "<p><b>Indicador de cambios:</b> Cuando hay cambios sin guardar, aparece un asterisco (*) "
        "en el título de la ventana junto al nombre del archivo.</p>"
        "<p><b>Recomendación:</b> Guarde regularmente su trabajo, especialmente después de realizar "
        "cambios importantes en la configuración de defensas o la aplicabilidad de eventos.</p>"
        "<h3>Guardar como...</h3>"
        "<p>Guarda el modelo actual con un nuevo nombre o en una ubicación diferente. Útil para:</p>"
        "<ul>"
        "<li><b>Crear variantes:</b> Analizar diferentes escenarios (ej: \"MARRTA_Actual.praxis\" vs "
        "\"MARRTA_Escenario_Mejora.praxis\")</li>"
        "<li><b>Hacer copias de seguridad:</b> Crear una copia antes de realizar cambios importantes</li>"
        "<li><b>Compartir modelos:</b> Guardar en una ubicación compartida para trabajo colaborativo</li>"
        "<li><b>Versionado manual:</b> Mantener versiones históricas (ej: \"MARRTA_2024.praxis\", "
        "\"MARRTA_2025.praxis\")</li>"
        "</ul>"
        "<p>Al usar \"Guardar como...\", el archivo original permanece intacto y la aplicación pasa "
        "a trabajar con el nuevo archivo.</p>"));
    
    // Section 3.3: Import from Excel
    QTreeWidgetItem *fileImportItem = new QTreeWidgetItem(fileMenuSection);
    fileImportItem->setText(0, tr("3.3 Importar desde Excel"));
    addDocumentationSection(tr("3.3 Importar desde Excel"),
        tr("<h2>Importar desde Excel</h2>"
        "<p><b>Esta es una de las funciones más potentes de MARRTA</b>, ya que permite importar modelos "
        "completos desde archivos Excel, facilitando:</p>"
        "<ul>"
        "<li>La creación de modelos en equipo usando Excel como herramienta de trabajo colaborativo</li>"
        "<li>La migración desde otros sistemas o formatos</li>"
        "<li>La actualización masiva de eventos y defensas</li>"
        "<li>El intercambio de modelos entre instituciones</li>"
        "</ul>"
        "<h3>Recomendación: Exportar primero para ver la estructura</h3>"
        "<p><b>Consejo importante:</b> Si es la primera vez que va a importar un modelo desde Excel, "
        "es muy recomendable que primero:</p>"
        "<ol>"
        "<li>Abra un modelo MARRTA existente (o cree uno simple con el asistente)</li>"
        "<li>Use la opción <b>Archivo → Exportar a Excel</b> para generar un archivo .xlsx</li>"
        "<li>Abra ese archivo Excel y estudie su estructura</li>"
        "<li>Use ese archivo como plantilla para crear su propio modelo</li>"
        "</ol>"
        "<p>Esto le ayudará a entender exactamente cómo debe estructurar su archivo Excel para una "
        "importación exitosa.</p>"
        "<h3>Proceso de importación</h3>"
        "<p>Al seleccionar <b>Archivo → Importar desde Excel...</b>, se inicia un asistente que:</p>"
        "<ol>"
        "<li><b>Selección de archivo:</b> Elija el archivo Excel (.xlsx) que contiene el modelo</li>"
        "<li><b>Validación:</b> MARRTA valida que el archivo tenga la estructura correcta</li>"
        "<li><b>Vista previa:</b> Se muestra un resumen de lo que se va a importar (número de etapas, "
        "eventos, defensas, etc.)</li>"
        "<li><b>Opciones de importación:</b> Puede elegir si desea crear un modelo nuevo o fusionar "
        "con el modelo actual</li>"
        "<li><b>Importación:</b> Se procesan los datos y se crea/actualiza el modelo</li>"
        "<li><b>Resumen:</b> Se muestra un informe de lo importado y cualquier advertencia o error</li>"
        "</ol>"
        "<h3>Formato del archivo Excel para importación</h3>"
        "<p>El archivo Excel debe contener <b>exactamente 6 hojas</b> con los nombres y estructura "
        "que se detallan a continuación. Es importante respetar los nombres de las hojas y columnas.</p>"));
    
    // Section 3.3.1: Excel Sheet 1
    QTreeWidgetItem *excelSheet1Item = new QTreeWidgetItem(fileImportItem);
    excelSheet1Item->setText(0, tr("3.3.1 Hoja \"Etapas\""));
    addDocumentationSection(tr("3.3.1 Hoja \"Etapas\""),
        tr("<h2>Hoja 1: \"Etapas\"</h2>"
        "<p>Esta hoja define las etapas principales del proceso de radioterapia.</p>"
        "<h3>Estructura de columnas</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Columna</th><th>Tipo</th><th>Obligatorio</th><th>Descripción</th><th>Ejemplo</th></tr>"
        "<tr><td><b>Código</b></td><td>Texto</td><td>Sí</td><td>Código único de la etapa (alfanumérico)</td><td>E001, ETAPA-01</td></tr>"
        "<tr><td><b>Nombre</b></td><td>Texto</td><td>Sí</td><td>Nombre descriptivo de la etapa</td><td>Prescripción médica</td></tr>"
        "<tr><td><b>Descripción</b></td><td>Texto</td><td>No</td><td>Descripción detallada de la etapa</td><td>Proceso de prescripción del tratamiento por el oncólogo radioterápico</td></tr>"
        "<tr><td><b>Orden</b></td><td>Número</td><td>Sí</td><td>Orden de visualización (1, 2, 3...)</td><td>1</td></tr>"
        "</table>"
        "<h3>Ejemplo de contenido</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Código</th><th>Nombre</th><th>Descripción</th><th>Orden</th></tr>"
        "<tr><td>E001</td><td>Prescripción médica</td><td>Definición de dosis y volúmenes de tratamiento</td><td>1</td></tr>"
        "<tr><td>E002</td><td>Simulación</td><td>Adquisición de imágenes y definición de posicionamiento</td><td>2</td></tr>"
        "<tr><td>E003</td><td>Planificación</td><td>Diseño y optimización del plan de tratamiento</td><td>3</td></tr>"
        "<tr><td>E004</td><td>Verificación</td><td>QA del plan y verificaciones pre-tratamiento</td><td>4</td></tr>"
        "<tr><td>E005</td><td>Tratamiento</td><td>Administración del tratamiento de radioterapia</td><td>5</td></tr>"
        "</table>"
        "<h3>Notas importantes</h3>"
        "<ul>"
        "<li>Los códigos deben ser únicos en toda la hoja</li>"
        "<li>El orden determina cómo se mostrarán las etapas en la interfaz</li>"
        "<li>Puede tener desde 1 hasta cualquier número de etapas (típicamente 5-15)</li>"
        "<li>Los nombres pueden contener tildes y caracteres especiales</li>"
        "</ul>"));
    
    // Section 3.3.2: Excel Sheet 2
    QTreeWidgetItem *excelSheet2Item = new QTreeWidgetItem(fileImportItem);
    excelSheet2Item->setText(0, tr("3.3.2 Hoja \"Subetapas\""));
    addDocumentationSection(tr("3.3.2 Hoja \"Subetapas\""),
        tr("<h2>Hoja 2: \"Subetapas\"</h2>"
        "<p>Esta hoja define las subetapas o pasos dentro de cada etapa principal.</p>"
        "<h3>Estructura de columnas</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Columna</th><th>Tipo</th><th>Obligatorio</th><th>Descripción</th><th>Ejemplo</th></tr>"
        "<tr><td><b>Código Etapa</b></td><td>Texto</td><td>Sí</td><td>Código de la etapa padre (debe existir en hoja Etapas)</td><td>E001</td></tr>"
        "<tr><td><b>Código</b></td><td>Texto</td><td>Sí</td><td>Código único de la subetapa</td><td>S001</td></tr>"
        "<tr><td><b>Nombre</b></td><td>Texto</td><td>Sí</td><td>Nombre de la subetapa</td><td>Revisión de historia clínica</td></tr>"
        "<tr><td><b>Descripción</b></td><td>Texto</td><td>No</td><td>Descripción detallada</td><td>Análisis de antecedentes del paciente y patología</td></tr>"
        "<tr><td><b>Orden</b></td><td>Número</td><td>Sí</td><td>Orden dentro de la etapa padre</td><td>1</td></tr>"
        "</table>"
        "<h3>Ejemplo de contenido</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Código Etapa</th><th>Código</th><th>Nombre</th><th>Descripción</th><th>Orden</th></tr>"
        "<tr><td>E001</td><td>S001</td><td>Revisión historia clínica</td><td>Análisis de información médica previa</td><td>1</td></tr>"
        "<tr><td>E001</td><td>S002</td><td>Decisión de tratamiento</td><td>Definición de intención y técnica</td><td>2</td></tr>"
        "<tr><td>E001</td><td>S003</td><td>Prescripción de dosis</td><td>Especificación de dosis y fraccionamiento</td><td>3</td></tr>"
        "<tr><td>E002</td><td>S004</td><td>Posicionamiento del paciente</td><td>Colocación en dispositivos de inmovilización</td><td>1</td></tr>"
        "<tr><td>E002</td><td>S005</td><td>Adquisición de TAC</td><td>Obtención de imágenes de planificación</td><td>2</td></tr>"
        "</table>"
        "<h3>Notas importantes</h3>"
        "<ul>"
        "<li>Cada subetapa debe estar asociada a una etapa válida mediante \"Código Etapa\"</li>"
        "<li>Los códigos de subetapa deben ser únicos en toda la hoja</li>"
        "<li>El orden es relativo dentro de cada etapa (cada etapa tiene su propia secuencia 1, 2, 3...)</li>"
        "<li>Una etapa puede tener desde 0 hasta cualquier número de subetapas</li>"
        "<li>Si una etapa no tiene subetapas, los eventos se asignarán directamente a la etapa</li>"
        "</ul>"));
    
    // Section 3.3.3: Excel Sheet 3
    QTreeWidgetItem *excelSheet3Item = new QTreeWidgetItem(fileImportItem);
    excelSheet3Item->setText(0, tr("3.3.3 Hoja \"Sucesos\""));
    addDocumentationSection(tr("3.3.3 Hoja \"Sucesos\""),
        tr("<h2>Hoja 3: \"Sucesos\" (Sucesos Iniciadores)</h2>"
        "<p>Esta es la hoja más importante ya que contiene todos los sucesos iniciadores del modelo.</p>"
        "<h3>Estructura de columnas</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Columna</th><th>Tipo</th><th>Obligatorio</th><th>Descripción</th><th>Valores/Ejemplo</th></tr>"
        "<tr><td><b>Código</b></td><td>Texto</td><td>Sí</td><td>Código único del evento</td><td>EV001, EVT-001</td></tr>"
        "<tr><td><b>Nombre</b></td><td>Texto</td><td>Sí</td><td>Nombre breve del evento</td><td>Error en dosis prescrita</td></tr>"
        "<tr><td><b>Descripción</b></td><td>Texto</td><td>Sí</td><td>Descripción detallada del evento</td><td>Prescripción de dosis incorrecta...</td></tr>"
        "<tr><td><b>Código Etapa</b></td><td>Texto</td><td>Sí</td><td>Código de la etapa (debe existir)</td><td>E001</td></tr>"
        "<tr><td><b>Código Subetapa</b></td><td>Texto</td><td>No*</td><td>Código de la subetapa (si aplica)</td><td>S001</td></tr>"
        "<tr><td><b>Frecuencia Base</b></td><td>Número</td><td>Sí</td><td>Nivel de frecuencia sin defensas</td><td>0, 1, 2, 3</td></tr>"
        "<tr><td><b>Consecuencia Base</b></td><td>Número</td><td>Sí</td><td>Nivel de consecuencia sin defensas</td><td>0, 1, 2, 3</td></tr>"
        "<tr><td><b>Aplicable</b></td><td>Número</td><td>Sí</td><td>¿Aplicable a nuestro servicio?</td><td>0=No, 1=Sí</td></tr>"
        "<tr><td><b>Error Humano</b></td><td>Número</td><td>No</td><td>¿Involucra error humano?</td><td>0=No, 1=Sí</td></tr>"
        "<tr><td><b>Base</b></td><td>Número</td><td>No</td><td>¿Es evento base del modelo?</td><td>0=No, 1=Sí (normalmente 1)</td></tr>"
        "</table>"
        "<p style='font-size: 0.9em;'><i>*Si la etapa no tiene subetapas definidas, este campo puede dejarse vacío</i></p>"
        "<h3>Valores de Frecuencia y Consecuencia Base</h3>"
        "<p>Los valores numéricos representan niveles cualitativos:</p>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse;'>"
        "<tr style='background-color: #f0f0f0;'><th>Valor</th><th>Frecuencia Base</th><th>Consecuencia Base</th></tr>"
        "<tr><td><b>0</b></td><td>FMB - Muy Baja</td><td>CB - Bajas</td></tr>"
        "<tr><td><b>1</b></td><td>FB - Baja</td><td>CM - Medias</td></tr>"
        "<tr><td><b>2</b></td><td>FM - Media</td><td>CA - Altas</td></tr>"
        "<tr><td><b>3</b></td><td>FA - Alta</td><td>CMA - Muy Altas</td></tr>"
        "</table>"
        "<h3>Ejemplo de contenido</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%; font-size: 0.85em;'>"
        "<tr style='background-color: #e3f2fd;'><th>Código</th><th>Nombre</th><th>Descripción</th><th>Cód. Etapa</th><th>Cód. Subetapa</th><th>Frec. Base</th><th>Cons. Base</th><th>Aplicable</th><th>Error Humano</th><th>Base</th></tr>"
        "<tr><td>EV001</td><td>Dosis errónea</td><td>Error en la dosis prescrita por el médico</td><td>E001</td><td>S003</td><td>2</td><td>3</td><td>1</td><td>1</td><td>1</td></tr>"
        "<tr><td>EV002</td><td>Volumen incorrecto</td><td>Definición errónea del volumen a tratar</td><td>E001</td><td>S003</td><td>2</td><td>3</td><td>1</td><td>1</td><td>1</td></tr>"
        "<tr><td>EV003</td><td>Paciente equivocado</td><td>Confusión de identidad del paciente</td><td>E002</td><td>S004</td><td>1</td><td>3</td><td>1</td><td>1</td><td>1</td></tr>"
        "</table>"
        "<h3>Interpretación del campo \"Aplicable\"</h3>"
        "<p><b>MUY IMPORTANTE:</b> El campo \"Aplicable\" NO significa \"ya completado\" o \"ya ocurrió\". "
        "Significa: <b>\"Este evento es relevante y aplicable a nuestro servicio o departamento de radioterapia\"</b>.</p>"
        "<ul>"
        "<li><b>Aplicable = 1:</b> El evento es posible en nuestro contexto y debe incluirse en el análisis</li>"
        "<li><b>Aplicable = 0:</b> El evento no es relevante para nuestro servicio (por ejemplo, eventos "
        "relacionados con técnicas que no utilizamos)</li>"
        "</ul>"
        "<p>Por ejemplo: Si su servicio no realiza braquiterapia, los eventos relacionados con braquiterapia "
        "deberían marcarse como No Aplicables (0).</p>"));
    
    // Section 3.3.4: Excel Sheet 4
    QTreeWidgetItem *excelSheet4Item = new QTreeWidgetItem(fileImportItem);
    excelSheet4Item->setText(0, tr("3.3.4 Hoja \"Reductores de frecuencia\""));
    addDocumentationSection(tr("3.3.4 Hoja \"Reductores de frecuencia\""),
        tr("<h2>Hoja 4: \"Reductores de frecuencia\"</h2>"
        "<p>Esta hoja contiene los reductores de frecuencia del modelo. Estas defensas reducen "
        "la frecuencia de ocurrencia de los eventos.</p>"
        "<h3>Estructura de columnas</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Columna</th><th>Tipo</th><th>Obligatorio</th><th>Descripción</th><th>Ejemplo</th></tr>"
        "<tr><td><b>Id</b></td><td>Número</td><td>Sí</td><td>Identificador único del reductor</td><td>1, 2, 3...</td></tr>"
        "<tr><td><b>Suceso</b></td><td>Número</td><td>Sí</td><td>ID del evento al que pertenece</td><td>1, 5, 12...</td></tr>"
        "<tr><td><b>Código</b></td><td>Texto</td><td>Sí</td><td>Código único del reductor</td><td>FMB-001, FMB-002</td></tr>"
        "<tr><td><b>Nombre</b></td><td>Texto</td><td>Sí</td><td>Nombre del reductor</td><td>Protocolo escrito</td></tr>"
        "<tr><td><b>Descripción</b></td><td>Texto</td><td>No</td><td>Descripción detallada</td><td>Protocolo documentado del proceso</td></tr>"
        "<tr><td><b>Robustez</b></td><td>Número</td><td>Sí</td><td>Nivel de robustez del reductor</td><td>-1, 0, 1, 2, 3</td></tr>"
        "<tr><td><b>Marcado</b></td><td>Número</td><td>Sí</td><td>¿Está activo para este evento?</td><td>0=No, 1=Sí</td></tr>"
        "<tr><td><b>Orden</b></td><td>Número</td><td>No</td><td>Orden de visualización</td><td>1, 2, 3...</td></tr>"
        "</table>"
        "<h3>Valores de Robustez</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse;'>"
        "<tr style='background-color: #f0f0f0;'><th>Valor</th><th>Nivel</th><th>Descripción</th></tr>"
        "<tr><td><b>-1</b></td><td>Muy blanda</td><td>Reductor con efecto muy limitado</td></tr>"
        "<tr><td><b>0</b></td><td>Blanda</td><td>Reductor con efecto limitado</td></tr>"
        "<tr><td><b>1</b></td><td>Normal</td><td>Reductor con efecto moderado</td></tr>"
        "<tr><td><b>2</b></td><td>Robusta</td><td>Reductor efectivo</td></tr>"
        "<tr><td><b>3</b></td><td>Muy robusta</td><td>Reductor muy efectivo</td></tr>"
        "</table>"
        "<h3>Ejemplo de contenido</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%; font-size: 0.85em;'>"
        "<tr style='background-color: #e3f2fd;'><th>Id</th><th>Suceso</th><th>Código</th><th>Nombre</th><th>Descripción</th><th>Robustez</th><th>Marcado</th><th>Orden</th></tr>"
        "<tr><td>1</td><td>1</td><td>FMB-001</td><td>Protocolo escrito</td><td>Protocolo documentado del proceso</td><td>1</td><td>1</td><td>1</td></tr>"
        "<tr><td>2</td><td>1</td><td>FMB-002</td><td>Formación específica</td><td>Formación regular del personal</td><td>1</td><td>1</td><td>2</td></tr>"
        "<tr><td>3</td><td>2</td><td>FMB-003</td><td>Checklist</td><td>Lista de verificación sistemática</td><td>2</td><td>1</td><td>1</td></tr>"
        "</table>"
        "<h3>Notas importantes</h3>"
        "<ul>"
        "<li>Cada fila representa un reductor de frecuencia asociado a un evento específico</li>"
        "<li>El campo \"Suceso\" debe corresponder a un ID válido de la hoja \"Sucesos\"</li>"
        "<li>\"Marcado = 1\" indica que el reductor está activo para ese evento</li>"
        "<li>Un evento puede tener múltiples reductores de frecuencia</li>"
        "</ul>"));
    
    // Section 3.3.5: Excel Sheet 5
    QTreeWidgetItem *excelSheet5Item = new QTreeWidgetItem(fileImportItem);
    excelSheet5Item->setText(0, tr("3.3.5 Hoja \"Barreras\""));
    addDocumentationSection(tr("3.3.5 Hoja \"Barreras\""),
        tr("<h2>Hoja 5: \"Barreras\"</h2>"
        "<p>Esta hoja contiene las barreras del modelo. Las barreras reducen la probabilidad de que "
        "el evento cause consecuencias.</p>"
        "<h3>Estructura de columnas</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Columna</th><th>Tipo</th><th>Obligatorio</th><th>Descripción</th><th>Ejemplo</th></tr>"
        "<tr><td><b>Id</b></td><td>Número</td><td>Sí</td><td>Identificador único de la barrera</td><td>1, 2, 3...</td></tr>"
        "<tr><td><b>Suceso</b></td><td>Número</td><td>Sí</td><td>ID del evento al que pertenece</td><td>1, 5, 12...</td></tr>"
        "<tr><td><b>Código</b></td><td>Texto</td><td>Sí</td><td>Código único de la barrera</td><td>BAR-001, BAR-002</td></tr>"
        "<tr><td><b>Nombre</b></td><td>Texto</td><td>Sí</td><td>Nombre de la barrera</td><td>Doble verificación</td></tr>"
        "<tr><td><b>Descripción</b></td><td>Texto</td><td>No</td><td>Descripción detallada</td><td>Verificación independiente por segundo profesional</td></tr>"
        "<tr><td><b>Robustez</b></td><td>Número</td><td>Sí</td><td>Nivel de robustez de la barrera</td><td>-1, 0, 1, 2, 3</td></tr>"
        "<tr><td><b>Marcado</b></td><td>Número</td><td>Sí</td><td>¿Está activa para este evento?</td><td>0=No, 1=Sí</td></tr>"
        "<tr><td><b>Orden</b></td><td>Número</td><td>No</td><td>Orden de visualización</td><td>1, 2, 3...</td></tr>"
        "</table>"
        "<h3>Valores de Robustez</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse;'>"
        "<tr style='background-color: #f0f0f0;'><th>Valor</th><th>Nivel</th><th>Descripción</th></tr>"
        "<tr><td><b>-1</b></td><td>Muy blanda</td><td>Barrera de eficacia muy limitada o muy fácilmente evitable</td></tr>"
        "<tr><td><b>0</b></td><td>Blanda</td><td>Barrera de eficacia limitada o fácilmente evitable</td></tr>"
        "<tr><td><b>1</b></td><td>Normal</td><td>Barrera con eficacia moderada</td></tr>"
        "<tr><td><b>2</b></td><td>Robusta</td><td>Barrera eficaz y difícil de eludir</td></tr>"
        "<tr><td><b>3</b></td><td>Muy robusta</td><td>Barrera muy eficaz y muy difícil de eludir</td></tr>"
        "</table>"
        "<h3>Ejemplo de contenido</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%; font-size: 0.85em;'>"
        "<tr style='background-color: #e3f2fd;'><th>Id</th><th>Suceso</th><th>Código</th><th>Nombre</th><th>Descripción</th><th>Robustez</th><th>Marcado</th><th>Orden</th></tr>"
        "<tr><td>1</td><td>1</td><td>BAR-001</td><td>Doble verificación</td><td>Verificación independiente por segundo profesional</td><td>2</td><td>1</td><td>1</td></tr>"
        "<tr><td>2</td><td>1</td><td>BAR-002</td><td>Sistema informático</td><td>Sistema de verificación automática</td><td>2</td><td>1</td><td>2</td></tr>"
        "<tr><td>3</td><td>2</td><td>BAR-003</td><td>Alarmas</td><td>Sistema de alertas automáticas</td><td>1</td><td>1</td><td>1</td></tr>"
        "</table>"
        "<h3>Notas importantes</h3>"
        "<ul>"
        "<li>Cada fila representa una barrera asociada a un evento específico</li>"
        "<li>El campo \"Suceso\" debe corresponder a un ID válido de la hoja \"Sucesos\"</li>"
        "<li>\"Marcado = 1\" indica que la barrera está activa para ese evento</li>"
        "<li>Un evento puede tener múltiples barreras</li>"
        "</ul>"));
    
    // Section 3.3.6: Excel Sheet 6
    QTreeWidgetItem *excelSheet6Item = new QTreeWidgetItem(fileImportItem);
    excelSheet6Item->setText(0, tr("3.3.6 Hoja \"Reductores de consecuencia\""));
    addDocumentationSection(tr("3.3.6 Hoja \"Reductores de consecuencia\""),
        tr("<h2>Hoja 6: \"Reductores de consecuencia\"</h2>"
        "<p>Esta hoja contiene los reductores de consecuencia del modelo. Estas defensas mitigan "
        "las consecuencias si el evento ocurre.</p>"
        "<h3>Estructura de columnas</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%;'>"
        "<tr style='background-color: #e3f2fd;'><th>Columna</th><th>Tipo</th><th>Obligatorio</th><th>Descripción</th><th>Ejemplo</th></tr>"
        "<tr><td><b>Id</b></td><td>Número</td><td>Sí</td><td>Identificador único del reductor</td><td>1, 2, 3...</td></tr>"
        "<tr><td><b>Suceso</b></td><td>Número</td><td>Sí</td><td>ID del evento al que pertenece</td><td>1, 5, 12...</td></tr>"
        "<tr><td><b>Código</b></td><td>Texto</td><td>Sí</td><td>Código único del reductor</td><td>RMA-001, RMA-002</td></tr>"
        "<tr><td><b>Nombre</b></td><td>Texto</td><td>Sí</td><td>Nombre del reductor</td><td>Imágenes diarias</td></tr>"
        "<tr><td><b>Descripción</b></td><td>Texto</td><td>No</td><td>Descripción detallada</td><td>Verificación con imagen portal diaria</td></tr>"
        "<tr><td><b>Robustez</b></td><td>Número</td><td>Sí</td><td>Nivel de robustez del reductor</td><td>-1, 0, 1, 2, 3</td></tr>"
        "<tr><td><b>Marcado</b></td><td>Número</td><td>Sí</td><td>¿Está activo para este evento?</td><td>0=No, 1=Sí</td></tr>"
        "<tr><td><b>Orden</b></td><td>Número</td><td>No</td><td>Orden de visualización</td><td>1, 2, 3...</td></tr>"
        "</table>"
        "<h3>Valores de Robustez</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse;'>"
        "<tr style='background-color: #f0f0f0;'><th>Valor</th><th>Nivel</th><th>Descripción</th></tr>"
        "<tr><td><b>-1</b></td><td>Muy blanda</td><td>Reductor con efecto muy limitado</td></tr>"
        "<tr><td><b>0</b></td><td>Blanda</td><td>Reductor con efecto limitado</td></tr>"
        "<tr><td><b>1</b></td><td>Normal</td><td>Reductor con efecto moderado</td></tr>"
        "<tr><td><b>2</b></td><td>Robusta</td><td>Reductor efectivo</td></tr>"
        "<tr><td><b>3</b></td><td>Muy robusta</td><td>Reductor muy efectivo</td></tr>"
        "</table>"
        "<h3>Ejemplo de contenido</h3>"
        "<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse; width: 100%; font-size: 0.85em;'>"
        "<tr style='background-color: #e3f2fd;'><th>Id</th><th>Suceso</th><th>Código</th><th>Nombre</th><th>Descripción</th><th>Robustez</th><th>Marcado</th><th>Orden</th></tr>"
        "<tr><td>1</td><td>1</td><td>RMA-001</td><td>Imágenes diarias</td><td>Verificación con imagen portal diaria</td><td>2</td><td>1</td><td>1</td></tr>"
        "<tr><td>2</td><td>1</td><td>RMA-002</td><td>Plan de contingencia</td><td>Protocolo de actuación ante eventos adversos</td><td>1</td><td>1</td><td>2</td></tr>"
        "<tr><td>3</td><td>2</td><td>RMA-003</td><td>Seguimiento estrecho</td><td>Control radiológico frecuente</td><td>2</td><td>0</td><td>1</td></tr>"
        "</table>"
        "<h3>Notas importantes</h3>"
        "<ul>"
        "<li>Cada fila representa un reductor de consecuencia asociado a un evento específico</li>"
        "<li>El campo \"Suceso\" debe corresponder a un ID válido de la hoja \"Sucesos\"</li>"
        "<li>\"Marcado = 1\" indica que el reductor está activo para ese evento</li>"
        "<li>Un evento puede tener múltiples reductores de consecuencia</li>"
        "</ul>"));
    
    // Section 3.4: Export to Excel
    QTreeWidgetItem *fileExportItem = new QTreeWidgetItem(fileMenuSection);
    fileExportItem->setText(0, tr("3.4 Exportar a Excel"));
    addDocumentationSection(tr("3.4 Exportar a Excel"),
        tr("<h2>Exportar a Excel</h2>"
        "<p>Exporta el modelo actual a un archivo Excel (.xlsx) con exactamente la misma estructura "
        "descrita en la sección de importación.</p>"
        "<h3>¿Cuándo usar la exportación?</h3>"
        "<ul>"
        "<li><b>Como plantilla:</b> Exportar un modelo existente para usarlo como base para crear uno nuevo</li>"
        "<li><b>Para análisis externo:</b> Procesar los datos en Excel, SPSS, R u otras herramientas</li>"
        "<li><b>Para compartir:</b> Distribuir el modelo a colegas en un formato universal</li>"
        "<li><b>Para documentación:</b> Generar un respaldo del modelo en formato legible</li>"
        "<li><b>Para edición masiva:</b> Modificar muchos eventos o defensas simultáneamente en Excel "
        "y luego reimportar</li>"
        "</ul>"
        "<h3>Proceso de exportación</h3>"
        "<ol>"
        "<li>Seleccione <b>Archivo → Exportar a Excel...</b></li>"
        "<li>Elija la ubicación y nombre del archivo .xlsx</li>"
        "<li>MARRTA genera un archivo Excel con las 6 hojas estándar</li>"
        "<li>Puede abrir el archivo inmediatamente o guardarlo para uso posterior</li>"
        "</ol>"
        "<h3>Contenido del archivo exportado</h3>"
        "<p>El archivo Excel generado contiene:</p>"
        "<ul>"
        "<li><b>Hoja 1 - Etapas:</b> Todas las etapas con sus códigos, nombres, descripciones y orden</li>"
        "<li><b>Hoja 2 - Subetapas:</b> Todas las subetapas vinculadas a sus etapas padre</li>"
        "<li><b>Hoja 3 - Sucesos:</b> Todos los sucesos iniciadores con su información completa, "
        "incluyendo frecuencia base, consecuencia base, y estado de aplicabilidad</li>"
        "<li><b>Hoja 4 - Reductores de frecuencia:</b> Todos los reductores de frecuencia asociados a eventos</li>"
        "<li><b>Hoja 5 - Barreras:</b> Todas las barreras asociadas a eventos</li>"
        "<li><b>Hoja 6 - Reductores de consecuencia:</b> Todos los reductores de consecuencia asociados a eventos</li>"
        "</ul>"
        "<p><b>Importante:</b> Los valores exportados reflejan el estado actual del modelo en MARRTA, "
        "incluyendo qué eventos están marcados como aplicables y qué defensas están activas.</p>"
        "<h3>Ejemplo de flujo de trabajo</h3>"
        "<ol>"
        "<li>Exportar modelo actual a \"modelo_base.xlsx\"</li>"
        "<li>Abrir el archivo en Excel</li>"
        "<li>Duplicar la hoja \"Sucesos\" y nombrarla \"Sucesos_Modificados\"</li>"
        "<li>Realizar cambios en la nueva hoja (agregar eventos, modificar frecuencias, etc.)</li>"
        "<li>Eliminar la hoja \"Sucesos\" original y renombrar \"Sucesos_Modificados\" a \"Sucesos\"</li>"
        "<li>Guardar el archivo como \"modelo_actualizado.xlsx\"</li>"
        "<li>En MARRTA, usar <b>Archivo → Importar desde Excel...</b> con \"modelo_actualizado.xlsx\"</li>"
        "</ol>"));
    
    // Section 3.5: Close and Exit
    QTreeWidgetItem *fileCloseItem = new QTreeWidgetItem(fileMenuSection);
    fileCloseItem->setText(0, tr("3.5 Cerrar y Salir"));
    addDocumentationSection(tr("3.5 Cerrar y Salir"),
        tr("<h2>Cerrar y Salir</h2>"
        "<h3>Cerrar</h3>"
        "<p>Cierra el modelo actualmente abierto sin cerrar la aplicación MARRTA.</p>"
        "<ul>"
        "<li>Si hay cambios sin guardar, se le preguntará si desea guardarlos</li>"
        "<li>La ventana principal queda vacía, lista para abrir otro modelo</li>"
        "<li>Los archivos recientes permanecen disponibles en el menú</li>"
        "</ul>"
        "<h3>Salir</h3>"
        "<p>Cierra completamente la aplicación MARRTA.</p>"
        "<ul>"
        "<li>Si hay cambios sin guardar, se le preguntará si desea guardarlos</li>"
        "<li>La configuración de la aplicación se guarda automáticamente</li>"
        "<li>La lista de archivos recientes se preserva para la próxima sesión</li>"
        "</ul>"
        "<p><b>Atajo de teclado:</b> Alt+F4 (Windows/Linux) o Cmd+Q (macOS) para salir rápidamente.</p>"));
    
    addDocumentationSection(tr("3. Menú Archivo"),
        tr("<h2>Menú Archivo - Resumen General</h2>"
        "<p>El menú Archivo es el centro de control para gestionar los archivos de modelo de MARRTA. "
        "Proporciona todas las funciones necesarias para crear, abrir, guardar e intercambiar modelos de análisis de riesgo.</p>"
        "<h3>Opciones disponibles</h3>"
        "<ul>"
        "<li><b>Nuevo modelo:</b> Crear un modelo desde cero con el asistente guiado</li>"
        "<li><b>Abrir:</b> Cargar un modelo existente (.praxis)</li>"
        "<li><b>Archivos recientes:</b> Acceso rápido a los últimos 5 archivos usados</li>"
        "<li><b>Guardar / Guardar como:</b> Preservar cambios y crear versiones alternativas</li>"
        "<li><b>Importar desde Excel:</b> Cargar modelos completos desde archivos .xlsx</li>"
        "<li><b>Exportar a Excel:</b> Generar archivos .xlsx para análisis externo o compartir</li>"
        "<li><b>Cerrar:</b> Cerrar el modelo actual</li>"
        "<li><b>Salir:</b> Cerrar la aplicación</li>"
        "</ul>"
        "<p>Explore las subsecciones en el panel izquierdo para obtener información detallada sobre cada opción, "
        "especialmente la sección de <b>Importar desde Excel</b> que incluye especificaciones completas del formato "
        "con ejemplos de las 5 hojas requeridas.</p>"));
    
    // Section 4: Model menu
    QTreeWidgetItem *modelMenuSection = new QTreeWidgetItem(sectionTree);
    modelMenuSection->setText(0, tr("4. Menú Modelo"));
    addDocumentationSection(tr("4. Menú Modelo"),
        tr("<h2>Menú Modelo</h2>"
        "<p>El menú Modelo contiene opciones para manipular los sucesos iniciadores y defensas del modelo actual.</p>"
        "<h3>Marcar todos los iniciadores como aplicables</h3>"
        "<p>Marca todos los sucesos iniciadores del modelo como aplicables al departamento. "
        "<b>Esta operación afecta SOLO a los iniciadores que están visibles según los filtros actuales de la ventana principal.</b></p>"
        "<p><b>Importante:</b> Si tiene filtros activos (por nombre, etapa, frecuencia, etc.), "
        "solo se marcarán los eventos que cumplan con esos filtros y estén visibles en la lista.</p>"
        "<h3>Marcar todos los iniciadores como no aplicables</h3>"
        "<p>Desmarca todos los sucesos iniciadores como no aplicables. "
        "<b>Esta operación también respeta los filtros activos y solo afecta a los eventos visibles.</b></p>"
        "<h3>Marcar/Desmarcar defensas</h3>"
        "<p>Las siguientes opciones permiten activar o desactivar defensas en bloque:</p>"
        "<ul>"
        "<li><b>Marcar todas las defensas:</b> Activa todas las defensas del modelo</li>"
        "<li><b>Desmarcar todas las defensas:</b> Desactiva todas las defensas</li>"
        "<li><b>Marcar reductores de frecuencia:</b> Activa solo reductores de frecuencia</li>"
        "<li><b>Desmarcar reductores de frecuencia:</b> Desactiva solo reductores de frecuencia</li>"
        "<li><b>Marcar barreras:</b> Activa solo barreras</li>"
        "<li><b>Desmarcar barreras:</b> Desactiva solo barreras</li>"
        "<li><b>Marcar reductores de consecuencia:</b> Activa solo reductores de consecuencia</li>"
        "<li><b>Desmarcar reductores de consecuencia:</b> Desactiva solo reductores de consecuencia</li>"
        "</ul>"
        "<p><b>Nota crítica sobre defensas:</b> Estas opciones de marcar/desmarcar defensas "
        "afectan a TODAS las defensas del modelo, sin importar qué evento esté seleccionado o qué filtros estén activos. "
        "Son operaciones globales sobre el conjunto completo de defensas.</p>"
        "<p><b>Para filtrar y manipular defensas específicas:</b> "
        "Si necesita activar/desactivar solo ciertas defensas (por ejemplo, las asociadas a una etapa específica), "
        "utilice la opción <b>Administración → Defensas</b>, donde puede filtrar las defensas y "
        "seleccionar/deseleccionar grupos específicos de defensas de manera más granular.</p>"
        "<h3>Información del modelo</h3>"
        "<p>Muestra información general del modelo cargado: nombre, descripción, número de etapas, "
        "número de eventos, número de defensas, etc.</p>"));
    
    // Section 5: Tools menu
    QTreeWidgetItem *toolsMenuSection = new QTreeWidgetItem(sectionTree);
    toolsMenuSection->setText(0, tr("5. Menú Herramientas"));
    addDocumentationSection(tr("5. Menú Herramientas"),
        tr("<h2>Menú Herramientas</h2>"
        "<p>El menú Herramientas proporciona acceso a funcionalidades avanzadas de análisis y automatización.</p>"
        "<h3>Resumen</h3>"
        "<p>Abre una ventana con una tabla resumen de todos los eventos aplicables del modelo, mostrando:</p>"
        "<ul>"
        "<li>Código y nombre del evento</li>"
        "<li>Etapa y subetapa asociada</li>"
        "<li>Valores calculados de Frecuencia (F), Probabilidad (P), Consecuencia (C)</li>"
        "<li>Nivel de riesgo resultante (R)</li>"
        "<li>Indicador visual con color según el nivel de riesgo</li>"
        "</ul>"
        "<p>La tabla permite ordenar por cualquier columna haciendo clic en el encabezado.</p>"
        "<h3>Generar informe HTML</h3>"
        "<p>Genera un informe completo en formato HTML que incluye:</p>"
        "<ul>"
        "<li>Descripción del modelo y metodología aplicada</li>"
        "<li>Matriz de riesgo configurada</li>"
        "<li>Análisis detallado de cada evento aplicable con sus defensas</li>"
        "<li>Gráficos de distribución de riesgo, frecuencia y consecuencia</li>"
        "<li>Análisis por etapa con estadísticas</li>"
        "<li>Lista completa de defensas implementadas</li>"
        "</ul>"
        "<p>Puede configurar opciones del informe (incluir gráficos, nivel de detalle) antes de generarlo. "
        "El informe HTML puede abrirse en cualquier navegador web y es ideal para documentación oficial, "
        "presentaciones y distribución.</p>"
        "<h3>Generar flujograma</h3>"
        "<p>Crea una representación gráfica del proceso con eventos iniciadores y defensas asociadas. "
        "El flujograma muestra el flujo del proceso a través de las etapas, con los eventos y las "
        "medidas de seguridad en cada punto. Se genera en formato de imagen (PNG) y puede ser útil "
        "para visualizar el modelo completo de manera esquemática.</p>"
        "<h3>Editor de scripts (JavaScript)</h3>"
        "<p>Abre el editor de scripts JavaScript que permite automatizar tareas en MARRTA. "
        "El motor de scripting JavaScript proporciona acceso a:</p>"
        "<ul>"
        "<li><b>Objeto Marrta:</b> Control de la aplicación</li>"
        "<li><b>Objeto Events:</b> Manipulación de eventos iniciadores</li>"
        "<li><b>Objeto Defenses:</b> Gestión de defensas</li>"
        "<li><b>Objeto Stages:</b> Acceso a etapas y subetapas</li>"
        "<li><b>Objeto Charts:</b> Creación de gráficos y tablas</li>"
        "<li><b>Objeto console:</b> Salida de mensajes de depuración</li>"
        "</ul>"
        "<p>El editor incluye resaltado de sintaxis, autocompletado y una consola de salida. "
        "Puede guardar y cargar scripts para reutilizarlos. Consulte la documentación del sistema "
        "de scripting desde el propio editor para ver ejemplos y referencia de API.</p>"
        "<h3>Editor de scripts Python</h3>"
        "<p>Similar al editor de JavaScript, pero utilizando Python como lenguaje de scripting. "
        "<b>Requisito: Python 3.12 debe estar instalado en el sistema</b> para que esta funcionalidad esté disponible.</p>"
        "<p>El motor de scripting Python proporciona acceso a:</p>"
        "<ul>"
        "<li><b>Módulo Marrta:</b> Funciones de control de la aplicación</li>"
        "<li><b>Módulo Database:</b> Acceso directo a la base de datos SQLite del modelo</li>"
        "<li><b>Módulo Etapas:</b> Gestión de etapas con métodos como Etapa.Imprimir()</li>"
        "</ul>"
        "<p>Python es especialmente útil para análisis estadísticos avanzados, procesamiento de datos "
        "y generación de informes personalizados. Puede importar bibliotecas estándar de Python "
        "como pandas, matplotlib, numpy, etc. (si están instaladas en su sistema).</p>"));
    
    // Section 6: Administration menu
    QTreeWidgetItem *adminMenuSection = new QTreeWidgetItem(sectionTree);
    adminMenuSection->setText(0, tr("6. Menú Administración"));
    addDocumentationSection(tr("6. Menú Administración"),
        tr("<h2>Menú Administración</h2>"
        "<p>El menú Administración proporciona acceso a ventanas especializadas para gestionar "
        "los elementos estructurales del modelo.</p>"
        "<h3>Etapas</h3>"
        "<p>Abre la ventana de gestión de etapas donde puede:</p>"
        "<ul>"
        "<li><b>Ver todas las etapas:</b> Lista completa de etapas definidas en el modelo</li>"
        "<li><b>Agregar nueva etapa:</b> Crear una nueva etapa del proceso</li>"
        "<li><b>Editar etapa:</b> Modificar nombre, código, descripción y orden de una etapa existente</li>"
        "<li><b>Eliminar etapa:</b> Borrar una etapa (solo si no tiene eventos asociados)</li>"
        "<li><b>Gestionar subetapas:</b> Ver, agregar, editar y eliminar subetapas de cada etapa</li>"
        "<li><b>Reordenar etapas:</b> Cambiar el orden de visualización de las etapas</li>"
        "</ul>"
        "<p>Esta ventana es esencial para estructurar el proceso de radioterapia en las fases "
        "específicas de su departamento.</p>"
        "<h3>Sucesos iniciadores</h3>"
        "<p>Abre la ventana de gestión de sucesos iniciadores donde puede:</p>"
        "<ul>"
        "<li><b>Ver todos los eventos:</b> Lista completa con filtros por etapa, nombre, etc.</li>"
        "<li><b>Agregar nuevo evento:</b> Crear un suceso iniciador especificando:</li>"
        "<ul>"
        "<li>Código único</li>"
        "<li>Nombre descriptivo</li>"
        "<li>Descripción detallada</li>"
        "<li>Etapa y subetapa asociada</li>"
        "<li>Frecuencia base (0-3)</li>"
        "<li>Consecuencia base (0-3)</li>"
        "<li>Si es aplicable al departamento</li>"
        "<li>Si involucra error humano</li>"
        "</ul>"
        "<li><b>Editar evento:</b> Modificar cualquier propiedad de un evento existente</li>"
        "<li><b>Eliminar evento:</b> Borrar un evento del modelo (se eliminarán también sus relaciones con defensas)</li>"
        "<li><b>Buscar y filtrar:</b> Localizar eventos por diversos criterios</li>"
        "<li><b>Importar/Exportar eventos:</b> Transferir eventos entre modelos</li>"
        "</ul>"
        "<h3>Defensas</h3>"
        "<p>Abre la ventana de gestión de defensas donde puede:</p>"
        "<ul>"
        "<li><b>Ver todas las defensas:</b> Lista completa de defensas con filtros por tipo</li>"
        "<li><b>Agregar nueva defensa:</b> Crear una defensa especificando:</li>"
        "<ul>"
        "<li>Código único (FMB-xxx, BAR-xxx, RMA-xxx según el tipo)</li>"
        "<li>Nombre descriptivo</li>"
        "<li>Tipo: Reductor de Frecuencia, Barrera, o Reductor de Consecuencia</li>"
        "<li>Robustez: Muy blanda, Blanda, Normal, Robusta, Muy robusta</li>"
        "<li>Descripción detallada</li>"
        "<li>Estado (activa/inactiva)</li>"
        "</ul>"
        "<li><b>Editar defensa:</b> Modificar propiedades de una defensa existente</li>"
        "<li><b>Eliminar defensa:</b> Borrar una defensa (se eliminarán también sus asociaciones con eventos)</li>"
        "<li><b>Asociar defensas a eventos:</b> Vincular/desvincular defensas con eventos específicos</li>"
        "<li><b>Activar/Desactivar en bloque:</b> Cambiar el estado de múltiples defensas a la vez</li>"
        "<li><b>Filtrar por tipo y estado:</b> Visualizar solo las defensas relevantes</li>"
        "</ul>"
        "<p><b>Nota:</b> Esta ventana es ideal cuando necesita manipular defensas de manera granular, "
        "por ejemplo, activar todas las barreras de cierta robustez, o todas las defensas asociadas "
        "a eventos de una etapa específica. Es más flexible que las opciones globales del menú Modelo.</p>"));
    
    // Section 7: Help menu
    QTreeWidgetItem *helpMenuSection = new QTreeWidgetItem(sectionTree);
    helpMenuSection->setText(0, tr("7. Menú Ayuda"));
    addDocumentationSection(tr("7. Menú Ayuda"),
        tr("<h2>Menú Ayuda</h2>"
        "<p>El menú Ayuda proporciona recursos de soporte y actualización.</p>"
        "<h3>Ayuda (Manual de usuario)</h3>"
        "<p>Abre este manual de usuario con documentación completa sobre el uso de MARRTA.</p>"
        "<h3>Comprobar actualizaciones...</h3>"
        "<p>Verifica si hay una nueva versión de MARRTA disponible en el repositorio oficial. "
        "Si hay una actualización disponible, se le mostrará:</p>"
        "<ul>"
        "<li>Número de versión actual y nueva versión</li>"
        "<li>Notas de la versión con cambios y mejoras</li>"
        "<li>Enlace para descargar la nueva versión</li>"
        "</ul>"
        "<p>Es recomendable mantener MARRTA actualizado para beneficiarse de correcciones de errores, "
        "nuevas funcionalidades y mejoras de seguridad.</p>"
        "<h3>Comprobar nuevos modelos...</h3>"
        "<p>Verifica si hay nuevos modelos de riesgo disponibles en el repositorio oficial de MARRTA. "
        "Los modelos disponibles pueden incluir:</p>"
        "<ul>"
        "<li>Modelos para diferentes modalidades de radioterapia (IMRT, VMAT, braquiterapia, etc.)</li>"
        "<li>Modelos actualizados con nuevos eventos o defensas</li>"
        "<li>Modelos de ejemplo para capacitación</li>"
        "<li>Modelos específicos por patología o técnica</li>"
        "</ul>"
        "<p>Si hay modelos nuevos, se le mostrará una lista con descripción y opción de descarga.</p>"
        "<h3>Enviar comentario o sugerencia</h3>"
        "<p>Permite enviar comentarios, sugerencias de mejora, o reportar problemas al equipo de desarrollo de MARRTA. "
        "Al seleccionar esta opción se abrirá un formulario donde puede:</p>"
        "<ul>"
        "<li>Seleccionar el tipo de comentario: Sugerencia, Pregunta, Problema, Otro</li>"
        "<li>Describir su comentario o problema en detalle</li>"
        "<li>Incluir información de contacto (opcional)</li>"
        "</ul>"
        "<p>El formulario se conecta con el sistema de issues de GitHub, donde el equipo de desarrollo "
        "puede revisar, responder y dar seguimiento a su comentario.</p>"
        "<h3>Reportar un bug</h3>"
        "<p>Opción específica para reportar errores o comportamientos inesperados en MARRTA. "
        "Al reportar un bug, proporcione:</p>"
        "<ul>"
        "<li>Descripción clara del problema</li>"
        "<li>Pasos para reproducir el error</li>"
        "<li>Comportamiento esperado vs. comportamiento actual</li>"
        "<li>Versión de MARRTA que está utilizando</li>"
        "<li>Sistema operativo</li>"
        "<li>Capturas de pantalla si es relevante</li>"
        "</ul>"
        "<p>Los bugs reportados se registran en el sistema de issues de GitHub para su seguimiento y resolución.</p>"
        "<h3>Aviso legal</h3>"
        "<p>Muestra el aviso legal y términos de uso de MARRTA, incluyendo información sobre "
        "responsabilidad, uso apropiado del software, y consideraciones legales.</p>"
        "<h3>Licencia de uso</h3>"
        "<p>Muestra la licencia bajo la cual se distribuye MARRTA, incluyendo derechos y restricciones.</p>"
        "<h3>Acerca de MARRTA</h3>"
        "<p>Muestra información sobre la aplicación: versión, autores, créditos, agradecimientos "
        "y enlaces relevantes.</p>"));
    
    // Select first item by default
    if (sectionTree->topLevelItemCount() > 0) {
        sectionTree->setCurrentItem(sectionTree->topLevelItem(0));
    }
}

void UserManualDocumentation::addDocumentationSection(const QString &title, const QString &content)
{
    DocumentationItem item;
    item.title = title;
    item.content = content;
    documentationItems.append(item);
}

void UserManualDocumentation::onSectionChanged()
{
    QTreeWidgetItem *currentItem = sectionTree->currentItem();
    if (!currentItem) return;
    
    QString sectionTitle = currentItem->text(0);
    
    // Find matching documentation item
    for (const DocumentationItem &item : documentationItems) {
        if (item.title == sectionTitle) {
            contentBrowser->setHtml(item.content);
            return;
        }
    }
    
    // If no exact match, show a placeholder
    contentBrowser->setHtml(tr("<h2>%1</h2><p>Contenido de esta sección.</p>").arg(sectionTitle));
}
