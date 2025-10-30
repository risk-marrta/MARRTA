// Script de prueba para la API de Gráficas y Utilidades
// Este script verifica el funcionamiento de la API de Charts

console.log("=== PRUEBA DE API DE CHARTS ===\n");

// Datos de prueba
var testData = [
    {nombre: "Item A", valor: 10, categoria: "Cat1", activo: true},
    {nombre: "Item B", valor: 25, categoria: "Cat2", activo: false},
    {nombre: "Item C", valor: 15, categoria: "Cat1", activo: true},
    {nombre: "Item D", valor: 30, categoria: "Cat3", activo: true},
    {nombre: "Item E", valor: 20, categoria: "Cat2", activo: false}
];

// Test 1: Generar gráfica de barras en texto
console.log("1. Probando generateTextBarChart()...");
try {
    var chartData = {
        "Categoría 1": 25,
        "Categoría 2": 45,
        "Categoría 3": 30,
        "Categoría 4": 15
    };
    var chart = Charts.generateTextBarChart("Distribución de Datos", chartData, 40);
    console.log("✓ generateTextBarChart() funciona correctamente");
    console.log(chart);
} catch (e) {
    console.log("✗ Error en generateTextBarChart(): " + e);
}

console.log("");

// Test 2: Generar tabla
console.log("2. Probando generateTable()...");
try {
    var headers = ["ID", "Nombre", "Valor", "Categoría"];
    var rows = [
        [1, "Item A", 10, "Cat1"],
        [2, "Item B", 25, "Cat2"],
        [3, "Item C", 15, "Cat1"],
        [4, "Item D", 30, "Cat3"]
    ];
    var table = Charts.generateTable("Tabla de Datos", headers, rows);
    console.log("✓ generateTable() funciona correctamente");
    console.log(table);
} catch (e) {
    console.log("✗ Error en generateTable(): " + e);
}

console.log("");

// Test 3: Ordenar por propiedad
console.log("3. Probando sortByProperty()...");
try {
    var sortedAsc = Charts.sortByProperty(testData, "valor", true);
    console.log("✓ sortByProperty() funciona correctamente");
    console.log("  Ordenado ascendente por valor:");
    for (var i = 0; i < sortedAsc.length; i++) {
        console.log("    " + sortedAsc[i].nombre + ": " + sortedAsc[i].valor);
    }
    
    var sortedDesc = Charts.sortByProperty(testData, "valor", false);
    console.log("  Ordenado descendente por valor:");
    for (var i = 0; i < sortedDesc.length; i++) {
        console.log("    " + sortedDesc[i].nombre + ": " + sortedDesc[i].valor);
    }
} catch (e) {
    console.log("✗ Error en sortByProperty(): " + e);
}

console.log("");

// Test 4: Agrupar por propiedad
console.log("4. Probando groupByProperty()...");
try {
    var grouped = Charts.groupByProperty(testData, "categoria");
    console.log("✓ groupByProperty() funciona correctamente");
    for (var cat in grouped) {
        var items = grouped[cat];
        console.log("  " + cat + ": " + items.length + " items");
        for (var i = 0; i < items.length; i++) {
            console.log("    - " + items[i].nombre);
        }
    }
} catch (e) {
    console.log("✗ Error en groupByProperty(): " + e);
}

console.log("");

// Test 5: Contar por propiedad
console.log("5. Probando countByProperty()...");
try {
    var counts = Charts.countByProperty(testData, "categoria");
    console.log("✓ countByProperty() funciona correctamente");
    for (var cat in counts) {
        console.log("  " + cat + ": " + counts[cat]);
    }
} catch (e) {
    console.log("✗ Error en countByProperty(): " + e);
}

console.log("");

// Test 6: Filtrar por propiedad
console.log("6. Probando filterByProperty()...");
try {
    var filtered = Charts.filterByProperty(testData, "activo", true);
    console.log("✓ filterByProperty() funciona correctamente");
    console.log("  Items activos: " + filtered.length);
    for (var i = 0; i < filtered.length; i++) {
        console.log("    - " + filtered[i].nombre);
    }
} catch (e) {
    console.log("✗ Error en filterByProperty(): " + e);
}

console.log("");

// Test 7: Estadísticas numéricas
console.log("7. Probando getNumericStats()...");
try {
    var stats = Charts.getNumericStats(testData, "valor");
    console.log("✓ getNumericStats() funciona correctamente");
    console.log("  Cantidad: " + stats.count);
    console.log("  Mínimo: " + stats.min);
    console.log("  Máximo: " + stats.max);
    console.log("  Promedio: " + stats.avg);
    console.log("  Suma total: " + stats.sum);
} catch (e) {
    console.log("✗ Error en getNumericStats(): " + e);
}

console.log("");

// Test 8: Crear estructura de gráfica de barras
console.log("8. Probando createBarChart()...");
try {
    var barChartData = {
        "Enero": 100,
        "Febrero": 120,
        "Marzo": 90,
        "Abril": 150
    };
    var barChart = Charts.createBarChart("Ventas Mensuales", barChartData);
    console.log("✓ createBarChart() funciona correctamente");
    console.log("  Tipo: " + barChart.type);
    console.log("  Título: " + barChart.title);
    console.log("  Etiquetas: " + barChart.labels.length);
    console.log("  Valores: " + barChart.values.length);
} catch (e) {
    console.log("✗ Error en createBarChart(): " + e);
}

console.log("");

// Test 9: Crear estructura de gráfica circular
console.log("9. Probando createPieChart()...");
try {
    var pieChartData = {
        "Segmento A": 30,
        "Segmento B": 45,
        "Segmento C": 25
    };
    var pieChart = Charts.createPieChart("Distribución de Segmentos", pieChartData);
    console.log("✓ createPieChart() funciona correctamente");
    console.log("  Tipo: " + pieChart.type);
    console.log("  Título: " + pieChart.title);
    console.log("  Total: " + pieChart.total);
    console.log("  Segmentos: " + pieChart.segments.length);
    for (var i = 0; i < pieChart.segments.length; i++) {
        var seg = pieChart.segments[i];
        console.log("    - " + seg.label + ": " + seg.value + " (" + seg.percentage.toFixed(1) + "%)");
    }
} catch (e) {
    console.log("✗ Error en createPieChart(): " + e);
}

console.log("");

// Test 10: Ejemplo práctico con datos reales
console.log("10. Ejemplo Práctico: Análisis de Eventos...");
try {
    var events = Events.getAllEvents();
    if (events.length > 0) {
        console.log("✓ Datos obtenidos correctamente");
        
        // Contar eventos por riesgo
        var riskCounts = Charts.countByProperty(events, "riesgoTexto");
        console.log("\nDistribución de Eventos por Nivel de Riesgo:");
        console.log(Charts.generateTextBarChart("Eventos por Riesgo", riskCounts, 30));
        
        // Estadísticas de consecuencias
        var consecStats = Charts.getNumericStats(events, "consecuencia");
        console.log("Estadísticas de Consecuencias:");
        console.log("  Nivel mínimo: " + consecStats.min);
        console.log("  Nivel máximo: " + consecStats.max);
        console.log("  Nivel promedio: " + consecStats.avg.toFixed(2));
        
        // Filtrar eventos de alto riesgo
        var highRiskEvents = Charts.filterByProperty(events, "riesgo", 3);
        console.log("\nEventos de alto riesgo: " + highRiskEvents.length);
    } else {
        console.log("⚠ No hay eventos en el modelo para analizar");
    }
} catch (e) {
    console.log("✗ Error en ejemplo práctico: " + e);
}

console.log("\n=== FIN DE PRUEBAS ===");
