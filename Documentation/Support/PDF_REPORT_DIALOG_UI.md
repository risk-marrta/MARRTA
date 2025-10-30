# PDF Report Configuration Dialog - UI Design

## Dialog Layout

```
┌─────────────────────────────────────────────────────────────┐
│          Configuración del Informe PDF                       │
│                                                               │
│  Seleccione las secciones que desea incluir en el informe:   │
│                                                               │
│  ┌─ Secciones del Informe ──────────────────────────────┐   │
│  │                                                        │   │
│  │  ☐ Portada (disabled/grayed)                          │   │
│  │  ☐ Información del modelo (disabled/grayed)           │   │
│  │                                                        │   │
│  │  ☐ Secuencias de mayor riesgo                         │   │
│  │      ☐ RMA (Riesgo Muy Alto)                          │   │
│  │      ☐ RA (Riesgo Alto)                               │   │
│  │      ☐ RM (Riesgo Medio)                              │   │
│  │      ☐ RB (Riesgo Bajo)                               │   │
│  │                                                        │   │
│  │  ☐ Listado completo de secuencias                     │   │
│  │  ┌────────────────────────────────────────────────┐  │   │
│  │  │ ☐ TODAS LAS ETAPAS                             │  │   │
│  │  │   ☐ Etapa 1                                    │  │   │
│  │  │     ☐ Subetapa 1.1                             │  │   │
│  │  │     ☐ Subetapa 1.2                             │  │   │
│  │  │   ☐ Etapa 2                                    │  │   │
│  │  │     ☐ Subetapa 2.1                             │  │   │
│  │  │     ☐ Subetapa 2.2                             │  │   │
│  │  │   ☐ Etapa 3                                    │  │   │
│  │  │     ...                                         │  │   │
│  │  └────────────────────────────────────────────────┘  │   │
│  │                                                        │   │
│  │  ☐ Análisis de defensas (disabled/grayed)             │   │
│  │  ☐ Listado de iniciadores (disabled/grayed)           │   │
│  │  ☐ Listado de defensas (disabled/grayed)              │   │
│  │                                                        │   │
│  └────────────────────────────────────────────────────────┘   │
│                                                               │
│                                    [  OK  ]  [  Cancel  ]     │
└─────────────────────────────────────────────────────────────┘
```

## Interaction Behavior

### Checkbox Hierarchies

#### 1. Secuencias de mayor riesgo (High-Risk Sequences)
- Parent checkbox enables/disables child checkboxes
- When unchecked:
  - All child checkboxes (RMA, RA, RM, RB) are disabled and unchecked
- When checked:
  - Child checkboxes become enabled
  - User must select at least one risk level

#### 2. Listado completo de secuencias (Complete Listing)
- Parent checkbox enables/disables the tree widget
- When unchecked:
  - Tree widget is disabled
  - All tree items are unchecked
- When checked:
  - Tree widget becomes enabled
  - User can select stages/substages

#### 3. Stage/Substage Tree
- **TODAS LAS ETAPAS** (root):
  - When checked: Selects all stages and substages
  - When unchecked: Deselects all stages and substages
  
- **Stage items**:
  - When checked: Selects all substages under this stage
  - When unchecked: Deselects all substages under this stage
  - Shows partial check state when some (but not all) substages are selected
  
- **Substage items**:
  - When checked/unchecked: Updates parent stage checkbox state
  - If all substages are checked → parent becomes checked
  - If no substages are checked → parent becomes unchecked
  - If some substages are checked → parent shows partial state

## Validation Rules

When user clicks OK, the dialog validates:

1. **At least one section selected**:
   - Either "Secuencias de mayor riesgo" OR "Listado completo de secuencias" must be checked
   - Error message: "Debe seleccionar al menos una sección: 'Secuencias de mayor riesgo' o 'Listado completo de secuencias'."

2. **Risk level validation**:
   - If "Secuencias de mayor riesgo" is selected, at least one risk level (RMA, RA, RM, RB) must be checked
   - Error message: "Debe seleccionar al menos un nivel de riesgo (RMA, RA, RM o RB)."

3. **Stage/substage validation**:
   - If "Listado completo" is selected, at least one substage must be checked
   - Error message: "Debe seleccionar al menos una etapa o subetapa."

## UI States

### Initial State
- All checkboxes unchecked
- Risk level checkboxes disabled (parent unchecked)
- Tree widget disabled (parent unchecked)
- Disabled sections (Portada, Información, etc.) are grayed out

### After User Interaction

**Example 1: High-Risk Report**
```
☑ Secuencias de mayor riesgo
    ☑ RMA (Riesgo Muy Alto)
    ☑ RA (Riesgo Alto)
    ☐ RM (Riesgo Medio)
    ☐ RB (Riesgo Bajo)
☐ Listado completo de secuencias (tree disabled)
```

**Example 2: Complete Listing**
```
☐ Secuencias de mayor riesgo (children disabled)
☑ Listado completo de secuencias
    ▣ TODAS LAS ETAPAS (partial - not all checked)
      ☑ Etapa 1
        ☑ Subetapa 1.1
        ☑ Subetapa 1.2
      ▣ Etapa 2 (partial - not all checked)
        ☑ Subetapa 2.1
        ☐ Subetapa 2.2
```

**Example 3: Both Sections**
```
☑ Secuencias de mayor riesgo
    ☑ RMA (Riesgo Muy Alto)
    ☐ RA (Riesgo Alto)
    ☐ RM (Riesgo Medio)
    ☐ RB (Riesgo Bajo)
☑ Listado completo de secuencias
    ☑ TODAS LAS ETAPAS
      ☑ Etapa 1
        ☑ Subetapa 1.1
        ☑ Subetapa 1.2
      ☑ Etapa 2
        ☑ Subetapa 2.1
        ☑ Subetapa 2.2
```

## Visual Mockup Symbols

```
☐ - Unchecked checkbox (enabled)
☑ - Checked checkbox (enabled)
▣ - Partially checked checkbox (some children checked)
⊡ - Unchecked checkbox (disabled/grayed)
```

## Keyboard Navigation

- Tab: Move between controls
- Space: Toggle checkbox
- Arrow keys: Navigate tree
- Enter: Accept dialog (if validation passes)
- Escape: Cancel dialog

## Dialog Size

- Width: 600 pixels
- Height: 700 pixels
- Resizable: Yes (tree widget can expand)

## Color Scheme

- Disabled text: Gray (#888888)
- Normal text: Black (#000000)
- Selected tree item: System highlight color
- Partial checkbox: System partial check indicator

## Translation Keys

All text in the dialog uses `tr()` for translation:

- Dialog title: `tr("Configurar Informe")`
- Main title: `tr("Configuración del Informe PDF")`
- Description: `tr("Seleccione las secciones que desea incluir en el informe:")`
- Group box: `tr("Secciones del Informe")`
- Checkboxes use appropriate `tr()` calls for all labels
- Tree root: `tr("TODAS LAS ETAPAS")`
- Tree header: `tr("Etapas y Subetapas")`

## Accessibility

- All controls have proper labels
- Tab order is logical (top to bottom)
- Keyboard navigation fully supported
- Screen reader friendly with proper labels
- High contrast mode compatible
