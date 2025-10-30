# PDF Report Table Format Examples

## Overview

This document shows examples of the improved table formats in the PDF report, comparing the old format with the new format.

## 1. High-Risk Sequences Table (RARMA Table)

### Old Format (5 columns)

```
┌────────┬──────────────────────────────┬──────────────────┬────────────────┬────────────────────────────┐
│ Código │ Suceso iniciador             │ Riesgo de        │ Riesgo         │ Barreras y reductores      │
│        │                              │ Referencia       │ Obtenido       │ faltantes                  │
├────────┼──────────────────────────────┼──────────────────┼────────────────┼────────────────────────────┤
│ IE1.1  │ Error en administración de   │ RMA              │ RA             │ Barreras                   │
│        │ medicamento (FA, CMA)        │                  │                │ - Doble verificación       │
│        │                              │                  │                │ - Sistema de alertas       │
│        │                              │                  │                │ Reductores de frecuencia   │
│        │                              │                  │                │ - Formación específica     │
├────────┼──────────────────────────────┼──────────────────┼────────────────┼────────────────────────────┤
│ IE2.3  │ Fallo en identificación      │ RA               │ RM             │ Ninguna                    │
│        │ paciente (FM, CA)            │                  │                │                            │
└────────┴──────────────────────────────┴──────────────────┴────────────────┴────────────────────────────┘
```

### New Format (7 columns, Landscape)

```
┌────────┬─────────────────────┬───────────────┬──────────────────┬─────────────┬──────────────────┬─────────────────────────────┐
│ Código │ Suceso iniciador    │ F, C          │ F, P, C          │ Riesgo      │ Riesgo           │ Barreras y reductores       │
│        │                     │ nominales     │ calculadas       │ calc.       │ Referencia       │ faltantes                   │
├────────┼─────────────────────┼───────────────┼──────────────────┼─────────────┼──────────────────┼─────────────────────────────┤
│ IE1.1  │ Error en            │ FA, CMA       │ FM, PB, CA       │ RA          │ RMA              │ Barreras                    │
│        │ administración de   │               │                  │             │                  │ B001 - Doble verificación   │
│        │ medicamento         │               │                  │             │                  │ B003 - Sistema de alertas   │
│        │                     │               │                  │             │                  │ Reductores de frecuencia    │
│        │                     │               │                  │             │                  │ RF002 - Formación específica│
├────────┼─────────────────────┼───────────────┼──────────────────┼─────────────┼──────────────────┼─────────────────────────────┤
│ IE2.3  │ Fallo en            │ FM, CA        │ FB, PMB, CM      │ RM          │ RA               │ Ninguna                     │
│        │ identificación      │               │                  │             │                  │                             │
│        │ paciente            │               │                  │             │                  │                             │
└────────┴─────────────────────┴───────────────┴──────────────────┴─────────────┴──────────────────┴─────────────────────────────┘
```

**Key Improvements:**
1. Event name in separate column (no frequency/consequence in parentheses)
2. Separate column for nominal frequency and consequences
3. **NEW**: Calculated frequency, probability, and consequences shown
4. **NEW**: Calculated risk shown separately
5. Reference risk (minimum risk) clearly labeled
6. **Defense codes** now shown (e.g., "B001 - Doble verificación")

## 2. Defense Tables (Barriers, Frequency Reducers, Consequence Reducers)

### Old Format (Not in original report for individual events)

Not previously shown in detail pages.

### New Format (4 columns)

#### Barriers Table

```
┌────────┬──────────────────────────┬─────────────────────────────────┬─────────────────┐
│ Código │ Nombre                   │ Descripción                     │ Robustez        │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ B001   │ Doble verificación       │ Dos personas verifican la       │ Robusta         │
│        │                          │ medicación antes de administrar │                 │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ B002   │ Código de barras         │ Sistema automático de           │ Muy robusta     │
│        │                          │ verificación por código de      │                 │
│        │                          │ barras                          │                 │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ B003   │ Sistema de alertas       │ Alertas automáticas en el       │ Normal          │
│        │                          │ sistema electrónico             │                 │
└────────┴──────────────────────────┴─────────────────────────────────┴─────────────────┘
```

#### Frequency Reducers Table

```
┌────────┬──────────────────────────┬─────────────────────────────────┬─────────────────┐
│ Código │ Nombre                   │ Descripción                     │ Robustez        │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RF001  │ Protocolo estandarizado  │ Procedimiento documentado y     │ Robusta         │
│        │                          │ validado                        │                 │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RF002  │ Formación específica     │ Capacitación regular del        │ Normal          │
│        │                          │ personal sanitario              │                 │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RF003  │ Lista de verificación    │ Checklist obligatorio antes     │ Blanda          │
│        │                          │ del procedimiento               │                 │
└────────┴──────────────────────────┴─────────────────────────────────┴─────────────────┘
```

#### Consequence Reducers Table

```
┌────────┬──────────────────────────┬─────────────────────────────────┬─────────────────┐
│ Código │ Nombre                   │ Descripción                     │ Robustez        │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RC001  │ Monitorización continua  │ Vigilancia constante de         │ Muy robusta     │
│        │                          │ signos vitales                  │                 │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RC002  │ Equipo de emergencia     │ Carro de paros disponible       │ Robusta         │
│        │                          │ en todas las unidades           │                 │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RC003  │ Protocolo de respuesta   │ Plan de acción ante eventos     │ Normal          │
│        │ rápida                   │ adversos                        │                 │
└────────┴──────────────────────────┴─────────────────────────────────┴─────────────────┘
```

**Key Improvements:**
1. **Defense code** shown in first column (instead of ID number)
2. **NEW**: Robustness column shows text values:
   - "Muy blanda" (Very soft, -1)
   - "Blanda" (Soft, 0)
   - "Normal" (Normal, 1)
   - "Robusta" (Robust, 2)
   - "Muy robusta" (Very robust, 3)
3. Same format applied to all three defense types (barriers, frequency reducers, consequence reducers)
4. Consistent column widths and formatting

## 3. Event Detail Page

### New Format (Complete Listing Section)

```
═══════════════════════════════════════════════════════════════════════════════
Iniciador: IE1.1 - Error en administración de medicamento
═══════════════════════════════════════════════════════════════════════════════

Información del Iniciador
┌──────────────────────────────┬──────────────────────────────────────────────┐
│ Código                       │ IE1.1                                        │
├──────────────────────────────┼──────────────────────────────────────────────┤
│ Nombre                       │ Error en administración de medicamento       │
├──────────────────────────────┼──────────────────────────────────────────────┤
│ Frecuencia nominal           │ FA (Frecuencia Alta)                         │
├──────────────────────────────┼──────────────────────────────────────────────┤
│ Consecuencias nominales      │ CMA (Consecuencias Muy Altas)                │
├──────────────────────────────┼──────────────────────────────────────────────┤
│ F, P, C calculadas           │ FM, PB, CA                                   │
├──────────────────────────────┼──────────────────────────────────────────────┤
│ Riesgo calculado             │ RA (Riesgo Alto)                             │
└──────────────────────────────┴──────────────────────────────────────────────┘

Barreras
┌────────┬──────────────────────────┬─────────────────────────────────┬─────────────────┐
│ Código │ Nombre                   │ Descripción                     │ Robustez        │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ B001   │ Doble verificación       │ Dos personas verifican la       │ Robusta         │
│        │                          │ medicación antes de administrar │                 │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ B003   │ Sistema de alertas       │ Alertas automáticas en el       │ Normal          │
│        │                          │ sistema electrónico             │                 │
└────────┴──────────────────────────┴─────────────────────────────────┴─────────────────┘

Reductores de Frecuencia
┌────────┬──────────────────────────┬─────────────────────────────────┬─────────────────┐
│ Código │ Nombre                   │ Descripción                     │ Robustez        │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RF002  │ Formación específica     │ Capacitación regular del        │ Normal          │
│        │                          │ personal sanitario              │                 │
└────────┴──────────────────────────┴─────────────────────────────────┴─────────────────┘

Reductores de Consecuencia
┌────────┬──────────────────────────┬─────────────────────────────────┬─────────────────┐
│ Código │ Nombre                   │ Descripción                     │ Robustez        │
├────────┼──────────────────────────┼─────────────────────────────────┼─────────────────┤
│ RC001  │ Monitorización continua  │ Vigilancia constante de         │ Muy robusta     │
│        │                          │ signos vitales                  │                 │
└────────┴──────────────────────────┴─────────────────────────────────┴─────────────────┘
```

**Key Features:**
1. Each event starts on a new page
2. Event information table includes calculated values
3. Separate tables for each defense type
4. All tables show codes and robustness

## 4. Code Abbreviations

### Frequency (Frecuencia)
- **FMB**: Frecuencia Muy Baja (Very Low Frequency)
- **FB**: Frecuencia Baja (Low Frequency)
- **FM**: Frecuencia Media (Medium Frequency)
- **FA**: Frecuencia Alta (High Frequency)

### Probability (Probabilidad)
- **PMB**: Probabilidad Muy Baja (Very Low Probability)
- **PB**: Probabilidad Baja (Low Probability)
- **PM**: Probabilidad Media (Medium Probability)
- **PA**: Probabilidad Alta (High Probability)

### Consequences (Consecuencias)
- **CB**: Consecuencias Bajas (Low Consequences)
- **CM**: Consecuencias Medias (Medium Consequences)
- **CA**: Consecuencias Altas (High Consequences)
- **CMA**: Consecuencias Muy Altas (Very High Consequences)

### Risk (Riesgo)
- **RB**: Riesgo Bajo (Low Risk)
- **RM**: Riesgo Medio (Medium Risk)
- **RA**: Riesgo Alto (High Risk)
- **RMA**: Riesgo Muy Alto (Very High Risk)

### Robustness (Robustez)
- **Muy blanda**: Very Soft (hardiness = -1)
- **Blanda**: Soft (hardiness = 0)
- **Normal**: Normal (hardiness = 1)
- **Robusta**: Robust (hardiness = 2)
- **Muy robusta**: Very Robust (hardiness = 3)

## 5. Page Orientation

### Old Format
- **Portrait** (vertical) orientation
- Limited column width
- Text wrapping in cells
- Difficult to read wide tables

### New Format
- **Landscape** (horizontal) orientation
- More space for columns
- Better readability
- Reduced text wrapping

```
Portrait:                    Landscape:
┌──────────┐                ┌────────────────────────────────┐
│          │                │                                │
│          │                │                                │
│          │                │                                │
│          │                │                                │
│          │                │                                │
│  A4      │                │            A4                  │
│          │                │                                │
│          │                │                                │
│          │                └────────────────────────────────┘
│          │
│          │
└──────────┘
```

## 6. Report Structure Examples

### Example 1: High-Risk Only Report

```
1. Cover Page (always included)
2. Resume/Summary (always included)
3. Sequences with RMA
   - Table of all RMA events with enhanced columns
4. Sequences with RA
   - Table of all RA events with enhanced columns
5. Risk Profile and Model Characteristics (always included)
6. Defenses and Initiating Events Relationship (always included)
```

### Example 2: Complete Listing Report

```
1. Cover Page (always included)
2. Resume/Summary (always included)
3. Complete Sequences Listing
   - Event IE1.1 (detail page with tables)
   - Event IE1.2 (detail page with tables)
   - Event IE2.1 (detail page with tables)
   - ... (all events from selected substages)
4. Risk Profile and Model Characteristics (always included)
5. Defenses and Initiating Events Relationship (always included)
```

### Example 3: Combined Report

```
1. Cover Page (always included)
2. Resume/Summary (always included)
3. Sequences with RMA
4. Sequences with RA
5. Complete Sequences Listing (selected substages)
6. Risk Profile and Model Characteristics (always included)
7. Defenses and Initiating Events Relationship (always included)
```

## Translation Note

All table headers and labels use `tr()` for proper internationalization:
- `tr("Código")` - Code
- `tr("Nombre")` - Name
- `tr("Descripción")` - Description
- `tr("Robustez")` - Robustness
- `tr("F, C nominales")` - Nominal F, C
- `tr("F, P, C calculadas")` - Calculated F, P, C
- `tr("Riesgo calc.")` - Calculated risk
- etc.

This ensures the report can be easily translated to other languages (English, Catalan, etc.).
