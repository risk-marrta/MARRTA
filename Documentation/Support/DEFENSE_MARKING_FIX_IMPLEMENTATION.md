# Defense Marking/Unmarking Fix Implementation Summary

## Overview

This document describes the implementation of the fix for the defense marking/unmarking functionality in MARRTA. The issue was that the mark/unmark all defenses options were inverting the state instead of properly setting all defenses to the desired state.

## Problem Statement

The original implementation had a logic error in `on_actionMarcarDefensas_triggered()` and `on_actionDesmarcarDefensas_triggered()`:

```cpp
// OLD BUGGY CODE
for (auto &defensa : event.reductoresFrecuencias) {
    if (!defensa.active) {  // Only changes if NOT active
        defensa.active = true;
        defensa.changed = true;
    }
}
```

This meant:
- If a defense was already marked, clicking "Mark all" would NOT mark it (skipped)
- If a defense was unmarked, clicking "Mark all" would mark it
- Result: The function was **inverting** states instead of ensuring all are marked

## Solution

Following the pattern established in PR #181 for marking initiators, the solution includes:

1. **Helper Function**: Created `markAllDefenses(int tipoDefensa, bool marcar)` that properly sets the state
2. **Submenu Structure**: Converted single menu items to submenus with 4 options each
3. **8 Slot Functions**: One for each combination of mark/unmark and defense type

### Helper Function Logic

```cpp
void MainWindow::markAllDefenses(int tipoDefensa, bool marcar)
{
    for(auto &eventsInStages : events) {
        auto &eventsInStage = eventsInStages.second;

        for (auto &event : eventsInStage) {
            // Process frequency reducers if requested or if all defenses requested
            if (tipoDefensa == -1 || tipoDefensa == static_cast<int>(ReductoresFrecuencia)) {
                for (auto &defensa : event.reductoresFrecuencias) {
                    if (defensa.active != marcar) {  // CORRECT: Sets to desired state
                        defensa.active = marcar;
                        defensa.changed = true;
                    }
                }
            }
            
            // Similar logic for barriers and consequence reducers...
        }
    }

    // Refresh display if an event is selected
    if ((actualStageIndex != "") && (actualCompleteEvent != nullptr)) {
        toggleActualEvent();
    }
}
```

Key differences from buggy code:
- Uses `defensa.active != marcar` instead of `!defensa.active`
- Properly sets to desired state: `defensa.active = marcar`
- Supports filtering by defense type with `tipoDefensa` parameter

### Defense Type Parameter

The `tipoDefensa` parameter uses:
- `-1` = All defense types
- `0` = `ReductoresFrecuencia` (Frequency Reducers)
- `1` = `Barreras` (Barriers)
- `2` = `ReductoresConsecuencia` (Consequence Reducers)

## UI Structure

### Before

```
Modelo Menu
├── ...
├── Marcar todas las defensas
├── Desmarcar todas las defensas
├── ...
```

### After

```
Modelo Menu
├── ...
├── Marcar defensas ▶
│   ├── Marcar todas las defensas
│   ├── ───────────────────────
│   ├── Marcar todos los reductores de frecuencia
│   ├── Marcar todas las barreras
│   └── Marcar todos los reductores de consecuencia
├── Desmarcar defensas ▶
│   ├── Desmarcar todas las defensas
│   ├── ───────────────────────
│   ├── Desmarcar todos los reductores de frecuencia
│   ├── Desmarcar todas las barreras
│   └── Desmarcar todos los reductores de consecuencia
├── ...
```

## Files Modified

### mainwindow.h
- Removed old slot declarations:
  - `on_actionMarcarDefensas_triggered()`
  - `on_actionDesmarcarDefensas_triggered()`
- Added 8 new slot declarations:
  - `on_actionMarcarTodasDefensas_triggered()`
  - `on_actionMarcarReductoresFrecuencia_triggered()`
  - `on_actionMarcarBarreras_triggered()`
  - `on_actionMarcarReductoresConsecuencia_triggered()`
  - `on_actionDesmarcarTodasDefensas_triggered()`
  - `on_actionDesmarcarReductoresFrecuencia_triggered()`
  - `on_actionDesmarcarBarreras_triggered()`
  - `on_actionDesmarcarReductoresConsecuencia_triggered()`
- Added helper function declaration:
  - `void markAllDefenses(int tipoDefensa, bool marcar);`

### mainwindow.cpp
- Replaced old buggy functions with helper function and 8 slot functions
- Each slot function:
  1. Checks if a model is loaded
  2. Calls `markAllDefenses()` with appropriate parameters
  3. No duplication thanks to helper function

### mainwindow.ui
- Converted flat menu items to submenus:
  - Created `menuMarcarDefensas` submenu with icon
  - Created `menuDesmarcarDefensas` submenu with icon
- Removed old actions:
  - `actionMarcarDefensas`
  - `actionDesmarcarDefensas`
- Added 8 new actions:
  - `actionMarcarTodasDefensas`
  - `actionMarcarReductoresFrecuencia`
  - `actionMarcarBarreras`
  - `actionMarcarReductoresConsecuencia`
  - `actionDesmarcarTodasDefensas`
  - `actionDesmarcarReductoresFrecuencia`
  - `actionDesmarcarBarreras`
  - `actionDesmarcarReductoresConsecuencia`
- Each action has appropriate icon and text

## Testing

The implementation should be tested by:

1. Loading a PRAXIS model with events and defenses
2. Marking some defenses manually
3. Using "Marcar todas las defensas" - verify all become marked
4. Using "Desmarcar todas las barreras" - verify only barriers become unmarked
5. Using "Marcar todos los reductores de frecuencia" - verify only frequency reducers become marked
6. Saving the model and verifying changes persist in database

## Benefits

1. **Fixed Bug**: Defenses now properly marked/unmarked instead of state inversion
2. **More Control**: Users can mark/unmark by defense type
3. **Code Quality**: Follows established patterns from PR #181
4. **No Duplication**: Helper function eliminates code duplication
5. **Better UX**: Organized submenu structure is clearer

## Pattern Consistency

This implementation follows the same pattern as `markAllInitiators()` from PR #181:
- Helper function for business logic
- Multiple slot functions that call the helper
- Proper state checking and setting
- Display refresh after changes

## References

- Issue: [Small fixex] - Arreglar el cambio de todos las barreras
- Related PR: theotocopulitos/MARRTA#181 (Initiator marking pattern)
- Defense Types Enum: `mainwindow.h` line 42
- Database Persistence: `dbevent.cpp` `checkAndUpdate()` function
