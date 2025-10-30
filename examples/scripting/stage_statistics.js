/**
 * Example: Stage Statistics
 * 
 * This script demonstrates how to analyze and display statistics about
 * stages in the current PRAXIS file.
 * 
 * It shows:
 * - Total number of stages
 * - Count by type (regular vs virtual)
 * - Count of stages with/without substages
 * - Stage code patterns
 * 
 * To run this script:
 * 1. Open a PRAXIS file in MARRTA
 * 2. Go to Action → Editor
 * 3. Copy and paste this script into the editor
 * 4. Click Execute
 */

// Get all stages
var stages = Marrta.getAllStages();

console.log("==========================================================");
console.log("MARRTA Stage Statistics Report");
console.log("==========================================================");
console.log("");

// Count stages by type
var virtualCount = 0;
var regularCount = 0;
var withSubstagesCount = 0;
var withoutSubstagesCount = 0;

for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    
    // Count by ID type
    if (stage.id < 0) {
        virtualCount++;
    } else {
        regularCount++;
    }
    
    // Count by substage presence
    if (stage.subetapas && stage.subetapas.length > 0) {
        withSubstagesCount++;
    } else {
        withoutSubstagesCount++;
    }
}

// Display statistics
console.log("STAGE TYPE STATISTICS:");
console.log("  Total stages: " + stages.length);
console.log("  Regular stages (id >= 0): " + regularCount);
console.log("  Virtual stages (id < 0): " + virtualCount);
console.log("");

console.log("SUBSTAGE STATISTICS:");
console.log("  Stages with substages: " + withSubstagesCount);
console.log("  Stages without substages: " + withoutSubstagesCount);
console.log("");

// Find stages with most substages
if (withSubstagesCount > 0) {
    var maxSubstages = 0;
    var stageWithMostSubstages = null;
    
    for (var i = 0; i < stages.length; i++) {
        var stage = stages[i];
        if (stage.subetapas && stage.subetapas.length > maxSubstages) {
            maxSubstages = stage.subetapas.length;
            stageWithMostSubstages = stage;
        }
    }
    
    console.log("SUBSTAGE DETAILS:");
    console.log("  Stage with most substages: " + stageWithMostSubstages.nombre);
    console.log("  Number of substages: " + maxSubstages);
    console.log("  Substages: " + stageWithMostSubstages.subetapas.join(", "));
    console.log("");
}

// List all stage codes
console.log("STAGE CODES:");
for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    var type = stage.id < 0 ? "[V]" : "[R]";
    console.log("  " + type + " " + stage.codigo + " - " + stage.nombre);
}
console.log("");

console.log("==========================================================");
console.log("End of Report");
console.log("==========================================================");
