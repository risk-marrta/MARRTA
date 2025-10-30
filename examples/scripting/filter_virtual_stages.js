/**
 * Example: Filter Virtual Stages
 * 
 * This script demonstrates how to filter and display only virtual stages
 * (stages with id < 0) from the PRAXIS file.
 * 
 * Virtual stages are special stages used for specific purposes in the
 * risk analysis workflow.
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
console.log("Virtual Stages Report");
console.log("==========================================================");

// Filter for virtual stages (id < 0)
var virtualStages = stages.filter(function(stage) {
    return stage.id < 0;
});

// Filter for regular stages (id >= 0)
var regularStages = stages.filter(function(stage) {
    return stage.id >= 0;
});

console.log("Total stages: " + stages.length);
console.log("Regular stages: " + regularStages.length);
console.log("Virtual stages: " + virtualStages.length);
console.log("");

if (virtualStages.length > 0) {
    console.log("Virtual Stages Details:");
    console.log("----------------------------------------");
    
    for (var i = 0; i < virtualStages.length; i++) {
        var stage = virtualStages[i];
        console.log((i + 1) + ". " + stage.nombre);
        console.log("   ID: " + stage.id);
        console.log("   Code: " + stage.codigo);
        console.log("   Description: " + stage.descripcion);
        
        if (stage.subetapas && stage.subetapas.length > 0) {
            console.log("   Substages: " + stage.subetapas.join(", "));
        }
        console.log("");
    }
} else {
    console.log("No virtual stages found in this PRAXIS file.");
}

console.log("==========================================================");
console.log("End of Report");
console.log("==========================================================");
