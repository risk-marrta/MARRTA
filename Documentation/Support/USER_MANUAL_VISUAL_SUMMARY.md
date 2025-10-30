# User Manual Documentation - Visual Summary

## Overview
This document provides a visual representation of the User Manual Documentation dialog that was implemented to replace the simple "coming soon" message box.

## Dialog Appearance

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ Manual de Usuario - MARRTA                                            [×] │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌────────────────────┬──────────────────────────────────────────────────┐ │
│  │ Índice             │                                                  │ │
│  │                    │                                                  │ │
│  │ ▼ 1. Introducción  │  Introducción General                           │ │
│  │   ▸ 1.1 Elementos  │                                                  │ │
│  │ ▸ 2. Proceso de    │  MARRTA (Metodología de Análisis de Riesgos     │ │
│  │     análisis       │  y Rendimiento en Radioterapia Avanzada) es     │ │
│  │ ▸ 3. Menú Archivo  │  una herramienta integral para el análisis...   │ │
│  │ ▸ 4. Menú Modelo   │                                                  │ │
│  │ ▸ 5. Menú          │  Requisitos del Sistema                         │ │
│  │     Herramientas   │  • Sistema operativo: Windows, Linux o macOS    │ │
│  │ ▸ 6. Menú          │  • Qt 6.10 o superior                           │ │
│  │     Administración │  • SQLite (incluido)                            │ │
│  │ ▸ 7. Menú Ayuda    │  • Python 3.12 (opcional, para scripting)      │ │
│  │                    │                                                  │ │
│  │                    │  Posibilidades y Funcionalidades                │ │
│  │                    │  • Gestión de modelos de riesgo                 │ │
│  │                    │  • Análisis de sucesos iniciadores              │ │
│  │                    │  • Gestión de defensas                          │ │
│  │                    │  • Cálculo automático de riesgo                 │ │
│  │                    │  • Informes detallados                          │ │
│  │                    │  • Importación/Exportación Excel                │ │
│  │                    │  • Motores de scripting JavaScript y Python     │ │
│  │                    │  • Gestión de etapas                            │ │
│  │                    │  • Filtrado avanzado                            │ │
│  │                    │                                                  │ │
│  │                    │                                                  │ │
│  │                    │  [Scrollable content area]                      │ │
│  │                    │                                                  │ │
│  └────────────────────┴──────────────────────────────────────────────────┘ │
│                                                                             │
│                                                          [Cerrar]           │
└─────────────────────────────────────────────────────────────────────────────┘
```

## Navigation Tree Structure

When user expands all sections:

```
Índice
├─ ▼ 1. Introducción
│  └─ • 1.1 Elementos de la pantalla principal
├─ • 2. Proceso de análisis
├─ • 3. Menú Archivo
├─ • 4. Menú Modelo
├─ • 5. Menú Herramientas
├─ • 6. Menú Administración
└─ • 7. Menú Ayuda
```

## Content Sections Preview

### Section 1: Introducción
- General introduction to MARRTA
- System requirements bullet list
- Capabilities and functionalities (10 main features)

### Section 1.1: Elementos de la pantalla principal
Detailed description of UI components:
- Menu bar at top
- Stage navigation panel (left side)
- Event list panel (center)
- Filter controls (top bar)
- Event details panel (right side)
- Defense lists (3 sections at right bottom)
- Status bar at bottom
- Color coding explanation for risk levels

### Section 2: Proceso de análisis
Step-by-step analysis workflow:
1. Load a model (Archivo → Abrir)
2. Review and mark initiator events as "applicable"
3. Configure defenses (activate relevant ones)
4. Understand the difference between:
   - **Suceso Iniciador**: F and C with P=max (worst case)
   - **Secuencia**: F, P, C calculated with active defenses
5. Review results (Summary vs. Detailed Report comparison table)

### Section 3: Menú Archivo
Comprehensive File menu documentation:
- New model wizard
- Open existing file
- Save and Save As
- **Import from Excel** (detailed format specification):
  - 5 sheets with complete column definitions
  - Example tables for each sheet
  - Data type specifications
  - Clarification: "aplicable" = applicable to department
- Export to Excel
- Close and Exit

### Section 4: Menú Modelo
Model menu options:
- Mark/unmark initiators (affects visible/filtered events only)
- Mark/unmark defenses (affects ALL defenses globally)
- Defense type filtering (Frequency Reducers, Barriers, Consequence Reducers)
- Note: Use Administration → Defenses for granular filtering

### Section 5: Menú Herramientas
Tools menu description:
- Summary table (interactive view)
- Generate HTML report (detailed documentation)
- Generate flowchart (graphical representation)
- JavaScript scripting engine (API objects listed)
- Python scripting engine (requires Python 3.12, API modules listed)

### Section 6: Menú Administración
Administration menu with 3 management windows:
1. **Stages**: Create, edit, delete stages and substages
2. **Initiator Events**: Full CRUD operations, filtering, bulk actions
3. **Defenses**: Manage defenses, associate with events, bulk operations

### Section 7: Menú Ayuda
Help menu resources:
- User manual (this dialog)
- Check for updates (version checking)
- Check for new models (download latest models)
- Send comments/suggestions (GitHub integration)
- Report bugs (GitHub issues)
- Legal notice and license information
- About MARRTA

## Content Formatting Features

The documentation uses rich HTML formatting:

### Headers
- `<h2>` for main section titles
- `<h3>` for subsections
- `<h4>` for sub-subsections

### Lists
- Unordered lists (`<ul>`) for feature lists
- Ordered lists (`<ol>`) for sequential steps
- Nested lists for hierarchical information

### Tables
Used extensively in Section 3 (Import from Excel):
```html
<table border='1' cellpadding='5' cellspacing='0' style='border-collapse: collapse;'>
<tr style='background-color: #e3f2fd;'>
  <th>Column</th><th>Description</th><th>Example</th>
</tr>
<tr><td>Code</td><td>Unique code</td><td>E001</td></tr>
...
</table>
```

### Emphasis
- `<b>` for bold text (key terms, field names)
- `<p>` for paragraphs
- Color-coded text for risk levels (inline styles)

### Code Blocks
- `<pre>` tags with background color for file structure examples

## User Interaction Flow

1. **Open Dialog**: Menu → Ayuda → Ayuda
2. **Navigate**: Click on any section in the tree
3. **Read**: Content displays instantly in the browser pane
4. **Scroll**: If content is long, scroll within the browser pane
5. **Resize**: Drag window borders to resize
6. **Adjust**: Drag splitter between tree and content to adjust proportions
7. **Close**: Click "Cerrar" button or X to close

## Accessibility Features

- Keyboard navigation in tree widget
- Scrollable content for long sections
- Resizable window and splitter
- High contrast for table headers
- Clear visual hierarchy with headers
- Descriptive section titles

## Translation Support

All strings are wrapped with `tr()`:
- Window title: `tr("Manual de Usuario - MARRTA")`
- Tree header: `tr("Índice")`
- Button: `tr("Cerrar")`
- All section titles and content use `tr()`

To support English:
```bash
lupdate marrta.pro  # Extract strings
linguist marrta_en.ts  # Translate to English
lrelease marrta.pro  # Compile
```

## Comparison: Before vs. After

### Before
```
┌────────────────────────────┐
│ Ayuda                  [×] │
├────────────────────────────┤
│                            │
│  La documentación de ayuda │
│  estará disponible         │
│  próximamente.             │
│                            │
│  Por favor, consulte la    │
│  documentación en el       │
│  repositorio de GitHub:    │
│  https://github.com/       │
│  risk-marrta/MARRTA        │
│                            │
│           [ OK ]           │
└────────────────────────────┘
```

### After
```
┌─────────────────────────────────────────────────┐
│ Manual de Usuario - MARRTA            [×]      │
├─────────────────────────────────────────────────┤
│  ┌────────┬──────────────────────────────────┐ │
│  │ Tree   │ Rich HTML content with:          │ │
│  │ Nav    │ • 7 comprehensive sections       │ │
│  │ with   │ • Detailed procedures            │ │
│  │ 7      │ • Tables and examples            │ │
│  │ main   │ • Step-by-step guides            │ │
│  │ items  │ • Complete Excel format specs    │ │
│  │        │ • Menu option descriptions       │ │
│  └────────┴──────────────────────────────────┘ │
│                                    [Cerrar]     │
└─────────────────────────────────────────────────┘
```

## Benefits of New Implementation

1. **Comprehensive**: All aspects of MARRTA covered in detail
2. **Organized**: Logical structure with 7 main sections
3. **Searchable**: Tree navigation for quick access
4. **Professional**: Rich HTML formatting with tables
5. **Translatable**: Full i18n support with tr()
6. **Self-contained**: No external dependencies
7. **Maintainable**: Easy to add/modify sections
8. **User-friendly**: Intuitive navigation and layout
9. **Detailed**: Special focus on complex features (Excel import)
10. **Contextual**: Explains concepts and workflows, not just UI

## Testing Checklist

- [ ] Dialog opens from Help → Ayuda
- [ ] Window title is correct
- [ ] Tree shows 7 main sections
- [ ] Section 1 has subsection 1.1
- [ ] All sections display content when clicked
- [ ] HTML formatting renders correctly
- [ ] Tables display properly
- [ ] Lists are formatted correctly
- [ ] Bold and emphasis work
- [ ] Window is resizable
- [ ] Splitter is draggable
- [ ] Close button works
- [ ] Spanish translations display correctly
- [ ] (Optional) English translations work after lupdate/lrelease
