# Model Comparison Feature Implementation Summary

## Overview

This document describes the implementation of the Model Comparison feature (Comparador de Modelos) for MARRTA. This feature allows users to compare two risk models side-by-side, highlighting differences in stages, initiators, defenses, and risk profiles.

## Problem Statement

Users needed a way to:
1. Compare different versions of risk models to track changes over time
2. Identify differences between models created by different analysts or teams
3. Understand how model modifications affect risk assessments
4. Generate reports documenting model differences for audit or review purposes

## Solution

A comprehensive comparison dialog that:
- Opens a second PRAXIS database for comparison
- Analyzes and compares all aspects of the models:
  - Stages and substages structure
  - Initiators (codes, names, frequency, consequence values)
  - Defenses (codes, names, robustness levels)
  - Defense associations (which defenses are linked to which initiators)
  - Risk profiles (distribution of risk levels)
- Presents results in a tabbed interface with color-coded differences
- Generates visual charts for risk profile comparison
- Exports comprehensive HTML reports

## Features

### 1. Stage and Substage Comparison
- Counts stages and substages in each model
- Identifies stages unique to either model
- Highlights differences in substage counts
- Color codes: Green (equal), Yellow (different), Red (only in one model)

### 2. Initiator Comparison
- Compares all initiators by code
- Shows name differences between models
- Highlights changes in frequency (F) and consequence (C) values
- Identifies initiators added or removed
- Displays stage/substage context for each initiator

### 3. Defense Comparison
- Compares all unique defenses by code
- Shows name differences
- Highlights robustness level changes
- Identifies new or removed defenses
- Covers all defense types (frequency reducers, barriers, consequence reducers)

### 4. Defense Association Comparison
- Shows which defenses are "established" (present) for each initiator
- Shows which defenses are "applied" (active) for each initiator
- Highlights differences in associations between models
- Color codes changes in establishment or application status

### 5. Risk Profile Visualization
- Generates bar charts comparing risk distributions
- Shows counts for each risk level (Very High, High, Medium, Low)
- Displays statistics for applicable initiators
- Uses Qt Charts for professional visualization

### 6. HTML Export
- Generates comprehensive HTML report
- Includes summary statistics for all comparison categories
- Professional formatting with CSS styling
- Includes timestamp and model identification

## User Interface

### Dialog Layout
```
┌─────────────────────────────────────────────────────────────┐
│ Comparador de Modelos                                    [X] │
├─────────────────────────────────────────────────────────────┤
│ ┌─────────────────────────────────────────────────────────┐ │
│ │ Modelos a Comparar                                      │ │
│ │                                                         │ │
│ │ Modelo Actual: [Model Name (path/to/file.pdb)       ] │ │
│ │ Modelo a Comparar: [                    ] [Seleccionar] │ │
│ │                            [Comparar Modelos]          │ │
│ └─────────────────────────────────────────────────────────┘ │
│                                                             │
│ ┌─────────────────────────────────────────────────────────┐ │
│ │ [Resumen][Etapas][Iniciadores][Defensas][Asociac.][Gráf]│ │
│ │                                                         │ │
│ │  Comparison results displayed here in tabs             │ │
│ │                                                         │ │
│ │                                                         │ │
│ └─────────────────────────────────────────────────────────┘ │
│                                                             │
│ [Exportar Reporte]                              [Cerrar]  │
└─────────────────────────────────────────────────────────────┘
```

### Tabs

1. **Resumen (Summary)**: HTML summary with statistics
2. **Etapas y Subetapas**: Table with stage comparison
3. **Iniciadores**: Detailed initiator comparison table
4. **Defensas**: Defense comparison table
5. **Asociaciones**: Defense-to-initiator association comparison
6. **Gráficas**: Visual risk profile comparison charts

## Files Modified

### New Files Created

1. **dlgcomparadormodelos.h**
   - Dialog class declaration
   - Comparison data structures
   - Method declarations for comparison logic

2. **dlgcomparadormodelos.cpp**
   - Complete implementation of comparison logic
   - Database loading and querying
   - Table population with comparison results
   - Chart generation
   - HTML report generation

3. **dlgcomparadormodelos.ui**
   - Qt Designer UI file
   - Tabbed interface layout
   - Tables for each comparison category
   - Button controls

### Modified Files

1. **mainwindow.h**
   - Added slot declaration: `on_actionComparadorModelos_triggered()`

2. **mainwindow.cpp**
   - Added include: `#include "dlgcomparadormodelos.h"`
   - Implemented action handler to open comparison dialog
   - Checks for open model before allowing comparison

3. **mainwindow.ui**
   - Added action `actionComparadorModelos` to actions list
   - Added menu item to "Administración" menu with separator
   - Set icon (search.svg) and tooltip text

4. **marrta.pro**
   - Added dlgcomparadormodelos.cpp to SOURCES
   - Added dlgcomparadormodelos.h to HEADERS
   - Added dlgcomparadormodelos.ui to FORMS

5. **RELEASE_INFO.md**
   - Added comprehensive entry documenting the new feature

## Technical Details

### Database Handling

- Creates a separate QSqlDatabase connection for the comparison model
- Uses unique connection name to avoid conflicts: `"comparacion_" + timestamp`
- Properly closes and removes connection when dialog closes
- Validates database format before loading

### Data Structures

The comparison uses several internal structures:

```cpp
struct StageComparison {
    QString nombre;
    int substagesActual;
    int substagesComparacion;
    bool equal;
};

struct InitiatorComparison {
    QString codigo;
    QString nombreActual;
    QString nombreComparacion;
    int frecuenciaActual;
    int frecuenciaComparacion;
    int consecuenciaActual;
    int consecuenciaComparacion;
    QString etapa;
    bool codeOrNameDifferent;
    bool valuesDifferent;
};

struct DefenseComparison {
    QString codigo;
    QString nombreActual;
    QString nombreComparacion;
    int robustezActual;
    int robustezComparacion;
    bool codeOrNameDifferent;
    bool robustnessDifferent;
};

struct DefenseAssociationComparison {
    QString iniciadorCodigo;
    QString defensaCodigo;
    bool establecidaActual;
    bool establecidaComparacion;
    bool aplicadaActual;
    bool aplicadaComparacion;
};
```

### Key Methods

1. **cargarModeloComparacion()**: Opens and validates comparison database
2. **cargarEventosDeDB()**: Loads all events and defenses from a database
3. **realizarComparacion()**: Orchestrates all comparison operations
4. **compararEtapas()**: Compares stage/substage structure
5. **compararIniciadores()**: Compares initiator details
6. **compararDefensas()**: Compares defense details
7. **compararAsociacionesDefensas()**: Compares defense associations
8. **generarGraficas()**: Creates risk profile charts using Qt Charts
9. **generarReporteHTML()**: Generates exportable HTML report

### Translation Support

All user-facing strings use Qt's `tr()` function for internationalization:
- Dialog titles and labels
- Table headers
- Status messages
- Error messages
- Report text

This ensures the feature can be translated to multiple languages.

## Usage Instructions

### For Users

1. **Open a Model**: Open a PRAXIS model in MARRTA
2. **Access Comparison**: Go to menu "Administración" → "Comparador de modelos"
3. **Select Comparison Model**: Click "Seleccionar..." to choose a second .pdb file
4. **Run Comparison**: Click "Comparar Modelos" button
5. **Review Results**: Browse through the tabs to see different aspects
6. **Export Report**: Click "Exportar Reporte" to save HTML report

### For Developers

To extend the comparison functionality:

1. Add new comparison criteria in the data structures
2. Implement comparison logic in new methods
3. Update the UI to display new comparisons
4. Update HTML report generation to include new data

## Testing

### Manual Testing Checklist

- [ ] Dialog opens when menu item is clicked
- [ ] Warning shown if no model is open
- [ ] File picker allows selecting .pdb files
- [ ] Error shown if invalid file selected
- [ ] Cannot compare model with itself
- [ ] All tabs populate correctly after comparison
- [ ] Tables show correct data with proper formatting
- [ ] Color coding works correctly (green/yellow/red)
- [ ] Charts display correctly with proper labels
- [ ] Export creates valid HTML file
- [ ] Exported HTML displays correctly in browsers
- [ ] Dialog closes properly and cleans up resources
- [ ] Works with models of different sizes
- [ ] Handles models with missing stages/initiators/defenses

### Test Scenarios

1. **Identical Models**: Compare model with a copy (should show all equal)
2. **Modified Model**: Compare with version that has changes
3. **Different Models**: Compare completely different risk models
4. **Empty vs Full**: Compare empty model with populated one
5. **Large Models**: Test performance with large databases

## Future Enhancements

Potential improvements for future versions:

1. **Difference Highlighting**: Visual diff view for specific items
2. **Filtering**: Filter comparison results by type of change
3. **Merge Functionality**: Allow selective import of changes
4. **History Tracking**: Track multiple comparisons over time
5. **Version Control Integration**: Link with VCS systems
6. **Comparison Presets**: Save comparison configurations
7. **PDF Export**: Alternative export format
8. **Side-by-Side View**: Split screen comparison for details
9. **Change Summary**: Executive summary of key differences
10. **Statistical Analysis**: More detailed statistical comparisons

## Known Limitations

1. Comparison is read-only (no merge functionality)
2. Large models may take time to load and compare
3. Requires both models to be valid PRAXIS databases
4. Memory usage scales with model size
5. Charts limited to risk distribution (could add more metrics)

## Performance Considerations

- Database queries are optimized with proper indexing
- Event loading uses prepared statements
- Tables populated incrementally
- Charts generated only when tab is visible
- HTML generation done on-demand

## Security Considerations

- File paths validated before opening
- Database connections properly closed
- No modification of source databases
- Export path validated before writing
- Proper error handling for corrupt databases

## Conclusion

The Model Comparison feature provides MARRTA users with a powerful tool for understanding differences between risk models. The comprehensive comparison across all model dimensions, combined with visual presentation and export capabilities, makes it valuable for model validation, version control, and collaborative risk analysis.
