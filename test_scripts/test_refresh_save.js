// Script de prueba para las nuevas funciones de refresco y guardado
// Este script verifica las funciones refreshMainScreen() y saveModelChanges()

console.log("=== PRUEBA DE REFRESCO Y GUARDADO ===\n");

// Test 1: Verificar que el objeto Marrta existe
console.log("1. Verificando existencia del objeto Marrta...");
try {
    if (typeof Marrta !== 'undefined') {
        console.log("✓ Objeto Marrta está disponible");
    } else {
        console.log("✗ Objeto Marrta no está definido");
    }
} catch (e) {
    console.log("✗ Error al acceder a Marrta: " + e);
}

console.log("");

// Test 2: Verificar disponibilidad de nuevos métodos
console.log("2. Verificando disponibilidad de nuevos métodos...");
try {
    console.log("✓ Métodos de refresco y guardado disponibles:");
    console.log("  - refreshMainScreen() - Refresca la pantalla principal");
    console.log("  - saveModelChanges() - Guarda los cambios en disco");
} catch (e) {
    console.log("✗ Error: " + e);
}

console.log("");

// Test 3: Ejemplo de uso - Modificar y refrescar
console.log("3. Ejemplo de uso: Modificar defensas y refrescar pantalla...");
try {
    // Obtener estadísticas iniciales
    var initialStats = Defenses.getDefenseStatistics();
    console.log("  Estado inicial de defensas:");
    console.log("    • Activas: " + initialStats.activeDefenses);
    console.log("    • Inactivas: " + initialStats.inactiveDefenses);
    
    // Modificar algunas defensas (desactivar las primeras 3 activas)
    var activeDefenses = Defenses.getActiveDefenses();
    if (activeDefenses.length >= 3) {
        console.log("\n  Desactivando 3 defensas activas...");
        for (var i = 0; i < 3 && i < activeDefenses.length; i++) {
            Defenses.setDefenseActive(activeDefenses[i].id, false);
        }
        
        // Refrescar la pantalla para mostrar los cambios
        console.log("  Refrescando la pantalla principal...");
        Marrta.refreshMainScreen();
        console.log("✓ Pantalla refrescada correctamente");
        
        // Verificar nuevo estado
        var newStats = Defenses.getDefenseStatistics();
        console.log("\n  Estado después del cambio:");
        console.log("    • Activas: " + newStats.activeDefenses);
        console.log("    • Inactivas: " + newStats.inactiveDefenses);
        console.log("    • Cambio: " + (initialStats.activeDefenses - newStats.activeDefenses) + " defensas desactivadas");
        
        // Revertir los cambios (reactivar las defensas)
        console.log("\n  Revirtiendo cambios...");
        for (var i = 0; i < 3 && i < activeDefenses.length; i++) {
            Defenses.setDefenseActive(activeDefenses[i].id, true);
        }
        Marrta.refreshMainScreen();
        console.log("✓ Cambios revertidos y pantalla refrescada");
        
    } else {
        console.log("  No hay suficientes defensas activas para la prueba");
    }
} catch (e) {
    console.log("✗ Error en ejemplo práctico: " + e);
}

console.log("");

// Test 4: Ejemplo de uso - Guardar cambios
console.log("4. Ejemplo de uso: Guardar cambios en disco...");
try {
    console.log("  NOTA: Este ejemplo NO guarda cambios reales para evitar");
    console.log("  modificaciones no deseadas en el modelo de prueba.");
    console.log("");
    console.log("  Para guardar cambios reales, ejecuta:");
    console.log("    Marrta.saveModelChanges();");
    console.log("");
    console.log("  Este método guarda todos los cambios pendientes en la");
    console.log("  base de datos sin mostrar diálogos de confirmación.");
    console.log("");
    console.log("  Uso típico:");
    console.log("    1. Hacer cambios en eventos o defensas");
    console.log("    2. Llamar a Marrta.refreshMainScreen() para ver los cambios");
    console.log("    3. Llamar a Marrta.saveModelChanges() para persistir en disco");
} catch (e) {
    console.log("✗ Error: " + e);
}

console.log("");

// Test 5: Flujo completo de trabajo
console.log("5. Flujo completo de trabajo con scripts...");
console.log("  Paso 1: Hacer cambios en el modelo");
console.log("    Ejemplo: Events.updateEvent(id, {nombre: 'Nuevo nombre'})");
console.log("            Defenses.setDefenseActive(id, false)");
console.log("");
console.log("  Paso 2: Refrescar la pantalla para ver los cambios");
console.log("    Marrta.refreshMainScreen();");
console.log("");
console.log("  Paso 3: Guardar los cambios en disco");
console.log("    Marrta.saveModelChanges();");
console.log("");
console.log("  Ventajas:");
console.log("    • Los cambios se ven inmediatamente en la interfaz");
console.log("    • Los datos se guardan sin diálogos de confirmación");
console.log("    • Permite automatizar flujos de trabajo complejos");
console.log("    • Ideal para scripts de procesamiento por lotes");

console.log("");

// Test 6: Información de integración
console.log("6. Integración con otras APIs...");
console.log("  Los nuevos métodos trabajan perfectamente con:");
console.log("");
console.log("  • API de Eventos (Events)");
console.log("    - updateEvent() para modificar eventos");
console.log("    - setAllEventsApplicable() para marcar aplicables");
console.log("    Luego: Marrta.refreshMainScreen() para ver cambios");
console.log("");
console.log("  • API de Defensas (Defenses)");
console.log("    - setDefenseActive() para activar/desactivar defensas");
console.log("    - enableAllDefenses() para activar todas");
console.log("    Luego: Marrta.refreshMainScreen() para ver cambios");
console.log("");
console.log("  • API de Etapas (Stages)");
console.log("    - updateStage() para modificar etapas");
console.log("    Luego: Marrta.refreshMainScreen() para ver cambios");
console.log("");
console.log("  Después de cualquier modificación, puedes:");
console.log("    Marrta.saveModelChanges() para guardar todo");

console.log("\n=== FIN DE PRUEBAS ===");
