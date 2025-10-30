// Script 2: Análisis de importancia de las defensas
// Calcula métricas de importancia basadas en análisis de sensibilidad de riesgo
// Referencia: https://www.nrc.gov/docs/ML1216/ML12160A479.pdf

console.log("=== ANÁLISIS DE IMPORTANCIA DE DEFENSAS ===\n");
console.log("Calculando métricas de importancia para cada defensa...\n");
console.log("NOTA: Este análisis modifica temporalmente el estado de las defensas");
console.log("      para calcular escenarios de riesgo. Los cambios se revierten al final.\n");

// Obtener todos los eventos y defensas
var allEvents = Events.getAllEvents();
var allDefenses = Defenses.getAllDefenses();

console.log("Total de eventos: " + allEvents.length);
console.log("Total de defensas: " + allDefenses.length + "\n");

// Función auxiliar para calcular el riesgo medio y su histograma
function calculateMeanRiskWithHistogram(events) {
    if (events.length === 0) return {mean: 0, histogram: {0: 0, 1: 0, 2: 0, 3: 0}};
    
    var totalRisk = 0;
    var histogram = {0: 0, 1: 0, 2: 0, 3: 0};
    
    for (var i = 0; i < events.length; i++) {
        var risk = events[i].riesgo;
        totalRisk += risk;
        if (risk >= 0 && risk <= 3) {
            histogram[risk]++;
        }
    }
    
    return {
        mean: totalRisk / events.length,
        histogram: histogram,
        total: events.length
    };
}

// Función para guardar el estado actual de las defensas
function saveDefenseStates(defenses) {
    var states = [];
    for (var i = 0; i < defenses.length; i++) {
        states.push({
            id: defenses[i].id,
            active: defenses[i].active,
            hardiness: defenses[i].hardiness
        });
    }
    return states;
}

// Función para restaurar el estado de las defensas
function restoreDefenseStates(states) {
    for (var i = 0; i < states.length; i++) {
        Defenses.setDefenseActive(states[i].id, states[i].active);
        Defenses.setDefenseHardiness(states[i].id, states[i].hardiness);
    }
}

// Calcular R: valor medio del riesgo total (todos los sucesos)
var RData = calculateMeanRiskWithHistogram(allEvents);
var R = RData.mean;
console.log("R (Riesgo medio de todos los sucesos): " + R.toFixed(4));
console.log("Histograma de riesgos:");
console.log("  Riesgo 0 (Tolerable):   " + RData.histogram[0] + " eventos");
console.log("  Riesgo 1 (Moderado):    " + RData.histogram[1] + " eventos");
console.log("  Riesgo 2 (Importante):  " + RData.histogram[2] + " eventos");
console.log("  Riesgo 3 (Inaceptable): " + RData.histogram[3] + " eventos");
console.log("");

// Estructura para almacenar defensas únicas
// Clave: codigo + "|" + nombre + "|" + hardiness + "|" + type
var uniqueDefenses = {};

// Crear mapa de eventos por ID para acceso rápido
var eventMap = {};
for (var i = 0; i < allEvents.length; i++) {
    eventMap[allEvents[i].id] = allEvents[i];
}

// Agrupar defensas por su identificador único
for (var i = 0; i < allDefenses.length; i++) {
    var defense = allDefenses[i];
    
    // Crear clave única: código + nombre + robustez + tipo
    var uniqueKey = defense.code + "|" + defense.nombre + "|" + defense.hardiness + "|" + defense.type;
    
    if (!uniqueDefenses[uniqueKey]) {
        uniqueDefenses[uniqueKey] = {
            code: defense.code,
            nombre: defense.nombre,
            type: defense.type,
            typeString: defense.typeString,
            hardiness: defense.hardiness,
            hardinessString: defense.hardinessString,
            eventIds: [], // IDs de eventos donde aparece
            defenseIds: [] // IDs de las instancias de esta defensa
        };
    }
    
    // Añadir el evento a la lista si no está ya
    var eventAlreadyAdded = false;
    for (var j = 0; j < uniqueDefenses[uniqueKey].eventIds.length; j++) {
        if (uniqueDefenses[uniqueKey].eventIds[j] === defense.eventId) {
            eventAlreadyAdded = true;
            break;
        }
    }
    
    if (!eventAlreadyAdded) {
        uniqueDefenses[uniqueKey].eventIds.push(defense.eventId);
    }
    
    uniqueDefenses[uniqueKey].defenseIds.push(defense.id);
}

console.log("Defensas únicas identificadas: " + Object.keys(uniqueDefenses).length + "\n");

// Función para calcular métricas de importancia para una defensa
function analyzeDefenseImportance(defense, defenseIndex, totalDefenses) {
    console.log("Analizando defensa " + (defenseIndex + 1) + "/" + totalDefenses + ": " + defense.code);
    
    var results = {
        code: defense.code,
        nombre: defense.nombre,
        type: defense.typeString,
        hardiness: defense.hardiness,
        hardinessString: defense.hardinessString,
        numEvents: defense.eventIds.length
    };
    
    // Identificar eventos con y sin esta defensa
    var eventsWithDefense = [];
    var eventsWithoutDefense = [];
    
    for (var i = 0; i < allEvents.length; i++) {
        var event = allEvents[i];
        var hasDefense = false;
        
        for (var j = 0; j < defense.eventIds.length; j++) {
            if (event.id === defense.eventIds[j]) {
                hasDefense = true;
                break;
            }
        }
        
        if (hasDefense) {
            eventsWithDefense.push(event);
        } else {
            eventsWithoutDefense.push(event);
        }
    }
    
    // Ri: valor medio del riesgo solo de los sucesos en que aparece
    var RiData = calculateMeanRiskWithHistogram(eventsWithDefense);
    results.Ri = RiData.mean;
    results.RiHistogram = RiData.histogram;
    
    // Guardar el estado actual de TODAS las defensas de esta defensa única
    var originalStates = [];
    for (var i = 0; i < defense.defenseIds.length; i++) {
        var defId = defense.defenseIds[i];
        // Encontrar la defensa en allDefenses para obtener su estado actual
        for (var j = 0; j < allDefenses.length; j++) {
            if (allDefenses[j].id === defId) {
                originalStates.push({
                    id: defId,
                    active: allDefenses[j].active,
                    hardiness: allDefenses[j].hardiness
                });
                break;
            }
        }
    }
    
    // R1: Riesgo si la defensa siempre está aplicable (activa)
    // Activar todas las instancias de esta defensa
    for (var i = 0; i < defense.defenseIds.length; i++) {
        Defenses.setDefenseActive(defense.defenseIds[i], true);
    }
    // Recalcular riesgos obteniendo eventos de nuevo (esto consulta la BD y recalcula)
    var eventsR1 = Events.getAllEvents();
    var R1Data = calculateMeanRiskWithHistogram(eventsR1);
    results.R1 = R1Data.mean;
    results.R1Histogram = R1Data.histogram;
    
    // R0: Riesgo si la defensa nunca está aplicable (inactiva)
    // Desactivar todas las instancias de esta defensa
    for (var i = 0; i < defense.defenseIds.length; i++) {
        Defenses.setDefenseActive(defense.defenseIds[i], false);
    }
    // Recalcular riesgos obteniendo eventos de nuevo
    var eventsR0 = Events.getAllEvents();
    var R0Data = calculateMeanRiskWithHistogram(eventsR0);
    results.R0 = R0Data.mean;
    results.R0Histogram = R0Data.histogram;
    
    // R-: Riesgo si la robustez se reduce en 1
    if (defense.hardiness > 0) {
        // Restaurar estado activo y reducir robustez
        for (var i = 0; i < originalStates.length; i++) {
            Defenses.setDefenseActive(originalStates[i].id, originalStates[i].active);
            Defenses.setDefenseHardiness(originalStates[i].id, defense.hardiness - 1);
        }
        // Recalcular riesgos obteniendo eventos de nuevo
        var eventsRminus = Events.getAllEvents();
        var RminusData = calculateMeanRiskWithHistogram(eventsRminus);
        results.Rminus = RminusData.mean;
        results.RminusHistogram = RminusData.histogram;
    } else {
        // Si robustez es 0, no se puede reducir más, usar R0
        results.Rminus = results.R0;
        results.RminusHistogram = results.R0Histogram;
    }
    
    // R+: Riesgo si la robustez se incrementa en 1
    if (defense.hardiness < 3) {
        // Restaurar estado activo y aumentar robustez
        for (var i = 0; i < originalStates.length; i++) {
            Defenses.setDefenseActive(originalStates[i].id, originalStates[i].active);
            Defenses.setDefenseHardiness(originalStates[i].id, defense.hardiness + 1);
        }
        // Recalcular riesgos obteniendo eventos de nuevo
        var eventsRplus = Events.getAllEvents();
        var RplusData = calculateMeanRiskWithHistogram(eventsRplus);
        results.Rplus = RplusData.mean;
        results.RplusHistogram = RplusData.histogram;
    } else {
        // Si robustez es 3, no se puede aumentar más, usar R1
        results.Rplus = results.R1;
        results.RplusHistogram = results.R1Histogram;
    }
    
    // Restaurar el estado original de las defensas
    for (var i = 0; i < originalStates.length; i++) {
        Defenses.setDefenseActive(originalStates[i].id, originalStates[i].active);
        Defenses.setDefenseHardiness(originalStates[i].id, originalStates[i].hardiness);
    }
    
    // Calcular métricas de importancia
    
    // Fussell-Vesely (F-V): mide la contribución relativa al riesgo
    if (R > 0) {
        results.FV = results.Ri / R;
    } else {
        results.FV = 0;
    }
    
    // Risk Reduction Worth (RRW): cuánto aumentaría el riesgo sin la defensa
    if (results.R1 > 0) {
        results.RRW = R / results.R1;
    } else {
        results.RRW = 0;
    }
    
    // Risk Reduction Interval (RRI): diferencia absoluta de riesgo
    results.RRI = R - results.R1;
    
    // Risk Achievement Worth (RAW): cuánto peor sería sin la defensa
    if (R > 0) {
        results.RAW = results.R0 / R;
    } else {
        results.RAW = 0;
    }
    
    // Risk Increase Interval (RII): cuánto aumentaría el riesgo
    results.RII = results.R0 - R;
    
    // Risk Reduction Interval for increased strength (RRI+)
    results.RRIplus = R - results.Rplus;
    
    // Risk Increase Interval for decreased strength (RRI-)
    results.RRIminus = results.Rminus - R;
    
    // Birnbaum Importance (B): sensibilidad del riesgo a la defensa
    if (R > 0) {
        results.B = (results.R0 - results.R1) / R;
    } else {
        results.B = 0;
    }
    
    // Birnbaum Importance for increased strength (B+)
    if (R > 0) {
        results.Bplus = (results.Rminus - results.Rplus) / R;
    } else {
        results.Bplus = 0;
    }
    
    return results;
}

// Guardar el estado original de TODAS las defensas antes de empezar
console.log("Guardando estado original de las defensas...\n");
var allDefenseStates = saveDefenseStates(allDefenses);

// Analizar todas las defensas únicas
var analysisResults = [];
var defenseArray = [];
for (var key in uniqueDefenses) {
    defenseArray.push(uniqueDefenses[key]);
}

console.log("Iniciando análisis de " + defenseArray.length + " defensas únicas...\n");

for (var i = 0; i < defenseArray.length; i++) {
    var defense = defenseArray[i];
    var results = analyzeDefenseImportance(defense, i, defenseArray.length);
    analysisResults.push(results);
}

// Restaurar el estado original de TODAS las defensas al final
console.log("\nRestaurando estado original de las defensas...");
restoreDefenseStates(allDefenseStates);
console.log("Estado restaurado correctamente.\n");

// Ordenar por RAW (Risk Achievement Worth) descendente
// RAW alto indica defensas más importantes (mayor impacto si fallan)
analysisResults.sort(function(a, b) {
    return b.RAW - a.RAW;
});

// Función auxiliar para mostrar histograma
function showHistogram(label, histogram, total) {
    console.log("   " + label + ":");
    console.log("     R0 (Tolerable):   " + histogram[0] + " (" + (100*histogram[0]/total).toFixed(1) + "%)");
    console.log("     R1 (Moderado):    " + histogram[1] + " (" + (100*histogram[1]/total).toFixed(1) + "%)");
    console.log("     R2 (Importante):  " + histogram[2] + " (" + (100*histogram[2]/total).toFixed(1) + "%)");
    console.log("     R3 (Inaceptable): " + histogram[3] + " (" + (100*histogram[3]/total).toFixed(1) + "%)");
}

// Mostrar resultados
console.log("=== RESULTADOS DEL ANÁLISIS DE IMPORTANCIA ===\n");
console.log("Los valores R1, R0, R-, R+ han sido calculados mediante modificación");
console.log("del estado de las defensas y recálculo del riesgo del modelo.\n");

// Mostrar top 20 defensas más importantes
var topN = Math.min(20, analysisResults.length);
console.log("TOP " + topN + " DEFENSAS MÁS IMPORTANTES (por RAW):\n");

for (var i = 0; i < topN; i++) {
    var r = analysisResults[i];
    
    console.log((i + 1) + ". " + r.code + " - " + r.nombre);
    console.log("   Tipo: " + r.type + " | Robustez: " + r.hardinessString);
    console.log("   Aparece en: " + r.numEvents + " evento(s)");
    console.log("");
    console.log("   Valores de riesgo:");
    console.log("   - Ri (riesgo en eventos donde aparece): " + r.Ri.toFixed(4));
    showHistogram("     Histograma Ri", r.RiHistogram, allEvents.length);
    console.log("   - R1 (si siempre aplicable):            " + r.R1.toFixed(4));
    showHistogram("     Histograma R1", r.R1Histogram, allEvents.length);
    console.log("   - R0 (si nunca aplicable):              " + r.R0.toFixed(4));
    showHistogram("     Histograma R0", r.R0Histogram, allEvents.length);
    console.log("   - R- (robustez reducida):               " + r.Rminus.toFixed(4));
    showHistogram("     Histograma R-", r.RminusHistogram, allEvents.length);
    console.log("   - R+ (robustez incrementada):           " + r.Rplus.toFixed(4));
    showHistogram("     Histograma R+", r.RplusHistogram, allEvents.length);
    console.log("");
    console.log("   Métricas de importancia:");
    console.log("   - F-V  (Fussell-Vesely):                " + r.FV.toFixed(4));
    console.log("   - RRW  (Risk Reduction Worth):          " + r.RRW.toFixed(4));
    console.log("   - RRI  (Risk Reduction Interval):       " + r.RRI.toFixed(4));
    console.log("   - RAW  (Risk Achievement Worth):        " + r.RAW.toFixed(4));
    console.log("   - RII  (Risk Increase Interval):        " + r.RII.toFixed(4));
    console.log("   - RRI+ (RRI para mayor robustez):       " + r.RRIplus.toFixed(4));
    console.log("   - RRI- (RII para menor robustez):       " + r.RRIminus.toFixed(4));
    console.log("   - B    (Birnbaum Importance):           " + r.B.toFixed(4));
    console.log("   - B+   (Birnbaum para robustez):        " + r.Bplus.toFixed(4));
    console.log("");
}

// Resumen por tipo de defensa
console.log("\n=== RESUMEN POR TIPO DE DEFENSA ===\n");

var typeGroups = {
    "Reductor de Frecuencia": [],
    "Barrera": [],
    "Reductor de Consecuencia": []
};

for (var i = 0; i < analysisResults.length; i++) {
    typeGroups[analysisResults[i].type].push(analysisResults[i]);
}

for (var typeName in typeGroups) {
    var group = typeGroups[typeName];
    if (group.length === 0) continue;
    
    console.log(typeName + ":");
    console.log("  Total: " + group.length + " defensas");
    
    // Promedios
    var avgRAW = 0, avgRRW = 0, avgFV = 0, avgB = 0;
    for (var i = 0; i < group.length; i++) {
        avgRAW += group[i].RAW;
        avgRRW += group[i].RRW;
        avgFV += group[i].FV;
        avgB += group[i].B;
    }
    avgRAW /= group.length;
    avgRRW /= group.length;
    avgFV /= group.length;
    avgB /= group.length;
    
    console.log("  RAW promedio: " + avgRAW.toFixed(4));
    console.log("  RRW promedio: " + avgRRW.toFixed(4));
    console.log("  F-V promedio: " + avgFV.toFixed(4));
    console.log("  B promedio:   " + avgB.toFixed(4));
    console.log("");
}

// Interpretación de métricas
console.log("\n=== GUÍA DE INTERPRETACIÓN ===\n");
console.log("RAW (Risk Achievement Worth): Indica cuánto peor sería el riesgo si la defensa");
console.log("     fallara completamente. Valores altos (>1.5) indican defensas críticas.");
console.log("");
console.log("RRW (Risk Reduction Worth): Indica el beneficio de tener la defensa.");
console.log("     Valores cercanos a 1 indican mayor importancia.");
console.log("");
console.log("F-V (Fussell-Vesely): Contribución relativa de la defensa al riesgo total.");
console.log("     Valores >0.1 indican defensas significativas.");
console.log("");
console.log("B (Birnbaum): Sensibilidad del riesgo total a cambios en la defensa.");
console.log("     Valores altos indican que pequeños cambios tienen gran impacto.");
console.log("");
console.log("RRI/RII: Intervalos de variación del riesgo (valores absolutos).");
console.log("");

console.log("\n=== FIN DEL ANÁLISIS ===");
