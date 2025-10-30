#include "scriptingdocumentation.h"
#include <QTreeWidgetItem>
#include <QHeaderView>

ScriptingDocumentation::ScriptingDocumentation(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Documentación del Sistema de Scripting");
    setMinimumSize(800, 600);
    resize(1000, 700);
    
    setupUI();
    populateDocumentation();
}

void ScriptingDocumentation::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create splitter for two-panel layout
    splitter = new QSplitter(Qt::Horizontal, this);
    
    // Left panel: Section tree
    sectionTree = new QTreeWidget();
    sectionTree->setHeaderLabel("Secciones");
    sectionTree->setMaximumWidth(300);
    sectionTree->setMinimumWidth(250);
    
    // Right panel: Content browser
    contentBrowser = new QTextBrowser();
    
    splitter->addWidget(sectionTree);
    splitter->addWidget(contentBrowser);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    
    // Bottom buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    insertButton = new QPushButton("Insertar Ejemplo");
    insertButton->setEnabled(false);
    closeButton = new QPushButton("Cerrar");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(insertButton);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addWidget(splitter);
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(sectionTree, &QTreeWidget::currentItemChanged, this, &ScriptingDocumentation::onSectionChanged);
    connect(insertButton, &QPushButton::clicked, this, &ScriptingDocumentation::onExampleInserted);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);
}

void ScriptingDocumentation::populateDocumentation()
{
    // Clear existing items
    sectionTree->clear();
    documentationItems.clear();
    
    // Introduction section
    QTreeWidgetItem *introItem = new QTreeWidgetItem(sectionTree);
    introItem->setText(0, "Introducción");
    addDocumentationSection("Introducción", 
        "<h2>Sistema de Scripting MARRTA</h2>"
        "<p>El sistema de scripting de MARRTA permite automatizar tareas y análisis mediante JavaScript. "
        "Proporciona acceso a los datos del modelo actual, incluyendo eventos iniciadores, defensas, etapas y estadísticas.</p>"
        "<h3>Objetos Disponibles</h3>"
        "<ul>"
        "<li><b>console</b>: Para mostrar mensajes en la consola</li>"
        "<li><b>Marrta</b>: Acceso a la ventana principal</li>"
        "<li><b>Events</b>: Gestión de eventos iniciadores</li>"
        "<li><b>Defenses</b>: Gestión de defensas (barreras, reductores)</li>"
        "<li><b>Stages</b>: Acceso a etapas y subetapas del modelo</li>"
        "<li><b>Charts</b>: Utilidades para crear gráficas y tablas</li>"
        "</ul>");
    
    // Events API section
    QTreeWidgetItem *eventsItem = new QTreeWidgetItem(sectionTree);
    eventsItem->setText(0, "API de Eventos");
    addDocumentationSection("API de Eventos",
        "<h2>Objeto Events</h2>"
        "<p>Proporciona acceso a los eventos iniciadores del modelo.</p>"
        "<h3>Métodos Disponibles</h3>"
        "<ul>"
        "<li><b>getAllEvents()</b>: Obtiene todos los eventos<br/>"
        "  Retorna: Array de objetos Event</li>"
        "<li><b>getEventsByStage(stageId, substageId)</b>: Eventos por etapa<br/>"
        "  Parámetros: ID de etapa (Number), ID de subetapa (Number, opcional, default 0)<br/>"
        "  Retorna: Array de objetos Event filtrados</li>"
        "<li><b>getEventsByStageGrouped()</b>: Eventos agrupados por etapa<br/>"
        "  Retorna: Object con nombres de etapa como claves y arrays de eventos como valores</li>"
        "<li><b>countEventsByStage()</b>: Cuenta eventos por etapa<br/>"
        "  Retorna: Object con nombres de etapa como claves y conteos como valores</li>"
        "<li><b>getEventsByConsequence()</b>: Eventos agrupados por consecuencia<br/>"
        "  Retorna: Object con niveles de consecuencia como claves y arrays de eventos como valores</li>"
        "<li><b>getEventsByRisk()</b>: Eventos agrupados por riesgo<br/>"
        "  Retorna: Object con niveles de riesgo como claves y arrays de eventos como valores</li>"
        "<li><b>getEventStatistics()</b>: Estadísticas generales<br/>"
        "  Retorna: Object con totalEvents, eventsByStage, eventsByRisk, humanErrorEvents, etc.</li>"
        "<li><b>updateEvent(eventId, properties)</b>: Actualizar evento<br/>"
        "  Parámetros: ID del evento, Object con propiedades a actualizar<br/>"
        "  Retorna: Boolean (true si exitoso)</li>"
        "<li><b>setAllEventsApplicable()</b>: Marcar todos como aplicables<br/>"
        "  Retorna: Number (cantidad de eventos actualizados)</li>"
        "<li><b>setAllEventsNotApplicable()</b>: Marcar todos como no aplicables<br/>"
        "  Retorna: Number (cantidad de eventos actualizados)</li>"
        "<li><b>searchReplaceInDescriptions(searchTerm, replaceTerm, caseSensitive)</b>: Buscar y reemplazar texto<br/>"
        "  Parámetros: Texto a buscar, texto de reemplazo, sensible a mayúsculas (Boolean, default false)<br/>"
        "  Retorna: Number (cantidad de eventos modificados)</li>"
        "</ul>"
        "<h3>Propiedades de Evento</h3>"
        "<p>Cada evento contiene:</p>"
        "<ul>"
        "<li><b>id</b>: Identificador único (Number)</li>"
        "<li><b>codigo</b>: Código del evento (String)</li>"
        "<li><b>nombre</b>: Nombre descriptivo (String)</li>"
        "<li><b>descripcion</b>: Descripción detallada (String)</li>"
        "<li><b>etapa</b>: Nombre de la etapa (String)</li>"
        "<li><b>etapaId</b>: ID de la etapa (Number)</li>"
        "<li><b>subetapa</b>: Nombre de la subetapa (String)</li>"
        "<li><b>subetapaId</b>: ID de la subetapa (Number)</li>"
        "<li><b>frecuenciaBase</b>: Nivel de frecuencia base del evento 0-3 (Number)</li>"
        "<li><b>consecuenciaBase</b>: Nivel de consecuencia base del evento 0-3 (Number)</li>"
        "<li><b>riesgoBase</b>: Nivel de riesgo base (asume probabilidad máxima PA=3) 0-3 (Number)</li>"
        "<li><b>riesgoBaseTexto</b>: Texto del riesgo base: RB, RM, RA, RMA (String)</li>"
        "<li><b>frecuencia</b>: Nivel de frecuencia calculado con defensas 0-3 (Number)</li>"
        "<li><b>frecuenciaTexto</b>: Texto de frecuencia: FMB (Muy baja), FB (Baja), FM (Media), FA (Alta) (String)</li>"
        "<li><b>probabilidad</b>: Nivel de probabilidad calculado desde barreras 0-3 (Number)</li>"
        "<li><b>probabilidadTexto</b>: Texto de probabilidad: PMB (Muy baja), PB (Baja), PM (Media), PA (Alta) (String)</li>"
        "<li><b>consecuencia</b>: Nivel de consecuencia calculado con defensas 0-3 (Number)</li>"
        "<li><b>consecuenciaTexto</b>: Texto de consecuencia: CB (Bajas), CM (Medias), CA (Altas), CMA (Muy altas) (String)</li>"
        "<li><b>errorHumano</b>: Indica si involucra error humano (Boolean)</li>"
        "<li><b>base</b>: Indica si es evento base (Boolean)</li>"
        "<li><b>riesgo</b>: Nivel de riesgo calculado desde F, P, C usando matriz de riesgo 0-3 (Number)</li>"
        "<li><b>riesgoTexto</b>: Descripción del nivel de riesgo: RB (Bajo), RM (Medio), RA (Alto), RMA (Muy Alto) (String)</li>"
        "</ul>"
        "<p><b>Nota:</b> Los valores de frecuencia, probabilidad, consecuencia y riesgo son CALCULADOS considerando las defensas activas. "
        "La probabilidad se calcula desde las barreras, la frecuencia desde los reductores de frecuencia, "
        "la consecuencia desde los reductores de consecuencia, y el riesgo desde la matriz de riesgo aplicada a P, F, C calculados.</p>");
    
    // Defenses API section
    QTreeWidgetItem *defensesItem = new QTreeWidgetItem(sectionTree);
    defensesItem->setText(0, "API de Defensas");
    addDocumentationSection("API de Defensas",
        "<h2>Objeto Defenses</h2>"
        "<p>Proporciona acceso a las defensas del modelo (barreras, reductores de frecuencia y consecuencia).</p>"
        "<h3>Métodos Disponibles</h3>"
        "<ul>"
        "<li><b>getAllDefenses(eventId)</b>: Obtiene todas las defensas<br/>"
        "  Parámetros: ID de evento (Number, opcional, 0 para todos los eventos)<br/>"
        "  Retorna: Array de objetos Defense</li>"
        "<li><b>getDefensesByType(type, eventId)</b>: Defensas por tipo<br/>"
        "  Parámetros: tipo (0=Frecuencia, 1=Barrera, 2=Consecuencia), ID de evento (opcional)<br/>"
        "  Retorna: Array de objetos Defense filtrados</li>"
        "<li><b>getBarriers(eventId)</b>: Solo barreras<br/>"
        "  Parámetros: ID de evento (Number, opcional)<br/>"
        "  Retorna: Array de barreras</li>"
        "<li><b>getFrequencyReducers(eventId)</b>: Solo reductores de frecuencia<br/>"
        "  Parámetros: ID de evento (Number, opcional)<br/>"
        "  Retorna: Array de reductores</li>"
        "<li><b>getConsequenceReducers(eventId)</b>: Solo reductores de consecuencia<br/>"
        "  Parámetros: ID de evento (Number, opcional)<br/>"
        "  Retorna: Array de reductores</li>"
        "<li><b>getDefensesByTypeGrouped(eventId)</b>: Defensas agrupadas por tipo<br/>"
        "  Parámetros: ID de evento (Number, opcional)<br/>"
        "  Retorna: Object con tipos como claves y arrays de defensas como valores</li>"
        "<li><b>setHumanBarriersNotApplicable()</b>: Marca barreras humanas como no aplicables<br/>"
        "  Retorna: Número de barreras modificadas (Number)</li>"
        "<li><b>setNonHumanBarriersApplicable()</b>: Marca barreras no humanas como aplicables<br/>"
        "  Retorna: Número de barreras modificadas (Number)</li>"
        "<li><b>getDefenseStatistics()</b>: Estadísticas de defensas<br/>"
        "  Retorna: Object con totalDefenses, activeDefenses, humanDefenses, etc.</li>"
        "</ul>"
        "<h3>Propiedades de Defensa</h3>"
        "<p>Cada defensa contiene:</p>"
        "<ul>"
        "<li><b>id</b>: Identificador único (Number)</li>"
        "<li><b>code</b>: Código de la defensa (String)</li>"
        "<li><b>nombre</b>: Nombre descriptivo (String)</li>"
        "<li><b>descripcion</b>: Descripción detallada (String)</li>"
        "<li><b>type</b>: Tipo numérico (0, 1, o 2) (Number)</li>"
        "<li><b>typeString</b>: Descripción del tipo (String)</li>"
        "<li><b>active</b>: Estado activo/inactivo (Boolean)</li>"
        "<li><b>hardiness</b>: Nivel de dureza (0-3) (Number)</li>"
        "<li><b>hardinessString</b>: Descripción de la dureza: Blanda, Normal, Robusta, Muy robusta (String)</li>"
        "<li><b>eventId</b>: ID del evento asociado (Number)</li>"
        "<li><b>orderStageId</b>: ID de etapa de orden (Number)</li>"
        "<li><b>orderStage</b>: Etapa de orden (String)</li>"
        "<li><b>isHuman</b>: Indica si es barrera humana (hardiness >= 3) (Boolean)</li>"
        "</ul>");
    
    // Charts API section
    QTreeWidgetItem *chartsItem = new QTreeWidgetItem(sectionTree);
    chartsItem->setText(0, "API de Gráficas");
    addDocumentationSection("API de Gráficas",
        "<h2>Objeto Charts</h2>"
        "<p>Proporciona utilidades para crear gráficas y tablas, así como para procesar datos.</p>"
        "<h3>Métodos de Visualización</h3>"
        "<ul>"
        "<li><b>generateTextBarChart(title, data, maxWidth)</b>: Gráfica de barras en texto<br/>"
        "  Parámetros: título (String), datos (Object con etiquetas:valores), ancho máximo (Number, default 50)<br/>"
        "  Retorna: String con la gráfica formateada</li>"
        "<li><b>generateTable(title, headers, rows)</b>: Tabla formateada<br/>"
        "  Parámetros: título (String), encabezados (Array), filas (Array de Arrays)<br/>"
        "  Retorna: String con la tabla formateada</li>"
        "<li><b>createBarChart(title, data)</b>: Estructura de datos para gráfica de barras<br/>"
        "  Parámetros: título (String), datos (Object con etiquetas:valores)<br/>"
        "  Retorna: Object con type, title, labels, values</li>"
        "<li><b>createPieChart(title, data)</b>: Estructura de datos para gráfica circular<br/>"
        "  Parámetros: título (String), datos (Object con etiquetas:valores)<br/>"
        "  Retorna: Object con type, title, segments, total</li>"
        "</ul>"
        "<h3>Métodos de Análisis</h3>"
        "<ul>"
        "<li><b>sortByProperty(array, property, ascending)</b>: Ordena array por propiedad<br/>"
        "  Parámetros: array (Array), propiedad (String), ascendente (Boolean, default true)<br/>"
        "  Retorna: Array ordenado</li>"
        "<li><b>groupByProperty(array, property)</b>: Agrupa array por propiedad<br/>"
        "  Parámetros: array (Array), propiedad (String)<br/>"
        "  Retorna: Object con valores de propiedad como claves y arrays como valores</li>"
        "<li><b>countByProperty(array, property)</b>: Cuenta por propiedad<br/>"
        "  Parámetros: array (Array), propiedad (String)<br/>"
        "  Retorna: Object con valores de propiedad como claves y conteos como valores</li>"
        "<li><b>filterByProperty(array, property, value)</b>: Filtra array<br/>"
        "  Parámetros: array (Array), propiedad (String), valor a buscar (Any)<br/>"
        "  Retorna: Array filtrado</li>"
        "<li><b>getNumericStats(array, property)</b>: Estadísticas numéricas<br/>"
        "  Parámetros: array (Array), propiedad numérica (String)<br/>"
        "  Retorna: Object con count, sum, min, max, avg</li>"
        "</ul>"
        "<h3>Ejemplos de Uso</h3>"
        "<pre style='background-color: #f5f5f5; padding: 10px;'>"
        "// Gráfica de barras\n"
        "var data = {'Cat1': 10, 'Cat2': 25, 'Cat3': 15};\n"
        "console.log(Charts.generateTextBarChart('Título', data));\n\n"
        "// Filtrar y contar\n"
        "var events = Events.getAllEvents();\n"
        "var highRisk = Charts.filterByProperty(events, 'riesgo', 3);\n"
        "var counts = Charts.countByProperty(events, 'etapa');\n"
        "</pre>");
    
    // Stages API section
    QTreeWidgetItem *stagesItem = new QTreeWidgetItem(sectionTree);
    stagesItem->setText(0, "API de Etapas");
    addDocumentationSection("API de Etapas",
        "<h2>Objeto Stages</h2>"
        "<p>Proporciona acceso a las etapas y subetapas del modelo de análisis de riesgos.</p>"
        "<h3>Métodos Disponibles</h3>"
        "<ul>"
        "<li><b>getAllStages()</b>: Obtiene todas las etapas del modelo</li>"
        "<li><b>getAllSubstages()</b>: Obtiene todas las subetapas</li>"
        "<li><b>getSubstagesByStage(stageId)</b>: Obtiene subetapas de una etapa específica</li>"
        "<li><b>getStagesWithSubstages()</b>: Etapas agrupadas con sus subetapas</li>"
        "<li><b>getStageById(stageId)</b>: Obtiene una etapa por su ID</li>"
        "<li><b>getSubstageById(substageId)</b>: Obtiene una subetapa por su ID</li>"
        "<li><b>countSubstagesByStage()</b>: Cuenta subetapas por etapa</li>"
        "</ul>"
        "<h3>Propiedades de Etapa</h3>"
        "<p>Cada etapa contiene: id, nombre, codigo, descripcion, orden</p>"
        "<h3>Propiedades de Subetapa</h3>"
        "<p>Cada subetapa contiene: id, nombre, descripcion, etapaId, activo, orden</p>");
    
    // Test Scripts section
    QTreeWidgetItem *testScriptsItem = new QTreeWidgetItem(sectionTree);
    testScriptsItem->setText(0, "Scripts de Ejemplo");
    addDocumentationSection("Scripts de Ejemplo",
        "<h2>Scripts de Ejemplo Predefinidos</h2>"
        "<p>MARRTA incluye varios scripts predefinidos en la carpeta <b>test_scripts/</b> que demuestran "
        "el uso de las APIs y proporcionan herramientas de análisis útiles.</p>"
        "<h3>Scripts de Prueba de API</h3>"
        "<ul>"
        "<li><b>test_marrta_api.js</b>: Verifica objetos globales disponibles y resumen del modelo cargado</li>"
        "<li><b>test_events_api.js</b>: Prueba completa de la API de eventos (Events)</li>"
        "<li><b>test_defenses_api.js</b>: Prueba completa de la API de defensas (Defenses)</li>"
        "<li><b>test_stages_api.js</b>: Prueba completa de la API de etapas (Stages)</li>"
        "<li><b>test_charts_api.js</b>: Prueba de utilidades de gráficas y análisis (Charts)</li>"
        "</ul>"
        "<h3>Scripts de Análisis y Consulta</h3>"
        "<ul>"
        "<li><b>event_details_by_code.js</b>: Busca un suceso por código y muestra información detallada "
        "en tablas (características, defensas por tipo, valores calculados)</li>"
        "<li><b>defense_list_grouped.js</b>: Lista todas las defensas únicas agrupadas por tipo, "
        "mostrando los códigos de sucesos donde aparecen y el total de apariciones</li>"
        "<li><b>defense_importance_analysis.js</b>: Análisis cuantitativo de importancia de defensas "
        "con 9 métricas (F-V, RAW, RRW, RII, RRI, B, etc.), calculando riesgos reales mediante modificación "
        "temporal del estado de defensas</li>"
        "</ul>"
        "<h3>Cómo Usar los Scripts</h3>"
        "<ol>"
        "<li>Cargar un modelo (.pdb) en MARRTA</li>"
        "<li>Ir a <i>Herramientas > Editor de Scripts</i></li>"
        "<li>Usar el botón \"Cargar\" para seleccionar un script de test_scripts/</li>"
        "<li>Hacer clic en \"Ejecutar\" para ejecutar el script</li>"
        "<li>Ver los resultados en la consola</li>"
        "</ol>"
        "<p><b>Nota:</b> Los scripts de análisis avanzado pueden tardar varios minutos en modelos grandes, "
        "ya que recalculan el modelo completo múltiples veces.</p>"
        "<p>Para más información, consultar <b>test_scripts/README.md</b> y <b>test_scripts/DEFENSE_SCRIPTS_GUIDE.md</b></p>");
    // Marrta API section
    QTreeWidgetItem *marrtaItem = new QTreeWidgetItem(sectionTree);
    marrtaItem->setText(0, "Objeto Marrta");
    addDocumentationSection("Objeto Marrta",
        "<h2>Objeto Marrta</h2>"
        "<p>Proporciona acceso a la ventana principal de la aplicación y funciones de control general.</p>"
        "<h3>Métodos de Control de Interfaz</h3>"
        "<ul>"
        "<li><b>refreshMainScreen()</b>: Refresca la pantalla principal<br/>"
        "  Actualiza la lista de eventos iniciadores y los detalles del evento seleccionado.<br/>"
        "  Recalcula y muestra valores calculados (frecuencia, probabilidad, consecuencia, riesgo).<br/>"
        "  <br/>"
        "  <b>Cuándo usar:</b> Después de modificar eventos, defensas o etapas mediante scripts,<br/>"
        "  para que los cambios se vean reflejados en la interfaz.</li>"
        "<li><b>saveModelChanges()</b>: Guarda los cambios en disco<br/>"
        "  Guarda todos los cambios pendientes en la base de datos sin mostrar diálogos de confirmación.<br/>"
        "  Ideal para scripts de procesamiento por lotes.<br/>"
        "  Persiste cambios en eventos, defensas y etapas.<br/>"
        "  <br/>"
        "  <b>Cuándo usar:</b> Después de hacer modificaciones mediante scripts que deseas<br/>"
        "  persistir en la base de datos.</li>"
        "</ul>"
        "<h3>Flujo de Trabajo Típico</h3>"
        "<pre style='background-color: #f5f5f5; padding: 10px;'>"
        "// 1. Hacer cambios en el modelo\n"
        "Events.setAllEventsApplicable();\n"
        "Defenses.setDefenseActive(someId, true);\n"
        "\n"
        "// 2. Refrescar la pantalla para ver los cambios\n"
        "Marrta.refreshMainScreen();\n"
        "\n"
        "// 3. Guardar los cambios en disco\n"
        "Marrta.saveModelChanges();\n"
        "</pre>"
        "<h3>Otros Métodos</h3>"
        "<ul>"
        "<li><b>toggleActualEvent()</b>: Alterna el estado del evento seleccionado</li>"
        "<li><b>alternarDefensasEventoActual(tipo, habilitar)</b>: Cambia el estado de las defensas del evento actual<br/>"
        "  Parámetros: tipo (0=Reductores Frecuencia, 1=Barreras, 2=Reductores Consecuencia), habilitar (Boolean)</li>"
        "<li><b>alternarDefensaModelo(tipo, habilitar, pos)</b>: Cambia el estado de defensas en todo el modelo</li>"
        "</ul>"
        "<p><b>Nota:</b> Los métodos toggleActualEvent y alternar* están pensados para uso interno.<br/>"
        "Para gestión desde scripts, es recomendable usar las APIs específicas (Events, Defenses, Stages)<br/>"
        "seguido de refreshMainScreen() y saveModelChanges() según sea necesario.</p>");
    
    // Examples section
    QTreeWidgetItem *examplesItem = new QTreeWidgetItem(sectionTree);
    examplesItem->setText(0, "Ejemplos");
    
    // Example 1: Events by stage with chart
    QTreeWidgetItem *example1Item = new QTreeWidgetItem(examplesItem);
    example1Item->setText(0, "1. Iniciadores por Etapa");
    addExampleSection("Ejemplo 1: Listar Iniciadores por Etapa con Gráfica",
        "// Obtener eventos agrupados por etapa\n"
        "var eventsByStage = Events.getEventsByStageGrouped();\n"
        "var counts = Events.countEventsByStage();\n"
        "\n"
        "// Mostrar gráfica de barras\n"
        "console.log(Charts.generateTextBarChart('Número de Iniciadores por Etapa', counts));\n"
        "\n"
        "// Mostrar detalle por etapa\n"
        "for (var stage in eventsByStage) {\n"
        "    console.log('\\n=== ' + stage + ' ===');\n"
        "    var events = eventsByStage[stage];\n"
        "    for (var i = 0; i < events.length; i++) {\n"
        "        var event = events[i];\n"
        "        console.log('- ' + event.codigo + ': ' + event.nombre);\n"
        "    }\n"
        "}",
        "Este ejemplo muestra una gráfica de barras con el número de iniciadores por etapa y luego lista los detalles de cada iniciador.");
    
    // Example 2: Events by consequence and stage
    QTreeWidgetItem *example2Item = new QTreeWidgetItem(examplesItem);
    example2Item->setText(0, "2. Iniciadores por Consecuencias");
    addExampleSection("Ejemplo 2: Lista de Iniciadores por Consecuencias y Etapas",
        "// Obtener eventos por consecuencia\n"
        "var eventsByConsequence = Events.getEventsByConsequence();\n"
        "var eventsByStage = Events.getEventsByStageGrouped();\n"
        "\n"
        "// Tabla de resumen por consecuencias\n"
        "console.log('\\n=== RESUMEN POR CONSECUENCIAS ===');\n"
        "for (var consequence in eventsByConsequence) {\n"
        "    var events = eventsByConsequence[consequence];\n"
        "    console.log(consequence + ': ' + events.length + ' iniciadores');\n"
        "}\n"
        "\n"
        "// Detalle por etapas y consecuencias\n"
        "console.log('\\n=== DETALLE POR ETAPAS Y CONSECUENCIAS ===');\n"
        "for (var stage in eventsByStage) {\n"
        "    console.log('\\n--- ' + stage + ' ---');\n"
        "    var events = eventsByStage[stage];\n"
        "    \n"
        "    // Agrupar por consecuencia dentro de la etapa\n"
        "    var stageByConsequence = Charts.groupByProperty(events, 'consecuencia');\n"
        "    for (var cons in stageByConsequence) {\n"
        "        var consEvents = stageByConsequence[cons];\n"
        "        console.log('  Consecuencia ' + cons + ':');\n"
        "        for (var i = 0; i < consEvents.length; i++) {\n"
        "            var event = consEvents[i];\n"
        "            console.log('    - ' + event.codigo + ': ' + event.nombre);\n"
        "        }\n"
        "    }\n"
        "}",
        "Este ejemplo agrupa los iniciadores por nivel de consecuencia y muestra un desglose detallado por etapas.");
    
    // Example 3: Set human barriers
    QTreeWidgetItem *example3Item = new QTreeWidgetItem(examplesItem);
    example3Item->setText(0, "3. Barreras Humanas No Aplicables");
    addExampleSection("Ejemplo 3: Poner Barreras Humanas como No Aplicables",
        "// Obtener estadísticas antes del cambio\n"
        "var statsBefore = Defenses.getDefenseStatistics();\n"
        "console.log('Estadísticas ANTES del cambio:');\n"
        "console.log('- Defensas activas: ' + statsBefore.activeDefenses);\n"
        "console.log('- Defensas humanas: ' + statsBefore.humanDefenses);\n"
        "\n"
        "// Realizar el cambio\n"
        "var changedCount = Defenses.setHumanBarriersNotApplicable();\n"
        "console.log('\\nSe modificaron ' + changedCount + ' barreras humanas.');\n"
        "\n"
        "// Obtener estadísticas después del cambio\n"
        "var statsAfter = Defenses.getDefenseStatistics();\n"
        "console.log('\\nEstadísticas DESPUÉS del cambio:');\n"
        "console.log('- Defensas activas: ' + statsAfter.activeDefenses);\n"
        "console.log('- Defensas inactivas: ' + statsAfter.inactiveDefenses);\n"
        "\n"
        "// Mostrar detalles de barreras humanas\n"
        "console.log('\\n=== BARRERAS HUMANAS (Dureza >= 3) ===');\n"
        "var allBarriers = Defenses.getBarriers();\n"
        "var humanBarriers = Charts.filterByProperty(allBarriers, 'isHuman', true);\n"
        "for (var i = 0; i < humanBarriers.length; i++) {\n"
        "    var barrier = humanBarriers[i];\n"
        "    var status = barrier.active ? 'ACTIVA' : 'INACTIVA';\n"
        "    console.log('- ' + barrier.code + ': ' + barrier.nombre + ' [' + status + ']');\n"
        "}",
        "Este ejemplo cambia todas las barreras humanas (dureza >= 3) a estado no aplicable y muestra estadísticas del cambio.");
    
    // Example 4: Set non-human barriers
    QTreeWidgetItem *example4Item = new QTreeWidgetItem(examplesItem);
    example4Item->setText(0, "4. Barreras No Humanas Aplicables");
    addExampleSection("Ejemplo 4: Poner Barreras No Humanas como Aplicables",
        "// Obtener todas las barreras\n"
        "var allBarriers = Defenses.getBarriers();\n"
        "console.log('Total de barreras en el modelo: ' + allBarriers.length);\n"
        "\n"
        "// Separar humanas de no humanas\n"
        "var humanBarriers = Charts.filterByProperty(allBarriers, 'isHuman', true);\n"
        "var nonHumanBarriers = Charts.filterByProperty(allBarriers, 'isHuman', false);\n"
        "\n"
        "console.log('- Barreras humanas: ' + humanBarriers.length);\n"
        "console.log('- Barreras no humanas: ' + nonHumanBarriers.length);\n"
        "\n"
        "// Realizar el cambio\n"
        "var changedCount = Defenses.setNonHumanBarriersApplicable();\n"
        "console.log('\\nSe activaron ' + changedCount + ' barreras no humanas.');\n"
        "\n"
        "// Mostrar detalles de barreras no humanas por dureza\n"
        "console.log('\\n=== BARRERAS NO HUMANAS POR DUREZA ===');\n"
        "var barriersByHardiness = Charts.groupByProperty(nonHumanBarriers, 'hardinessString');\n"
        "for (var hardiness in barriersByHardiness) {\n"
        "    var barriers = barriersByHardiness[hardiness];\n"
        "    console.log('\\n' + hardiness + ' (' + barriers.length + ' barreras):');\n"
        "    for (var i = 0; i < barriers.length; i++) {\n"
        "        var barrier = barriers[i];\n"
        "        var status = barrier.active ? 'ACTIVA' : 'INACTIVA';\n"
        "        console.log('  - ' + barrier.code + ': ' + barrier.nombre + ' [' + status + ']');\n"
        "    }\n"
        "}",
        "Este ejemplo activa todas las barreras no humanas (dureza < 3) y muestra un análisis por nivel de dureza.");
    
    // Example 5: List stages and substages
    QTreeWidgetItem *example5Item = new QTreeWidgetItem(examplesItem);
    example5Item->setText(0, "5. Listar Etapas y Subetapas");
    addExampleSection("Ejemplo 5: Explorar Estructura de Etapas y Subetapas",
        "// Obtener todas las etapas\n"
        "var stages = Stages.getAllStages();\n"
        "console.log('Total de etapas en el modelo: ' + stages.length);\n"
        "console.log('');\n"
        "\n"
        "// Mostrar etapas con sus subetapas\n"
        "var stagesWithSubstages = Stages.getStagesWithSubstages();\n"
        "for (var stageName in stagesWithSubstages) {\n"
        "    var stageData = stagesWithSubstages[stageName];\n"
        "    console.log('=== ' + stageData.codigo + ' - ' + stageName + ' ===');\n"
        "    console.log('Orden: ' + stageData.orden);\n"
        "    \n"
        "    var substages = stageData.substages;\n"
        "    console.log('Subetapas (' + substages.length + '):');\n"
        "    for (var i = 0; i < substages.length; i++) {\n"
        "        var substage = substages[i];\n"
        "        var status = substage.activo ? 'ACTIVA' : 'INACTIVA';\n"
        "        console.log('  ' + (i + 1) + '. ' + substage.nombre + ' [' + status + ']');\n"
        "    }\n"
        "    console.log('');\n"
        "}\n"
        "\n"
        "// Contar subetapas por etapa\n"
        "console.log('\\n=== RESUMEN DE SUBETAPAS ===');\n"
        "var counts = Stages.countSubstagesByStage();\n"
        "for (var stage in counts) {\n"
        "    console.log(stage + ': ' + counts[stage] + ' subetapas');\n"
        "}",
        "Este ejemplo explora la estructura completa de etapas y subetapas del modelo, mostrando su jerarquía y estado.");
    
    // Example 6: Separate process stages from instrumental stages
    QTreeWidgetItem *example6Item = new QTreeWidgetItem(examplesItem);
    example6Item->setText(0, "6. Separar Etapas del Proceso e Instrumentales");
    addExampleSection("Ejemplo 6: Distinguir Etapas del Proceso de Etapas Instrumentales",
        "// Obtener todas las etapas (incluyendo instrumentales)\n"
        "var allStages = Stages.getAllStages();\n"
        "\n"
        "// Separar por tipo de ID\n"
        "var processStages = [];\n"
        "var instrumentalStages = [];\n"
        "\n"
        "for (var i = 0; i < allStages.length; i++) {\n"
        "    var stage = allStages[i];\n"
        "    if (stage.id > 0) {\n"
        "        processStages.push(stage);\n"
        "    } else if (stage.id < 0) {\n"
        "        instrumentalStages.push(stage);\n"
        "    }\n"
        "}\n"
        "\n"
        "// Ordenar por campo 'orden'\n"
        "processStages = Charts.sortByProperty(processStages, 'orden', true);\n"
        "instrumentalStages = Charts.sortByProperty(instrumentalStages, 'orden', true);\n"
        "\n"
        "// Mostrar etapas del proceso\n"
        "console.log('=== ETAPAS DEL PROCESO (ID > 0) ===');\n"
        "for (var i = 0; i < processStages.length; i++) {\n"
        "    var stage = processStages[i];\n"
        "    console.log('[' + stage.orden + '] ' + stage.codigo + ': ' + stage.nombre);\n"
        "}\n"
        "\n"
        "// Mostrar etapas instrumentales\n"
        "console.log('\\n=== ETAPAS INSTRUMENTALES (ID < 0) ===');\n"
        "for (var i = 0; i < instrumentalStages.length; i++) {\n"
        "    var stage = instrumentalStages[i];\n"
        "    console.log('[' + stage.orden + '] ' + stage.codigo + ': ' + stage.nombre);\n"
        "}",
        "Las etapas con ID positivo son etapas del proceso donde ocurren eventos. Las etapas con ID negativo son etapas instrumentales usadas para organizar barreras.");
    
    // Example 7: Show event risk levels
    QTreeWidgetItem *example7Item = new QTreeWidgetItem(examplesItem);
    example7Item->setText(0, "7. Analizar Riesgo, Probabilidad y Consecuencia");
    addExampleSection("Ejemplo 7: Analizar Niveles de Riesgo con Defensas Aplicadas",
        "// Obtener todos los eventos\n"
        "var events = Events.getAllEvents();\n"
        "\n"
        "// Agrupar por nivel de riesgo\n"
        "var rma = [], ra = [], rm = [], rb = [];\n"
        "for (var i = 0; i < events.length; i++) {\n"
        "    var e = events[i];\n"
        "    if (e.riesgoTexto === 'RMA') rma.push(e);\n"
        "    else if (e.riesgoTexto === 'RA') ra.push(e);\n"
        "    else if (e.riesgoTexto === 'RM') rm.push(e);\n"
        "    else rb.push(e);\n"
        "}\n"
        "\n"
        "// Mostrar cada grupo\n"
        "var grupos = [{n:'RMA',e:rma},{n:'RA',e:ra},{n:'RM',e:rm},{n:'RB',e:rb}];\n"
        "for (var g = 0; g < grupos.length; g++) {\n"
        "    var grupo = grupos[g];\n"
        "    console.log('\\n=== RIESGO ' + grupo.n + ' (' + grupo.e.length + ' eventos) ===');\n"
        "    for (var i = 0; i < grupo.e.length; i++) {\n"
        "        var e = grupo.e[i];\n"
        "        console.log(e.codigo + ' - ' + e.nombre.substring(0,40) + ' [F:' + e.frecuenciaTexto + ' P:' + e.probabilidadTexto + ' C:' + e.consecuenciaTexto + ']');\n"
        "    }\n"
        "}",
        "Agrupa y muestra TODOS los eventos por nivel de riesgo (RMA/RA/RM/RB). Los valores F/P/C consideran defensas activas.");
    
    // Example 8: Mark all events as applicable
    QTreeWidgetItem *example8Item = new QTreeWidgetItem(examplesItem);
    example8Item->setText(0, "8. Marcar Todos los Eventos como Aplicables");
    addExampleSection("Ejemplo 8: Marcar Todos los Iniciadores como Aplicables",
        "// Contar eventos antes del cambio\n"
        "var totalEvents = Events.getAllEvents().length;\n"
        "console.log('Total de eventos en el modelo: ' + totalEvents);\n"
        "\n"
        "// Marcar todos como aplicables\n"
        "var count = Events.setAllEventsApplicable();\n"
        "\n"
        "console.log('\\n✓ Se marcaron ' + count + ' eventos como aplicables (completado=true)');\n"
        "console.log('Ahora todos los eventos se consideran en el análisis de riesgo.');",
        "Este ejemplo marca todos los eventos como aplicables/completados, lo que significa que se incluirán en el análisis de riesgo.");
    
    // Example 9: Mark all events as not applicable
    QTreeWidgetItem *example9Item = new QTreeWidgetItem(examplesItem);
    example9Item->setText(0, "9. Marcar Todos los Eventos como No Aplicables");
    addExampleSection("Ejemplo 9: Marcar Todos los Iniciadores como No Aplicables",
        "// Contar eventos antes del cambio\n"
        "var totalEvents = Events.getAllEvents().length;\n"
        "console.log('Total de eventos en el modelo: ' + totalEvents);\n"
        "\n"
        "// Marcar todos como no aplicables\n"
        "var count = Events.setAllEventsNotApplicable();\n"
        "\n"
        "console.log('\\n✓ Se marcaron ' + count + ' eventos como no aplicables (completado=false)');\n"
        "console.log('Ahora ningún evento se considerará en el análisis de riesgo.');",
        "Este ejemplo marca todos los eventos como no aplicables/no completados, excluyéndolos del análisis de riesgo.");
    
    // Example 10: Search and replace in descriptions
    QTreeWidgetItem *example10Item = new QTreeWidgetItem(examplesItem);
    example10Item->setText(0, "10. Buscar y Reemplazar en Descripciones");
    addExampleSection("Ejemplo 10: Buscar y Reemplazar Texto en Descripciones de Eventos",
        "// Pedir términos de búsqueda y reemplazo (simulado con variables)\n"
        "var searchTerm = 'paciente';  // En producción: usar input dialog\n"
        "var replaceTerm = 'usuario';\n"
        "\n"
        "console.log('Buscando \"' + searchTerm + '\" para reemplazar por \"' + replaceTerm + '\"...');\n"
        "\n"
        "// Realizar búsqueda y reemplazo (no sensible a mayúsculas/minúsculas)\n"
        "var count = Events.searchReplaceInDescriptions(searchTerm, replaceTerm, false);\n"
        "\n"
        "if (count > 0) {\n"
        "    console.log('\\n✓ Se actualizaron ' + count + ' descripciones de eventos.');\n"
        "    \n"
        "    // Mostrar algunos eventos modificados\n"
        "    console.log('\\nEjemplos de eventos modificados:');\n"
        "    var allEvents = Events.getAllEvents();\n"
        "    var shown = 0;\n"
        "    for (var i = 0; i < allEvents.length && shown < 3; i++) {\n"
        "        var e = allEvents[i];\n"
        "        if (e.descripcion.indexOf(replaceTerm) !== -1) {\n"
        "            console.log('  - ' + e.codigo + ': ' + e.descripcion.substring(0, 60) + '...');\n"
        "            shown++;\n"
        "        }\n"
        "    }\n"
        "} else {\n"
        "    console.log('\\n⚠ No se encontraron coincidencias.');\n"
        "}",
        "Este ejemplo busca un término en todas las descripciones de eventos y lo reemplaza por otro término, útil para actualización masiva de terminología.");
    
    // Example 11: Update specific event properties
    QTreeWidgetItem *example11Item = new QTreeWidgetItem(examplesItem);
    example11Item->setText(0, "11. Modificar Propiedades de un Evento");
    addExampleSection("Ejemplo 11: Actualizar Propiedades de un Evento Específico",
        "// Obtener un evento para modificar (por ejemplo, el primero)\n"
        "var events = Events.getAllEvents();\n"
        "if (events.length > 0) {\n"
        "    var event = events[0];\n"
        "    \n"
        "    console.log('Evento original:');\n"
        "    console.log('  ID: ' + event.id);\n"
        "    console.log('  Nombre: ' + event.nombre);\n"
        "    console.log('  Frecuencia: ' + event.frecuencia);\n"
        "    console.log('  Consecuencia: ' + event.consecuencia);\n"
        "    \n"
        "    // Modificar propiedades\n"
        "    var success = Events.updateEvent(event.id, {\n"
        "        frecuencia: 3,\n"
        "        consecuencia: 4,\n"
        "        descripcion: 'Descripción actualizada por script',\n"
        "        completado: true\n"
        "    });\n"
        "    \n"
        "    if (success) {\n"
        "        console.log('\\n✓ Evento actualizado correctamente');\n"
        "        \n"
        "        // Verificar cambios\n"
        "        var updatedEvents = Events.getAllEvents();\n"
        "        var updatedEvent = updatedEvents[0];\n"
        "        console.log('\\nEvento modificado:');\n"
        "        console.log('  Frecuencia: ' + updatedEvent.frecuencia);\n"
        "        console.log('  Consecuencia: ' + updatedEvent.consecuencia);\n"
        "    } else {\n"
        "        console.log('\\n✗ Error al actualizar el evento');\n"
        "    }\n"
        "} else {\n"
        "    console.log('No hay eventos en el modelo.');\n"
        "}",
        "Este ejemplo muestra cómo modificar las propiedades de un evento específico, incluyendo nombre, descripción, frecuencia, consecuencia, etapa, y estado de aplicabilidad.");
    
    // Example 12: Update stage properties
    QTreeWidgetItem *example12Item = new QTreeWidgetItem(examplesItem);
    example12Item->setText(0, "12. Modificar Propiedades de una Etapa");
    addExampleSection("Ejemplo 12: Actualizar Propiedades de una Etapa",
        "// Obtener todas las etapas del proceso (ID > 0)\n"
        "var allStages = Stages.getAllStages();\n"
        "var processStages = Charts.filterByProperty(allStages, 'id', function(id) { return id > 0; });\n"
        "\n"
        "// Mostrar etapas disponibles\n"
        "console.log('=== ETAPAS DEL PROCESO ===');\n"
        "for (var i = 0; i < allStages.length && i < 5; i++) {\n"
        "    var stage = allStages[i];\n"
        "    if (stage.id > 0) {\n"
        "        console.log('ID: ' + stage.id + ' - ' + stage.nombre + ' (orden: ' + stage.orden + ')');\n"
        "    }\n"
        "}\n"
        "\n"
        "// Modificar una etapa (ejemplo con ID 1)\n"
        "if (allStages.length > 0 && allStages[0].id > 0) {\n"
        "    var stageId = allStages[0].id;\n"
        "    \n"
        "    console.log('\\nModificando etapa con ID: ' + stageId);\n"
        "    \n"
        "    var success = Stages.updateStage(stageId, {\n"
        "        descripcion: 'Descripción actualizada desde script',\n"
        "        orden: 10\n"
        "    });\n"
        "    \n"
        "    if (success) {\n"
        "        console.log('✓ Etapa actualizada correctamente');\n"
        "    } else {\n"
        "        console.log('✗ Error al actualizar la etapa');\n"
        "    }\n"
        "}",
        "Este ejemplo muestra cómo modificar las propiedades de una etapa, incluyendo nombre, descripción, código y orden de visualización (pero no el ID).");
    
    // Example 13: Refresh screen and save changes
    QTreeWidgetItem *example13Item = new QTreeWidgetItem(examplesItem);
    example13Item->setText(0, "13. Refrescar Pantalla y Guardar Cambios");
    addExampleSection("Ejemplo 13: Usar Refrescar Pantalla y Guardar Cambios",
        "// Script que modifica defensas y guarda los cambios\n"
        "console.log('=== MODIFICACIÓN Y GUARDADO AUTOMATIZADO ===');\n"
        "\n"
        "// Paso 1: Ver estado inicial\n"
        "var stats = Defenses.getDefenseStatistics();\n"
        "console.log('Estado inicial:');\n"
        "console.log('  - Defensas activas: ' + stats.activeDefenses);\n"
        "console.log('  - Defensas inactivas: ' + stats.inactiveDefenses);\n"
        "\n"
        "// Paso 2: Hacer cambios en el modelo\n"
        "console.log('\\nActivando todas las barreras no humanas...');\n"
        "var changed = Defenses.setNonHumanBarriersApplicable();\n"
        "console.log('Se modificaron ' + changed + ' defensas.');\n"
        "\n"
        "// Paso 3: Refrescar la pantalla para ver los cambios\n"
        "console.log('\\nRefrescando la pantalla principal...');\n"
        "Marrta.refreshMainScreen();\n"
        "console.log('✓ Pantalla actualizada. Los cambios ahora son visibles en la interfaz.');\n"
        "\n"
        "// Paso 4: Verificar los cambios\n"
        "stats = Defenses.getDefenseStatistics();\n"
        "console.log('\\nEstado después del cambio:');\n"
        "console.log('  - Defensas activas: ' + stats.activeDefenses);\n"
        "console.log('  - Defensas inactivas: ' + stats.inactiveDefenses);\n"
        "\n"
        "// Paso 5: Guardar los cambios en disco\n"
        "console.log('\\nGuardando cambios en la base de datos...');\n"
        "Marrta.saveModelChanges();\n"
        "console.log('✓ Cambios guardados en disco correctamente.');\n"
        "\n"
        "console.log('\\n=== PROCESO COMPLETADO ===');\n"
        "console.log('Los cambios están ahora persistidos en el archivo .pdb');",
        "Este ejemplo demuestra el flujo completo: modificar datos, refrescar la pantalla para ver los cambios en la interfaz, y guardar todo en disco sin diálogos de confirmación.");
    
    // Expand the examples section
    examplesItem->setExpanded(true);
    
    // Select first item
    if (sectionTree->topLevelItemCount() > 0) {
        sectionTree->setCurrentItem(sectionTree->topLevelItem(0));
    }
}

void ScriptingDocumentation::addDocumentationSection(const QString &title, const QString &content)
{
    DocumentationItem item;
    item.title = title;
    item.content = content;
    item.isExample = false;
    documentationItems.append(item);
}

void ScriptingDocumentation::addExampleSection(const QString &title, const QString &code, const QString &description)
{
    DocumentationItem item;
    item.title = title;
    item.code = code;
    item.content = QString("<h2>%1</h2><p>%2</p><h3>Código:</h3><pre style='background-color: #f5f5f5; padding: 10px; border-radius: 5px;'>%3</pre>")
                          .arg(title.toHtmlEscaped())
                          .arg(description.toHtmlEscaped())
                          .arg(code.toHtmlEscaped());
    item.isExample = true;
    documentationItems.append(item);
}

void ScriptingDocumentation::onSectionChanged()
{
    QTreeWidgetItem *current = sectionTree->currentItem();
    if (!current) return;
    
    // Find the corresponding documentation item
    int index = -1;
    QTreeWidgetItem *parent = current->parent();
    
    if (!parent) {
        // Top level item
        for (int i = 0; i < sectionTree->topLevelItemCount(); i++) {
            if (sectionTree->topLevelItem(i) == current) {
                index = i;
                break;
            }
        }
    } else {
        // Child item (example)
        int parentIndex = sectionTree->indexOfTopLevelItem(parent);
        int childIndex = parent->indexOfChild(current);
        
        // Calculate the absolute index in documentationItems
        if (parentIndex == 7) { // Examples section (7th top-level item: 0-6 are other sections)
            index = 7 + childIndex; // Skip the intro + API sections
        }
    }
    
    if (index >= 0 && index < documentationItems.size()) {
        const DocumentationItem &item = documentationItems[index];
        contentBrowser->setHtml(item.content);
        insertButton->setEnabled(item.isExample);
    }
}

void ScriptingDocumentation::onExampleInserted()
{
    QTreeWidgetItem *current = sectionTree->currentItem();
    if (!current) return;
    
    // Find the corresponding example
    QTreeWidgetItem *parent = current->parent();
    if (!parent) return;
    
    int parentIndex = sectionTree->indexOfTopLevelItem(parent);
    int childIndex = parent->indexOfChild(current);
    
    if (parentIndex == 7) { // Examples section (7th top-level item: 0-6 are other sections)
        int exampleIndex = 7 + childIndex;
        if (exampleIndex < documentationItems.size()) {
            const DocumentationItem &item = documentationItems[exampleIndex];
            emit insertExample(item.code);
            close();
        }
    }
}