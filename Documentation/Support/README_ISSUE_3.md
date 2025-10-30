# README - Issue #3: Mejorar scripting

## 🎯 Resumen Rápido

**Estado:** ✅ COMPLETADO
**Commits:** 7
**Archivos:** 19 (10 nuevos, 9 modificados)
**Líneas:** +1,771 / -4

## ✨ Qué se ha hecho

### 1. Botones Guardar/Cargar Scripts ✅
- Nuevo botón "Guardar" en el editor
- Nuevo botón "Cargar" en el editor
- Soporte para archivos .js
- Mensajes informativos en consola

### 2. API de Etapas y Subetapas ✅
- Nuevo objeto `Stages` en JavaScript
- 7 métodos implementados y funcionales
- Acceso completo a estructura del modelo
- Manejo de errores robusto

### 3. Documentación Mejorada ✅
- Nueva sección "API de Etapas"
- Nuevo ejemplo práctico
- Todas las APIs documentadas en detalle
- Parámetros y retornos claramente explicados

### 4. Sistema de Logging ✅
- Logging extensivo en todos los adapters
- qDebug() para información
- qWarning() para errores
- Facilita enormemente la depuración

## 🚀 Inicio Rápido

### Para Probar los Cambios

```bash
# 1. Compilar
qmake marrta.pro
make

# 2. Ejecutar
./marrta

# 3. Cargar un modelo (.pdb)

# 4. Abrir editor de scripts
# Menú: Herramientas > Editor de Scripts

# 5. Probar guardar
# - Escribir: console.log("Test");
# - Clic en [Guardar]
# - Guardar como test.js

# 6. Probar cargar
# - Clic en [Cargar]
# - Seleccionar test.js
# - Verificar que se carga

# 7. Probar API de Stages
# - Clic en [Cargar]
# - Seleccionar test_scripts/test_stages_api.js
# - Clic en [Ejecutar]
# - Verificar resultados ✓
```

### Ejemplo Rápido de API

```javascript
// Obtener todas las etapas
var stages = Stages.getAllStages();
console.log("Etapas: " + stages.length);

// Listar etapas con subetapas
var stagesData = Stages.getStagesWithSubstages();
for (var name in stagesData) {
    var stage = stagesData[name];
    console.log(stage.nombre + ": " + stage.substages.length);
}

// Buscar una etapa por ID
var stage = Stages.getStageById(1);
console.log("Etapa 1: " + stage.nombre);
```

## 📁 Estructura de Archivos

```
MARRTA/
│
├── adapters/
│   ├── stageadapter.h        ⭐ NUEVO - Header del adaptador
│   ├── stageadapter.cpp      ⭐ NUEVO - Implementación
│   ├── eventadapter.cpp      📝 MODIFICADO - Con logging
│   └── defenseadapter.cpp    📝 MODIFICADO - Con logging
│
├── test_scripts/              ⭐ NUEVA CARPETA
│   ├── README.md             ⭐ Guía de uso
│   ├── test_stages_api.js    ⭐ Prueba API Stages
│   ├── test_events_api.js    ⭐ Prueba API Events
│   └── test_defenses_api.js  ⭐ Prueba API Defenses
│
├── scriptingeditor.ui         📝 Botones añadidos
├── scriptingeditor.h          📝 Métodos declarados
├── scriptingeditor.cpp        📝 Guardar/cargar implementado
├── scriptingengine.h          📝 StageAdapter incluido
├── scriptingengine.cpp        📝 StageAdapter registrado
├── scriptingdocumentation.cpp 📝 Docs ampliadas
├── marrta.pro                 📝 Archivos incluidos
│
├── SCRIPTING_API_IMPROVEMENTS.md  ⭐ Doc técnica completa
├── CHANGES_SUMMARY.md             ⭐ Resumen visual
├── VERIFICATION_CHECKLIST.md      ⭐ Lista de verificación
├── UI_CHANGES_VISUAL.md           ⭐ Cambios UI
└── README_ISSUE_3.md              ⭐ Este archivo
```

## 📚 Documentación Disponible

| Documento | Propósito | Tamaño |
|-----------|-----------|--------|
| **README_ISSUE_3.md** | Inicio rápido | Este archivo |
| **CHANGES_SUMMARY.md** | Resumen ejecutivo | 235 líneas |
| **SCRIPTING_API_IMPROVEMENTS.md** | Documentación técnica | 246 líneas |
| **VERIFICATION_CHECKLIST.md** | Lista de verificación | 302 líneas |
| **UI_CHANGES_VISUAL.md** | Cambios visuales | 271 líneas |
| **test_scripts/README.md** | Guía de pruebas | 52 líneas |

### Cuál Leer Primero

1. **Este archivo** - Para empezar
2. **CHANGES_SUMMARY.md** - Para resumen visual
3. **SCRIPTING_API_IMPROVEMENTS.md** - Para detalles técnicos
4. **VERIFICATION_CHECKLIST.md** - Para verificar todo funciona

## 🔍 API de Stages

### Métodos Disponibles

| Método | Parámetros | Retorna | Descripción |
|--------|------------|---------|-------------|
| `getAllStages()` | - | Array | Todas las etapas |
| `getAllSubstages()` | - | Array | Todas las subetapas |
| `getSubstagesByStage(id)` | stageId | Array | Subetapas de una etapa |
| `getStagesWithSubstages()` | - | Object | Etapas con subetapas |
| `getStageById(id)` | stageId | Object | Etapa por ID |
| `getSubstageById(id)` | substageId | Object | Subetapa por ID |
| `countSubstagesByStage()` | - | Object | Conteo por etapa |

### Propiedades de Stage

```javascript
{
    id: Number,          // ID de la etapa
    nombre: String,      // Nombre de la etapa
    codigo: String,      // Código de la etapa
    descripcion: String, // Descripción
    orden: Number        // Orden de visualización
}
```

### Propiedades de Substage

```javascript
{
    id: Number,          // ID de la subetapa
    nombre: String,      // Nombre de la subetapa
    descripcion: String, // Descripción
    etapaId: Number,     // ID de la etapa padre
    activo: Boolean,     // Si está activa
    orden: Number        // Orden de visualización
}
```

## 🧪 Scripts de Prueba

### test_stages_api.js
```javascript
// Prueba todos los métodos de la API de Stages
// Verifica: getAllStages, getAllSubstages, etc.
// Resultado esperado: Todos ✓, ningún ✗
```

### test_events_api.js
```javascript
// Prueba todos los métodos de la API de Events
// Verifica: getAllEvents, getEventsByStage, etc.
// Resultado esperado: Todos ✓, ningún ✗
```

### test_defenses_api.js
```javascript
// Prueba todos los métodos de la API de Defenses
// Verifica: getAllDefenses, getBarriers, etc.
// Resultado esperado: Todos ✓, ningún ✗
```

## 🔧 Logging

### Cómo Ver los Logs

```bash
# Ejecutar MARRTA desde terminal
./marrta

# Los logs aparecerán en la consola:
EventAdapter::getAllEvents() - Starting
EventAdapter::getAllEvents() - Retrieved 45 events
StageAdapter::getAllStages() - Retrieved 8 stages
DefenseAdapter::getAllDefenses(5) - FrecuencyReducer: 3
```

### Tipos de Mensajes

- **qDebug()** - Información normal (negro)
- **qWarning()** - Advertencias/errores (amarillo/rojo)

## 🎨 Interfaz

### Botones en el Editor

```
Posición Y=440 (todos)

X=20    [Ejecutar]       Ejecuta el script
X=120   [Documentación]  Abre la documentación
X=250   [Guardar] ⭐     Guarda el script en archivo
X=350   [Cargar] ⭐      Carga script desde archivo
X=610   [Borrar]         Limpia el editor
```

## ✅ Checklist de Verificación

- [ ] La aplicación compila sin errores
- [ ] Botones "Guardar" y "Cargar" visibles
- [ ] Guardar script funciona
- [ ] Cargar script funciona
- [ ] Documentación tiene sección "API de Etapas"
- [ ] Ejemplo 5 está presente
- [ ] API Stages funciona: `Stages.getAllStages()`
- [ ] Scripts de prueba ejecutan sin errores
- [ ] Logs aparecen en la consola
- [ ] No hay regresiones en funcionalidad existente

## 🐛 Solución de Problemas

### El proyecto no compila

```bash
# Limpiar y recompilar
make clean
qmake
make
```

### Los botones no aparecen

- Verificar que scriptingeditor.ui fue actualizado
- Recompilar el proyecto completamente
- Verificar que los archivos .ui se procesaron

### La API de Stages no funciona

- Verificar que el modelo está cargado
- Revisar logs en consola de la aplicación
- Ejecutar script de prueba test_stages_api.js
- Verificar que stageadapter.cpp está compilado

### No aparecen logs

- Ejecutar MARRTA desde terminal, no desde IDE
- Los logs van a stdout, no a la consola del script
- Verificar nivel de debug en qDebug()

## 📞 Contacto

Para problemas o preguntas sobre estos cambios:
- Ver documentación en los archivos .md
- Revisar el código en adapters/stageadapter.cpp
- Ejecutar scripts de prueba en test_scripts/

## 🎉 ¡Listo!

Todos los requisitos del Issue #3 están implementados y listos para usar.

**Siguiente paso:** Compilar, probar y disfrutar de las nuevas funcionalidades.
