# MARRTA Scripting Examples

This directory contains example scripts demonstrating how to use the MARRTA scripting API.

## How to Use These Examples

1. **Open MARRTA** and load a PRAXIS file
2. **Open the Scripting Editor**: Go to Action → Editor (or use the keyboard shortcut)
3. **Load an example**: Open one of the `.js` files in this directory with a text editor
4. **Copy the code**: Copy the entire content of the example file
5. **Paste into editor**: Paste the code into the MARRTA scripting editor
6. **Execute**: Click the "Execute" button to run the script
7. **View results**: Check the console output at the bottom of the editor

## Available Examples

### 1. list_all_stages.js
**Purpose:** List all stages in the current PRAXIS file with detailed information

**What it does:**
- Retrieves all stages (regular and virtual)
- Displays comprehensive information for each stage:
  - ID, name, code, description, order
  - List of substages (if any)
- Shows both regular and virtual stages

**Use case:** Get a complete overview of all stages in your project

---

### 2. filter_virtual_stages.js
**Purpose:** Filter and display only virtual stages

**What it does:**
- Retrieves all stages
- Separates virtual stages (id < 0) from regular stages
- Displays statistics and details about virtual stages only

**Use case:** Focus on virtual stages to understand special workflow stages

---

### 3. stage_statistics.js
**Purpose:** Generate comprehensive statistics about stages

**What it does:**
- Counts stages by type (regular vs virtual)
- Counts stages with/without substages
- Finds the stage with the most substages
- Lists all stage codes

**Use case:** Get a statistical overview of your stage structure

---

### 4. search_stages_by_code.js
**Purpose:** Search for stages using code patterns

**What it does:**
- Searches for stages whose code contains a specific pattern
- Displays matching stages with full details
- Can be customized by changing the `searchPattern` variable

**Use case:** Quickly find stages by their code identifier

**Customization:** 
```javascript
var searchPattern = "E"; // Change this to search for different patterns
```

## Scripting API Reference

For complete API documentation, see: [SCRIPTING_API.md](../../SCRIPTING_API.md)

## Key Concepts

### Virtual Stages
Virtual stages are stages with negative IDs (id < 0). They are special stages used for specific purposes in the MARRTA risk analysis workflow.

### Regular Stages
Regular stages have non-negative IDs (id >= 0) and represent standard workflow stages.

### Getting All Stages
The `getAllStages()` method returns both regular and virtual stages, which is important for complete workflow analysis:

```javascript
var stages = Marrta.getAllStages();
```

## Tips for Writing Your Own Scripts

1. **Always check array lengths** before accessing elements
2. **Use console.log()** frequently to debug your scripts
3. **Test on a backup** of your PRAXIS file first
4. **Start simple** and build complexity gradually
5. **Filter data** using JavaScript array methods like `filter()`, `map()`, and `reduce()`

## Example: Creating a Custom Filter

```javascript
// Get all stages
var stages = Marrta.getAllStages();

// Custom filter: stages with more than 2 substages
var busyStages = stages.filter(function(stage) {
    return stage.subetapas && stage.subetapas.length > 2;
});

console.log("Stages with more than 2 substages: " + busyStages.length);
for (var i = 0; i < busyStages.length; i++) {
    console.log("  - " + busyStages[i].nombre + " (" + busyStages[i].subetapas.length + " substages)");
}
```

## Support

For questions or issues with these examples, please visit:
https://github.com/theotocopulitos/MARRTA/issues

## Contributing

If you have created useful scripts that others might benefit from, consider contributing them back to the project!
