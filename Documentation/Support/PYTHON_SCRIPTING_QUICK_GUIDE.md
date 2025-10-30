# MARRTA Python Scripting - Quick Reference Guide

## Getting Started

### Opening the Python Editor

1. Open MARRTA application
2. Load a model
3. Go to menu: **Modelo → Scripting → Python**
4. The Python editor window opens

### Editor Layout

```
┌─────────────────────────────────────────────────┐
│  [Ejecutar] [Documentación] [Guardar] [Cargar] │
│  ... [Borrar]                                   │
├─────────────────────────────────────────────────┤
│                                                 │
│  Python Code Editor                             │
│  (with syntax highlighting)                     │
│                                                 │
├─────────────────────────────────────────────────┤
│  Buscar/Reemplazar                              │
│  Buscar: [____] [Buscar]                        │
│  Reemplazar: [____] [Reemplazar] [Reemplazar Todo] │
│  □ Coincidir mayúsculas  □ Palabras completas   │
├─────────────────────────────────────────────────┤
│  Resultados de la ejecución                     │
│  (console output)                               │
│  [Copiar] ... [Limpiar]                         │
└─────────────────────────────────────────────────┘
```

## Basic API

### 1. Marrta Object

```python
# Get MARRTA version
version = Marrta.get_version()
print(version)
```

### 2. Database Object

```python
# Execute SQL query
results = Database.query("SELECT * FROM Etapa ORDER BY orden")

# Results is a list of dictionaries
for row in results:
    print(f"Stage: {row['nombre']}")
    print(f"Code: {row['codigo']}")
```

### 3. Etapas Object

```python
# Get all stages
etapas = Etapas.get_all()

# Access stage properties
for etapa in etapas:
    print(f"{etapa.codigo}: {etapa.nombre}")
    print(f"  ID: {etapa.id}")
    print(f"  Orden: {etapa.orden}")
    print(f"  Descripción: {etapa.descripcion}")
```

### 4. Etapa.imprimir() Method

**Returns a tuple**: `(ID, código, nombre, descripción, (substage_IDs))`

```python
etapas = Etapas.get_all()

for etapa in etapas:
    # Call imprimir() to get complete data
    datos = etapa.imprimir()
    
    # Unpack the tuple
    id_etapa, codigo, nombre, desc, subetapas = datos
    
    print(f"Stage {id_etapa}: {codigo} - {nombre}")
    print(f"  Description: {desc}")
    print(f"  Substages: {len(subetapas)}")
    
    if len(subetapas) > 0:
        print(f"  Substage IDs: {subetapas}")
```

## Common Patterns

### List All Stages

```python
etapas = Etapas.get_all()
print(f"Total stages: {len(etapas)}\n")

for etapa in etapas:
    print(f"[{etapa.codigo}] {etapa.nombre}")
```

### Query Events

```python
# Get all events
eventos = Database.query("SELECT * FROM Suceso ORDER BY nombre")
print(f"Total events: {len(eventos)}\n")

for evento in eventos:
    print(f"{evento['nombre']}")
    print(f"  Frequency: {evento['frecuencia']}")
    print(f"  Consequence: {evento['consecuencia']}")
```

### Filter by Risk Level

```python
# High-risk events (frequency >= 4, consequence >= 4)
sql = """
SELECT nombre, frecuencia, consecuencia
FROM Suceso
WHERE frecuencia >= 4 AND consecuencia >= 4
ORDER BY frecuencia DESC, consecuencia DESC
"""

high_risk = Database.query(sql)

print(f"High-risk events: {len(high_risk)}\n")
for event in high_risk:
    print(f"{event['nombre']}")
    print(f"  Risk: F={event['frecuencia']} x C={event['consecuencia']}")
```

### Analyze Stage Structure

```python
etapas = Etapas.get_all()

total_substages = 0
for etapa in etapas:
    datos = etapa.imprimir()
    id_etapa, codigo, nombre, desc, subetapas = datos
    
    num_subs = len(subetapas)
    total_substages += num_subs
    
    print(f"{codigo}: {num_subs} substages")

print(f"\nTotal: {total_substages} substages")
print(f"Average: {total_substages / len(etapas):.2f} per stage")
```

### Cross-Reference with Database

```python
etapas = Etapas.get_all()

for etapa in etapas:
    # Get substages from imprimir()
    datos = etapa.imprimir()
    id_etapa, codigo, nombre, desc, subetapas_ids = datos
    
    # Get substage details from database
    if len(subetapas_ids) > 0:
        ids_str = ",".join(str(id) for id in subetapas_ids)
        sql = f"""
        SELECT nombre 
        FROM Subetapa 
        WHERE id IN ({ids_str})
        ORDER BY orden
        """
        substages = Database.query(sql)
        
        print(f"\n{codigo} - {nombre}:")
        for sub in substages:
            print(f"  • {sub['nombre']}")
```

## Database Schema Reference

### Key Tables

**Etapa** (Stages)
- `id`: Stage ID
- `codigo`: Stage code
- `nombre`: Stage name
- `descripcion`: Description
- `orden`: Display order

**Subetapa** (Substages)
- `id`: Substage ID
- `idEtapa`: Parent stage ID
- `nombre`: Substage name
- `descripcion`: Description
- `orden`: Display order
- `activo`: Active flag

**Suceso** (Events)
- `id`: Event ID
- `nombre`: Event name
- `frecuencia`: Frequency (1-5)
- `consecuencia`: Consequence (1-5)

**Defensa** (Defenses)
- `id`: Defense ID
- `nombre`: Defense name
- `tipo`: Defense type
- `activo`: Active flag

## Tips and Tricks

### 1. Use f-strings for Formatting

```python
# Good
print(f"Stage {etapa.codigo}: {etapa.nombre}")

# Less readable
print("Stage " + etapa.codigo + ": " + etapa.nombre)
```

### 2. Check for Empty Results

```python
results = Database.query("SELECT * FROM Etapa")

if len(results) == 0:
    print("No results found")
else:
    for row in results:
        print(row['nombre'])
```

### 3. Use Tuple Unpacking

```python
# Clean
id_etapa, codigo, nombre, desc, subs = etapa.imprimir()

# Less clean
datos = etapa.imprimir()
id_etapa = datos[0]
codigo = datos[1]
# ...
```

### 4. Count with SQL

```python
# Efficient - count in database
result = Database.query("SELECT COUNT(*) as total FROM Suceso")
total = result[0]['total']

# Less efficient - count in Python
results = Database.query("SELECT * FROM Suceso")
total = len(results)
```

### 5. Use JOIN for Related Data

```python
sql = """
SELECT s.nombre as substage, e.nombre as stage
FROM Subetapa s
JOIN Etapa e ON s.idEtapa = e.id
ORDER BY e.orden, s.orden
"""
results = Database.query(sql)

for row in results:
    print(f"{row['stage']} → {row['substage']}")
```

## Find/Replace Features

### Basic Find
1. Type search term in "Buscar" field
2. Click "Buscar" button
3. Editor highlights next occurrence
4. Click "Buscar" again for next match

### Replace
1. Type search term in "Buscar" field
2. Type replacement in "Reemplazar" field
3. Click "Reemplazar" to replace current match
4. Click "Reemplazar Todo" to replace all matches

### Options
- **Coincidir mayúsculas/minúsculas**: Case-sensitive search
- **Palabras completas**: Match whole words only

## Save and Load Scripts

### Save
1. Click "Guardar" button
2. Choose location and filename (`.py` extension)
3. Script is saved to disk

### Load
1. Click "Cargar" button
2. Select a `.py` file
3. Script content loads into editor

## Documentation Window

Click "Documentación" button to open API reference:
- Tree navigation on the left
- Detailed documentation on the right
- Click examples to insert into editor
- Window stays open while you work

## Error Handling

```python
try:
    results = Database.query("SELECT * FROM InvalidTable")
except Exception as e:
    print(f"Error: {e}")
```

## Example: Complete Analysis

```python
print("=" * 60)
print("MARRTA Model Analysis")
print("=" * 60)

# Get version
print(f"\nVersion: {Marrta.get_version()}")

# Stage statistics
etapas = Etapas.get_all()
print(f"\nStages: {len(etapas)}")

total_subs = 0
for etapa in etapas:
    _, _, _, _, subs = etapa.imprimir()
    total_subs += len(subs)

print(f"Substages: {total_subs}")

# Event statistics
eventos = Database.query("SELECT COUNT(*) as c FROM Suceso")
print(f"Events: {eventos[0]['c']}")

# High-risk count
high_risk = Database.query("""
    SELECT COUNT(*) as c 
    FROM Suceso 
    WHERE frecuencia >= 4 AND consecuencia >= 4
""")
print(f"High-risk events: {high_risk[0]['c']}")

print("\n" + "=" * 60)
```

## Getting Help

- Click **Documentación** button in editor
- Check `test_scripts/` folder for examples
- See `Documentation/Support/PYTHON_SCRIPTING_IMPLEMENTATION.md` for details

## Test Scripts

Pre-made test scripts in `test_scripts/` folder:
1. `test_python_marrta_api.py` - Test Marrta object
2. `test_python_database_api.py` - Test Database queries
3. `test_python_etapas_api.py` - Test Etapas and imprimir()
4. `test_python_complete.py` - Complete integration test

Load these with **Cargar** button to see examples in action.
