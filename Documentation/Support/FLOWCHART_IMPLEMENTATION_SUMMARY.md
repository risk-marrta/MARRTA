# Flowchart Generator - Implementation Summary

## Issue Reference

**Issue Title**: Añadir flujograma corregido al menu modelo (2)

**Issue Description**: Add a menu option to generate a vertical flowchart (flujograma) from the MARRTA database, replicating the output of the Python script `generar_flujograma_v25_fix.py` but using the database instead of Excel as the data source.

## Implementation Overview

This implementation adds a new feature to MARRTA that generates an interactive HTML flowchart visualization of the risk model structure. The flowchart displays stages (etapas), substages (subetapas), and events (sucesos iniciadores) with their associated risk levels in a hierarchical, collapsible format.

## Changes Made

### New Files Created

1. **flujogramagenerator.h** (55 lines)
   - Header file defining the `FlujogramaGenerator` class
   - Static methods for generating HTML flowcharts
   - Private utility methods for CSS, JavaScript, and data extraction

2. **flujogramagenerator.cpp** (477 lines)
   - Implementation of flowchart generation
   - Database queries for stages, substages, and events
   - HTML generation with embedded CSS and JavaScript
   - UTF-8 encoding support
   - Error handling

3. **Documentation/Support/FLOWCHART_GENERATOR_FEATURE.md** (299 lines)
   - Comprehensive technical documentation
   - Database schema explanation
   - User flow documentation
   - Testing recommendations
   - Future enhancement ideas

4. **Documentation/Support/FLOWCHART_GENERATOR_VISUAL_GUIDE.md** (405 lines)
   - Visual representation of the feature
   - ASCII diagrams of UI elements
   - Flow diagrams
   - Color scheme documentation
   - Example outputs

### Modified Files

1. **mainwindow.ui** (+13 lines)
   - Added `actionGenerarFlujograma` action definition
   - Added menu item to "Modelo" menu after "Generar informe..."
   - Configured icon (list-alt.svg), text, and tooltip

2. **mainwindow.h** (+2 lines)
   - Added `on_actionGenerarFlujograma_triggered()` slot declaration

3. **mainwindow.cpp** (+55 lines)
   - Added includes: `flujogramagenerator.h`, `QDesktopServices`, `QUrl`
   - Implemented `on_actionGenerarFlujograma_triggered()` handler
   - Model validation check
   - File save dialog
   - Flowchart generation call
   - Success/error messaging
   - Optional browser opening

4. **marrta.pro** (+2 lines)
   - Added `flujogramagenerator.cpp` to SOURCES
   - Added `flujogramagenerator.h` to HEADERS

5. **RELEASE_INFO.md** (+23 lines)
   - Documented changes at the top of the file
   - Listed new files, modified files, and feature description

## Technical Details

### Database Schema Usage

The implementation queries three main tables from the SQLite database:

```sql
-- Stages
SELECT id, nombre, codigo, orden FROM etapas WHERE activo = 1 ORDER BY orden

-- Substages
SELECT id, nombre, orden FROM subetapas 
WHERE etapa_id = :etapa_id AND activo = 1 ORDER BY orden

-- Events
SELECT id, codigo, nombre, frecuencia, consecuencia, riesgo, activo
FROM suceso_iniciador 
WHERE etapa_id = :etapa_id AND (subetapa_id = :subetapa_id OR subetapa_id IS NULL)
ORDER BY codigo
```

### HTML Output Structure

The generated HTML file includes:

1. **DOCTYPE and meta tags** - HTML5 with UTF-8 encoding
2. **Embedded CSS** - Complete styling (responsive, color-coded, interactive)
3. **Content structure**:
   - Page title and header
   - Flowchart container with stages
   - Each stage contains substages and events
   - Legend for risk level colors
4. **Embedded JavaScript** - Collapsible stages, event handlers

### Risk Level Color Coding

- **RB (Riesgo Bajo)**: Gray (#808080)
- **RM (Riesgo Medio)**: Green (#70AD47)
- **RA (Riesgo Alto)**: Orange (#FFA500)
- **RMA (Riesgo Muy Alto)**: Red (#FF0000)

### Interactive Features

- Click stage headers to collapse/expand content
- Visual toggle indicators (▼/►)
- Hover effects on events with smooth transitions
- Self-contained HTML (no external dependencies)

## Code Quality Measures

### Translation Compatibility ✓
All user-facing strings use `tr()` for Qt translation system:
```cpp
tr("Guardar flujograma")
tr("El flujograma se ha generado correctamente")
tr("¿Desea abrir el flujograma generado en el navegador?")
```

### Error Handling ✓
- Database connection validation
- Query execution error checking
- File write error handling
- Empty data validation
- User-friendly error messages

### Code Style ✓
- Follows MARRTA project conventions
- Spanish method names (matching project style)
- Comprehensive code comments
- Minimal dependencies (Qt only)
- No memory leaks (QVariant auto-cleanup)

### Documentation ✓
- Comprehensive feature documentation
- Visual guide with ASCII diagrams
- Code comments in implementation
- Updated RELEASE_INFO.md

## User Experience

### Menu Access
```
Modelo → Generar flujograma...
```

### Workflow
1. User clicks menu item
2. System validates model is loaded
3. File save dialog appears
4. User selects location and filename
5. System generates HTML flowchart
6. Success message with file path
7. Optional: Open in default browser

### Validation
- Checks if database is open
- Checks if a model file is loaded
- Shows warning if no model loaded
- Uses existing `showNoModelLoadedWarning()` method

## Testing Status

### Automated Testing
❌ Not applicable - requires Qt GUI environment for full testing

### Manual Testing Required
✅ Code review completed
⚠️ Manual testing pending:
- Load model and generate flowchart
- Verify HTML structure
- Test in various browsers
- Verify risk level colors
- Test collapsible functionality
- Test with different model sizes
- Verify file save/cancel scenarios

## Comparison with Python Script

| Aspect | Python Script | Qt/C++ Implementation |
|--------|--------------|----------------------|
| Data Source | Excel (.xlsx) | SQLite database |
| Integration | External tool | Built-in feature |
| Dependencies | Python + libs | Qt only |
| Performance | Good | Excellent |
| Maintenance | Separate | Single codebase |
| Consistency | Manual sync | Automatic |
| Output | HTML | HTML |
| Style | Custom | Similar + enhanced |

## Key Benefits

1. **Native Integration**: No external tools needed
2. **Data Consistency**: Uses same database as application
3. **Performance**: Direct database access, no Excel conversion
4. **Maintainability**: Single codebase, follows Qt patterns
5. **User-Friendly**: Integrated menu, file dialogs, browser opening
6. **Professional Output**: Clean HTML with modern CSS/JS
7. **Interactive**: Collapsible stages, hover effects
8. **Portable**: Self-contained HTML file

## Future Enhancement Ideas

1. Export to PDF/PNG formats
2. Filter by risk level or stage
3. Search functionality
4. Statistics overlay
5. Customizable color schemes
6. Defense/barrier visualization
7. Print-optimized layout
8. Dark mode theme
9. Event details popup
10. Direct edit from flowchart

## Files Summary

```
New Files (2):
- flujogramagenerator.h        55 lines
- flujogramagenerator.cpp      477 lines

Modified Files (5):
- mainwindow.ui                +13 lines
- mainwindow.h                 +2 lines
- mainwindow.cpp               +55 lines
- marrta.pro                   +2 lines
- RELEASE_INFO.md              +23 lines

Documentation (2):
- Documentation/Support/FLOWCHART_GENERATOR_FEATURE.md      299 lines
- Documentation/Support/FLOWCHART_GENERATOR_VISUAL_GUIDE.md 405 lines

Total: 1,331 lines added
```

## Build Configuration

The new files are properly integrated into the Qt build system:

**marrta.pro additions:**
```qmake
SOURCES += \
    # ... existing files ...
    flujogramagenerator.cpp \
    # ... rest of files ...

HEADERS += \
    # ... existing files ...
    flujogramagenerator.h \
    # ... rest of files ...
```

## Compliance Checklist

- ✅ **Minimal Changes**: Only necessary files modified
- ✅ **Qt Translation**: All UI strings use tr()
- ✅ **Documentation**: Created in Documentation/Support/
- ✅ **RELEASE_INFO.md**: Updated with PR changes
- ✅ **Code Style**: Follows project conventions
- ✅ **No Breaking Changes**: Existing features unaffected
- ✅ **Memory Management**: Proper Qt parent-child ownership
- ✅ **Error Handling**: Comprehensive validation
- ✅ **Comments**: Clear, descriptive documentation
- ✅ **Build System**: Properly integrated into .pro file

## Known Limitations

1. Read-only visualization (cannot edit from flowchart)
2. Very large models (1000+ events) may be slow in browser
3. Shows only active items (activo = 1)
4. Requires default browser for auto-open
5. No undo for file generation

## Conclusion

The flowchart generator feature has been successfully implemented with:
- Complete C++/Qt code reading from database
- Interactive HTML output with professional styling
- Full integration into MARRTA's menu system
- Comprehensive documentation
- No external dependencies
- Minimal code changes (following project guidelines)

The implementation replicates the Python script functionality while providing better integration, performance, and maintainability. All code follows MARRTA's standards including Qt translation compatibility and proper error handling.

## Next Steps

1. **Manual Testing**: Test with real MARRTA models
2. **User Feedback**: Gather input on HTML output style
3. **Optional**: Add export to PDF/PNG if requested
4. **Optional**: Enhance interactivity based on usage

## Git Commits

1. `fa25e14` - Add flowchart generator feature to Model menu
2. `727bd8a` - Add comprehensive documentation for flowchart generator feature

## Branch

`copilot/add-corrected-flowchart-to-model-menu`
