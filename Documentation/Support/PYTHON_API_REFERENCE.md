# MARRTA Python API Reference

This document describes the Python scripting API available in MARRTA for reading and writing model data.

## Overview

MARRTA's Python scripting engine provides access to the risk analysis model through several object modules:

- **Marrta**: Base application features
- **Database**: Direct SQL query access
- **Etapas**: Stages and substages (read-only)
- **Sucesos**: Events/initiating events (read-only)
- **Defensas**: Defenses (barriers and reducers) (read-only)

## Debugging Complex Scripts

When working with complex scripts that use external modules (like pandas, numpy, etc.) or perform advanced data processing, it's highly recommended to wrap your entire script in an error handling function. This provides comprehensive error reporting with full stack traces, making debugging much easier.

**Recommended Pattern:**
```python
import sys
import traceback

def safe_execute():
    try:
        # Put all your script code here
        print("Step 1: Loading data...")
        # Your code...
        
        print("Step 2: Processing...")
        # More code...
        
        print("Step 3: Generating output...")
        # Final code...
        
    except Exception as e:
        print(f"FULL ERROR: {type(e).__name__}")
        print(f"MESSAGE: {str(e)}")
        traceback.print_exc()
        sys.stderr.flush()
        sys.stdout.flush()

# Execute the script
safe_execute()
```

**Benefits:**
- Captures all exceptions with complete type information
- Shows full stack trace for easier debugging
- Prevents silent failures
- Useful for scripts with pandas, complex calculations, or file I/O
- Helps identify issues with data structure mismatches

**Example:** See `test_scripts/generar_flujograma_v25_desde_bd.py` for a production example using this pattern.

## Marrta Module

### Methods

#### `get_version()`
Returns the MARRTA version string.

**Returns:** String with version information

**Example:**
```python
version = Marrta.get_version()
print(f"MARRTA Version: {version}")
```

---

## Database Module

Direct SQL query access to the model database.

### Methods

#### `query(sql)`
Executes a SQL query and returns results as a list of dictionaries.

**Parameters:**
- `sql` (string): SQL query to execute

**Returns:** List of dictionaries, where each dictionary represents a row with column names as keys

**Example:**
```python
# Get all stages
results = Database.query("SELECT id, codigo, nombre FROM etapas ORDER BY orden")
for row in results:
    print(f"{row['codigo']}: {row['nombre']}")

# Count events with high risk
results = Database.query("SELECT COUNT(*) as count FROM suceso_iniciador WHERE riesgo >= 2")
high_risk_count = results[0]['count']
print(f"High risk events: {high_risk_count}")
```

**Note:** Use parameterized queries when possible. This method provides direct database access for advanced queries.

---

## Etapas Module

Access to stages (etapas) and their substages.

### Methods

#### `get_all()`
Returns all stages from the model.

**Returns:** List of Etapa objects with properties:
- `id` (int): Stage ID
- `codigo` (str): Stage code
- `nombre` (str): Stage name
- `descripcion` (str): Stage description
- `orden` (int): Display order

**Example:**
```python
stages = Etapas.get_all()
print(f"Total stages: {len(stages)}")

for stage in stages:
    print(f"[{stage.codigo}] {stage.nombre} (Order: {stage.orden})")
```

### Etapa Object Methods

#### `imprimir()`
Returns detailed information about the stage including substage IDs.

**Returns:** Tuple with (id, codigo, nombre, descripcion, tuple_of_substage_ids)

**Example:**
```python
stages = Etapas.get_all()
for stage in stages:
    id, codigo, nombre, desc, substage_ids = stage.imprimir()
    print(f"Stage {id}: {codigo} - {nombre}")
    print(f"  Substages: {len(substage_ids)}")
    
    # Get substage details from database
    if len(substage_ids) > 0:
        ids_str = ",".join(str(id) for id in substage_ids)
        sql = f"SELECT id, nombre FROM subetapas WHERE id IN ({ids_str})"
        substages = Database.query(sql)
        for sub in substages:
            print(f"    - {sub['nombre']}")
```

---

## Sucesos Module

Access to initiating events (sucesos iniciadores).

### Methods

#### `get_all()`
Returns all events from the model.

**Returns:** List of dictionaries with event properties:
- `id` (int): Event ID
- `codigo` (str): Event code
- `nombre` (str): Event name
- `descripcion` (str): Event description
- `frecuencia` (int): Frequency level (0-5)
- `consecuencia` (int): Consequence level (0-5)
- `riesgo` (int): Risk level (0-3): 0=Low, 1=Medium, 2=High, 3=Very High
- `error_humano` (bool): Whether event involves human error
- `base` (bool): Whether this is a base event
- `activo` (bool): Whether event is active
- `completado` (bool): Whether event is completed/applicable
- `etapa_id` (int): Parent stage ID
- `subetapa_id` (int): Parent substage ID (may be 0)
- `etapa_nombre` (str): Parent stage name
- `etapa_codigo` (str): Parent stage code
- `subetapa_nombre` (str): Parent substage name

**Example:**
```python
events = Sucesos.get_all()
print(f"Total events: {len(events)}")

# Find high-risk events
high_risk = [e for e in events if e['riesgo'] >= 2]
print(f"High risk events: {len(high_risk)}")

for event in high_risk[:5]:
    print(f"  {event['codigo']}: {event['nombre']}")
    print(f"    Risk: {event['riesgo']}, F: {event['frecuencia']}, C: {event['consecuencia']}")
```

#### `get_by_stage(stage_id, substage_id=0)`
Returns events filtered by stage and optionally substage.

**Parameters:**
- `stage_id` (int): Stage ID to filter by
- `substage_id` (int, optional): Substage ID to filter by. If 0 or omitted, returns all events in the stage.

**Returns:** List of event dictionaries (same structure as get_all())

**Example:**
```python
# Get all events in stage 1
stage1_events = Sucesos.get_by_stage(1)
print(f"Stage 1 has {len(stage1_events)} events")

# Get events in specific substage
substage_events = Sucesos.get_by_stage(1, 5)
print(f"Substage events: {len(substage_events)}")
```

---

## Defensas Module

Access to defenses (barriers, frequency reducers, and consequence reducers).

### Defense Types

Defenses are categorized by type:
- **Type 0**: Frequency Reducer (Reductor de Frecuencia) - Reduces event frequency
- **Type 1**: Barrier (Barrera) - Prevents event progression
- **Type 2**: Consequence Reducer (Reductor de Consecuencia) - Reduces event consequences

### Methods

#### `get_all(event_id=0)`
Returns all defenses, optionally filtered by event.

**Parameters:**
- `event_id` (int, optional): Event ID to filter by. If 0 or omitted, returns all defenses.

**Returns:** List of dictionaries with defense properties:
- `id` (int): Defense ID
- `codigo` (str): Defense code
- `nombre` (str): Defense name
- `descripcion` (str): Defense description
- `robustez` (int): Robustness/hardiness level (0-3)
- `tipo` (int): Defense type (0=FreqReducer, 1=Barrier, 2=ConsReducer)
- `tipo_str` (str): Defense type name in Spanish
- `activo` (bool): Whether defense is active/applicable
- `suceso_id` (int): Associated event ID
- `etapa_orden_id` (int): Stage ID for ordering
- `etapa_nombre` (str): Stage name
- `etapa_codigo` (str): Stage code

**Example:**
```python
# Get all defenses
all_defenses = Defensas.get_all()
print(f"Total defenses: {len(all_defenses)}")

# Get defenses for specific event
event_defenses = Defensas.get_all(event_id=10)
print(f"Event 10 has {len(event_defenses)} defenses")

# Count active defenses
active = sum(1 for d in all_defenses if d['activo'])
print(f"Active defenses: {active} / {len(all_defenses)}")
```

#### `get_by_type(tipo, event_id=0)`
Returns defenses filtered by type and optionally by event.

**Parameters:**
- `tipo` (int): Defense type (0=FreqReducer, 1=Barrier, 2=ConsReducer)
- `event_id` (int, optional): Event ID to filter by. If 0 or omitted, returns all defenses of the specified type.

**Returns:** List of defense dictionaries (same structure as get_all())

**Example:**
```python
# Get all barriers (type 1)
barriers = Defensas.get_by_type(1)
print(f"Total barriers: {len(barriers)}")

# Get frequency reducers for event 5
freq_reducers = Defensas.get_by_type(0, event_id=5)
print(f"Event 5 frequency reducers: {len(freq_reducers)}")

# Analyze defense types
for tipo in [0, 1, 2]:
    defenses = Defensas.get_by_type(tipo)
    print(f"Type {tipo}: {len(defenses)} defenses")
```

---

## Complete Examples

### Example 1: Model Summary Report

```python
"""Generate a complete model summary"""

print("=" * 60)
print("MARRTA Model Summary Report")
print("=" * 60)
print()

# Get data
stages = Etapas.get_all()
events = Sucesos.get_all()
defenses = Defensas.get_all()

# Overview
print(f"Stages: {len(stages)}")
print(f"Events: {len(events)}")
print(f"Defenses: {len(defenses)}")
print()

# Events by risk level
print("Events by Risk Level:")
risk_names = {0: "Low", 1: "Medium", 2: "High", 3: "Very High"}
for risk_level in sorted(set(e['riesgo'] for e in events)):
    count = sum(1 for e in events if e['riesgo'] == risk_level)
    name = risk_names.get(risk_level, f"Level {risk_level}")
    print(f"  {name}: {count}")
print()

# Defenses by type
print("Defenses by Type:")
type_names = {0: "Frequency Reducers", 1: "Barriers", 2: "Consequence Reducers"}
for tipo in [0, 1, 2]:
    count = sum(1 for d in defenses if d['tipo'] == tipo)
    name = type_names[tipo]
    print(f"  {name}: {count}")
```

### Example 2: Events per Stage Analysis

```python
"""Analyze event distribution across stages"""

stages = Etapas.get_all()
events = Sucesos.get_all()

print("Events per Stage:")
print("-" * 60)

for stage in stages:
    stage_events = [e for e in events if e['etapa_id'] == stage.id]
    print(f"\n{stage.codigo} - {stage.nombre}: {len(stage_events)} events")
    
    if len(stage_events) > 0:
        # Count by risk
        high_risk = sum(1 for e in stage_events if e['riesgo'] >= 2)
        print(f"  High risk: {high_risk}")
        
        # Count human error
        human_error = sum(1 for e in stage_events if e['error_humano'])
        print(f"  Human error: {human_error}")
```

### Example 3: Defense Coverage Analysis

```python
"""Analyze defense coverage for events"""

events = Sucesos.get_all()
all_defenses = Defensas.get_all()

print("Defense Coverage Analysis:")
print("-" * 60)

events_with_defenses = 0
events_without_defenses = 0

for event in events:
    event_defenses = [d for d in all_defenses if d['suceso_id'] == event['id']]
    
    if len(event_defenses) > 0:
        events_with_defenses += 1
    else:
        events_without_defenses += 1

print(f"Events with defenses: {events_with_defenses}")
print(f"Events without defenses: {events_without_defenses}")
print(f"Coverage: {events_with_defenses / len(events) * 100:.1f}%")

# Find events with most defenses
event_defense_counts = []
for event in events:
    count = sum(1 for d in all_defenses if d['suceso_id'] == event['id'])
    if count > 0:
        event_defense_counts.append((event, count))

event_defense_counts.sort(key=lambda x: x[1], reverse=True)

print("\nTop 5 events by defense count:")
for event, count in event_defense_counts[:5]:
    print(f"  {event['codigo']}: {count} defenses")
```

### Example 4: Generate Flowchart HTML

See the complete flowchart generator script at:
`test_scripts/generar_flujograma.py`

This script generates an interactive HTML visualization of the entire risk model with stages, events, and defenses.

---

## Notes

### Database Tables

The main database tables are:
- `etapas` - Stages
- `subetapas` - Substages
- `suceso_iniciador` - Initiating events
- `defensa` - Defenses

### Read-Only Access

The current API provides read-only access to the model. Write operations should be performed through the MARRTA user interface.

### Performance

For large models with thousands of events/defenses, consider filtering data at the database level using SQL queries rather than filtering in Python after retrieval.

### Error Handling

Always wrap API calls in try-except blocks to handle potential database errors:

```python
try:
    events = Sucesos.get_all()
    # Process events...
except Exception as e:
    print(f"Error: {e}")
```

---

## Version History

- **v1.5+**: Initial Python API implementation
  - Marrta module with version info
  - Database module with query() method
  - Etapas module with get_all() and Etapa.imprimir()
  - Sucesos module with get_all() and get_by_stage()
  - Defensas module with get_all() and get_by_type()
