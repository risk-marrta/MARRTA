# Scripts de Prueba para el Sistema de Scripting MARRTA

Esta carpeta contiene scripts de prueba para verificar el funcionamiento de la API de scripting de MARRTA (JavaScript y Python).

## Scripts de JavaScript

### test_stages_api.js
Prueba las funciones de la API de Etapas (Stages):

**Métodos de Lectura:**
- `getAllStages()` - Obtiene todas las etapas (incluye etapas instrumentales con ID < 0)
- `getAllSubstages()` - Obtiene todas las subetapas
- `getStagesWithSubstages()` - Obtiene etapas con sus subetapas
- `countSubstagesByStage()` - Cuenta subetapas por etapa
- `getStageById(id)` - Obtiene una etapa por su ID
- `getSubstageById(id)` - Obtiene una subetapa por su ID
- `getSubstagesByStage(stageId)` - Obtiene subetapas de una etapa específica

**Métodos de Escritura (Nuevos):**
- `updateStage(stageId, properties)` - Actualiza propiedades de una etapa (nombre, descripcion, codigo, orden)

**Propiedades de Stage:** id, nombre, codigo, descripcion, orden, activo

**Nota:** Las etapas pueden tener ID positivo (etapas del proceso) o negativo (etapas instrumentales para barreras)

### test_events_api.js
Prueba las funciones de la API de Eventos (Events):

**Métodos de Lectura:**
- `getAllEvents()` - Obtiene todos los eventos
- `getEventsByStageGrouped()` - Agrupa eventos por etapa
- `countEventsByStage()` - Cuenta eventos por etapa
- `getEventsByConsequence()` - Agrupa eventos por consecuencia
- `getEventStatistics()` - Obtiene estadísticas de eventos

**Métodos de Escritura (Nuevos):**
- `updateEvent(eventId, properties)` - Actualiza propiedades de un evento
- `setAllEventsApplicable()` - Marca todos los eventos como aplicables
- `setAllEventsNotApplicable()` - Marca todos los eventos como no aplicables
- `searchReplaceInDescriptions(searchTerm, replaceTerm, caseSensitive)` - Busca y reemplaza texto en descripciones

**Propiedades de Event:** id, codigo, nombre, descripcion, etapa, etapaId, subetapa, subetapaId, frecuenciaBase, consecuenciaBase, riesgoBase, riesgoBaseTexto, frecuencia, frecuenciaTexto, probabilidad, probabilidadTexto, consecuencia, consecuenciaTexto, errorHumano, base, riesgo, riesgoTexto

### test_defenses_api.js
Prueba las funciones de la API de Defensas (Defenses):

**Métodos Disponibles:**
- `getAllDefenses(eventId)` - Obtiene todas las defensas (opcional: filtrar por evento)
- `getBarriers(eventId)` - Obtiene solo barreras
- `getFrequencyReducers(eventId)` - Obtiene reductores de frecuencia
- `getConsequenceReducers(eventId)` - Obtiene reductores de consecuencia
- `getDefensesByTypeGrouped()` - Agrupa defensas por tipo
- `getDefenseStatistics()` - Obtiene estadísticas de defensas
- `getDefensesByEvent(eventId)` - Obtiene defensas de un evento específico
- `getDefensesByStage(stageName)` - Obtiene defensas de una etapa
- `getDefensesByHardiness(hardiness)` - Filtra por nivel de robustez
- `countDefensesByType()` - Cuenta defensas por tipo
- `getActiveDefenses()` - Obtiene solo defensas activas
- `getInactiveDefenses()` - Obtiene solo defensas inactivas

**Propiedades de Defense:** id, eventId, codigo, nombre, descripcion, type (0=Barrera, 1=Red.Frec, 2=Red.Cons), hardiness (0-3), hardinessString (Blanda/Normal/Robusta/Muy robusta), active, orderStage, humanBased

### test_charts_api.js
Prueba las funciones de la API de Gráficas (Charts):
- `generateTextBarChart()` - Genera gráficas de barras en texto
- `generateTable()` - Genera tablas formateadas
- `sortByProperty()` - Ordena arrays por propiedad
- `groupByProperty()` - Agrupa arrays por propiedad
- `countByProperty()` - Cuenta ocurrencias por propiedad
- `filterByProperty()` - Filtra arrays por propiedad
- `getNumericStats()` - Obtiene estadísticas numéricas
- `createBarChart()` - Crea estructura de gráfica de barras
- `createPieChart()` - Crea estructura de gráfica circular

### test_marrta_api.js
Prueba el objeto principal Marrta y verifica:
- Disponibilidad de objetos globales
- Métodos invocables desde JavaScript
- Resumen completo del modelo cargado
- Documentación de uso del objeto Marrta

### defense_list_grouped.js
Lista todas las defensas únicas agrupadas por tipo:
- Agrupa defensas por tipo (Reductores de Frecuencia, Barreras, Reductores de Consecuencia)
- Identifica defensas únicas (mismas características = misma defensa)
- Para cada defensa muestra:
  - Código y nombre
  - Nivel de robustez
  - Sucesos iniciadores donde aparece (códigos)
  - Número total de sucesos en que aparece
- Proporciona un resumen de defensas únicas por tipo

### defense_importance_analysis.js
Análisis de importancia de las defensas basado en métricas de sensibilidad de riesgo:
- Calcula el riesgo medio del modelo (R)
- Para cada defensa única calcula:
  - Ri: Riesgo medio en sucesos donde aparece
  - R1: Riesgo estimado si siempre aplicable
  - R0: Riesgo estimado si nunca aplicable
  - R-: Riesgo estimado con robustez reducida
  - R+: Riesgo estimado con robustez incrementada
- Métricas de importancia calculadas:
  - F-V (Fussell-Vesely): Contribución relativa al riesgo
  - RRW (Risk Reduction Worth): Beneficio de la defensa
  - RRI (Risk Reduction Interval): Reducción absoluta de riesgo
  - RAW (Risk Achievement Worth): Impacto si falla
  - RII (Risk Increase Interval): Incremento absoluto si falla
  - RRI+/RRI-: Intervalos para cambios en robustez
  - B (Birnbaum Importance): Sensibilidad del riesgo
  - B+: Birnbaum para cambios en robustez
- Ordena defensas por importancia (RAW)
- Proporciona resumen por tipo de defensa
- Incluye guía de interpretación de métricas

**Referencia:** https://www.nrc.gov/docs/ML1216/ML12160A479.pdf

### event_details_by_code.js
Muestra información detallada de un suceso iniciador buscado por código:
- Busca un suceso por su código (ej: "TTD-04")
- Muestra tabla con características básicas:
  - Código, nombre, frecuencia base, consecuencia base, riesgo base (calculado desde API)
- Muestra descripción del suceso
- Lista todas las defensas del suceso organizadas por tipo:
  - Reductores de Frecuencia (código, nombre, robustez)
  - Barreras (código, nombre, robustez)
  - Reductores de Consecuencia (código, nombre, robustez)
- Muestra valores calculados con las defensas activas:
  - Frecuencia calculada, probabilidad, consecuencia calculada, riesgo calculado
- Incluye resumen del número de defensas por tipo
- Formato de tabla fácil de leer con bordes y alineación
**Métodos Nuevos:**
- `refreshMainScreen()` - Refresca la pantalla principal para reflejar cambios
- `saveModelChanges()` - Guarda los cambios en disco sin diálogos de confirmación

### test_refresh_save.js
Prueba las nuevas funciones de refresco y guardado:
- Verificación de disponibilidad de los nuevos métodos
- Ejemplo de modificación de defensas y refresco de pantalla
- Flujo completo de trabajo con scripts
- Integración con otras APIs (Events, Defenses, Stages)

## Cómo Usar los Scripts de Prueba

1. Abrir MARRTA y cargar un modelo de análisis de riesgos (archivo .pdb)
2. Ir a `Herramientas > Editor de Scripts`
3. Usar el botón "Cargar" para cargar uno de los scripts de prueba
4. Hacer clic en "Ejecutar" para ejecutar el script
5. Verificar los resultados en la consola

## Verificación de Funcionalidad

Cada script imprime:
- ✓ para funciones que funcionan correctamente
- ✗ para funciones que generan errores

Los scripts también muestran información adicional para verificar que los datos se están recuperando correctamente.

## Orden Recomendado de Ejecución

Para verificar completamente el sistema de scripting, ejecuta los scripts en este orden:

1. **test_marrta_api.js** - Verifica el entorno global y obtiene resumen del modelo
2. **test_stages_api.js** - Verifica la estructura de etapas y subetapas
3. **test_events_api.js** - Verifica eventos iniciadores
4. **test_defenses_api.js** - Verifica defensas y barreras
5. **test_charts_api.js** - Verifica utilidades de gráficas y análisis
6. **test_refresh_save.js** - Verifica refresco de pantalla y guardado de cambios

## Scripts de Análisis Avanzado

Estos scripts proporcionan análisis más detallados del modelo:

6. **defense_list_grouped.js** - Lista completa de defensas únicas agrupadas por tipo
7. **defense_importance_analysis.js** - Análisis de importancia de defensas con métricas de sensibilidad

## Depuración

Si encuentras errores:
1. Revisa los mensajes de error en la consola del script
2. Busca mensajes de depuración (qDebug) en la salida de la aplicación
3. Verifica que el modelo esté correctamente cargado antes de ejecutar los scripts
4. Asegúrate de que el archivo .pdb contiene datos (eventos, defensas, etapas)

## Ejemplos de Uso

### Ejemplo 1: Verificación Rápida
```javascript
// Cargar y ejecutar test_marrta_api.js
// Debe mostrar todos los objetos disponibles y un resumen del modelo
```

### Ejemplo 2: Análisis de Defensas
```javascript
// Cargar y ejecutar test_defenses_api.js
// Verifica que todas las defensas se recuperen correctamente con sus estados activo/inactivo
```

### Ejemplo 3: Crear Gráficas Personalizadas
```javascript
// Cargar test_charts_api.js como base
// Modificar los ejemplos para crear tus propias visualizaciones
```

## Valores Calculados vs Base

Los eventos incluyen tanto valores base (de la base de datos) como valores calculados (considerando defensas activas):

### Valores Base
- `frecuenciaBase` - Frecuencia asignada originalmente al evento (0-3)
- `consecuenciaBase` - Consecuencia asignada originalmente al evento (0-3)
- `riesgoBase` - Riesgo base calculado asumiendo probabilidad máxima (PA=3) (0-3)
- `riesgoBaseTexto` - Texto del riesgo base (RB, RM, RA, RMA)

### Valores Calculados (Con Defensas)
- `frecuencia` - Frecuencia ajustada por reductores de frecuencia activos
- `probabilidad` - Calculada desde barreras activas y su robustez
- `consecuencia` - Consecuencia ajustada por reductores de consecuencia activos
- `riesgo` - Calculado desde la matriz de riesgo usando P, F, C calculados

### Literales de Texto

Todos los niveles numéricos (0-3) tienen representaciones textuales:

**Frecuencia:** FMB (Muy baja), FB (Baja), FM (Media), FA (Alta)

**Probabilidad:** PMB (Muy baja), PB (Baja), PM (Media), PA (Alta)

**Consecuencia:** CB (Bajas), CM (Medias), CA (Altas), CMA (Muy altas)

**Riesgo:** RB (Bajo), RM (Medio), RA (Alto), RMA (Muy Alto)

**Robustez:** Blanda, Normal, Robusta, Muy robusta

## Métodos de Escritura

A partir de la versión actual, la API permite modificar datos del modelo:

### Actualizar Eventos
```javascript
// Cambiar propiedades individuales de un evento
Events.updateEvent(eventId, {
    nombre: "Nuevo nombre",
    descripcion: "Nueva descripción",
    frecuenciaBase: 2,  // Nota: usar frecuenciaBase, no frecuencia
    consecuenciaBase: 1,
    errorHumano: true,
    completado: true  // Marca como aplicable
});

// Marcar todos los eventos como aplicables
Events.setAllEventsApplicable();

// Marcar todos los eventos como no aplicables
Events.setAllEventsNotApplicable();

// Buscar y reemplazar en descripciones
Events.searchReplaceInDescriptions("palabra_vieja", "palabra_nueva", false);
```

### Actualizar Etapas
```javascript
// Cambiar propiedades de una etapa
Stages.updateStage(stageId, {
    nombre: "Nuevo nombre de etapa",
    descripcion: "Nueva descripción",
    codigo: "NVO",
    orden: 5
});
```

**Importante:** Los cambios se guardan en la base de datos inmediatamente. Usa con precaución.

## Notas Importantes

- Todos los scripts requieren que haya un modelo cargado (.pdb)
- Los resultados varían según el contenido del modelo
- Si un test falla, verifica primero que el modelo tiene datos
- Los logs de depuración aparecen en la consola de la aplicación (no en el editor de scripts)
- Los métodos de escritura modifican permanentemente la base de datos
- Los valores calculados (frecuencia, probabilidad, consecuencia, riesgo) reflejan el estado actual con defensas activas

---

## Scripts de Prueba Python

Los scripts de prueba Python están numerados en orden recomendado de ejecución:

### 01_test_python_marrta_api.py
Prueba el objeto base Marrta en Python:

**Métodos disponibles:**
- `Marrta.get_version()` - Obtiene la versión de MARRTA

**Ejemplo:**
```python
version = Marrta.get_version()
print(f"Versión: {version}")
```

### 02_test_python_database_api.py
Prueba el acceso directo a la base de datos SQLite mediante SQL:

**Método disponible:**
- `Database.query(sql)` - Ejecuta consulta SQL y retorna lista de diccionarios

**Ejemplos:**
```python
# Obtener todas las etapas
results = Database.query("SELECT * FROM etapas ORDER BY orden")

# Filtrar eventos de alta frecuencia
sql = "SELECT nombre, frecuencia FROM suceso_iniciador WHERE frecuencia >= 4"
events = Database.query(sql)

# Contar registros
result = Database.query("SELECT COUNT(*) as total FROM suceso_iniciador")
total = result[0]['total']
```

**Retorno:** Lista de diccionarios donde cada diccionario representa una fila con nombres de columna como claves.

### 03_test_python_etapas_api.py
Prueba el objeto Etapas y el método Etapa.imprimir():

**Métodos disponibles:**
- `Etapas.get_all()` - Obtiene todas las etapas como objetos Etapa

**Propiedades de Etapa:**
- `id` (int) - ID de la etapa
- `codigo` (string) - Código de la etapa
- `nombre` (string) - Nombre de la etapa
- `descripcion` (string) - Descripción de la etapa
- `orden` (int) - Orden de visualización

**Método Etapa.imprimir():**
Retorna tupla de 5 elementos: `(ID, código, nombre, descripción, (IDs_subetapas))`

**Ejemplo:**
```python
etapas = Etapas.get_all()

for etapa in etapas:
    # Acceder a propiedades
    print(f"{etapa.codigo}: {etapa.nombre}")
    
    # Usar método imprimir()
    datos = etapa.imprimir()
    id_etapa, codigo, nombre, desc, subetapas_ids = datos
    
    print(f"  Etapa {id_etapa} tiene {len(subetapas_ids)} subetapas")
    if len(subetapas_ids) > 0:
        print(f"  IDs de subetapas: {subetapas_ids}")
```

### 06_test_python_complete.py
Test de integración completa que demuestra el uso combinado de todos los objetos:

**Incluye:**
- Obtención de versión MARRTA
- Estadísticas de base de datos (conteo de tablas)
- Consultas SQL personalizadas (eventos de alto riesgo)
- Uso de Etapas.get_all()
- Aplicación de Etapa.imprimir() con todas las etapas
- Análisis cruzado combinando Database y Etapas
- Conteo de eventos por etapa
- Estadísticas de modelo completo

**Ejemplo de salida:**
```
=== MARRTA Python API - Complete Integration Test ===

SECTION 1: Marrta Object
✓ MARRTA Version: MARRTA 1.5

SECTION 2: Database Object
  etapas: 5 records
  subetapas: 12 records
  suceso_iniciador: 25 records
  defensas: 18 records

SECTION 3: Etapas Object
  [E01] Etapa 1 (ID: 1, Orden: 1)
  ...
```

## Cómo usar los scripts Python

**Orden recomendado de ejecución de tests Python:**
1. **01_test_python_marrta_api.py** - Verifica objeto Marrta base
2. **02_test_python_database_api.py** - Verifica acceso SQL directo
3. **03_test_python_etapas_api.py** - Verifica API de Etapas
4. **04_test_python_sucesos_api.py** - Verifica API de Sucesos
5. **05_test_python_defensas_api.py** - Verifica API de Defensas
6. **06_test_python_complete.py** - Test de integración completa
7. **07_test_python_integration_complete.py** - Test de integración avanzada

1. **Abrir el Editor Python:**
   - Menú: `Modelo → Scripting → Python`

2. **Cargar un script de prueba:**
   - Clic en botón `[Cargar]`
   - Navegar a carpeta `test_scripts/`
   - Seleccionar archivo `.py`
   - El contenido se carga en el editor

3. **Ejecutar el script:**
   - Clic en botón `[Ejecutar]`
   - Los resultados aparecen en la consola inferior

4. **Modificar y re-ejecutar:**
   - Editar el código en el editor
   - Usar `[Ejecutar]` para probar cambios
   - Usar `[Guardar]` para guardar versiones modificadas

## Comparación JavaScript vs Python

| Característica | JavaScript | Python |
|----------------|------------|--------|
| Motor | QJSEngine | Python 3.12 C API |
| Resaltado sintaxis | ✓ | ✓ |
| Buscar/Reemplazar | ✗ | ✓ |
| Ventana documentación | ✓ | ✓ |
| Objeto Marrta | ✓ | ✓ (básico) |
| Acceso DB directo | ✗ | ✓ (Database.query) |
| API Events | ✓ | ✓ (Sucesos) |
| API Defenses | ✓ | ✓ (Defensas) |
| API Charts | ✓ | ✗ (futuro) |
| Etapa.imprimir() | ✗ | ✓ |
| API Stages completa | ✓ | ✓ (Etapas) |

---

## Nuevas APIs Python (Enhanced)

### 04_test_python_sucesos_api.py
Prueba el módulo Sucesos para acceso a eventos iniciadores:

**Métodos disponibles:**
- `Sucesos.get_all()` - Obtiene todos los eventos con detalles completos
- `Sucesos.get_by_stage(stage_id, substage_id=0)` - Obtiene eventos filtrados por etapa/subetapa

**Propiedades de Suceso (evento):**
- `id` (int) - ID del evento
- `codigo` (str) - Código del evento
- `nombre` (str) - Nombre del evento
- `descripcion` (str) - Descripción del evento
- `frecuencia` (int) - Nivel de frecuencia (0-5)
- `consecuencia` (int) - Nivel de consecuencia (0-5)
- `riesgo` (int) - Nivel de riesgo (0-3: Bajo, Medio, Alto, Muy Alto)
- `error_humano` (bool) - Si involucra error humano
- `base` (bool) - Si es evento base
- `activo` (bool) - Si está activo
- `completado` (bool) - Si está completado/aplicable
- `etapa_id` (int) - ID de etapa padre
- `subetapa_id` (int) - ID de subetapa padre
- `etapa_nombre` (str) - Nombre de etapa
- `etapa_codigo` (str) - Código de etapa
- `subetapa_nombre` (str) - Nombre de subetapa

**Ejemplos:**
```python
# Obtener todos los eventos
events = Sucesos.get_all()
print(f"Total de eventos: {len(events)}")

# Filtrar eventos de alto riesgo
high_risk = [e for e in events if e['riesgo'] >= 2]
print(f"Eventos de alto riesgo: {len(high_risk)}")

# Obtener eventos de una etapa específica
stage_events = Sucesos.get_by_stage(1)
print(f"Eventos en etapa 1: {len(stage_events)}")

# Obtener eventos de una subetapa específica
substage_events = Sucesos.get_by_stage(1, 5)
print(f"Eventos en subetapa 5 de etapa 1: {len(substage_events)}")

# Análisis por etapa
for event in events:
    print(f"{event['codigo']}: Riesgo={event['riesgo']}, F={event['frecuencia']}, C={event['consecuencia']}")
```

### 05_test_python_defensas_api.py
Prueba el módulo Defensas para acceso a defensas (barreras y reductores):

**Métodos disponibles:**
- `Defensas.get_all(event_id=0)` - Obtiene todas las defensas (opcional: filtrar por evento)
- `Defensas.get_by_type(tipo, event_id=0)` - Obtiene defensas por tipo

**Tipos de Defensa:**
- **Tipo 0**: Reductor de Frecuencia - Reduce frecuencia del evento
- **Tipo 1**: Barrera - Previene progresión del evento
- **Tipo 2**: Reductor de Consecuencia - Reduce consecuencias del evento

**Propiedades de Defensa:**
- `id` (int) - ID de la defensa
- `codigo` (str) - Código de la defensa
- `nombre` (str) - Nombre de la defensa
- `descripcion` (str) - Descripción de la defensa
- `robustez` (int) - Nivel de robustez (0-3)
- `tipo` (int) - Tipo de defensa (0, 1, 2)
- `tipo_str` (str) - Nombre del tipo en español
- `activo` (bool) - Si está activa/aplicable
- `suceso_id` (int) - ID del evento asociado
- `etapa_orden_id` (int) - ID de etapa para ordenamiento
- `etapa_nombre` (str) - Nombre de etapa
- `etapa_codigo` (str) - Código de etapa

**Ejemplos:**
```python
# Obtener todas las defensas
all_defenses = Defensas.get_all()
print(f"Total de defensas: {len(all_defenses)}")

# Obtener defensas de un evento específico
event_defenses = Defensas.get_all(event_id=10)
print(f"Defensas del evento 10: {len(event_defenses)}")

# Obtener solo barreras (tipo 1)
barriers = Defensas.get_by_type(1)
print(f"Total de barreras: {len(barriers)}")

# Obtener reductores de frecuencia para un evento
freq_reducers = Defensas.get_by_type(0, event_id=5)
print(f"Reductores de frecuencia del evento 5: {len(freq_reducers)}")

# Contar defensas activas vs inactivas
active = sum(1 for d in all_defenses if d['activo'])
print(f"Defensas activas: {active} / {len(all_defenses)}")

# Análisis por tipo
for tipo in [0, 1, 2]:
    defenses = Defensas.get_by_type(tipo)
    print(f"Tipo {tipo}: {len(defenses)} defensas")
```

### test_python_integration_complete.py
Test completo de integración que demuestra el uso combinado de todos los módulos Python:

**Incluye:**
1. Verificación de disponibilidad de todos los módulos (Marrta, Database, Etapas, Sucesos, Defensas)
2. Análisis de estructura del modelo (etapas, subetapas, eventos, defensas)
3. Análisis de distribución de riesgo en eventos
4. Análisis de tipos y robustez de defensas
5. Integración etapa-evento usando Sucesos.get_by_stage()
6. Análisis de cobertura de defensas por evento
7. Distribución de tipos de defensa por evento
8. Consultas SQL complejas integrando múltiples tablas
9. Filtrado por tipo de defensa
10. Resumen final de verificación

**Uso recomendado:**
Este script sirve como ejemplo completo de cómo usar todas las APIs juntas para análisis comprehensivos del modelo.

### generar_flujograma.py
**Script principal para generar flujogramas HTML interactivos**

Este script es el ejemplo práctico más completo que genera una visualización HTML del modelo de riesgo completo.

**Características:**
- Lee todas las etapas, subetapas, eventos y defensas del modelo
- Genera HTML auto-contenido con CSS embebido
- Visualización jerárquica: Etapas → Subetapas → Eventos → Defensas
- Código de colores por nivel de riesgo:
  - Verde: Riesgo Bajo
  - Amarillo: Riesgo Medio
  - Naranja: Riesgo Alto
  - Rojo: Riesgo Muy Alto
- Muestra defensas con iconos por tipo (RF, B, RC)
- Incluye información de robustez de cada defensa
- Resalta eventos con error humano
- Leyenda interactiva de niveles de riesgo
- Totalmente configurable mediante diccionario CONFIG

**Configuración disponible:**
```python
CONFIG = {
    'title': 'Flujograma del Modelo de Riesgo',  # Título del documento
    'show_defenses': True,      # Mostrar defensas
    'show_inactive': False,     # Mostrar elementos inactivos
    'color_by_risk': True,      # Colorear por nivel de riesgo
}
```

**Uso:**
1. Abrir Editor Python en MARRTA
2. Cargar `generar_flujograma.py`
3. Ejecutar el script
4. Copiar el HTML generado en la consola
5. Guardar como archivo `.html`
6. Abrir en navegador

**Salida:**
El script genera HTML completo que incluye:
- Encabezado con título configurable
- Leyenda de niveles de riesgo con colores
- Sección por cada etapa con:
  - Nombre y código de etapa
  - Lista de eventos con:
    - Código y nombre del evento
    - Etiqueta de subetapa
    - Información de riesgo (F, C, R)
    - Indicador de error humano
    - Descripción (truncada si es muy larga)
    - Lista de defensas con:
      - Tipo (RF, B, RC)
      - Nombre de la defensa
      - Nivel de robustez
- Resumen al final con estadísticas

**Personalización:**
Puedes modificar el script para:
- Cambiar los colores de riesgo (diccionario RISK_COLORS)
- Cambiar los colores de defensas (diccionario DEFENSE_COLORS)
- Filtrar por etapas específicas
- Agregar más información de cada elemento
- Cambiar el estilo CSS embebido
- Generar formatos alternativos (Markdown, PDF, etc.)

**Ejemplo de uso avanzado:**
```python
# Generar solo eventos de alto riesgo
events = [e for e in Sucesos.get_all() if e['riesgo'] >= 2]

# Generar solo para etapas específicas
target_stages = [1, 2, 3]
stages = [s for s in Etapas.get_all() if s.id in target_stages]

# Personalizar colores
RISK_COLORS = {
    0: '#00FF00',  # Verde brillante
    1: '#FFFF00',  # Amarillo brillante
    2: '#FF8000',  # Naranja brillante
    3: '#FF0000',  # Rojo brillante
}
```

## Documentación Completa Python API

Para referencia completa de la API Python, consulta:
`Documentation/Support/PYTHON_API_REFERENCE.md`

Este documento incluye:
- Descripción detallada de todos los módulos
- Todos los métodos disponibles con parámetros
- Tipos de retorno completos
- Ejemplos de código para cada función
- Casos de uso comunes
- Mejores prácticas
- Manejo de errores
- Notas de rendimiento

## Estructura de datos Python

### Database.query() retorna:
```python
[
    {'id': 1, 'codigo': 'E01', 'nombre': 'Etapa 1', ...},
    {'id': 2, 'codigo': 'E02', 'nombre': 'Etapa 2', ...},
    ...
]
```

### Etapas.get_all() retorna:
```python
[
    Etapa(id=1, codigo='E01', nombre='Etapa 1', ...),
    Etapa(id=2, codigo='E02', nombre='Etapa 2', ...),
    ...
]
```

### Etapa.imprimir() retorna:
```python
(
    12,                    # ID (int)
    'E01',                 # código (string)
    'Nombre de etapa',     # nombre (string)
    'Descripción...',      # descripción (string)
    (23, 45, 67, 89)      # IDs de subetapas (tuple de int)
)
```

## Tablas principales de la base de datos

- **Etapa**: Etapas del proceso
- **Subetapa**: Subetapas dentro de cada etapa
- **Suceso**: Eventos iniciadores
- **SucesoSubEtapa**: Relación eventos-subetapas
- **Defensa**: Defensas/barreras
- **DefensaSuceso**: Relación defensas-eventos

## Documentación adicional

- **Guía rápida**: `Documentation/Support/PYTHON_SCRIPTING_QUICK_GUIDE.md`
- **Guía visual**: `Documentation/Support/PYTHON_SCRIPTING_VISUAL_GUIDE.md`
- **Implementación**: `Documentation/Support/PYTHON_SCRIPTING_IMPLEMENTATION.md`
- **Ayuda en app**: Botón `[Documentación]` en el editor Python

## Consejos para Python

1. **Usa f-strings para formateo:**
   ```python
   print(f"Etapa {etapa.codigo}: {etapa.nombre}")
   ```

2. **Desempaqueta tuplas de imprimir():**
   ```python
   id_etapa, codigo, nombre, desc, subs = etapa.imprimir()
   ```

3. **Verifica resultados vacíos:**
   ```python
   results = Database.query(sql)
   if len(results) == 0:
       print("No se encontraron resultados")
   ```

4. **Combina APIs para análisis completos:**
   ```python
   etapas = Etapas.get_all()
   for etapa in etapas:
       sql = f"SELECT COUNT(*) as c FROM Subetapa WHERE idEtapa = {etapa.id}"
       count = Database.query(sql)[0]['c']
       print(f"{etapa.nombre}: {count} subetapas")
   ```

## Notas Python

- **Python 3.12 requerido**: La aplicación requiere Python 3.12 instalado
- **Sin sandboxing**: Los scripts tienen acceso completo a la base de datos
- **Ejecución síncrona**: Los scripts bloquean la UI durante ejecución
- **Sin imports externos**: No se pueden importar módulos Python adicionales
- **Manejo de errores**: Use try-except para capturar errores SQL

