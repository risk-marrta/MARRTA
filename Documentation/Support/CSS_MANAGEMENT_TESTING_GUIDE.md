# CSS Management and Logo Embedding Testing Guide

## Overview
This document describes how to test the new CSS management and logo embedding features for HTML reports in MARRTA.

## Features Implemented

### 1. External CSS File
- **Default CSS Location**: `resources/report_style.css`
- **User CSS Location**: `~/.local/share/MARRTA/report_style.css` (Linux) or equivalent AppData location (Windows/Mac)
- CSS is loaded from user location if exists, otherwise uses default from resources

### 2. Logo Embedding
- Logo is converted to base64 data URI at runtime
- Embedded directly in HTML as `data:image/png;base64,...`
- Works in saved HTML files without requiring Qt resources

### 3. UI Controls
Three new buttons in the "Configurar Informe" dialog:
- **Guardar CSS** - Exports current CSS to a file
- **Importar CSS** - Imports a custom CSS file for reports
- **Restaurar por defecto** - Resets to default CSS

## Testing Steps

### Test 1: Default CSS Usage
1. Open MARRTA application
2. Navigate to Reports → Configurar Informe
3. Generate a report or preview
4. **Expected**: Report uses default blue/professional styling
5. **Verify**: Logo appears on cover page

### Test 2: Export Current CSS
1. In report configuration dialog, click "Guardar CSS"
2. Choose a location to save (e.g., Desktop/my_report_style.css)
3. **Expected**: File is saved successfully
4. Open the saved CSS file in a text editor
5. **Verify**: Contains complete CSS with ~500+ lines

### Test 3: Import Custom CSS
1. Edit the saved CSS file (e.g., change `.cover-title` background-color from `#4A7EBB` to `#FF0000`)
2. In report configuration dialog, click "Importar CSS"
3. Select the modified CSS file
4. **Expected**: Success message appears
5. Generate a new report or preview
6. **Verify**: Report uses modified styles (red cover title background)

### Test 4: CSS Persistence
1. After importing custom CSS (Test 3), close the dialog
2. Reopen the report configuration dialog
3. Generate another report
4. **Expected**: Custom CSS is still applied
5. **Verify**: User CSS is stored in AppData directory

### Test 5: Reset to Default
1. After customizing CSS, click "Restaurar por defecto"
2. **Expected**: Confirmation dialog appears
3. Confirm the action
4. Generate a new report
5. **Verify**: Report uses original default blue styling

### Test 6: Logo in Saved HTML
1. Generate a report and save as PDF
2. During generation, the HTML is created temporarily
3. Alternatively, save the preview HTML:
   - Click "Vista Previa HTML"
   - In preview window, save HTML (if available)
4. Open the saved HTML in a web browser (Chrome, Firefox, etc.)
5. **Expected**: Logo appears correctly on cover page
6. **Verify**: HTML file works standalone without Qt application

### Test 7: Multiple Orientation Tests
1. Generate report with Portrait orientation
2. **Verify**: CSS contains `@page { size: A4 portrait }`
3. Generate report with Landscape orientation
4. **Verify**: CSS is updated to `@page { size: A4 landscape }`

### Test 8: CSS Syntax Validation
1. Create an invalid CSS file (missing closing braces, invalid properties)
2. Try to import it
3. **Expected**: Report may not render correctly or show errors
4. Reset to default CSS
5. **Verify**: Default CSS restores proper formatting

## File Locations to Check

### Windows
- User CSS: `%APPDATA%\MARRTA\report_style.css`
- Or: `C:\Users\<username>\AppData\Roaming\MARRTA\report_style.css`

### Linux
- User CSS: `~/.local/share/MARRTA/report_style.css`

### macOS
- User CSS: `~/Library/Application Support/MARRTA/report_style.css`

## Expected Behavior

### CSS Loading Priority
1. Check for user CSS in AppData location
2. If found and valid, use user CSS
3. If not found or empty, load default CSS from Qt resources
4. Apply orientation-specific modifications to @page rule

### Logo Base64 Conversion
- Logo loaded from: `:/logos/resources/logos/logo.png`
- Converted to: `data:image/png;base64,iVBORw0KGg...` (64KB+)
- Embedded in: `<img src="data:image/png;base64,..." />`

### Button Functionality
- **Guardar CSS**: Saves current active CSS (user or default) to selected file
- **Importar CSS**: Copies selected CSS to user AppData location
- **Restaurar**: Deletes user CSS file, forcing fallback to default

## Common Issues and Solutions

### Issue: Custom CSS not applying
- **Solution**: Check user CSS path exists and file has content
- **Solution**: Verify CSS syntax is valid
- **Solution**: Try Reset and Import again

### Issue: Logo not showing
- **Solution**: Verify logo.png exists in resources/logos/
- **Solution**: Check logos.qrc includes logo.png
- **Solution**: Rebuild project to update resources

### Issue: CSS changes not persisting
- **Solution**: Ensure AppData directory is writable
- **Solution**: Check file permissions on user CSS path

### Issue: Preview looks different than PDF
- **Solution**: Qt's QTextDocument rendering may differ slightly
- **Solution**: This is expected; PDF is the authoritative output

## Code References

### Key Functions
```cpp
QString loadCSS()                    // Load CSS (user or default)
QString getDefaultCSS()              // Load from Qt resources
QString getUserCSSPath()             // Get AppData path
bool saveUserCSS(const QString&)     // Save to AppData
QString getLogoAsBase64()            // Convert logo to base64
void on_btnSaveCSS_clicked()         // Export CSS handler
void on_btnLoadCSS_clicked()         // Import CSS handler
void on_btnResetCSS_clicked()        // Reset CSS handler
QString generateFullHtml()           // Generate HTML with CSS
QString generateCoverPage()          // Generate cover with logo
```

### Modified Files
- `dlgconfiginforme.h` - Added CSS management methods
- `dlgconfiginforme.cpp` - Implemented CSS loading and logo embedding
- `dlgconfiginforme.ui` - Added CSS management button group
- `logos.qrc` - Added CSS resource reference
- `resources/report_style.css` - Default CSS file

## Validation Checklist

- [ ] Default CSS loads correctly from resources
- [ ] CSS can be exported to external file
- [ ] Custom CSS can be imported and applied
- [ ] Custom CSS persists across dialog reopenings
- [ ] Reset to default removes custom CSS
- [ ] Logo appears in preview
- [ ] Logo is embedded as base64 in HTML
- [ ] Saved HTML works in external browsers
- [ ] Portrait/Landscape orientation affects CSS
- [ ] User CSS directory is created if missing
- [ ] File dialogs use correct filters (.css)
- [ ] Success/error messages are shown appropriately
- [ ] Confirmation dialog appears for Reset action

## Notes for Developers

### CSS Customization Examples
Users can customize any CSS property. Common customizations:

**Change color scheme:**
```css
.cover-title {
  background-color: #2E7D32; /* Green instead of blue */
}
```

**Change fonts:**
```css
body {
  font-family: 'Times New Roman', serif;
}
```

**Adjust spacing:**
```css
.content-block {
  padding: 30px; /* More padding */
  margin-bottom: 30px;
}
```

### Future Enhancements
- Add CSS editor within the application
- Provide multiple CSS templates
- Add CSS validation before import
- Include CSS preview in configuration dialog
- Export reports with bundled CSS file
