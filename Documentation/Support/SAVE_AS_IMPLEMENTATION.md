# Save As Implementation Summary

## Overview
This document describes the implementation of the "Guardar como..." (Save As) functionality for MARRTA database files (.pdb).

## Problem Statement
The "Guardar como..." menu option in MARRTA was defined in the UI but had no implementation. The action was connected in the UI file but the corresponding slot handler was missing, making the menu option non-functional.

## Solution

### Implementation Details

A new slot handler `on_actionGuardar_como_triggered()` was implemented to handle the Save As functionality. The implementation follows these steps:

1. **Validation**: Checks if a database is currently open
   - If no database is open, shows a warning using `showNoModelLoadedWarning()`
   - Returns early if validation fails

2. **Save Current Changes**: 
   - Saves any pending changes to the current model using `Database::DbEvent::checkAndUpdate(events)`
   - This ensures the copied file includes all recent modifications

3. **File Selection Dialog**:
   - Displays a save file dialog using `QFileDialog::getSaveFileName()`
   - Dialog title: "Guardar modelo como" (translatable)
   - File filter: "Modelo MARRTA (*.pdb)"
   - Default location: `settings.databaseFolder`
   - Returns early if user cancels the dialog

4. **Extension Validation**:
   - Ensures the selected filename has the `.pdb` extension
   - Automatically appends `.pdb` if not present

5. **Overwrite Handling**:
   - Checks if the target file already exists using `QFile::exists()`
   - If it exists:
     - Prompts user for confirmation with `QMessageBox::question()`
     - Returns early if user chooses not to overwrite
     - Removes the existing file using `QFile::remove()`
     - Shows error message if removal fails

6. **File Copy Operation**:
   - Copies the current database file to the new location using `QFile::copy(settings.lastFile, newFileName)`
   - Shows success message with the new file path on success
   - Shows error message on failure

### Key Features

- **Non-destructive**: Does not modify the original file - creates a copy instead
- **User-friendly**: Clear prompts and confirmation dialogs
- **Error handling**: Comprehensive error checking at each step
- **Internationalization**: All user messages use `tr()` for translation support
- **Consistent UX**: Follows existing patterns in the codebase for file operations

### Files Modified

1. **mainwindow.h**
   - Added slot declaration: `void on_actionGuardar_como_triggered();`

2. **mainwindow.cpp**
   - Implemented the complete Save As functionality (71 lines)
   - Uses existing utilities: QFileDialog, QMessageBox, QFile

3. **RELEASE_INFO.md**
   - Added documentation of the changes for future release notes

## Usage

1. Open a MARRTA model (.pdb file)
2. Click on "Archivo" menu → "Guardar como..."
3. Select destination folder and enter filename
4. Click Save
5. Confirm overwrite if file exists
6. Success message shows the saved location

## Technical Notes

- The implementation uses Qt's automatic signal-slot connection based on naming convention
- The action `actionGuardar_como` in mainwindow.ui is automatically connected to `on_actionGuardar_como_triggered()`
- All necessary Qt classes (QFileDialog, QMessageBox, QFile) were already included in mainwindow.cpp
- The implementation follows the project's coding standards, including proper use of `tr()` for all UI strings

## Testing Recommendations

Manual testing should verify:
1. Save As works when a model is loaded
2. Warning appears when no model is loaded
3. File dialog allows selecting location and filename
4. .pdb extension is automatically added if omitted
5. Overwrite confirmation works correctly
6. Success/error messages display appropriately
7. Original file remains unchanged after Save As
8. New file is a working copy of the original model
