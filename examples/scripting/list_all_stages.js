/**
 * Example: List All Stages
 * 
 * This script demonstrates how to use the getAllStages() method to retrieve
 * and display all stages in the current PRAXIS file, including virtual stages.
 * 
 * To run this script:
 * 1. Open a PRAXIS file in MARRTA
 * 2. Go to Action → Editor
 * 3. Copy and paste this script into the editor
 * 4. Click Execute
 */

// Get all stages including virtual stages (id < 0)
var stages = Marrta.getAllStages();

console.log("==========================================================");
console.log("MARRTA Stage List Report");
console.log("==========================================================");
console.log("Total stages found: " + stages.length);
console.log("");

// Display information for each stage
for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    var type = stage.id < 0 ? "[VIRTUAL]" : "[REGULAR]";
    
    console.log("Stage #" + (i + 1) + " " + type);
    console.log("  ID: " + stage.id);
    console.log("  Name: " + stage.nombre);
    console.log("  Code: " + stage.codigo);
    console.log("  Description: " + stage.descripcion);
    console.log("  Order: " + stage.orden);
    
    if (stage.subetapas && stage.subetapas.length > 0) {
        console.log("  Substages (" + stage.subetapas.length + "):");
        for (var j = 0; j < stage.subetapas.length; j++) {
            console.log("    - " + stage.subetapas[j]);
        }
    } else {
        console.log("  Substages: None");
    }
    console.log("");
}

console.log("==========================================================");
console.log("End of Report");
console.log("==========================================================");
