# Testing Guide for License and Disclaimer Feature

## Quick Test Checklist

### ✅ Test 1: First-Run Disclaimer Acceptance
**Purpose**: Verify disclaimer appears on first run and acceptance is saved

**Steps**:
1. Backup existing `marrta.ini` file
2. Delete or rename `marrta.ini` (creates first-run scenario)
3. Launch MARRTA application
4. **Expected**: Disclaimer dialog appears immediately before main window
5. Verify dialog shows:
   - Title: "Descargo de Responsabilidad / Disclaimer"
   - Full disclaimer text with all sections visible
   - Two buttons: "Acepto" and "No acepto"
6. Click "No acepto"
7. **Expected**: Application exits immediately
8. Verify `marrta.ini` was not created (no acceptance)

### ✅ Test 2: Disclaimer Rejection Behavior
**Purpose**: Ensure application exits cleanly on rejection

**Steps**:
1. Delete `marrta.ini` again
2. Launch MARRTA
3. **Expected**: Disclaimer dialog appears
4. Click "No acepto" button
5. **Expected**: 
   - Application closes immediately
   - No main window appears
   - Clean exit (no crash, no errors)

### ✅ Test 3: Disclaimer Acceptance and Persistence
**Purpose**: Verify acceptance is saved and not shown again

**Steps**:
1. Delete `marrta.ini` again
2. Launch MARRTA
3. **Expected**: Disclaimer dialog appears
4. Click "Acepto" button
5. **Expected**: 
   - Main window appears
   - Application continues normally
6. Close MARRTA completely
7. Check `marrta.ini` file contents
8. **Expected**: `disclaimerAccepted=true` entry exists
9. Launch MARRTA again
10. **Expected**: Disclaimer does NOT appear (goes directly to main window)

### ✅ Test 4: Menu Access to Disclaimer
**Purpose**: Users can view disclaimer anytime from menu

**Steps**:
1. Launch MARRTA (with disclaimer already accepted)
2. Navigate to menu: **Help → Aviso legal**
3. **Expected**: Disclaimer dialog opens
4. Verify:
   - Same disclaimer content as first-run
   - Buttons are "OK" and "Cancel" (not "Acepto"/"No acepto")
   - Dialog is informational only
5. Click OK or Cancel
6. **Expected**: Dialog closes, application continues

### ✅ Test 5: License Dialog Display
**Purpose**: License summary displays correctly with link button

**Steps**:
1. Launch MARRTA
2. Navigate to menu: **Help → Licencia de uso**
3. **Expected**: License dialog opens showing:
   - Title: "Licencia de uso - GPLv3"
   - GPLv3 summary text with all sections
   - Button: "Ver licencia completa en [LANGUAGE]"
   - Close button
4. Verify all text sections are visible and properly formatted
5. Click Close
6. **Expected**: Dialog closes normally

### ✅ Test 6: License Link - Spanish
**Purpose**: Verify Spanish license link opens correctly

**Steps**:
1. Verify `marrta.ini` has `language=es` (or set it)
2. Launch MARRTA
3. Open **Help → Licencia de uso**
4. Verify button text: "Ver licencia completa en ESPAÑOL"
5. Click the button
6. **Expected**: 
   - Default browser opens
   - URL: `https://www.gnu.org/licenses/gpl-3.0.es.html`
   - GNU GPL v3 page in Spanish loads

### ✅ Test 7: License Link - English
**Purpose**: Verify English license link opens correctly

**Steps**:
1. Edit `marrta.ini` to set `language=en`
2. Restart MARRTA
3. Open **Help → Licencia de uso**
4. Verify button text: "Ver licencia completa en ENGLISH"
5. Click the button
6. **Expected**: 
   - Browser opens
   - URL: `https://www.gnu.org/licenses/gpl-3.0.en.html`
   - GNU GPL v3 page in English loads

### ✅ Test 8: License Link - French
**Purpose**: Verify French license link opens correctly

**Steps**:
1. Edit `marrta.ini` to set `language=fr`
2. Restart MARRTA
3. Open **Help → Licencia de uso**
4. Verify button text: "Ver licencia completa en FRANÇAIS"
5. Click the button
6. **Expected**: 
   - Browser opens
   - URL: `https://www.gnu.org/licenses/gpl-3.0.fr.html`
   - GNU GPL v3 page in French loads

### ✅ Test 9: License Link - Portuguese (Fallback)
**Purpose**: Verify Portuguese falls back to English

**Steps**:
1. Edit `marrta.ini` to set `language=pt`
2. Restart MARRTA
3. Open **Help → Licencia de uso**
4. Verify button text: "Ver licencia completa en PORTUGUÊS"
5. Click the button
6. **Expected**: 
   - Browser opens
   - URL: `https://www.gnu.org/licenses/gpl-3.0.en.html` (English, as fallback)
   - GNU GPL v3 page in English loads

### ✅ Test 10: HTML Formatting
**Purpose**: Verify text displays with proper formatting

**Steps**:
1. Open disclaimer dialog (menu or first-run)
2. Verify formatting:
   - Headers are larger/bold (h2, h3 tags)
   - Lists show bullet points
   - Bold text (`<strong>`) is emphasized
   - Italic text (`<em>`) is styled
   - Text is readable and properly spaced
3. Scroll through entire text
4. Repeat for license dialog

### ✅ Test 11: Settings Integration
**Purpose**: Verify setting persists correctly across sessions

**Steps**:
1. Delete `marrta.ini`
2. Launch MARRTA, accept disclaimer
3. Make note of when disclaimer was accepted
4. Close MARRTA
5. Open `marrta.ini` in text editor
6. Verify:
   - `disclaimerAccepted=true` exists
   - Other settings present (debugMode, language, etc.)
7. Close file without changes
8. Launch MARRTA multiple times
9. **Expected**: Disclaimer never shows again (until setting deleted)

### ✅ Test 12: Dialog Modal Behavior
**Purpose**: Ensure dialogs block interaction properly

**Steps**:
1. On first-run, verify:
   - Disclaimer dialog appears before main window
   - Cannot interact with anything else
   - Must click Acepto or No acepto
2. From menu (after acceptance):
   - Disclaimer opens as modal
   - Main window is blocked while open
   - Cannot interact with main window until closed
3. Repeat for license dialog

## Platform-Specific Tests

### Windows
- Verify `marrta.ini` location: `<app_dir>/marrta.ini`
- Test file deletion requires administrator rights or file unlock
- Verify browser opens correctly (Edge, Chrome, Firefox, etc.)

### macOS
- Verify `marrta.ini` location: `<app.bundle>/Contents/../../marrta.ini`
- Test with different browsers (Safari, Chrome, Firefox)
- Verify dialog rendering on Retina displays

### Linux
- Verify `marrta.ini` location: `<app_dir>/marrta.ini`
- Test with different desktop environments (GNOME, KDE, XFCE)
- Verify browser opens correctly (Firefox, Chrome, etc.)

## Translation Tests

### Pre-requisites
Run translation update:
```bash
cd /home/runner/work/MARRTA/MARRTA
lupdate marrta.pro
# Open marrta_es.ts and marrta_en.ts in Qt Linguist
# Translate any new strings
lrelease marrta.pro
```

### Translation Verification
1. Verify all `tr()` strings appear in .ts files
2. Check translations are correct and complete
3. Test with language switching
4. Verify dynamic text (e.g., button labels) updates correctly

## Edge Cases and Error Conditions

### Test: Corrupted Settings File
1. Create invalid `marrta.ini` with corrupted content
2. Launch MARRTA
3. **Expected**: Treats as first-run, shows disclaimer

### Test: Read-Only Settings File
1. Accept disclaimer
2. Make `marrta.ini` read-only
3. Delete the `disclaimerAccepted` line
4. Launch MARRTA, accept disclaimer
5. **Expected**: May show error or continue (graceful handling)

### Test: Missing Browser
1. Test on system with no default browser set
2. Click license link
3. **Expected**: Error message or OS picker appears

### Test: Long Text Display
1. Verify disclaimer text doesn't overflow dialog
2. Check scrollbars appear and work
3. Resize dialog (if resizable) and verify reflow

## Performance Tests

### Startup Time Impact
1. Measure startup time with disclaimer (first-run)
2. Measure startup time without disclaimer (accepted)
3. **Expected**: Minimal difference (<100ms)

### Memory Usage
1. Open both dialogs
2. Monitor memory usage
3. **Expected**: No memory leaks on close

## Regression Tests

### Verify Existing Functionality
1. Ensure other Help menu items still work
2. Verify settings loading/saving for other values
3. Check that first-run doesn't break file opening
4. Ensure command-line arguments still work

## Expected Results Summary

| Test | Expected Outcome |
|------|-----------------|
| First-run | Disclaimer shows with Acepto/No acepto |
| Reject | Application exits cleanly |
| Accept | Main window opens, setting saved |
| Subsequent runs | Disclaimer does not appear |
| Menu access | Disclaimer shows with OK/Cancel |
| License display | GPLv3 summary with link button |
| ES link | Opens Spanish GNU GPL page |
| EN link | Opens English GNU GPL page |
| FR link | Opens French GNU GPL page |
| PT link | Opens English GNU GPL page (fallback) |
| HTML formatting | Text displays with proper styling |
| Settings persist | disclaimerAccepted=true in ini file |

## Known Limitations

1. Portuguese GNU GPL translation not available (falls back to English)
2. Dialog size is fixed (not resizable in current implementation)
3. Button text changes require application restart after language change
4. No version tracking for disclaimer (if text changes, same acceptance applies)

## Troubleshooting

### Disclaimer keeps appearing
- Check `marrta.ini` for `disclaimerAccepted=true`
- Verify file is writable
- Check file location is correct for platform

### License link doesn't open
- Verify internet connection
- Check default browser is set
- Try different language settings

### Text not displaying correctly
- Verify HTML tags are correct
- Check font availability
- Try different display scaling

## Acceptance Criteria

✅ All tests pass  
✅ No crashes or errors  
✅ Settings persist correctly  
✅ Links open in browser  
✅ Text is readable and properly formatted  
✅ First-run behavior works as expected  
✅ Menu access works correctly  
✅ Multi-language support functional  

## Sign-off

**Tested by**: _________________  
**Date**: _________________  
**Platform**: _________________  
**Qt Version**: _________________  
**Result**: ☐ Pass ☐ Fail  
**Notes**: _________________
