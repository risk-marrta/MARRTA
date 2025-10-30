# Flujograma Generator - Feature Documentation

## Overview

This document describes the implementation of the vertical flowchart generator feature for MARRTA. This feature allows users to generate an interactive HTML visualization of the risk model's structure, showing stages, substages, and events with their associated risk levels.

## Feature Description

The flowchart generator creates an HTML file that displays a vertical flowchart of the loaded risk model. The flowchart includes:

- **Stages (Etapas)**: Shown as collapsible blue headers with stage codes
- **Substages (Subetapas)**: Shown as purple-bordered sections within stages
- **Events (Sucesos Iniciadores)**: Individual event items with:
  - Event code
  - Event name
  - Risk level (RB, RM, RA, RMA) with color coding

## User Interface

### Menu Location

The feature is accessible from:
```
Modelo → Generar flujograma...
```

This menu item appears right after "Generar informe..." in the Modelo menu.

### Menu Action Properties

- **Icon**: list-alt.svg (vertical list icon)
- **Text**: "Generar flujograma..."
- **Tooltip**: "Generar flujograma vertical del modelo de riesgo"

### User Flow

1. User selects "Modelo → Generar flujograma..."
2. System validates that a model is loaded
   - If no model: Shows warning message
   - If model loaded: Continues to step 3
3. System opens file save dialog
   - Default filename: "flujograma_modelo.html"
   - Default location: User's home directory
   - File filter: "Archivos HTML (*.html *.htm)"
4. User selects save location and filename
5. System generates HTML flowchart file
6. System shows success message with file path
7. System asks if user wants to open the flowchart in browser
   - If Yes: Opens HTML file in default browser
   - If No: User can open it manually later

## Technical Implementation

### New Files

#### flujogramagenerator.h
Header file defining the `FlujogramaGenerator` class with static methods for generating HTML flowcharts.

**Public Methods:**
- `bool generarHTML(const QString& rutaSalida)` - Main method to generate the flowchart

**Private Methods:**
- `QString obtenerEstilosCSS()` - Returns CSS styles for the flowchart
- `QString obtenerJavaScript()` - Returns JavaScript code for interactivity
- `QVariantList obtenerDatosModelo()` - Queries database for model data
- `QString generarContenidoHTML(const QVariantList& datos)` - Generates HTML content
- `QString obtenerColorRiesgo(const QString& riesgo)` - Returns color for risk level

#### flujogramagenerator.cpp
Implementation of the flowchart generator.

**Key Features:**
- Queries SQLite database for stages, substages, and events
- Preserves hierarchical structure (stages → substages → events)
- Generates clean, valid HTML5 with embedded CSS and JavaScript
- Uses UTF-8 encoding for proper character support
- Includes error handling for database queries and file operations

### Modified Files

#### mainwindow.ui
- Added `actionGenerarFlujograma` action definition
- Added action to Modelo menu
- Configured icon, text, and tooltip properties

#### mainwindow.h
- Added `on_actionGenerarFlujograma_triggered()` slot declaration

#### mainwindow.cpp
- Added `#include "flujogramagenerator.h"`
- Added `#include <QDesktopServices>` and `#include <QUrl>`
- Implemented `on_actionGenerarFlujograma_triggered()` handler
  - Model validation
  - File save dialog
  - Flowchart generation
  - Success/error messaging
  - Optional browser opening

#### marrta.pro
- Added `flujogramagenerator.cpp` to SOURCES
- Added `flujogramagenerator.h` to HEADERS

## Database Schema Used

The flowchart generator queries the following tables:

### etapas (Stages)
```sql
SELECT id, nombre, codigo, orden 
FROM etapas 
WHERE activo = 1 
ORDER BY orden
```

### subetapas (Substages)
```sql
SELECT id, nombre, orden 
FROM subetapas 
WHERE etapa_id = :etapa_id AND activo = 1 
ORDER BY orden
```

### suceso_iniciador (Events)
```sql
SELECT id, codigo, nombre, frecuencia, consecuencia, riesgo, activo
FROM suceso_iniciador 
WHERE etapa_id = :etapa_id AND (subetapa_id = :subetapa_id OR subetapa_id IS NULL)
ORDER BY codigo
```

## HTML Output Structure

### CSS Styling

The generated HTML includes comprehensive CSS with:

- **Responsive design**: Works on different screen sizes
- **Color-coded risk levels**:
  - RB (Riesgo Bajo): Gray (#808080)
  - RM (Riesgo Medio): Green (#70AD47)
  - RA (Riesgo Alto): Orange (#FFA500)
  - RMA (Riesgo Muy Alto): Red (#FF0000)
- **Interactive elements**: Hover effects, transitions
- **Visual hierarchy**: Clear distinction between stages, substages, and events
- **Professional appearance**: Gradients, shadows, rounded corners

### JavaScript Functionality

The flowchart includes JavaScript for:

- **Collapsible stages**: Click stage header to show/hide content
- **Toggle icons**: Visual indicator of collapsed/expanded state
- **Event click handlers**: Ready for future enhancements
- **DOM-ready execution**: Ensures handlers are attached after page load

### HTML Structure

```html
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Flujograma del Modelo de Riesgo - MARRTA</title>
    <style>...</style>
</head>
<body>
    <div class="container">
        <h1>Flujograma del Modelo de Riesgo</h1>
        <div class="flowchart">
            <!-- Stages with substages and events -->
        </div>
        <div class="legend">
            <!-- Risk level legend -->
        </div>
    </div>
    <script>...</script>
</body>
</html>
```

## Code Quality Features

### Translation Compatibility

All user-facing strings use `tr()` for Qt translation compatibility:
- Dialog titles
- Button text
- Messages
- File filters

### Error Handling

- Database connection validation
- Query execution error checking
- File write error handling
- Empty data validation
- User-friendly error messages

### Code Style

- Follows MARRTA project conventions
- Clear method names in Spanish
- Comprehensive comments
- Proper memory management
- No memory leaks (QVariant automatic cleanup)

## Testing Recommendations

### Test Cases

1. **No model loaded**
   - Action: Select "Generar flujograma..." without loading a model
   - Expected: Warning message displayed

2. **Empty model**
   - Action: Load model with no stages/events, generate flowchart
   - Expected: Error message about no data

3. **Normal model**
   - Action: Load model with stages, substages, and events
   - Expected: HTML file generated successfully

4. **File save cancel**
   - Action: Open dialog and cancel file save
   - Expected: No error, operation cancelled gracefully

5. **File save error**
   - Action: Try to save to a read-only location
   - Expected: Error message displayed

6. **Large model**
   - Action: Generate flowchart from model with many stages/events
   - Expected: HTML generated successfully, performs well in browser

7. **Browser opening**
   - Action: Generate flowchart and choose to open in browser
   - Expected: Default browser opens with the HTML file

### Manual Verification Steps

1. Compile the project with the new files
2. Launch MARRTA
3. Load a risk model (.pdb file)
4. Go to Modelo → Generar flujograma...
5. Save the HTML file
6. Verify success message appears
7. Open the HTML file in a browser
8. Verify the flowchart displays correctly:
   - Stages are collapsible
   - Events show correct risk levels
   - Colors match risk levels
   - Layout is clean and readable

## Future Enhancements

Possible improvements for future versions:

1. **Export options**: PDF, PNG, SVG output
2. **Filtering**: Show only specific risk levels or stages
3. **Search**: Find events by code or name
4. **Statistics**: Show event counts by risk level
5. **Customization**: User-configurable colors and styles
6. **Zoom controls**: For very large models
7. **Print optimization**: Better layout for printing
8. **Dark mode**: Alternative color scheme
9. **Event details popup**: Click to show full event information
10. **Defense visualization**: Show defenses/barriers in the flowchart

## Known Limitations

1. Flowchart is read-only (cannot edit model from the HTML)
2. Very large models (1000+ events) may take time to render in browser
3. Flowchart shows only active items (activo = 1)
4. No undo/redo for file generation
5. Requires default browser to be configured for auto-open feature

## Comparison with Python Script

This implementation generates a similar vertical flowchart as the reference Python script (generar_flujograma_v25_fix.py), but:

- **Data source**: Reads from MARRTA database instead of Excel
- **Integration**: Native Qt/C++ code, no external Python dependency
- **Performance**: Faster for large models (no data conversion)
- **Consistency**: Uses exact same database as the application
- **Maintainability**: Single codebase for all features

## Related Files

- `/home/runner/work/MARRTA/MARRTA/flujogramagenerator.h`
- `/home/runner/work/MARRTA/MARRTA/flujogramagenerator.cpp`
- `/home/runner/work/MARRTA/MARRTA/mainwindow.ui`
- `/home/runner/work/MARRTA/MARRTA/mainwindow.h`
- `/home/runner/work/MARRTA/MARRTA/mainwindow.cpp`
- `/home/runner/work/MARRTA/MARRTA/marrta.pro`
- `/home/runner/work/MARRTA/MARRTA/RELEASE_INFO.md`

## Author Notes

This feature was implemented to provide users with a visual overview of their risk model structure. The HTML output is self-contained and can be shared, archived, or embedded in documentation. The implementation prioritizes code simplicity, maintainability, and consistency with existing MARRTA features.
