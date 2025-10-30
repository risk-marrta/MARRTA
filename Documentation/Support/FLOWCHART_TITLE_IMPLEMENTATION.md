# Flowchart Title Implementation Summary

## Overview

This document describes the implementation of the feature to replace the hardcoded "Filtros y herramientas" text in the flowchart control panel with the actual model name from the database.

## Problem Statement

The flowchart generator script (`generar_flujograma.py`) generated HTML flowcharts with a control panel that displayed generic text "Filtros y herramientas" (Filters and tools) in the header. This didn't provide any context about which model was being visualized, causing potential confusion when working with multiple models.

## Solution

Replace the hardcoded text with the model name retrieved from the database's `configuracion` table, which is already fetched by the script at startup.

## Visual Changes

### Before
```
┌─────────────────────────────────┐
│ Filtros y herramientas       ▾ │  ← Generic text
├─────────────────────────────────┤
│ Filtrar por etapas              │
│ ...                             │
└─────────────────────────────────┘
```

### After
```
┌─────────────────────────────────┐
│ Modelo de Riesgo - Planta    ▾ │  ← Model name from DB
├─────────────────────────────────┤
│ Filtrar por etapas              │
│ ...                             │
└─────────────────────────────────┘
```

## Implementation Details

### Files Modified

1. **`test_scripts/generar_flujograma.py`** - The flowchart generator script

### Changes Made

#### 1. Template Placeholder (Line 688)
**Before:**
```html
<span>Filtros y herramientas</span>
```

**After:**
```html
<span>[[MODEL_NAME]]</span>
```

#### 2. Template Substitution (Line 1051)
**Before:**
```python
html = (template
        .replace("[[INITCOL]]", str(INIT_COL_CENTER))
        .replace("[[WIDTH]]", str(width))
        .replace("[[HEIGHT]]", str(height))
        .replace("[[EDGES]]", "".join(edges))
        .replace("[[NODES]]", "".join(nodes))
        .replace("[[HEADERS]]", "".join(headers))
        .replace("[[OPT_INITS]]", opt_inits)
        .replace("[[OPT_DEFS]]", opt_defs)
       )
```

**After:**
```python
html = (template
        .replace("[[INITCOL]]", str(INIT_COL_CENTER))
        .replace("[[WIDTH]]", str(width))
        .replace("[[HEIGHT]]", str(height))
        .replace("[[EDGES]]", "".join(edges))
        .replace("[[NODES]]", "".join(nodes))
        .replace("[[HEADERS]]", "".join(headers))
        .replace("[[OPT_INITS]]", opt_inits)
        .replace("[[OPT_DEFS]]", opt_defs)
        .replace("[[MODEL_NAME]]", esc(model_name))
       )
```

### Existing Infrastructure Used

The implementation leverages existing code that was already in place:

1. **Model Name Retrieval (Lines 83-91):**
```python
try:
    config_result = Database.query("SELECT nombre FROM configuracion LIMIT 1")
    if config_result and len(config_result) > 0 and 'nombre' in config_result[0]:
        model_name = config_result[0]['nombre'] or "modelo"
    else:
        model_name = "modelo"
except:
    model_name = "modelo"
```

2. **HTML Escaping Function (Line 45):**
```python
def esc(s):
    s = "" if s is None else str(s)
    return s.replace("&","&amp;").replace("<","&lt;").replace(">","&gt;")
```

## Security Considerations

- The model name is properly escaped using the `esc()` function before insertion into the HTML
- This prevents HTML injection attacks if the model name contains special characters like `<`, `>`, or `&`
- Example: A model name like `Test <Model> & Data` becomes `Test &lt;Model&gt; &amp; Data`

## Testing

### Test Cases Verified

1. **Normal model name**: "Modelo de Riesgo - Planta Industrial" displays correctly
2. **Special characters**: "Modelo <Test> & Análisis" is properly escaped
3. **Default fallback**: Empty or missing model name defaults to "modelo"
4. **Placeholder replacement**: The `[[MODEL_NAME]]` placeholder is completely replaced in the output

### Syntax Validation

```bash
$ python3 -m py_compile test_scripts/generar_flujograma.py
✓ Syntax is valid
```

## Benefits

1. **Clarity**: Users immediately know which model they are viewing
2. **Context**: No confusion when working with multiple models or comparing flowcharts
3. **Professional**: Personalized flowcharts with meaningful titles
4. **Zero extra work**: Model name is automatically fetched from the database (no manual input required)
5. **Backward compatible**: If model name is missing, defaults to "modelo"

## Usage

No changes to user workflow are required. The script works exactly as before:

```bash
# From MARRTA Python Editor
1. Load a model in MARRTA
2. Open Python Scripting Editor (Modelo → Scripting → Python)
3. Load test_scripts/generar_flujograma.py
4. Execute (F5 or Ejecutar button)
5. Open generated HTML file - the control panel will show the model name
```

## Database Schema

The implementation relies on the `configuracion` table:

```sql
CREATE TABLE configuracion (
    version INTEGER,
    nombre TEXT,        -- Model name used in flowchart title
    tecnica TEXT,
    analistas TEXT,
    institucion TEXT,
    fecha VARCHAR(10),
    descripcion TEXT,
    observaciones TEXT
);
```

## Related Files

- `test_scripts/generar_flujograma.py` - Main script (modified)
- `RELEASE_INFO.md` - Release notes (updated)
- `Documentation/Support/FLOWCHART_TITLE_IMPLEMENTATION.md` - This document

## Future Enhancements

Possible future improvements could include:
- Adding model date or version number alongside the name
- Making the title clickable to show full model details
- Adding a subtitle with the generation date
- Allowing custom title format in configuration

## References

- Issue: "Añade titulo al flujograma"
- Requirement: Replace "Filtros y herramientas" with model name from database
- Script: `test_scripts/generar_flujograma.py`
- Database table: `configuracion`
