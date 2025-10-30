# HTML Report Generator Implementation

## Overview

This document describes the implementation of the HTML Report Generator feature for MARRTA. This feature allows users to generate professional PDF reports with HTML/CSS formatting from event data.

## Problem Statement

Users needed a way to generate structured reports in A4 format with a specific professional layout that includes:
- Event stage information
- Initiator details
- Error descriptions
- Frequency and consequences
- Context information
- Tables for barriers and reducers

## Solution

The solution consists of three main components:

### 1. InformeGenerator Class

**Files:**
- `informegenerator.h`
- `informegenerator.cpp`

**Purpose:** Core class that handles the generation of HTML content and PDF output.

**Key Components:**

#### Data Structures
```cpp
struct Barrera {
    int id;
    QString descripcion;
    QString implementacion;
};

struct Reductor {
    int id;
    QString descripcion;
};

struct DatosInforme {
    QString etapa;
    QString iniciador;
    QString descripcionError;
    QString frecuencia;
    QString consecuencias;
    QString contexto;
    QVector<Barrera> barreras;
    QVector<Reductor> reductoresFrecuencia;
    QVector<Reductor> reductoresConsecuencias;
};
```

#### Main Methods

1. **`generarPDF(const DatosInforme& datos, const QString& rutaSalida)`**
   - Creates QPrinter configured for A4 PDF output
   - Generates HTML content and renders it to PDF
   - Returns success status

2. **`generarHTML(const DatosInforme& datos)`**
   - Constructs complete HTML document
   - Includes CSS styling
   - Conditionally adds sections based on available data

3. **`obtenerEstilosCSS()`**
   - Returns CSS styling for professional report appearance
   - Matches Word template specifications:
     - Blue headers (#4472C4)
     - Gray sections (#D9D9D9)
     - Bordered tables
     - A4 page layout

4. **`generarTablaBarreras(const QVector<Barrera>& barreras)`**
   - Creates HTML table for barriers
   - Three columns: ID, Description, Implementation

5. **`generarTablaReductores(const QVector<Reductor>& reductores)`**
   - Creates HTML table for reducers
   - Two columns: ID, Description

### 2. DlgGenerarInforme Dialog

**Files:**
- `dlggenerarinforme.h`
- `dlggenerarinforme.cpp`
- `dlggenerarinforme.ui`

**Purpose:** User interface for selecting events and generating reports.

**Features:**

1. **Event Selection**
   - Combo box listing all available events
   - Shows event code and name

2. **Preview Panel**
   - Displays summary of selected event
   - Shows counts of barriers and reducers

3. **Generate Button**
   - Opens file save dialog
   - Converts event data to report format
   - Calls InformeGenerator to create PDF

**Translation Support:**
All UI strings use `tr()` for Qt translation compatibility:
```cpp
setWindowTitle(tr("Generar Informe HTML"));
QString preview = tr("Etapa: %1\n").arg(nombreEtapa);
QMessageBox::warning(this, tr("Error"), tr("Por favor seleccione un evento"));
```

### 3. MainWindow Integration

**Modified Files:**
- `mainwindow.h` - Added slot declaration
- `mainwindow.cpp` - Implemented slot and included dialog
- `mainwindow.ui` - Added menu action
- `marrta.pro` - Added new files to build

**Menu Location:**
The new option "Generar informe HTML..." appears in the File menu under the report section, after "Editar informe...".

**Implementation:**
```cpp
void MainWindow::on_actionGenerarInformeHTML_triggered()
{
    // Check database is open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        showNoModelLoadedWarning();
        return;
    }
    
    // Collect all events
    QList<CompleteEventDefinition> allEvents;
    for (const auto& pair : events) {
        for (const auto& event : pair.second) {
            allEvents.append(event);
        }
    }
    
    // Show dialog
    DlgGenerarInforme dialog(allEvents, this);
    dialog.exec();
}
```

## Report Format Specification

### HTML Structure

1. **Header Section (Blue)**
   - Shows stage name
   - Blue background (#4472C4)
   - White text, bold, centered

2. **Initiator Section (Gray)**
   - Shows initiator name
   - Gray background (#D9D9D9)
   - Bold text

3. **Description Box**
   - Error description
   - White background with border
   - Padding for readability

4. **Info Bar (Blue)**
   - Frequency and consequences
   - Blue background, white text

5. **Context Paragraph**
   - Justified text
   - Additional context information

6. **Barriers Table** (if present)
   - Three columns: ID (60px), Description (45%), Implementation (remaining)
   - Bordered cells
   - Gray header cells

7. **Frequency Reducers Table** (if present)
   - Two columns: ID (60px), Description (remaining)
   - Bordered table

8. **Consequence Reducers Table** (if present)
   - Two columns: ID (60px), Description (remaining)
   - Bordered table

### CSS Styling Highlights

```css
body {
    font-family: 'Calibri', 'Segoe UI', Arial, sans-serif;
    font-size: 10pt;
    color: #000000;
    line-height: 1.4;
}

.header-azul {
    background-color: #4472C4;
    color: white;
    padding: 8px 15px;
    font-weight: bold;
}

.titulo-seccion {
    color: #4472C4;
    font-weight: bold;
    border-bottom: 2px solid #4472C4;
}
```

## Data Flow

1. **User Action:** User clicks "Generar informe HTML..." menu option
2. **Validation:** System checks if database is open
3. **Data Collection:** All events are collected from the events map
4. **Dialog Display:** DlgGenerarInforme shows with event list
5. **Event Selection:** User selects event from combo box
6. **Preview Update:** Preview panel shows event summary
7. **Generate Request:** User clicks "Generar PDF" button
8. **File Selection:** Save file dialog appears
9. **Data Conversion:** Event data converted to DatosInforme structure
10. **PDF Generation:** InformeGenerator creates HTML and renders to PDF
11. **Confirmation:** Success/error message shown to user

## Files Modified

### New Files Created
- `/home/runner/work/MARRTA/MARRTA/informegenerator.h`
- `/home/runner/work/MARRTA/MARRTA/informegenerator.cpp`
- `/home/runner/work/MARRTA/MARRTA/dlggenerarinforme.h`
- `/home/runner/work/MARRTA/MARRTA/dlggenerarinforme.cpp`
- `/home/runner/work/MARRTA/MARRTA/dlggenerarinforme.ui`
- `/home/runner/work/MARRTA/MARRTA/Documentation/Support/HTML_REPORT_IMPLEMENTATION.md`

### Modified Files
- `/home/runner/work/MARRTA/MARRTA/marrta.pro` - Added new files to build
- `/home/runner/work/MARRTA/MARRTA/mainwindow.h` - Added slot declaration
- `/home/runner/work/MARRTA/MARRTA/mainwindow.cpp` - Implemented slot, added include
- `/home/runner/work/MARRTA/MARRTA/mainwindow.ui` - Added menu action
- `/home/runner/work/MARRTA/MARRTA/RELEASE_INFO.md` - Documented changes

## Translation Compliance

All user-facing strings follow Qt translation standards:
- All UI text wrapped with `tr()`
- Dynamic content uses `.arg()` for placeholders
- Follows MARRTA's translation guidelines

Examples:
```cpp
setWindowTitle(tr("Generar Informe HTML"));
QString msg = tr("Se encontraron %1 eventos").arg(count);
QMessageBox::information(this, tr("Éxito"), tr("Informe generado correctamente"));
```

## Testing Instructions

### Manual Testing Steps

1. **Prerequisites:**
   - Open MARRTA application
   - Load a model with events, barriers, and reducers

2. **Access Feature:**
   - Go to File menu (or Reports menu)
   - Click "Generar informe HTML..."

3. **Select Event:**
   - Choose an event from the dropdown
   - Verify preview shows correct information

4. **Generate Report:**
   - Click "Generar PDF"
   - Choose save location
   - Verify PDF is created successfully

5. **Verify PDF Content:**
   - Open generated PDF
   - Check all sections are present
   - Verify formatting matches specification
   - Confirm barriers and reducers tables are correct

### Test Cases

1. **Event with all data:**
   - Event with barriers, frequency reducers, and consequence reducers
   - Should show all sections

2. **Event with partial data:**
   - Event with only barriers
   - Should only show barrier section

3. **Empty event:**
   - Event with no defenses
   - Should show basic info without defense tables

4. **Multiple events:**
   - Test with different events
   - Verify correct data appears for each

## Future Enhancements

Potential improvements for future versions:

1. **Batch Generation:** Generate reports for multiple events at once
2. **Template Customization:** Allow users to customize report templates
3. **HTML Export:** Option to export as HTML instead of PDF
4. **Report Preview:** Show PDF preview before saving
5. **Additional Formats:** Support for DOCX or ODT export
6. **Custom Styling:** User-configurable colors and fonts
7. **Logo Integration:** Add organization logos to reports
8. **Multi-language Reports:** Generate reports in different languages

## Technical Notes

### Qt Components Used
- QPrinter: PDF generation
- QTextDocument: HTML to PDF rendering
- QFileDialog: File save functionality
- QComboBox: Event selection
- QPlainTextEdit: Preview display

### Database Access
Uses existing MARRTA database classes:
- `Database::DbElementStage` for stage information
- `CompleteEventDefinition` for event data
- DefenseDefinition for defense information

### Memory Management
- Qt's parent-child ownership for widgets
- No manual memory leaks (Qt handles cleanup)
- Temporary QVector objects cleaned up automatically

## Compatibility

- **Qt Version:** Compatible with Qt 5.x
- **C++ Standard:** C++17
- **Platforms:** Windows, Linux, macOS (Qt supported)
- **Dependencies:** QPrinter, QTextDocument (already in MARRTA)
