# Diagrama de Flujo: Añadir Iconos Ikonate
# Flow Diagram: Adding Ikonate Icons

## Flujo Visual del Proceso / Visual Process Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                    INICIO / START                                │
│              Descargar ikonate.zip del issue                     │
│              Download ikonate.zip from issue                     │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│                PASO 1: EXTRAER / STEP 1: EXTRACT                 │
│                                                                   │
│  mkdir -p /tmp/ikonate                                           │
│  unzip ikonate.zip -d /tmp/ikonate                              │
│                                                                   │
│  Resultado: Directorio con archivos SVG                         │
│  Result: Directory with SVG files                                │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
         ┌────────────┴────────────┐
         │  ¿Qué método usar?      │
         │  Which method to use?   │
         └────┬────────┬────────┬──┘
              │        │        │
      ┌───────▼─┐  ┌──▼───┐  ┌─▼────────┐
      │ Script  │  │Bash  │  │ Manual   │
      │ Python  │  │Script│  │          │
      └────┬────┘  └──┬───┘  └─┬────────┘
           │          │         │
           ▼          ▼         ▼

╔══════════════════════════════════════════════════════════════════╗
║           OPCIÓN A: SCRIPT PYTHON (RECOMENDADO)                  ║
║           OPTION A: PYTHON SCRIPT (RECOMMENDED)                  ║
╚══════════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────────────────────────────────┐
│  ./manage_ikonate_icons.py add /tmp/ikonate/icons/              │
│                                                                   │
│  ✓ Valida SVG automáticamente                                   │
│  ✓ Validates SVG automatically                                   │
│  ✓ Hace backup del QRC                                          │
│  ✓ Creates QRC backup                                            │
│  ✓ Actualiza icons.qrc                                          │
│  ✓ Updates icons.qrc                                             │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│  Verificar con: ./manage_ikonate_icons.py check                 │
│  Verify with: ./manage_ikonate_icons.py check                   │
└─────────────────────┬───────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════╗
║               OPCIÓN B: SCRIPT BASH                               ║
║               OPTION B: BASH SCRIPT                               ║
╚══════════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────────────────────────────────┐
│  ./add_ikonate_icons.sh                                         │
│                                                                   │
│  Menú interactivo:                                               │
│  Interactive menu:                                               │
│    1) Mostrar iconos / Show icons                               │
│    2) Añadir desde directorio / Add from directory              │
│    3) Añadir uno / Add single                                   │
│    ...                                                           │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│  Seleccionar opción 2                                           │
│  Select option 2                                                 │
│  Ingresar: /tmp/ikonate/icons/                                  │
│  Enter: /tmp/ikonate/icons/                                     │
└─────────────────────┬───────────────────────────────────────────┘

╔══════════════════════════════════════════════════════════════════╗
║               OPCIÓN C: MANUAL                                    ║
║               OPTION C: MANUAL                                    ║
╚══════════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────────────────────────────────┐
│  PASO 1: Copiar archivos                                         │
│  STEP 1: Copy files                                              │
│                                                                   │
│  cp /tmp/ikonate/icons/*.svg resources/icons/                   │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│  PASO 2: Hacer backup                                            │
│  STEP 2: Create backup                                           │
│                                                                   │
│  cp icons.qrc icons.qrc.backup                                  │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│  PASO 3: Editar icons.qrc                                        │
│  STEP 3: Edit icons.qrc                                          │
│                                                                   │
│  Añadir antes de </qresource>:                                  │
│  Add before </qresource>:                                        │
│                                                                   │
│    <file>resources/icons/folder.svg</file>                      │
│    <file>resources/icons/edit.svg</file>                        │
│    <file>resources/icons/settings.svg</file>                    │
└─────────────────────┬───────────────────────────────────────────┘

         ┌────────────┴────────────┐
         │  Todos los métodos      │
         │  All methods converge   │
         └────────────┬────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│           PASO 3: RECOMPILAR / STEP 3: RECOMPILE                │
│                                                                   │
│  make clean                  # Limpiar / Clean                  │
│  qmake marrta.pro           # Generar Makefile                  │
│  make                        # Compilar / Compile                │
│                                                                   │
│  O en un comando / Or in one command:                           │
│  qmake && make                                                   │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│     PASO 4: USAR EN CÓDIGO / STEP 4: USE IN CODE                │
│                                                                   │
│  QIcon(":/icons/resources/icons/folder.svg")                    │
│                                                                   │
│  Ejemplos / Examples:                                            │
│  • menu.addAction(QIcon(":/icons/..."), "Text", ...)           │
│  • button->setIcon(QIcon(":/icons/..."))                        │
│  • toolButton->setIcon(QIcon(":/icons/..."))                    │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────────┐
│              PASO 5: EJECUTAR / STEP 5: RUN                      │
│                                                                   │
│  ./marrta                                                        │
│                                                                   │
│  Verificar que los iconos aparecen correctamente                │
│  Verify icons appear correctly                                   │
└─────────────────────┬───────────────────────────────────────────┘
                      │
                      ▼
                  ┌───────┐
                  │ FIN   │
                  │ END   │
                  └───────┘
```

## Estructura de Archivos / File Structure

```
MARRTA/
├── resources/
│   └── icons/              ← Copiar archivos SVG aquí / Copy SVG files here
│       ├── activity.svg
│       ├── checked.svg
│       ├── folder.svg      ← NUEVO / NEW
│       ├── edit.svg        ← NUEVO / NEW
│       └── ...
│
├── icons.qrc              ← Registrar iconos aquí / Register icons here
│   
├── icons.qrc.backup       ← Backup automático / Automatic backup
│
├── manage_ikonate_icons.py ← Script Python / Python script
├── add_ikonate_icons.sh    ← Script Bash / Bash script
│
└── mainwindow.cpp         ← Usar iconos aquí / Use icons here
```

## Ejemplo de icons.qrc / Example icons.qrc

```xml
<RCC>
    <qresource prefix="/icons">
        <!-- Iconos existentes / Existing icons -->
        <file>resources/icons/checked.svg</file>
        <file>resources/icons/stop.svg</file>
        <file>resources/icons/save.svg</file>
        
        <!-- NUEVOS ICONOS / NEW ICONS -->
        <file>resources/icons/folder.svg</file>
        <file>resources/icons/edit.svg</file>
        <file>resources/icons/settings.svg</file>
        <file>resources/icons/search.svg</file>
    </qresource>
</RCC>
```

## Mapa de Decisiones / Decision Map

```
¿Tienes experiencia con Qt?
Do you have Qt experience?
        │
        ├── SÍ / YES ──────────► OPCIÓN C: Manual
        │                        (Rápido y directo / Fast and direct)
        │
        └── NO ────────────────► OPCIÓN A: Script Python
                                 (Automático y seguro / Automatic and safe)

¿Necesitas validación de SVG?
Do you need SVG validation?
        │
        ├── SÍ / YES ──────────► OPCIÓN A: Script Python
        │                        (Valida formato / Validates format)
        │
        └── NO ────────────────► OPCIÓN B o C
                                 (Más rápido / Faster)

¿Prefieres interfaz interactiva?
Do you prefer interactive interface?
        │
        ├── SÍ / YES ──────────► OPCIÓN B: Bash Script
        │                        (Menú interactivo / Interactive menu)
        │
        └── NO ────────────────► OPCIÓN A o C
                                 (Línea de comandos / Command line)
```

## Verificación Post-Instalación / Post-Installation Verification

```
1. Verificar archivos copiados / Verify copied files:
   ls -la resources/icons/*.svg

2. Verificar QRC actualizado / Verify updated QRC:
   grep '<file>resources/icons/' icons.qrc

3. Verificar compilación / Verify compilation:
   strings marrta | grep "resources/icons"

4. Verificar en aplicación / Verify in application:
   ./marrta
   (Los iconos deben aparecer / Icons should appear)
```

## Errores Comunes y Soluciones / Common Errors and Solutions

```
❌ Error: "file not found in QRC"
   Solución / Solution:
   → Verificar ruta en icons.qrc
   → Check path in icons.qrc
   → Debe ser: resources/icons/nombre.svg

❌ Error: "cannot parse QRC"
   Solución / Solution:
   → Verificar XML válido
   → Verify valid XML
   → Restaurar backup: cp icons.qrc.backup icons.qrc

❌ Error: "icons not appearing"
   Solución / Solution:
   → Recompilar completamente / Full rebuild:
     make clean && qmake && make

❌ Error: "qmake not found"
   Solución / Solution:
   → Instalar Qt5 / Install Qt5:
     sudo apt-get install qt5-qmake qtbase5-dev
```

## Tiempos Estimados / Estimated Times

```
Método A (Python):    3-5 minutos  / 3-5 minutes
Método B (Bash):      5-7 minutos  / 5-7 minutes  
Método C (Manual):    8-10 minutos / 8-10 minutes

Compilación:          2-5 minutos  / 2-5 minutes
Total:                5-15 minutos / 5-15 minutes
```
