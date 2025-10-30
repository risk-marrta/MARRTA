# Mejoras del Sistema de Scripting MARRTA

Este documento describe las mejoras implementadas en el sistema de scripting de MARRTA (Issue #3).

## Cambios Implementados

### 1. Botones de Guardar y Cargar Scripts

Se han añadido dos nuevos botones en el editor de scripts:

- **Botón "Guardar"**: Permite guardar el script actual en un archivo `.js`
  - Ubicación: Entre el botón "Documentación" y "Borrar" en la ventana del editor
  - Funcionalidad: Abre un diálogo para seleccionar la ubicación y nombre del archivo
  - Formato: Archivos JavaScript (*.js)

- **Botón "Cargar"**: Permite cargar un script desde un archivo
  - Ubicación: Junto al botón "Guardar"
  - Funcionalidad: Abre un diálogo para seleccionar un archivo existente
  - El contenido del archivo reemplaza el texto actual en el editor

**Archivos modificados:**
- `scriptingeditor.ui` - Interfaz de usuario
- `scriptingeditor.h` - Declaraciones de métodos
- `scriptingeditor.cpp` - Implementación de funcionalidad de guardar/cargar

### 2. Nueva API de Etapas y Subetapas

Se ha creado un nuevo adaptador `StageAdapter` que expone información sobre etapas y subetapas al motor de scripting.

**Métodos disponibles en el objeto `Stages`:**

- `getAllStages()` - Obtiene todas las etapas del modelo
  - Retorna: Array de objetos con propiedades: id, nombre, codigo, descripcion, orden

- `getAllSubstages()` - Obtiene todas las subetapas
  - Retorna: Array de objetos con propiedades: id, nombre, descripcion, etapaId, activo, orden

- `getSubstagesByStage(stageId)` - Obtiene subetapas de una etapa específica
  - Parámetro: stageId (entero)
  - Retorna: Array de subetapas

- `getStagesWithSubstages()` - Obtiene etapas agrupadas con sus subetapas
  - Retorna: Objeto con nombres de etapas como claves y objetos con info de etapa + array de subetapas

- `getStageById(stageId)` - Obtiene una etapa por su ID
  - Parámetro: stageId (entero)
  - Retorna: Objeto de etapa o vacío si no se encuentra

- `getSubstageById(substageId)` - Obtiene una subetapa por su ID
  - Parámetro: substageId (entero)
  - Retorna: Objeto de subetapa o vacío si no se encuentra

- `countSubstagesByStage()` - Cuenta subetapas por etapa
  - Retorna: Objeto con nombres de etapas como claves y conteos como valores

**Archivos nuevos:**
- `adapters/stageadapter.h` - Definición de la clase StageAdapter
- `adapters/stageadapter.cpp` - Implementación del adaptador

**Archivos modificados:**
- `scriptingengine.h` - Inclusión del nuevo adaptador
- `scriptingengine.cpp` - Inicialización y registro del adaptador
- `marrta.pro` - Adición de archivos al proyecto

### 3. Documentación Mejorada de la API

La documentación del sistema de scripting ha sido ampliada con:

**Nueva sección: API de Etapas**
- Descripción detallada de todos los métodos del objeto `Stages`
- Explicación de las propiedades de etapas y subetapas
- Información sobre los valores retornados

**Nuevo ejemplo: Explorar Estructura de Etapas y Subetapas**
- Ejemplo 5: Muestra cómo listar todas las etapas con sus subetapas
- Demuestra el uso de los métodos de la API de Stages
- Incluye visualización de jerarquía y estados

**Mejoras en documentación existente:**
- Actualización de la sección de introducción para incluir el objeto `Stages`
- Clarificación de parámetros y valores de retorno en todas las APIs
- Ejemplos más detallados con mejor formato

**Archivos modificados:**
- `scriptingdocumentation.cpp` - Adición de nueva sección y ejemplo

### 4. Logging y Depuración

Se ha añadido logging extensivo usando `qDebug()` para facilitar la identificación de problemas:

**EventAdapter:**
- Logs al inicio de cada método con parámetros
- Conteo de resultados obtenidos
- Información sobre operaciones realizadas

**DefenseAdapter:**
- Logs detallados por tipo de defensa
- Conteo de defensas procesadas
- Información sobre cambios de estado

**StageAdapter:**
- Logs en todas las operaciones de consulta
- Mensajes de advertencia con `qWarning()` para errores
- Información sobre etapas/subetapas encontradas o no encontradas

**Ejemplo de logs generados:**
```
EventAdapter::getAllEvents() - Starting
EventAdapter::getAllEvents() - Retrieved 45 events
StageAdapter::getAllStages() - Retrieved 8 stages
DefenseAdapter::getAllDefenses(12) - Starting
DefenseAdapter::getAllDefenses() - FrecuencyReducer: 3
DefenseAdapter::getAllDefenses() - Barrier: 5
DefenseAdapter::getAllDefenses() - ConsequenceReducer: 2
```

**Archivos modificados:**
- `adapters/eventadapter.cpp` - Adición de logs
- `adapters/defenseadapter.cpp` - Adición de logs
- `adapters/stageadapter.cpp` - Implementación con logging completo

## Scripts de Prueba

Se han creado scripts de prueba para verificar el funcionamiento de todas las APIs:

**test_scripts/test_stages_api.js**
- Verifica todos los métodos de la API de Stages
- Imprime resultados y conteos

**test_scripts/test_events_api.js**
- Verifica todos los métodos de la API de Events
- Valida agrupaciones y estadísticas

**test_scripts/test_defenses_api.js**
- Verifica todos los métodos de la API de Defenses
- Comprueba filtros y clasificaciones

**test_scripts/README.md**
- Documentación sobre cómo usar los scripts de prueba
- Guía de interpretación de resultados

## Cómo Usar las Nuevas Funcionalidades

### Guardar un Script

1. Escribir o cargar un script en el editor
2. Hacer clic en el botón "Guardar"
3. Elegir ubicación y nombre del archivo en el diálogo
4. El script se guarda en formato JavaScript (.js)

### Cargar un Script

1. Hacer clic en el botón "Cargar"
2. Seleccionar un archivo .js en el diálogo
3. El contenido se carga en el editor
4. Hacer clic en "Ejecutar" para ejecutar el script

### Usar la API de Etapas

```javascript
// Obtener todas las etapas
var stages = Stages.getAllStages();
console.log("Total de etapas: " + stages.length);

// Listar etapas con subetapas
var stagesWithSubs = Stages.getStagesWithSubstages();
for (var stageName in stagesWithSubs) {
    var stage = stagesWithSubs[stageName];
    console.log(stage.nombre + " tiene " + stage.substages.length + " subetapas");
}

// Obtener una etapa específica
var stage = Stages.getStageById(1);
console.log("Etapa: " + stage.nombre);
```

## Depuración

Para depurar problemas con la API:

1. **Ejecutar la aplicación desde la consola** para ver los mensajes de qDebug()
2. **Revisar los logs** en la salida estándar
3. **Usar los scripts de prueba** en la carpeta `test_scripts/`
4. **Verificar que el modelo esté cargado** antes de ejecutar scripts
5. **Comprobar la consola del editor** para errores de JavaScript

### Ejemplo de ejecución con logs:

```bash
./marrta
# Logs aparecerán en la consola cuando se ejecuten scripts:
# EventAdapter::getAllEvents() - Starting
# EventAdapter::getAllEvents() - Retrieved 45 events
```

## Cambios en Archivos

### Archivos Nuevos
- `adapters/stageadapter.h`
- `adapters/stageadapter.cpp`
- `test_scripts/test_stages_api.js`
- `test_scripts/test_events_api.js`
- `test_scripts/test_defenses_api.js`
- `test_scripts/README.md`

### Archivos Modificados
- `scriptingeditor.ui` - UI con botones nuevos
- `scriptingeditor.h` - Declaraciones de métodos
- `scriptingeditor.cpp` - Implementación guardar/cargar
- `scriptingengine.h` - Inclusión StageAdapter
- `scriptingengine.cpp` - Registro StageAdapter
- `scriptingdocumentation.cpp` - Documentación ampliada
- `adapters/eventadapter.cpp` - Logging añadido
- `adapters/defenseadapter.cpp` - Logging añadido
- `marrta.pro` - Inclusión de nuevos archivos

## Verificación de Cambios

Para verificar que todas las mejoras funcionan:

1. Compilar el proyecto con los cambios
2. Ejecutar la aplicación
3. Cargar un modelo de análisis de riesgos
4. Abrir el editor de scripts (Herramientas > Editor de Scripts)
5. Verificar que los botones "Guardar" y "Cargar" están presentes
6. Probar guardar y cargar un script
7. Ejecutar los scripts de prueba en `test_scripts/`
8. Verificar la documentación (botón "Documentación")
9. Comprobar que la sección "API de Etapas" existe
10. Revisar los logs en la consola de la aplicación

## Notas Técnicas

- El adaptador `StageAdapter` utiliza manejo de excepciones para capturar errores de base de datos
- Todos los métodos que consultan la base de datos incluyen logging para facilitar depuración
- Los logs usan `qDebug()` para información y `qWarning()` para errores
- La documentación está en español, consistente con el resto de la aplicación
- Los archivos de script de prueba usan JavaScript estándar sin dependencias externas

## Próximos Pasos Sugeridos

1. Probar exhaustivamente las nuevas funcionalidades con diferentes modelos
2. Verificar el rendimiento con modelos grandes
3. Considerar añadir persistencia de la ruta del último archivo guardado/cargado
4. Evaluar si se necesitan más métodos en la API de Stages
5. Considerar añadir una API similar para otras entidades del modelo
