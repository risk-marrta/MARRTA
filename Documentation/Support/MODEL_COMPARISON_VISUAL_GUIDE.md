# Model Comparison Feature - Visual Guide

## Feature Location

The Model Comparison feature is accessible from the main menu:

```
┌─────────────────────────────────────────────────────────────────┐
│ MARRTA                                                    [_][□][X]│
├─────────────────────────────────────────────────────────────────┤
│ Prácticas | Modelo | Administración | Ayuda                     │
│                      │                                           │
│                      ├─ Etapas                                   │
│                      ├─ Sucesos iniciadores                      │
│                      ├─ Defensas                                 │
│                      ├─────────────────                          │
│                      └─► Comparador de modelos  ← NEW FEATURE   │
│                                                                   │
└───────────────────────────────────────────────────────────────────┘
```

## Dialog Interface

### Initial State

```
┌─────────────────────────────────────────────────────────────────────┐
│ Comparador de Modelos                                          [X]  │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│ ╔═══════════════════════════════════════════════════════════════╗ │
│ ║ Modelos a Comparar                                            ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║                                                               ║ │
│ ║  Modelo Actual:                                               ║ │
│ ║  ┌──────────────────────────────────────────────────────────┐ ║ │
│ ║  │ Mi Modelo v1.0 (/home/user/models/modelo.pdb)           │ ║ │
│ ║  └──────────────────────────────────────────────────────────┘ ║ │
│ ║                                                               ║ │
│ ║  Modelo a Comparar:                                           ║ │
│ ║  ┌──────────────────────────────────────────────┐            ║ │
│ ║  │ Seleccione un modelo para comparar...       │ [Seleccionar...]║ │
│ ║  └──────────────────────────────────────────────┘            ║ │
│ ║                                                               ║ │
│ ║               [Comparar Modelos] (disabled)                   ║ │
│ ║                                                               ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
│                                                                     │
│ ╔═══════════════════════════════════════════════════════════════╗ │
│ ║ [Resumen] [Etapas] [Iniciadores] [Defensas] [Asociac] [Gráf] ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║                                                               ║ │
│ ║  Comparación de Modelos                                       ║ │
│ ║                                                               ║ │
│ ║  Seleccione un modelo para comparar con el modelo actual.     ║ │
│ ║                                                               ║ │
│ ║                                                               ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
│                                                                     │
│  [Exportar Reporte] (disabled)                          [Cerrar]   │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### After Comparison - Resumen Tab

```
┌─────────────────────────────────────────────────────────────────────┐
│ Comparador de Modelos                                          [X]  │
├─────────────────────────────────────────────────────────────────────┤
│ ╔═══════════════════════════════════════════════════════════════╗ │
│ ║ Modelos a Comparar                                            ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║  Modelo Actual: Mi Modelo v1.0 (/home/user/modelo.pdb)       ║ │
│ ║  Modelo a Comparar: Mi Modelo v2.0 (/home/user/modelo2.pdb) [S]║ │
│ ║                      [Comparar Modelos]                       ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
│                                                                     │
│ ╔═══════════════════════════════════════════════════════════════╗ │
│ ║►[Resumen] [Etapas] [Iniciadores] [Defensas] [Asociac] [Gráf] ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║  Comparación de Modelos de Riesgo                             ║ │
│ ║                                                               ║ │
│ ║  Modelo Actual: Mi Modelo v1.0                                ║ │
│ ║  Modelo Comparación: Mi Modelo v2.0                           ║ │
│ ║  Fecha de comparación: 24/10/2025 04:15:00                    ║ │
│ ║                                                               ║ │
│ ║  Resumen de Etapas y Subetapas                                ║ │
│ ║  Total de etapas: 8                                           ║ │
│ ║  Etapas idénticas: 7                                          ║ │
│ ║  Etapas con diferencias: 1                                    ║ │
│ ║                                                               ║ │
│ ║  Resumen de Iniciadores                                       ║ │
│ ║  Total de iniciadores: 45                                     ║ │
│ ║  Iniciadores sin cambios: 38                                  ║ │
│ ║  Iniciadores con diferencias: 7                               ║ │
│ ║                                                               ║ │
│ ║  Resumen de Defensas                                          ║ │
│ ║  Total de defensas: 52                                        ║ │
│ ║  Defensas sin cambios: 49                                     ║ │
│ ║  Defensas con diferencias: 3                                  ║ │
│ ║                                                               ║ │
│ ║  Resumen de Asociaciones                                      ║ │
│ ║  Total de asociaciones: 180                                   ║ │
│ ║  Asociaciones sin cambios: 175                                ║ │
│ ║  Asociaciones con diferencias: 5                              ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
│                                                                     │
│  [Exportar Reporte]                                     [Cerrar]   │
└─────────────────────────────────────────────────────────────────────┘
```

### Etapas Tab - Stage Comparison

```
┌─────────────────────────────────────────────────────────────────────┐
│ ║ [Resumen]►[Etapas] [Iniciadores] [Defensas] [Asociac] [Gráf] ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║ ┌───────────────────────────────────────────────────────────┐ ║ │
│ ║ │Etapa            │Subetapas│Subetapas│Estado              │ ║ │
│ ║ │                 │(Actual) │(Comp.)  │                    │ ║ │
│ ║ ├─────────────────┼─────────┼─────────┼────────────────────┤ ║ │
│ ║ │Operación Normal │    3    │    3    │Igual          [==] │ ║ │ Green
│ ║ │Mantenimiento    │    2    │    3    │Diferente      [≠]  │ ║ │ Yellow
│ ║ │Carga Combustible│    4    │    4    │Igual          [==] │ ║ │ Green
│ ║ │Parada           │    2    │    2    │Igual          [==] │ ║ │ Green
│ ║ │Nueva Etapa      │    0    │    1    │Solo en comp.  [+]  │ ║ │ Red
│ ║ │Etapa Antigua    │    1    │    0    │Solo en actual [-]  │ ║ │ Blue
│ ║ └───────────────────────────────────────────────────────────┘ ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
└─────────────────────────────────────────────────────────────────────┘
```

### Iniciadores Tab - Initiator Comparison

```
┌─────────────────────────────────────────────────────────────────────┐
│ ║ [Resumen] [Etapas]►[Iniciadores] [Defensas] [Asociac] [Gráf] ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║ ┌───────────────────────────────────────────────────────────┐ ║ │
│ ║ │Código│Nombre   │Nombre   │F  │F  │C  │C  │Etapa│Diferenc.│ ║ │
│ ║ │      │(Actual) │(Comp.)  │(A)│(C)│(A)│(C)│     │         │ ║ │
│ ║ ├──────┼─────────┼─────────┼───┼───┼───┼───┼─────┼─────────┤ ║ │
│ ║ │SI-001│Fallo    │Fallo    │ 2 │ 2 │ 1 │ 1 │Oper.│Sin camb.│ ║ │ White
│ ║ │SI-002│Rotura   │Rotura   │ 3 │ 2 │ 2 │ 2 │Oper.│Frec. dif│ ║ │ Yellow
│ ║ │SI-003│Error op.│Error    │ 1 │ 1 │ 3 │ 2 │Mant.│Nombre y │ ║ │ Yellow
│ ║ │      │         │operador │   │   │   │   │     │Cons. dif│ ║ │
│ ║ │SI-004│Pérdida  │(No ex.) │ 2 │ - │ 2 │ - │Oper.│Elim. en │ ║ │ Red
│ ║ │      │         │         │   │   │   │   │     │comparac.│ ║ │
│ ║ │SI-005│(No ex.) │Nuevo    │ - │ 1 │ - │ 2 │Para.│Nuevo en │ ║ │ Red
│ ║ │      │         │iniciador│   │   │   │   │     │comparac.│ ║ │
│ ║ └───────────────────────────────────────────────────────────┘ ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
└─────────────────────────────────────────────────────────────────────┘
```

### Defensas Tab - Defense Comparison

```
┌─────────────────────────────────────────────────────────────────────┐
│ ║ [Resumen] [Etapas] [Iniciadores]►[Defensas] [Asociac] [Gráf] ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║ ┌───────────────────────────────────────────────────────────┐ ║ │
│ ║ │Código  │Nombre     │Nombre     │Robustez│Robustez│Dif.   │ ║ │
│ ║ │        │(Actual)   │(Comp.)    │(Actual)│(Comp.) │       │ ║ │
│ ║ ├────────┼───────────┼───────────┼────────┼────────┼───────┤ ║ │
│ ║ │FMB-001 │Sistema    │Sistema    │   2    │   2    │Sin    │ ║ │ White
│ ║ │        │detección  │detección  │        │        │cambios│ ║ │
│ ║ │FMB-002 │Alarma     │Alarma     │   1    │   2    │Robust.│ ║ │ Yellow
│ ║ │        │temprana   │temprana   │        │        │difernt│ ║ │
│ ║ │BA-001  │Contención │Muro de    │   3    │   3    │Nombre │ ║ │ Yellow
│ ║ │        │primaria   │contención │        │        │dif.   │ ║ │
│ ║ │BA-002  │Sistema    │(No existe)│   2    │   -    │Elim.  │ ║ │ Red
│ ║ │        │redundante │           │        │        │en comp│ ║ │
│ ║ │RC-001  │(No existe)│Sistema    │   -    │   1    │Nueva  │ ║ │ Red
│ ║ │        │           │mitigación │        │        │en comp│ ║ │
│ ║ └───────────────────────────────────────────────────────────┘ ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
└─────────────────────────────────────────────────────────────────────┘
```

### Asociaciones Tab - Defense Association Comparison

```
┌─────────────────────────────────────────────────────────────────────┐
│ ║ [Resumen] [Etapas] [Iniciadores] [Defensas]►[Asociac] [Gráf] ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║ ┌───────────────────────────────────────────────────────────┐ ║ │
│ ║ │Iniciador│Defensa │Establ.│Establ.│Aplica.│Aplica.│Estado │ ║ │
│ ║ │         │        │(Actu.)│(Comp.)│(Actu.)│(Comp.)│       │ ║ │
│ ║ ├─────────┼────────┼───────┼───────┼───────┼───────┼───────┤ ║ │
│ ║ │SI-001   │FMB-001 │  Sí   │  Sí   │  Sí   │  Sí   │Sin    │ ║ │ Green
│ ║ │         │        │       │       │       │       │cambios│ ║ │
│ ║ │SI-001   │BA-001  │  Sí   │  Sí   │  Sí   │  No   │Aplic. │ ║ │ Yellow
│ ║ │         │        │       │       │       │       │difernt│ ║ │
│ ║ │SI-002   │FMB-002 │  Sí   │  No   │  Sí   │  No   │Establ.│ ║ │ Red
│ ║ │         │        │       │       │       │       │difernt│ ║ │
│ ║ │SI-003   │BA-002  │  No   │  Sí   │  No   │  Sí   │Establ.│ ║ │ Red
│ ║ │         │        │       │       │       │       │difernt│ ║ │
│ ║ └───────────────────────────────────────────────────────────┘ ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
└─────────────────────────────────────────────────────────────────────┘
```

### Gráficas Tab - Risk Profile Charts

```
┌─────────────────────────────────────────────────────────────────────┐
│ ║ [Resumen] [Etapas] [Iniciadores] [Defensas] [Asociac]►[Gráf] ║ │
│ ╠═══════════════════════════════════════════════════════════════╣ │
│ ║                                                               ║ │
│ ║          Perfil de Riesgo - Comparación                       ║ │
│ ║                                                               ║ │
│ ║  Número de Iniciadores                                        ║ │
│ ║  ↑                                                            ║ │
│ ║ 20│                                                           ║ │
│ ║   │       ██                                                  ║ │
│ ║ 15│   ██  ██  ██                                              ║ │
│ ║   │   ██  ██  ██                                              ║ │
│ ║ 10│   ██  ██  ██  ██          ■ Modelo Actual                ║ │
│ ║   │   ██  ██  ██  ██          ■ Modelo Comparación           ║ │
│ ║  5│   ██  ██  ██  ██  ██                                      ║ │
│ ║   │   ██  ██  ██  ██  ██  ██                                 ║ │
│ ║  0├───┴───┴───┴───┴───┴───┴───────────────────────────────→  ║ │
│ ║     RMA   RA   RM   RB                                        ║ │
│ ║                                                               ║ │
│ ║  Estadísticas:                                                ║ │
│ ║  Total iniciadores aplicables (Actual): 42                    ║ │
│ ║  Total iniciadores aplicables (Comparación): 45               ║ │
│ ║                                                               ║ │
│ ╚═══════════════════════════════════════════════════════════════╝ │
└─────────────────────────────────────────────────────────────────────┘
```

## Color Coding Legend

### Status Colors

- **Green**: Items are identical between models
- **Yellow**: Items exist in both but have differences
- **Red**: Items only in one model (added/removed)
- **Blue**: Items only in current model (for stage view)

### Table Cell Colors

```
┌────────────────────────────────────────┐
│ Status          │ Background Color     │
├─────────────────┼──────────────────────┤
│ Igual/Sin camb. │ Light Green #C8FFC8  │
│ Diferente/Dif.  │ Light Yellow #FFFFC8 │
│ Solo en uno     │ Light Red #FFC8C8    │
│ Only in current │ Light Blue #C8C8FF   │
└────────────────────────────────────────┘
```

## Export Output Example

The "Exportar Reporte" button generates an HTML file like this:

```html
<!DOCTYPE html>
<html>
<head>
    <style>
        body { font-family: Arial, sans-serif; }
        h1 { color: #0066cc; }
        h2 { color: #0088cc; margin-top: 20px; }
        table { border-collapse: collapse; width: 100%; margin: 10px 0; }
        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
        th { background-color: #0066cc; color: white; }
        .igual { background-color: #c8ffc8; }
        .diferente { background-color: #ffffc8; }
        .nuevo { background-color: #ffc8c8; }
    </style>
</head>
<body>
    <h1>Comparación de Modelos de Riesgo</h1>
    <p><b>Modelo Actual:</b> Mi Modelo v1.0</p>
    <p><b>Modelo Comparación:</b> Mi Modelo v2.0</p>
    <p><b>Fecha de comparación:</b> 24/10/2025 04:15:00</p>
    
    <h2>Resumen de Etapas y Subetapas</h2>
    <p>Total de etapas: 8</p>
    <p>Etapas idénticas: 7</p>
    <p>Etapas con diferencias: 1</p>
    
    <!-- ... more statistics ... -->
</body>
</html>
```

## Usage Workflow

```
1. User opens MARRTA
   ↓
2. Loads a risk model (.pdb file)
   ↓
3. Goes to menu: Administración → Comparador de modelos
   ↓
4. Dialog opens showing current model
   ↓
5. User clicks "Seleccionar..." button
   ↓
6. File picker dialog appears
   ↓
7. User selects another .pdb file to compare
   ↓
8. Dialog validates the file
   ↓
9. User clicks "Comparar Modelos" button
   ↓
10. Application loads and compares both models
   ↓
11. Results appear in tabbed interface
   ↓
12. User reviews different aspects in tabs
   ↓
13. (Optional) User clicks "Exportar Reporte"
   ↓
14. HTML report is saved to disk
   ↓
15. User closes dialog
```

## Key Features Visualization

### 1. Comprehensive Comparison
```
┌────────────────────────────────────────┐
│ What Gets Compared:                    │
│                                        │
│ ✓ Stage structure                      │
│ ✓ Substage counts                      │
│ ✓ Initiator codes                      │
│ ✓ Initiator names                      │
│ ✓ Frequency values (F)                 │
│ ✓ Consequence values (C)               │
│ ✓ Defense codes                        │
│ ✓ Defense names                        │
│ ✓ Defense robustness                   │
│ ✓ Defense-initiator associations       │
│ ✓ Active/inactive status               │
│ ✓ Risk profiles                        │
└────────────────────────────────────────┘
```

### 2. User-Friendly Interface
```
┌────────────────────────────────────────┐
│ Interface Features:                    │
│                                        │
│ • Tabbed layout for organized view     │
│ • Color-coded differences              │
│ • Sortable tables                      │
│ • Detailed tooltips                    │
│ • Professional charts                  │
│ • One-click export                     │
│ • Easy file selection                  │
│ • Clear status messages                │
└────────────────────────────────────────┘
```

### 3. Report Generation
```
┌────────────────────────────────────────┐
│ Export Features:                       │
│                                        │
│ • Professional HTML format             │
│ • All statistics included              │
│ • Timestamped output                   │
│ • Browser-friendly styling             │
│ • Easy to share                        │
│ • Printable format                     │
│ • Automatic filename generation        │
└────────────────────────────────────────┘
```

## Success Criteria

✅ Dialog opens from Administration menu
✅ Current model info displayed automatically
✅ File picker allows selecting comparison model
✅ Validation prevents comparing model with itself
✅ All comparison tabs populate correctly
✅ Color coding helps identify differences
✅ Charts display risk profile comparison
✅ Export generates valid HTML report
✅ All UI text uses tr() for translation
✅ Proper error handling for invalid files
✅ Database connections managed correctly
✅ Memory cleaned up when dialog closes

## Notes for Developers

- All database access uses QSqlQuery with prepared statements
- Comparison database uses unique connection name to avoid conflicts
- Tables use alternating row colors for better readability
- Charts use Qt Charts library (QtCharts module)
- All user-facing strings wrapped in tr() for internationalization
- Color coding follows consistent scheme across all tabs
- HTML export uses UTF-8 encoding
- Dialog properly cleans up resources in destructor
