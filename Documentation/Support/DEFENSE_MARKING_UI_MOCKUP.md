# Defense Marking UI Changes - Visual Mockup

## Menu Structure Changes

This document provides a visual representation of the UI changes made to the defense marking/unmarking functionality.

## Before (Old Implementation)

```
┌─────────────────────────────────────────┐
│ Modelo                              [▼] │
├─────────────────────────────────────────┤
│ Información                             │
│ ─────────────────────────────────────── │
│ Resumen                                 │
│ Generar informe                         │
│ ─────────────────────────────────────── │
│ Generar informe HTML                    │
│ ─────────────────────────────────────── │
│ Guardar los cambios                     │
│ ─────────────────────────────────────── │
│ ✓ Marcar todas las defensas             │ ← Single action (BUGGY)
│ ✗ Desmarcar todas las defensas          │ ← Single action (BUGGY)
│ ─────────────────────────────────────── │
│ ✓ Marcar iniciadores aplicables         │
│ ✗ Marcar iniciadores NO aplicables      │
│ ─────────────────────────────────────── │
│ Más información                      ▶  │
└─────────────────────────────────────────┘
```

## After (New Implementation)

```
┌─────────────────────────────────────────────────────────────────┐
│ Modelo                                                      [▼] │
├─────────────────────────────────────────────────────────────────┤
│ Información                                                     │
│ ─────────────────────────────────────────────────────────────── │
│ Resumen                                                         │
│ Generar informe                                                 │
│ ─────────────────────────────────────────────────────────────── │
│ Generar informe HTML                                            │
│ ─────────────────────────────────────────────────────────────── │
│ Guardar los cambios                                             │
│ ─────────────────────────────────────────────────────────────── │
│ ✓ Marcar defensas                                            ▶  │ ← New submenu
│ ✗ Desmarcar defensas                                         ▶  │ ← New submenu
│ ─────────────────────────────────────────────────────────────── │
│ ✓ Marcar iniciadores aplicables                                 │
│ ✗ Marcar iniciadores NO aplicables                              │
│ ─────────────────────────────────────────────────────────────── │
│ Más información                                              ▶  │
└─────────────────────────────────────────────────────────────────┘
```

### Submenu: "Marcar defensas" (expanded)

```
┌──────────────────────────────────────────────────────────────┐
│ ✓ Marcar defensas                                         ▶  │
│   ┌──────────────────────────────────────────────────────┐   │
│   │ ✓ Marcar todas las defensas                          │   │ ← All defense types
│   │ ────────────────────────────────────────────────────  │   │
│   │ ✓ Marcar todos los reductores de frecuencia          │   │ ← Only frequency reducers
│   │ ✓ Marcar todas las barreras                          │   │ ← Only barriers
│   │ ✓ Marcar todos los reductores de consecuencia        │   │ ← Only consequence reducers
│   └──────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────┘
```

### Submenu: "Desmarcar defensas" (expanded)

```
┌──────────────────────────────────────────────────────────────┐
│ ✗ Desmarcar defensas                                      ▶  │
│   ┌──────────────────────────────────────────────────────┐   │
│   │ ✗ Desmarcar todas las defensas                       │   │ ← All defense types
│   │ ────────────────────────────────────────────────────  │   │
│   │ ✗ Desmarcar todos los reductores de frecuencia       │   │ ← Only frequency reducers
│   │ ✗ Desmarcar todas las barreras                       │   │ ← Only barriers
│   │ ✗ Desmarcar todos los reductores de consecuencia     │   │ ← Only consequence reducers
│   └──────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────┘
```

## Icon Legend

- ✓ = checked.svg (green checkmark icon)
- ✗ = stop.svg (red stop icon)
- ▶ = Indicates submenu (Qt automatic)

## Defense Types

The system recognizes three types of defenses:

1. **Reductores de Frecuencia** (Frequency Reducers)
   - Defenses that reduce the frequency/likelihood of an event occurring
   - Corresponds to `TipoDefensa::ReductoresFrecuencia` (enum value 0)

2. **Barreras** (Barriers)
   - Defenses that act as barriers to prevent event progression
   - Corresponds to `TipoDefensa::Barreras` (enum value 1)

3. **Reductores de Consecuencia** (Consequence Reducers)
   - Defenses that reduce the consequences/impact of an event
   - Corresponds to `TipoDefensa::ReductoresConsecuencia` (enum value 2)

## User Workflow Examples

### Example 1: Mark only barriers
1. User opens "Modelo" menu
2. Hovers over "✓ Marcar defensas"
3. Clicks "Marcar todas las barreras"
4. System marks all barriers in all events as active
5. System refreshes display if an event is selected
6. User saves model to persist changes

### Example 2: Unmark all defenses
1. User opens "Modelo" menu
2. Hovers over "✗ Desmarcar defensas"
3. Clicks "Desmarcar todas las defensas"
4. System unmarks ALL defenses (all types) in all events
5. System refreshes display if an event is selected
6. User saves model to persist changes

### Example 3: Selective marking
1. User unmarks all defenses
2. User marks only frequency reducers
3. User marks only barriers
4. Result: Only frequency reducers and barriers are active, consequence reducers remain inactive

## Benefits Over Old Implementation

1. **More Control**: Users can now mark/unmark by specific defense type
2. **Better Organization**: Submenu structure groups related actions
3. **Clearer Intent**: Icon and text clearly indicate mark/unmark action
4. **Fixed Logic**: No more state inversion bug
5. **Consistent Pattern**: Follows same structure as "Más información" submenu

## Implementation Notes

- Each submenu has a separator after the "all defenses" option for visual clarity
- Each action maintains the same icon style (checked.svg for mark, stop.svg for unmark)
- Menu structure defined in `mainwindow.ui` XML format
- Qt Designer can be used to further customize appearance if needed

## Database Impact

When marking/unmarking defenses:
1. In-memory state is updated immediately (`defensa.active = marcar`)
2. Change flag is set (`defensa.changed = true`)
3. Display is refreshed if an event is selected
4. Changes are persisted to database when user saves model
5. Database update uses prepared statements for safety
6. Only changed defenses are updated in database (optimization)

## Related UI Elements

These defense lists are displayed in the main window:
- **lReductoresFrecuencia**: ListView for frequency reducers
- **lBarreras**: ListView for barriers  
- **lReductoresConsecuencia**: ListView for consequence reducers

Each list shows defenses for the currently selected event with checkboxes indicating active state.
