# MARRTA Scripting API - Referencia Completa

## Índice

1. [API de Eventos (Events)](#api-de-eventos-events)
2. [API de Defensas (Defenses)](#api-de-defensas-defenses)
3. [API de Etapas (Stages)](#api-de-etapas-stages)
4. [API de Gráficas (Charts)](#api-de-gráficas-charts)
5. [Objeto Global Marrta](#objeto-global-marrta)
6. [Valores Calculados vs Base](#valores-calculados-vs-base)
7. [Literales de Texto](#literales-de-texto)

---

## API de Eventos (Events)

### Métodos de Lectura

#### `getAllEvents()`
Obtiene todos los eventos del modelo.

**Retorna:** Array de objetos Event

**Ejemplo:**
```javascript
var events = Events.getAllEvents();
console.log("Total de eventos: " + events.length);
```

#### `getEventsByStage(stageId)`
Obtiene eventos de una etapa específica.

**Parámetros:**
- `stageId` (Number): ID de la etapa

**Retorna:** Array de objetos Event

**Ejemplo:**
```javascript
var events = Events.getEventsByStage(1);
console.log("Eventos en etapa 1: " + events.length);
```

#### `getEventsByStageGrouped()`
Agrupa eventos por nombre de etapa.

**Retorna:** Object con estructura `{ nombreEtapa: [eventos] }`

**Ejemplo:**
```javascript
var grouped = Events.getEventsByStageGrouped();
for (var stageName in grouped) {
    console.log(stageName + ": " + grouped[stageName].length + " eventos");
}
```

#### `countEventsByStage()`
Cuenta eventos por etapa.

**Retorna:** Object con estructura `{ nombreEtapa: count }`

**Ejemplo:**
```javascript
var counts = Events.countEventsByStage();
for (var stageName in counts) {
    console.log(stageName + ": " + counts[stageName] + " eventos");
}
```

#### `getEventsByConsequence(level)`
Filtra eventos por nivel de consecuencia.

**Parámetros:**
- `level` (Number): Nivel de consecuencia (0-3)

**Retorna:** Array de objetos Event

#### `getEventsByHumanError(isHumanError)`
Filtra eventos por si involucran error humano.

**Parámetros:**
- `isHumanError` (Boolean): true para eventos con error humano

**Retorna:** Array de objetos Event

#### `getEventStatistics()`
Obtiene estadísticas globales de eventos.

**Retorna:** Object con propiedades:
- `total` (Number): Total de eventos
- `humanError` (Number): Eventos con error humano
- `noHumanError` (Number): Eventos sin error humano
- `byConsequence` (Object): Conteo por nivel de consecuencia
- `byRisk` (Object): Conteo por nivel de riesgo

### Métodos de Escritura

#### `updateEvent(eventId, properties)`
Actualiza propiedades de un evento específico.

**Parámetros:**
- `eventId` (Number): ID del evento a actualizar
- `properties` (Object): Objeto con propiedades a actualizar

**Propiedades actualizables:**
- `nombre` (String): Nombre del evento
- `descripcion` (String): Descripción del evento
- `codigo` (String): Código del evento
- `frecuenciaBase` (Number): Nivel de frecuencia base (0-3)
- `consecuenciaBase` (Number): Nivel de consecuencia base (0-3)
- `etapaId` (Number): ID de la etapa
- `subetapaId` (Number): ID de la subetapa
- `errorHumano` (Boolean): Si involucra error humano
- `completado` (Boolean): Si el evento es aplicable

**Retorna:** true si se actualizó correctamente

**Ejemplo:**
```javascript
Events.updateEvent(5, {
    nombre: "Nuevo nombre",
    descripcion: "Nueva descripción actualizada",
    frecuenciaBase: 2,
    errorHumano: true,
    completado: true
});
```

#### `setAllEventsApplicable()`
Marca todos los eventos como aplicables (completado=true).

**Retorna:** Number - Cantidad de eventos actualizados

**Ejemplo:**
```javascript
var count = Events.setAllEventsApplicable();
console.log("Marcados " + count + " eventos como aplicables");
```

#### `setAllEventsNotApplicable()`
Marca todos los eventos como no aplicables (completado=false).

**Retorna:** Number - Cantidad de eventos actualizados

**Ejemplo:**
```javascript
var count = Events.setAllEventsNotApplicable();
console.log("Marcados " + count + " eventos como no aplicables");
```

#### `searchReplaceInDescriptions(searchTerm, replaceTerm, caseSensitive)`
Busca y reemplaza texto en las descripciones de todos los eventos.

**Parámetros:**
- `searchTerm` (String): Texto a buscar
- `replaceTerm` (String): Texto de reemplazo
- `caseSensitive` (Boolean): Si la búsqueda distingue mayúsculas (opcional, default: false)

**Retorna:** Number - Cantidad de eventos actualizados

**Ejemplo:**
```javascript
// Reemplazar "paciente" por "usuario" en todas las descripciones
var count = Events.searchReplaceInDescriptions("paciente", "usuario", false);
console.log("Actualizadas " + count + " descripciones");
```

### Propiedades del Objeto Event

```javascript
{
    id: Number,                    // ID único del evento
    codigo: String,                // Código del evento
    nombre: String,                // Nombre del evento
    descripcion: String,           // Descripción detallada
    etapa: String,                 // Nombre de la etapa
    etapaId: Number,               // ID de la etapa
    subetapa: String,              // Nombre de la subetapa
    subetapaId: Number,            // ID de la subetapa
    
    // Valores base (de la base de datos)
    frecuenciaBase: Number,        // Frecuencia original (0-3)
    consecuenciaBase: Number,      // Consecuencia original (0-3)
    
    // Valores calculados (con defensas activas)
    frecuencia: Number,            // Frecuencia ajustada (0-3)
    frecuenciaTexto: String,       // FMB, FB, FM, FA
    probabilidad: Number,          // Probabilidad calculada (0-3)
    probabilidadTexto: String,     // PMB, PB, PM, PA
    consecuencia: Number,          // Consecuencia ajustada (0-3)
    consecuenciaTexto: String,     // CB, CM, CA, CMA
    riesgo: Number,                // Riesgo calculado (0-3)
    riesgoTexto: String,           // RB, RM, RA, RMA
    
    // Otros
    errorHumano: Boolean,          // Involucra error humano
    base: Boolean,                 // Es evento base
    completado: Boolean            // Es aplicable (alias: aplicable)
}
```

---

## API de Defensas (Defenses)

### Métodos Disponibles

#### `getAllDefenses(eventId)`
Obtiene todas las defensas, opcionalmente filtradas por evento.

**Parámetros:**
- `eventId` (Number, opcional): ID del evento (0 o undefined para todas)

**Retorna:** Array de objetos Defense

#### `getBarriers(eventId)`
Obtiene solo las barreras.

**Parámetros:**
- `eventId` (Number, opcional): ID del evento

**Retorna:** Array de objetos Defense (tipo 0)

#### `getFrequencyReducers(eventId)`
Obtiene solo los reductores de frecuencia.

**Parámetros:**
- `eventId` (Number, opcional): ID del evento

**Retorna:** Array de objetos Defense (tipo 1)

#### `getConsequenceReducers(eventId)`
Obtiene solo los reductores de consecuencia.

**Parámetros:**
- `eventId` (Number, opcional): ID del evento

**Retorna:** Array de objetos Defense (tipo 2)

#### `getDefensesByTypeGrouped()`
Agrupa todas las defensas por tipo.

**Retorna:** Object con propiedades:
- `barriers` (Array): Barreras
- `frequencyReducers` (Array): Reductores de frecuencia
- `consequenceReducers` (Array): Reductores de consecuencia

#### `getDefenseStatistics()`
Obtiene estadísticas de defensas.

**Retorna:** Object con propiedades:
- `total` (Number): Total de defensas
- `active` (Number): Defensas activas
- `inactive` (Number): Defensas inactivas
- `human` (Number): Defensas basadas en humanos
- `nonHuman` (Number): Defensas no basadas en humanos
- `byType` (Object): Conteo por tipo
- `byHardiness` (Object): Conteo por robustez

#### `getDefensesByEvent(eventId)`
Obtiene defensas de un evento específico.

**Parámetros:**
- `eventId` (Number): ID del evento

**Retorna:** Array de objetos Defense

#### `getDefensesByStage(stageName)`
Obtiene defensas asociadas a una etapa.

**Parámetros:**
- `stageName` (String): Nombre de la etapa

**Retorna:** Array de objetos Defense

#### `getDefensesByHardiness(hardiness)`
Filtra defensas por nivel de robustez.

**Parámetros:**
- `hardiness` (Number): Nivel de robustez (0-3)

**Retorna:** Array de objetos Defense

#### `countDefensesByType()`
Cuenta defensas por tipo.

**Retorna:** Object con conteos por tipo

#### `getActiveDefenses()`
Obtiene solo las defensas activas.

**Retorna:** Array de objetos Defense

#### `getInactiveDefenses()`
Obtiene solo las defensas inactivas.

**Retorna:** Array de objetos Defense

### Propiedades del Objeto Defense

```javascript
{
    id: Number,                    // ID único de la defensa
    eventId: Number,               // ID del evento asociado
    codigo: String,                // Código de la defensa
    nombre: String,                // Nombre de la defensa
    descripcion: String,           // Descripción detallada
    type: Number,                  // 0=Barrera, 1=Red.Frec, 2=Red.Cons
    hardiness: Number,             // Robustez (0-3)
    hardinessString: String,       // "Blanda", "Normal", "Robusta", "Muy robusta"
    active: Boolean,               // Si está activa
    orderStage: String,            // Etapa de orden
    humanBased: Boolean            // Si es basada en humanos
}
```

---

## API de Etapas (Stages)

### Métodos de Lectura

#### `getAllStages()`
Obtiene todas las etapas del modelo, incluyendo etapas instrumentales (ID < 0).

**Retorna:** Array de objetos Stage

**Ejemplo:**
```javascript
var stages = Stages.getAllStages();
// Separar por tipo
var processStages = stages.filter(s => s.id > 0);
var instrumentalStages = stages.filter(s => s.id < 0);
console.log("Etapas del proceso: " + processStages.length);
console.log("Etapas instrumentales: " + instrumentalStages.length);
```

#### `getAllSubstages()`
Obtiene todas las subetapas.

**Retorna:** Array de objetos Substage

#### `getStageById(stageId)`
Obtiene una etapa por su ID.

**Parámetros:**
- `stageId` (Number): ID de la etapa (puede ser negativo)

**Retorna:** Object Stage o null

#### `getSubstageById(substageId)`
Obtiene una subetapa por su ID.

**Parámetros:**
- `substageId` (Number): ID de la subetapa

**Retorna:** Object Substage o null

#### `getSubstagesByStage(stageId)`
Obtiene las subetapas de una etapa específica.

**Parámetros:**
- `stageId` (Number): ID de la etapa

**Retorna:** Array de objetos Substage

#### `getStagesWithSubstages()`
Obtiene todas las etapas con sus subetapas anidadas.

**Retorna:** Object con estructura `{ nombreEtapa: { stage, substages } }`

#### `countSubstagesByStage()`
Cuenta subetapas por etapa.

**Retorna:** Object con estructura `{ nombreEtapa: count }`

### Métodos de Escritura

#### `updateStage(stageId, properties)`
Actualiza propiedades de una etapa.

**Parámetros:**
- `stageId` (Number): ID de la etapa
- `properties` (Object): Propiedades a actualizar

**Propiedades actualizables:**
- `nombre` (String): Nombre de la etapa
- `descripcion` (String): Descripción
- `codigo` (String): Código
- `orden` (Number): Orden de visualización

**Retorna:** true si se actualizó correctamente

**Ejemplo:**
```javascript
Stages.updateStage(3, {
    nombre: "Nueva etapa actualizada",
    orden: 10
});
```

### Propiedades del Objeto Stage

```javascript
{
    id: Number,                    // ID de la etapa (puede ser < 0 para instrumentales)
    nombre: String,                // Nombre de la etapa
    codigo: String,                // Código de la etapa
    descripcion: String,           // Descripción
    orden: Number,                 // Orden de visualización
    activo: Boolean                // Si está activa
}
```

### Propiedades del Objeto Substage

```javascript
{
    id: Number,                    // ID de la subetapa
    nombre: String,                // Nombre de la subetapa
    descripcion: String,           // Descripción
    etapaId: Number,               // ID de la etapa padre
    activo: Boolean,               // Si está activa
    orden: Number                  // Orden de visualización
}
```

### Nota sobre Etapas Instrumentales

Las etapas pueden tener ID negativo. Estas son "etapas instrumentales" usadas para asociar barreras, no son etapas del proceso normal. Siempre verifica el ID:

```javascript
var stages = Stages.getAllStages();
var processStages = stages.filter(s => s.id > 0).sort((a,b) => a.orden - b.orden);
var instrumentalStages = stages.filter(s => s.id < 0).sort((a,b) => a.orden - b.orden);
```

---

## API de Gráficas (Charts)

### Métodos de Utilidad

#### `generateTextBarChart(title, data)`
Genera un gráfico de barras ASCII.

**Parámetros:**
- `title` (String): Título del gráfico
- `data` (Object): Datos `{ label: value }`

**Retorna:** String con el gráfico

#### `generateTable(title, headers, rows)`
Genera una tabla formateada.

**Parámetros:**
- `title` (String): Título de la tabla
- `headers` (Array): Array de encabezados
- `rows` (Array): Array de arrays con datos

**Retorna:** String con la tabla

#### `sortByProperty(array, property, ascending)`
Ordena un array por una propiedad.

**Parámetros:**
- `array` (Array): Array a ordenar
- `property` (String): Nombre de la propiedad
- `ascending` (Boolean): true para ascendente

**Retorna:** Array ordenado

#### `groupByProperty(array, property)`
Agrupa elementos por una propiedad.

**Parámetros:**
- `array` (Array): Array a agrupar
- `property` (String): Nombre de la propiedad

**Retorna:** Object con grupos

#### `countByProperty(array, property)`
Cuenta ocurrencias por propiedad.

**Parámetros:**
- `array` (Array): Array a contar
- `property` (String): Nombre de la propiedad

**Retorna:** Object con conteos

#### `filterByProperty(array, property, value)`
Filtra array por valor de propiedad.

**Parámetros:**
- `array` (Array): Array a filtrar
- `property` (String): Nombre de la propiedad
- `value` (Any): Valor a buscar

**Retorna:** Array filtrado

#### `getNumericStats(array, property)`
Calcula estadísticas numéricas.

**Parámetros:**
- `array` (Array): Array de objetos
- `property` (String): Nombre de la propiedad numérica

**Retorna:** Object con:
- `min` (Number): Valor mínimo
- `max` (Number): Valor máximo
- `sum` (Number): Suma total
- `avg` (Number): Promedio
- `count` (Number): Cantidad de valores

#### `createBarChart(title, categories, values)`
Crea estructura de gráfico de barras.

**Retorna:** Object con estructura del gráfico

#### `createPieChart(title, labels, values)`
Crea estructura de gráfico circular.

**Retorna:** Object con estructura del gráfico

---

## Objeto Global Marrta

El objeto `Marrta` proporciona acceso a la ventana principal de la aplicación y funciones de control general.

### Métodos de Control de Interfaz

#### `refreshMainScreen()`
Refresca la pantalla principal para reflejar los cambios realizados por scripts.

**Descripción:**
- Actualiza la lista de eventos iniciadores
- Refresca los detalles del evento actualmente seleccionado
- Recalcula y muestra valores calculados (frecuencia, probabilidad, consecuencia, riesgo)

**Cuándo usar:**
Después de modificar eventos, defensas o etapas mediante las APIs de scripting, para que los cambios se vean reflejados en la interfaz.

**Ejemplo:**
```javascript
// Modificar algunas defensas
Defenses.setDefenseActive(1, false);
Defenses.setDefenseActive(2, true);

// Refrescar la pantalla para ver los cambios
Marrta.refreshMainScreen();
```

#### `saveModelChanges()`
Guarda los cambios efectuados en el modelo en la base de datos.

**Descripción:**
- Guarda todos los cambios pendientes en disco
- No muestra diálogos de confirmación
- Ideal para scripts de procesamiento por lotes
- Persiste cambios en eventos, defensas y etapas

**Cuándo usar:**
Después de hacer modificaciones mediante scripts que deseas persistir en la base de datos.

**Ejemplo:**
```javascript
// Hacer cambios en el modelo
Events.updateEvent(eventId, {nombre: "Nuevo nombre"});
Defenses.enableAllDefenses();

// Refrescar la pantalla
Marrta.refreshMainScreen();

// Guardar los cambios en disco
Marrta.saveModelChanges();
```

### Flujo de Trabajo Típico

```javascript
// 1. Hacer cambios en el modelo
Events.setAllEventsApplicable();
Defenses.setDefenseActive(someId, true);

// 2. Refrescar la pantalla para ver los cambios
Marrta.refreshMainScreen();

// 3. Guardar los cambios en disco
Marrta.saveModelChanges();
```

### Otros Métodos Disponibles

```javascript
Marrta.toggleActualEvent()
// Alterna el estado del evento actualmente seleccionado

Marrta.alternarDefensasEventoActual(tipoDefensa, habilitar)
// Cambia el estado de las defensas del evento actual
// tipoDefensa: 0=Reductores Frecuencia, 1=Barreras, 2=Reductores Consecuencia

Marrta.alternarDefensaModelo(tipoDefensa, habilitar, pos)
// Cambia el estado de defensas en todo el modelo
```

**Nota:** Los métodos `toggleActualEvent` y `alternar*` están pensados para uso interno de la aplicación. Para gestión desde scripts, es recomendable usar las APIs específicas (Events, Defenses, Stages) seguido de `refreshMainScreen()` y `saveModelChanges()` según sea necesario.

---

## Valores Calculados vs Base

Los eventos incluyen tanto valores **base** (almacenados en la base de datos) como valores **calculados** (considerando defensas activas):

### Valores Base
- `frecuenciaBase`: Frecuencia asignada originalmente (0-3)
- `consecuenciaBase`: Consecuencia asignada originalmente (0-3)

### Valores Calculados

#### Probabilidad
Se calcula a partir de las **barreras** activas y su robustez:
```
Probabilidad = f(Barreras activas y sus niveles de robustez)
```

#### Frecuencia
Se calcula desde la frecuencia base, ajustada por **reductores de frecuencia**:
```
Frecuencia = f(FrecuenciaBase, ErrorHumano, Reductores de Frecuencia)
```

#### Consecuencia
Se calcula desde la consecuencia base, ajustada por **reductores de consecuencia**:
```
Consecuencia = f(ConsecuenciaBase, Reductores de Consecuencia)
```

#### Riesgo
Se calcula aplicando la **matriz de riesgo** a P, F, C calculados:
```
Riesgo = MatrizRiesgo(Probabilidad, Frecuencia, Consecuencia)
```

### Ejemplo de Uso

```javascript
var events = Events.getAllEvents();
var event = events[0];

console.log("=== Valores Base ===");
console.log("Frecuencia Base: " + event.frecuenciaBase);
console.log("Consecuencia Base: " + event.consecuenciaBase);

console.log("\n=== Valores Calculados (con defensas) ===");
console.log("Frecuencia: " + event.frecuencia + " (" + event.frecuenciaTexto + ")");
console.log("Probabilidad: " + event.probabilidad + " (" + event.probabilidadTexto + ")");
console.log("Consecuencia: " + event.consecuencia + " (" + event.consecuenciaTexto + ")");
console.log("Riesgo: " + event.riesgo + " (" + event.riesgoTexto + ")");
```

---

## Literales de Texto

Todos los niveles numéricos (0-3) tienen representaciones textuales para facilitar la lectura:

### Frecuencia (frecuenciaTexto)
- **FMB** - Muy baja
- **FB** - Baja
- **FM** - Media
- **FA** - Alta

### Probabilidad (probabilidadTexto)
- **PMB** - Muy baja
- **PB** - Baja
- **PM** - Media
- **PA** - Alta

### Consecuencia (consecuenciaTexto)
- **CB** - Bajas
- **CM** - Medias
- **CA** - Altas
- **CMA** - Muy altas

### Riesgo (riesgoTexto)
- **RB** - Bajo
- **RM** - Medio
- **RA** - Alto
- **RMA** - Muy Alto

### Robustez de Defensas (hardinessString)
- **Blanda** (0)
- **Normal** (1)
- **Robusta** (2)
- **Muy robusta** (3)

### Ejemplo de Visualización

```javascript
var events = Events.getAllEvents();
console.log("Código | Frec | Prob | Cons | Riesgo");
console.log("-------|------|------|------|-------");

for (var i = 0; i < Math.min(10, events.length); i++) {
    var e = events[i];
    console.log(
        e.codigo + " | " +
        e.frecuenciaTexto + " | " +
        e.probabilidadTexto + " | " +
        e.consecuenciaTexto + " | " +
        e.riesgoTexto
    );
}
```

---

## Consejos y Mejores Prácticas

### 1. Siempre Verificar que el Modelo Esté Cargado

```javascript
if (!Marrta.isModelLoaded) {
    console.log("ERROR: No hay modelo cargado");
    return;
}
```

### 2. Manejar Etapas Instrumentales Correctamente

```javascript
var stages = Stages.getAllStages();
var processStages = stages.filter(s => s.id > 0).sort((a,b) => a.orden - b.orden);
var instrumentalStages = stages.filter(s => s.id < 0).sort((a,b) => a.orden - b.orden);
```

### 3. Usar Métodos de Escritura con Precaución

Los cambios son permanentes y se guardan inmediatamente en la base de datos:

```javascript
// MAL - No confirmar antes de cambios masivos
Events.setAllEventsNotApplicable();

// BIEN - Confirmar primero
if (confirm("¿Marcar todos los eventos como no aplicables?")) {
    var count = Events.setAllEventsNotApplicable();
    console.log("Actualizados " + count + " eventos");
}
```

### 4. Aprovechar los Literales de Texto

```javascript
// En lugar de interpretar números
if (event.riesgo === 3) { ... }

// Usa los literales de texto
console.log("Evento " + event.codigo + " tiene riesgo " + event.riesgoTexto);
```

### 5. Usar Charts para Visualización

```javascript
var events = Events.getAllEvents();
var riskCounts = Charts.countByProperty(events, 'riesgoTexto');
console.log(Charts.generateTextBarChart('Distribución de Riesgos', riskCounts));
```

---

## Ejemplos Completos

### Ejemplo 1: Análisis de Riesgos por Etapa

```javascript
var events = Events.getAllEvents();
var stages = Stages.getAllStages().filter(s => s.id > 0);

stages.sort((a,b) => a.orden - b.orden);

console.log("=== ANÁLISIS DE RIESGOS POR ETAPA ===\n");

for (var i = 0; i < stages.length; i++) {
    var stage = stages[i];
    var stageEvents = events.filter(e => e.etapaId === stage.id);
    
    if (stageEvents.length === 0) continue;
    
    var highRisk = stageEvents.filter(e => e.riesgo >= 2).length;
    var avgRisk = stageEvents.reduce((sum, e) => sum + e.riesgo, 0) / stageEvents.length;
    
    console.log(stage.nombre);
    console.log("  Total eventos: " + stageEvents.length);
    console.log("  Riesgo alto/muy alto: " + highRisk);
    console.log("  Riesgo promedio: " + avgRisk.toFixed(2) + "\n");
}
```

### Ejemplo 2: Efectividad de Defensas

```javascript
var events = Events.getAllEvents();

console.log("=== ANÁLISIS DE EFECTIVIDAD DE DEFENSAS ===\n");

for (var i = 0; i < Math.min(5, events.length); i++) {
    var e = events[i];
    
    console.log("Evento: " + e.nombre);
    console.log("  Frecuencia: " + e.frecuenciaBase + " → " + e.frecuencia + " (" + e.frecuenciaTexto + ")");
    console.log("  Consecuencia: " + e.consecuenciaBase + " → " + e.consecuencia + " (" + e.consecuenciaTexto + ")");
    console.log("  Riesgo final: " + e.riesgo + " (" + e.riesgoTexto + ")\n");
}
```

### Ejemplo 3: Actualización Masiva Controlada

```javascript
var events = Events.getAllEvents();
var highRiskEvents = events.filter(e => e.riesgo >= 2);

console.log("Eventos de alto riesgo: " + highRiskEvents.length);

if (confirm("¿Marcar eventos de alto riesgo como aplicables?")) {
    var updated = 0;
    
    for (var i = 0; i < highRiskEvents.length; i++) {
        Events.updateEvent(highRiskEvents[i].id, { completado: true });
        updated++;
    }
    
    console.log("Actualizados " + updated + " eventos");
}
```

---

## Notas Finales

- Todos los métodos están optimizados para rendimiento
- Los cálculos de riesgo replican exactamente la lógica de la aplicación principal
- Las defensas inactivas NO se consideran en los cálculos
- Los cambios mediante API de escritura son permanentes
- Consulta la documentación en línea (Herramientas > Editor de Scripts > Documentación) para más detalles

Para más información y ejemplos, consulta los scripts de prueba en `/test_scripts/`.
