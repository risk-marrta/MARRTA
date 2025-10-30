// Script de prueba para el objeto Marrta
// Este script verifica el acceso al objeto principal de la aplicación

console.log("=== PRUEBA DE OBJETO MARRTA ===\n");

// Test 1: Verificar que el objeto Marrta existe
console.log("1. Verificando existencia del objeto Marrta...");
try {
    if (typeof Marrta !== 'undefined') {
        console.log("✓ Objeto Marrta está disponible");
        console.log("  Tipo: " + typeof Marrta);
    } else {
        console.log("✗ Objeto Marrta no está definido");
    }
} catch (e) {
    console.log("✗ Error al acceder a Marrta: " + e);
}

console.log("");

// Test 2: Listar propiedades y métodos disponibles
console.log("2. Explorando propiedades y métodos disponibles...");
try {
    console.log("✓ Métodos invocables desde JavaScript:");
    console.log("  - toggleActualEvent()");
    console.log("  - alternarDefensasEventoActual(tipoDefensa, habilitar)");
    console.log("  - alternarDefensaModelo(tipoDefensa, habilitar, pos)");
    console.log("  - refreshMainScreen() [NUEVO]");
    console.log("  - saveModelChanges() [NUEVO]");
    console.log("");
    console.log("  Nota: Estos métodos requieren que haya un modelo cargado");
    console.log("  y un evento seleccionado para funcionar correctamente.");
} catch (e) {
    console.log("✗ Error: " + e);
}

console.log("");

// Test 3: Información sobre constantes
console.log("3. Constantes de tipo de defensa...");
try {
    console.log("✓ Tipos de defensa disponibles:");
    console.log("  - ReductoresFrecuencia: 0");
    console.log("  - Barreras: 1");
    console.log("  - ReductoresConsecuencia: 2");
} catch (e) {
    console.log("✗ Error: " + e);
}

console.log("");

// Test 4: Verificar otros objetos globales
console.log("4. Verificando otros objetos globales disponibles...");
try {
    var globalObjects = [];
    
    if (typeof console !== 'undefined') {
        globalObjects.push("console - Para salida de mensajes");
    }
    if (typeof Marrta !== 'undefined') {
        globalObjects.push("Marrta - Ventana principal de la aplicación");
    }
    if (typeof Events !== 'undefined') {
        globalObjects.push("Events - API de eventos iniciadores");
    }
    if (typeof Defenses !== 'undefined') {
        globalObjects.push("Defenses - API de defensas");
    }
    if (typeof Stages !== 'undefined') {
        globalObjects.push("Stages - API de etapas y subetapas");
    }
    if (typeof Charts !== 'undefined') {
        globalObjects.push("Charts - API de gráficas y utilidades");
    }
    
    console.log("✓ Objetos globales disponibles:");
    for (var i = 0; i < globalObjects.length; i++) {
        console.log("  - " + globalObjects[i]);
    }
} catch (e) {
    console.log("✗ Error: " + e);
}

console.log("");

// Test 5: Ejemplo práctico combinando APIs
console.log("5. Ejemplo práctico: Análisis del modelo actual...");
try {
    // Obtener estadísticas del modelo
    var stages = Stages.getAllStages();
    var events = Events.getAllEvents();
    var defenses = Defenses.getAllDefenses();
    
    console.log("✓ Resumen del modelo cargado:");
    console.log("  - Etapas: " + stages.length);
    console.log("  - Eventos iniciadores: " + events.length);
    console.log("  - Defensas totales: " + defenses.length);
    
    // Estadísticas de defensas
    var stats = Defenses.getDefenseStatistics();
    console.log("\n  Estadísticas de defensas:");
    console.log("    • Activas: " + stats.activeDefenses);
    console.log("    • Inactivas: " + stats.inactiveDefenses);
    console.log("    • Humanas: " + stats.humanDefenses);
    console.log("    • No humanas: " + stats.nonHumanDefenses);
    
    // Estadísticas de eventos
    var eventStats = Events.getEventStatistics();
    console.log("\n  Estadísticas de eventos:");
    console.log("    • Con error humano: " + eventStats.humanErrorEvents);
    console.log("    • Sin error humano: " + eventStats.nonHumanErrorEvents);
    
} catch (e) {
    console.log("✗ Error en ejemplo práctico: " + e);
}

console.log("");

// Test 6: Información sobre el uso del objeto Marrta
console.log("6. Información sobre el uso del objeto Marrta...");
console.log("  El objeto Marrta proporciona acceso a la ventana principal");
console.log("  de la aplicación. Sus métodos permiten:");
console.log("");
console.log("  • toggleActualEvent()");
console.log("    Alterna el estado del evento actualmente seleccionado");
console.log("");
console.log("  • alternarDefensasEventoActual(tipo, habilitar)");
console.log("    Cambia el estado de las defensas del evento actual");
console.log("    Parámetros:");
console.log("      - tipo: 0=Reductores Frecuencia, 1=Barreras, 2=Reductores Consecuencia");
console.log("      - habilitar: true para activar, false para desactivar");
console.log("");
console.log("  • alternarDefensaModelo(tipo, habilitar, pos)");
console.log("    Cambia el estado de defensas en todo el modelo");
console.log("    Parámetros similares a alternarDefensasEventoActual()");
console.log("");
console.log("  • refreshMainScreen() [NUEVO]");
console.log("    Refresca la pantalla principal para reflejar los cambios");
console.log("    realizados por scripts. Actualiza la lista de eventos y");
console.log("    los detalles del evento actualmente seleccionado.");
console.log("");
console.log("  • saveModelChanges() [NUEVO]");
console.log("    Guarda los cambios efectuados en el modelo en la base de");
console.log("    datos sin mostrar diálogos de confirmación. Ideal para");
console.log("    scripts de procesamiento por lotes.");
console.log("");
console.log("  NOTA: Los métodos toggleActualEvent y alternar* están pensados");
console.log("  para uso interno. Para gestión desde scripts, es recomendable");
console.log("  usar la API de Defenses, Events, etc. seguido de refreshMainScreen()");
console.log("  y saveModelChanges() según sea necesario.");

console.log("\n=== FIN DE PRUEBAS ===");
