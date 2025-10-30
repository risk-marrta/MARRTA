# MARRTA Scripting API Documentation

## Overview

MARRTA includes a JavaScript scripting engine that allows users to automate tasks and interact with the application programmatically. The scripting engine is based on Qt's QJSEngine and provides access to various application features through the global `Marrta` object.

## Accessing the Scripting Engine

To access the scripting engine:
1. Open a PRAXIS file in MARRTA
2. Go to the menu: **Action → Editor** (or use the keyboard shortcut)
3. The scripting editor will open where you can write and execute JavaScript code

## Global Objects

### `Marrta`

The `Marrta` object provides access to the main application window and its methods.

### `console`

The `console` object provides methods for outputting messages:
- `console.log(message)` - Outputs a message to the script console

## API Methods

### Stage Management

#### `Marrta.getAllStages()`

Returns all stages defined in the current PRAXIS file, including virtual stages (stages with negative IDs).

**Returns:** `Array<Object>` - An array of stage objects

**Stage Object Properties:**
- `id` (Number) - The stage ID (can be negative for virtual stages)
- `nombre` (String) - The stage name
- `descripcion` (String) - The stage description
- `orden` (Number) - The display order
- `codigo` (String) - The stage code
- `subetapas` (Array<String>) - Array of substage names

**Example:**
```javascript
// Get all stages including virtual stages (id < 0)
var stages = Marrta.getAllStages();

console.log("Total stages: " + stages.length);

// Display information for each stage
for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    console.log("Stage: " + stage.nombre);
    console.log("  ID: " + stage.id);
    console.log("  Code: " + stage.codigo);
    console.log("  Description: " + stage.descripcion);
    console.log("  Order: " + stage.orden);
    if (stage.subetapas && stage.subetapas.length > 0) {
        console.log("  Substages: " + stage.subetapas.join(", "));
    }
    console.log("");
}

// Filter for virtual stages (id < 0)
var virtualStages = stages.filter(function(stage) {
    return stage.id < 0;
});

console.log("Virtual stages: " + virtualStages.length);
for (var i = 0; i < virtualStages.length; i++) {
    console.log("  - " + virtualStages[i].nombre + " (id: " + virtualStages[i].id + ")");
}

// Filter for regular stages (id >= 0)
var regularStages = stages.filter(function(stage) {
    return stage.id >= 0;
});

console.log("Regular stages: " + regularStages.length);
```

### Event Management

#### `Marrta.toggleActualEvent()`

Toggles the active state of the currently selected event.

**Returns:** `void`

**Example:**
```javascript
// Toggle the current event's active state
Marrta.toggleActualEvent();
console.log("Event toggled");
```

#### `Marrta.alternarDefensasEventoActual(tipoDefensa, habilitar)`

Enables or disables all defenses of a specific type for the currently selected event.

**Parameters:**
- `tipoDefensa` (Number) - The type of defense:
  - `0` - Reductores de Frecuencia (Frequency Reducers)
  - `1` - Barreras (Barriers)
  - `2` - Reductores de Consecuencia (Consequence Reducers)
- `habilitar` (Boolean) - `true` to enable, `false` to disable

**Returns:** `void`

**Example:**
```javascript
// Enable all barriers for the current event
Marrta.alternarDefensasEventoActual(1, true);
console.log("All barriers enabled");

// Disable all frequency reducers for the current event
Marrta.alternarDefensasEventoActual(0, false);
console.log("All frequency reducers disabled");
```

#### `Marrta.alternarDefensaModelo(tipoDefensa, habilitar, pos)`

Enables or disables a specific defense across the entire model.

**Parameters:**
- `tipoDefensa` (Number) - The type of defense (0, 1, or 2)
- `habilitar` (Boolean) - `true` to enable, `false` to disable
- `pos` (QPoint) - The position in the defense list

**Returns:** `void`

**Note:** This method is typically used internally by the UI and may require specific position information.

## Tips and Best Practices

1. **Error Handling:** Always check if objects exist before accessing their properties
2. **Testing:** Test your scripts on a backup copy of your PRAXIS file first
3. **Performance:** For large datasets, consider processing in batches
4. **Debugging:** Use `console.log()` liberally to understand what your script is doing

## Example Scripts

### List All Stages with Their Codes

```javascript
var stages = Marrta.getAllStages();

console.log("=== Stage List ===");
for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    var type = stage.id < 0 ? "[VIRTUAL]" : "[REGULAR]";
    console.log(type + " " + stage.codigo + " - " + stage.nombre);
}
```

### Count Stages by Type

```javascript
var stages = Marrta.getAllStages();

var virtualCount = 0;
var regularCount = 0;

for (var i = 0; i < stages.length; i++) {
    if (stages[i].id < 0) {
        virtualCount++;
    } else {
        regularCount++;
    }
}

console.log("Statistics:");
console.log("  Regular stages: " + regularCount);
console.log("  Virtual stages: " + virtualCount);
console.log("  Total stages: " + stages.length);
```

### Find Stages by Code Pattern

```javascript
var stages = Marrta.getAllStages();
var searchPattern = "E"; // Change this to search for different patterns

console.log("Searching for stages with code containing '" + searchPattern + "':");

for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    if (stage.codigo.indexOf(searchPattern) !== -1) {
        console.log("  Found: " + stage.codigo + " - " + stage.nombre);
    }
}
```

## Version History

- **v2.0.0 (2025-10-24)** - Major release with Python scripting engine, integrated stage management, interactive flowcharts, comprehensive user manual, enhanced reports, and GitHub integration
- **v1.5 (2025-10-11)** - Added defenses management window and various UI improvements
- **v1.4.5 (2025-10-08)** - Version bump and release preparation
- **v1.4.4 (2025-10-07)** - Added New Model Creation Wizard with comprehensive stage management
- **v1.4.3 (2025-10-07)** - Bug fixes and stability improvements
- **v1.4.2 (2025-09-30)** - Added `getAllStages()` method to access virtual stages from scripting

## Support

For issues, questions, or contributions, please visit:
https://github.com/theotocopulitos/MARRTA
