#include "pythonscriptingdocumentation.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QTreeWidgetItem>

PythonScriptingDocumentation::PythonScriptingDocumentation(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    populateDocumentation();
    
    setWindowTitle(tr("Documentación API Python"));
    resize(900, 700);
}

PythonScriptingDocumentation::~PythonScriptingDocumentation()
{
}

void PythonScriptingDocumentation::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create splitter for tree and documentation
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Tree widget for navigation
    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabel(tr("API"));
    treeWidget->setMinimumWidth(200);
    connect(treeWidget, &QTreeWidget::itemClicked, 
            this, &PythonScriptingDocumentation::onTreeItemClicked);
    
    // Documentation browser
    docBrowser = new QTextBrowser(this);
    docBrowser->setOpenExternalLinks(false);
    
    splitter->addWidget(treeWidget);
    splitter->addWidget(docBrowser);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    
    mainLayout->addWidget(splitter);
    
    // Insert example button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    insertButton = new QPushButton(tr("Insertar ejemplo en editor"), this);
    insertButton->setEnabled(false);
    connect(insertButton, &QPushButton::clicked, 
            this, &PythonScriptingDocumentation::onInsertExampleClicked);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(insertButton);
    mainLayout->addLayout(buttonLayout);
}

void PythonScriptingDocumentation::populateDocumentation()
{
    // Marrta base object
    QTreeWidgetItem *marrtaItem = new QTreeWidgetItem(treeWidget);
    marrtaItem->setText(0, tr("Marrta"));
    marrtaItem->setData(0, Qt::UserRole, "marrta");
    
    QTreeWidgetItem *marrtaVersionItem = new QTreeWidgetItem(marrtaItem);
    marrtaVersionItem->setText(0, tr("get_version()"));
    marrtaVersionItem->setData(0, Qt::UserRole, "marrta_version");
    
    // Database object
    QTreeWidgetItem *databaseItem = new QTreeWidgetItem(treeWidget);
    databaseItem->setText(0, tr("Database"));
    databaseItem->setData(0, Qt::UserRole, "database");
    
    QTreeWidgetItem *queryItem = new QTreeWidgetItem(databaseItem);
    queryItem->setText(0, tr("query(sql)"));
    queryItem->setData(0, Qt::UserRole, "database_query");
    
    // Etapas object
    QTreeWidgetItem *etapasItem = new QTreeWidgetItem(treeWidget);
    etapasItem->setText(0, tr("Etapas"));
    etapasItem->setData(0, Qt::UserRole, "etapas");
    
    QTreeWidgetItem *getAllItem = new QTreeWidgetItem(etapasItem);
    getAllItem->setText(0, tr("get_all()"));
    getAllItem->setData(0, Qt::UserRole, "etapas_getall");
    
    QTreeWidgetItem *etapaItem = new QTreeWidgetItem(etapasItem);
    etapaItem->setText(0, tr("Etapa"));
    etapaItem->setData(0, Qt::UserRole, "etapa");
    
    QTreeWidgetItem *imprimirItem = new QTreeWidgetItem(etapaItem);
    imprimirItem->setText(0, tr("imprimir()"));
    imprimirItem->setData(0, Qt::UserRole, "etapa_imprimir");
    
    // Examples
    QTreeWidgetItem *examplesItem = new QTreeWidgetItem(treeWidget);
    examplesItem->setText(0, tr("Ejemplos"));
    examplesItem->setData(0, Qt::UserRole, "examples");
    
    QTreeWidgetItem *example1Item = new QTreeWidgetItem(examplesItem);
    example1Item->setText(0, tr("Listar todas las etapas"));
    example1Item->setData(0, Qt::UserRole, "example1");
    
    QTreeWidgetItem *example2Item = new QTreeWidgetItem(examplesItem);
    example2Item->setText(0, tr("Usar Etapa.imprimir()"));
    example2Item->setData(0, Qt::UserRole, "example2");
    
    QTreeWidgetItem *example3Item = new QTreeWidgetItem(examplesItem);
    example3Item->setText(0, tr("Consulta SQL a la base de datos"));
    example3Item->setData(0, Qt::UserRole, "example3");
    
    treeWidget->expandAll();
    
    // Show initial documentation
    docBrowser->setHtml(getDocumentationHTML());
}

void PythonScriptingDocumentation::onTreeItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    
    QString section = item->data(0, Qt::UserRole).toString();
    QString html;
    
    if (section == "marrta") {
        html = R"HTML(
<h2>Objeto Marrta</h2>
<p>El objeto <b>Marrta</b> proporciona acceso a las funcionalidades principales de la aplicación.</p>
<h3>Métodos disponibles:</h3>
<ul>
<li><b>get_version()</b> - Obtiene la versión de MARRTA</li>
</ul>
)HTML";
    } else if (section == "marrta_version") {
        html = R"HTML(
<h2>Marrta.get_version()</h2>
<p>Obtiene la versión actual de MARRTA.</p>
<h3>Retorna:</h3>
<p>String con la versión de MARRTA</p>
<h3>Ejemplo:</h3>
<pre>version = Marrta.get_version()
print(f"Versión de MARRTA: {version}")</pre>
)HTML";
        insertButton->setEnabled(true);
        insertButton->setProperty("example", "version = Marrta.get_version()\nprint(f\"Versión de MARRTA: {version}\")\n");
    } else if (section == "database") {
        html = R"HTML(
<h2>Objeto Database</h2>
<p>El objeto <b>Database</b> proporciona acceso directo a la base de datos SQLite mediante consultas SQL.</p>
<h3>Métodos disponibles:</h3>
<ul>
<li><b>query(sql)</b> - Ejecuta una consulta SQL y devuelve los resultados</li>
</ul>
<p><b>Nota:</b> Tenga cuidado al ejecutar consultas que modifiquen la base de datos.</p>
)HTML";
    } else if (section == "database_query") {
        html = R"HTML(
<h2>Database.query(sql)</h2>
<p>Ejecuta una consulta SQL en la base de datos y devuelve los resultados como una lista de diccionarios.</p>
<h3>Parámetros:</h3>
<ul>
<li><b>sql</b> (string) - Consulta SQL a ejecutar</li>
</ul>
<h3>Retorna:</h3>
<p>Lista de diccionarios, donde cada diccionario representa una fila con los nombres de las columnas como claves.</p>
<h3>Ejemplo:</h3>
<pre>results = Database.query("SELECT * FROM Etapa ORDER BY orden")
for row in results:
    print(f"Etapa: {row['nombre']}")</pre>
)HTML";
        insertButton->setEnabled(true);
        insertButton->setProperty("example", "results = Database.query(\"SELECT * FROM Etapa ORDER BY orden\")\nfor row in results:\n    print(f\"Etapa: {row['nombre']}\")\n");
    } else if (section == "etapas") {
        html = R"HTML(
<h2>Objeto Etapas</h2>
<p>El objeto <b>Etapas</b> proporciona acceso a las etapas del modelo de análisis de riesgos.</p>
<h3>Métodos disponibles:</h3>
<ul>
<li><b>get_all()</b> - Obtiene todas las etapas como objetos Etapa</li>
</ul>
)HTML";
    } else if (section == "etapas_getall") {
        html = R"HTML(
<h2>Etapas.get_all()</h2>
<p>Obtiene todas las etapas del modelo como una lista de objetos Etapa.</p>
<h3>Retorna:</h3>
<p>Lista de objetos Etapa</p>
<h3>Ejemplo:</h3>
<pre>etapas = Etapas.get_all()
print(f"Total de etapas: {len(etapas)}")
for etapa in etapas:
    print(f"{etapa.codigo}: {etapa.nombre}")</pre>
)HTML";
        insertButton->setEnabled(true);
        insertButton->setProperty("example", "etapas = Etapas.get_all()\nprint(f\"Total de etapas: {len(etapas)}\")\nfor etapa in etapas:\n    print(f\"{etapa.codigo}: {etapa.nombre}\")\n");
    } else if (section == "etapa") {
        html = R"HTML(
<h2>Clase Etapa</h2>
<p>Representa una etapa del modelo de análisis de riesgos.</p>
<h3>Propiedades:</h3>
<ul>
<li><b>id</b> - ID único de la etapa</li>
<li><b>codigo</b> - Código de la etapa</li>
<li><b>nombre</b> - Nombre de la etapa</li>
<li><b>descripcion</b> - Descripción de la etapa</li>
<li><b>orden</b> - Orden de visualización</li>
</ul>
<h3>Métodos:</h3>
<ul>
<li><b>imprimir()</b> - Devuelve una tupla con información completa de la etapa</li>
</ul>
)HTML";
    } else if (section == "etapa_imprimir") {
        html = R"HTML(
<h2>Etapa.imprimir()</h2>
<p>Devuelve una tupla con toda la información de la etapa, incluyendo los IDs de las subetapas.</p>
<h3>Retorna:</h3>
<p>Tupla con 5 elementos:</p>
<ol>
<li><b>ID</b> (int) - ID de la etapa</li>
<li><b>código</b> (string) - Código de la etapa</li>
<li><b>nombre</b> (string) - Nombre de la etapa</li>
<li><b>descripción</b> (string) - Descripción de la etapa</li>
<li><b>(IDs subetapas)</b> (tupla) - Tupla con los IDs de las subetapas (vacía si no hay subetapas)</li>
</ol>
<h3>Ejemplo:</h3>
<pre>etapas = Etapas.get_all()
for etapa in etapas:
    datos = etapa.imprimir()
    id_etapa, codigo, nombre, desc, subetapas = datos
    print(f"Etapa {id_etapa}: {codigo} - {nombre}")
    print(f"  Descripción: {desc}")
    print(f"  Subetapas: {len(subetapas)}")</pre>
)HTML";
        insertButton->setEnabled(true);
        insertButton->setProperty("example", "etapas = Etapas.get_all()\nfor etapa in etapas:\n    datos = etapa.imprimir()\n    id_etapa, codigo, nombre, desc, subetapas = datos\n    print(f\"Etapa {id_etapa}: {codigo} - {nombre}\")\n    print(f\"  Descripción: {desc}\")\n    print(f\"  Subetapas: {len(subetapas)}\")\n");
    } else if (section == "example1") {
        html = R"HTML(
<h2>Ejemplo: Listar todas las etapas</h2>
<pre>etapas = Etapas.get_all()
print(f"=== Total: {len(etapas)} etapas ===\n")

for etapa in etapas:
    print(f"[{etapa.codigo}] {etapa.nombre}")
    print(f"    ID: {etapa.id}")
    print(f"    Descripción: {etapa.descripcion}")
    print()</pre>
)HTML";
        insertButton->setEnabled(true);
        insertButton->setProperty("example", "etapas = Etapas.get_all()\nprint(f\"=== Total: {len(etapas)} etapas ===\\n\")\n\nfor etapa in etapas:\n    print(f\"[{etapa.codigo}] {etapa.nombre}\")\n    print(f\"    ID: {etapa.id}\")\n    print(f\"    Descripción: {etapa.descripcion}\")\n    print()\n");
    } else if (section == "example2") {
        html = R"HTML(
<h2>Ejemplo: Usar Etapa.imprimir()</h2>
<pre>etapas = Etapas.get_all()
print("=== Información detallada de etapas ===\n")

for etapa in etapas:
    # Usar el método imprimir()
    datos = etapa.imprimir()
    id_etapa, codigo, nombre, desc, subetapas_ids = datos
    
    print(f"Etapa {id_etapa}: {codigo} - {nombre}")
    print(f"  Descripción: {desc}")
    print(f"  Número de subetapas: {len(subetapas_ids)}")
    
    if len(subetapas_ids) > 0:
        print(f"  IDs de subetapas: {subetapas_ids}")
    print()</pre>
)HTML";
        insertButton->setEnabled(true);
        insertButton->setProperty("example", "etapas = Etapas.get_all()\nprint(\"=== Información detallada de etapas ===\\n\")\n\nfor etapa in etapas:\n    # Usar el método imprimir()\n    datos = etapa.imprimir()\n    id_etapa, codigo, nombre, desc, subetapas_ids = datos\n    \n    print(f\"Etapa {id_etapa}: {codigo} - {nombre}\")\n    print(f\"  Descripción: {desc}\")\n    print(f\"  Número de subetapas: {len(subetapas_ids)}\")\n    \n    if len(subetapas_ids) > 0:\n        print(f\"  IDs de subetapas: {subetapas_ids}\")\n    print()\n");
    } else if (section == "example3") {
        html = R"HTML(
<h2>Ejemplo: Consulta SQL a la base de datos</h2>
<pre># Consultar eventos de alta frecuencia
query_sql = """
SELECT nombre, frecuencia, consecuencia 
FROM Suceso 
WHERE frecuencia >= 4 
ORDER BY frecuencia DESC, consecuencia DESC
"""

resultados = Database.query(query_sql)
print(f"=== Eventos de alta frecuencia: {len(resultados)} ===\n")

for evento in resultados:
    print(f"Evento: {evento['nombre']}")
    print(f"  Frecuencia: {evento['frecuencia']}")
    print(f"  Consecuencia: {evento['consecuencia']}")
    print()</pre>
)HTML";
        insertButton->setEnabled(true);
        insertButton->setProperty("example", "# Consultar eventos de alta frecuencia\nquery_sql = \"\"\"\nSELECT nombre, frecuencia, consecuencia \nFROM Suceso \nWHERE frecuencia >= 4 \nORDER BY frecuencia DESC, consecuencia DESC\n\"\"\"\n\nresultados = Database.query(query_sql)\nprint(f\"=== Eventos de alta frecuencia: {len(resultados)} ===\")\n\nfor evento in resultados:\n    print(f\"Evento: {evento['nombre']}\")\n    print(f\"  Frecuencia: {evento['frecuencia']}\")\n    print(f\"  Consecuencia: {evento['consecuencia']}\")\n    print()\n");
    } else {
        html = getDocumentationHTML();
        insertButton->setEnabled(false);
    }
    
    docBrowser->setHtml(html);
}

void PythonScriptingDocumentation::onInsertExampleClicked()
{
    QString example = insertButton->property("example").toString();
    if (!example.isEmpty()) {
        emit insertExample(example);
    }
}

QString PythonScriptingDocumentation::getDocumentationHTML()
{
    return R"HTML(
<h1>API de Scripting Python de MARRTA</h1>
<p>Bienvenido a la documentación de la API de scripting Python de MARRTA.</p>

<h2>Introducción</h2>
<p>El motor de scripting Python permite automatizar tareas y realizar análisis personalizados en MARRTA. 
La API proporciona acceso a:</p>

<ul>
<li><b>Marrta</b>: Objeto base para acceder a funcionalidades de la aplicación</li>
<li><b>Database</b>: Acceso directo a la base de datos mediante consultas SQL</li>
<li><b>Etapas</b>: Acceso a las etapas del modelo con el método Etapa.imprimir()</li>
</ul>

<h2>Comenzando</h2>
<p>Seleccione un elemento del árbol de navegación para ver su documentación detallada y ejemplos de uso.</p>

<h2>Ejemplo básico</h2>
<pre># Obtener todas las etapas
etapas = Etapas.get_all()
print(f"Total de etapas: {len(etapas)}")

# Usar el método imprimir() de cada etapa
for etapa in etapas:
    datos = etapa.imprimir()
    id_etapa, codigo, nombre, desc, subetapas = datos
    print(f"{codigo}: {nombre} ({len(subetapas)} subetapas)")</pre>

<h2>Notas importantes</h2>
<ul>
<li>El método <b>Etapa.imprimir()</b> devuelve una tupla: (ID, código, nombre, descripción, (IDs_subetapas))</li>
<li>Use <b>Database.query()</b> para ejecutar consultas SQL personalizadas</li>
<li>La tupla de IDs de subetapas está vacía si la etapa no tiene subetapas</li>
</ul>
)HTML";
}
