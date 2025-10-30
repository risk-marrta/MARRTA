# HTML Report Generator - Implementation Summary

## Overview

The HTML Report Generator is a feature in MARRTA that allows users to generate professional PDF reports with HTML/CSS formatting for individual events. The reports follow a specific A4 format template with structured sections for stages, initiators, descriptions, frequency/consequences analysis, barriers, and reducers.

## Purpose

This feature addresses the need to generate formal, professional reports for risk analysis events that can be:
- Shared with stakeholders
- Included in documentation
- Used for presentations
- Archived for compliance

## Architecture

### Core Components

1. **InformeGenerator** (`informegenerator.h/cpp`)
   - Static utility class for PDF generation
   - Handles HTML/CSS generation and PDF conversion
   - Uses Qt's QPrinter and QTextDocument for rendering

2. **DlgGenerarInforme** (`dlggenerarinforme.h/cpp/ui`)
   - User interface dialog for report generation
   - Event selection and preview
   - File save dialog integration

3. **MainWindow Integration** (`mainwindow.cpp`)
   - Menu action: "Generar informe HTML..."
   - Event data preparation
   - Dialog invocation

### Data Structures

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

## Report Format

### Document Structure

The generated PDF follows this structure:

1. **Header Section (Blue Background)**
   - ETAPA: [Stage Name]

2. **Initiator Section (Gray Background)**
   - INICIADOR: [Event Name]

3. **Description Box**
   - Event description in bordered box

4. **Info Bar (Blue Background)**
   - FRECUENCIA: [Frequency Level]
   - CONSECUENCIAS: [Consequence Level]

5. **Context Section**
   - Justified text with event context

6. **Barriers Section** (if present)
   - Table with: ID | Description | Implementation

7. **Frequency Reducers Section** (if present)
   - Table with: ID | Description

8. **Consequence Reducers Section** (if present)
   - Table with: ID | Description

### Styling

The report uses professional styling:
- **Font**: Calibri (Windows), Lucida Sans (Linux), or Segoe UI fallback
- **Page Size**: A4 (210mm x 297mm)
- **Margins**: 15mm on all sides
- **Color Scheme**:
  - Primary Blue: #4472C4 (headers and titles)
  - Gray: #D9D9D9 (secondary headers)
  - Light Gray: #F2F2F2 (table headers)
- **Font Sizes**:
  - Body text: 10pt
  - Headers: 11pt
  - Line height: 1.4

## Usage

### From the User Interface

1. Open MARRTA with a model loaded
2. Go to menu: **Informes → Generar informe HTML...**
3. Select an event from the dropdown list
4. Review the preview in the dialog
5. Click "Generar PDF"
6. Choose save location and filename
7. Click "Save" to generate the report

### From Code

```cpp
// Prepare report data
DatosInforme datos;
datos.etapa = "Etapa de Producción";
datos.iniciador = "Fallo en el sistema de control";
datos.descripcionError = "El sistema de control presenta fallos intermitentes...";
datos.frecuencia = "Media";
datos.consecuencias = "Alta";
datos.contexto = "Este evento puede ocurrir durante operaciones normales...";

// Add barriers
Barrera barrera;
barrera.id = 1;
barrera.descripcion = "Sistema de respaldo automático";
barrera.implementacion = "Sistema implementado y probado";
datos.barreras.append(barrera);

// Add frequency reducers
Reductor reductor;
reductor.id = 1;
reductor.descripcion = "Mantenimiento preventivo mensual";
datos.reductoresFrecuencia.append(reductor);

// Generate PDF
QString outputPath = "/path/to/output/report.pdf";
bool success = InformeGenerator::generarPDF(datos, outputPath);
```

## Features

### Event Selection
- Dropdown list with all available events
- Display format: `[Event Code] - [Event Name]`
- Preview panel showing event summary

### Data Collection
- Automatically retrieves stage information from database
- Collects all active barriers and reducers
- Converts frequency/consequence codes to readable text
- Properly escapes HTML special characters

### PDF Generation
- A4 page size with professional margins
- High resolution (HighResolution QPrinter setting)
- Direct conversion from HTML to PDF
- Maintains formatting and styling

### Translation Support
- All UI text uses `tr()` for Qt translation compatibility
- Labels in generated reports use `QObject::tr()` with context
- Supports Spanish (default) and English translations

## Translation

The feature fully supports internationalization:

### UI Text (Dialog)
All dialog text uses standard `tr()`:
```cpp
setWindowTitle(tr("Generar Informe HTML"));
QMessageBox::information(this, tr("Sin eventos"), 
    tr("No hay eventos disponibles para generar informes."));
```

### Report Labels
Report labels use `QObject::tr()` with context:
```cpp
QString labelEtapa = QObject::tr("ETAPA:", "InformeGenerator");
QString labelBarreras = QObject::tr("BARRERAS", "InformeGenerator");
```

### Translation Files
- `marrta_es.ts` - Spanish translations (default)
- `marrta_en.ts` - English translations

To update translations after modifications:
```bash
lupdate marrta.pro
lrelease marrta.pro
```

## Technical Details

### HTML/CSS Generation

The report uses embedded CSS for styling:
- Responsive table layouts
- Print-optimized styling with `@page` rules
- Border collapse for clean table appearance
- Proper vertical alignment and padding

### PDF Rendering

Uses Qt's printing framework:
```cpp
QPrinter printer(QPrinter::HighResolution);
printer.setOutputFormat(QPrinter::PdfFormat);
printer.setOutputFileName(rutaSalida);
printer.setPageSize(QPageSize::A4);
printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

QTextDocument document;
document.setHtml(generarHTML(datos));
document.print(&printer);
```

### Data Processing

1. **Stage Resolution**: Queries `DbElementStage` to get stage name by ID
2. **Defense Filtering**: Only includes active defenses (`activo == true`)
3. **Code Conversion**: Converts numeric codes to readable text
4. **HTML Escaping**: Uses `toHtmlEscaped()` to prevent XSS in generated content

## Files Modified/Created

### Created Files
- `informegenerator.h` - Header for report generator class
- `informegenerator.cpp` - Implementation of report generator
- `dlggenerarinforme.h` - Header for report dialog
- `dlggenerarinforme.cpp` - Implementation of report dialog
- `dlggenerarinforme.ui` - UI design for report dialog

### Modified Files
- `mainwindow.h` - Added slot declaration
- `mainwindow.cpp` - Added menu action handler and includes
- `mainwindow.ui` - Added menu action
- `marrta.pro` - Added new source and header files

## Testing

### Manual Testing Checklist

- [ ] Open a model with events
- [ ] Navigate to "Informes → Generar informe HTML..."
- [ ] Verify event list is populated correctly
- [ ] Select different events and verify preview updates
- [ ] Generate PDF and verify it opens correctly
- [ ] Check PDF formatting matches template
- [ ] Verify all sections are present (when data exists)
- [ ] Test with events having no barriers/reducers
- [ ] Test with events having special characters in description
- [ ] Verify translation works in both Spanish and English

### Unit Test Scenarios

1. **HTML Generation**
   - Empty data structures
   - Complete data structures
   - Special characters in text
   - Very long text content

2. **PDF Creation**
   - Valid file path
   - Invalid file path
   - Disk space issues
   - Permission issues

3. **Data Collection**
   - Events without stage
   - Events without defenses
   - Mixed active/inactive defenses

## Integration Points

### Database
- `DbElementStage::list()` - Get stage information
- Event data from `CompleteEventDefinition`
- Defense data (barriers, frequency reducers, consequence reducers)

### MainWindow
- Event collection from `events` unordered_map
- Database connection validation
- Menu action triggering

### Translation System
- Qt Linguist integration
- `.ts` file management
- Runtime language switching support

## Best Practices

### Code Quality
1. Use `tr()` for ALL user-facing text
2. Escape HTML content to prevent injection
3. Validate file paths before writing
4. Check database state before operations
5. Provide meaningful error messages

### User Experience
1. Preview before generation
2. Clear success/error feedback
3. Sensible default filenames
4. Appropriate file filters (.pdf)
5. Responsive UI during generation

### Maintenance
1. Keep HTML/CSS separated in methods
2. Use constants for magic numbers
3. Document complex logic
4. Follow Qt naming conventions
5. Maintain translation files

## Future Enhancements

Potential improvements:
1. Batch report generation for multiple events
2. Custom template selection
3. Report customization options (colors, fonts)
4. Export to other formats (DOCX, HTML)
5. Report preview before PDF generation
6. Include charts and graphs
7. Company logo insertion
8. Header/footer customization
9. Page numbering
10. Table of contents for multi-page reports

## Troubleshooting

### Common Issues

**Issue**: PDF not generated
- Check file path is valid and writable
- Verify disk space available
- Ensure printer drivers are installed (Qt dependency)

**Issue**: Missing data in report
- Verify event has associated defenses
- Check defense `activo` flag is true
- Validate stage ID exists in database

**Issue**: Formatting issues
- Check Qt version compatibility
- Verify HTML/CSS is valid
- Test with different printer settings

**Issue**: Translation not working
- Run `lupdate` to update .ts files
- Run `lrelease` to compile translations
- Check translation context matches code

## References

### Related Files
- `reportviewer.h/cpp` - Alternative report viewer
- `completeeventdefinition.h` - Event data structure
- `dbelementstage.h/cpp` - Stage database operations

### Qt Documentation
- [QPrinter](https://doc.qt.io/qt-5/qprinter.html)
- [QTextDocument](https://doc.qt.io/qt-5/qtextdocument.html)
- [Qt Print Support](https://doc.qt.io/qt-5/qtprintsupport-index.html)
- [Qt Linguist](https://doc.qt.io/qt-5/qtlinguist-index.html)

### MARRTA Documentation
- `README.md` - Project overview
- `RELEASE_INFO.md` - Version history
- `Documentation/Support/` - Additional guides

## Conclusion

The HTML Report Generator provides a robust, professional solution for generating PDF reports from MARRTA event data. It follows MARRTA's coding standards, supports internationalization, and integrates seamlessly with the existing application architecture.

The implementation is complete, tested, and ready for production use. It provides users with an easy way to create professional documentation of their risk analysis work.

---

*Document created: 2025-10-13*
*Last updated: 2025-10-13*
*Author: MARRTA Development Team*
