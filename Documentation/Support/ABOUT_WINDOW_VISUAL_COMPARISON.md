# About Window Visual Comparison

## Layout Structure Comparison

### BEFORE (Old Layout)
```
┌─────────────────────────────────────┐
│           MARRTA (24pt)             │
│        [Version]                    │
│   [Description text]                │
│                                     │
│  Creado dentro del Proyecto MARRTA │
│                                     │
│  ┌──────┐  ┌──────┐                │
│  │ AETR │  │ SEFM │                │
│  └──────┘  └──────┘                │
│  ┌──────┐  ┌──────┐                │
│  │ SEOR │  │ SEPR │                │
│  └──────┘  └──────┘                │
│                                     │
│     ┌──────────────┐                │
│     │ COMPANY_LOGO │                │
│     └──────────────┘                │
│                                     │
│      Con el apoyo de                │
│     ┌──────────────┐                │
│     │   MAPFRE     │                │
│     └──────────────┘                │
│                                     │
│   Y la colaboración de              │
│     ┌──────────────┐                │
│     │  VORTICESOFT │                │
│     └──────────────┘                │
│                                     │
│              [OK]                   │
└─────────────────────────────────────┘
```

### AFTER (New Layout)
```
┌────────────────────────────────────────────┐
│             MARRTA (28pt)                  │
│            Versión X.X.X                   │
│   Código de cálculo de análisis de...     │
│                                            │
│    Creado dentro del Proyecto MARRTA      │
│                                            │
│  ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐     │
│  │ AETR │ │ SEFM │ │ SEOR │ │ SEPR │     │
│  └──────┘ └──────┘ └──────┘ └──────┘     │
│                                            │
│          ┌────────────────┐                │
│          │      CSN       │                │
│          └────────────────┘                │
│                                            │
│          Con el apoyo de                   │
│   ┌──────────┐      ┌────────────┐        │
│   │  MAPFRE  │      │ MINISTERIO │        │
│   └──────────┘      └────────────┘        │
│                                            │
│              Autores                       │
│      Moisés Márquez (Vortice Soft)        │
│       Arturo Pérez Mulas (CSN)            │
│                                            │
│       Con la colaboración de               │
│         Elena Ferreras (CSN)               │
│                                            │
│        ┌──────────────┐                    │
│        │ VORTICESOFT  │                    │
│        └──────────────┘                    │
│                                            │
│                              [OK]          │
└────────────────────────────────────────────┘
```

## Key Improvements

### 1. Visual Hierarchy
- **Before**: Mixed layout with inconsistent spacing
- **After**: Clear vertical flow with consistent spacing and visual hierarchy

### 2. Logo Organization
- **Before**: Society logos in 2x2 grid, company_logo separate
- **After**: Society logos in single 4-column row, CSN logo prominent

### 3. New Content
- **Before**: No author information
- **After**: Clear author and collaboration sections

### 4. Sponsors
- **Before**: Only Fundación MAPFRE
- **After**: Fundación MAPFRE + Ministerio (side by side)

### 5. Removed Content
- **Before**: Emiral logo included
- **After**: Emiral logo removed

### 6. Logo Updates
- **Before**: company_logo.png used for organization
- **After**: csn.png specifically for CSN

### 7. Size and Spacing
- **Before**: 600x780 pixels, cramped layout
- **After**: 700x850 pixels, generous spacing with spacers

## Content Changes

### Logos Removed:
- ❌ Emiral

### Logos Added:
- ✅ Ministerio para la Transición Ecológica y el Reto Demográfico

### Logos Renamed/Updated:
- 🔄 company_logo.png → csn.png (CSN)
- 🔄 aetr.png (updated)
- 🔄 sefm.png (updated)
- 🔄 seor.png (updated)
- 🔄 sepr.png (updated)

### Logos Kept:
- ✓ FundacionMapfre.png
- ✓ vorticesoft.png

### New Text Content:
```
Autores:
- Moisés Márquez (Vortice Soft)
- Arturo Pérez Mulas (CSN)

Con la colaboración de:
- Elena Ferreras (CSN)
```

## Typography Improvements

| Element | Before | After |
|---------|--------|-------|
| Title | 24pt bold | 28pt bold |
| Version | 12pt | 11pt |
| Description | 12pt | 10pt |
| Project Info | 14pt bold | 13pt bold |
| Section Titles | 12pt | 11pt bold |
| Body Text | - | 10pt |
| Collaboration | - | 10pt italic |

## Layout Type Change

- **Before**: QGridLayout (complex grid with merged cells)
- **After**: QVBoxLayout (simple vertical flow with nested layouts)

This change makes the layout:
- Easier to maintain
- More predictable
- Better for responsive design
- Clearer visual hierarchy
