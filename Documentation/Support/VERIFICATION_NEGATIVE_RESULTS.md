# Verificación de Resultados Negativos en Comprobación de Actualizaciones

## Descripción del Issue

**Issue:** [Optimización] Información de resultado negativo en el chequeo de nuevas versiones (solo cuando se hace manual)

**Objetivo:** Generar información de resultado negativo en el chequeo de nuevas versiones del software, similar a las del modelo (pero solo cuando se hace manual).

## Implementación

### Estado Previo
El sistema ya tenía implementada la funcionalidad de mostrar resultados negativos para comprobaciones manuales, pero el mensaje de versiones no era consistente con el de modelos.

### Cambio Realizado
Se modificó el mensaje de resultado negativo de la comprobación de versiones para que sea consistente con el de modelos.

**Archivo modificado:** `updatechecker.cpp` (línea 220)

**Antes:**
```cpp
msgBox.setText(tr("Ya tienes la última versión de MARRTA        "));
```

**Después:**
```cpp
msgBox.setText(tr("No hay nuevas versiones disponibles desde la última comprobación."));
```

## Verificación del Comportamiento

### 1. Comprobación Manual de Versiones (sin actualizaciones disponibles)

**Acción del usuario:**
- Menú: Ayuda → Comprobar actualizaciones...

**Comportamiento esperado:**
- Se realiza consulta a GitHub API
- Si NO hay nueva versión disponible:
  - ✅ Muestra diálogo con título "Sin actualizaciones"
  - ✅ Mensaje: "No hay nuevas versiones disponibles desde la última comprobación."
  - ✅ Información adicional: "Versión actual: X.X.X"
  - ✅ Botón "Aceptar"

**Código relevante:** `updatechecker.cpp` líneas 217-226

### 2. Comprobación Manual de Modelos (sin modelos nuevos)

**Acción del usuario:**
- Menú: Ayuda → Comprobar nuevos modelos...

**Comportamiento esperado:**
- Se realiza consulta a GitHub API
- Si NO hay nuevos modelos:
  - ✅ Muestra diálogo con título "Sin nuevos modelos"
  - ✅ Mensaje: "No hay nuevos modelos disponibles desde la última comprobación."
  - ✅ Botón "Aceptar"

**Código relevante:** `updatechecker.cpp` líneas 338-346

### 3. Comprobación Automática (silent mode)

**Cuándo ocurre:**
- Al iniciar la aplicación
- Si han pasado 14 días desde la última comprobación

**Comportamiento esperado:**
- Se realizan comprobaciones con `silent = true`
- Si NO hay actualizaciones/modelos:
  - ✅ NO muestra ningún diálogo al usuario
  - ✅ Solo registra en logs de debug
- Si SÍ hay actualizaciones/modelos:
  - ✅ Muestra diálogo informativo

**Código relevante:** `updatechecker.cpp` líneas 93-112

## Consistencia de Mensajes

Ambos mensajes negativos ahora tienen la misma estructura:

| Comprobación | Mensaje |
|--------------|---------|
| **Versiones** | "No hay nuevas versiones disponibles desde la última comprobación." |
| **Modelos** | "No hay nuevos modelos disponibles desde la última comprobación." |

## Lógica de Control (silent parameter)

```cpp
// Comprobación manual (desde menú)
updateChecker->checkForUpdates(false);      // silent = false
updateChecker->checkForNewModels(false);    // silent = false

// Comprobación automática (al iniciar)
updateChecker->checkForUpdates(true);       // silent = true
updateChecker->checkForNewModels(true);     // silent = true
```

### En el código de UpdateChecker:

```cpp
void UpdateChecker::onVersionCheckFinished(QNetworkReply *reply)
{
    // ...
    if (isNewerVersion(currentVersion, latestVersion)) {
        // Siempre muestra diálogo si hay nueva versión
    } else {
        if (!silentVersionCheck) {  // Solo si NO es silencioso
            // ✅ Muestra diálogo "Sin actualizaciones"
        }
    }
}

void UpdateChecker::onModelsCheckFinished(QNetworkReply *reply)
{
    // ...
    if (newModelsCount > 0) {
        // Siempre muestra diálogo si hay nuevos modelos
    } else {
        if (!silentModelsCheck) {  // Solo si NO es silencioso
            // ✅ Muestra diálogo "Sin nuevos modelos"
        }
    }
}
```

## Casos de Prueba

### Caso 1: Comprobación manual sin actualizaciones
- **Precondición:** La versión actual es la última disponible en GitHub
- **Acción:** Usuario selecciona "Ayuda → Comprobar actualizaciones..."
- **Resultado esperado:** ✅ Muestra diálogo "No hay nuevas versiones disponibles desde la última comprobación."

### Caso 2: Comprobación manual sin modelos nuevos
- **Precondición:** No hay modelos más recientes que la fecha de última comprobación
- **Acción:** Usuario selecciona "Ayuda → Comprobar nuevos modelos..."
- **Resultado esperado:** ✅ Muestra diálogo "No hay nuevos modelos disponibles desde la última comprobación."

### Caso 3: Comprobación automática sin actualizaciones
- **Precondición:** La versión actual es la última disponible y han pasado 14+ días
- **Acción:** Usuario inicia MARRTA
- **Resultado esperado:** ✅ NO muestra ningún diálogo (comprobación silenciosa)

### Caso 4: Comprobación automática sin modelos nuevos
- **Precondición:** No hay modelos nuevos y han pasado 14+ días
- **Acción:** Usuario inicia MARRTA
- **Resultado esperado:** ✅ NO muestra ningún diálogo (comprobación silenciosa)

### Caso 5: Comprobación manual con actualización disponible
- **Precondición:** Hay una nueva versión en GitHub
- **Acción:** Usuario selecciona "Ayuda → Comprobar actualizaciones..."
- **Resultado esperado:** ✅ Muestra diálogo con información de la nueva versión

### Caso 6: Comprobación manual con modelos nuevos
- **Precondición:** Hay modelos más recientes que la última comprobación
- **Acción:** Usuario selecciona "Ayuda → Comprobar nuevos modelos..."
- **Resultado esperado:** ✅ Muestra diálogo con lista de nuevos modelos

## Conclusión

✅ **La funcionalidad está completamente implementada y es consistente:**

1. Las comprobaciones manuales **siempre** muestran resultado (positivo o negativo)
2. Las comprobaciones automáticas **solo** muestran resultado positivo (actualizaciones/modelos disponibles)
3. Los mensajes negativos tienen formato consistente entre versiones y modelos
4. Se cumple el requisito del issue: "solo cuando se hace manual"

## Cambios Mínimos Realizados

**Total de líneas modificadas:** 1 línea en `updatechecker.cpp`

El cambio fue quirúrgico y preciso, mejorando la consistencia del mensaje sin alterar la lógica existente.
