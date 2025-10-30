// Script 1: Listado de defensas agrupadas por tipo
// Lista cada defensa única (mismo ID y parámetros = misma defensa)
// Para cada defensa, muestra los iniciadores en que aparece y el número total de sucesos

console.log("=== LISTADO DE DEFENSAS AGRUPADAS POR TIPO ===\n");

// Obtener todas las defensas
var allDefenses = Defenses.getAllDefenses();
console.log("Total de defensas en el modelo: " + allDefenses.length + "\n");

// Estructura para almacenar defensas únicas
// Clave: codigo + "|" + nombre + "|" + hardiness + "|" + type
var uniqueDefenses = {};

// Agrupar defensas por su identificador único
for (var i = 0; i < allDefenses.length; i++) {
    var defense = allDefenses[i];
    
    // Crear clave única: código + nombre + robustez + tipo
    // Defensas con diferente robustez son consideradas diferentes
    var uniqueKey = defense.code + "|" + defense.nombre + "|" + defense.hardiness + "|" + defense.type;
    
    if (!uniqueDefenses[uniqueKey]) {
        uniqueDefenses[uniqueKey] = {
            code: defense.code,
            nombre: defense.nombre,
            descripcion: defense.descripcion,
            type: defense.type,
            typeString: defense.typeString,
            hardiness: defense.hardiness,
            hardinessString: defense.hardinessString,
            events: [], // Array de eventos donde aparece
            eventCodes: [], // Códigos de los eventos
            totalOccurrences: 0
        };
    }
    
    // Añadir el evento a la lista si no está ya
    var eventAlreadyAdded = false;
    for (var j = 0; j < uniqueDefenses[uniqueKey].events.length; j++) {
        if (uniqueDefenses[uniqueKey].events[j] === defense.eventId) {
            eventAlreadyAdded = true;
            break;
        }
    }
    
    if (!eventAlreadyAdded) {
        uniqueDefenses[uniqueKey].events.push(defense.eventId);
        uniqueDefenses[uniqueKey].totalOccurrences++;
    }
}

// Obtener todos los eventos para poder mostrar sus códigos
var allEvents = Events.getAllEvents();
var eventMap = {};
for (var i = 0; i < allEvents.length; i++) {
    eventMap[allEvents[i].id] = allEvents[i].codigo;
}

// Añadir los códigos de eventos a cada defensa
for (var key in uniqueDefenses) {
    var defense = uniqueDefenses[key];
    for (var i = 0; i < defense.events.length; i++) {
        var eventId = defense.events[i];
        if (eventMap[eventId]) {
            defense.eventCodes.push(eventMap[eventId]);
        }
    }
}

// Agrupar defensas por tipo
var defensesByType = {
    "Reductor de Frecuencia": [],
    "Barrera": [],
    "Reductor de Consecuencia": []
};

for (var key in uniqueDefenses) {
    var defense = uniqueDefenses[key];
    // Asegurarse de que el tipo existe en el objeto antes de hacer push
    if (!defensesByType[defense.typeString]) {
        defensesByType[defense.typeString] = [];
    }
    defensesByType[defense.typeString].push(defense);
}

// Mapeo de tipo a nombre plural para mostrar
var typeDisplayNames = {
    "Reductor de Frecuencia": "REDUCTORES DE FRECUENCIA",
    "Barrera": "BARRERAS",
    "Reductor de Consecuencia": "REDUCTORES DE CONSECUENCIA"
};

// Mostrar resultados por tipo
for (var typeName in defensesByType) {
    var defenses = defensesByType[typeName];
    
    if (defenses.length === 0) {
        console.log(typeName + ": No hay defensas de este tipo\n");
        continue;
    }
    
    var displayName = typeDisplayNames[typeName] || typeName.toUpperCase();
    console.log("=== " + displayName + " ===");
    console.log("Total de defensas únicas: " + defenses.length + "\n");
    
    // Ordenar por código
    defenses.sort(function(a, b) {
        if (a.code < b.code) return -1;
        if (a.code > b.code) return 1;
        return 0;
    });
    
    // Mostrar cada defensa
    for (var i = 0; i < defenses.length; i++) {
        var defense = defenses[i];
        
        console.log((i + 1) + ". Código: " + defense.code);
        console.log("   Nombre: " + defense.nombre);
        console.log("   Robustez: " + defense.hardinessString + " (" + defense.hardiness + ")");
        
        if (defense.descripcion && defense.descripcion.length > 0) {
            // Limitar descripción a 80 caracteres
            var desc = defense.descripcion;
            if (desc.length > 80) {
                desc = desc.substring(0, 77) + "...";
            }
            console.log("   Descripción: " + desc);
        }
        
        console.log("   Aparece en " + defense.totalOccurrences + " suceso(s) iniciador(es):");
        
        // Mostrar códigos de iniciadores (máximo 10 por línea)
        var codes = defense.eventCodes;
        var line = "   ";
        for (var j = 0; j < codes.length; j++) {
            line += codes[j];
            if (j < codes.length - 1) {
                line += ", ";
            }
            
            // Nueva línea cada 10 códigos
            if ((j + 1) % 10 === 0 && j < codes.length - 1) {
                console.log(line);
                line = "   ";
            }
        }
        if (line.trim().length > 0) {
            console.log(line);
        }
        
        console.log("");
    }
    
    console.log("");
}

// Resumen final
console.log("=== RESUMEN ===");
var totalUniqueDefenses = 0;
var typePluralNames = {
    "Reductor de Frecuencia": "Reductores de Frecuencia",
    "Barrera": "Barreras",
    "Reductor de Consecuencia": "Reductores de Consecuencia"
};
for (var typeName in defensesByType) {
    totalUniqueDefenses += defensesByType[typeName].length;
    var pluralName = typePluralNames[typeName] || typeName;
    console.log(pluralName + ": " + defensesByType[typeName].length + " defensas únicas");
}
console.log("Total de defensas únicas en el modelo: " + totalUniqueDefenses);

console.log("\n=== FIN DEL LISTADO ===");
