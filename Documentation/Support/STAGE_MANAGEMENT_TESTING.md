# Testing Checklist for DlgGestionEtapas

## Overview
This document provides a comprehensive testing checklist for the new stage management dialog.

## Prerequisites
- MARRTA application built successfully
- A PRAXIS database file loaded (or create a new one)
- Access to the "Etapas" menu item in the application

## Test Cases

### Stage Management

#### TC1: Open Dialog
- [ ] Open MARRTA and load a database
- [ ] Click on "Etapas" menu item
- [ ] Verify dialog opens with title "Gestión de Etapas y Subetapas"
- [ ] Verify left panel shows existing stages (if any)
- [ ] Verify right panel shows stage details

#### TC2: Create New Stage - Success
- [ ] Click "Nueva" button in stage section
- [ ] Enter name: "Test Stage 1"
- [ ] Click OK
- [ ] Enter code: "TS1"
- [ ] Click OK
- [ ] Verify stage appears in left list
- [ ] Verify stage details show in right panel
- [ ] Verify substage section is now enabled

#### TC3: Create New Stage - Validation
- [ ] Click "Nueva" button
- [ ] Leave name empty and click OK
- [ ] Verify nothing happens (dialog closes without creating)
- [ ] Click "Nueva" button again
- [ ] Enter name: "Test Stage 2"
- [ ] Click OK
- [ ] Leave code empty and click OK
- [ ] Verify nothing happens
- [ ] Click "Nueva" button again
- [ ] Enter name: "Test Stage 3", click OK
- [ ] Enter code: "TOOLONG"
- [ ] Verify warning message about max 3 characters
- [ ] Enter code: "TS3", click OK
- [ ] Verify stage is created

#### TC4: Edit Stage Name
- [ ] Select a stage from the list
- [ ] Edit the name in the "Nombre" field
- [ ] Verify the list updates immediately
- [ ] Close and reopen dialog
- [ ] Verify the name change persisted

#### TC5: Edit Stage Code
- [ ] Select a stage
- [ ] Edit the code field
- [ ] Try entering more than 3 characters
- [ ] Verify it's limited to 3 characters
- [ ] Enter a valid 2-3 character code
- [ ] Close and reopen dialog
- [ ] Verify the code change persisted

#### TC6: Edit Stage Description
- [ ] Select a stage
- [ ] Enter/edit description text
- [ ] Close and reopen dialog
- [ ] Verify the description persisted

#### TC7: Reorder Stages - Move Up
- [ ] Create at least 3 stages if not already present
- [ ] Select the second stage
- [ ] Verify "Subir" button is enabled
- [ ] Click "Subir" button
- [ ] Verify stage moves up one position
- [ ] Close and reopen dialog
- [ ] Verify order persisted

#### TC8: Reorder Stages - Move Down
- [ ] Select a stage that's not last
- [ ] Verify "Bajar" button is enabled
- [ ] Click "Bajar" button
- [ ] Verify stage moves down one position
- [ ] Close and reopen dialog
- [ ] Verify order persisted

#### TC9: Reorder Stages - Button States
- [ ] Select the first stage
- [ ] Verify "Subir" button is disabled
- [ ] Verify "Bajar" button is enabled (if not last)
- [ ] Select the last stage
- [ ] Verify "Bajar" button is disabled
- [ ] Verify "Subir" button is enabled (if not first)

#### TC10: Delete Stage - Cancel
- [ ] Select a stage
- [ ] Click "Eliminar" button
- [ ] Verify confirmation dialog appears
- [ ] Click "No"
- [ ] Verify stage is not deleted

#### TC11: Delete Stage - Confirm
- [ ] Create a test stage with substages
- [ ] Click "Eliminar" button on the stage
- [ ] Verify confirmation mentions substage deletion
- [ ] Click "Yes"
- [ ] Verify stage is removed from list
- [ ] Close and reopen dialog
- [ ] Verify stage is gone from database

### Substage Management

#### TC12: Substage Section Initial State
- [ ] Open dialog with no stages
- [ ] Verify substage section is disabled
- [ ] Create or select a stage
- [ ] Verify substage section becomes enabled

#### TC13: Create New Substage - Success
- [ ] Select a stage
- [ ] Click "Nueva" button in substage section
- [ ] Enter name: "Test Substage 1"
- [ ] Click OK
- [ ] Verify substage appears in substage list
- [ ] Verify substage details show below
- [ ] Verify "Activa" checkbox is checked by default

#### TC14: Create New Substage - Validation
- [ ] Click "Nueva" button in substage section
- [ ] Leave name empty and click OK
- [ ] Verify nothing happens

#### TC15: Edit Substage Name
- [ ] Select a substage
- [ ] Edit the name in substage details
- [ ] Verify the list updates immediately
- [ ] Close and reopen dialog
- [ ] Verify the name change persisted

#### TC16: Edit Substage Description
- [ ] Select a substage
- [ ] Enter/edit description
- [ ] Close and reopen dialog
- [ ] Verify description persisted

#### TC17: Toggle Substage Active Status
- [ ] Select a substage
- [ ] Verify "Activa" checkbox state
- [ ] Uncheck the checkbox
- [ ] Verify list shows "(Inactiva)" suffix
- [ ] Close and reopen dialog
- [ ] Verify inactive status persisted and suffix shows

#### TC18: Reorder Substages
- [ ] Create at least 3 substages for a stage
- [ ] Select middle substage
- [ ] Click "Subir" button
- [ ] Verify substage moves up
- [ ] Click "Bajar" button twice
- [ ] Verify substage moves down
- [ ] Close and reopen dialog
- [ ] Verify order persisted

#### TC19: Delete Substage
- [ ] Select a substage
- [ ] Click "Eliminar" button
- [ ] Verify confirmation dialog
- [ ] Click "Yes"
- [ ] Verify substage is removed
- [ ] Close and reopen dialog
- [ ] Verify deletion persisted

#### TC20: Switch Between Stages
- [ ] Create 2 stages with different substages
- [ ] Select first stage
- [ ] Note its substages
- [ ] Select second stage
- [ ] Verify substage list changes to second stage's substages
- [ ] Select first stage again
- [ ] Verify substage list changes back

### Integration Tests

#### TC21: Close Dialog and Reload
- [ ] Make various changes to stages/substages
- [ ] Click "Cerrar" button
- [ ] Verify MainWindow reloads (check if any visual update occurs)
- [ ] Reopen dialog
- [ ] Verify all changes persisted

#### TC22: Multiple Edit Session
- [ ] Open dialog
- [ ] Create a stage
- [ ] Close dialog
- [ ] Reopen dialog
- [ ] Add substages to the stage
- [ ] Close dialog
- [ ] Reopen dialog
- [ ] Edit the stage and substages
- [ ] Verify all changes work correctly

#### TC23: Error Handling
- [ ] If possible, simulate database connection issue
- [ ] Verify appropriate error messages appear
- [ ] Verify application doesn't crash

### UI/UX Tests

#### TC24: Splitter Functionality
- [ ] Drag the splitter between left and right panels
- [ ] Verify both panels resize appropriately
- [ ] Make left panel very small
- [ ] Verify stage list still shows scrollbar if needed
- [ ] Make right panel very small
- [ ] Verify detail fields adjust appropriately

#### TC25: Selection Behavior
- [ ] Click on different stages
- [ ] Verify right panel updates for each selection
- [ ] Click on different substages
- [ ] Verify substage details update for each selection

#### TC26: Empty States
- [ ] Delete all stages
- [ ] Verify empty list shows appropriately
- [ ] Verify detail panels clear
- [ ] Create a stage with no substages
- [ ] Verify empty substage list shows appropriately

#### TC27: Visual Feedback
- [ ] Verify alternating row colors in lists
- [ ] Verify selected items are highlighted
- [ ] Verify buttons show icons correctly
- [ ] Verify tooltips if any

### Translation Tests (if multiple languages available)

#### TC28: Spanish (default)
- [ ] Verify all labels are in Spanish
- [ ] Verify all buttons show Spanish text
- [ ] Verify all messages are in Spanish

#### TC29: English (if available)
- [ ] Switch to English language
- [ ] Open dialog
- [ ] Verify all UI elements are translated
- [ ] Verify placeholder texts are translated
- [ ] Verify error messages are translated

## Regression Tests

#### TC30: Old Functionality Still Works
- [ ] Verify other parts of MARRTA still work after changes
- [ ] Check event management
- [ ] Check defense management
- [ ] Check report generation
- [ ] Verify stages show in other parts of UI

## Performance Tests

#### TC31: Large Number of Stages
- [ ] Create 50+ stages
- [ ] Verify list scrolls smoothly
- [ ] Verify selection works quickly
- [ ] Verify reordering is responsive

#### TC32: Large Number of Substages
- [ ] Create a stage with 50+ substages
- [ ] Verify list scrolls smoothly
- [ ] Verify operations are responsive

## Bug Report Template

If you find issues, report with:
- Test case number
- Steps to reproduce
- Expected behavior
- Actual behavior
- Screenshots if applicable
- Database state (number of stages/substages)

## Success Criteria

All test cases should pass with:
- ✓ No crashes
- ✓ No data loss
- ✓ All changes persist correctly
- ✓ UI is responsive
- ✓ Error messages are clear and helpful
- ✓ All text is properly translated
