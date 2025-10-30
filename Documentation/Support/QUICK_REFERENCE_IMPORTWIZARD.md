# Quick Reference - ImportWizard Optimization

## What Was Changed?

**Issue:** ImportWizard didn't save the current database before importing and didn't load the new database automatically.

**Solution:** Made ImportWizard behave exactly like NewModelWizard.

## Files Modified

| File | Changes | Lines |
|------|---------|-------|
| `mainwindow.cpp` | Added DB check & auto-load | +24 |
| `importwizard.h` | Added slot declaration | +1 |
| `importwizard.cpp` | Changed button & logic | +13 |
| **Total** | | **+36/-4** |

## Key Code Changes

### 1. mainwindow.cpp - Database Check

```cpp
void MainWindow::on_actionImportarExcel_triggered()
{
    // NEW: Check if database is open
    if (praxisDb.isOpen() && !settings.lastFile.isEmpty()) {
        // Show Save/Discard/Cancel dialog
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Base de datos abierta"));
        msgBox.setText(tr("Hay una base de datos abierta actualmente."));
        msgBox.setInformativeText(tr("¿Desea guardar los cambios antes de importar desde Excel?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        
        int ret = msgBox.exec();
        if (ret == QMessageBox::Save) {
            saveModel();
        } else if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    
    // NEW: Check acceptance and load automatically
    ImportWizard wizard(this);
    if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
        openPraxisFile(wizard.getPdbFile());
    }
}
```

### 2. importwizard.cpp - Button Text

```cpp
// In updateButtons():
if (cancelButton) {
    if (currentStep == 3 && conversionSuccessful) {
        cancelButton->setText(tr("Terminar"));  // Changed from "Cerrar"
    } else {
        cancelButton->setText(tr("Cancelar"));
    }
}
```

### 3. importwizard.cpp - Accept/Reject Logic

```cpp
// NEW function:
void ImportWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && conversionSuccessful) {
        accept();  // Accept when successful
    } else {
        reject();  // Reject when cancelled
    }
}
```

## User Experience

### Before
1. Click "Importar desde Excel"
2. Complete wizard
3. Click "Cerrar"
4. Manually open the .pdb file
5. **Problem:** Unsaved changes lost without warning

### After
1. Click "Importar desde Excel"
2. **Warning dialog** if database is open (Save/Discard/Cancel)
3. Complete wizard
4. Click **"Terminar"**
5. **New database loads automatically**
6. ✅ Changes protected, automatic loading

## Quick Test

```bash
# Test Case 1: Database open
1. Open a database
2. Go to File → Import from Excel
3. Should see warning dialog
4. Choose "Save" or "Discard"
5. Complete import
6. Click "Terminar"
7. New database should load automatically

# Test Case 2: No database open
1. Start without database
2. Go to File → Import from Excel
3. No warning dialog
4. Complete import
5. Click "Terminar"
6. New database should load automatically
```

## Pattern Used

This is now the **standard pattern** for all wizards that create/import models:

```cpp
// In MainWindow action handler:
if (praxisDb.isOpen() && !settings.lastFile.isEmpty()) {
    // Show Save/Discard/Cancel dialog
    // ...
}

MyWizard wizard(this);
if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
    openPraxisFile(wizard.getPdbFile());
}
```

```cpp
// In Wizard cancel button handler:
void MyWizard::on_cancelButtonClicked()
{
    if (lastStep && successFlag) {
        accept();  // Success
    } else {
        reject();  // Cancel
    }
}
```

## Documentation Files

1. **IMPORT_WIZARD_OPTIMIZATION.md** - Detailed implementation
2. **IMPORT_WIZARD_FLOW_COMPARISON.md** - Before/after flows
3. **ISSUE_IMPORTWIZARD_SUMMARY.md** - Complete summary
4. **IMPORT_WIZARD_VISUAL_GUIDE.md** - Visual diagrams
5. **QUICK_REFERENCE_IMPORTWIZARD.md** - This file

## Comparison with NewModelWizard

| Feature | NewModelWizard | ImportWizard |
|---------|----------------|--------------|
| DB check | ✅ | ✅ |
| Save dialog | ✅ | ✅ |
| "Terminar" button | ✅ | ✅ |
| Auto-load | ✅ | ✅ |
| accept() on success | ✅ | ✅ |

**Result:** Identical behavior ✅

## Benefits Summary

- ✅ **Data Protection**: Warns before losing changes
- ✅ **User Experience**: Automatic loading, no manual steps
- ✅ **Consistency**: Same as NewModelWizard
- ✅ **Clarity**: "Terminar" indicates success clearly

## Implementation Status

✅ **COMPLETE**
- Code changes: Done
- Documentation: Done
- Ready for Qt testing

## Related Files

Reference implementation in:
- `FEEDBACK_CHANGES.md` (NewModelWizard pattern)
- `newmodelwizard.cpp` (reference code)
- `newmodelwizard.h` (reference headers)

---

**Created:** 2025-10-08  
**Issue:** [Optimización] Guardar base de datos antes de importar un excel y cargar la nueva al terminar  
**Status:** ✅ Implemented
