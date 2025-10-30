# Flowchart Generator - Testing Guide

## Prerequisites

- Qt 6.10 development environment
- MARRTA compiled with the new flowchart generator feature
- A valid MARRTA risk model file (.pdb)

## Building with the New Feature

### Step 1: Update Translation Files (Optional)

If you want to test translations:

```bash
cd /home/runner/work/MARRTA/MARRTA
lupdate marrta.pro
lrelease marrta.pro
```

### Step 2: Compile MARRTA

```bash
cd /home/runner/work/MARRTA/MARRTA
qmake marrta.pro
make
```

Expected output:
- No compilation errors
- flujogramagenerator.cpp compiles successfully
- Executable created: `./marrta`

## Testing Scenarios

### Test 1: Menu Item Visibility

**Steps:**
1. Launch MARRTA: `./marrta`
2. Open the "Modelo" menu
3. Locate "Generar flujograma..." menu item

**Expected Result:**
- Menu item appears after "Generar informe..."
- Has list-alt icon
- Tooltip shows "Generar flujograma vertical del modelo de riesgo"

**Pass Criteria:** ✓ Menu item visible and properly positioned

---

### Test 2: No Model Loaded Warning

**Steps:**
1. Launch MARRTA without loading a model
2. Click "Modelo → Generar flujograma..."

**Expected Result:**
- Warning dialog appears
- Title: "No hay modelo cargado"
- Message: "No hay ningún modelo de riesgo cargado.\nCargue un modelo para poder editarlo."
- Icon: Warning (⚠️)

**Pass Criteria:** ✓ Warning dialog prevents action when no model loaded

---

### Test 3: File Save Dialog

**Steps:**
1. Load a MARRTA model (.pdb file)
2. Click "Modelo → Generar flujograma..."

**Expected Result:**
- File save dialog appears
- Title: "Guardar flujograma"
- Default filename: "flujograma_modelo.html"
- Default location: User's home directory
- File filter: "Archivos HTML (*.html *.htm)"

**Pass Criteria:** ✓ File save dialog appears with correct settings

---

### Test 4: Cancel File Save

**Steps:**
1. Load a MARRTA model
2. Click "Modelo → Generar flujograma..."
3. Click "Cancel" in file save dialog

**Expected Result:**
- Dialog closes
- No file created
- No error message
- Application returns to normal state

**Pass Criteria:** ✓ Cancel operation handled gracefully

---

### Test 5: Successful Flowchart Generation

**Steps:**
1. Load a MARRTA model with stages and events
2. Click "Modelo → Generar flujograma..."
3. Choose a location and filename
4. Click "Guardar"

**Expected Result:**
- Success message appears
- Title: "Flujograma generado"
- Message includes file path
- HTML file created at specified location

**Pass Criteria:** ✓ File created successfully with success message

---

### Test 6: Browser Opening Prompt

**Steps:**
1. Generate flowchart (Test 5)
2. After success message, observe the prompt

**Expected Result:**
- Prompt dialog appears
- Title: "Abrir flujograma"
- Message: "¿Desea abrir el flujograma generado en el navegador?"
- Buttons: "Sí" and "No"

**Pass Criteria:** ✓ Browser prompt appears after successful generation

---

### Test 7: Browser Opening (Yes)

**Steps:**
1. Generate flowchart
2. Click "Sí" in browser prompt

**Expected Result:**
- Default browser opens
- HTML file loads
- Flowchart displays correctly

**Pass Criteria:** ✓ Browser opens with the flowchart

---

### Test 8: Browser Opening (No)

**Steps:**
1. Generate flowchart
2. Click "No" in browser prompt

**Expected Result:**
- Prompt closes
- Browser does not open
- User can manually open file later

**Pass Criteria:** ✓ No browser opens, operation completes

---

### Test 9: HTML Structure Validation

**Steps:**
1. Generate flowchart
2. Open HTML file in text editor
3. Verify structure

**Expected Elements:**
- `<!DOCTYPE html>`
- `<html lang="es">`
- `<meta charset="UTF-8">`
- `<title>Flujograma del Modelo de Riesgo - MARRTA</title>`
- `<style>` section with CSS
- `<script>` section with JavaScript
- Flowchart content with stages/events

**Pass Criteria:** ✓ Valid HTML5 structure

---

### Test 10: Visual Appearance

**Steps:**
1. Generate flowchart
2. Open in browser (Chrome, Firefox, Edge, Safari)

**Expected Visual Elements:**
- Page title: "Flujograma del Modelo de Riesgo"
- Blue gradient headers for stages
- Purple-bordered substages
- Event items with:
  - Event code (left)
  - Event name (center)
  - Risk level badge (right)
- Legend at bottom with color meanings

**Pass Criteria:** ✓ Professional, clean appearance

---

### Test 11: Risk Level Colors

**Steps:**
1. Generate flowchart with events of different risk levels
2. Verify colors in browser

**Expected Colors:**
- RB: Gray (#808080)
- RM: Green (#70AD47)
- RA: Orange (#FFA500)
- RMA: Red (#FF0000)

**Pass Criteria:** ✓ Risk levels properly color-coded

---

### Test 12: Interactive Features

**Steps:**
1. Open flowchart in browser
2. Click on stage headers

**Expected Behavior:**
- First click: Stage collapses, content hides
- Toggle icon changes: ▼ → ►
- Second click: Stage expands, content shows
- Toggle icon changes: ► → ▼
- Animation is smooth

**Pass Criteria:** ✓ Collapsible stages work correctly

---

### Test 13: Hover Effects

**Steps:**
1. Open flowchart in browser
2. Hover mouse over events

**Expected Behavior:**
- Event slides slightly to the right
- Shadow appears beneath event
- Transition is smooth
- Returns to normal when mouse leaves

**Pass Criteria:** ✓ Hover effects work smoothly

---

### Test 14: Small Model (< 10 events)

**Steps:**
1. Load model with few stages/events
2. Generate flowchart
3. Verify output

**Expected Result:**
- File generates in < 1 second
- All events visible
- Layout is not too sparse

**Pass Criteria:** ✓ Works correctly with small model

---

### Test 15: Medium Model (10-100 events)

**Steps:**
1. Load model with moderate number of events
2. Generate flowchart
3. Verify output

**Expected Result:**
- File generates in 1-3 seconds
- All stages/events present
- Scrolling works smoothly
- No performance issues

**Pass Criteria:** ✓ Works correctly with medium model

---

### Test 16: Large Model (100+ events)

**Steps:**
1. Load model with many events
2. Generate flowchart
3. Verify output

**Expected Result:**
- File generates in reasonable time (< 10 seconds)
- Browser can load and render file
- Collapsible stages help manage content
- Scrolling is smooth

**Pass Criteria:** ✓ Works correctly with large model

---

### Test 17: Model with No Events

**Steps:**
1. Load model with stages but no events
2. Generate flowchart

**Expected Result:**
- Flowchart generates
- Shows stages only
- No errors
- Empty stages display correctly

**Pass Criteria:** ✓ Handles empty stages gracefully

---

### Test 18: Model with Events Without Risk

**Steps:**
1. Generate flowchart with events that have NULL risk
2. Verify display

**Expected Result:**
- Events display correctly
- Default gray color used for NULL risk
- No JavaScript errors in console

**Pass Criteria:** ✓ Handles NULL risk values

---

### Test 19: UTF-8 Character Support

**Steps:**
1. Generate flowchart from model with:
   - Spanish characters (ñ, á, é, í, ó, ú)
   - Special symbols
2. Verify in browser

**Expected Result:**
- All characters display correctly
- No encoding issues
- Text is readable

**Pass Criteria:** ✓ UTF-8 characters display correctly

---

### Test 20: File Overwrite

**Steps:**
1. Generate flowchart, save as "test.html"
2. Generate again, save with same filename
3. Confirm overwrite in system dialog

**Expected Result:**
- System prompts for overwrite confirmation
- File is overwritten when confirmed
- New content replaces old content

**Pass Criteria:** ✓ File overwrite works correctly

---

## Browser Compatibility Testing

Test the generated HTML in multiple browsers:

| Browser | Version | Status |
|---------|---------|--------|
| Chrome | Latest | ☐ Pass / ☐ Fail |
| Firefox | Latest | ☐ Pass / ☐ Fail |
| Edge | Latest | ☐ Pass / ☐ Fail |
| Safari | Latest | ☐ Pass / ☐ Fail |

**Expected:** Works identically in all modern browsers

---

## Performance Benchmarks

| Model Size | Generation Time | File Size | Browser Load Time |
|------------|----------------|-----------|-------------------|
| 10 events | < 1 second | ~100 KB | < 1 second |
| 50 events | 1-2 seconds | ~200 KB | < 1 second |
| 100 events | 2-3 seconds | ~400 KB | < 2 seconds |
| 200 events | 3-5 seconds | ~800 KB | < 3 seconds |
| 500 events | 5-10 seconds | ~1.5 MB | < 5 seconds |

---

## Error Handling Tests

### Test E1: Read-Only Directory

**Steps:**
1. Try to save flowchart to a read-only location

**Expected:** Error message displayed

---

### Test E2: Invalid Filename

**Steps:**
1. Try to save with invalid characters in filename

**Expected:** System dialog prevents invalid name

---

### Test E3: Disk Full

**Steps:**
1. Try to save when disk is full (if possible)

**Expected:** Error message about disk space

---

### Test E4: Database Corruption

**Steps:**
1. Load corrupted database
2. Try to generate flowchart

**Expected:** Error message or empty flowchart

---

## Regression Testing

Verify these existing features still work:

- ☐ Load model from file
- ☐ Generate HTML report
- ☐ Generate PDF report
- ☐ Save model
- ☐ Other Model menu items function correctly
- ☐ No crashes or memory leaks

---

## Translation Testing (if applicable)

If translations are set up:

### Spanish (es)
- ☐ "Generar flujograma..."
- ☐ "Guardar flujograma"
- ☐ "Flujograma generado"
- ☐ "Abrir flujograma"
- ☐ All dialog messages in Spanish

### English (en)
- ☐ Menu item translated
- ☐ Dialogs translated
- ☐ Messages translated

---

## Console Output Verification

During testing, monitor console for:

- ☐ No error messages
- ☐ No warning messages
- ☐ No memory leaks reported
- ☐ Database queries execute successfully

---

## Accessibility Testing

- ☐ Keyboard navigation works (Tab, Enter, Esc)
- ☐ Screen reader compatible (if possible)
- ☐ High contrast mode compatible
- ☐ Text is readable at different zoom levels

---

## Mobile/Responsive Testing

Open generated HTML on mobile devices or narrow browser windows:

- ☐ Horizontal scrolling enabled for narrow screens
- ☐ Minimum width enforced (800px)
- ☐ Content remains readable
- ☐ Interactive features still work

---

## Security Considerations

- ☐ No SQL injection vulnerabilities (using prepared statements)
- ☐ HTML output is properly escaped
- ☐ No XSS vulnerabilities in generated HTML
- ☐ File paths are validated

---

## Test Summary Template

```
Date: _______________
Tester: _____________
MARRTA Version: _____
Qt Version: _________

Tests Passed: ____ / 20
Tests Failed: ____
Critical Issues: ____
Minor Issues: ____

Overall Status: ☐ Pass ☐ Fail ☐ Needs Work

Notes:
_______________________________________
_______________________________________
_______________________________________
```

---

## Bug Report Template

If issues are found:

```markdown
### Bug Report

**Test Case:** Test #__
**Severity:** Critical / High / Medium / Low
**Description:** 
[Describe the issue]

**Steps to Reproduce:**
1. 
2. 
3. 

**Expected Behavior:**
[What should happen]

**Actual Behavior:**
[What actually happens]

**Environment:**
- OS: 
- Qt Version: 
- MARRTA Version:
- Browser (if applicable):

**Screenshots/Logs:**
[Attach if available]
```

---

## Sign-Off

Once all tests pass:

```
Testing completed by: _______________
Date: _______________
Approved for merge: ☐ Yes ☐ No

Comments:
_______________________________________
_______________________________________
```

---

## Quick Test Script (Bash)

Save as `test_flowchart.sh`:

```bash
#!/bin/bash

echo "=== MARRTA Flowchart Generator Test Script ==="
echo ""

# Check if MARRTA executable exists
if [ ! -f "./marrta" ]; then
    echo "❌ MARRTA executable not found"
    exit 1
fi
echo "✓ MARRTA executable found"

# Check if flujogramagenerator files exist
if [ ! -f "flujogramagenerator.cpp" ] || [ ! -f "flujogramagenerator.h" ]; then
    echo "❌ Flowchart generator files not found"
    exit 1
fi
echo "✓ Flowchart generator files found"

# Check if files are in .pro
if ! grep -q "flujogramagenerator" marrta.pro; then
    echo "❌ Flowchart generator not in marrta.pro"
    exit 1
fi
echo "✓ Flowchart generator in build configuration"

# Check if action is in mainwindow.ui
if ! grep -q "actionGenerarFlujograma" mainwindow.ui; then
    echo "❌ Menu action not in mainwindow.ui"
    exit 1
fi
echo "✓ Menu action defined"

echo ""
echo "=== Pre-flight checks passed ==="
echo "Ready for manual testing in Qt environment"
```

---

## Additional Resources

- Feature Documentation: `Documentation/Support/FLOWCHART_GENERATOR_FEATURE.md`
- Visual Guide: `Documentation/Support/FLOWCHART_GENERATOR_VISUAL_GUIDE.md`
- Implementation Summary: `Documentation/Support/FLOWCHART_IMPLEMENTATION_SUMMARY.md`
- Release Notes: `RELEASE_INFO.md`

---

## Contact

For issues or questions about testing, refer to the implementation documentation or contact the development team.
