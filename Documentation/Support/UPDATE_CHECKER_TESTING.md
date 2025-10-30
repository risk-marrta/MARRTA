# Testing Guide for Update Checker Feature

## Pre-requisites

- MARRTA must be compiled and running
- Internet connection required for GitHub API access
- Access to `marrta.ini` configuration file

## Test Cases

### Test 1: Manual Version Check - New Version Available

**Setup:**
1. Ensure a newer version exists in GitHub releases (or temporarily modify APP_VERSION to an older version like "1.0.0")

**Steps:**
1. Launch MARRTA
2. Go to menu: **Ayuda → Comprobar actualizaciones...**
3. Wait for the dialog to appear

**Expected Result:**
- Dialog should show "Nueva versión disponible"
- Should display the new version number
- Should show current version
- Should offer to open the release page

**Verification:**
- [ ] Dialog appears with correct title
- [ ] New version number is displayed
- [ ] Current version is displayed
- [ ] Botón "Sí" abre el navegador en la página de lanzamientos de GitHub
- [ ] Botón "No" cierra el diálogo sin realizar ninguna acción

---

### Test 2: Manual Version Check - No New Version

**Setup:**
1. Ensure APP_VERSION matches the latest release in GitHub

**Steps:**
1. Launch MARRTA
2. Go to menu: **Ayuda → Comprobar actualizaciones...**
3. Wait for the dialog to appear

**Expected Result:**
- Dialog should show "Sin actualizaciones"
- Should indicate user has the latest version

**Verification:**
- [ ] Dialog appears with correct message
- [ ] Current version is displayed
- [ ] Dialog closes after clicking OK

---

### Test 3: Manual Models Check - New Models Available

**Setup:**
1. Clear or set an old date for `lastModelsCheck` in `marrta.ini`
2. Ensure there are releases in the models repository

**Steps:**
1. Launch MARRTA
2. Go to menu: **Ayuda → Comprobar nuevos modelos...**
3. Wait for the dialog to appear

**Expected Result:**
- Dialog should show "Nuevos modelos disponibles"
- Should list new models (up to 5)
- Should offer to open the models repository

**Verification:**
- [ ] Dialog appears with correct title
- [ ] Model names are listed
- [ ] If more than 5 models, shows "... y X más"
- [ ] "Yes" button opens browser to models repository
- [ ] "No" button closes dialog without action
- [ ] `lastModelsCheck` in `marrta.ini` is updated after check

---

### Test 4: Manual Models Check - No New Models

**Setup:**
1. Perform a models check once to update `lastModelsCheck`
2. Ensure no new models have been released since

**Steps:**
1. Launch MARRTA
2. Go to menu: **Ayuda → Comprobar nuevos modelos...**
3. Wait for the dialog to appear

**Expected Result:**
- Dialog should show "Sin nuevos modelos"
- Should indicate no new models since last check

**Verification:**
- [ ] Dialog appears with correct message
- [ ] Dialog closes after clicking OK

---

### Test 5: Automatic Check on Startup - First Time

**Setup:**
1. Delete or clear `lastVersionCheck` and `lastModelsCheck` from `marrta.ini`
2. Ensure there are updates/models available

**Steps:**
1. Launch MARRTA
2. Wait a few seconds for background checks

**Expected Result:**
- If new version: dialog appears about new version
- If new models: dialog appears about new models
- Checks should happen silently in background

**Verification:**
- [ ] Application starts normally
- [ ] Dialogs appear only if updates/models are found
- [ ] `lastVersionCheck` appears in `marrta.ini` after launch
- [ ] `lastModelsCheck` appears in `marrta.ini` after launch

---

### Test 6: Automatic Check on Startup - Within 2 Weeks

**Setup:**
1. Set `lastVersionCheck` and `lastModelsCheck` to a recent date (< 14 days ago)

**Steps:**
1. Launch MARRTA
2. Wait a few seconds

**Expected Result:**
- No automatic checks should be performed
- No dialogs should appear
- Application starts normally

**Verification:**
- [ ] No update dialogs appear
- [ ] Application starts normally
- [ ] Dates in `marrta.ini` remain unchanged

---

### Test 7: Automatic Check on Startup - After 2 Weeks

**Setup:**
1. Set `lastVersionCheck` and `lastModelsCheck` to a date > 14 days ago

**Steps:**
1. Launch MARRTA
2. Wait a few seconds for background checks

**Expected Result:**
- Automatic checks should be performed
- If updates/models found, dialogs appear
- Dates in `marrta.ini` are updated

**Verification:**
- [ ] Checks are performed (visible in debug output if enabled)
- [ ] Dialogs appear only if updates/models are found
- [ ] Dates in `marrta.ini` are updated to current date

---

### Test 8: Network Error Handling - Manual Check

**Setup:**
1. Disconnect from internet or block GitHub access

**Steps:**
1. Launch MARRTA
2. Go to menu: **Ayuda → Comprobar actualizaciones...**
3. Wait for error

**Expected Result:**
- Error dialog should appear
- Should show descriptive error message

**Verification:**
- [ ] Error dialog appears
- [ ] Error message is clear and in Spanish
- [ ] HTTP status code displayed (if available)
- [ ] Dialog closes after clicking OK
- [ ] Application remains stable

**Note:** Ver **TROUBLESHOOTING_GITHUB_API.md** para diagnóstico detallado de problemas de red.

---

### Test 9: Network Error Handling - Automatic Check

**Setup:**
1. Disconnect from internet or block GitHub access
2. Set check dates to trigger automatic check

**Steps:**
1. Launch MARRTA
2. Application starts

**Expected Result:**
- No error dialogs (silent failure)
- Application starts normally
- Errors logged to debug output

**Verification:**
- [ ] No error dialogs appear
- [ ] Application starts normally
- [ ] If verbose mode enabled, errors appear in console

---

### Test 10: Version Comparison Logic

**Test Cases:**

| Current Version      | GitHub Version | Expected Result |
|---------------------|----------------|-----------------|
| 1.4.4 (2025-10-07) | 1.4.5         | Newer available |
| 1.4.4 (2025-10-07) | 1.5.0         | Newer available |
| 1.4.4 (2025-10-07) | 2.0.0         | Newer available |
| 1.4.4 (2025-10-07) | 1.4.4         | No update       |
| 1.4.4 (2025-10-07) | 1.4.3         | No update       |
| 1.4.4 (2025-10-07) | v1.4.5        | Newer available |

**Steps:**
These can be tested by temporarily modifying APP_VERSION in mainwindow.h

**Verification:**
- [ ] Major version increments detected
- [ ] Minor version increments detected
- [ ] Patch version increments detected
- [ ] Equal versions correctly identified
- [ ] Older versions correctly identified
- [ ] 'v' prefix in tags handled correctly

---

## Configuration File (marrta.ini)

After running checks, verify `marrta.ini` contains:

```ini
lastVersionCheck=<ISO DateTime>
lastModelsCheck=<ISO DateTime>
```

**Example:**
```ini
lastVersionCheck=2025-01-15T10:30:45
lastModelsCheck=2025-01-15T10:30:46
```

---

## Debug Output

When running with verbose mode (--verboso flag), you should see debug output like:

```
Checking for updates... (user requested)
Latest version from GitHub: 1.4.5 Current: 1.4.4 (2025-10-07)
New version available: 1.4.5
Updated lastVersionCheck to <datetime>
```

---

## Summary Checklist

### Functionality
- [ ] Manual version check works
- [ ] Manual models check works
- [ ] Automatic checks work on first startup
- [ ] Automatic checks respect 2-week interval
- [ ] Version comparison logic is correct
- [ ] Settings are persisted correctly

### User Experience
- [ ] Dialogs are in Spanish
- [ ] Messages are clear and informative
- [ ] GitHub pages open correctly in browser
- [ ] No crashes or freezes during checks

### Error Handling
- [ ] Network errors handled gracefully
- [ ] Invalid responses handled
- [ ] Silent checks don't show errors
- [ ] Manual checks show error details

### Integration
- [ ] Menu items appear in Help menu
- [ ] Icons display correctly (if theme supports them)
- [ ] No conflicts with existing functionality
- [ ] Application starts normally with feature enabled

---

## Notes

- Tests should be performed on different platforms (Windows, macOS, Linux) if possible
- Check console output with verbose mode for debugging information
- Verify that the feature doesn't slow down application startup significantly
- Test with different network conditions (fast, slow, offline)
