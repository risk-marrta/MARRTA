# Implementation Summary: Python API for Flowchart Generation

## Overview

This PR implements a comprehensive Python API for MARRTA's scripting engine, enabling users to generate interactive HTML flowcharts from loaded risk models. The implementation adds two new Python modules (`Sucesos` and `Defensas`) that provide read access to events and defenses, complementing the existing `Etapas` module.

## Problem Statement

The issue requested:
> "Apoyado en el motor de scripting que hemos creado en python, tienes que crear un script python para dicho engine, que genere exactamente ese tipo de salida html pero a partir de una base de datos del modelo cargada en MARRTA, no del xlsx."

Translation: Create a Python script for the scripting engine that generates HTML flowchart output from a MARRTA model database (not from Excel).

## Solution Overview

### 1. Extended Python API (pythonscriptingengine.cpp/h)

Added two new Python modules to access model data:

#### Sucesos Module (Events)
- `Sucesos.get_all()` → Returns list of all events with complete details
- `Sucesos.get_by_stage(stage_id, substage_id=0)` → Returns filtered events

**Event properties:**
- Basic: id, codigo, nombre, descripcion
- Risk data: frecuencia, consecuencia, riesgo (0-3)
- Flags: error_humano, base, activo, completado
- Relationships: etapa_id, subetapa_id, etapa_nombre, etapa_codigo, subetapa_nombre

#### Defensas Module (Defenses)
- `Defensas.get_all(event_id=0)` → Returns all defenses (optional event filter)
- `Defensas.get_by_type(tipo, event_id=0)` → Returns defenses by type

**Defense types:**
- Type 0: Frequency Reducer (Reductor de Frecuencia)
- Type 1: Barrier (Barrera)
- Type 2: Consequence Reducer (Reductor de Consecuencia)

**Defense properties:**
- Basic: id, codigo, nombre, descripcion
- Technical: robustez (0-3), tipo (0-2), tipo_str
- State: activo
- Relationships: suceso_id, etapa_orden_id, etapa_nombre, etapa_codigo

### 2. Flowchart Generator Script

**File:** `test_scripts/generar_flujograma.py`

Complete Python script that:
1. Fetches all stages, substages, events, and defenses from the loaded model
2. Organizes data hierarchically (stages → substages → events → defenses)
3. Generates self-contained interactive HTML with embedded CSS
4. Color-codes events by risk level (gray/green/orange/red)
5. Shows defenses with type indicators and robustness levels
6. Includes legend and summary statistics

**Key features:**
- Configurable via `CONFIG` dictionary (title, show defenses, inactive items, color coding)
- Pure Python implementation using only MARRTA API (no external dependencies)
- Generates production-ready HTML that can be saved and shared
- Handles edge cases (events without defenses, empty stages, missing data)

### 3. Test Scripts

Created comprehensive test suite:

- **test_python_sucesos_api.py**: Tests all Sucesos methods, statistics, and stage filtering
- **test_python_defensas_api.py**: Tests all Defensas methods, type filtering, and event integration
- **test_python_integration_complete.py**: Full integration test covering all modules with 9 test sections

### 4. Documentation

Created extensive documentation:

- **PYTHON_API_REFERENCE.md**: Complete API reference with all methods, parameters, return types, and examples
- **PYTHON_API_FLOWCHART_IMPLEMENTATION.md**: Architecture overview, data flow diagrams, implementation details
- **PYTHON_FLOWCHART_EXAMPLES.md**: 5 practical examples showing real-world usage patterns
- **test_scripts/README.md**: Updated with comprehensive Python API section

## Technical Implementation

### C++ Side (pythonscriptingengine.cpp)

```cpp
// New functions for Sucesos module
static PyObject* sucesos_get_all(PyObject *self, PyObject *args);
static PyObject* sucesos_get_by_stage(PyObject *self, PyObject *args);

// New functions for Defensas module
static PyObject* defensas_get_all(PyObject *self, PyObject *args);
static PyObject* defensas_get_by_type(PyObject *self, PyObject *args);

// Module registration in constructor
void PythonScriptingEngine::registerSucesosObject();
void PythonScriptingEngine::registerDefensasObject();
```

**Implementation approach:**
1. Use QSqlQuery for database access with prepared statements
2. Return data as Python lists of dictionaries (PyDict)
3. Include LEFT JOIN to get related entity names in single query
4. Helper functions for Qt-Python type conversions
5. Proper error handling with PyErr_SetString

### Python Side (generar_flujograma.py)

```python
# Data collection
stages = Etapas.get_all()
events = Sucesos.get_all()
defenses = Defensas.get_all()

# Data organization
events_by_stage = {}
defenses_by_event = {}

# HTML generation with styling
html = generate_header() + generate_legend() + 
       generate_stages() + generate_footer()
```

## Files Modified

1. **pythonscriptingengine.h**
   - Added `registerSucesosObject()` method declaration
   - Added `registerDefensasObject()` method declaration
   - Updated class documentation

2. **pythonscriptingengine.cpp**
   - Added Sucesos module implementation (200+ lines)
   - Added Defensas module implementation (200+ lines)
   - Added module registration calls in constructor
   - Added stub implementations for non-Python builds

3. **RELEASE_INFO.md**
   - Added PR entry with complete list of changes

4. **test_scripts/README.md**
   - Added comprehensive Python API section
   - Updated API comparison table
   - Added usage examples

## Files Added

### Test Scripts
1. `test_scripts/test_python_sucesos_api.py` (138 lines)
2. `test_scripts/test_python_defensas_api.py` (172 lines)
3. `test_scripts/test_python_integration_complete.py` (334 lines)
4. `test_scripts/generar_flujograma.py` (420 lines)

### Documentation
5. `Documentation/Support/PYTHON_API_REFERENCE.md` (571 lines)
6. `Documentation/Support/PYTHON_API_FLOWCHART_IMPLEMENTATION.md` (539 lines)
7. `Documentation/Support/PYTHON_FLOWCHART_EXAMPLES.md` (630 lines)

**Total:** 7 new files, 2,804 lines of code and documentation

## Usage Example

```python
# 1. Load script in MARRTA Python Editor
# Menu: Modelo → Scripting → Python
# Click [Cargar] and select generar_flujograma.py

# 2. Execute script
# Click [Ejecutar]

# 3. Output is generated to console
# Copy HTML output and save to .html file

# 4. Open in browser
# Double-click the saved HTML file
```

## API Examples

### Get all events
```python
events = Sucesos.get_all()
print(f"Total events: {len(events)}")

for event in events:
    print(f"{event['codigo']}: {event['nombre']}")
    print(f"  Risk: {event['riesgo']}, F: {event['frecuencia']}, C: {event['consecuencia']}")
```

### Get events by stage
```python
stages = Etapas.get_all()
for stage in stages:
    events = Sucesos.get_by_stage(stage.id)
    print(f"{stage.codigo}: {len(events)} events")
```

### Get defenses by type
```python
# Get all barriers (type 1)
barriers = Defensas.get_by_type(1)
active_barriers = [b for b in barriers if b['activo']]
print(f"Active barriers: {len(active_barriers)} / {len(barriers)}")
```

### Analyze defense coverage
```python
events = Sucesos.get_all()
for event in events:
    defenses = Defensas.get_all(event['id'])
    print(f"{event['codigo']}: {len(defenses)} defenses")
```

## Testing

All new functionality has been tested:

1. **Unit Tests**: Each module tested individually
   - Sucesos.get_all() returns correct structure
   - Sucesos.get_by_stage() filters correctly
   - Defensas.get_all() returns all defenses
   - Defensas.get_by_type() filters by type correctly

2. **Integration Tests**: Cross-module functionality
   - Events can be cross-referenced with defenses
   - Stages can be cross-referenced with events
   - SQL queries return correct joined data

3. **Real-World Test**: Flowchart generator
   - Processes complete model successfully
   - Generates valid HTML
   - Handles edge cases (no defenses, empty stages)
   - Output renders correctly in browsers

## Design Decisions

### 1. Read-Only API
The current implementation provides read-only access. This is intentional because:
- Safer for initial release
- Prevents accidental data corruption
- Write operations can be added later if needed
- Follows JavaScript API pattern (which also started read-only)

### 2. Dictionary Return Types
Chose Python dictionaries over custom objects because:
- More Pythonic and flexible
- Easy to serialize (JSON, etc.)
- Consistent with Database.query() pattern
- Simple to extend with new properties

### 3. Single Query per Method
Each API method executes one optimized SQL query because:
- Better performance than multiple queries
- LEFT JOIN includes related data
- Simpler to understand and maintain
- Reduces database round-trips

### 4. Color Coding by Risk
Flowchart uses risk-based coloring because:
- Immediate visual risk identification
- Industry standard practice
- Easy to scan for high-risk areas
- Configurable if different scheme needed

## Future Enhancements

Possible future additions (not in this PR):

1. **Write Operations**
   - `Sucesos.update(event_id, properties)`
   - `Defensas.set_active(defense_id, active)`

2. **Additional Filtering**
   - `Sucesos.get_by_risk(min_level)`
   - `Defensas.get_active_only()`

3. **Statistics Functions**
   - `Sucesos.get_statistics()`
   - `Defensas.get_coverage_stats()`

4. **Bulk Operations**
   - `Sucesos.bulk_update(updates)`
   - `Defensas.activate_all(event_id)`

## Compatibility

- **Python Version**: Requires Python 3.12 (already required by existing Python engine)
- **MARRTA Version**: Compatible with current version
- **Database**: Works with existing SQLite schema
- **Optional Compilation**: Python support is optional (PYTHON_ENABLED flag)
- **Backwards Compatible**: Does not break existing Python scripts

## Benefits

1. **Enables Automation**: Users can script complex analyses and reports
2. **Flexible Reporting**: Generate custom visualizations and outputs
3. **Data Analysis**: Powerful querying and filtering capabilities
4. **Documentation**: Complete reference and examples provided
5. **Extensible**: Architecture supports future enhancements
6. **No Dependencies**: Uses only built-in Python and MARRTA APIs

## Conclusion

This PR successfully implements a comprehensive Python API for flowchart generation and model analysis. The implementation:

✅ Meets the requirements specified in the issue  
✅ Provides complete read access to events and defenses  
✅ Includes a production-ready flowchart generator  
✅ Has comprehensive test coverage  
✅ Is fully documented with examples  
✅ Follows MARRTA coding standards  
✅ Is backwards compatible  
✅ Enables powerful scripting capabilities  

The new API empowers users to create custom analyses, reports, and visualizations of their risk models using Python scripts within MARRTA.
