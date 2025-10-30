# Python Flowchart Generator - Usage Examples

This document shows practical examples of using the enhanced Python API and flowchart generator.

## Quick Start

### 1. Open Python Editor
In MARRTA: `Modelo → Scripting → Python`

### 2. Load Flowchart Script
Click `[Cargar]` and select `test_scripts/generar_flujograma.py`

### 3. Execute
Click `[Ejecutar]`

### 4. Save Output
Copy the HTML from console output and save to a `.html` file.

## Example 1: Basic Flowchart Generation

### Input (generar_flujograma.py)
```python
CONFIG = {
    'title': 'Flujograma del Modelo de Riesgo',
    'show_defenses': True,
    'show_inactive': False,
    'color_by_risk': True,
}

# Script automatically:
# 1. Fetches all stages, events, defenses
# 2. Organizes data hierarchically
# 3. Generates HTML with styling
# 4. Outputs to console
```

### Output
```html
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Flujograma del Modelo de Riesgo</title>
    <style>
        /* CSS styling for flowchart... */
    </style>
</head>
<body>
    <div class="flowchart">
        <h1>Flujograma del Modelo de Riesgo</h1>
        
        <!-- Legend -->
        <div class="legend">
            <div class="legend-title">Niveles de Riesgo:</div>
            <div class="legend-item">
                <span class="legend-color" style="background-color: #A6A6A6;"></span>
                Bajo
            </div>
            <!-- More legend items... -->
        </div>
        
        <!-- Stage 1 -->
        <div class="stage">
            <div class="stage-header">
                <span class="stage-code">[E01]</span>
                Planificación
            </div>
            <div class="stage-content">
                <!-- Events in this stage... -->
            </div>
        </div>
        
        <!-- More stages... -->
    </div>
</body>
</html>
```

### Visual Result
When opened in browser, displays:
- Header with title
- Color-coded legend
- Each stage as expandable section
- Events within stages with risk colors
- Defenses listed under each event
- Summary statistics at bottom

## Example 2: Custom Event Analysis

### Script: analyze_high_risk_events.py
```python
"""
Analyze high-risk events and their defense coverage
"""

print("High-Risk Events Analysis")
print("=" * 60)

# Get all events
events = Sucesos.get_all()

# Filter high-risk events (risk >= 2)
high_risk_events = [e for e in events if e['riesgo'] >= 2]

print(f"Total Events: {len(events)}")
print(f"High-Risk Events: {len(high_risk_events)}")
print(f"Percentage: {len(high_risk_events)/len(events)*100:.1f}%")
print()

# Analyze each high-risk event
print("High-Risk Event Details:")
print("-" * 60)

for event in high_risk_events:
    print(f"\n[{event['codigo']}] {event['nombre']}")
    print(f"  Stage: {event['etapa_codigo']} - {event['etapa_nombre']}")
    print(f"  Risk: {event['riesgo']}, Freq: {event['frecuencia']}, Cons: {event['consecuencia']}")
    
    # Get defenses for this event
    defenses = Defensas.get_all(event['id'])
    print(f"  Defenses: {len(defenses)}")
    
    if len(defenses) > 0:
        # Count by type
        barriers = [d for d in defenses if d['tipo'] == 1]
        freq_red = [d for d in defenses if d['tipo'] == 0]
        cons_red = [d for d in defenses if d['tipo'] == 2]
        
        print(f"    Barriers: {len(barriers)}")
        print(f"    Frequency Reducers: {len(freq_red)}")
        print(f"    Consequence Reducers: {len(cons_red)}")
        
        # Count active defenses
        active = [d for d in defenses if d['activo']]
        print(f"    Active: {len(active)} / {len(defenses)}")
    else:
        print("    ⚠ WARNING: No defenses!")

print()
print("=" * 60)
print("Analysis Complete")
```

### Output
```
High-Risk Events Analysis
============================================================
Total Events: 45
High-Risk Events: 12
Percentage: 26.7%

High-Risk Event Details:
------------------------------------------------------------

[SI-03] Fallo en sistema de control
  Stage: E02 - Operación
  Risk: 3, Freq: 4, Cons: 5
  Defenses: 5
    Barriers: 2
    Frequency Reducers: 2
    Consequence Reducers: 1
    Active: 4 / 5

[SI-08] Error en procedimiento crítico
  Stage: E03 - Mantenimiento
  Risk: 2, Freq: 3, Cons: 4
  Defenses: 3
    Barriers: 1
    Frequency Reducers: 1
    Consequence Reducers: 1
    Active: 3 / 3

[SI-15] Pérdida de suministro eléctrico
  Stage: E04 - Servicios
  Risk: 2, Freq: 2, Cons: 5
  Defenses: 0
    ⚠ WARNING: No defenses!

============================================================
Analysis Complete
```

## Example 3: Defense Coverage Report

### Script: defense_coverage_report.py
```python
"""
Generate a comprehensive defense coverage report
"""

print("Defense Coverage Report")
print("=" * 70)
print()

# Get all data
stages = Etapas.get_all()
events = Sucesos.get_all()
all_defenses = Defensas.get_all()

# Analyze each stage
for stage in stages:
    print(f"Stage: [{stage.codigo}] {stage.nombre}")
    print("-" * 70)
    
    # Get events in this stage
    stage_events = Sucesos.get_by_stage(stage.id)
    
    if len(stage_events) == 0:
        print("  No events in this stage")
        print()
        continue
    
    # Count defenses per event
    events_with_defenses = 0
    total_defenses = 0
    
    for event in stage_events:
        defenses = Defensas.get_all(event['id'])
        defense_count = len(defenses)
        total_defenses += defense_count
        
        if defense_count > 0:
            events_with_defenses += 1
    
    # Calculate statistics
    coverage_pct = (events_with_defenses / len(stage_events)) * 100
    avg_defenses = total_defenses / len(stage_events)
    
    print(f"  Events: {len(stage_events)}")
    print(f"  Events with defenses: {events_with_defenses}")
    print(f"  Coverage: {coverage_pct:.1f}%")
    print(f"  Total defenses: {total_defenses}")
    print(f"  Avg defenses per event: {avg_defenses:.2f}")
    
    # Risk distribution
    high_risk = sum(1 for e in stage_events if e['riesgo'] >= 2)
    if high_risk > 0:
        print(f"  High-risk events: {high_risk}")
    
    print()

# Overall summary
print("=" * 70)
print("Overall Summary")
print("-" * 70)

total_events = len(events)
events_with_defenses = sum(1 for e in events if len(Defensas.get_all(e['id'])) > 0)
overall_coverage = (events_with_defenses / total_events) * 100

print(f"Total Events: {total_events}")
print(f"Events with Defenses: {events_with_defenses}")
print(f"Overall Coverage: {overall_coverage:.1f}%")
print(f"Total Defenses: {len(all_defenses)}")
print(f"Avg Defenses per Event: {len(all_defenses)/total_events:.2f}")

# Defense type distribution
barriers = [d for d in all_defenses if d['tipo'] == 1]
freq_reducers = [d for d in all_defenses if d['tipo'] == 0]
cons_reducers = [d for d in all_defenses if d['tipo'] == 2]

print()
print("Defense Type Distribution:")
print(f"  Barriers: {len(barriers)} ({len(barriers)/len(all_defenses)*100:.1f}%)")
print(f"  Frequency Reducers: {len(freq_reducers)} ({len(freq_reducers)/len(all_defenses)*100:.1f}%)")
print(f"  Consequence Reducers: {len(cons_reducers)} ({len(cons_reducers)/len(all_defenses)*100:.1f}%)")

print()
print("=" * 70)
```

### Output
```
Defense Coverage Report
======================================================================

Stage: [E01] Identificación
----------------------------------------------------------------------
  Events: 8
  Events with defenses: 7
  Coverage: 87.5%
  Total defenses: 15
  Avg defenses per event: 1.88

Stage: [E02] Análisis
----------------------------------------------------------------------
  Events: 12
  Events with defenses: 10
  Coverage: 83.3%
  Total defenses: 24
  Avg defenses per event: 2.00
  High-risk events: 3

Stage: [E03] Evaluación
----------------------------------------------------------------------
  Events: 10
  Events with defenses: 8
  Coverage: 80.0%
  Total defenses: 18
  Avg defenses per event: 1.80
  High-risk events: 2

======================================================================
Overall Summary
----------------------------------------------------------------------
Total Events: 45
Events with Defenses: 38
Overall Coverage: 84.4%
Total Defenses: 89
Avg Defenses per Event: 1.98

Defense Type Distribution:
  Barriers: 42 (47.2%)
  Frequency Reducers: 28 (31.5%)
  Consequence Reducers: 19 (21.3%)

======================================================================
```

## Example 4: Custom HTML Flowchart

### Script: custom_flowchart_minimal.py
```python
"""
Minimal flowchart showing only essential information
"""

print("Generating Minimal Flowchart...")

# Get data
stages = Etapas.get_all()
events = Sucesos.get_all()

# Start HTML
html = """<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Minimal Flowchart</title>
    <style>
        body { font-family: Arial; margin: 20px; }
        .stage { margin: 20px 0; border: 2px solid #333; padding: 10px; }
        .stage-name { font-weight: bold; font-size: 18px; margin-bottom: 10px; }
        .event { margin: 5px 0; padding: 5px; background: #f0f0f0; }
        .high-risk { background: #ffcccc; }
    </style>
</head>
<body>
    <h1>Risk Model Flowchart</h1>
"""

# Add each stage
for stage in stages:
    html += f'<div class="stage">'
    html += f'<div class="stage-name">{stage.codigo} - {stage.nombre}</div>'
    
    # Get events for this stage
    stage_events = [e for e in events if e['etapa_id'] == stage.id]
    
    for event in stage_events:
        risk_class = "high-risk" if event['riesgo'] >= 2 else ""
        html += f'<div class="event {risk_class}">'
        html += f'{event["codigo"]}: {event["nombre"]} '
        html += f'(R:{event["riesgo"]}, F:{event["frecuencia"]}, C:{event["consecuencia"]})'
        html += '</div>'
    
    html += '</div>'

# Close HTML
html += """
</body>
</html>
"""

print(html)
print()
print("Flowchart generated!")
```

## Example 5: Database Integration

### Script: complex_analysis.py
```python
"""
Complex analysis combining all APIs
"""

print("Complex Model Analysis")
print("=" * 70)
print()

# 1. Get basic counts using different methods
print("1. Data Verification:")
print("-" * 70)

# Method 1: Using API modules
stages_api = Etapas.get_all()
events_api = Sucesos.get_all()
defenses_api = Defensas.get_all()

print(f"Via API:")
print(f"  Stages: {len(stages_api)}")
print(f"  Events: {len(events_api)}")
print(f"  Defenses: {len(defenses_api)}")

# Method 2: Using direct SQL
stages_sql = Database.query("SELECT COUNT(*) as count FROM etapas")
events_sql = Database.query("SELECT COUNT(*) as count FROM suceso_iniciador")
defenses_sql = Database.query("SELECT COUNT(*) as count FROM defensa")

print(f"Via SQL:")
print(f"  Stages: {stages_sql[0]['count']}")
print(f"  Events: {events_sql[0]['count']}")
print(f"  Defenses: {defenses_sql[0]['count']}")

# 2. Complex SQL query
print()
print("2. Events per Stage (SQL):")
print("-" * 70)

sql = """
SELECT 
    e.codigo as stage_code,
    e.nombre as stage_name,
    COUNT(s.id) as event_count,
    AVG(s.riesgo) as avg_risk
FROM etapas e
LEFT JOIN suceso_iniciador s ON e.id = s.etapa_id
GROUP BY e.id
ORDER BY e.orden
"""

results = Database.query(sql)
for row in results:
    avg_risk = row['avg_risk'] if row['avg_risk'] is not None else 0
    print(f"  {row['stage_code']}: {row['event_count']} events, "
          f"avg risk: {avg_risk:.2f}")

# 3. Defense coverage by stage
print()
print("3. Defense Coverage by Stage:")
print("-" * 70)

for stage in stages_api:
    stage_events = Sucesos.get_by_stage(stage.id)
    
    total_defenses = 0
    for event in stage_events:
        defenses = Defensas.get_all(event['id'])
        total_defenses += len(defenses)
    
    if len(stage_events) > 0:
        avg = total_defenses / len(stage_events)
        print(f"  {stage.codigo}: {total_defenses} defenses for "
              f"{len(stage_events)} events (avg: {avg:.2f})")

# 4. High-risk events with no defenses (GAPS!)
print()
print("4. Risk Gaps (High-risk events without defenses):")
print("-" * 70)

gaps_found = False
for event in events_api:
    if event['riesgo'] >= 2:  # High or Very High risk
        defenses = Defensas.get_all(event['id'])
        if len(defenses) == 0:
            gaps_found = True
            print(f"  ⚠ {event['codigo']}: {event['nombre']}")
            print(f"     Risk: {event['riesgo']}, Stage: {event['etapa_codigo']}")

if not gaps_found:
    print("  ✓ No gaps found - all high-risk events have defenses")

print()
print("=" * 70)
print("Analysis Complete")
```

## Tips for Creating Custom Scripts

### 1. Start Simple
```python
# Get data
events = Sucesos.get_all()
print(f"Total events: {len(events)}")
```

### 2. Add Filtering
```python
# Filter by criteria
high_risk = [e for e in events if e['riesgo'] >= 2]
active_only = [e for e in events if e['activo']]
```

### 3. Combine Data Sources
```python
# Cross-reference events and defenses
for event in events:
    defenses = Defensas.get_all(event['id'])
    print(f"{event['codigo']}: {len(defenses)} defenses")
```

### 4. Use SQL for Complex Queries
```python
# When you need joins or aggregations
sql = """
SELECT s.codigo, COUNT(d.id) as defense_count
FROM suceso_iniciador s
LEFT JOIN defensa d ON s.id = d.suceso_iniciador_id
GROUP BY s.id
HAVING defense_count < 2
"""
results = Database.query(sql)
```

### 5. Generate Outputs
```python
# HTML, CSV, or just formatted text
output = ""
for event in events:
    output += f"{event['codigo']},{event['nombre']},{event['riesgo']}\n"
print(output)
```

## Common Patterns

### Pattern 1: Iterate All Events
```python
events = Sucesos.get_all()
for event in events:
    # Process each event
    pass
```

### Pattern 2: Events by Stage
```python
stages = Etapas.get_all()
for stage in stages:
    events = Sucesos.get_by_stage(stage.id)
    # Process stage's events
```

### Pattern 3: Defenses per Event
```python
events = Sucesos.get_all()
for event in events:
    defenses = Defensas.get_all(event['id'])
    # Process event's defenses
```

### Pattern 4: Filter and Count
```python
events = Sucesos.get_all()
high_risk_count = sum(1 for e in events if e['riesgo'] >= 2)
```

### Pattern 5: Group by Property
```python
events = Sucesos.get_all()
by_stage = {}
for event in events:
    stage_id = event['etapa_id']
    if stage_id not in by_stage:
        by_stage[stage_id] = []
    by_stage[stage_id].append(event)
```

## Troubleshooting

### Issue: No data returned
```python
# Check if model is loaded
events = Sucesos.get_all()
if len(events) == 0:
    print("No events found. Is a model loaded?")
```

### Issue: KeyError in dictionary
```python
# Check for None values
event = events[0]
if 'subetapa_nombre' in event:
    print(event['subetapa_nombre'])
else:
    print("No substage name")
```

### Issue: SQL errors
```python
# Use try-except
try:
    results = Database.query(sql)
except Exception as e:
    print(f"SQL Error: {e}")
```

---

## Next Steps

1. **Try the Examples**: Run each example script to understand the output
2. **Modify CONFIG**: Change flowchart generation parameters
3. **Create Custom Scripts**: Use patterns to build your own analysis tools
4. **Combine Techniques**: Mix API calls with SQL for powerful queries
5. **Share Results**: Export HTML/reports for stakeholders

## Further Reading

- **Complete API Reference**: `Documentation/Support/PYTHON_API_REFERENCE.md`
- **Implementation Details**: `Documentation/Support/PYTHON_API_FLOWCHART_IMPLEMENTATION.md`
- **Test Scripts**: `test_scripts/test_python_*.py`
