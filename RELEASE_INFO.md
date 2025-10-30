# MARRTA Release Information

This document records changes and updates for each version of MARRTA.

---

## Upcoming Changes (Not Yet Released)

### Chart Improvements

- **Enhanced Color Scheme for Summary Charts** - Updated the frequency and consequence distribution charts to use the same color scheme as the risk distribution chart
  - Frequency categories (Muy Baja, Baja, Media, Alta) now use risk colors (gray, green, yellow, red) 
  - Consequence categories (Bajas, Medias, Altas, Muy Altas) now use risk colors (gray, green, yellow, red)
  - Base values use lighter tones of the same colors for better differentiation
  - Calculated values use the exact risk colors from the main chart
  - Each category now has distinct color coding matching its risk level
## Upcoming Changes (In Development)

### Python Scripting Engine - Improved Initialization Resilience

**Changes Made:**
- Enhanced Python interpreter initialization with comprehensive error handling
- Added graceful degradation when Python initialization fails - application continues with Python features disabled
- Added `pythonInitializationError()` getter to retrieve human-readable error messages
- Added `isPythonAvailable()` convenience method for checking Python availability
- Improved error reporting - errors are logged but not shown on application startup
- **Error dialogs moved to Python editor**: Users only see Python error messages when they attempt to open the Python script editor
- **Installation instructions included**: When Python is not available, the error dialog provides:
  - Link to download Python 3.12 (compatible version)
  - Step-by-step installation instructions
  - Troubleshooting tips for common issues
  - Technical error details for debugging
- Added proper cleanup (Py_Finalize) when initialization fails after Py_Initialize succeeds
- All initialization failures are now non-fatal - errors are logged with qCritical()
- Error messages use Qt tr() for internationalization support
- Use PyConfig API for safer initialization that prevents fatal errors and aborts
- Check for Python errors (PyErr_Occurred) after initialization to catch library loading issues
- Disable site_import during initialization to avoid requiring full Python stdlib

**Files Modified:**
- `pythonscriptingengine.h`: Added `pythonInitError_` member, `pythonInitializationError()` and `isPythonAvailable()` methods
- `pythonscriptingengine.cpp`: Improved `initializePython()` with PyConfig-based initialization, comprehensive error handling, PyErr checking, removed startup dialogs
- `pythonscriptingeditor.cpp`: Added detailed error dialog with installation instructions when Python is not available
## Upcoming Changes (TBD)

### UI Improvements
- **Added Authors List to About Dialog**: Added dedicated "Autores (orden alfabético)" section in the About dialog (about.ui) as a separate section with horizontal line separators and bold title. Lists 13 project authors in alphabetical order: Arturo Pérez, Carlos Ferrer, Carlos Prieto, Cristina Sánchez, Elena Ferreras, Jessica Vilanova, José Miguel Delgado, José Pardo, María Jesús Suárez, María José Rot, María Luisa Ramírez, Sheila Ruíz, and Vanesa Vázquez. Section appears after the "Creado dentro del Proyecto MARRTA" logos section.

---

## Version 2.0.0 (2025-10-24)

**Major Release - Significant Feature Enhancements**

This release represents a major milestone in MARRTA development, introducing powerful new capabilities and significant improvements to user experience, analysis tools, and automation features.

### Summary of New Features

1. Python Scripting Engine (v3.12) - Complete Python integration with comprehensive API
2. Integrated Stage Management Dialog - Unified interface with immediate database persistence
3. Interactive Flowchart Generator - HTML flowcharts with visual risk analysis
4. Comprehensive User Manual Documentation - Built-in 7-section user guide
5. Enhanced HTML Report Generator - Professional reports with CSS customization
6. GitHub Integration for User Feedback - Direct bug reporting system
7. Legal Compliance System - Disclaimer and license acceptance dialogs
8. Excel Import/Export Improvements - Fixed validations and documentation
9. Defense Management Enhancements - Mark/unmark by type with real-time updates
10. Interface and Menu Improvements - Reorganized UI with new features

### Top 10 New Features

1. **Python Scripting Engine (v3.12)**
   - Complete Python 3.12 scripting engine integrated alongside JavaScript engine
   - Comprehensive Python API: `Marrta`, `Database`, `Etapas`, `Sucesos`, and `Defensas` modules
   - Python syntax highlighting, find/replace, and interactive documentation
   - Optional compilation flag for flexible deployment
   - Test scripts and complete API reference documentation

2. **Integrated Stage Management Dialog**
   - Completely redesigned unified stage/substage management interface
   - Split-panel layout with immediate database persistence
   - Inline editing of all stage and substage properties
   - Drag-and-drop reordering with up/down buttons
   - Enhanced validation and transaction support
   - Replaces separate dialogs with streamlined workflow

3. **Interactive Flowchart Generator**
   - Generate HTML flowcharts with visual risk analysis
   - Color-coded risk levels (RB, RM, RA, RMA) and defense types
   - Interactive filtering by stage, substage, risk level, and robustness
   - Right-click drag to reposition fictional stages
   - Zoom/pan controls with fast pan mode (Shift+drag)
   - Export to HTML for sharing and presentations

4. **Comprehensive User Manual Documentation**
   - Built-in user manual with 7 main sections
   - Two-panel navigation with tree view and content browser
   - Complete Excel import format specification with examples
   - Detailed workflow guides for risk analysis process
   - All menu and feature explanations
   - Replaces simple help messages with professional documentation

5. **Enhanced HTML Report Generator**
   - Professional MARRTA-branded reports with custom CSS
   - External CSS management: save, import, and customize styles
   - Logo embedding for portable HTML reports
   - Report preview with DevTools for CSS/HTML debugging
   - Configurable sections: stages, sequences, initiators, defenses
   - Stage/substage filtering with risk level selection
   - Portrait and landscape orientation support

6. **GitHub Integration for User Feedback**
   - Direct bug reporting and feature requests to GitHub
   - Structured feedback form with category selection
   - Support for file attachments via external links
   - Automatic issue creation with labels
   - Alternative email contact when GitHub submission fails
   - First-run disclaimer and license acceptance system

7. **Legal Compliance System**
   - Medical device disclaimer dialog with first-run acceptance
   - GPLv3 license dialog with multi-language support (ES/EN/FR/PT)
   - Persistent acceptance tracking in settings
   - Application exits if disclaimer not accepted
   - Professional legal notices accessible from Help menu

8. **Excel Import/Export Improvements**
   - Fixed sheet count validation (6 sheets required)
   - Corrected sheet names: "Sucesos" instead of "Eventos"
   - Fixed robustness values documentation (-1 to 3 scale)
   - Removed incorrect "Efectividad" field references
   - Updated inline help with accurate field specifications
   - Enhanced import wizard with model loading after conversion

9. **Defense Management Enhancements**
   - Mark/unmark defenses by type with submenus
   - Fixed defense marking logic (explicit set vs toggle)
   - Real-time event list updates with risk recalculation
   - Defense codes and hardiness display in lists
   - Context menu operations with proper risk updates
   - Defense counter display [n/m] for each type

10. **Interface and Menu Improvements**
    - Renamed "Análisis y Generación" to "Herramientas"
    - New Excel import icon (blue color)
    - SEVRRA import placeholder (disabled, future feature)
    - Reorganized toolbar with better visual separation
    - Model verification before opening scripting editors
    - Contact information in error messages
    - Save As functionality implemented

### Additional Improvements

- **Report DevTools Support**: Chromium developer tools for HTML/CSS inspection
- **Event Filtering**: Fixed filter behavior to respect stage selection
- **Event Counter**: Added [n/m] counter showing applicable events
- **Mark/Unmark Initiators**: New menu options for bulk initiator management
- **Native Platform Colors**: System-themed UI with soft alternating rows
- **Compilation Fixes**: MSVC compatibility, Qt 6.10 API updates
- **Translation Support**: All UI text uses tr() for internationalization
- **Code Quality**: Extracted duplicated logic, improved error handling

### Breaking Changes

- None - This release maintains backward compatibility with existing models and scripts

### Migration Notes

- Python support is optional - application works without Python installed
- Existing JavaScript scripts continue to work unchanged
- Database format remains compatible with version 1.x models
- Custom CSS styles from previous versions may need minor adjustments

### Known Issues

- None reported

### Contributors

Thanks to all contributors who made this release possible.

---

## Versión 2.0.0 (2025-10-24)

**Lanzamiento Mayor - Mejoras Significativas de Funcionalidades**

Esta versión representa un hito importante en el desarrollo de MARRTA, introduciendo nuevas capacidades potentes y mejoras significativas en la experiencia de usuario, herramientas de análisis y funciones de automatización.

### Resumen de Nuevas Funcionalidades

1. Motor de Scripting en Python (v3.12) - Integración completa de Python con API integral
2. Diálogo Integrado de Gestión de Etapas - Interfaz unificada con persistencia inmediata en base de datos
3. Generador Interactivo de Flujogramas - Flujogramas HTML con análisis visual de riesgos
4. Documentación Completa del Manual de Usuario - Manual integrado con 7 secciones
5. Generador Mejorado de Informes HTML - Informes profesionales con personalización CSS
6. Integración con GitHub para Retroalimentación - Sistema de reporte directo de errores
7. Sistema de Cumplimiento Legal - Diálogos de aviso legal y aceptación de licencia
8. Mejoras en Importación/Exportación de Excel - Validaciones y documentación corregidas
9. Mejoras en Gestión de Defensas - Marcar/desmarcar por tipo con actualizaciones en tiempo real
10. Mejoras de Interfaz y Menús - UI reorganizada con nuevas funcionalidades

### Top 10 Nuevas Funcionalidades

1. **Motor de Scripting en Python (v3.12)**
   - Motor de scripting Python 3.12 completo integrado junto al motor JavaScript
   - API Python integral: módulos `Marrta`, `Database`, `Etapas`, `Sucesos` y `Defensas`
   - Resaltado de sintaxis Python, buscar/reemplazar y documentación interactiva
   - Flag de compilación opcional para despliegue flexible
   - Scripts de prueba y documentación completa de referencia API

2. **Diálogo Integrado de Gestión de Etapas**
   - Interfaz unificada completamente rediseñada para gestión de etapas/subetapas
   - Diseño de panel dividido con persistencia inmediata en base de datos
   - Edición en línea de todas las propiedades de etapas y subetapas
   - Reordenamiento con arrastrar y soltar usando botones arriba/abajo
   - Validación mejorada y soporte de transacciones
   - Reemplaza diálogos separados con flujo de trabajo optimizado

3. **Generador Interactivo de Flujogramas**
   - Generación de flujogramas HTML con análisis visual de riesgos
   - Niveles de riesgo codificados por color (RB, RM, RA, RMA) y tipos de defensa
   - Filtrado interactivo por etapa, subetapa, nivel de riesgo y robustez
   - Arrastre con botón derecho para reposicionar etapas ficticias
   - Controles de zoom/paneo con modo de paneo rápido (Shift+arrastrar)
   - Exportación a HTML para compartir y presentaciones

4. **Documentación Completa del Manual de Usuario**
   - Manual de usuario integrado con 7 secciones principales
   - Navegación de dos paneles con vista de árbol y navegador de contenido
   - Especificación completa del formato de importación Excel con ejemplos
   - Guías detalladas del flujo de trabajo del proceso de análisis de riesgos
   - Explicaciones de todos los menús y funcionalidades
   - Reemplaza mensajes de ayuda simples con documentación profesional

5. **Generador Mejorado de Informes HTML**
   - Informes profesionales con marca MARRTA y CSS personalizable
   - Gestión de CSS externo: guardar, importar y personalizar estilos
   - Incrustación de logo para informes HTML portables
   - Vista previa de informes con DevTools para depuración CSS/HTML
   - Secciones configurables: etapas, secuencias, iniciadores, defensas
   - Filtrado por etapa/subetapa con selección de nivel de riesgo
   - Soporte de orientación vertical y apaisada

6. **Integración con GitHub para Retroalimentación de Usuarios**
   - Reporte directo de errores y solicitudes de funcionalidades a GitHub
   - Formulario de retroalimentación estructurado con selección de categoría
   - Soporte para adjuntos de archivos vía enlaces externos
   - Creación automática de issues con etiquetas
   - Contacto por email alternativo cuando falla el envío a GitHub
   - Sistema de aceptación de aviso legal y licencia en primera ejecución

7. **Sistema de Cumplimiento Legal**
   - Diálogo de aviso legal de dispositivo médico con aceptación en primera ejecución
   - Diálogo de licencia GPLv3 con soporte multiidioma (ES/EN/FR/PT)
   - Seguimiento persistente de aceptación en configuración
   - La aplicación se cierra si no se acepta el aviso legal
   - Avisos legales profesionales accesibles desde el menú Ayuda

8. **Mejoras en Importación/Exportación de Excel**
   - Validación corregida del número de hojas (6 hojas requeridas)
   - Nombres de hojas corregidos: "Sucesos" en lugar de "Eventos"
   - Documentación corregida de valores de robustez (escala -1 a 3)
   - Referencias eliminadas al campo incorrecto "Efectividad"
   - Ayuda en línea actualizada con especificaciones de campos precisas
   - Asistente de importación mejorado con carga de modelo después de conversión

9. **Mejoras en Gestión de Defensas**
   - Marcar/desmarcar defensas por tipo con submenús
   - Lógica de marcado de defensas corregida (asignación explícita vs alternar)
   - Actualizaciones de lista de eventos en tiempo real con recálculo de riesgos
   - Códigos de defensa y dureza mostrados en listas
   - Operaciones de menú contextual con actualizaciones apropiadas de riesgo
   - Visualización de contador de defensas [n/m] para cada tipo

10. **Mejoras de Interfaz y Menús**
    - Renombrado "Análisis y Generación" a "Herramientas"
    - Nuevo icono de importación Excel (color azul)
    - Marcador de posición para importación SEVRRA (deshabilitado, funcionalidad futura)
    - Barra de herramientas reorganizada con mejor separación visual
    - Verificación de modelo antes de abrir editores de scripting
    - Información de contacto en mensajes de error
    - Funcionalidad Guardar Como implementada

### Mejoras Adicionales

- **Soporte DevTools para Informes**: Herramientas de desarrollo Chromium para inspección HTML/CSS
- **Filtrado de Eventos**: Comportamiento de filtro corregido para respetar selección de etapa
- **Contador de Eventos**: Contador [n/m] añadido mostrando eventos aplicables
- **Marcar/Desmarcar Iniciadores**: Nuevas opciones de menú para gestión masiva de iniciadores
- **Colores Nativos de Plataforma**: UI con tema del sistema con filas alternas suaves
- **Correcciones de Compilación**: Compatibilidad con MSVC, actualizaciones de API Qt 6.10
- **Soporte de Traducción**: Todo el texto de UI usa tr() para internacionalización
- **Calidad de Código**: Lógica duplicada extraída, manejo de errores mejorado

### Cambios Incompatibles

- Ninguno - Esta versión mantiene compatibilidad hacia atrás con modelos y scripts existentes

### Notas de Migración

- El soporte de Python es opcional - la aplicación funciona sin Python instalado
- Los scripts JavaScript existentes continúan funcionando sin cambios
- El formato de base de datos permanece compatible con modelos versión 1.x
- Los estilos CSS personalizados de versiones anteriores pueden necesitar ajustes menores

### Problemas Conocidos

- Ninguno reportado

### Colaboradores

Gracias a todos los colaboradores que hicieron posible esta versión.

---

## Unreleased

### PR #259: Add Model Comparison Feature (Comparador de Modelos) - 2025-10-24
#### Changes
- **New Model Comparison Dialog**:
  - Created `DlgComparadorModelos` dialog (dlgcomparadormodelos.h/cpp/ui) for comprehensive model comparison
  - Compare current risk model with another PRAXIS database file
  - Comprehensive comparison features:
    - **Stages and Substages**: Shows number of stages/substages in each model and highlights differences
    - **Initiators**: Compares total initiators, by stage, and applicable initiators
    - **Initiator Details**: Identifies initiators with different codes, names, or F/C values
    - **Defenses**: Compares defense codes, names, and robustness levels
    - **Defense Associations**: Shows which defenses are established and applied to each initiator in both models
    - **Risk Profile Charts**: Generates visual comparison charts showing risk distribution
  - Results displayed in tabbed interface with 6 tabs:
    - Summary: Overall comparison statistics
    - Stages: Detailed stage/substage comparison table
    - Initiators: Initiator-by-initiator comparison with differences highlighted
    - Defenses: Defense comparison with robustness differences
    - Associations: Defense-to-initiator association comparison
    - Charts: Visual risk profile comparison charts
  - Color-coded differences for easy identification
  - Export functionality to HTML report with all comparison data
- **Menu Integration**:
  - Added "Comparador de modelos" action to "Administración" menu
  - Uses search icon for visual consistency
  - Requires a model to be open before use
- **Technical Implementation**:
  - Opens second database connection for comparison model
  - Loads all events and defenses from both models
  - Performs intelligent comparison across all dimensions
  - Uses Qt Charts for visual risk profile comparison
  - Generates comprehensive HTML reports for export
- **Files Added**:
  - dlgcomparadormodelos.h/cpp/ui: Complete model comparison dialog
- **Files Modified**:
  - mainwindow.h: Added slot for comparison action
  - mainwindow.cpp: Added include and action handler implementation
  - mainwindow.ui: Added menu action "actionComparadorModelos" to Administration menu
  - marrta.pro: Added new dialog files to SOURCES, HEADERS, and FORMS sections
### PR: Fix Compilation Errors in Online Help (usermanualdocumentation.cpp)
#### Changes
- **Fixed Syntax Error on Line 825**:
  - Added missing opening double quote `"` before `<li>Robustez: Débil, Media, Robusta</li>`
  - This line was being interpreted as C++ code instead of a string literal
  - Resolved compilation errors: C2065 (undeclared identifiers 'li' and 'Robustez'), C2143 (syntax errors)
- **Corrected Robustness Values Documentation**:
  - Updated robustness values from incorrect "Débil, Media, Robusta" (3 values) to correct "Muy blanda, Blanda, Normal, Robusta, Muy robusta" (5 values)
  - Updated all three "Valores de Robustez" tables in Excel import documentation to show complete range: -1 (Muy blanda), 0 (Blanda), 1 (Normal), 2 (Robusta), 3 (Muy robusta)
  - Updated example value ranges in column descriptions from "0, 1, 2" to "-1, 0, 1, 2, 3"
  - These values now match the actual implementation in matrizriesgo.cpp and listabarreras.cpp
- **Impact**:
  - Fixes build failures on Windows with MSVC compiler
  - User manual documentation now compiles correctly
  - Documentation now accurately reflects the robustness scale used in the application

### PR: Fix Inline Help for Excel Import
#### Changes
- **Fixed Excel Sheet Count**:
  - Corrected documentation from "exactamente 5 hojas" to "exactamente 6 hojas"
  - The import code actually expects 6 sheets, not 5
- **Corrected Sheet Names**:
  - Changed sheet 3 from "Eventos" to "Sucesos" (the actual sheet name used by import code)
  - Replaced incorrect "Hoja 4: Defensas" with "Hoja 4: Reductores de frecuencia"
  - Replaced incorrect "Hoja 5: Defensas por Evento" with "Hoja 5: Barreras"
  - Added "Hoja 6: Reductores de consecuencia" (was missing)
- **Removed Incorrect "Efectividad" Field**:
  - Removed all references to "Efectividad" field in defense documentation
  - All defense types (Reductores de frecuencia, Barreras, Reductores de consecuencia) use "Robustez" field
  - The import code only uses "Robustez", not "Efectividad"
- **Updated Sheet Structure Documentation**:
  - Each defense sheet now correctly documents: Id, Suceso, Código, Nombre, Descripción, Robustez, Marcado, Orden
  - Provided accurate examples matching actual Excel export format
  - Updated all cross-references to use correct sheet names
- **Export Documentation**:
  - Updated export section to reflect 6 sheets instead of 5
  - Corrected examples to use "Sucesos" instead of "Eventos"

### PR: New Integrated Stage Management Dialog
#### Changes
- **New DlgGestionEtapas Dialog**:
  - Created completely redesigned stage/substage management window
  - Replaces separate Etapas and Subetapas dialogs with unified interface
  - Split-panel layout: stages on left, details and substages on right
  - All changes save directly to database immediately (no Save button)
- **Stage Management Features**:
  - Create stages with required Name and Code (max 3 chars), optional Description
  - Delete stages with confirmation and cascade delete of substages
  - Reorder stages with up/down buttons
  - Inline editing of all stage properties
- **Substage Management Features**:
  - Create substages with required Name, optional Description
  - Delete substages with confirmation
  - Reorder substages with up/down buttons
  - Toggle active/inactive status with checkbox
  - Inline editing of all substage properties
- **Technical Improvements**:
  - Immediate database persistence with transaction support
  - All UI text uses tr() for internationalization
  - Better visual hierarchy showing stage-substage relationships
  - Proper validation of required fields
  - Prevents recursive UI updates with updatingUI flag
- **Integration**:
  - Updated MainWindow to use new dialog
  - Model reloads automatically when dialog closes
  - Added files to marrta.pro build system
  - Created comprehensive implementation documentation
### PR: Interface Improvements for Menus and Toolbar
#### Changes
- **Menu Renamed**:
  - Changed "Análisis y Generación" to "Herramientas"
- **Excel Import Icon**:
  - Changed import Excel icon to blue color (excel-blue.svg)
  - Created new blue version of Excel icon
- **New Import Option**:
  - Added "Importar de SEVRRA" option below Excel import (disabled/grayed out)
  - Placeholder for future SEVRRA import functionality
- **Archivo Menu Cleanup**:
  - Removed "Scripting" submenu from "Archivo" menu (moved to Herramientas)
- **Modelo Menu Cleanup**:
  - Removed "Generar informe" entry from Modelo menu (now only in Herramientas)
  - Removed duplicate "Generar informe..." entry (actionGenerarInformeHTML)
  - Removed "Generar flujograma..." entry (moved to Herramientas menu)
- **Contact Information Added**:
  - Added email contact (matrices.marrta@gmail.com) to feedback error messages
  - Added email to "Enviar modelo de riesgo" message
  - Users now have alternative contact method when GitHub submission fails
- **Scripting Model Check**:
  - Added model open verification to both scripting editors
  - Editor JS now checks if model is loaded before opening
  - Python editor now checks if model is loaded before opening
  - Shows warning message "No hay ningún modelo de riesgo cargado" if no model is open
- **Toolbar Updates**:
  - Removed "Editor JS" button from toolbar
  - Added two separators before "Generar informe" button (replacing Editor JS position)
  - Added two separators between "Resumen" and "Etapas" buttons
  - Toolbar now has better visual separation of functional groups
- **Translation Improvements**:
  - Updated saveModel() messages to use tr() for proper internationalization
  - All user-facing strings now properly translatable
- **Files Modified**:
  - mainwindow.ui: Menu structure changes, toolbar updates, new action definitions
  - mainwindow.cpp: Model checks, contact information in messages
  - dlgsendfeedback.cpp: Email contact in error messages
  - icons.qrc: Added excel-blue.svg resource
  - resources/icons/excel-blue.svg: New blue Excel icon (NEW)
  - RELEASE_INFO.md: This file
### PR: Add User Manual Documentation Dialog
#### Changes
- **New User Manual Documentation**:
  - Created `UserManualDocumentation` dialog (usermanualdocumentation.h/cpp) with comprehensive user manual
  - Replaces simple "coming soon" message box in Help → Ayuda with full documentation window
  - Two-panel layout with navigation tree (left) and content browser (right)
  - Documentation organized in 7 main sections as requested:
    1. **Introducción**: General introduction, system requirements, and capabilities
    1.1. **Elementos de la pantalla principal**: Detailed description of UI elements including menu bar, navigation panel, event list, filters, event details, defense lists, and status bar
    2. **Proceso de análisis**: Complete analysis workflow including model loading, marking initiators as applicable, configuring defenses, understanding initiator vs. sequence differences, and reviewing results
    3. **Menú Archivo**: Explanation of all File menu options with special detail on Import from Excel including complete Excel file format specification with example tables for each sheet (Etapas, Subetapas, Eventos, Defensas, Defensas por Evento)
    4. **Menú Modelo**: Model menu options with clear explanation of how mark/unmark operations affect visible events (filtered) vs. all defenses (global)
    5. **Menú Herramientas**: Description of Tools menu including scripting engines (JavaScript and Python with Python 3.12 requirement), reports, and flowcharts
    6. **Menú Administración**: Description of Administration menu with three management windows (Stages, Initiator Events, and Defenses)
    7. **Menú Ayuda**: Help menu description including update checks, new model checks, and bug reporting through GitHub integration
  - Comprehensive Excel import format documentation with tables showing required columns, allowed values, and examples
  - Clarification on "aplicable" terminology (applicable to our department) vs. "completado" in database
  - Detailed explanation of difference between initiator event (F, C with P=max) and sequence (F, P, C with active defenses)
  - Comparison table between Summary view and Detailed HTML report
  - All text uses tr() for proper translation support
  - Clean, professional HTML formatting with tables, lists, and styled sections
  - Window is resizable with configurable splitter between navigation and content
- **Updated Help Menu Action**:
  - Modified `MainWindow::on_actionAyuda_triggered()` to open UserManualDocumentation dialog instead of simple message box
  - Added include for usermanualdocumentation.h in mainwindow.cpp
- **Files Added**:
  - usermanualdocumentation.h: Header for user manual documentation dialog
  - usermanualdocumentation.cpp: Implementation with complete manual content in Spanish
- **Files Modified**:
  - mainwindow.cpp: Updated on_actionAyuda_triggered() to use new dialog, added include
  - marrta.pro: Added usermanualdocumentation.cpp to SOURCES and usermanualdocumentation.h to HEADERS

### PR: Add License and Disclaimer Dialogs with First-Run Acceptance
#### Changes
- **New Disclaimer Dialog**:
  - Created `DlgDisclaimer` dialog (dlgdisclaimer.h/cpp/ui) with full medical disclaimer text
  - Displays comprehensive warning that software is not a medical device
  - Includes sections on: limitations of use, software purpose, user responsibility, and liability limitations
  - Full text is translatable for internationalization support
- **New License Dialog**:
  - Created `DlgLicense` dialog (dlglicense.h/cpp/ui) with GPLv3 license summary
  - Explains key freedoms: use, study, distribution, and improvement
  - Describes copyleft requirements and source code obligations
  - Button to open full license text in user's configured language (ES, EN, FR, PT)
  - Links to official GNU GPL pages in multiple languages
- **First-Run Disclaimer Acceptance**:
  - Added `disclaimerAccepted` field to settings.h for tracking acceptance
  - Main.cpp now checks on first run and displays disclaimer dialog
  - Custom button labels: "Acepto" (I accept) and "No acepto" (I don't accept)
  - Application exits if user clicks "No acepto"
  - Acceptance is saved to settings file (marrta.ini) to prevent showing on subsequent runs
- **Updated Menu Actions**:
  - "Aviso legal" (Legal Notice) now opens full disclaimer dialog instead of simple message box
  - "Licencia de uso" (Usage License) now opens full license dialog with link to complete text
- **Settings Integration**:
  - Added disclaimerAccepted to loadSettings() and saveSettings() in MainWindow
  - Setting persists across application sessions
- **Files Added**:
  - dlgdisclaimer.h/cpp/ui: Disclaimer dialog with full medical disclaimer text
  - dlglicense.h/cpp/ui: License dialog with GPLv3 summary and multi-language links
- **Files Modified**:
  - settings.h: Added disclaimerAccepted boolean field
  - main.cpp: Added first-run check and disclaimer acceptance logic
  - mainwindow.h: Added includes for new dialogs
  - mainwindow.cpp: Updated action handlers to use new dialogs, added disclaimer setting to load/save
  - marrta.pro: Added new dialog files to SOURCES, HEADERS, and FORMS sections

### PR: Add GitHub Issue Submission Feature (Enviar comentario) - Updated
#### Latest Changes
- **New Help Menu Items**:
  - Added "Ayuda" menu item at the top of Help menu with separator
  - Added "Aviso legal" menu item below "Acerca de..." to show legal notice
  - Added "Licencia de uso" menu item below "Aviso legal" to show usage license
  - All new menu items have appropriate icons (help.svg, clipboard.svg, book.svg)
  - Placeholder dialogs implemented for each new menu item with links to GitHub repository
- **Files Modified**:
  - mainwindow.ui: Added three new actions (actionAyuda, actionAvisoLegal, actionLicenciaUso) to Help menu
  - mainwindow.h: Added slot declarations for the three new actions
  - mainwindow.cpp: Added implementations showing information dialogs

#### Recent Changes
- **Enhanced Privacy Warning**:
  - Updated confirmation dialog to explicitly warn users that information will be publicly visible on GitHub
  - Changed dialog title to "Información importante" with information icon
  - Uses separate setText and setInformativeText for better clarity
  - Default button set to "No" for safety
- **File Attachments Changed to Links**:
  - Replaced file upload functionality with link input system
  - Users now provide links to files hosted on external services (Google Drive, OneDrive, iCloud, Dropbox, etc.)
  - Added informative text explaining users must upload files to hosting services first
  - Changed "Archivos adjuntos" to "Enlaces a archivos adjuntos"
  - Replaced file selection dialogs with simple text input dialog for links
  - Links are displayed and included directly in the GitHub issue body
  - Removed unused file upload methods (onFileUploaded, uploadAttachments)
- **Files Modified**:
  - dlgsendfeedback.ui: Updated attachments section with new labels and single "Añadir enlace" button
  - dlgsendfeedback.h: Changed attachmentPaths to attachmentLinks, removed file upload methods
  - dlgsendfeedback.cpp: Replaced file dialogs with link input, updated confirmation dialog, simplified issue body generation

#### Original Changes
- **New Dialog for Feedback Submission**:
  - Created `DlgSendFeedback` dialog (dlgsendfeedback.h/cpp/ui) for sending comments/bug reports to GitHub
  - Form includes fields for: comment type (bug/comment/feature request), name, institution, contact email, description
  - Bug-specific fields (conditionally shown): steps to reproduce, expected result, actual result
  - Support for adding links to hosted files (images/screenshots and .pbd database files)
  - Disclaimer text about response time variability
- **Menu Integration**:
  - Added two new menu items in Help menu (menuAyuda) with separators:
    - "Enviar comentario": Opens feedback submission dialog
    - "Enviar modelo de riesgo": Placeholder for future implementation
  - Menu items include appropriate icons (upload.svg and share.svg)
- **GitHub API Integration**:
  - Implemented GitHub API calls to create issues in risk-marrta/MARRTA repository
  - Automatic issue formatting with sender information, description, and bug details
  - Automatic label assignment (bug, enhancement, question, user-submitted)
  - Display link to created issue with option to open in browser
  - Graceful handling of authentication errors with fallback to manual submission
- **Translation Support**:
  - All UI text uses `tr()` for Qt translation compatibility
  - Supports both Spanish (default) and English translations
- **Files Added**:
  - dlgsendfeedback.h: Dialog header with network manager and link handling
  - dlgsendfeedback.cpp: Implementation with form validation and GitHub API integration
  - dlgsendfeedback.ui: Qt Designer form with all feedback fields
- **Files Modified**:
  - mainwindow.ui: Added actionEnviarComentario and actionEnviarModeloRiesgo actions to Help menu
  - mainwindow.h: Added slot declarations for new menu actions
  - mainwindow.cpp: Added implementations and DlgSendFeedback include
  - marrta.pro: Added new dialog files to SOURCES, HEADERS, and FORMS sections
### PR: Fix Compilation Error in mainwindow.cpp
#### Changes
- **Bug Fix**:
  - Fixed compilation error C2065 at mainwindow.cpp:1207 where variable 'texto' was used before declaration
  - Removed orphaned line that attempted to assign to 'texto' before the variable was declared
  - The variable is now properly declared before use in the actualizarContadorEventos() function
- **Files Modified**:
  - mainwindow.cpp: Removed line 1207 that caused the compilation error

### PR: Report Configuration Dialog Improvements and Bug Fixes
#### Changes
- **Compilation Fix**:
  - Fixed compilation error where `def.name` was used instead of `def.nombre` for DefenseDefinition objects
  - DefenseDefinition inherits from OrderableDefinition which has `nombre` field, not `name`
- **Dialog Layout Redesign**:
  - Removed title label "Configuración del Informe" from dialog
  - Made window wider (850px) and shorter (550px) for better usability
  - Reorganized layout into two-column design:
    - Left column: Checklist of report sections
    - Right column: Stage tree, format options, and action buttons
- **Stage Selection Improvements**:
  - Moved stage tree to right side with new header "Etapas que abarcará el informe"
  - "TODAS LAS ETAPAS" now selected by default
  - Stage selection now properly filters all stage-related report sections (Secuencias, Listado completo, Listado de iniciadores, Listado de defensas)
  - Unified validation error messages for stage selection
- **Listado de Iniciadores Updates**:
  - Headers now display stage/substage names in uppercase on separate lines (ETAPA\nSUBETAPA)
  - Reduced font size to 10pt for better table layout
  - Code column width increased to 120px for better readability
- **Listado de Defensas Updates**:
  - Fixed defense grouping logic: now groups by stage (not substage) as defenses are stage-associated
  - Includes defenses with `etapa_orden_id < 0` (general defenses)
  - Headers now in uppercase with improved formatting
  - Added new column "INICIADORES (ACTIVOS)" showing total initiators and active count in format "N (M)"
  - Added explanatory text: "La última columna muestra los iniciadores a los que está asociada la defensa, y entre paréntesis en cuántos se ha considerado aplicable"
- **Defensas No Aplicables Updates**:
  - Replaced hardcoded "0" with actual initiator count in format "N (M)" where N is total and M is active
  - Added same explanatory text as defense list
  - Column renamed to "INICIADORES (ACTIVOS)" for consistency
- **Files Modified**:
  - dlgconfiginforme.ui: Complete layout redesign with two-column structure
  - dlgconfiginforme.cpp: Stage filtering logic, report generation updates, added QSqlQuery includes
  - Added necessary headers: QSqlQuery, QSqlError

### PR: Extract CSS and Embed Logo in HTML Reports
#### Changes
- **External CSS Management**:
  - Extracted CSS from embedded code to external file `resources/report_style.css` (~500 lines)
  - CSS now loaded from user's AppData directory if customized, otherwise uses default from resources
  - User CSS location: `~/.local/share/MARRTA/report_style.css` (Linux) or equivalent on Windows/Mac
  - Enables users to customize report styling without modifying application code
- **Logo Embedding**:
  - Logo now embedded in HTML as base64 data URI (`data:image/png;base64,...`)
  - Saved HTML reports display logo correctly in external browsers
  - No dependency on Qt resource system for saved HTML files
  - Logo automatically converted to base64 (~64KB) at report generation time
- **New UI Controls** in Report Configuration Dialog:
  - "Guardar CSS" button - Export current CSS to a file for editing
  - "Importar CSS" button - Import custom CSS file to personalize reports
  - "Restaurar por defecto" button - Reset to default CSS and remove customizations
  - New "Gestión de Estilos CSS" group box containing all CSS management buttons
- **CSS Features**:
  - CSS orientation (@page rule) automatically updated based on portrait/landscape selection
  - User customizations persist across application restarts
  - Safe reset mechanism with confirmation dialog
  - CSS syntax validation through Qt file operations
- **Files Added**:
  - resources/report_style.css: Default CSS file with complete report styling
  - Documentation/Support/CSS_MANAGEMENT_TESTING_GUIDE.md: Comprehensive testing guide
- **Files Modified**:
  - dlgconfiginforme.h: Added CSS management method declarations
  - dlgconfiginforme.cpp: Implemented CSS loading, saving, and logo base64 conversion
  - dlgconfiginforme.ui: Added CSS management button group
  - logos.qrc: Added report_style.css resource under /report prefix
- **Benefits**:
  - Users can customize report colors, fonts, spacing, and layout
  - Reports are portable and display correctly outside MARRTA
  - Easy to share custom CSS styles between users
  - No need to recompile application to change report styling

### PR: Add DevTools to Report Viewer
#### Changes
- **Added Developer Tools Support**:
  - Implemented DevTools for HTML report preview in dlgconfiginforme with two access methods:
    1. "Inspeccionar estilos" button (bottom-left of preview dialog) - explicitly opens DevTools
    2. Right-click → "Inspect Element" on report preview - also opens DevTools
  - Changed preview dialog from QTextBrowser to QWebEngineView to support DevTools
  - DevTools window opens in a separate dialog (1200x800) with full Chromium inspector
  - Users can inspect HTML/CSS, debug JavaScript, view console output, and monitor network requests
  - Lazy initialization: DevTools only created when user clicks button (no overhead if not used)
  - Fixed dialog hang issue: WebEngineView properly cleaned up after exec() returns
- **Removed Unused Code**:
  - Completely removed ReportViewer class and all files (reportviewer.h, reportviewer.cpp, reportviewer.ui)
  - Removed ReportViewer references from marrta.pro build file
  - Removed unused #include "reportviewer.h" from mainwindow.cpp
  - Removed unused #include <QTextBrowser> from dlgconfiginforme.cpp
  - ReportViewer was not used anywhere in the application
- **Menu Cleanup**:
  - Removed duplicate "Generar informe..." entry from Modelo menu
  - Toolbar icon correctly points to remaining "Generar informe" action
- **Files Modified**:
  - dlgconfiginforme.cpp: Modified on_btnVistaPrevia_clicked() to use QWebEngineView, add "Inspeccionar estilos" button, and properly manage DevTools lifecycle
  - mainwindow.cpp: Removed unused #include "reportviewer.h"
  - mainwindow.ui: Removed duplicate menu item
  - marrta.pro: Removed reportviewer.cpp, reportviewer.h, reportviewer.ui from build
- **Files Deleted**:
  - reportviewer.h, reportviewer.cpp, reportviewer.ui (unused files removed)
### PR: Fix "Guardar como..." (Save As) Functionality
#### Changes
- **Save As Implementation**:
  - Added `on_actionGuardar_como_triggered()` slot to handle "Guardar como..." menu action
  - Function now prompts user for a new file name and location with .pdb extension
  - Saves pending changes to current model before copying
  - Copies the current database file to the new location instead of overwriting
  - Uses native file dialog's overwrite confirmation (in system language)
  - Shows appropriate success/error messages to the user
  - Uses proper Qt translation (`tr()`) for all user-facing messages
- **Files Modified**:
  - mainwindow.h: Added `on_actionGuardar_como_triggered()` slot declaration
  - mainwindow.cpp: Implemented "Save As" functionality with file dialog and copy logic
### PR: Change Python scripting menu icon
#### Changes
- **Updated Python scripting menu icon**:
  - Created new `python.svg` icon in ikonate style matching the official Python logo structure
  - Added `python.svg` to `icons.qrc` resource file
  - Updated `mainwindow.ui` to use the new Python icon for the Python scripting menu action
  - The new icon features the recognizable interlocking snake head design from the official Python logo, adapted to ikonate's minimalist line-based style
  - JavaScript editor continues to use the `edit.svg` icon for differentiation

### PR: Add model name title to flowchart control panel
#### Changes
- **Updated `generar_flujograma.py` flowchart generator**:
  - Replaced the hardcoded "Filtros y herramientas" text in the control panel header with the actual model name from the database
  - The flowchart control panel now displays the model name (retrieved from `configuracion` table) instead of generic text
  - Added `[[MODEL_NAME]]` placeholder in HTML template (line 688)
  - Added model name replacement in template substitution section using the already-fetched `model_name` variable
  - Model name is properly escaped using the `esc()` function to prevent HTML injection

### PR: Fix DataFrame creation in flujograma script
#### Changes
- **Fixed `generar_flujograma_v25_desde_bd.py` script**:
  - Fixed DataFrame creation error where `Etapas.get_all()` returns custom `Etapa` objects instead of dictionaries
  - The script was creating a DataFrame with column "0" containing objects instead of proper column names (id, codigo, nombre, etc.)
  - **Added universal object-to-dict conversion for all API calls** (Etapas, Sucesos, Defensas)
  - Script now automatically detects if API returns objects or dictionaries and converts accordingly
  - Extracts all attributes from objects to create proper DataFrame columns
  - Fixed `KeyError: 'etapa_id'` by ensuring all object attributes are converted to dictionary keys
  - Fixed syntax errors: removed broken `safe_execute()` wrapper and empty `main_script()` function
  - Fixed indentation issues that prevented script from compiling
  - Removed debug code that was added for troubleshooting
  - Added robust error handling and output flushing for debugging
  - Script now correctly creates DataFrames with proper column names for all data sources
  - See `Documentation/Support/FLUJOGRAMA_FIX_SUMMARY.md` for detailed implementation notes

### PR: Enhanced Python API with Sucesos and Defensas modules + Flowchart Generator
#### Changes
- **Extended Python API**:
  - Added `Sucesos` module for accessing initiating events:
    - `Sucesos.get_all()`: Get all events with complete details (id, codigo, nombre, descripcion, frecuencia, consecuencia, riesgo, error_humano, activo, completado, stage/substage info)
    - `Sucesos.get_by_stage(stage_id, substage_id=0)`: Get events filtered by stage/substage
  - Added `Defensas` module for accessing defenses (barriers and reducers):
    - `Defensas.get_all(event_id=0)`: Get all defenses with optional event filter (id, codigo, nombre, descripcion, robustez, tipo, tipo_str, activo, suceso_id, stage info)
    - `Defensas.get_by_type(tipo, event_id=0)`: Get defenses by type (0=FreqReducer, 1=Barrier, 2=ConsReducer)
  - Both modules provide comprehensive model data access for scripting and analysis
  - **Fixed `Etapas.get_all()` to include all stages** (including fictitious stages with id <= 0)
  - **Fixed `Etapa.imprimir()` substage query** to use correct table name (`subetapas` with `etapa_id` column)
- **Latest Fixes**:
  - Fixed `NameError: name '__file__' is not defined` in MARRTA's embedded console (added fallback to `os.getcwd()`)
  - Fixed `SyntaxWarning: invalid escape sequence '\s'` in HTML template (using raw string r""")
  - Added validation for empty data with helpful error messages
  - Fixed `KeyError: 'id'` by validating data before DataFrame creation
- **Flowchart Generator Script**:
  - New `generar_flujograma.py` script generates interactive HTML flowcharts from loaded MARRTA model
  - New `generar_flujograma_v25_desde_bd.py` - exact replica of v25 flowchart reading from database (941 lines)
  - Visual representation of stages, substages, events, and defenses
  - Color-coded by risk level (gray=low, green=medium, orange=high, red=very high)
  - Shows defense types with icons (RF=FreqReducer, B=Barrier, RC=ConsReducer)
  - Configurable display options (show/hide inactive elements, defenses, etc.)
  - Self-contained HTML output that can be saved and shared
- **Test Scripts**:
  - `test_python_sucesos_api.py`: Tests Sucesos module with statistics and stage analysis
  - `test_python_defensas_api.py`: Tests Defensas module with type filtering and event integration
  - `test_python_etapas_api.py`: Tests Etapas module and Etapa.imprimir() method
- **Documentation**:
  - New `Documentation/Support/PYTHON_API_REFERENCE.md`: Complete Python API reference with examples
  - Covers all modules: Marrta, Database, Etapas, Sucesos, Defensas
  - Includes complete usage examples and best practices
- **Files Modified**:
  - `pythonscriptingengine.h`: Added registerSucesosObject() and registerDefensasObject() methods
  - `pythonscriptingengine.cpp`: Implemented Sucesos and Defensas C++/Python bindings with SQL queries
- **Files Added**:
  - `test_scripts/test_python_sucesos_api.py`: Sucesos module test script
  - `test_scripts/test_python_defensas_api.py`: Defensas module test script
  - `test_scripts/generar_flujograma.py`: HTML flowchart generator script
  - `Documentation/Support/PYTHON_API_REFERENCE.md`: Complete API reference documentation

### PR: Integrar motor de scripting en Python
#### Changes
- **New Python Scripting Engine**:
  - Integrated Python 3.12 scripting engine alongside existing JavaScript engine
  - Added `PythonScriptingEngine` class for executing Python code with access to MARRTA objects
  - Python engine exposes three main objects:
    - `Marrta`: Base object for accessing application features (get_version())
    - `Database`: Direct SQL query access via Database.query(sql)
    - `Etapas`: Stage management with Etapa.imprimir() method returning tuple (ID, código, nombre, descripción, (substage_ids))
  - **Optional compilation**: Python support is now optional via `PYTHON_ENABLED=1` flag
  - When disabled, Python menu still appears but shows helpful error message with build instructions
- **Python Editor Dialog**:
  - New `PythonScriptingEditor` dialog with same features as JavaScript editor
  - Python syntax highlighting with keywords, strings, comments, numbers, and built-in functions
  - Find/Replace functionality with case-sensitive and whole-word options
  - Save/Load Python scripts (.py files)
  - Brace matching for (), [], {}
  - Non-modal documentation window
- **Python Documentation Window**:
  - Comprehensive API documentation with examples
  - Interactive tree navigation for API exploration
  - Click-to-insert examples into editor
  - Detailed documentation for Marrta, Database, Etapas, and Etapa.imprimir()
- **Menu Integration**:
  - Added "Python" entry under Modelo/Scripting menu (alongside "Editor JS")
  - Menu action `actionEditorPython` opens Python scripting editor
- **Files Added**:
  - `python_highlighter.h/cpp`: Python syntax highlighter
  - `pythonscriptingengine.h/cpp`: Python interpreter integration with conditional compilation
  - `pythonscriptingeditor.h/cpp/ui`: Python editor dialog
  - `pythonscriptingdocumentation.h/cpp`: Python API documentation window
- **Build Configuration** (Optional):
  - Python support is **optional** - application compiles without Python installed
  - To enable: `qmake "PYTHON_ENABLED=1" "PYTHON_INCLUDE=<path>" "PYTHON_LIB=<path>"`
  - Unix/Linux example: `qmake "PYTHON_ENABLED=1"`
  - Windows example: `qmake "PYTHON_ENABLED=1" "PYTHON_INCLUDE=C:/Python312/include" "PYTHON_LIB=C:/Python312/libs"`
  - When disabled, stub implementations provide helpful error messages
  - **Windows/MSVC**: Fixed Qt `slots` macro conflict with Python's `object.h` by carefully ordering includes
- **Files Modified**:
  - `mainwindow.h/cpp`: Added Python engine initialization and menu handler
  - `mainwindow.ui`: Added Python menu entry and action definition
  - `marrta.pro`: Added Python source files and optional Python library dependencies
  - `pythonscriptingengine.h/cpp`: Added conditional compilation with `#ifdef PYTHON_ENABLED`
### PR: Fix Mark/Unmark Defense Operations
#### Changes
- **Bug Fix - Defense Marking/Unmarking**:
  - Fixed mark/unmark operations to no longer be a toggle operation
  - "Marcar" (Mark) now explicitly sets active=true/1
  - "Desmarcar" (Unmark) now explicitly sets active=false/0
  - Operations now save directly to database AND update in-memory state
  - Follow the same pattern as iniciadores (events) marking:
    - Use database transaction for batch updates
    - Commit all changes to database first
    - Only update in-memory state after successful commit
    - Rollback on error with proper user notification
  - Call `refreshMainScreen()` after successful operation to refresh the UI
  
- **Bug Fix - Risk Color Synchronization**:
  - Fixed synchronization of F, P, C and risk colors between panel and initiator list
  - After marking/unmarking defenses, the risk is now recalculated for all affected events
  - Risk values are updated in both in-memory data and database
  - This ensures the colored risk indicator in the initiator list matches the panel display
  
- **Files Modified**:
  - dbdefense.h: Added `setActive(int defenseId, bool active)` static method declaration
  - dbdefense.cpp: Implemented `setActive()` method to update defense active state in database
  - dbevent.h: Added `updateRisk(int eventId, int riesgo)` static method declaration
  - dbevent.cpp: Implemented `updateRisk()` method to update event risk in database
  - mainwindow.cpp:
    - Modified `markAllDefenses(bool marcar)` to use database transactions and recalculate risks
    - Modified `markAllDefenses(TipoDefensa tipo, bool marcar)` to use database transactions and recalculate risks
    - Updated all action handlers (on_actionMarcar*/on_actionDesmarcar*) to:
      - Check for database availability
      - Use try-catch for error handling
      - Call `refreshMainScreen()` after successful operation

### PR: Fix Event List Filters and Add Event Counter
#### Changes
- **Bug Fix - Filter Behavior**:
  - Fixed filter behavior to respect stage selection
  - Previously: when any filter was active, the application ignored stage selection and showed all matching events from all stages
  - Now: filters apply only to events within the selected stage/substage
  - Stage selection now works correctly even when filters are active
  
- **Enhancement - Event Counter**:
  - Added [n/m] event counter to stage title (similar to existing defense counters)
  - Counter displays: [applicable events / total events shown]
  - Applicable events = events with completado=1
  - Counter uses simple text format appended to stage name (more visible than right-aligned)
  - Counter now appears in "All Stages" mode as well
  - Counter updates automatically when:
    - Stage/substage selection changes
    - Any filter is applied or modified
    - Events are toggled between applicable/not applicable (via double-click, context menu, or mark all actions)
  
- **Enhancement - Mark/Unmark Actions**:
  - Updated menu texts to clarify that actions only affect visible (filtered) events:
    - "Marcar todas las defensas" → "Marcar todas las defensas en iniciadores visibles"
    - "Desmarcar todas las defensas" → "Desmarcar todas las defensas en iniciadores visibles"
    - "Marcar iniciadores aplicables" → "Marcar iniciadores visibles como aplicables"
    - "Marcar iniciadores NO aplicables" → "Marcar iniciadores visibles como NO aplicables"
  - Updated behavior to only affect events currently shown in the event list (respecting filters and stage selection)
  - This prevents accidentally modifying filtered-out events
  
- **Files Modified**:
  - mainwindow.h: Added `actualizarContadorEventos()` method declaration
  - mainwindow.cpp: 
    - Modified `mostrarEventosSeleccion()` to apply filters within selected stage
    - Implemented `actualizarContadorEventos()` to count and display events
    - Updated `markAllDefenses()` and `markAllInitiators()` to only affect visible events
    - Updated all filter change handlers and event toggle methods to update counter
  - mainwindow.ui: Updated menu action texts to reflect new behavior

### PR: Fix Compilation Error - Missing Method Declaration
#### Changes
- **Bug Fix**:
  - Added missing declaration for `splitDefenseDisplayText()` method in defenseitemdelegate.h
  - The method was implemented in defenseitemdelegate.cpp but not declared in the header file, causing compilation error C2039
  - Declared the method as private helper method with signature: `QPair<QString, QString> splitDefenseDisplayText(const QString &fullText) const`
- **Files Modified**:
  - defenseitemdelegate.h: Added missing private method declaration

### PR: Extract Duplicated Defense Name Extraction Logic
#### Changes
- **Code Refactoring**:
  - Extracted duplicated name extraction logic into a reusable private helper method `extractDefenseNameFromDisplay()`
  - The helper method extracts defense names from the display format `[code] Name [Hardness]` by removing the code prefix and hardiness suffix
  - Replaced 10 duplicate instances of the extraction logic with calls to the helper method:
    - 3 instances in dataChanged handlers (reductoresFrecuencia, barreras, reductoresConsecuencias)
    - 3 instances in double-click handlers (on_lReductoresFrecuencia_doubleClicked, on_lBarreras_doubleClicked, on_lReductoresConsecuencia_doubleClicked)
    - 3 instances in custom context menu handlers (on_lReductoresFrecuencia_customContextMenuRequested, on_lBarreras_customContextMenuRequested, on_lReductoresConsecuencia_customContextMenuRequested)
    - 1 instance in alternarDefensaModelo method
  - Improves code maintainability by centralizing the extraction logic in a single location
  - No functional changes - purely refactoring to reduce code duplication
- **Files Modified**:
  - mainwindow.h: Added `extractDefenseNameFromDisplay()` method declaration
  - mainwindow.cpp: Implemented helper method and replaced all duplicated instances

### PR: Add Defense Codes and Hardiness to Defense Lists Display
#### Changes
- **Defense Lists Display Enhancement**:
  - Modified defense list display format to show code and name on the left, hardiness properly right-aligned: `[code] Name       [Hardiness]`
  - Example: `[FMB-001] Sistema de detección de incendios                                      [Robusta]`
  - Implemented custom item delegate (DefenseItemDelegate) to ensure proper right-alignment of hardiness regardless of font or text length
  - Applied to all three defense types:
    - Reductores de Frecuencia (Frequency Reducers)
    - Barreras (Barriers)
    - Reductores de Consecuencia (Consequence Reducers)
  - Hardiness levels displayed in Spanish, right-aligned in brackets:
    - "[Muy blanda]" (Very soft)
    - "[Blanda]" (Soft)
    - "[Normal]" (Normal)
    - "[Robusta]" (Robust)
    - "[Muy robusta]" (Very robust)
  - Updated all interaction handlers to properly extract name from display format:
    - Checkbox state change handlers in `mostrarReductoresFrecuencia()`, `mostrarBarreras()`, `mostrarReductoresConsecuencia()`
    - Double-click handlers for showing defense information
    - Context menu handlers for defense operations
    - `alternarDefensaModelo()` method for marking/unmarking defenses across the model
  - Users can now quickly identify defenses by their code and see their hardiness level at a glance without opening the details dialog
- **Files Modified**:
  - mainwindow.cpp: Updated defense display methods and all interaction handlers, added delegate setup
  - defenseitemdelegate.h/cpp: New custom delegate for proper right-aligned rendering
  - marrta.pro: Added new delegate files

### PR: Use Windows Vista Style for Proper Blue Selection Colors
#### Changes
- **Windows Vista Style on Windows**:
  - Explicitly set Windows Vista style on Windows platforms using `#ifdef Q_OS_WIN`
  - Ensures proper blue selection colors on Windows (not green/teal)
  - Other platforms (macOS, Linux) use their native default styles
  - Maintains soft alternating row color (#f5f5f5) for better readability
- **Files Modified**:
  - main.cpp: Added conditional Windows Vista style selection for Windows platform

### PR: Use Native Platform Colors and Fix Risk Recalculation in Context Menu Operations
#### Changes
- **Native Platform Colors**:
  - Disabled Fusion style to use native platform theme (Windows, macOS, Linux)
  - Selection and hover colors now match the operating system's native theme
  - On Windows: Blue selection colors
  - On macOS: Blue/purple selection colors  
  - On Linux: Depends on desktop environment theme
  - Soft alternating row color (#f5f5f5) maintained for better readability
- **Fixed Risk Recalculation for Context Menu Operations**:
  - Context menu operations now properly recalculate risk before refreshing event list
  - Added `evento.calcularReductores()` and `evento.calcularRiesgo()` calls in:
    - `alternarDefensasEventoActual()` - mark/unmark all defenses of a type
    - `alternarDefensaModelo()` - mark/unmark a defense across entire model
  - Risk color indicators in event list now update correctly for all operations
  - Previously only checkbox clicks recalculated risk; context menu operations now work correctly
- **Files Modified**:
  - main.cpp: Disabled Fusion style (commented out) to use native platform styling
  - mainwindow.cpp: Added risk recalculation calls in context menu handler methods

### PR: Use System Colors with Softer Alternating Rows and Fix Context Menu Event Updates
#### Changes
- **System Color Scheme**:
  - Removed custom blue colors for selection and hover
  - Now using system default colors for better consistency across platforms
  - Selection and hover colors match the operating system theme
  - Softer alternating row color: light gray (#f5f5f5) instead of system default
  - Applied to all list views: stages, events, and all defense lists
- **Checkbox Appearance**:
  - Using system default checkbox styling
  - Checkboxes now properly aligned without spacing issues on hover
- **Fixed Context Menu Event List Updates**:
  - Event list now updates when using right-click context menu options
  - "Marcar todas" (mark all) now triggers event list refresh
  - "Desmarcar todas" (unmark all) now triggers event list refresh
  - "Marcar en todo el modelo" (mark in entire model) now triggers event list refresh
  - "Desmarcar en todo el modelo" (unmark in entire model) now triggers event list refresh
  - Added `refrescarListaEventos()` calls to `alternarDefensasEventoActual()` and `alternarDefensaModelo()`
- **Files Modified**:
  - mainwindow.cpp: Simplified stylesheet to use system colors with soft alternating rows, added refrescarListaEventos() calls to context menu handlers

### PR: Improve List View Styling and Real-Time Event List Updates
#### Changes
- **Consistent Blue Color Scheme**:
  - Selection color: Bright blue (#0077ff) matching application theme
  - Hover color: Very light blue (#cce5ff) for better visibility
  - Border colors: Blue (#0077ff) on hover/checked states for consistency
- **Improved Checkbox Visibility**:
  - Added black checkmark icon (check-black.svg) for better contrast
  - Checkboxes now have white background with visible black tick
  - Border radius added for modern appearance
  - Increased size from 13px to 16px for better visibility
- **Real-Time Event List Updates**:
  - Event list now refreshes immediately when defense is toggled
  - Risk color indicators update in real-time without requiring save
  - Current selection is preserved during refresh
  - New method `refrescarListaEventos()` handles refresh with selection preservation
- **Files Modified**:
  - mainwindow.cpp: Updated QListView stylesheet, added refrescarListaEventos() method, called in all defense dataChanged handlers
  - mainwindow.h: Added refrescarListaEventos() method declaration
  - icons.qrc: Added check-black.svg resource
  - resources/icons/check-black.svg: New black checkmark icon (created)

### PR: Restore Original Light Blue List View Colors
#### Changes
- **Qt Fusion Style Enabled**:
  - Uncommented `QApplication::setStyle(QStyleFactory::create("Fusion"))` in main.cpp
  - Ensures consistent cross-platform appearance
- **Custom QListView Stylesheet**:
  - Applied custom stylesheet to defense lists and event list
  - Selection color: Light blue (#4A90D9) instead of dark teal/green
  - Hover color: Very light blue (#A0C8E8) for non-selected items
  - Checkbox indicators: White background with gray border (no colored fill)
  - Checkmark icon displayed using existing SVG resource
- **Affected Widgets**:
  - lReductoresFrecuencia (Frequency Reducers list)
  - lBarreras (Barriers list)
  - lReductoresConsecuencia (Consequence Reducers list)
  - eventos (Events list)
- **Files Modified**:
  - main.cpp: Enabled Fusion style
  - mainwindow.cpp: Added QListView custom stylesheet in constructor

### PR: Display Defense Count [n/m] for Each Defense Type
#### Changes
- **Defense Counter Display**:
  - Added defense counter labels displaying [n/m] format next to each defense type header
  - "n" represents the number of active/selected defenses of that type
  - "m" represents the total number of defenses of that type
  - Counters appear to the right of the defense type headers: "Reductores de Frecuencia", "Barreras", and "Reductores de Consecuencias"
  - Counters update in real-time when defense active state is toggled
  - Counters are cleared when no event is selected
- **Real-Time Updates**:
  - Counters automatically update when switching between events
  - Counters automatically update when toggling defense checkboxes
  - Counters reset to [0/0] when details are cleared
- **Files Modified**:
  - mainwindow.ui: Added three new QLabel widgets (lblCountReductoresFrecuencia, lblCountBarreras, lblCountReductoresConsecuencia) with horizontal layouts for each defense type header
  - mainwindow.h: Added actualizarContadoresDefensas() method declaration
  - mainwindow.cpp: Implemented actualizarContadoresDefensas() method and integrated it into event display and defense state change handlers
### PR: Fix Defense Marking/Unmarking with Submenus by Defense Type
#### Changes
- **Fixed Defense Marking Logic**:
  - Fixed bug where marking/unmarking defenses would invert their states instead of setting them to the desired state
  - Old implementation only changed defenses that were NOT already in the target state
  - New implementation properly sets all defenses to the target state regardless of current state
- **New Submenu Structure**:
  - Converted "Marcar todas las defensas" to submenu with 4 options:
    - Marcar todas las defensas
    - Marcar todos los reductores de frecuencia
    - Marcar todas las barreras
    - Marcar todos los reductores de consecuencia
  - Converted "Desmarcar todas las defensas" to submenu with 4 options:
    - Desmarcar todas las defensas
    - Desmarcar todos los reductores de frecuencia
    - Desmarcar todas las barreras
    - Desmarcar todos los reductores de consecuencia
  - Each submenu has separator after the first "all defenses" option
- **Helper Function Pattern**:
  - Created `markAllDefenses(int tipoDefensa, bool marcar)` helper function
  - Follows same pattern as `markAllInitiators()` from PR #181
  - Avoids code duplication across 8 slot functions
  - Uses -1 for all defense types, 0 for frequency reducers, 1 for barriers, 2 for consequence reducers
- **Files Modified**:
  - mainwindow.h: Added 8 new slot declarations and helper function declaration
  - mainwindow.cpp: Implemented helper function and 8 slot functions, removed old functions
  - mainwindow.ui: Converted menu items to submenus, added 8 new actions with icons

### PR: Add Mark/Unmark All Initiators Options and Update Defense Window Label
#### Changes
- **New Menu Options for Marking Initiators**:
  - Added "Marcar todos los iniciadores como aplicables" menu option in Modelo menu
  - Added "Marcar todos los iniciadores como NO aplicables" menu option in Modelo menu
  - Both options appear after the existing defense marking options, separated by a separator
  - These options mark/unmark the "completado" field for all initiators in the model
  - The main window refreshes after marking/unmarking without changing the selected stage and initiator focus
- **Defense Window Label Update**:
  - Updated label in defense list window from "Doble clic para ver descripción" to "Doble click para editar nombre y descripción"
  - More accurately describes the action (editing) when double-clicking on a defense
- **Files Modified**:
  - mainwindow.ui: Added two new menu actions with icons and separators
  - mainwindow.h: Added slot declarations for the new menu actions
  - mainwindow.cpp: Implemented the slot functions to update all initiators and refresh the display
  - listabarreras.ui: Updated the informational label text
### PR: Replace Slider with Right Mouse Button to Move Fictional Stages in Flujograma
#### Changes
- **Right Mouse Button Moves Fictional Stages**:
  - Replaced the vertical slider control with right mouse button drag to move fictional stages (etapas ficticias) vertically
  - Right mouse button now moves ONLY the fictional stages group while keeping arrows pointing to their original targets
  - Fictional edges (arrows) automatically update their paths to maintain connections as fictional stages move
  - Left mouse button continues to provide full directional panning of the viewport (both horizontal and vertical)
  - Context menu disabled on the SVG canvas to prevent interference with right-click functionality
- **Simplified User Interface**:
  - Removed the "Posición vertical etapas ficticias" slider card from the control panel
  - Cleaner interface with one less control element
  - Removed associated CSS styles for slider components (slider-container, input[type="range"])
- **Updated Instructions**:
  - Control panel instructions now clearly indicate: "Mover etapas ficticias: Botón derecho + Arrastrar"
  - Users have clear guidance that right button moves fictional stages, not the entire viewport
- **Technical Implementation**:
  - Added `fictOffset` variable to track fictional stages vertical offset independently from viewport position
  - Implemented `updateFictionalOffset()` function that updates both the fictional group transform and fictional edges
  - Fictional edges parse their path data and update the Y1 coordinate while keeping Y2 (target) constant
  - Reset button now also resets fictional offset to 0
- **Modified Files**:
  - flujogramagenerator.cpp: Removed slider HTML generation, restored and adapted original fictional stages offset logic for right mouse button, updated CSS to remove slider styles, updated control instructions text

### PR: Flowchart Fictional Stage Headers and Filtering Improvements
#### Changes
- **Conditional Display of Fictional Stage Headers**:
  - Headers for support stages ("Etapas de apoyo", "Barreras", "Reductores Frecuencia", "Reductores Consecuencias") now only appear when there are fictional stages (etapas de apoyo) present in the model
  - This provides a cleaner interface when no fictional stages exist
  - Headers are conditionally generated based on whether `etapasFicticias` list is empty
- **Unified Header Styling**:
  - All column headers now use uppercase text for consistency
  - "Etapas ficticias" renamed to "ETAPAS DE APOYO" for clarity
  - All headers use the same font size (22px) with normal font weight (400)
  - Provides consistent, professional appearance across all flowchart columns
- **Improved Fictional Stage Filtering Behavior**:
  - When filtering by a specific stage in the "Etapa de sucesos" dropdown, fictional stages (Etapa Ficticia) are no longer hidden
  - Fictional stages now remain visible regardless of stage filter selection
  - This allows better context when examining specific stages while maintaining visibility of support stages
  - Only real stages (Etapa) and substages (Subetapa) are affected by the stage filter
- **Modified Files**:
  - flujogramagenerator.cpp: Updated header generation logic, unified CSS styling, modified JavaScript filtering logic

### PR: Add Vertical Slider for Fictional Stage Positioning
#### Changes
- **Fictional Stage Vertical Position Control**:
  - Added a vertical slider control in the flowchart panel to adjust the Y position of fictional stages (etapas ficticias)
  - **Slider only appears when there are fictional stages present** in the model and real stages exist
  - Slider allows moving fictional stages and their associated defenses from y=0 to the maximum Y position of real stages
  - This solves the problem of fictional stages overlapping or being hard to position in large models
  - The slider updates the position in real-time as it is moved
  - Position resets to 0 when "Reiniciar" button is clicked
- **Arrow Redrawing Fix**:
  - Fictional stage arrows (edges) are now correctly redrawn when the slider moves
  - Arrows maintain connection between moving fictional defenses and static initiators
  - Only the source Y coordinate (from defense) is updated; target Y (initiator) remains static
  - Arrows are positioned outside the `fictional-group` to prevent incorrect translation
- **Technical Implementation**:
  - Fictional stages and their defenses (barriers, RF, RC) are now grouped in a dedicated SVG group (`fictional-group`)
  - Fictional edges are placed outside the group and updated dynamically via JavaScript
  - JavaScript applies SVG transform to the group based on slider value
  - JavaScript redraws arrow paths by parsing and updating Y1 coordinates while preserving Y2
  - Maximum slider value is calculated from the real stages' maximum Y position
  - Slider has 10px step increments for smooth positioning
  - Slider card is conditionally generated only when needed to avoid cluttering the UI
- **Modified Files**:
  - flujogramagenerator.cpp: Separated fictional elements into dedicated groups, moved fictional edges outside the group, added conditional slider UI control, added JavaScript to handle slider events, apply offset transform, and redraw arrows dynamically
### PR: Prevent Text Selection During Flowchart Panning
#### Changes
- **User Experience Improvement**:
  - Fixed issue where dragging the mouse to pan the flowchart would select text in the boxes, making reading difficult
  - Added CSS `user-select: none` property to SVG element to prevent text selection during mouse drag operations
  - Solution is cross-browser compatible with vendor prefixes for WebKit, Mozilla, and Microsoft browsers
  - All other functionality remains intact: panning, zooming, clicking, tooltips, and interactions
- **Modified Files**:
  - flujogramagenerator.cpp: Added user-select CSS properties to prevent text selection on SVG element

### PR: Add Risk Color Coding and Filtering to Flowchart
#### Changes
- **Risk Color Coding for Initiator Nodes**:
  - Initiator nodes (sucesos iniciadores) in the flowchart are now color-coded based on their calculated risk level
  - Risk colors are slightly brighter than robustness colors for visual distinction:
    - RB (Bajo/Low): Light gray (#e0e0e0)
    - RM (Medio/Medium): Light green (#d4f1d4)
    - RA (Alto/High): Light orange (#ffe4b3)
    - RMA (Muy Alto/Very High): Light red (#ffd6d6)
- **Risk Level Filtering**:
  - Added new "Riesgo (sucesos iniciadores)" filter section in control panel
  - 4 checkboxes to filter initiator events by risk level: RB, RM, RA, RMA
  - All risk levels are checked (visible) by default
  - Filtering updates in real-time as checkboxes are toggled
- **Risk Information Display**:
  - Risk level now displayed in node tooltips (e.g., "Riesgo: Alto")
  - Risk data stored in data-riesgo attribute on iniciador nodes
- **Modified Files**:
  - flujogramagenerator.cpp: Added risk data attributes to iniciador nodes, CSS styling for risk colors, risk filter UI, and JavaScript filtering logic

### PR: Replace Flowchart Control Panel Title with Model Name
#### Changes
- **Flowchart Display Enhancement**:
  - Replaced hardcoded "Filtros y herramientas" text in the flowchart control panel with the actual model name from the database
  - Model name is retrieved from the `configuracion` table using `Database::DbManager::checkDatabase()`
  - Panel title now displays the model name, making it immediately clear which model the flowchart represents
  - Uses HTML escaping for safe rendering of model names with special characters
  - Falls back to "Modelo" if model name is empty or cannot be retrieved
- **Modified Files**:
  - flujogramagenerator.h: Updated `generarHTMLCompleto` signature to accept model name parameter
  - flujogramagenerator.cpp: Added model name retrieval, updated function calls, replaced hardcoded panel title with dynamic model name

### PR: Add Support for Robustez -1 in Flowchart Display
#### Changes
- **Flowchart Display**:
  - Added support for robustez -1 (muy blanda/very soft) defenses in flowchart visualization
  - Elements with robustez -1 are now visible with pale gray color (#f3f4f6)
  - Added robustez -1 filter checkbox in control panel (5 robustez levels: -1, 0, 1, 2, 3)
  - Updated tooltip display to show "muy blanda" for robustez -1
  - Updated normalization function to handle -1 values and map "muy blanda" text
- **Files Modified**:
  - flujogramagenerator.cpp: CSS styling, filter panel, tooltip, and normalization logic

### PR: Fix Flowchart Generation - Style and Localization Improvements
#### Changes
- **CSS Style Updates**:
  - Changed title text size from 15px to 16px for better readability
  - Changed title font-weight from 700 to 600 for a lighter appearance
  - Updated arrow styles: stroke-width from 1.5 to 1, opacity from .7 to .5 for all arrows
  - Adjusted title vertical position: y from 18 to 12
  - Adjusted subtitle spacing: dy from 18 to 24
  - Reduced panel handle font-weight from 700 to 525 (25% lighter)
  - Reduced panel handle height by 25%: padding from 10px to 7.5px
- **UI Improvements**:
  - Removed SVG and PNG export buttons
  - Added inline instructions for zoom/pan controls: "Zoom: Rueda del ratón | Pan: Arrastrar | Pan rápido: Shift + Arrastrar"
  - Simplified "Filtros Rápidos" section: renamed to "Búsqueda", removed "Subetapas" and "Iniciadores" checkboxes, removed label text
  - Substages and initiators are now always visible (no toggle option)
  - Moved "Solo defensas que actúan sobre sucesos visibles" checkbox to "Mostrar" section for better organization
- **Localization Fixes**:
  - Fixed confirmation dialog to use translatable Spanish buttons ("Sí"/"No") instead of hardcoded "Yes"/"No"
  - Ensures proper internationalization support for the dialog buttons
- **Filename Generation Improvements**:
  - Updated default filename pattern to "flujograma-{model_name}-{YY-MM-DD}.html"
  - Uses actual model name directly from database configuration table (no fallback to "modelo")
  - Retrieves model name from Database::ConfiguracionModelo
  - Sanitizes model name for use in filenames (removes invalid characters)
  - Uses current date in YY-MM-DD format
- **Modified Files**:
  - flujogramagenerator.cpp: Updated CSS template, SVG text positioning, panel styling, removed export functionality, added control instructions
  - mainwindow.cpp: Fixed button translations, improved filename generation to use actual model name, added QRegularExpression include

### PR: Add Flowchart Generator to Model Menu
#### Changes
- **New Feature: Vertical Flowchart Generator**:
  - Added menu option "Generar flujograma..." under "Modelo" menu
  - Generates an HTML vertical flowchart visualization of the risk model from the database
  - Shows stages, substages, and events with their risk levels (RB, RM, RA, RMA)
  - Interactive HTML with collapsible stages and color-coded risk levels
  - Includes CSS styling and JavaScript for interactivity
  - Opens generated flowchart in default browser after creation
- **New Files**:
  - flujogramagenerator.h: Header file for flowchart generator class
  - flujogramagenerator.cpp: Implementation of flowchart generation from database
- **Modified Files**:
  - mainwindow.ui: Added actionGenerarFlujograma action to Modelo menu
  - mainwindow.h: Added on_actionGenerarFlujograma_triggered() slot declaration
  - mainwindow.cpp: Implemented flowchart generation handler with file save dialog
  - marrta.pro: Added flujogramagenerator.h/cpp to build configuration
- **User Interface**:
  - New menu item appears after "Generar informe..." in Modelo menu
  - Uses list-alt icon for the menu action
  - Validates that a model is loaded before generating flowchart
  - Prompts user to save HTML file and optionally open it in browser

### PR: Fix MSVC Build Errors - Missing Files and Linker Flag Issue
#### Changes
- **MSVC Linker Compatibility**:
  - Fixed Unix-specific linker flag `-Wl,-rpath` that was causing MSVC LNK4044 warning
  - Added `unix:` prefix to `QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"` (line 32 in marrta.pro)
  - This flag is now only applied on Unix/Linux systems, not on Windows MSVC builds
- **Missing Source Files Added to Build**:
  - Added `dlgconfiginforme.cpp`, `dlgconfiginforme.h`, `dlgconfiginforme.ui` to build configuration
  - Added `informegenerator.cpp` and `informegenerator.h` to build configuration
  - Added `switch.cpp` and `switch.h` to build configuration
  - Added `style.h` to build configuration
  - These files existed in the repository but were not included in SOURCES/HEADERS/FORMS sections
- **Qt 6.10 API Compatibility Fixes in switch.cpp/h**:
  - Fixed `enterEvent()` signature: changed from `QEvent*` to `QEnterEvent*` for Qt 6+ compatibility
  - Fixed deprecated `QFontMetrics::width()`: replaced with `horizontalAdvance()` for Qt 6 compatibility
  - Resolves MSVC errors C3668, C2664, and C2039
- **Resolved Linker Errors**:
  - Fixed LNK2019 error for missing `DlgConfigInforme` symbols by including dlgconfiginforme.cpp
  - All source, header, and UI files now properly included in marrta.pro
- **Files Modified**:
  - marrta.pro: Fixed linker flags and added missing files to SOURCES, HEADERS, and FORMS sections
  - switch.h: Updated enterEvent() signature for Qt 6 compatibility
  - switch.cpp: Updated enterEvent() implementation and replaced deprecated width() with horizontalAdvance()

### PR: Fix MSVC compilation error with QWebEnginePage::print in Qt 6.10
#### Changes
- **Qt 6.10 API Compatibility**:
  - Fixed compilation error: `QWebEnginePage::printFinished` signal removed in Qt 6.10
  - Changed from `QWebEnginePage::printFinished` to `QWebEngineView::printFinished` signal
  - Used string-based SIGNAL/SLOT connection for MSVC compatibility with Qt 6.10
  - Added `handlePrintFinished()` slot to ReportViewer class for print completion handling
  - Fixed string literal error: changed single quotes to double quotes in `tr()` call
  - Fixed missing include: added `#include <QCoreApplication>` in informegenerator.cpp for `QCoreApplication::translate()` calls
  - Fixed slot name mismatch in dlgconfiginforme.cpp: renamed `on_checkModelInfo_stateChanged` to `on_checkInfoModelo_stateChanged`
  - Maintains backward compatibility with existing print functionality
- **Files Modified**:
  - reportviewer.cpp: Updated `on_btPrint_clicked()` method to use Qt 6.10 API with string-based connection
  - reportviewer.h: Added `handlePrintFinished(bool)` slot declaration
  - informegenerator.cpp: Added missing QCoreApplication include for MSVC compatibility
  - dlgconfiginforme.cpp: Fixed slot name to match header declaration

### PR: Remove LimeReport dependency
#### Changes
- **LimeReport Removal**:
  - Removed all LimeReport library dependencies from the project
  - Deleted LimeReport directory and all associated files
  - Removed LimeReport menu actions: "Lanzar informe..." and "Editar informe..."
  - Removed LimeReport functions: `on_actionLanzar_informe_triggered()` and `on_actionEditar_informe_triggered()`
  - Cleaned up includes: removed `#include "LimeReport/debug/include/lrreportengine.h"`
  - Updated marrta.pro: removed library linking (`-llimereportd`), include paths, and dependency paths
  - The built-in HTML report generator (Generar informe...) remains as the primary reporting system

### PR: Migrate Report Rendering from QTextEdit to QtWebEngine
#### Changes
- **Rendering Engine Migration**:
  - Changed report viewer from QTextEdit/QTextBrowser to QtWebEngine
  - Enables better HTML/CSS rendering with modern web standards support
  - Provides access to Developer Tools for debugging CSS and HTML
  - Improved rendering quality for complex HTML reports
- **Updated Printing and Export**:
  - Print functionality now uses QtWebEngine's native print method
  - PDF export uses QtWebEngine's printToPdf() for better quality
  - ODT export maintained by recreating temporary QTextDocument
- **Code Changes**:
  - Updated marrta.pro to include webenginewidgets module
  - Modified reportviewer.ui to use QWebEngineView widget
  - Refactored reportviewer.cpp to generate HTML and load in WebEngine
  - Enabled JavaScript and configured security settings for WebEngine
  - Commented out legacy printing methods (paintPage, printDocument, mmToPixels)
  - Added Qt 5/6 compatibility for QtWebEngine headers
- **Requirements**:
  - **Qt 5**: Requires qtwebengine5-dev and libqt5webenginewidgets5 packages
  - **Qt 6**: Requires Qt WebEngine module (may need separate installation on Windows)
  - **Windows**: Qt WebEngine may require separate download from Qt installer
  - **Linux**: Install via package manager (qtwebengine5-dev for Qt 5, qt6-webengine-dev for Qt 6)

### PR: Add frequency and consequence distribution charts
#### Changes
- **New Chart Visualizations**:
  - Added new tab "Distribución por frecuencia" with frequency distribution chart
  - Added new tab "Distribución por consecuencia" with consequence distribution chart
  - Both charts show overlay of base values vs calculated values (after applying defenses)
  - Frequency chart displays: Muy Baja, Baja, Media, Alta categories
  - Consequence chart displays: Bajas, Medias, Altas, Muy Altas categories
  - Charts use different colors to differentiate base and calculated values
  - Added legend to charts for clarity
- **PNG Export Functionality**:
  - Added "Exportar como PNG" buttons to all three chart tabs
  - High-resolution PNG export (1200x800) for report quality
  - Export to frequency distribution: `distribucion_frecuencia.png`
  - Export to consequence distribution: `distribucion_consecuencia.png`
  - Export to risk by stages: `distribucion_riesgo_etapas.png`
  - User can choose custom filename and location via file dialog
- **Translation Improvements**:
  - All chart titles and labels now use tr() for proper internationalization
  - Button text "Ver renderizado" and "Ver Markdown" now use tr()
  - Export dialog messages use tr() for internationalization
- **UI Enhancements**:
  - Added frequencyChartsWidget and consequenceChartsWidget to resumen.ui
  - Improved tab organization in summary dialog
  - Added export buttons with proper spacing at bottom of each chart tab
### PR: Update CSS to Qt 6.8.0 optimized version for MARRTA reports
#### Changes
- **Qt 6.8.0 CSS Optimization**:
  - Updated CSS to Qt 6.8.0 optimized version with explicit display properties
  - Changed font-family from quoted strings to unquoted (e.g., `Arial, Helvetica` instead of `'Arial', 'Helvetica'`)
  - Added explicit color values using hex notation (#ffffff instead of white)
  - Added `display: block` and `display: table` properties for better Qt compatibility
  - Updated `.initiator-type-bar` to use `display: table` with `table-cell` children instead of floats
  - Added explicit `width: 100%` to block-level elements for consistent rendering
  - Enhanced `.defense-item` with clearfix pattern and explicit layout structure
  - Changed `.risk-profile` from float-based to `display: table` layout
  - Updated responsive design to use `display: block` instead of `float: none`
  - Added `margin: 0` to body element for better page edge handling
  - Added `content-block p` margin-bottom style
  - Improved `.stage-name` width calculation using `calc(100% - 200px)`
  - Enhanced responsive breakpoint with better mobile support

### PR: Update comprehensive CSS styles for MARRTA reports
#### Changes
- **Report Styling Enhancements**:
  - Updated CSS to comprehensive Qt-compatible version with all necessary classes
  - Changed `.cover-title` background from gradient to solid color (#4A7EBB) for better Qt compatibility
  - Updated `.section-header` padding from `20px 30px` to `50px 30px` for improved visual balance
  - Added comprehensive styles for initiator cards, defense sections, and risk profiles
  - Added complete table styling with alternating row colors and hover effects
  - Added stage list, summary table, and content block styles
  - Added clearfix utility class for proper float clearing
  - Added responsive design breakpoints for mobile devices
  - Added print-specific styles to prevent awkward page breaks
  - Fixed color references (removed invalid "theotocopulitos/MARRTA#" prefix)

### PR #141: Fix compilation errors, add HTML preview, and enhance report generation UI
#### Changes
- **Compilation Errors Fixed**:
  - Fixed variable naming conflict: renamed local `config` to `modelConfig` to avoid conflict with member variable
  - Added missing `#include "dbmanager.h"` for Database::DbManager access
  - Removed unused variables `numEtapas` and `numSubetapas` to eliminate compiler warnings
  - Fixed Qt 6 compatibility: replaced `QSet<DefenseKey>` with `QMap<DefenseKey, bool>` (Qt 6 qHash requirement)
  - Removed `QTextStream::setCodec()` call (not needed in Qt 5/6)

- **HTML Preview Functionality**:
  - Added "Vista Previa HTML" button to report configuration dialog
  - Implemented preview dialog with QTextBrowser for reviewing HTML before PDF generation
  - Added option to save HTML directly from preview dialog
  - Refactored HTML generation into reusable `generateFullHtml()` method

- **Report Configuration UI Improvements**:
  - Changed menu text from "Generar informe HTML" to "Generar informe"
  - Renamed section "Listado completo de secuencias" to "Listado detallado de secuencias"
  - Auto-select all stages/substages when "Listado detallado" checkbox is checked
  - Enhanced validation for both preview and generate buttons
  - Reordered format options: Vertical (left, default) and Apaisado (right)
  - Increased button widths for better text visibility (Vista Previa: 160px, Generar: 140px)
  - Added icons to format radio buttons

- **Professional Report Styling**:
  - Implemented MARRTA color scheme: primary blue (#4A7EBB), dark blue (#2E5C8A), light blue (#6B9FD4)
  - Created professional cover page with MARRTA logo and gradient title box
  - Added section styling: dark blue right-aligned titles, primary blue headers, light blue subheaders
  - Centered tables in "Información del modelo" section (80% width)
  - Fixed percentage display from "%%" to "%" throughout
  - Split initiator table into two: "Número de iniciadores" and "Número de defensas"
  - Implemented unique defense counting by code, name, and robustness combination
  - Applied risk profile color coding: RMA (#FFCCCC), RA (#FFE4CC), RM (#FFFFCC), RB (#CCFFCC)
  - Replaced CSS variables with direct color values for Qt WebEngine/QTextBrowser compatibility

- **Menu and Toolbar Enhancements**:
  - Added "Guardar como..." to Archivo/Modelo menu with save.svg icon
  - Changed "Editor" to "Editor JS" in Scripting menu with edit.svg icon
  - Added "Scripting" action to main toolbar between "Guardar los cambios" and "Informe"
  - Linked ribbon "Informe" button to generate HTML report dialog

- **About Dialog Improvements**:
  - Added "Recursos en línea" section with clickable hyperlinks
  - Latest releases: https://github.com/risk-marrta/MARRTA/releases
  - Latest models: https://github.com/risk-marrta/marrta-modelos-de-riesgo/releases

- **Code Cleanup**:
  - Removed unused `dlggenerarinforme` files and references
  - Fixed UI translation strings (removed tr() wrappers from .ui files)
  - Moved all .md documentation files to `Documentation/Support/` (except README.md and RELEASE_INFO.md)

---

### Previous Changes
- **HTML Report Improvements**: Enhanced HTML report generation with comprehensive configuration
  - Added new report configuration dialog (`DlgConfigInforme`) for HTML reports
  - Implemented hierarchical stage/substage tree selection for selecting events
  - Added support for filtering reports by risk levels (RMA, RA, RM, RB)
  - Changed PDF output to landscape orientation for wider tables
  - Enhanced tables to show defense codes instead of IDs
  - Added robustness column (as text) to defense tables
  - Included calculated frequency, probability, consequences, and risk in report
  - Applied consistent table format to frequency reducers and consequence reducers
  - Added progress dialog with status messages during report generation
  - Generate multi-event reports with one page per event
  - All UI text properly uses `tr()` for translation compatibility

- **Bug Fix**: Fixed compilation errors in `dlggenerarinforme.cpp`
  - Corrected type declaration from `QList<ElementStage>` to `QList<EtapaDefinition>` on lines 46 and 98
  - Fixed DefenseDefinition member access: changed `activo` to `active` and `codigo` to `code`
  - Resolves all compilation errors in report generation dialog
  - Fixes issue #137

- **HTML Report Generator - Documentation and Translation**
  - Fixed translation support: all UI labels now use `QObject::tr()` for proper internationalization
  - Added comprehensive technical documentation: `Documentation/Support/HTML_REPORT_GENERATOR.md`
  - Added detailed user guide in Spanish: `Documentation/Support/HTML_REPORT_USER_GUIDE.md`
  - Improved HTML escaping for security
  - Documentation includes architecture, usage examples, troubleshooting, and best practices

- **New feature**: HTML Report Generator
  - Added ability to generate professional PDF reports with HTML/CSS formatting
  - New `InformeGenerator` class for generating structured reports
  - New dialog `DlgGenerarInforme` for selecting events and generating reports
  - Reports include: event stages, initiators, descriptions, frequency/consequences, barriers, and reducers
  - Professional A4 format with customizable styling matching Word template specifications
  - Menu option: "Generar informe HTML..." added to report menu
### Changes
- **LimeReport Removal**: Removed all LimeReport library references and functionality
  - Deleted LimeReport directory and library files
  - Removed LimeReport include statements from mainwindow.cpp
  - Removed menu actions "Lanzar informe" and "Editar informe" from UI
  - Removed LimeReport library configuration from marrta.pro (LIBS, INCLUDEPATH, DEPENDPATH)
  - Cleaned up .gitignore to remove LimeReport-specific entries
  - Note: Internal ReportViewer functionality (using Qt's native components) is preserved
- **MSVC Compiler Compatibility**: Fixed compilation errors when using MSVC compiler
  - Replaced GCC-specific `__attribute__((unused))` with Qt's cross-platform `Q_UNUSED()` macro
  - Modified files: `dbevent.cpp`, `dbmanager.cpp`, `mainwindow.cpp`, `sucesos.cpp`, `subetapas.cpp`, `modelos/xlsx.h`
  - Enables WebEngine usage by allowing compilation with MSVC

---

## Version 1.5 (2025-10-11)

### Changes
- **New function**: Added new defenses management window: can change application, add new events, and play with its hardiness, add defense to new event(s)

---

## Version 1.4.5 (2025-10-08)

### Changes
- **Code Optimization**: Extract GitHub URLs to constants for update checker (PR #78)
  - Improved maintainability of update checker module
  - Centralized GitHub API URLs in constant definitions
  - Modified `updatechecker.h` to use defined constants
- **Improve excel import wizard**: Saves open database and opens model after conversion

---

## Version 1.4.4 (2025-10-07)

### New Features
- **New Model Creation Wizard**: Added comprehensive wizard for creating new MARRTA models
  - Step-by-step guided process for model creation
  - File location selection with .pdb extension handling
  - Basic model information input (nombre, fecha, técnica, analistas, institución, descripción, observaciones)
  - Optional stage and substage configuration
  - Interactive tree view for managing stages and substages
  - Reordering capabilities with "Subir" and "Bajar" buttons
  - Progress tracking during model creation

---

## Version 1.4.3 (2025-10-07)

### Changes
- Version bump from 1.4.2 to 1.4.3
- Bug fixes and stability improvements
- Documentation updates

### Files Updated
- `mainwindow.h` - Updated APP_VERSION to 1.4.3
- `README.md` - Updated version information
- `SCRIPTING_API.md` - Added version 1.4.3 to version history

---

## Version 1.4.2 (2025-09-30)

### New Features
- Added `getAllStages()` method to access virtual stages (stages with negative IDs) from scripting
- Enhanced scripting capabilities for stage management

### Scripting API Improvements
- New method: `Marrta.getAllStages()` returns all stages including virtual stages
- Improved stage object properties with complete stage information

### Documentation
- Updated SCRIPTING_API.md with comprehensive documentation for `getAllStages()` method
- Added examples for working with virtual stages and regular stages

---

## Version 1.4.1 and Earlier

### Major Features (Historical)
- JavaScript scripting engine based on Qt's QJSEngine
- Complete scripting API for events, defenses, and charts
- Stage and substage management system
- Event management with toggle capabilities
- Defense management with frequency reducers, barriers, and consequence reducers
- Risk analysis and matrix visualization
- Report generation capabilities
- Save and load scripts functionality
- Interactive scripting documentation

### Scripting API
- **ConsoleAdapter**: Console output for debugging
- **EventAdapter**: Event management and querying
- **DefenseAdapter**: Defense management and analysis
- **ChartAdapter**: Chart generation and visualization
- **StageAdapter**: Stage and substage information access

### Core Features
- Stage Management: Define and organize project stages and substages
- Event Management: Create and manage initiating events
- Defense Management: Configure frequency reducers, barriers, and consequence reducers
- Risk Analysis: Calculate and visualize risk matrices
- Reporting: Generate comprehensive reports
- Scripting Engine: Automate tasks using JavaScript

---

## Support

For issues, questions, or contributions, please visit:
https://github.com/theotocopulitos/MARRTA

## Changelog Format

Each release includes:
- **Version Number**: Semantic versioning (MAJOR.MINOR.PATCH)
- **Release Date**: ISO format (YYYY-MM-DD)
- **Changes**: List of new features, improvements, and bug fixes
- **Files Updated**: List of modified files for version-specific changes

---

*Last Updated: 2025-10-11*
