// Script: Buscar un suceso por código y mostrar sus detalles completos
// Ejemplo: Muestra información detallada de un suceso incluyendo sus defensas

console.log("=== DETALLES DE SUCESO INICIADOR ===\n");

// Código del suceso a buscar (modificar según necesidad)
var codigoBuscado = "TTD-04";

console.log("Buscando suceso con código: " + codigoBuscado + "\n");

// Obtener todos los eventos
var allEvents = Events.getAllEvents();

// Buscar el evento con el código especificado
var eventoEncontrado = null;
for (var i = 0; i < allEvents.length; i++) {
    if (allEvents[i].codigo === codigoBuscado) {
        eventoEncontrado = allEvents[i];
        break;
    }
}

if (!eventoEncontrado) {
    console.log("✗ No se encontró ningún suceso con el código: " + codigoBuscado);
    console.log("\nCódigos disponibles (primeros 10):");
    for (var i = 0; i < Math.min(10, allEvents.length); i++) {
        console.log("  - " + allEvents[i].codigo);
    }
} else {
    console.log("✓ Suceso encontrado\n");
    
    // Tabla de características básicas
    console.log("┌─────────────────────────────────────────────────────────────────┐");
    console.log("│ CARACTERÍSTICAS BÁSICAS                                         │");
    console.log("├─────────────────────────────────────────────────────────────────┤");
    console.log("│ Código:              " + padRight(eventoEncontrado.codigo, 42) + "│");
    console.log("│ Nombre:              " + padRight(eventoEncontrado.nombre, 42) + "│");
    console.log("│ Frecuencia Base:     " + padRight(eventoEncontrado.frecuenciaBase + " (" + obtenerTextoFrecuencia(eventoEncontrado.frecuenciaBase) + ")", 42) + "│");
    console.log("│ Consecuencia Base:   " + padRight(eventoEncontrado.consecuenciaBase + " (" + obtenerTextoConsecuencia(eventoEncontrado.consecuenciaBase) + ")", 42) + "│");
    console.log("│ Riesgo Base:         " + padRight(eventoEncontrado.riesgoBase + " (" + eventoEncontrado.riesgoBaseTexto + ")", 42) + "│");
    console.log("└─────────────────────────────────────────────────────────────────┘");
    console.log("");
    
    // Descripción
    console.log("┌─────────────────────────────────────────────────────────────────┐");
    console.log("│ DESCRIPCIÓN                                                     │");
    console.log("├─────────────────────────────────────────────────────────────────┤");
    var descripcion = eventoEncontrado.descripcion || "(Sin descripción)";
    var lineasDesc = splitIntoLines(descripcion, 63);
    for (var i = 0; i < lineasDesc.length; i++) {
        console.log("│ " + padRight(lineasDesc[i], 63) + "│");
    }
    console.log("└─────────────────────────────────────────────────────────────────┘");
    console.log("");
    
    // Obtener todas las defensas del evento
    var allDefenses = Defenses.getAllDefenses(eventoEncontrado.id);
    
    // Filtrar por tipo
    var reductoresFrecuencia = [];
    var barreras = [];
    var reductoresConsecuencia = [];
    
    for (var i = 0; i < allDefenses.length; i++) {
        var def = allDefenses[i];
        if (def.type === 0) {
            reductoresFrecuencia.push(def);
        } else if (def.type === 1) {
            barreras.push(def);
        } else if (def.type === 2) {
            reductoresConsecuencia.push(def);
        }
    }
    
    // Tabla de Reductores de Frecuencia
    console.log("┌────────────────────────────────────────────────────────────────────────────┐");
    console.log("│ REDUCTORES DE FRECUENCIA                                                   │");
    console.log("├──────────────┬───────────────────────────┬──────────────────┬──────────────┤");
    console.log("│ Código       │ Nombre                    │ Robustez         │ Estado       │");
    console.log("├──────────────┼───────────────────────────┼──────────────────┼──────────────┤");
    
    if (reductoresFrecuencia.length === 0) {
        console.log("│              │ (Sin reductores)          │                  │              │");
    } else {
        for (var i = 0; i < reductoresFrecuencia.length; i++) {
            var def = reductoresFrecuencia[i];
            var codigo = truncate(def.code, 12);
            var nombre = truncate(def.nombre, 25);
            var robustez = def.hardinessString + " (" + def.hardiness + ")";
            var estado = def.active ? "Aplicada" : "No aplicada";
            console.log("│ " + padRight(codigo, 12) + " │ " + padRight(nombre, 25) + " │ " + padRight(robustez, 16) + " │ " + padRight(estado, 12) + " │");
        }
    }
    console.log("└──────────────┴───────────────────────────┴──────────────────┴──────────────┘");
    console.log("");
    
    // Tabla de Barreras
    console.log("┌────────────────────────────────────────────────────────────────────────────┐");
    console.log("│ BARRERAS                                                                   │");
    console.log("├──────────────┬───────────────────────────┬──────────────────┬──────────────┤");
    console.log("│ Código       │ Nombre                    │ Robustez         │ Estado       │");
    console.log("├──────────────┼───────────────────────────┼──────────────────┼──────────────┤");
    
    if (barreras.length === 0) {
        console.log("│              │ (Sin barreras)            │                  │              │");
    } else {
        for (var i = 0; i < barreras.length; i++) {
            var def = barreras[i];
            var codigo = truncate(def.code, 12);
            var nombre = truncate(def.nombre, 25);
            var robustez = def.hardinessString + " (" + def.hardiness + ")";
            var estado = def.active ? "Aplicada" : "No aplicada";
            console.log("│ " + padRight(codigo, 12) + " │ " + padRight(nombre, 25) + " │ " + padRight(robustez, 16) + " │ " + padRight(estado, 12) + " │");
        }
    }
    console.log("└──────────────┴───────────────────────────┴──────────────────┴──────────────┘");
    console.log("");
    
    // Tabla de Reductores de Consecuencia
    console.log("┌────────────────────────────────────────────────────────────────────────────┐");
    console.log("│ REDUCTORES DE CONSECUENCIA                                                 │");
    console.log("├──────────────┬───────────────────────────┬──────────────────┬──────────────┤");
    console.log("│ Código       │ Nombre                    │ Robustez         │ Estado       │");
    console.log("├──────────────┼───────────────────────────┼──────────────────┼──────────────┤");
    
    if (reductoresConsecuencia.length === 0) {
        console.log("│              │ (Sin reductores)          │                  │              │");
    } else {
        for (var i = 0; i < reductoresConsecuencia.length; i++) {
            var def = reductoresConsecuencia[i];
            var codigo = truncate(def.code, 12);
            var nombre = truncate(def.nombre, 25);
            var robustez = def.hardinessString + " (" + def.hardiness + ")";
            var estado = def.active ? "Aplicada" : "No aplicada";
            console.log("│ " + padRight(codigo, 12) + " │ " + padRight(nombre, 25) + " │ " + padRight(robustez, 16) + " │ " + padRight(estado, 12) + " │");
        }
    }
    console.log("└──────────────┴───────────────────────────┴──────────────────┴──────────────┘");
    console.log("");
    
    // Tabla de valores calculados con defensas
    console.log("┌─────────────────────────────────────────────────────────────────┐");
    console.log("│ VALORES CALCULADOS (CON DEFENSAS)                              │");
    console.log("├─────────────────────────────────────────────────────────────────┤");
    console.log("│ Frecuencia Calculada:    " + padRight(eventoEncontrado.frecuencia + " (" + eventoEncontrado.frecuenciaTexto + ")", 36) + "│");
    console.log("│ Probabilidad:            " + padRight(eventoEncontrado.probabilidad + " (" + eventoEncontrado.probabilidadTexto + ")", 36) + "│");
    console.log("│ Consecuencia Calculada:  " + padRight(eventoEncontrado.consecuencia + " (" + eventoEncontrado.consecuenciaTexto + ")", 36) + "│");
    console.log("│ Riesgo Calculado:        " + padRight(eventoEncontrado.riesgo + " (" + eventoEncontrado.riesgoTexto + ")", 36) + "│");
    console.log("└─────────────────────────────────────────────────────────────────┘");
    console.log("");
    
    // Resumen de defensas
    var totalAplicadas = 0;
    var totalNoAplicadas = 0;
    for (var i = 0; i < allDefenses.length; i++) {
        if (allDefenses[i].active) {
            totalAplicadas++;
        } else {
            totalNoAplicadas++;
        }
    }
    
    console.log("RESUMEN:");
    console.log("- Reductores de Frecuencia: " + reductoresFrecuencia.length);
    console.log("- Barreras: " + barreras.length);
    console.log("- Reductores de Consecuencia: " + reductoresConsecuencia.length);
    console.log("- Total de defensas: " + allDefenses.length);
    console.log("- Defensas aplicadas: " + totalAplicadas);
    console.log("- Defensas no aplicadas: " + totalNoAplicadas);
}

console.log("\n=== FIN ===");

// Funciones auxiliares

// Funciones para convertir valores numéricos a texto según MARRTA
function obtenerTextoFrecuencia(nivel) {
    var textos = ["FMB", "FB", "FM", "FA"];
    if (nivel >= 0 && nivel < textos.length) {
        return textos[nivel];
    }
    return "F" + nivel;
}

function obtenerTextoProbabilidad(nivel) {
    var textos = ["PMB", "PB", "PM", "PA"];
    if (nivel >= 0 && nivel < textos.length) {
        return textos[nivel];
    }
    return "P" + nivel;
}

function obtenerTextoConsecuencia(nivel) {
    var textos = ["CB", "CM", "CA", "CMA"];
    if (nivel >= 0 && nivel < textos.length) {
        return textos[nivel];
    }
    return "C" + nivel;
}

function obtenerTextoRiesgo(nivel) {
    var textos = ["RB", "RM", "RA", "RMA"];
    if (nivel >= 0 && nivel < textos.length) {
        return textos[nivel];
    }
    return "R" + nivel;
}

function padRight(str, length) {
    str = String(str);
    while (str.length < length) {
        str += " ";
    }
    return str;
}

function truncate(str, maxLength) {
    str = String(str);
    if (str.length > maxLength) {
        return str.substring(0, maxLength - 3) + "...";
    }
    return str;
}

function splitIntoLines(text, maxWidth) {
    var words = text.split(' ');
    var lines = [];
    var currentLine = '';
    
    for (var i = 0; i < words.length; i++) {
        var word = words[i];
        if (currentLine.length + word.length + 1 <= maxWidth) {
            if (currentLine.length > 0) {
                currentLine += ' ';
            }
            currentLine += word;
        } else {
            if (currentLine.length > 0) {
                lines.push(currentLine);
            }
            currentLine = word;
        }
    }
    
    if (currentLine.length > 0) {
        lines.push(currentLine);
    }
    
    return lines;
}

// Nota: La función calcularRiesgoBase() ya no es necesaria porque el riesgo base
// ahora se calcula automáticamente en la API de Events y está disponible como
// evento.riesgoBase y evento.riesgoBaseTexto
