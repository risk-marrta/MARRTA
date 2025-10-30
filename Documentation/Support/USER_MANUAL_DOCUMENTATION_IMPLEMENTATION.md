# User Manual Documentation Implementation

## Overview
This document describes the implementation of the comprehensive user manual documentation dialog for MARRTA, replacing the simple "coming soon" message box with a full-featured help system.

## Implementation Details

### Files Created
1. **usermanualdocumentation.h**: Header file for UserManualDocumentation dialog class
2. **usermanualdocumentation.cpp**: Implementation with complete user manual content

### Files Modified
1. **mainwindow.cpp**: 
   - Added include for usermanualdocumentation.h
   - Modified `on_actionAyuda_triggered()` to open UserManualDocumentation dialog
2. **marrta.pro**: Added new files to SOURCES and HEADERS sections
3. **RELEASE_INFO.md**: Documented changes at the top of the file

## Dialog Structure

### UI Layout
The UserManualDocumentation dialog uses a two-panel layout:
- **Left Panel**: QTreeWidget navigation tree with sections and subsections
- **Right Panel**: QTextBrowser content display with HTML formatting
- **Bottom**: Close button

### Navigation Tree Structure
```
├─ 1. Introducción
│  └─ 1.1 Elementos de la pantalla principal
├─ 2. Proceso de análisis
├─ 3. Menú Archivo
├─ 4. Menú Modelo
├─ 5. Menú Herramientas
├─ 6. Menú Administración
└─ 7. Menú Ayuda
```

## Documentation Content

### Section 1: Introducción
- General introduction to MARRTA
- System requirements
- Capabilities and functionalities overview
- **Subsection 1.1**: Detailed description of main screen elements including:
  - Menu bar
  - Stage navigation panel (left)
  - Event list panel (center)
  - Filter controls (top)
  - Event details panel (right)
  - Defense lists (right bottom)
  - Status bar (bottom)

### Section 2: Proceso de análisis
- Complete analysis workflow
- Step-by-step guide:
  1. Load a model
  2. Review and mark initiator events as applicable
  3. Configure applicable defenses
  4. Understand initiator vs. sequence differences
  5. Review results
- Detailed explanation of the difference between:
  - **Initiator Event** (base): F and C with P=max (worst case)
  - **Sequence** (with defenses): F, P, C calculated with active defenses
- Comparison table between Summary view and Detailed HTML report

### Section 3: Menú Archivo
- Explanation of all File menu options
- **Special emphasis on Import from Excel** with complete format specification:
  - Sheet 1: Etapas (Stages) - columns and example data
  - Sheet 2: Subetapas (Substages) - columns and example data
  - Sheet 3: Eventos (Events) - columns and allowed values
  - Sheet 4: Defensas (Defenses) - columns and types
  - Sheet 5: Defensas por Evento (Defenses by Event) - relationships
- Clarification on "aplicable" terminology (applicable to department) vs. "completado" in database
- Tables showing required columns, data types, and examples for each sheet

### Section 4: Menú Modelo
- Model menu options explanation
- **Critical clarification**: 
  - Mark/unmark initiator operations affect only VISIBLE (filtered) events
  - Mark/unmark defense operations affect ALL defenses (global)
- Note on using Administration → Defenses for granular defense filtering

### Section 5: Menú Herramientas
- Description of Tools menu options:
  - Summary table view
  - HTML report generation
  - Flowchart generation
  - JavaScript scripting engine
  - Python scripting engine (requires Python 3.12)
- API objects available in each scripting engine

### Section 6: Menú Administración
- Description of three management windows:
  1. **Stages**: Manage stages and substages
  2. **Initiator Events**: Create, edit, delete, and filter events
  3. **Defenses**: Manage defenses with granular filtering and bulk operations

### Section 7: Menú Ayuda
- Help menu description:
  - Check for updates
  - Check for new models
  - Send comments/suggestions (GitHub integration)
  - Report bugs (GitHub issues)
  - Legal notice and license information
  - About MARRTA

## Technical Implementation

### Key Design Decisions

1. **HTML Content**: All documentation content is in HTML format for rich formatting including:
   - Headers (h2, h3, h4)
   - Lists (ul, ol)
   - Tables with borders and styling
   - Inline styling for emphasis and colors
   - Code blocks with pre tags

2. **Translation Support**: All user-facing strings use `tr()` for proper internationalization:
   - Window title
   - Section headers
   - Navigation tree labels
   - All content text
   - Button labels

3. **Structure**: Documentation items stored in QList<DocumentationItem> where each item contains:
   - title: Section title for matching with tree selection
   - content: HTML content to display

4. **Navigation**: QTreeWidget::currentItemChanged signal connected to onSectionChanged() slot for instant navigation

### Code Pattern

```cpp
// Adding a documentation section
QTreeWidgetItem *sectionItem = new QTreeWidgetItem(sectionTree);
sectionItem->setText(0, tr("Section Title"));
addDocumentationSection(tr("Section Title"),
    tr("<h2>Section Title</h2>"
    "<p>Content here...</p>"));

// For subsections
QTreeWidgetItem *subsectionItem = new QTreeWidgetItem(parentItem);
subsectionItem->setText(0, tr("Subsection Title"));
addDocumentationSection(tr("Subsection Title"),
    tr("<h2>Subsection Title</h2>"
    "<p>Content here...</p>"));
```

## Usage

### User Experience
1. User clicks **Ayuda → Ayuda** in the menu
2. UserManualDocumentation dialog opens as a modal window
3. Navigation tree shows all sections on the left
4. User clicks on any section to view its content
5. Content displays in HTML browser on the right with rich formatting
6. User can resize the window and adjust the splitter
7. User clicks "Cerrar" to close the dialog

### Developer Experience
To add or modify documentation sections:
1. Locate `populateDocumentation()` method in usermanualdocumentation.cpp
2. Add/modify QTreeWidgetItem for navigation
3. Add/modify `addDocumentationSection()` call with title and HTML content
4. Ensure all strings use `tr()` for translation
5. Test by running the application and opening Help → Ayuda

## Translation

All strings in the documentation are wrapped with `tr()` to ensure they can be translated. To update translations:

```bash
# Extract translatable strings
lupdate marrta.pro

# Edit translations with Qt Linguist
linguist marrta_es.ts
linguist marrta_en.ts

# Compile translations
lrelease marrta.pro
```

## Future Enhancements

Potential improvements for future versions:
1. Add screenshots/images to documentation sections
2. Add search functionality to find content across all sections
3. Add "Print" button to export documentation to PDF
4. Add "Export to HTML" to save documentation as standalone file
5. Link to online documentation for latest updates
6. Add video tutorials or animations for complex procedures
7. Context-sensitive help (F1 key) from different parts of the application

## Testing

Since Qt is not installed in the CI environment, testing must be done manually:

### Manual Test Steps
1. Build MARRTA with qmake and make
2. Run the application
3. Open **Ayuda → Ayuda** menu
4. Verify dialog opens with correct title
5. Verify all 7 sections appear in the navigation tree
6. Click each section and verify:
   - Content displays correctly
   - HTML formatting is proper
   - Tables render correctly
   - Lists and headers display properly
7. Verify subsection 1.1 appears under section 1
8. Resize the window and verify layout adjusts correctly
9. Adjust the splitter and verify it works smoothly
10. Click "Cerrar" to close dialog
11. Repeat test with different language settings (Spanish/English)

## Issue Resolution

This implementation resolves the issue [Documentación] Manual de usuario (i) by:
- ✅ Creating inline documentation dialog (similar to scripting documentation)
- ✅ Appearing in Help → Ayuda menu replacing simple message box
- ✅ Including all 7 requested sections with detailed content
- ✅ Special detail on Import from Excel with table examples
- ✅ Clarification on "aplicable" terminology
- ✅ Explanation of initiator vs. sequence differences
- ✅ Description of all menu options
- ✅ Proper translation support with tr()
