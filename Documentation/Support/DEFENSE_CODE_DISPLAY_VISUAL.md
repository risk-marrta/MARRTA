# Defense Code and Hardiness Display - Visual Comparison

## Before: Defense Lists Without Codes or Hardiness

```
┌─────────────────────────────────────────────────┐
│ Reductores de Frecuencia                        │
├─────────────────────────────────────────────────┤
│ ☐ Sistema de detección de incendios            │
│ ☑ Alarmas de temperatura                       │
│ ☐ Inspecciones regulares                       │
│ ☑ Mantenimiento preventivo                     │
│ ☐ Capacitación del personal                    │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│ Barreras                                        │
├─────────────────────────────────────────────────┤
│ ☑ Puertas cortafuego                           │
│ ☐ Sistema de extinción automático              │
│ ☑ Compartimentación                            │
│ ☐ Vías de evacuación señalizadas               │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│ Reductores de Consecuencia                      │
├─────────────────────────────────────────────────┤
│ ☐ Plan de emergencia                           │
│ ☑ Brigada de emergencia                        │
│ ☐ Seguros contra incendio                      │
│ ☑ Sistema de respaldo de datos                 │
└─────────────────────────────────────────────────┘
```

**Problem:** Users need to double-click each defense to see its code and hardiness level, which is inefficient when trying to locate a specific defense by its code or assess its robustness.

---

## After: Defense Lists With Codes and Right-Aligned Hardiness

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│ Reductores de Frecuencia                                                                         │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ☐ [FMB-001] Sistema de detección de incendios                              [Robusta]            │
│ ☑ [FMB-002] Alarmas de temperatura                                         [Normal]             │
│ ☐ [FMB-003] Inspecciones regulares                                         [Blanda]             │
│ ☑ [FMB-004] Mantenimiento preventivo                                       [Muy robusta]        │
│ ☐ [FMB-005] Capacitación del personal                                      [Normal]             │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│ Barreras                                                                                         │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ☑ [RA-001] Puertas cortafuego                                              [Muy robusta]        │
│ ☐ [RA-002] Sistema de extinción automático                                 [Robusta]            │
│ ☑ [RA-003] Compartimentación                                               [Robusta]            │
│ ☐ [RA-004] Vías de evacuación señalizadas                                  [Normal]             │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│ Reductores de Consecuencia                                                                       │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ ☐ [RMA-001] Plan de emergencia                                             [Normal]             │
│ ☑ [RMA-002] Brigada de emergencia                                          [Robusta]            │
│ ☐ [RMA-003] Seguros contra incendio                                        [Blanda]             │
│ ☑ [RMA-004] Sistema de respaldo de datos                                   [Muy robusta]        │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
```

**Benefits:**
- Defense code is immediately visible on the left
- Hardiness level is right-aligned in brackets for easy scanning
- Easy to identify and locate specific defenses by code
- Users can quickly compare hardiness levels across defenses
- Users can prioritize defenses based on robustness without opening dialogs
- No need to open defense details dialog for basic identification
- Consistent format across all defense types

---

## Display Format

**Complete Format:** `[code] Name       [Hardiness]` (right-aligned)

**Examples:**
- `[FMB-001] Sistema de detección de incendios                              [Robusta]`
- `[RA-003] Compartimentación                                               [Muy robusta]`
- `[RMA-004] Sistema de respaldo de datos                                   [Normal]`

**Code Prefixes:**
- **FMB**: Reductores de Frecuencia (Frequency Reducers)
- **RA**: Barreras (Barriers)  
- **RMA**: Reductores de Consecuencia (Consequence Reducers)

**Hardiness Levels (displayed in brackets, right-aligned):**
- **[Muy blanda]** (Very soft) - Lowest robustness
- **[Blanda]** (Soft) - Low robustness
- **[Normal]** (Normal) - Medium robustness
- **[Robusta]** (Robust) - High robustness
- **[Muy robusta]** (Very robust) - Highest robustness

---

## User Interactions Preserved

All user interactions continue to work correctly with the new display format:

### 1. Checkbox Click
- Clicking the checkbox activates/deactivates the defense
- Code is extracted from display text to find matching defense in data model

### 2. Double-Click
- Double-clicking opens the defense information dialog
- Shows complete details: code, type, name, description, robustez

### 3. Context Menu (Right-Click)
```
┌──────────────────────────────────────────────┐
│ ☑ Marcar todos en este iniciador             │
│ ☐ Desmarcar todos en este iniciador          │
├──────────────────────────────────────────────┤
│ ☑ Marcar esta defensa en todos los iniciadores │
│ ☐ Desmarcar esta defensa en todos los iniciadores │
├──────────────────────────────────────────────┤
│ 🔍 Analizar fortaleza                        │
│ 📋 Mostrar descripción                       │
└──────────────────────────────────────────────┘
```

All menu actions work correctly by extracting the defense name from the display text.

---

## Technical Implementation

### Display Text Generation
```cpp
QString displayText = QString("[%1] %2").arg(linea.code, linea.nombre);
```

### Name Extraction from Display Text
```cpp
QString temp = displayText.section("] ", 1); // Remove "[code] " part
// Remove the trailing "[Hardness]" part by finding the last '['
int lastBracket = temp.lastIndexOf('[');
if (lastBracket > 0) {
    temp = temp.left(lastBracket).trimmed(); // Remove "[Hardness]" and trim spaces
}
QString nombre = temp;
```

**Example:**
- Input: `"[FMB-001] Sistema de detección de incendios       [Robusta]"`
- After first section: `"Sistema de detección de incendios       [Robusta]"`
- After finding last '[': position found
- After left() and trim: `"Sistema de detección de incendios"`

The extraction is done in steps:
1. `section("] ", 1)` removes the `[code] ` prefix
2. `lastIndexOf('[')` finds the position of the hardiness opening bracket
3. `left(lastBracket).trimmed()` removes the `[Hardiness]` suffix and trims spaces
