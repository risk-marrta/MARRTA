# About Window Redesign - Changes Documentation

## Overview
The About window has been completely redesigned to be more visually appealing and to include updated logos and author information as requested in the issue.

## Changes Made

### 1. Logo Updates

#### New Logos Added:
- **ministerio.png**: Logo for Ministerio para la Transición Ecológica y el Reto Demográfico
- **csn.png**: Logo for Consejo de Seguridad Nuclear (CSN) - replaces the old "company_logo.png"

#### Updated Society Logos:
- **aetr.png**: Asociación Española de Técnicos en Radioprotección
- **sefm.png**: Sociedad Española de Física Médica
- **seor.png**: Sociedad Española de Oncología Radioterápica
- **sepr.png**: Sociedad Española de Protección Radiológica

#### Removed:
- **emiral.png**: Removed from the about window and logos.qrc

#### Kept Unchanged:
- **FundacionMapfre.png**: Fundación MAPFRE logo
- **vorticesoft.png**: VorticeSoft logo

### 2. Layout Redesign

The about window layout has been completely restructured:

- **Window size**: Increased from 600x780 to 700x850 pixels for better spacing
- **Layout type**: Changed from QGridLayout to QVBoxLayout for better vertical flow
- **Spacing**: Added proper spacing between all sections using spacers
- **Typography**: Improved font sizes and hierarchy:
  - Title (MARRTA): 28pt bold
  - Project info: 13pt bold
  - Section titles: 11pt bold
  - Body text: 10pt

### 3. Content Organization

The new layout follows this structure:

1. **Header Section**
   - MARRTA title (28pt)
   - Version information (11pt)
   - Description text (10pt)

2. **Society Logos Section**
   - Four society logos displayed in a single horizontal row
   - Equal sizing (140x70 pixels each)
   - AETR, SEFM, SEOR, SEPR

3. **CSN Logo Section**
   - Prominent CSN logo (80px height)
   - Centered display

4. **Sponsors Section**
   - "Con el apoyo de" heading
   - Fundación MAPFRE and Ministerio logos side by side
   - Horizontal layout with proper spacing

5. **Authors Section** (NEW)
   - "Autores" heading
   - Moisés Márquez (Vortice Soft)
   - Arturo Pérez Mulas (CSN)

6. **Collaboration Section** (NEW)
   - "Con la colaboración de" in italic
   - Elena Ferreras (CSN)

7. **Footer**
   - VorticeSoft logo
   - OK button

### 4. Resource Files Updated

- **logos.qrc**: Updated to include new logos and remove emiral
- **.gitignore**: Added pattern to ignore backup logo files (*_old.png)

## Important Notes

### Placeholder Logos
The current logos for AETR, SEFM, SEOR, SEPR, CSN, and Ministerio are **placeholder images** created programmatically. These should be replaced with the actual high-resolution logos from the official sources.

### How to Replace Placeholder Logos

To replace the placeholder logos with actual logos:

1. **Download the official logos** from these sources:
   - **Ministerio**: https://commons.wikimedia.org/wiki/File:Logotipo_del_Ministerio_para_la_Transici%C3%B3n_Ecol%C3%B3gica_y_el_Reto_Demogr%C3%A1fico.svg
   - **CSN**: https://www.vectorlogo.es/logos/logo-vector-consejo-de-seguridad-nuclear/
   - **SEPR**: https://www.sepr.es/comunicacion/sala-de-prensa/identidad-corporativa
   - **SEFM**: https://sefm.es/
   - **SEOR**: https://seor.es/recursos-audiovisuales/
   - **AETR**: Contact AETR for official logo

2. **Convert logos to PNG format** if needed (preferably with transparent backgrounds)

3. **Replace the files** in `resources/logos/` directory:
   ```bash
   cp downloaded_ministerio_logo.png resources/logos/ministerio.png
   cp downloaded_csn_logo.png resources/logos/csn.png
   cp downloaded_aetr_logo.png resources/logos/aetr.png
   cp downloaded_sefm_logo.png resources/logos/sefm.png
   cp downloaded_seor_logo.png resources/logos/seor.png
   cp downloaded_sepr_logo.png resources/logos/sepr.png
   ```

4. **Recommended logo dimensions**:
   - Society logos (AETR, SEFM, SEOR, SEPR): 280x140 pixels or similar aspect ratio
   - CSN logo: 400x160 pixels or similar aspect ratio
   - Ministerio logo: 400x120 pixels or similar aspect ratio
   - Logos will be scaled to fit, but maintaining aspect ratio is important

## Testing

To test the changes:

1. Build the project with qmake:
   ```bash
   qmake marrta.pro
   make
   ```

2. Run the application and navigate to Help → About (or however the About dialog is accessed)

3. Verify that:
   - All logos display correctly
   - Layout is visually appealing
   - Author information is clearly visible
   - Text is properly aligned and readable
   - No missing images or broken layouts

## Visual Mockup

A visual mockup of the new layout has been created in `/tmp/about_window_mockup.html`. Open this file in a web browser to see a preview of the layout structure.

## Files Modified

- `about.ui` - Complete redesign of the dialog layout
- `logos.qrc` - Updated resource file with new logos
- `.gitignore` - Added pattern for backup files
- `resources/logos/aetr.png` - Updated (placeholder)
- `resources/logos/sefm.png` - Updated (placeholder)
- `resources/logos/seor.png` - Updated (placeholder)
- `resources/logos/sepr.png` - Updated (placeholder)
- `resources/logos/csn.png` - New (placeholder)
- `resources/logos/ministerio.png` - New (placeholder)

## Files Backed Up (Not in Git)

- `resources/logos/aetr_old.png`
- `resources/logos/sefm_old.png`
- `resources/logos/seor_old.png`
- `resources/logos/sepr_old.png`

These backup files are excluded from git via .gitignore and can be deleted once the new logos are confirmed to work correctly.
