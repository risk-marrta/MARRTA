/**
 * Example: Search Stages by Code
 * 
 * This script demonstrates how to search for stages using code patterns.
 * You can modify the searchPattern variable to search for different codes.
 * 
 * To run this script:
 * 1. Open a PRAXIS file in MARRTA
 * 2. Go to Action → Editor
 * 3. Copy and paste this script into the editor
 * 4. Modify the searchPattern variable below to match what you're looking for
 * 5. Click Execute
 */

// *** MODIFY THIS VALUE TO SEARCH FOR DIFFERENT PATTERNS ***
var searchPattern = "E"; // Example: "E", "01", "A", etc.

// Get all stages
var stages = Marrta.getAllStages();

console.log("==========================================================");
console.log("Stage Code Search");
console.log("==========================================================");
console.log("Search pattern: '" + searchPattern + "'");
console.log("Total stages to search: " + stages.length);
console.log("");

// Search for stages matching the pattern
var matchingStages = [];

for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    if (stage.codigo.indexOf(searchPattern) !== -1) {
        matchingStages.push(stage);
    }
}

// Display results
console.log("Found " + matchingStages.length + " matching stage(s):");
console.log("----------------------------------------");

if (matchingStages.length > 0) {
    for (var i = 0; i < matchingStages.length; i++) {
        var stage = matchingStages[i];
        var type = stage.id < 0 ? "[VIRTUAL]" : "[REGULAR]";
        
        console.log((i + 1) + ". " + type + " " + stage.codigo);
        console.log("   Name: " + stage.nombre);
        console.log("   ID: " + stage.id);
        console.log("   Description: " + stage.descripcion);
        
        if (stage.subetapas && stage.subetapas.length > 0) {
            console.log("   Substages: " + stage.subetapas.join(", "));
        }
        console.log("");
    }
} else {
    console.log("No stages found matching the pattern '" + searchPattern + "'");
    console.log("");
    console.log("Available stage codes:");
    for (var i = 0; i < stages.length; i++) {
        console.log("  - " + stages[i].codigo);
    }
}

console.log("==========================================================");
console.log("End of Search");
console.log("==========================================================");
