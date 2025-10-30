// Script de prueba para la API de Eventos
// Este script verifica el funcionamiento de la API de Events

console.log("=== PRUEBA DE API DE EVENTOS ===\n");

// Test 1: Obtener todos los eventos
console.log("1. Probando getAllEvents()...");
try {
    var events = Events.getAllEvents();
    console.log("✓ getAllEvents() funciona correctamente");
    console.log("  Total de eventos: " + events.length);
    if (events.length > 0) {
        console.log("  Primer evento: " + events[0].codigo + " - " + events[0].nombre);
    }
} catch (e) {
    console.log("✗ Error en getAllEvents(): " + e);
}

console.log("");

// Test 2: Obtener eventos agrupados por etapa
console.log("2. Probando getEventsByStageGrouped()...");
try {
    var eventsByStage = Events.getEventsByStageGrouped();
    console.log("✓ getEventsByStageGrouped() funciona correctamente");
    var stageCount = 0;
    for (var stage in eventsByStage) {
        stageCount++;
        var events = eventsByStage[stage];
        console.log("  " + stage + ": " + events.length + " eventos");
    }
    console.log("  Total de etapas con eventos: " + stageCount);
} catch (e) {
    console.log("✗ Error en getEventsByStageGrouped(): " + e);
}

console.log("");

// Test 3: Contar eventos por etapa
console.log("3. Probando countEventsByStage()...");
try {
    var counts = Events.countEventsByStage();
    console.log("✓ countEventsByStage() funciona correctamente");
    for (var stage in counts) {
        console.log("  " + stage + ": " + counts[stage] + " eventos");
    }
} catch (e) {
    console.log("✗ Error en countEventsByStage(): " + e);
}

console.log("");

// Test 4: Obtener eventos por consecuencia
console.log("4. Probando getEventsByConsequence()...");
try {
    var eventsByConseq = Events.getEventsByConsequence();
    console.log("✓ getEventsByConsequence() funciona correctamente");
    for (var conseq in eventsByConseq) {
        var events = eventsByConseq[conseq];
        console.log("  " + conseq + ": " + events.length + " eventos");
    }
} catch (e) {
    console.log("✗ Error en getEventsByConsequence(): " + e);
}

console.log("");

// Test 5: Obtener estadísticas de eventos
console.log("5. Probando getEventStatistics()...");
try {
    var stats = Events.getEventStatistics();
    console.log("✓ getEventStatistics() funciona correctamente");
    console.log("  Total de eventos: " + stats.totalEvents);
    console.log("  Eventos con error humano: " + stats.humanErrorEvents);
    console.log("  Eventos sin error humano: " + stats.nonHumanErrorEvents);
} catch (e) {
    console.log("✗ Error en getEventStatistics(): " + e);
}

console.log("");

// Test 6: Verificar propiedades riesgoBase
console.log("6. Probando propiedades riesgoBase...");
try {
    var events = Events.getAllEvents();
    if (events.length > 0) {
        var event = events[0];
        console.log("✓ Propiedades riesgoBase disponibles");
        console.log("  Evento: " + event.codigo);
        console.log("  Frecuencia Base: " + event.frecuenciaBase);
        console.log("  Consecuencia Base: " + event.consecuenciaBase);
        console.log("  Riesgo Base: " + event.riesgoBase + " (" + event.riesgoBaseTexto + ")");
        console.log("  Riesgo Calculado: " + event.riesgo + " (" + event.riesgoTexto + ")");
    } else {
        console.log("  (No hay eventos para probar)");
    }
} catch (e) {
    console.log("✗ Error probando riesgoBase: " + e);
}

console.log("\n=== FIN DE PRUEBAS ===");
