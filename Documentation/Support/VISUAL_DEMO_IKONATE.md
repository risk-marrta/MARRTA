# Demostración Visual de Iconos Ikonate
# Visual Demonstration of Ikonate Icons

## Ejemplo de Icono SVG / SVG Icon Example

### Estructura de un Icono Ikonate

```xml
<svg role="img" 
     xmlns="http://www.w3.org/2000/svg" 
     width="48px" 
     height="48px" 
     viewBox="0 0 24 24" 
     aria-labelledby="folderIconTitle" 
     stroke="#2329D6"        <!-- Color del trazo / Stroke color -->
     stroke-width="1"        <!-- Grosor del trazo / Stroke width -->
     stroke-linecap="square" 
     stroke-linejoin="miter" 
     fill="none"             <!-- Sin relleno / No fill -->
     color="#2329D6">
  <title id="folderIconTitle">Folder</title>
  <path d="M3 6v13h18V9h-8.4L10 6z"/>
</svg>
```

### Características Clave / Key Characteristics

1. **Basado en Trazos** (Stroke-based)
   - `stroke="#2329D6"` - Define el color
   - `fill="none"` - No tiene relleno
   - Fácil de cambiar color editando el atributo stroke

2. **Escalable** (Scalable)
   - `viewBox="0 0 24 24"` - Tamaño base 24x24
   - Se escala perfectamente a cualquier tamaño
   - No pierde calidad

3. **Accesible** (Accessible)
   - Incluye `role="img"` y `<title>`
   - Descripción para lectores de pantalla

## Integración en Qt / Qt Integration

### 1. Archivo en el Sistema / File in System

```
resources/icons/folder.svg
```

### 2. Registro en QRC / Registration in QRC

```xml
<!-- icons.qrc -->
<RCC>
    <qresource prefix="/icons">
        <file>resources/icons/folder.svg</file>
    </qresource>
</RCC>
```

### 3. Compilación / Compilation

```bash
qmake marrta.pro
# Genera / Generates: qrc_icons.cpp
# Contiene / Contains: Icono empaquetado en el binario
#                      Icon packaged in binary

make
# Compila / Compiles: Icono integrado en el ejecutable
#                     Icon integrated in executable
```

### 4. Uso en Código C++ / Usage in C++ Code

```cpp
// Crear QIcon desde recurso / Create QIcon from resource
QIcon folderIcon(":/icons/resources/icons/folder.svg");

// En un botón / In a button
QPushButton *openBtn = new QPushButton("Abrir", this);
openBtn->setIcon(folderIcon);
openBtn->setIconSize(QSize(24, 24));

// En una acción de menú / In a menu action
QAction *openAction = new QAction(folderIcon, "Abrir Archivo", this);
fileMenu->addAction(openAction);

// En un tool button / In a tool button
QToolButton *toolBtn = new QToolButton(this);
toolBtn->setIcon(folderIcon);
toolBtn->setToolTip("Abrir carpeta");
```

## Personalización de Colores / Color Customization

### Color Original / Original Color
```xml
<svg stroke="#2329D6" ...>  <!-- Azul / Blue -->
```

### Variaciones / Variations

#### Negro / Black
```xml
<svg stroke="#000000" ...>
```

#### Blanco / White
```xml
<svg stroke="#FFFFFF" ...>
```

#### Azul Oscuro / Dark Blue
```xml
<svg stroke="#0066CC" ...>
```

#### Verde / Green
```xml
<svg stroke="#00AA00" ...>
```

#### Rojo / Red
```xml
<svg stroke="#CC0000" ...>
```

### Cambiar Color con Comando / Change Color with Command

```bash
# Cambiar a negro / Change to black
sed -i 's/stroke="#2329D6"/stroke="#000000"/g' resources/icons/folder.svg

# Cambiar grosor de trazo / Change stroke width
sed -i 's/stroke-width="1"/stroke-width="1.5"/g' resources/icons/folder.svg
```

## Comparación: Theme vs Ikonate / Comparison: Theme vs Ikonate

### Método Anterior (Theme Icons)

```cpp
// Depende del tema del sistema / Depends on system theme
QIcon icon = QIcon::fromTheme("document-open");

// Problemas:
// - Puede verse diferente en cada OS
// - Requiere tema instalado
// - No garantiza que el icono exista
// - Difícil de personalizar
```

### Método Actual (Ikonate Icons)

```cpp
// Icono empaquetado / Packaged icon
QIcon icon(":/icons/resources/icons/folder.svg");

// Ventajas:
// - Se ve igual en todos los OS
// - No requiere dependencias externas
// - Garantizado que existe
// - Fácil de personalizar (editar SVG)
// - Empaquetado en el ejecutable
```

## Flujo Completo / Complete Flow

```
┌─────────────────────────────────────────────┐
│  1. DESCARGAR IKONATE.ZIP                   │
│     Download ikonate.zip                    │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────┐
│  2. EXTRAER SVG                             │
│     Extract SVG                             │
│                                             │
│  unzip ikonate.zip -d /tmp/ikonate         │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────┐
│  3. COPIAR A PROYECTO                       │
│     Copy to project                         │
│                                             │
│  cp /tmp/ikonate/icons/folder.svg \        │
│     resources/icons/                        │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────┐
│  4. REGISTRAR EN QRC                        │
│     Register in QRC                         │
│                                             │
│  <file>resources/icons/folder.svg</file>   │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────┐
│  5. COMPILAR                                │
│     Compile                                 │
│                                             │
│  qmake && make                             │
└──────────────────┬──────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────┐
│  6. USAR EN CÓDIGO                          │
│     Use in code                             │
│                                             │
│  QIcon(":/icons/resources/icons/folder.svg")│
└─────────────────────────────────────────────┘
```

## Ejemplos de Uso Real / Real Usage Examples

### En mainwindow.cpp (Actual)

```cpp
// Ejemplo 1: Menú contextual / Context menu
QMenu menu(this);
menu.addAction(
    QIcon(":/icons/resources/icons/checked.svg"),
    "Marcar todos",
    [=]{ /* código */ }
);

// Ejemplo 2: Acción de guardar / Save action
menu.addAction(
    QIcon(":/icons/resources/icons/save.svg"),
    "Guardar",
    [=]{ /* código */ }
);

// Ejemplo 3: Acción de imprimir / Print action
menu.addAction(
    QIcon(":/icons/resources/icons/print.svg"),
    "Imprimir",
    [=]{ /* código */ }
);
```

### Casos de Uso Comunes / Common Use Cases

#### 1. Toolbar / Barra de Herramientas
```cpp
QToolBar *toolbar = new QToolBar(this);

QAction *newAction = toolbar->addAction(
    QIcon(":/icons/resources/icons/new.svg"),
    "Nuevo"
);

QAction *saveAction = toolbar->addAction(
    QIcon(":/icons/resources/icons/save.svg"),
    "Guardar"
);

QAction *printAction = toolbar->addAction(
    QIcon(":/icons/resources/icons/print.svg"),
    "Imprimir"
);
```

#### 2. Tree Widget Items / Items de Árbol
```cpp
QTreeWidgetItem *item = new QTreeWidgetItem();
item->setText(0, "Carpeta");
item->setIcon(0, QIcon(":/icons/resources/icons/folder.svg"));
```

#### 3. List Widget Items / Items de Lista
```cpp
QListWidgetItem *item = new QListWidgetItem(
    QIcon(":/icons/resources/icons/document.svg"),
    "Documento.pdf"
);
listWidget->addItem(item);
```

#### 4. Tab Widget / Widget de Pestañas
```cpp
tabWidget->addTab(
    widget,
    QIcon(":/icons/resources/icons/dashboard.svg"),
    "Panel"
);
```

## Iconos Disponibles Actualmente / Currently Available Icons

```
✓ activity.svg       - Actividad / Activity
✓ book-opened.svg    - Libro abierto / Open book
✓ box.svg            - Caja / Box
✓ celluar.svg        - Red celular / Cellular
✓ checked.svg        - Verificado / Checked
✓ chevron-down.svg   - Flecha abajo / Down arrow
✓ chevron-up.svg     - Flecha arriba / Up arrow
✓ circle-ok.svg      - OK circular / Circle OK
✓ close.svg          - Cerrar / Close
✓ dashboard.svg      - Panel / Dashboard
✓ excel.svg          - Excel
✓ info.svg           - Información / Information
✓ line-chart.svg     - Gráfico / Chart
✓ new.svg            - Nuevo / New
✓ news.svg           - Noticias / News
✓ next.svg           - Siguiente / Next
✓ no-entry.svg       - Prohibido / No entry
✓ print.svg          - Imprimir / Print
✓ remove.svg         - Eliminar / Remove
✓ save.svg           - Guardar / Save
✓ stop.svg           - Parar / Stop
✓ trending-down.svg  - Tendencia baja / Trending down
✓ upload.svg         - Subir / Upload
```

## Iconos Sugeridos para Añadir / Suggested Icons to Add

```
→ folder.svg         - Carpeta / Folder
→ folder-open.svg    - Carpeta abierta / Open folder
→ edit.svg           - Editar / Edit
→ settings.svg       - Configuración / Settings
→ search.svg         - Buscar / Search
→ filter.svg         - Filtrar / Filter
→ download.svg       - Descargar / Download
→ trash.svg          - Papelera / Trash
→ copy.svg           - Copiar / Copy
→ refresh.svg        - Actualizar / Refresh
→ menu.svg           - Menú / Menu
→ grid.svg           - Cuadrícula / Grid
→ list.svg           - Lista / List
→ warning.svg        - Advertencia / Warning
→ alert.svg          - Alerta / Alert
```

## Verificación / Verification

### 1. Verificar Archivo SVG / Check SVG File
```bash
file resources/icons/folder.svg
# Output: resources/icons/folder.svg: SVG Scalable Vector Graphics image
```

### 2. Verificar en QRC / Check in QRC
```bash
grep 'folder.svg' icons.qrc
# Output: <file>resources/icons/folder.svg</file>
```

### 3. Verificar en Ejecutable / Check in Executable
```bash
strings marrta | grep folder.svg
# Output: resources/icons/folder.svg
```

### 4. Verificar Visualmente / Visual Check
```bash
# Abrir aplicación / Open application
./marrta
# Los iconos deben aparecer / Icons should appear
```

## Recursos / Resources

- **Ikonate**: https://ikonate.com/
- **Qt Resources**: https://doc.qt.io/qt-5/resources.html
- **QIcon**: https://doc.qt.io/qt-5/qicon.html
- **SVG**: https://developer.mozilla.org/en-US/docs/Web/SVG

## Scripts de Ayuda / Helper Scripts

```bash
# Listar iconos / List icons
./manage_ikonate_icons.py list

# Verificar QRC / Check QRC
./manage_ikonate_icons.py check

# Añadir iconos / Add icons
./manage_ikonate_icons.py add /path/to/icons/

# Generar ejemplos / Generate examples
./manage_ikonate_icons.py examples
```
