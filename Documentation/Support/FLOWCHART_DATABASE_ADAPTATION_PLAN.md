# Flowchart Database Adaptation Plan

**STATUS: ✅ COMPLETED**

## Overview
Adapt `generar_flujograma_v25_fix.py` (1400+ lines) to read from MARRTA's loaded database instead of Excel files.

**Implementation:** `test_scripts/generar_flujograma_v25_desde_bd.py` (941 lines, 46KB)

## Original Script Analysis

**File:** generar_flujograma_v25_fix.py  
**Size:** 50KB, ~1400 lines  
**Purpose:** Generate interactive HTML flowchart from Excel model

### Key Components:

1. **Data Loading** (lines 1-150)
   - Reads 6 Excel sheets: Etapas, Subetapas, Sucesos, Barreras, RF, RC
   - Normalizes column names and data types
   - Creates mappings (code2id, name2id)

2. **Data Aggregation** (lines 151-300)
   - Aggregates defenses by (EtapaId, Codigo)
   - Parses target initiators from defense definitions
   - Handles robustness levels (0-3)

3. **Layout Calculation** (lines 301-600)
   - Computes node positions and heights
   - Handles vertical stacking of stages
   - Manages columns: fictitious left, barriers, real stages, RF/RC right

4. **SVG Generation** (lines 601-900)
   - Renders nodes (stages, substages, initiators, defenses)
   - Creates connecting arrows
   - Stores position data for interaction

5. **HTML Template** (lines 901-1400)
   - Embedded CSS for styling
   - Interactive JavaScript for:
     - Filtering (text, stage, type, robustness)
     - Selection/highlighting
     - Zoom/pan with wheel and drag
     - SVG/PNG export

## Database Adaptation Strategy

### Phase 1: Data Loading Replacement

Replace pandas Excel reading with MARRTA Python API:

```python
# Original (Excel):
etapas = pd.read_excel(excel, sheet_name="Etapas")
subetapas = pd.read_excel(excel, sheet_name="Subetapas")
sucesos = pd.read_excel(excel, sheet_name="Sucesos")
# ...

# Adapted (Database):
# Get stages from Etapas API
raw_stages = Etapas.get_all()
etapas = pandas_from_dict_list(raw_stages)

# Get substages from Etapas API  
raw_substages = []
for stage in raw_stages:
    subs = stage.get('subetapas', [])
    raw_substages.extend(subs)
subetapas = pandas_from_dict_list(raw_substages)

# Get events from Sucesos API
raw_events = Sucesos.get_all()
sucesos = pandas_from_dict_list(raw_events)

# Get defenses from Defensas API
all_defenses = Defensas.get_all()
# Separate by type: 0=RF, 1=Barrier, 2=RC
barreras = pandas_from_dict_list([d for d in all_defenses if d.get('tipo')==1])
rf = pandas_from_dict_list([d for d in all_defenses if d.get('tipo')==0])
rc = pandas_from_dict_list([d for d in all_defenses if d.get('tipo')==2])
```

### Phase 2: Column Mapping

Original script uses flexible column name matching. Database has fixed schema:

**Etapas/Stages:**
- Id → id
- Nombre → nombre
- Codigo → codigo

**Subetapas/Substages:**
- Id → id
- Nombre → nombre
- Etapa → etapa_id

**Sucesos/Events:**
- Id → id
- Nombre → nombre
- Codigo → codigo
- Etapa → etapa_id
- Subetapa → subetapa_id

**Defensas/Defenses:**
- Id → id
- Nombre → nombre  
- Codigo → codigo
- Orden → (stage code - need to map)
- Robustez → robustez
- Iniciadores/Sucesos → suceso_id (single, not list)

### Phase 3: Data Structure Adaptation

**Challenge:** Original expects defense targets as semicolon-separated list.  
**Solution:** Database stores one defense-event relationship per row. Need to aggregate.

```python
# Group defenses by (stage, code) and collect target event IDs
def aggregate_defenses_from_db(defenses_list):
    grouped = {}
    for d in defenses_list:
        key = (d['etapa_id'], d['codigo'])
        if key not in grouped:
            grouped[key] = {
                '_EtapaId': d['etapa_id'],
                '_Codigo': d['codigo'],
                '_Nombre': d['nombre'],
                '_Rob': d['robustez'],
                '_Targets': set()
            }
        if d.get('suceso_id'):
            grouped[key]['_Targets'].add(d['suceso_id'])
    return list(grouped.values())
```

### Phase 4: Keep Original Logic

**Important:** Keep ALL layout calculation, SVG generation, and HTML template logic UNCHANGED. Only replace data loading.

## Implementation Steps

1. **Create helper functions** for converting API data to pandas DataFrames
2. **Replace data loading section** (lines 1-150)
3. **Test with small model** to verify data structure compatibility
4. **Validate output** HTML matches original format
5. **Test interactive features** (filtering, selection, zoom, export)

## Testing Plan

1. Load a test model in MARRTA
2. Run original script with exported Excel
3. Run adapted script from Python editor
4. Compare HTML outputs (should be identical)
5. Test all interactive features in browser

## Compatibility Notes

- Script must run inside MARRTA's Python engine (has access to Etapas, Sucesos, Defensas)
- Cannot use external file I/O for database
- Output HTML should be printed to console for user to save
- All pandas operations should work (pandas is available in MARRTA Python)

## Deliverables

1. `generar_flujograma_v25_desde_bd.py` - Full adapted script
2. Test report showing identical output to original
3. Usage documentation for MARRTA users
