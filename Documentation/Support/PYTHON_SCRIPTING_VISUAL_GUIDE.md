# MARRTA Python Scripting - Visual Guide

## Menu Access

```
┌────────────────────────────────────────┐
│  MARRTA Main Window                    │
├────────────────────────────────────────┤
│  Menú:                                 │
│    Prácticas                           │
│    Modelo ▼                            │
│    │  Más información                  │
│    │  ──────────────                   │
│    │  Resumen                          │
│    │  Informe                          │
│    │  ──────────────                   │
│    │  Generar Informe HTML             │
│    │  ──────────────                   │
│    │  Guardar Modelo                   │
│    │  ──────────────                   │
│    │  Marcar Defensas                  │
│    │  Desmarcar Defensas               │
│    Administración                      │
│    │  Etapas                           │
│    │  Sucesos iniciadores              │
│    │  Defensas                         │
│    │  ──────────────                   │
│    │  Importar                         │
│    │    Excel...                       │
│    │  ──────────────                   │
│    │  Scripting ▼                      │
│    │    Editor JS          ← Existing  │
│    │    Python             ← NEW!      │
│    Ayuda                               │
└────────────────────────────────────────┘
```

## Python Editor Window

```
┌──────────────────────────────────────────────────────────────┐
│  Editor de scripts Python                               [X]   │
├──────────────────────────────────────────────────────────────┤
│  ┌────────────────────────────────────────────────────────┐  │
│  │  # Python Code Editor (with syntax highlighting)      │  │
│  │  etapas = Etapas.get_all()                            │  │
│  │  print(f"Total stages: {len(etapas)}\n")              │  │
│  │                                                        │  │
│  │  for etapa in etapas:                                 │  │
│  │      datos = etapa.imprimir()                         │  │
│  │      id_etapa, codigo, nombre, desc, subs = datos     │  │
│  │      print(f"{codigo}: {nombre} ({len(subs)} subs)")  │  │
│  │                                                        │  │
│  └────────────────────────────────────────────────────────┘  │
│  [Ejecutar] [Documentación] [Guardar] [Cargar] ... [Borrar] │
│                                                               │
│  ┌─ Buscar/Reemplazar ───────────────────────────────────┐   │
│  │  Buscar:      [________________] [Buscar]             │   │
│  │  Reemplazar:  [________________] [Reemplazar] [Todo]  │   │
│  │  ☐ Coincidir mayúsculas/minúsculas                    │   │
│  │  ☐ Palabras completas                                 │   │
│  └────────────────────────────────────────────────────────┘   │
│                                                               │
│  Resultados de la ejecución                                  │
│  ┌────────────────────────────────────────────────────────┐  │
│  │  Total stages: 5                                       │  │
│  │                                                        │  │
│  │  E01: Etapa 1 (3 subs)                                │  │
│  │  E02: Etapa 2 (2 subs)                                │  │
│  │  E03: Etapa 3 (4 subs)                                │  │
│  │  E04: Etapa 4 (1 subs)                                │  │
│  │  E05: Etapa 5 (0 subs)                                │  │
│  │                                                        │  │
│  └────────────────────────────────────────────────────────┘  │
│  [Copiar] .......................................... [Limpiar] │
└──────────────────────────────────────────────────────────────┘
```

## Documentation Window

```
┌──────────────────────────────────────────────────────────────┐
│  Documentación API Python                               [X]   │
├──────────────────────────────────────────────────────────────┤
│  ┌─ API ──────┐ ┌─ Documentation ────────────────────────┐   │
│  │ ▼ Marrta   │ │ Objeto Marrta                         │   │
│  │   get_vers │ │                                        │   │
│  │            │ │ El objeto Marrta proporciona acceso   │   │
│  │ ▼ Database │ │ a las funcionalidades principales de  │   │
│  │   query()  │ │ la aplicación.                        │   │
│  │            │ │                                        │   │
│  │ ▼ Etapas   │ │ Métodos disponibles:                  │   │
│  │   get_all()│ │ • get_version() - Obtiene versión     │   │
│  │   ▼ Etapa  │ │                                        │   │
│  │     imprm  │ │ Ejemplo:                              │   │
│  │            │ │   version = Marrta.get_version()      │   │
│  │ ▼ Ejemplos │ │   print(f"Version: {version}")        │   │
│  │   Listar   │ │                                        │   │
│  │   Imprim   │ │                                        │   │
│  │   SQL      │ │                                        │   │
│  └────────────┘ └───────────────────────────────────────┘   │
│                                                               │
│  ....................................... [Insertar ejemplo]   │
└──────────────────────────────────────────────────────────────┘
```

## Syntax Highlighting Examples

### Keywords and Control Flow
```python
# Blue, bold
if condition:
    for item in items:
        while running:
            def function_name():
                return result
```

### Built-in Functions
```python
# Yellow
print("Hello")
len(list_data)
range(10)
type(variable)
```

### Strings
```python
# Orange-brown
"double quoted string"
'single quoted string'
"""triple quoted
   multiline string"""
```

### Comments
```python
# Green, italic
# This is a comment
```

### Numbers and Operators
```python
# Numbers: light green
# Operators: light grey
value = 42 + 3.14
result = data * 2
```

## API Objects Hierarchy

```
Marrta
  └─ get_version() → String

Database
  └─ query(sql) → List[Dict]

Etapas
  └─ get_all() → List[Etapa]
      │
      Etapa (object)
        ├─ id (int)
        ├─ codigo (string)
        ├─ nombre (string)
        ├─ descripcion (string)
        ├─ orden (int)
        └─ imprimir() → Tuple(int, str, str, str, Tuple[int])
```

## Etapa.imprimir() Return Value

```
         ┌─────────────────────────────────────────────────────┐
         │  Tuple with 5 elements                              │
         └─────────────────────────────────────────────────────┘
                 │
      ┌──────────┴────────┬──────────┬──────────┬──────────┐
      │                   │          │          │          │
      ▼                   ▼          ▼          ▼          ▼
   Index 0             Index 1    Index 2    Index 3    Index 4
   ───────             ───────    ───────    ───────    ───────
   ID (int)            código     nombre     descrip    (substage
                       (string)   (string)   (string)    IDs)
                                                         (tuple)
   Example:
   12         "E01"     "Stage 1"  "Desc..."  (23, 45, 67)
```

## Example: Using imprimir()

```python
# Step 1: Get all stages
etapas = Etapas.get_all()

# Step 2: Loop through stages
for etapa in etapas:
    
    # Step 3: Call imprimir()
    datos = etapa.imprimir()
    
    # Step 4: Unpack the tuple
    id_etapa, codigo, nombre, desc, subetapas = datos
    #   ↓        ↓       ↓       ↓        ↓
    #  int     string  string  string   tuple
    
    # Step 5: Use the data
    print(f"Stage {id_etapa}: {codigo} - {nombre}")
    print(f"  Description: {desc}")
    print(f"  Has {len(subetapas)} substages")
    
    if len(subetapas) > 0:
        print(f"  Substage IDs: {subetapas}")
```

## Database Query Flow

```
Python Code                    Database                 Result
─────────────                  ────────                 ──────

sql = "SELECT * FROM Etapa"
         │
         ├───────────────────────────►
         │                        Query executed
         │                             │
         │                        Rows retrieved
         │                             │
         │◄────────────────────────────┘
         │
results = Database.query(sql)
         │
         └────► List[Dict]
                 │
                 ├─► Dict: {id: 1, codigo: "E01", ...}
                 ├─► Dict: {id: 2, codigo: "E02", ...}
                 └─► Dict: {id: 3, codigo: "E03", ...}
```

## Comparison: JavaScript vs Python

```
┌─────────────────────┬──────────────────┬──────────────────┐
│ Feature             │ JavaScript       │ Python           │
├─────────────────────┼──────────────────┼──────────────────┤
│ Access from menu    │ Editor JS        │ Python           │
├─────────────────────┼──────────────────┼──────────────────┤
│ Syntax highlight    │ Yes              │ Yes              │
├─────────────────────┼──────────────────┼──────────────────┤
│ Find/Replace        │ No               │ Yes ✓            │
├─────────────────────┼──────────────────┼──────────────────┤
│ Documentation       │ Yes              │ Yes              │
├─────────────────────┼──────────────────┼──────────────────┤
│ Marrta object       │ Marrta.xxx()     │ Marrta.xxx()     │
├─────────────────────┼──────────────────┼──────────────────┤
│ Direct DB access    │ No               │ Yes ✓            │
│                     │                  │ Database.query() │
├─────────────────────┼──────────────────┼──────────────────┤
│ Stages access       │ Marrta.getAll    │ Etapas.get_all() │
│                     │ Stages()         │                  │
├─────────────────────┼──────────────────┼──────────────────┤
│ Etapa.imprimir()    │ No               │ Yes ✓            │
├─────────────────────┼──────────────────┼──────────────────┤
│ Events API          │ Events.xxx()     │ Not yet          │
├─────────────────────┼──────────────────┼──────────────────┤
│ Defenses API        │ Defenses.xxx()   │ Not yet          │
├─────────────────────┼──────────────────┼──────────────────┤
│ Charts API          │ Charts.xxx()     │ Not yet          │
└─────────────────────┴──────────────────┴──────────────────┘
```

## Common Workflows

### Workflow 1: List All Stages

```
1. Open Python editor
   (Modelo → Scripting → Python)
         │
         ▼
2. Write code:
   etapas = Etapas.get_all()
   for etapa in etapas:
       print(etapa.nombre)
         │
         ▼
3. Click [Ejecutar]
         │
         ▼
4. See results in console
   Stage 1
   Stage 2
   Stage 3
   ...
```

### Workflow 2: Analyze Stage Structure

```
1. Open Python editor
         │
         ▼
2. Load test script
   Click [Cargar]
   Select: test_python_etapas_api.py
         │
         ▼
3. Click [Ejecutar]
         │
         ▼
4. Review detailed output
   - Stage properties
   - Substage counts
   - imprimir() results
         │
         ▼
5. Modify and re-run
```

### Workflow 3: Custom SQL Analysis

```
1. Open Python editor
         │
         ▼
2. Write SQL query:
   sql = """
   SELECT nombre, frecuencia
   FROM Suceso
   WHERE frecuencia >= 4
   """
   results = Database.query(sql)
         │
         ▼
3. Process results:
   for row in results:
       print(row['nombre'])
         │
         ▼
4. Execute and analyze
```

## Keyboard Shortcuts (Standard Qt)

```
Ctrl+A    Select All
Ctrl+C    Copy
Ctrl+V    Paste
Ctrl+X    Cut
Ctrl+Z    Undo
Ctrl+Y    Redo
Ctrl+F    Focus Find field
```

## File Operations

### Save Script
```
[Guardar] → Choose location → filename.py → Save
```

### Load Script
```
[Cargar] → Browse to file → Select .py file → Load
```

### File Format
```python
# Standard Python file (.py)
# Can include:
# - Comments (lines starting with #)
# - Python code
# - API calls (Marrta, Database, Etapas)

# Example:
etapas = Etapas.get_all()
for etapa in etapas:
    datos = etapa.imprimir()
    print(datos)
```

## Error Handling Example

```python
# Good practice: use try-except

try:
    results = Database.query("SELECT * FROM Etapa")
    
    if len(results) == 0:
        print("No stages found")
    else:
        for row in results:
            print(row['nombre'])
            
except Exception as e:
    print(f"Error occurred: {e}")
```

## Tips for Success

1. **Start with Documentation**
   - Click [Documentación] to explore API
   - Try the examples provided

2. **Use Test Scripts**
   - Load pre-made test scripts from test_scripts/
   - Understand patterns before writing your own

3. **Incremental Development**
   - Write small pieces of code
   - Test frequently with [Ejecutar]
   - Build up complex scripts gradually

4. **Check Results**
   - Always verify query results
   - Use print() liberally
   - Check for empty results

5. **Save Your Work**
   - Use [Guardar] to save useful scripts
   - Build a library of analysis scripts
   - Share scripts with team

## Next Steps

After mastering the basics:

1. Explore all test scripts
2. Create custom analysis scripts
3. Combine Database queries with Etapas API
4. Build automated reports
5. Share findings with team

## Support Resources

- **In-app Documentation**: Click [Documentación] button
- **Implementation Guide**: `Documentation/Support/PYTHON_SCRIPTING_IMPLEMENTATION.md`
- **Quick Reference**: `Documentation/Support/PYTHON_SCRIPTING_QUICK_GUIDE.md`
- **Test Scripts**: `test_scripts/test_python_*.py`
