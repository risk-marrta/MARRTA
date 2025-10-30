# Ejemplo Práctico: Añadir Iconos desde ikonate.zip
# Practical Example: Adding Icons from ikonate.zip

## Escenario / Scenario

Has descargado `ikonate.zip` del issue y quieres añadir los iconos al proyecto MARRTA.
You've downloaded `ikonate.zip` from the issue and want to add the icons to the MARRTA project.

## Paso a Paso / Step by Step

### 1. Descomprimir el archivo ZIP / Extract the ZIP file

```bash
# Crear directorio temporal
mkdir -p /tmp/ikonate

# Descomprimir
unzip ikonate.zip -d /tmp/ikonate

# Ver contenido
ls -la /tmp/ikonate/
```

### 2. Identificar los iconos que necesitas / Identify needed icons

```bash
# Listar todos los SVG en el ZIP
find /tmp/ikonate -name "*.svg" -type f

# O si están en un subdirectorio específico
ls /tmp/ikonate/icons/*.svg
```

**Iconos comunes que podrías querer añadir:**
- `folder.svg` - Para gestión de archivos
- `folder-open.svg` - Carpeta abierta
- `edit.svg` - Para editar
- `settings.svg` - Configuración
- `search.svg` - Búsqueda
- `filter.svg` - Filtros
- `download.svg` - Descargas
- `trash.svg` - Eliminar
- `copy.svg` - Copiar
- `refresh.svg` - Actualizar
- `menu.svg` - Menú
- `grid.svg` - Vista de cuadrícula
- `list.svg` - Vista de lista

### 3. Opción A: Usar el Script Python (Recomendado) / Option A: Use Python Script (Recommended)

```bash
cd /home/runner/work/MARRTA/MARRTA

# Añadir todos los iconos del directorio
./manage_ikonate_icons.py add /tmp/ikonate/icons/

# O añadir iconos específicos
./manage_ikonate_icons.py add /tmp/ikonate/icons/folder.svg
./manage_ikonate_icons.py add /tmp/ikonate/icons/edit.svg
./manage_ikonate_icons.py add /tmp/ikonate/icons/settings.svg
```

### 4. Opción B: Usar el Script Bash / Option B: Use Bash Script

```bash
cd /home/runner/work/MARRTA/MARRTA

# Ejecutar el script interactivo
./add_ikonate_icons.sh

# Seleccionar opción 2 (Add icons from directory)
# Ingresar: /tmp/ikonate/icons/
```

### 5. Opción C: Manualmente / Option C: Manually

```bash
cd /home/runner/work/MARRTA/MARRTA

# Copiar iconos específicos
cp /tmp/ikonate/icons/folder.svg resources/icons/
cp /tmp/ikonate/icons/edit.svg resources/icons/
cp /tmp/ikonate/icons/settings.svg resources/icons/
cp /tmp/ikonate/icons/search.svg resources/icons/
cp /tmp/ikonate/icons/filter.svg resources/icons/

# Hacer backup del QRC
cp icons.qrc icons.qrc.backup

# Editar icons.qrc y añadir las líneas (antes de </qresource>):
cat >> icons_additions.txt << 'EOF'
        <file>resources/icons/folder.svg</file>
        <file>resources/icons/edit.svg</file>
        <file>resources/icons/settings.svg</file>
        <file>resources/icons/search.svg</file>
        <file>resources/icons/filter.svg</file>
EOF

# Nota: Debes insertar esto manualmente en icons.qrc antes de </qresource>
```

### 6. Verificar los cambios / Verify changes

```bash
# Listar iconos en el directorio
ls -la resources/icons/*.svg

# Verificar que están en el QRC
grep '<file>resources/icons/' icons.qrc

# O usar el script Python
./manage_ikonate_icons.py check
```

### 7. Recompilar el proyecto / Recompile the project

```bash
# Limpiar build anterior
make clean 2>/dev/null || true

# Recompilar
qmake marrta.pro && make

# O si tienes Qt5
qmake-qt5 marrta.pro && make -j$(nproc)
```

### 8. Usar los nuevos iconos en el código / Use new icons in code

Edita tus archivos .cpp para usar los nuevos iconos:

```cpp
// En mainwindow.cpp o donde necesites

// Ejemplo 1: Botón de búsqueda
QPushButton *searchBtn = new QPushButton("Buscar", this);
searchBtn->setIcon(QIcon(":/icons/resources/icons/search.svg"));

// Ejemplo 2: Acción de menú para configuración
QAction *settingsAction = new QAction(
    QIcon(":/icons/resources/icons/settings.svg"),
    "Configuración",
    this
);
connect(settingsAction, &QAction::triggered, this, &MainWindow::onSettings);

// Ejemplo 3: Tool button para editar
QToolButton *editBtn = new QToolButton(this);
editBtn->setIcon(QIcon(":/icons/resources/icons/edit.svg"));
editBtn->setToolTip("Editar elemento");

// Ejemplo 4: Acción en menú contextual
QMenu contextMenu(this);
contextMenu.addAction(
    QIcon(":/icons/resources/icons/folder.svg"),
    "Abrir carpeta",
    [=]{ openFolder(); }
);
contextMenu.addAction(
    QIcon(":/icons/resources/icons/trash.svg"),
    "Eliminar",
    [=]{ deleteItem(); }
);
```

## Ejemplo Completo de Flujo de Trabajo / Complete Workflow Example

```bash
#!/bin/bash
# Script de ejemplo completo

# 1. Preparar entorno
cd /home/runner/work/MARRTA/MARRTA
mkdir -p /tmp/ikonate

# 2. Descomprimir ikonate.zip (asumiendo que está en Downloads)
# unzip ~/Downloads/ikonate.zip -d /tmp/ikonate

# 3. Ver qué iconos hay disponibles
echo "Iconos disponibles en ikonate.zip:"
find /tmp/ikonate -name "*.svg" -type f | sort

# 4. Añadir iconos automáticamente
./manage_ikonate_icons.py add /tmp/ikonate/icons/

# 5. Verificar
echo -e "\nVerificando iconos añadidos:"
./manage_ikonate_icons.py check

# 6. Generar ejemplos de uso
echo -e "\nEjemplos de uso en C++:"
./manage_ikonate_icons.py examples

# 7. Recompilar
echo -e "\nRecompilando proyecto..."
make clean
qmake marrta.pro && make

# 8. Ejecutar aplicación
./marrta
```

## Iconos más Útiles para MARRTA / Most Useful Icons for MARRTA

Basado en la funcionalidad de MARRTA (análisis de riesgos), estos iconos serían útiles:

**Navegación y Archivo:**
- `folder.svg`, `folder-open.svg` - Gestión de archivos
- `document.svg` - Documentos
- `archive.svg` - Archivar

**Acciones:**
- `edit.svg` - Editar
- `copy.svg` - Copiar
- `delete.svg`, `trash.svg` - Eliminar
- `refresh.svg` - Actualizar

**Búsqueda y Filtrado:**
- `search.svg` - Buscar
- `filter.svg` - Filtrar

**Interfaz:**
- `settings.svg` - Configuración
- `menu.svg` - Menú
- `grid.svg` - Vista de cuadrícula
- `list.svg` - Vista de lista

**Seguridad y Alertas:**
- `lock.svg`, `unlock.svg` - Seguridad
- `alert.svg`, `warning.svg` - Alertas
- `shield.svg` - Protección

**Análisis:**
- `chart.svg` - Gráficos (complementa los existentes)
- `trending-up.svg` - Complementa trending-down.svg
- `analytics.svg` - Análisis

## Solución de Problemas Comunes / Common Troubleshooting

### Problema: El ZIP no se extrae correctamente
```bash
# Verificar el archivo
file ikonate.zip

# Listar contenido sin extraer
unzip -l ikonate.zip

# Extraer con verbose
unzip -v ikonate.zip -d /tmp/ikonate
```

### Problema: Los iconos no tienen el formato correcto
```bash
# Verificar que son SVG válidos
file /tmp/ikonate/icons/*.svg

# Ver el contenido de un SVG
head /tmp/ikonate/icons/folder.svg
```

### Problema: La compilación falla
```bash
# Verificar sintaxis del QRC
xmllint --noout icons.qrc

# O con Python
python3 -c "import xml.etree.ElementTree as ET; ET.parse('icons.qrc')"

# Restaurar backup si hay problemas
cp icons.qrc.backup icons.qrc
```

### Problema: Los iconos no se ven en la aplicación
```bash
# Verificar que se compilaron en el ejecutable
strings marrta | grep "resources/icons"

# Recompilar completamente
rm -f moc_*.cpp *.o
make clean
qmake && make
```

## Personalización de Iconos / Icon Customization

Si quieres cambiar el color de los iconos Ikonate:

```bash
# Cambiar color en un SVG (ejemplo: a negro)
sed -i 's/stroke="#2329D6"/stroke="#000000"/g' resources/icons/folder.svg

# O a azul oscuro
sed -i 's/stroke="#2329D6"/stroke="#0066CC"/g' resources/icons/edit.svg

# Cambiar el grosor del trazo
sed -i 's/stroke-width="1"/stroke-width="1.5"/g' resources/icons/settings.svg
```

## Resumen del Proceso / Process Summary

1. ✅ Descomprimir ikonate.zip
2. ✅ Copiar iconos a resources/icons/
3. ✅ Añadir entradas a icons.qrc
4. ✅ Recompilar con qmake && make
5. ✅ Usar en código con QIcon(":/icons/resources/icons/nombre.svg")

**Tiempo estimado:** 5-10 minutos
**Estimated time:** 5-10 minutes
