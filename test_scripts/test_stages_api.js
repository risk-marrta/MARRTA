// Script de prueba para la API de Etapas
// Este script verifica el funcionamiento de la API de Stages

console.log("=== PRUEBA DE API DE ETAPAS ===\n");

// Test 1: Obtener todas las etapas
console.log("1. Probando getAllStages()...");
try {
    var stages = Stages.getAllStages();
    console.log("✓ getAllStages() funciona correctamente");
    console.log("  Total de etapas: " + stages.length);
    if (stages.length > 0) {
        console.log("  Primera etapa: " + stages[0].nombre);
    }
} catch (e) {
    console.log("✗ Error en getAllStages(): " + e);
}

console.log("");

// Test 2: Obtener todas las subetapas
console.log("2. Probando getAllSubstages()...");
try {
    var substages = Stages.getAllSubstages();
    console.log("✓ getAllSubstages() funciona correctamente");
    console.log("  Total de subetapas: " + substages.length);
    if (substages.length > 0) {
        console.log("  Primera subetapa: " + substages[0].nombre);
    }
} catch (e) {
    console.log("✗ Error en getAllSubstages(): " + e);
}

console.log("");

// Test 3: Obtener etapas con subetapas
console.log("3. Probando getStagesWithSubstages()...");
try {
    var stagesWithSubs = Stages.getStagesWithSubstages();
    console.log("✓ getStagesWithSubstages() funciona correctamente");
    var count = 0;
    for (var stage in stagesWithSubs) {
        count++;
    }
    console.log("  Etapas recuperadas: " + count);
} catch (e) {
    console.log("✗ Error en getStagesWithSubstages(): " + e);
}

console.log("");

// Test 4: Contar subetapas por etapa
console.log("4. Probando countSubstagesByStage()...");
try {
    var counts = Stages.countSubstagesByStage();
    console.log("✓ countSubstagesByStage() funciona correctamente");
    for (var stage in counts) {
        console.log("  " + stage + ": " + counts[stage] + " subetapas");
    }
} catch (e) {
    console.log("✗ Error en countSubstagesByStage(): " + e);
}

console.log("\n=== FIN DE PRUEBAS ===");
