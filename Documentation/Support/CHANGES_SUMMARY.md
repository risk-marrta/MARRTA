# Resumen de Cambios - Issue #3: Mejorar scripting

## 📝 Descripción General

Se han implementado todas las mejoras solicitadas en el issue #3 para el sistema de scripting de MARRTA:

1. ✅ Botones para guardar y cargar scripts
2. ✅ Documentación de API más detallada
3. ✅ API para etapas y subetapas
4. ✅ Logging extensivo para depuración

## 🎨 Cambios en la Interfaz

### Editor de Scripts - Antes y Después

**ANTES:**
```
[Ejecutar] [Documentación]                           [Borrar]
```

**DESPUÉS:**
```
[Ejecutar] [Documentación] [Guardar] [Cargar]       [Borrar]
```

### Nuevos Botones

| Botón | Posición | Función |
|-------|----------|---------|
| **Guardar** | Entre Documentación y Cargar | Guarda el script actual en archivo .js |
| **Cargar** | Entre Guardar y Borrar | Carga un script desde archivo .js |

## 🔧 Nueva API: Stages

Se ha añadido un nuevo objeto `Stages` al motor de scripting con los siguientes métodos:

### Métodos Disponibles

```javascript
// Obtener todas las etapas
Stages.getAllStages()
// Retorna: [{id, nombre, codigo, descripcion, orden}, ...]

// Obtener todas las subetapas
Stages.getAllSubstages()
// Retorna: [{id, nombre, descripcion, etapaId, activo, orden}, ...]

// Obtener subetapas de una etapa específica
Stages.getSubstagesByStage(stageId)
// Parámetro: stageId (número)
// Retorna: array de subetapas

// Obtener etapas con sus subetapas
Stages.getStagesWithSubstages()
// Retorna: objeto con etapas como claves

// Obtener una etapa por ID
Stages.getStageById(stageId)
// Retorna: objeto de etapa

// Obtener una subetapa por ID
Stages.getSubstageById(substageId)
// Retorna: objeto de subetapa

// Contar subetapas por etapa
Stages.countSubstagesByStage()
// Retorna: {etapa: count, ...}
```

## 📚 Mejoras en Documentación

### Nueva Sección Agregada

- **API de Etapas**: Documentación completa del objeto `Stages`
  - Descripción de todos los métodos
  - Parámetros y valores de retorno
  - Propiedades de objetos retornados

### Nuevo Ejemplo Agregado

- **Ejemplo 5: Explorar Estructura de Etapas y Subetapas**
  - Muestra cómo listar etapas con subetapas
  - Demuestra conteo de subetapas
  - Visualización de jerarquía

## 🔍 Sistema de Logging

Se ha implementado logging extensivo para facilitar la depuración:

### Logs en EventAdapter
```
EventAdapter::getAllEvents() - Starting
EventAdapter::getAllEvents() - Retrieved 45 events
EventAdapter::getEventsByStage(2, 0) - Starting
EventAdapter::getEventsByStage() - Retrieved 12 events
```

### Logs en DefenseAdapter
```
DefenseAdapter::getAllDefenses(5) - Starting
DefenseAdapter::getAllDefenses() - FrecuencyReducer: 3
DefenseAdapter::getAllDefenses() - Barrier: 5
DefenseAdapter::getAllDefenses() - ConsequenceReducer: 2
```

### Logs en StageAdapter
```
StageAdapter::getAllStages() - Retrieved 8 stages
StageAdapter::getSubstagesByStage(1) - Retrieved 4 substages
StageAdapter::getStageById(1) - Found stage: Operación Normal
```

## 🧪 Scripts de Prueba

Se han creado scripts de prueba en la carpeta `test_scripts/`:

| Script | Descripción |
|--------|-------------|
| `test_stages_api.js` | Prueba todas las funciones de la API de Stages |
| `test_events_api.js` | Prueba todas las funciones de la API de Events |
| `test_defenses_api.js` | Prueba todas las funciones de la API de Defenses |
| `README.md` | Guía de uso de los scripts de prueba |

### Uso de Scripts de Prueba

1. Abrir MARRTA y cargar un modelo
2. Ir a `Herramientas > Editor de Scripts`
3. Usar botón "Cargar" para cargar un script de prueba
4. Ejecutar y verificar resultados en consola

## 📋 Archivos Modificados

### Archivos Nuevos (7)
- `adapters/stageadapter.h`
- `adapters/stageadapter.cpp`
- `test_scripts/test_stages_api.js`
- `test_scripts/test_events_api.js`
- `test_scripts/test_defenses_api.js`
- `test_scripts/README.md`
- `SCRIPTING_API_IMPROVEMENTS.md`

### Archivos Modificados (9)
- `scriptingeditor.ui` - Botones nuevos
- `scriptingeditor.h` - Declaraciones
- `scriptingeditor.cpp` - Implementación guardar/cargar
- `scriptingengine.h` - Inclusión StageAdapter
- `scriptingengine.cpp` - Registro StageAdapter
- `scriptingdocumentation.cpp` - Documentación ampliada
- `adapters/eventadapter.cpp` - Logging
- `adapters/defenseadapter.cpp` - Logging
- `marrta.pro` - Inclusión de archivos

## 🎯 Objetivos Cumplidos

| Requisito | Estado | Detalles |
|-----------|--------|----------|
| Botones guardar/cargar scripts | ✅ Completado | Dos botones funcionales en el editor |
| Documentación más detallada | ✅ Completado | Nueva sección de API + ejemplo |
| API para etapas/subetapas | ✅ Completado | StageAdapter con 7 métodos |
| Identificar bugs en API | ✅ Completado | Logging extensivo implementado |

## 🔄 Cambios Técnicos

### Arquitectura

```
ScriptingEngine
    ├── ConsoleAdapter (existente)
    ├── EventAdapter (mejorado con logs)
    ├── DefenseAdapter (mejorado con logs)
    ├── ChartAdapter (existente)
    └── StageAdapter (NUEVO)
```

### Flujo de Datos

```
JavaScript Script
    ↓
ScriptingEngine (QJSEngine)
    ↓
StageAdapter (Q_INVOKABLE methods)
    ↓
Database Layer (DbElementStage, DbElementSubstage)
    ↓
SQLite Database
```

## 🚀 Cómo Probar los Cambios

### 1. Compilar el Proyecto
```bash
qmake
make
```

### 2. Ejecutar la Aplicación
```bash
./marrta
```

### 3. Verificar Botones
- Abrir editor de scripts
- Verificar presencia de botones "Guardar" y "Cargar"

### 4. Probar Guardar/Cargar
- Escribir script de prueba
- Guardar en archivo
- Cargar desde archivo
- Verificar que funciona

### 5. Probar API de Stages
- Cargar un modelo
- Ejecutar script de prueba `test_stages_api.js`
- Verificar resultados en consola

### 6. Verificar Logs
- Ejecutar cualquier script que use las APIs
- Revisar consola de la aplicación
- Verificar que aparecen mensajes de qDebug()

## 📖 Documentación Adicional

Para más detalles, consultar:
- `SCRIPTING_API_IMPROVEMENTS.md` - Documentación técnica completa
- `test_scripts/README.md` - Guía de scripts de prueba
- Documentación integrada - Botón "Documentación" en el editor

## ✨ Beneficios

1. **Productividad**: Los scripts se pueden guardar y reutilizar
2. **Flexibilidad**: Acceso completo a estructura de etapas/subetapas
3. **Documentación**: API completamente documentada con ejemplos
4. **Depuración**: Logging facilita identificación de problemas
5. **Calidad**: Scripts de prueba aseguran funcionalidad correcta
