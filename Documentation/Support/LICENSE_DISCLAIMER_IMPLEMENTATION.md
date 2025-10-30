# License and Disclaimer Implementation Summary

## Overview
This implementation adds comprehensive license and disclaimer dialogs to MARRTA, including first-run disclaimer acceptance functionality as requested in the issue.

## Components Implemented

### 1. Disclaimer Dialog (`DlgDisclaimer`)
**Files**: `dlgdisclaimer.h`, `dlgdisclaimer.cpp`, `dlgdisclaimer.ui`

The disclaimer dialog displays a comprehensive medical disclaimer in HTML format covering:
- **Warning**: Software is NOT a medical device
- **Limitations of Use**: Not for clinical decisions, diagnosis, or treatment
- **Software Purpose**: Administrative risk analysis at institutional level
- **User Responsibility**: Users must verify information independently
- **Liability Limitation**: No responsibility for decisions based on software
- **Emergency Notice**: Contact medical professionals for emergencies

**Key Features**:
- Full HTML formatting with headers, lists, and emphasis
- All text wrapped with `tr()` for translation support
- Read-only text browser for safe display
- OK/Cancel button box for acceptance

### 2. License Dialog (`DlgLicense`)
**Files**: `dlglicense.h`, `dlglicense.cpp`, `dlglicense.ui`

The license dialog provides a GPLv3 license summary with:
- **Freedoms**: Use, study, distribution, improvement
- **Copyleft Requirements**: Derivative works must use same license
- **Source Code Obligations**: Must provide source code
- **No Warranties**: Software provided "as is"

**Key Features**:
- GPLv3 summary in clear, understandable language
- Button to open full license in user's language
- Multi-language support (ES, EN, FR, PT) with links to GNU official pages
- Detects current language from settings file
- Uses `QDesktopServices::openUrl()` to open browser

**License URLs by Language**:
- Spanish (es): https://www.gnu.org/licenses/gpl-3.0.es.html
- English (en): https://www.gnu.org/licenses/gpl-3.0.en.html
- French (fr): https://www.gnu.org/licenses/gpl-3.0.fr.html
- Portuguese (pt): https://www.gnu.org/licenses/gpl-3.0.en.html (fallback to English)

### 3. First-Run Acceptance Logic
**Modified**: `main.cpp`, `settings.h`

**Flow**:
1. Application starts and reads settings from `marrta.ini`
2. Checks `disclaimerAccepted` setting (default: false)
3. If not accepted, displays disclaimer dialog BEFORE MainWindow
4. Custom button labels: "Acepto" (I accept) / "No acepto" (I don't accept)
5. If user clicks "No acepto" (Cancel), application exits immediately
6. If user clicks "Acepto" (OK), sets `disclaimerAccepted = true` and continues
7. Setting is saved to `marrta.ini` and persists across sessions

**Settings Integration**:
- Added `disclaimerAccepted` boolean to `Settings` struct
- Integrated into `loadSettings()` and `saveSettings()` in MainWindow
- Uses same settings file as other application preferences

### 4. Menu Action Updates
**Modified**: `mainwindow.cpp`

Updated existing menu actions to use new dialogs:
- **"Aviso legal"** (Legal Notice): Opens `DlgDisclaimer` 
- **"Licencia de uso"** (Usage License): Opens `DlgLicense`

Previous simple QMessageBox implementations were replaced with full dialog implementations for richer content display.

## Translation Support

All user-visible text uses Qt's `tr()` function for internationalization:
- Disclaimer text (full HTML content)
- License summary text (full HTML content)
- Dialog titles
- Button labels
- Dynamic text (e.g., "Ver licencia completa en ES")

**Translation Files**: `marrta_es.ts`, `marrta_en.ts`

To update translations after this implementation:
```bash
lupdate marrta.pro
# Edit .ts files with Qt Linguist if needed
lrelease marrta.pro
```

## Testing Instructions

### Test 1: First-Run Experience
1. Delete or rename `marrta.ini` file (location depends on OS)
2. Launch MARRTA
3. **Expected**: Disclaimer dialog appears immediately
4. **Test**: Click "No acepto" → Application should exit
5. Launch MARRTA again (with deleted/renamed ini)
6. **Test**: Click "Acepto" → Application should continue to MainWindow
7. Close and relaunch MARRTA
8. **Expected**: Disclaimer should NOT appear (acceptance was saved)

### Test 2: Disclaimer Dialog from Menu
1. Open MARRTA (with disclaimer already accepted)
2. Navigate to: Help menu → "Aviso legal"
3. **Expected**: Disclaimer dialog opens with full text
4. **Verify**: Text is displayed properly with HTML formatting
5. **Verify**: Text is scrollable if needed
6. Click OK to close

### Test 3: License Dialog from Menu
1. Open MARRTA
2. Navigate to: Help menu → "Licencia de uso"
3. **Expected**: License dialog opens with GPLv3 summary
4. **Verify**: Text is displayed properly with HTML formatting
5. Click "Ver licencia completa en [LANG]" button
6. **Expected**: Browser opens with GNU GPL page in correct language
7. **Verify**: URL matches configured language (es/en/fr/pt)

### Test 4: Multi-Language Support
1. Change language setting in `marrta.ini` to different values:
   - `language=es` → Should use Spanish license URL
   - `language=en` → Should use English license URL
   - `language=fr` → Should use French license URL
   - `language=pt` → Should use English license URL (fallback)
2. Open license dialog and verify button text updates
3. Click button and verify correct language URL opens

### Test 5: Settings Persistence
1. Check `marrta.ini` file after accepting disclaimer
2. **Expected**: `disclaimerAccepted=true` entry exists
3. Manually edit file to set `disclaimerAccepted=false`
4. Launch MARRTA
5. **Expected**: Disclaimer appears again

## Technical Notes

### Button Customization in First-Run
The first-run implementation customizes the standard QDialogButtonBox buttons:
```cpp
QDialogButtonBox *buttonBox = disclaimerDlg.findChild<QDialogButtonBox*>("buttonBox");
QPushButton *acceptButton = buttonBox->button(QDialogButtonBox::Ok);
acceptButton->setText(QObject::tr("Acepto"));
```

This provides clearer, more explicit labels than the default "OK"/"Cancel".

### Settings File Location
Settings are stored in platform-specific locations:
- **macOS**: `<app_dir>/../../../marrta.ini`
- **Other platforms**: `<app_dir>/marrta.ini`

### QDesktopServices for External Links
The license dialog uses Qt's `QDesktopServices::openUrl()` to open the full license in the default browser. This is cross-platform and respects user's default browser setting.

### HTML Content in Dialogs
Both dialogs use `QTextBrowser` widgets which support a subset of HTML:
- Headers (`<h2>`, `<h3>`)
- Paragraphs (`<p>`)
- Lists (`<ul>`, `<li>`)
- Bold (`<strong>`)
- Emphasis (`<em>`)

This provides rich formatting without requiring a full web engine.

## Files Modified

### New Files (6)
- `dlgdisclaimer.h` - Disclaimer dialog header
- `dlgdisclaimer.cpp` - Disclaimer dialog implementation
- `dlgdisclaimer.ui` - Disclaimer dialog UI definition
- `dlglicense.h` - License dialog header
- `dlglicense.cpp` - License dialog implementation  
- `dlglicense.ui` - License dialog UI definition

### Modified Files (5)
- `settings.h` - Added disclaimerAccepted field
- `main.cpp` - Added first-run check and disclaimer acceptance logic
- `mainwindow.cpp` - Updated action handlers, added settings integration
- `marrta.pro` - Added new files to build configuration
- `RELEASE_INFO.md` - Documented changes for release notes

## Compliance with Requirements

✅ **Disclaimer text**: Full text from issue included with all sections  
✅ **License summary**: GPLv3 explained in clear terms  
✅ **Multi-language links**: ES, EN, FR, PT supported  
✅ **First-run acceptance**: Implemented with "Acepto"/"No acepto" buttons  
✅ **Exit on rejection**: Application exits if user rejects disclaimer  
✅ **Translatable**: All text uses tr() for Qt translation system  
✅ **Menu integration**: Both actions updated to use new dialogs  
✅ **Settings persistence**: Acceptance tracked across sessions  

## Future Enhancements (Optional)

1. **Portuguese License Translation**: Add link to Portuguese translation if GNU publishes one
2. **Rich Text Formatting**: Consider using QWebEngineView for more advanced formatting
3. **Accept Checkbox**: Add "Don't show this again" checkbox to menu-triggered disclaimer
4. **Version Tracking**: Track disclaimer version to re-show if disclaimer text changes
5. **Analytics**: Log acceptance rate (if privacy policy allows)

## Conclusion

This implementation provides a robust, user-friendly system for displaying legal disclaimers and license information, with special attention to first-run acceptance and multi-language support as required by the issue.
