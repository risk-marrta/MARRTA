# Implementación de Refresco de Pantalla y Guardado desde Scripts

## Resumen

Se han agregado dos nuevos métodos Q_INVOKABLE al objeto `Marrta` para permitir que los scripts:
1. **Refresquen la pantalla principal** para mostrar los cambios realizados
2. **Guarden los cambios en disco** sin diálogos de confirmación

## Cambios Realizados

### 1. mainwindow.h
Se agregaron dos nuevos métodos Q_INVOKABLE:

```cpp
Q_INVOKABLE void refreshMainScreen();
Q_INVOKABLE void saveModelChanges();
```

### 2. mainwindow.cpp
Se implementaron los dos nuevos métodos:

#### `refreshMainScreen()`
```cpp
void MainWindow::refreshMainScreen()
{
    // Refresh the event list
    mostrarEventosSeleccion();
    // Refresh the current event details if one is selected
    if (actualCompleteEvent != nullptr) {
        mostrarEvento();
    }
}
```

**Funcionalidad:**
- Actualiza la lista de eventos iniciadores mediante `mostrarEventosSeleccion()`
- Si hay un evento seleccionado, refresca sus detalles mediante `mostrarEvento()`
- Recalcula y muestra todos los valores calculados (frecuencia, probabilidad, consecuencia, riesgo)

#### `saveModelChanges()`
```cpp
void MainWindow::saveModelChanges()
{
    Database::DbEvent dbEvent;
    dbEvent.checkAndUpdate(events);
}
```

**Funcionalidad:**
- Guarda todos los cambios pendientes en la base de datos
- No muestra diálogos de confirmación
- Utiliza el mismo mecanismo de guardado que el método `saveModel()` pero sin la interfaz de usuario

### 3. Documentación

#### test_scripts/test_refresh_save.js (NUEVO)
Script de prueba completo que demuestra:
- Verificación de disponibilidad de los nuevos métodos
- Ejemplo de modificación de defensas y refresco de pantalla
- Flujo de trabajo completo con scripts
- Integración con otras APIs (Events, Defenses, Stages)

#### test_scripts/test_marrta_api.js (ACTUALIZADO)
- Se agregaron referencias a los nuevos métodos en la lista de métodos disponibles
- Se actualizó la documentación de uso del objeto Marrta

#### test_scripts/README.md (ACTUALIZADO)
- Se documentaron los nuevos métodos en la sección del objeto Marrta
- Se agregó test_refresh_save.js al orden de ejecución recomendado

#### SCRIPTING_API_REFERENCE.md (ACTUALIZADO)
- Se expandió significativamente la sección "Objeto Global Marrta"
- Se documentaron los dos nuevos métodos con ejemplos de uso
- Se agregó una sección "Flujo de Trabajo Típico"
- Se explicó cuándo y cómo usar cada método

## Uso desde JavaScript

### Ejemplo Básico

```javascript
// 1. Hacer cambios en el modelo
Events.updateEvent(eventId, {nombre: "Nuevo nombre"});
Defenses.setDefenseActive(someId, true);

// 2. Refrescar la pantalla para ver los cambios
Marrta.refreshMainScreen();

// 3. Guardar los cambios en disco
Marrta.saveModelChanges();
```

### Ejemplo Completo

```javascript
// Desactivar todas las defensas de un evento
var defenses = Defenses.getDefensesByEvent(eventId);
for (var i = 0; i < defenses.length; i++) {
    Defenses.setDefenseActive(defenses[i].id, false);
}

// Refrescar la pantalla para ver cómo cambian los valores calculados
Marrta.refreshMainScreen();

// Si estamos satisfechos con los cambios, guardarlos
Marrta.saveModelChanges();

console.log("Cambios guardados exitosamente");
```

## Casos de Uso

### 1. Procesamiento por Lotes
Los scripts pueden ahora modificar múltiples elementos y luego guardar todos los cambios de una vez:

```javascript
// Marcar todos los eventos como aplicables
Events.setAllEventsApplicable();

// Activar todas las defensas
Defenses.enableAllDefenses();

// Refrescar y guardar
Marrta.refreshMainScreen();
Marrta.saveModelChanges();
```

### 2. Automatización de Flujos de Trabajo
Los usuarios pueden crear scripts que automatizan tareas repetitivas:

```javascript
// Script para activar solo defensas robustas
var defenses = Defenses.getAllDefenses();
for (var i = 0; i < defenses.length; i++) {
    if (defenses[i].hardiness >= 2) {  // Robusta o Muy robusta
        Defenses.setDefenseActive(defenses[i].id, true);
    }
}

Marrta.refreshMainScreen();
Marrta.saveModelChanges();
```

### 3. Análisis Iterativo
Los scripts pueden hacer cambios, ver el resultado, y ajustar según sea necesario:

```javascript
// Desactivar una defensa
Defenses.setDefenseActive(defenseId, false);
Marrta.refreshMainScreen();

// Ver el nuevo riesgo calculado
var event = Events.getEventsByStage(stageId)[0];
console.log("Nuevo nivel de riesgo: " + event.riesgoTexto);

// Si el resultado es aceptable, guardar
if (event.riesgo <= 2) {  // Riesgo medio o bajo
    Marrta.saveModelChanges();
    console.log("Cambios guardados");
} else {
    console.log("Riesgo demasiado alto, no se guardan cambios");
}
```

## Integración con APIs Existentes

Los nuevos métodos trabajan perfectamente con todas las APIs existentes:

### API de Eventos (Events)
```javascript
Events.updateEvent(id, {nombre: "Nuevo"});
Marrta.refreshMainScreen();
Marrta.saveModelChanges();
```

### API de Defensas (Defenses)
```javascript
Defenses.setDefenseActive(id, false);
Marrta.refreshMainScreen();
Marrta.saveModelChanges();
```

### API de Etapas (Stages)
```javascript
Stages.updateStage(id, {nombre: "Nueva etapa"});
Marrta.refreshMainScreen();
Marrta.saveModelChanges();
```

## Ventajas de la Implementación

1. **Mínima Invasividad**: Los cambios utilizan métodos existentes (`mostrarEventosSeleccion()`, `mostrarEvento()`, `DbEvent::checkAndUpdate()`)
2. **Consistencia**: Los métodos funcionan exactamente igual que cuando el usuario usa la interfaz gráfica
3. **Seguridad**: `saveModelChanges()` usa el mismo mecanismo de guardado probado que el resto de la aplicación
4. **Flexibilidad**: Los scripts pueden decidir cuándo refrescar y cuándo guardar
5. **Feedback Visual**: `refreshMainScreen()` permite al usuario ver los efectos de sus cambios inmediatamente

## Consideraciones de Diseño

### ¿Por qué no incluir diálogo de confirmación en saveModelChanges()?
Los scripts están diseñados para automatización. Los usuarios que ejecutan scripts manualmente pueden:
1. Revisar los cambios visualmente después de `refreshMainScreen()`
2. Decidir si llamar o no a `saveModelChanges()`
3. Usar el sistema de control de versiones de archivos si necesitan deshacer cambios

### ¿Por qué separar refresh y save?
Separar estas operaciones permite:
1. Ver los efectos de los cambios antes de guardarlos
2. Hacer múltiples cambios y guardar solo una vez
3. Decidir no guardar si el resultado no es el esperado
4. Mayor flexibilidad en la automatización

## Pruebas

El script `test_scripts/test_refresh_save.js` proporciona:
- Verificación de que los métodos están disponibles
- Ejemplos de uso práctico
- Demostración de reversión de cambios
- Documentación de flujos de trabajo completos

Para ejecutar las pruebas:
1. Abrir MARRTA y cargar un modelo (.pdb)
2. Ir a `Herramientas > Editor de Scripts`
3. Cargar `test_scripts/test_refresh_save.js`
4. Hacer clic en "Ejecutar"

## Impacto en el Usuario

### Usuario Final
- Puede automatizar tareas repetitivas de forma más efectiva
- Puede ver los resultados de scripts inmediatamente en la interfaz
- Puede guardar cambios realizados por scripts sin diálogos intermedios

### Desarrollador de Scripts
- Tiene control completo sobre cuándo refrescar la interfaz
- Tiene control completo sobre cuándo guardar cambios
- Puede crear flujos de trabajo más sofisticados

## Compatibilidad

- Los cambios son completamente compatibles hacia atrás
- Scripts existentes siguen funcionando sin modificaciones
- Los nuevos métodos son opcionales (no se requieren para scripts simples)

## Conclusión

Esta implementación cumple completamente con los requisitos del issue:
1. ✅ Permite refrescar la pantalla principal desde scripts
2. ✅ Permite guardar cambios en disco desde scripts

La solución es mínimamente invasiva, utiliza código existente y probado, y proporciona máxima flexibilidad a los usuarios que escriben scripts de automatización.
