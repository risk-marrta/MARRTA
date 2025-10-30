# New Model Creation Wizard - UI Description

## Visual Layout Overview

The wizard uses a modern, clean interface similar to the import wizard, with a stacked widget design that presents one page at a time.

---

## Page 1: File Location Selection

```
┌─────────────────────────────────────────────────────────────┐
│ Asistente de Creación de Nuevo Modelo                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Paso 1: Ubicación del modelo                              │
│                                                             │
│  Seleccione dónde desea guardar el nuevo archivo de        │
│  modelo (.pdb).                                             │
│                                                             │
│  ┌────────────────────────────────────────┐  ┌───────────┐ │
│  │ Ruta del archivo .pdb...               │  │ Examinar..│ │
│  └────────────────────────────────────────┘  └───────────┘ │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                            ┌──────────┐  ┌──────────────┐  │
│                            │ Cancelar │  │  Siguiente   │  │
│                            └──────────┘  └──────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

**Features:**
- Clean, uncluttered layout
- Read-only text field showing selected path
- Browse button opens file save dialog
- Automatic .pdb extension handling

---

## Page 2: Basic Model Information

```
┌─────────────────────────────────────────────────────────────┐
│ Asistente de Creación de Nuevo Modelo                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Paso 2: Información básica del modelo                     │
│                                                             │
│  Ingrese la información básica del modelo.                 │
│                                                             │
│  Nombre del modelo:      ┌─────────────────────────────┐   │
│                          │ Ej: Acelerador Lineal       │   │
│                          └─────────────────────────────┘   │
│                                                             │
│  Fecha de inicio:        ┌──────────────┐ [▼]             │
│                          │ 2025-01-07   │                  │
│                          └──────────────┘                  │
│                                                             │
│  Técnica:                ┌─────────────────────────────┐   │
│                          │ Ej: Radioterapia            │   │
│                          └─────────────────────────────┘   │
│                                                             │
│  Analistas:              ┌─────────────────────────────┐   │
│                          │ Nombres de los analistas    │   │
│                          └─────────────────────────────┘   │
│                                                             │
│  Institución/Hospital:   ┌─────────────────────────────┐   │
│                          │ Nombre de la institución    │   │
│                          └─────────────────────────────┘   │
│                                                             │
│  Descripción:            ┌─────────────────────────────┐   │
│                          │ Descripción del modelo      │   │
│                          └─────────────────────────────┘   │
│                                                             │
│  Observaciones:          ┌─────────────────────────────┐   │
│                          │ Observaciones adicionales   │   │
│                          └─────────────────────────────┘   │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                  ┌──────────┐  ┌──────────┐  ┌──────────┐  │
│                  │  Atrás   │  │ Cancelar │  │Siguiente │  │
│                  └──────────┘  └──────────┘  └──────────┘  │
└─────────────────────────────────────────────────────────────┘
```

**Features:**
- Form layout with clear labels
- Date picker with calendar popup
- Multi-line text fields for longer content
- Placeholder text showing examples
- Only "Nombre" is required

---

## Page 3: Process Stages

### Initial View (Empty)
```
┌─────────────────────────────────────────────────────────────┐
│ Asistente de Creación de Nuevo Modelo                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Paso 3: Etapas del proceso                                │
│                                                             │
│  ¿Quiere añadir las etapas del proceso?                    │
│                                                             │
│  ┌──────────────┐  ┌───────────────┐                       │
│  │ Añadir Etapa │  │ Eliminar Etapa│                       │
│  └──────────────┘  └───────────────┘                       │
│                                                             │
│  ┌───────────────────────────────────────────────────────┐ │
│  │ Nombre                    │ Código (solo etapas)      │ │
│  ├───────────────────────────┼───────────────────────────┤ │
│  │                           │                           │ │
│  │                           │                           │ │
│  │                           │                           │ │
│  │                           │                           │ │
│  └───────────────────────────────────────────────────────┘ │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│                  │  Atrás   │  │ Cancelar │  │Crear modelo│ │
│                  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### With Stages and Substages
```
┌─────────────────────────────────────────────────────────────┐
│ Asistente de Creación de Nuevo Modelo                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Paso 3: Etapas del proceso                                │
│                                                             │
│  ¿Quiere añadir las etapas del proceso?                    │
│                                                             │
│  ┌──────────────┐  ┌───────────────┐                       │
│  │ Añadir Etapa │  │ Eliminar Etapa│                       │
│  └──────────────┘  └───────────────┘                       │
│                                                             │
│  ┌───────────────────────────────────────────────────────┐ │
│  │ Nombre                    │ Código (solo etapas)      │ │
│  ├───────────────────────────┼───────────────────────────┤ │
│  │ ▼ Control de calidad      │ CON                       │ │
│  │   ├─ Calibración diaria   │                           │ │
│  │   └─ Verificación output  │                           │ │
│  │ ▼ Previas al proceso      │ PRE                       │ │
│  │   ├─ Preparación paciente │                           │ │
│  │   └─ Verificación equipo  │                           │ │
│  │ ▶ Identificación paciente │ IDE                       │ │
│  └───────────────────────────────────────────────────────┘ │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                  ┌──────────┐  ┌──────────┐  ┌───────────┐ │
│                  │  Atrás   │  │ Cancelar │  │Crear modelo│ │
│                  └──────────┘  └──────────┘  └───────────┘ │
└─────────────────────────────────────────────────────────────┘
```

**Features:**
- Tree widget showing hierarchical structure
- Stages shown with name and code
- Substages nested under their parent stage
- Expandable/collapsible tree items
- Can select and delete items

### Dialog for Adding Stage
```
┌──────────────────────────────────┐
│ Añadir Etapa                     │
├──────────────────────────────────┤
│                                  │
│  Nombre de la etapa:             │
│  ┌────────────────────────────┐  │
│  │ Control de calidad         │  │
│  └────────────────────────────┘  │
│                                  │
│        ┌────────┐  ┌─────────┐   │
│        │ Cancel │  │   OK    │   │
│        └────────┘  └─────────┘   │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│ Añadir Etapa                     │
├──────────────────────────────────┤
│                                  │
│  Código de la etapa              │
│  (3 caracteres):                 │
│  ┌────────────────────────────┐  │
│  │ CON                        │  │
│  └────────────────────────────┘  │
│                                  │
│        ┌────────┐  ┌─────────┐   │
│        │ Cancel │  │   OK    │   │
│        └────────┘  └─────────┘   │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│ Subetapas                        │
├──────────────────────────────────┤
│                                  │
│  ¿Quiere añadir subetapas a     │
│  esta etapa?                     │
│                                  │
│        ┌────────┐  ┌─────────┐   │
│        │   No   │  │   Sí    │   │
│        └────────┘  └─────────┘   │
└──────────────────────────────────┘
```

### Dialog for Adding Substage
```
┌──────────────────────────────────┐
│ Añadir Subetapa                  │
├──────────────────────────────────┤
│                                  │
│  Nombre de la subetapa:          │
│  ┌────────────────────────────┐  │
│  │ Calibración diaria         │  │
│  └────────────────────────────┘  │
│                                  │
│        ┌────────┐  ┌─────────┐   │
│        │ Cancel │  │   OK    │   │
│        └────────┘  └─────────┘   │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│ Más Subetapas                    │
├──────────────────────────────────┤
│                                  │
│  ¿Quiere añadir más subetapas   │
│  a esta etapa?                   │
│                                  │
│        ┌────────┐  ┌─────────┐   │
│        │   No   │  │   Sí    │   │
│        └────────┘  └─────────┘   │
└──────────────────────────────────┘
```

---

## Page 4: Creation Progress

### During Creation
```
┌─────────────────────────────────────────────────────────────┐
│ Asistente de Creación de Nuevo Modelo                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Paso 4: Creación del modelo                               │
│                                                             │
│  Creando base de datos...                                  │
│                                                             │
│  ┌───────────────────────────────────────────────────────┐ │
│  │████████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ 40%│ │
│  └───────────────────────────────────────────────────────┘ │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                                           ┌──────────┐      │
│                                           │ Cancelar │      │
│                                           └──────────┘      │
└─────────────────────────────────────────────────────────────┘
```

### Success
```
┌─────────────────────────────────────────────────────────────┐
│ Asistente de Creación de Nuevo Modelo                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  Paso 4: Creación del modelo                               │
│                                                             │
│  ¡Modelo creado exitosamente!                              │
│                                                             │
│  ┌───────────────────────────────────────────────────────┐ │
│  │██████████████████████████████████████████████████ 100%│ │
│  └───────────────────────────────────────────────────────┘ │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│                                           ┌──────────┐      │
│                                           │  Cerrar  │      │
│                                           └──────────┘      │
└─────────────────────────────────────────────────────────────┘
```

---

## Design Principles

1. **Consistency**: Matches ImportWizard visual style
2. **Clarity**: Each page has clear title and instructions
3. **Progression**: Users always know where they are in the process
4. **Flexibility**: Can skip optional sections
5. **Feedback**: Progress bar and status messages keep users informed
6. **Spanish Language**: All UI text in Spanish as requested
7. **Validation**: Clear error messages prevent mistakes
8. **Accessibility**: Large buttons, clear labels, logical tab order

## Color Scheme
- Uses default Qt theme colors for consistency with existing MARRTA UI
- Headers use larger font size for emphasis
- Progress bar shows green for success
- Error messages would appear in red (if any)

## Responsive Behavior
- Window size: 700x500 pixels
- Resizable for users who need more space
- Scrollable text fields for long content
- Tree widget adjusts to show all items
