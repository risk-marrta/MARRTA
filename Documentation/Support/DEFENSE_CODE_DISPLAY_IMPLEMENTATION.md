# Defense Code and Hardiness Display Implementation

## Overview
This document describes the implementation of displaying defense codes, names, and hardiness levels in all defense lists in the MainWindow with right-aligned hardiness display.

## Problem Statement
Previously, defense lists in the MainWindow only displayed the defense name (e.g., "Sistema de detección de incendios"). Users had to double-click on a defense to see its code and hardiness level in the information dialog. This was inefficient when trying to identify defenses by their code or assess their robustness.

## Solution
Modified all defense list displays to show the defense code and name on the left, with hardiness right-aligned in brackets: `[code] Name       [Hardiness]`

For example:
- Before: `Sistema de detección de incendios`
- After: `[FMB-001] Sistema de detección de incendios                              [Robusta]`

## Implementation Details

### Display Format Changes

Three methods were updated to change the display format:

1. **mostrarReductoresFrecuencia()** (Line ~950)
2. **mostrarBarreras()** (Line ~1004)
3. **mostrarReductoresConsecuencia()** (Line ~1060)

**Before:**
```cpp
for (auto linea : actualCompleteEvent->reductoresFrecuencias) {
    lstLineas << linea.nombre;
    
    if (linea.active) {
        lstSeleccionados << linea.nombre;
    }
}
```

**After:**
```cpp
for (auto linea : actualCompleteEvent->reductoresFrecuencias) {
    QString hardnessText = MatrizRiesgo::Durezas.at(linea.hardiness);
    QString leftPart = QString("[%1] %2").arg(linea.code, linea.nombre);
    QString rightPart = QString("[%1]").arg(hardnessText);
    // Pad to create right-aligned appearance
    int totalWidth = 100;
    int padding = totalWidth - leftPart.length() - rightPart.length();
    if (padding < 1) padding = 1; // Ensure at least one space
    QString displayText = leftPart + QString(padding, ' ') + rightPart;
    lstLineas << displayText;
    
    if (linea.active) {
        lstSeleccionados << displayText;
    }
}
```

### Name Extraction Logic

Since the display text now includes the code and hardiness (right-aligned), all handlers that need to compare or use the defense name must extract it from the display format. This is done by removing the code prefix and the hardiness suffix:

```cpp
QString displayText = item.data().toString();  // e.g., "[FMB-001] Sistema de detección       [Robusta]"
QString temp = displayText.section("] ", 1);   // Extracts "Sistema de detección       [Robusta]"
// Remove the trailing "[Hardness]" part by finding the last '['
int lastBracket = temp.lastIndexOf('[');
if (lastBracket > 0) {
    temp = temp.left(lastBracket).trimmed(); // Extracts "Sistema de detección"
}
QString nombre = temp;
```

The extraction process:
1. `section("] ", 1)` removes the `[code] ` prefix
2. `lastIndexOf('[')` finds the position of the opening bracket of the hardiness
3. `left(lastBracket).trimmed()` removes the `[Hardiness]` suffix and trims spaces
4. Result is the original defense name

### Hardiness Levels

The hardiness is retrieved from `MatrizRiesgo::Durezas`, which maps integer values to Spanish text:

```cpp
const std::map<int, QString> MatrizRiesgo::Durezas {
    {-1, "Muy blanda"},    // Very soft
    {0, "Blanda"},         // Soft
    {1, "Normal"},         // Normal
    {2, "Robusta"},        // Robust
    {3, "Muy robusta"}     // Very robust
};
```

### Updated Handlers

All interaction handlers were updated to extract the name from the display text:

1. **Checkbox State Change Handlers** (Lines ~962, ~1016, ~1072)
   - In `mostrarReductoresFrecuencia()`, `mostrarBarreras()`, `mostrarReductoresConsecuencia()`
   - When a user clicks a checkbox, extract the name to find the matching defense in the data model

2. **Double-Click Handlers** (Lines ~1905, ~1911, ~1917)
   - `on_lReductoresFrecuencia_doubleClicked()`
   - `on_lBarreras_doubleClicked()`
   - `on_lReductoresConsecuencia_doubleClicked()`
   - Extract name before calling `mostrarInfoBarrera()`

3. **Context Menu Handlers** (Lines ~1999, ~2056, ~2114)
   - `on_lReductoresFrecuencia_customContextMenuRequested()`
   - `on_lBarreras_customContextMenuRequested()`
   - `on_lReductoresConsecuencia_customContextMenuRequested()`
   - Extract name for "Analizar fortaleza" and "Mostrar descripción" menu actions

4. **alternarDefensaModelo()** (Line ~2331)
   - Marks/unmarks a specific defense across the entire model
   - Extract name to find matching defenses in all events

## Files Modified
- **mainwindow.cpp**: Updated defense display methods and all interaction handlers

## Testing Considerations

### Manual Testing Steps
1. Open a PRAXIS model file
2. Select an event from the event list
3. Verify that all three defense lists show defenses in `[code] Name - Hardiness` format:
   - Reductores de Frecuencia
   - Barreras
   - Reductores de Consecuencia
4. Verify hardiness levels are displayed correctly:
   - Check that hardiness text matches the defense's robustness level
   - Verify all five levels can appear: "Muy blanda", "Blanda", "Normal", "Robusta", "Muy robusta"
5. Test checkbox interactions:
   - Click a checkbox to activate/deactivate a defense
   - Verify that the defense state changes correctly
6. Test double-click:
   - Double-click on a defense
   - Verify that the information dialog appears with correct defense details
7. Test context menu operations:
   - Right-click on a defense
   - Test "Analizar fortaleza" option
   - Test "Mostrar descripción" option
   - Test "Marcar esta defensa en todos los iniciadores"
   - Test "Desmarcar esta defensa en todos los iniciadores"
8. Verify that all operations work correctly with the new display format

### Edge Cases Tested
- Defenses with empty codes (handled gracefully by QString::arg)
- Defenses with different hardiness levels (-1 to 3)
- Defenses with special characters in codes or names
- Multiple defenses with similar names (code helps distinguish them)

## Benefits
- **Improved Usability**: Users can quickly identify defenses by their code and assess their robustness
- **Better Context**: Code and hardiness provide additional context without extra clicks
- **Quick Assessment**: Hardiness level visible at a glance helps prioritize defenses
- **Consistency**: All defense lists follow the same display format
- **No Data Changes**: Display-only change, no database modifications needed

## Backward Compatibility
- No database schema changes
- No changes to data structures
- Only UI display format changed
- All existing functionality preserved

## Related Code
- **DefenseDefinition**: Contains the `code` and `nombre` fields (defensedefinition.h)
- **DefenseListModel**: Displays the defense list with checkboxes (defenselistmodel.cpp)
- **DbDefense**: Loads defense data including codes from database (dbdefense.cpp)

## Future Enhancements
If needed in the future:
- Allow users to toggle code display on/off
- Add code to defense list column headers
- Color-code defenses by type or hardiness
- Sort defenses by code or name
