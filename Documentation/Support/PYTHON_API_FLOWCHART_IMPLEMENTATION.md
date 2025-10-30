# Python API Structure for Flowchart Generation

This document provides a visual overview of the Python API implementation for generating flowcharts from MARRTA models.

## API Architecture

```
MARRTA Python Scripting Engine
├── Marrta (Base Module)
│   └── get_version() -> str
│
├── Database (SQL Access Module)
│   └── query(sql: str) -> List[Dict]
│
├── Etapas (Stages Module)
│   ├── get_all() -> List[Etapa]
│   └── Etapa Object
│       ├── Properties: id, codigo, nombre, descripcion, orden
│       └── imprimir() -> Tuple(id, codigo, nombre, desc, (substage_ids))
│
├── Sucesos (Events Module) [NEW]
│   ├── get_all() -> List[Dict]
│   └── get_by_stage(stage_id, substage_id=0) -> List[Dict]
│
└── Defensas (Defenses Module) [NEW]
    ├── get_all(event_id=0) -> List[Dict]
    └── get_by_type(tipo, event_id=0) -> List[Dict]
```

## Data Flow for Flowchart Generation

```
┌─────────────────────────────────────────────────────────────┐
│                   MARRTA Database                           │
│  (SQLite with tables: etapas, subetapas,                    │
│   suceso_iniciador, defensa)                                │
└──────────────────┬──────────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────────┐
│              Python Scripting Engine                        │
│                                                             │
│  C++ Functions (pythonscriptingengine.cpp)                  │
│  ├── sucesos_get_all()                                      │
│  ├── sucesos_get_by_stage()                                 │
│  ├── defensas_get_all()                                     │
│  └── defensas_get_by_type()                                 │
│                                                             │
│  Registered as Python Modules:                              │
│  ├── Sucesos (PyModuleDef)                                  │
│  └── Defensas (PyModuleDef)                                 │
└──────────────────┬──────────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────────┐
│              Python Script Layer                            │
│                                                             │
│  generar_flujograma.py                                      │
│  ├── 1. Fetch all stages (Etapas.get_all())                │
│  ├── 2. Fetch all events (Sucesos.get_all())               │
│  ├── 3. Fetch all defenses (Defensas.get_all())            │
│  ├── 4. Organize data by stage                             │
│  ├── 5. Generate HTML structure                            │
│  └── 6. Apply risk-based styling                           │
└──────────────────┬──────────────────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────────────────┐
│              HTML Output                                    │
│                                                             │
│  Interactive visualization with:                            │
│  ├── Stage sections (color-coded headers)                   │
│  ├── Event boxes (color by risk level)                     │
│  ├── Defense lists (color by type)                         │
│  └── Legend and statistics                                 │
└─────────────────────────────────────────────────────────────┘
```

## Object Structure

### Suceso (Event) Object
```python
{
    'id': int,                    # Event ID
    'codigo': str,                # Event code (e.g., "SI-01")
    'nombre': str,                # Event name
    'descripcion': str,           # Event description
    'frecuencia': int,            # Frequency level (0-5)
    'consecuencia': int,          # Consequence level (0-5)
    'riesgo': int,                # Risk level (0-3)
    'error_humano': bool,         # Human error flag
    'base': bool,                 # Base event flag
    'activo': bool,               # Active flag
    'completado': bool,           # Completed flag
    'etapa_id': int,              # Parent stage ID
    'subetapa_id': int,           # Parent substage ID
    'etapa_nombre': str,          # Stage name
    'etapa_codigo': str,          # Stage code
    'subetapa_nombre': str        # Substage name
}
```

### Defensa (Defense) Object
```python
{
    'id': int,                    # Defense ID
    'codigo': str,                # Defense code
    'nombre': str,                # Defense name
    'descripcion': str,           # Defense description
    'robustez': int,              # Hardiness/robustness (0-3)
    'tipo': int,                  # Type (0=FreqRed, 1=Barrier, 2=ConsRed)
    'tipo_str': str,              # Type name in Spanish
    'activo': bool,               # Active/applicable flag
    'suceso_id': int,             # Associated event ID
    'etapa_orden_id': int,        # Stage ID for ordering
    'etapa_nombre': str,          # Stage name
    'etapa_codigo': str           # Stage code
}
```

## Flowchart Generation Process

### Step 1: Data Collection
```python
# Collect all model data
stages = Etapas.get_all()           # Get all stages
events = Sucesos.get_all()          # Get all events
defenses = Defensas.get_all()       # Get all defenses
```

### Step 2: Data Organization
```python
# Group events by stage
events_by_stage = {}
for event in events:
    stage_id = event['etapa_id']
    if stage_id not in events_by_stage:
        events_by_stage[stage_id] = []
    events_by_stage[stage_id].append(event)

# Group defenses by event
defenses_by_event = {}
for defense in defenses:
    event_id = defense['suceso_id']
    if event_id not in defenses_by_event:
        defenses_by_event[event_id] = []
    defenses_by_event[event_id].append(defense)
```

### Step 3: HTML Generation
```python
# For each stage
for stage in stages:
    # Generate stage header
    html += generate_stage_header(stage)
    
    # For each event in stage
    for event in events_by_stage.get(stage.id, []):
        # Generate event box (colored by risk)
        html += generate_event_box(event)
        
        # For each defense in event
        for defense in defenses_by_event.get(event['id'], []):
            # Generate defense item
            html += generate_defense_item(defense)
```

## Visual Layout Structure

```
┌────────────────────────────────────────────────────────────┐
│                    FLUJOGRAMA TITLE                        │
├────────────────────────────────────────────────────────────┤
│  Legend: [Gray=Low] [Green=Med] [Orange=High] [Red=VHigh]│
├────────────────────────────────────────────────────────────┤
│                                                            │
│  ┌─────────────────────────────────────────────────────┐  │
│  │ [E01] STAGE 1 NAME                                  │  │
│  ├─────────────────────────────────────────────────────┤  │
│  │                                                     │  │
│  │  ╔══════════════════════════════════════╗          │  │
│  │  ║ [Substage 1.1]                       ║          │  │
│  │  ║ [SI-01] Event Name                   ║ (Color)  │  │
│  │  ║ Risk: High  F: 3  C: 4               ║          │  │
│  │  ║                                       ║          │  │
│  │  ║ Defensas:                            ║          │  │
│  │  ║   [RF] Frequency Reducer (R: 2)      ║          │  │
│  │  ║   [B]  Barrier Name (R: 3)           ║          │  │
│  │  ║   [RC] Consequence Reducer (R: 1)    ║          │  │
│  │  ╚══════════════════════════════════════╝          │  │
│  │                                                     │  │
│  │  ╔══════════════════════════════════════╗          │  │
│  │  ║ [Substage 1.2]                       ║          │  │
│  │  ║ [SI-02] Another Event                ║ (Color)  │  │
│  │  ║ ...                                  ║          │  │
│  │  ╚══════════════════════════════════════╝          │  │
│  │                                                     │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                            │
│  ┌─────────────────────────────────────────────────────┐  │
│  │ [E02] STAGE 2 NAME                                  │  │
│  ├─────────────────────────────────────────────────────┤  │
│  │  ... (more events)                                  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                            │
├────────────────────────────────────────────────────────────┤
│  Summary: X stages, Y events, Z defenses                  │
└────────────────────────────────────────────────────────────┘
```

## Color Coding

### Risk Levels (Event Background)
```
0 (Low)       → #A6A6A6 (Gray)
1 (Medium)    → #00B050 (Green)
2 (High)      → #FFD966 (Orange)
3 (Very High) → #FF0000 (Red)
```

### Defense Types (Border Color)
```
0 (Freq Reducer)  → #ADD8E6 (Light Blue)
1 (Barrier)       → #DDA0DD (Plum/Purple)
2 (Cons Reducer)  → #F0E68C (Khaki/Yellow-Green)
```

## Key Features

### 1. **Hierarchical Organization**
   - Top level: Stages (Etapas)
   - Second level: Substages within stages
   - Third level: Events within substages
   - Fourth level: Defenses for each event

### 2. **Visual Risk Assessment**
   - Immediate risk identification through color coding
   - Easy scanning for high-risk areas
   - Visual patterns in risk distribution

### 3. **Defense Coverage**
   - Quick view of which events have defenses
   - Defense type distribution visible at a glance
   - Robustness levels shown for each defense

### 4. **Interactive HTML**
   - Self-contained (no external dependencies)
   - Can be saved and shared
   - Opens in any modern web browser
   - Print-friendly layout

### 5. **Configurable Output**
   - Show/hide inactive elements
   - Show/hide defenses
   - Toggle risk-based coloring
   - Customizable colors and styling

## Example Use Cases

### 1. **Risk Overview Presentation**
Generate a complete flowchart for management review:
```python
CONFIG = {
    'title': 'Risk Model Overview - Management Review',
    'show_defenses': True,
    'show_inactive': False,
    'color_by_risk': True
}
```

### 2. **Defense Gap Analysis**
Focus on events without adequate defenses:
```python
# Filter events with few defenses
events = Sucesos.get_all()
for event in events:
    defenses = Defensas.get_all(event['id'])
    if len(defenses) < 2:
        print(f"Gap: {event['codigo']} has only {len(defenses)} defenses")
```

### 3. **Stage-by-Stage Review**
Generate separate flowcharts for each stage:
```python
stages = Etapas.get_all()
for stage in stages:
    events = Sucesos.get_by_stage(stage.id)
    # Generate HTML for this stage only
    # ... (filtering logic)
```

### 4. **High-Risk Focus Report**
Show only high and very-high risk events:
```python
events = [e for e in Sucesos.get_all() if e['riesgo'] >= 2]
# Generate flowchart with filtered events
```

## Implementation Details

### C++ Side (pythonscriptingengine.cpp)

The C++ implementation uses:
- **PyObject* functions** for module methods
- **QSqlQuery** for database access
- **PyDict** for returning structured data
- **Helper functions** for Qt-Python conversions
- **Error handling** with PyErr_SetString

### Python Side (generar_flujograma.py)

The Python script uses:
- **String concatenation** for HTML building
- **Dictionaries** for data organization
- **List comprehensions** for filtering
- **f-strings** for formatting
- **Escape functions** for HTML safety

## Performance Considerations

1. **Single Query Approach**: Each module method executes one SQL query
2. **In-Memory Processing**: All data loaded into Python structures
3. **String Building**: HTML built in memory as list of strings
4. **No External Dependencies**: Pure Python + MARRTA modules only

## Future Enhancements

Possible extensions to the API:

1. **Write Operations**:
   - `Sucesos.update(event_id, properties)`
   - `Defensas.set_active(defense_id, active)`
   
2. **Filtering**:
   - `Sucesos.get_by_risk(min_level)`
   - `Defensas.get_active_only()`
   
3. **Statistics**:
   - `Sucesos.get_statistics()`
   - `Defensas.get_coverage_stats()`
   
4. **Batch Operations**:
   - `Sucesos.bulk_update(updates)`
   - `Defensas.activate_all(event_id)`

## Testing

The implementation includes comprehensive test scripts:

- **test_python_sucesos_api.py**: Tests all Sucesos methods
- **test_python_defensas_api.py**: Tests all Defensas methods  
- **test_python_integration_complete.py**: Full integration test
- **generar_flujograma.py**: Real-world application example

## Documentation

Complete documentation available at:
- **API Reference**: `Documentation/Support/PYTHON_API_REFERENCE.md`
- **Test Scripts Guide**: `test_scripts/README.md`
- **Release Notes**: `RELEASE_INFO.md`

---

## Summary

The enhanced Python API provides:

✅ **Complete read access** to events and defenses  
✅ **Flexible filtering** by stage, type, and event  
✅ **Rich data structures** with all relevant properties  
✅ **Practical example** (flowchart generator)  
✅ **Comprehensive tests** for validation  
✅ **Full documentation** for reference  

This enables powerful scripting capabilities for:
- Report generation
- Data analysis
- Visualization
- Model validation
- Custom workflows
