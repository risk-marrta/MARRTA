# FAQ: Iconos Ikonate en MARRTA
# FAQ: Ikonate Icons in MARRTA

## 🇪🇸 Español

### Preguntas Generales

#### ¿Qué es Ikonate?
Ikonate es una colección de iconos SVG minimalistas, customizables y de código abierto. Son ideales para aplicaciones porque:
- Son vectoriales (escalables sin pérdida de calidad)
- Tienen un estilo consistente basado en trazos (stroke)
- Son personalizables (color, grosor)
- Son ligeros (archivos pequeños)

#### ¿Por qué usar Ikonate en vez de Theme/XDG icons?
Las ventajas son:
- **Portabilidad**: Los iconos van empaquetados en el ejecutable
- **Consistencia**: Se ven igual en todos los sistemas operativos
- **No dependen del sistema**: No requieren temas instalados
- **Customizables**: Puedes cambiar colores fácilmente
- **Modernos**: Estilo actual y profesional

#### ¿Cuántos iconos tiene Ikonate?
Ikonate tiene más de 100 iconos diferentes, cubriendo:
- Navegación (flechas, chevrons)
- Archivos y carpetas
- Edición y acciones
- Comunicación
- Multimedia
- Seguridad
- Análisis y gráficos

### Instalación y Configuración

#### ¿Cómo descargo los iconos de Ikonate?
El archivo `ikonate.zip` está adjunto en el issue. También puedes:
1. Descargar desde el issue de GitHub
2. Visitar https://ikonate.com/
3. Clonar el repositorio oficial de Ikonate

#### ¿Dónde coloco los archivos SVG?
Todos los archivos SVG van en el directorio:
```
/home/runner/work/MARRTA/MARRTA/resources/icons/
```

#### ¿Qué hago después de copiar los SVG?
Debes:
1. Registrarlos en `icons.qrc`
2. Recompilar el proyecto con `qmake && make`

#### ¿Puedo usar subdirectorios dentro de icons/?
No es recomendable. El sistema actual usa una estructura plana. Si usas subdirectorios, deberías:
- Actualizar las rutas en `icons.qrc`
- Actualizar las rutas en el código C++

### Uso de los Scripts

#### ¿Qué script debo usar?
Depende de tu preferencia:
- **manage_ikonate_icons.py**: Si quieres validación automática y más funciones
- **add_ikonate_icons.sh**: Si prefieres un menú interactivo en bash
- **Manual**: Si tienes experiencia con Qt y prefieres control total

#### ¿El script Python funciona en mi sistema?
El script requiere Python 3.6+ (viene instalado en la mayoría de sistemas Linux). Usa solo bibliotecas estándar:
- `xml.etree.ElementTree` (incluida)
- `pathlib` (incluida)
- `shutil` (incluida)

#### ¿Cómo uso el script Python desde línea de comandos?
```bash
# Ver iconos actuales
./manage_ikonate_icons.py list

# Verificar QRC
./manage_ikonate_icons.py check

# Añadir iconos
./manage_ikonate_icons.py add /ruta/a/iconos/

# Modo interactivo
./manage_ikonate_icons.py
```

#### ¿El script hace backup automático?
Sí, ambos scripts (Python y Bash) hacen backup automático de `icons.qrc` antes de modificarlo.

### Compilación

#### ¿Qué comando debo usar para recompilar?
```bash
qmake marrta.pro && make
```

O si tienes Qt5 específicamente:
```bash
qmake-qt5 marrta.pro && make
```

#### ¿Necesito limpiar antes de compilar?
No siempre, pero es recomendable cuando:
- Añades/eliminas muchos iconos
- Cambias el archivo QRC significativamente
- Tienes problemas con la compilación

Para limpiar:
```bash
make clean
qmake && make
```

#### ¿Cuánto tarda la compilación?
Depende de tu sistema:
- Primera compilación completa: 5-10 minutos
- Recompilación incremental: 30-60 segundos
- Solo recursos (QRC): 10-20 segundos

#### ¿Qué hago si qmake no está instalado?
En Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install qt5-qmake qtbase5-dev
```

En Fedora/RHEL:
```bash
sudo dnf install qt5-qtbase-devel
```

### Uso en Código

#### ¿Cómo uso un icono en mi código C++?
```cpp
QIcon(":/icons/resources/icons/nombre-icono.svg")
```

#### ¿Puedo acortar la ruta del icono?
El prefijo `:/icons/` se puede personalizar en `icons.qrc`, pero no es recomendable cambiarlo porque ya está usado en todo el código.

#### ¿Cómo cambio un icono existente?
Solo cambia el nombre del archivo en la llamada a QIcon:
```cpp
// Antes
button->setIcon(QIcon(":/icons/resources/icons/old-icon.svg"));

// Después
button->setIcon(QIcon(":/icons/resources/icons/new-icon.svg"));
```

#### ¿Puedo cambiar el icono dinámicamente en runtime?
Sí:
```cpp
// Cambiar icono según estado
if (condition) {
    button->setIcon(QIcon(":/icons/resources/icons/checked.svg"));
} else {
    button->setIcon(QIcon(":/icons/resources/icons/stop.svg"));
}
```

### Personalización

#### ¿Puedo cambiar el color de los iconos?
Sí, editando el archivo SVG:
```xml
<!-- En el archivo .svg, cambiar: -->
<svg stroke="#2329D6" ...>
<!-- Por: -->
<svg stroke="#000000" ...>  <!-- Negro -->
```

Colores comunes:
- Negro: `#000000`
- Blanco: `#FFFFFF`
- Azul: `#0066CC`
- Verde: `#00AA00`
- Rojo: `#CC0000`

#### ¿Puedo cambiar el grosor del trazo?
Sí:
```xml
<!-- Cambiar -->
stroke-width="1"
<!-- Por -->
stroke-width="1.5"  <!-- Más grueso -->
stroke-width="0.5"  <!-- Más delgado -->
```

#### ¿Puedo cambiar el tamaño del icono?
Los iconos SVG escalan automáticamente. En el código:
```cpp
QIcon icon(":/icons/resources/icons/folder.svg");
button->setIconSize(QSize(32, 32));  // Tamaño en píxeles
```

#### ¿Necesito recompilar después de cambiar colores en SVG?
Sí, porque los SVG se empaquetan en el ejecutable durante la compilación.

### Troubleshooting

#### Los iconos no aparecen en la aplicación
Posibles causas:
1. No recompilaste después de añadirlos
   → Ejecuta: `qmake && make`

2. La ruta en el código es incorrecta
   → Verifica: `QIcon(":/icons/resources/icons/nombre.svg")`

3. El icono no está en el QRC
   → Ejecuta: `./manage_ikonate_icons.py check`

4. El archivo SVG está corrupto
   → Verifica: `file resources/icons/nombre.svg`

#### Error: "RCC: Cannot open file"
El archivo listado en `icons.qrc` no existe.
```bash
# Verificar que todos los archivos existen
for icon in $(grep '<file>' icons.qrc | sed 's/.*<file>\(.*\)<\/file>/\1/'); do
    [ -f "$icon" ] || echo "Missing: $icon"
done
```

#### Error: "XML parsing failed"
El archivo `icons.qrc` tiene XML inválido.
```bash
# Validar XML
xmllint --noout icons.qrc

# O con Python
python3 -c "import xml.etree.ElementTree as ET; ET.parse('icons.qrc')"

# Restaurar backup si es necesario
cp icons.qrc.backup icons.qrc
```

#### Los iconos se ven borrosos
Los iconos Ikonate son SVG y no deberían verse borrosos. Si pasa:
1. Verifica que el archivo sea SVG válido
2. Asegúrate de que el viewBox esté correcto (24x24)
3. No uses escalado extremo (muy grande o muy pequeño)

#### El script Python da error de permisos
```bash
# Dar permisos de ejecución
chmod +x manage_ikonate_icons.py

# O ejecutar con python3 directamente
python3 manage_ikonate_icons.py
```

### Mejores Prácticas

#### ¿Debo añadir todos los iconos de Ikonate?
No, solo añade los que necesites para:
- Mantener el tamaño del ejecutable pequeño
- Facilitar el mantenimiento
- Evitar confusión

#### ¿Cómo organizo los iconos?
Sugerencias:
1. Usa nombres descriptivos en inglés (ej: `folder.svg`, no `carpeta.svg`)
2. Mantén consistencia con los nombres existentes
3. Documenta qué iconos usas y dónde

#### ¿Debo hacer backup del QRC antes de cambios?
Sí, siempre:
```bash
cp icons.qrc icons.qrc.backup
```
Los scripts lo hacen automáticamente.

#### ¿Cómo pruebo que un icono funciona?
1. Compilar: `qmake && make`
2. Ejecutar: `./marrta`
3. Verificar visualmente en la UI

### Recursos Adicionales

#### ¿Dónde encuentro más iconos de Ikonate?
- Sitio oficial: https://ikonate.com/
- GitHub: https://github.com/mikolajdobrucki/ikonate
- El archivo ikonate.zip del issue

#### ¿Hay documentación de Qt sobre recursos?
Sí:
- Qt Resource System: https://doc.qt.io/qt-5/resources.html
- QIcon: https://doc.qt.io/qt-5/qicon.html
- QRC format: https://doc.qt.io/qt-5/resources.html

---

## 🇬🇧 English

### General Questions

#### What is Ikonate?
Ikonate is a collection of minimalist, customizable, open-source SVG icons. They're ideal for applications because:
- They're vector-based (scalable without quality loss)
- They have a consistent stroke-based style
- They're customizable (color, thickness)
- They're lightweight (small files)

#### Why use Ikonate instead of Theme/XDG icons?
Advantages:
- **Portability**: Icons are packaged in the executable
- **Consistency**: Look the same across all operating systems
- **System independent**: Don't require installed themes
- **Customizable**: Easy color changes
- **Modern**: Current and professional style

### Installation and Configuration

#### Where do I place the SVG files?
All SVG files go in:
```
/home/runner/work/MARRTA/MARRTA/resources/icons/
```

#### What do I do after copying the SVGs?
You must:
1. Register them in `icons.qrc`
2. Recompile the project with `qmake && make`

### Using the Scripts

#### Which script should I use?
Depends on your preference:
- **manage_ikonate_icons.py**: For automatic validation and more features
- **add_ikonate_icons.sh**: For interactive bash menu
- **Manual**: If you have Qt experience and prefer full control

#### How do I use the Python script from command line?
```bash
# View current icons
./manage_ikonate_icons.py list

# Check QRC
./manage_ikonate_icons.py check

# Add icons
./manage_ikonate_icons.py add /path/to/icons/

# Interactive mode
./manage_ikonate_icons.py
```

### Compilation

#### What command should I use to recompile?
```bash
qmake marrta.pro && make
```

Or if you have Qt5 specifically:
```bash
qmake-qt5 marrta.pro && make
```

### Using in Code

#### How do I use an icon in my C++ code?
```cpp
QIcon(":/icons/resources/icons/icon-name.svg")
```

#### Can I change icons dynamically at runtime?
Yes:
```cpp
if (condition) {
    button->setIcon(QIcon(":/icons/resources/icons/checked.svg"));
} else {
    button->setIcon(QIcon(":/icons/resources/icons/stop.svg"));
}
```

### Customization

#### Can I change the icon color?
Yes, by editing the SVG file:
```xml
<!-- In the .svg file, change: -->
<svg stroke="#2329D6" ...>
<!-- To: -->
<svg stroke="#000000" ...>  <!-- Black -->
```

### Troubleshooting

#### Icons don't appear in the application
Possible causes:
1. Didn't recompile after adding them
   → Run: `qmake && make`

2. Path in code is incorrect
   → Verify: `QIcon(":/icons/resources/icons/name.svg")`

3. Icon not in QRC
   → Run: `./manage_ikonate_icons.py check`

#### Error: "RCC: Cannot open file"
The file listed in `icons.qrc` doesn't exist.
```bash
# Verify all files exist
for icon in $(grep '<file>' icons.qrc | sed 's/.*<file>\(.*\)<\/file>/\1/'); do
    [ -f "$icon" ] || echo "Missing: $icon"
done
```

### Best Practices

#### Should I add all Ikonate icons?
No, only add what you need to:
- Keep executable size small
- Facilitate maintenance
- Avoid confusion

---

## Comandos Rápidos / Quick Commands

```bash
# Listar iconos actuales / List current icons
./manage_ikonate_icons.py list

# Verificar QRC / Check QRC
./manage_ikonate_icons.py check

# Añadir iconos / Add icons
./manage_ikonate_icons.py add /tmp/ikonate/icons/

# Backup QRC
./manage_ikonate_icons.py backup

# Recompilar / Recompile
qmake && make

# Ejecutar / Run
./marrta
```
