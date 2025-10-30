// Script de prueba para la API de Defensas
// Este script verifica el funcionamiento de la API de Defenses

console.log("=== PRUEBA DE API DE DEFENSAS ===\n");

// Test 1: Obtener todas las defensas
console.log("1. Probando getAllDefenses()...");
try {
    var defenses = Defenses.getAllDefenses();
    console.log("✓ getAllDefenses() funciona correctamente");
    console.log("  Total de defensas: " + defenses.length);
    if (defenses.length > 0) {
        console.log("  Primera defensa: " + defenses[0].code + " - " + defenses[0].nombre);
    }
} catch (e) {
    console.log("✗ Error en getAllDefenses(): " + e);
}

console.log("");

// Test 2: Obtener solo barreras
console.log("2. Probando getBarriers()...");
try {
    var barriers = Defenses.getBarriers();
    console.log("✓ getBarriers() funciona correctamente");
    console.log("  Total de barreras: " + barriers.length);
} catch (e) {
    console.log("✗ Error en getBarriers(): " + e);
}

console.log("");

// Test 3: Obtener reductores de frecuencia
console.log("3. Probando getFrequencyReducers()...");
try {
    var freqReducers = Defenses.getFrequencyReducers();
    console.log("✓ getFrequencyReducers() funciona correctamente");
    console.log("  Total de reductores de frecuencia: " + freqReducers.length);
} catch (e) {
    console.log("✗ Error en getFrequencyReducers(): " + e);
}

console.log("");

// Test 4: Obtener reductores de consecuencia
console.log("4. Probando getConsequenceReducers()...");
try {
    var consReducers = Defenses.getConsequenceReducers();
    console.log("✓ getConsequenceReducers() funciona correctamente");
    console.log("  Total de reductores de consecuencia: " + consReducers.length);
} catch (e) {
    console.log("✗ Error en getConsequenceReducers(): " + e);
}

console.log("");

// Test 5: Obtener defensas agrupadas por tipo
console.log("5. Probando getDefensesByTypeGrouped()...");
try {
    var grouped = Defenses.getDefensesByTypeGrouped();
    console.log("✓ getDefensesByTypeGrouped() funciona correctamente");
    for (var type in grouped) {
        var defenses = grouped[type];
        console.log("  " + type + ": " + defenses.length + " defensas");
    }
} catch (e) {
    console.log("✗ Error en getDefensesByTypeGrouped(): " + e);
}

console.log("");

// Test 6: Obtener estadísticas de defensas
console.log("6. Probando getDefenseStatistics()...");
try {
    var stats = Defenses.getDefenseStatistics();
    console.log("✓ getDefenseStatistics() funciona correctamente");
    console.log("  Total de defensas: " + stats.totalDefenses);
    console.log("  Defensas activas: " + stats.activeDefenses);
    console.log("  Defensas inactivas: " + stats.inactiveDefenses);
    console.log("  Defensas humanas: " + stats.humanDefenses);
    console.log("  Defensas no humanas: " + stats.nonHumanDefenses);
} catch (e) {
    console.log("✗ Error en getDefenseStatistics(): " + e);
}

console.log("\n=== FIN DE PRUEBAS ===");
