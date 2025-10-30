# Testing Checklist for Save Crash Bug Fix

## Overview
This checklist covers testing for the bug fix that resolved program crashes after saving the database.

## Prerequisites
- MARRTA application compiled with latest changes
- Sample PRAXIS database file for testing
- Memory profiling tools (optional but recommended)

## Test Scenarios

### 1. Basic Save Functionality
- [ ] Open a PRAXIS file
- [ ] Make some changes (enable/disable defenses)
- [ ] Click Save button (Action → Save Model)
- [ ] Verify save confirmation dialog appears
- [ ] Verify application does NOT crash
- [ ] Verify changes are persisted to database

### 2. Multiple Save Operations
- [ ] Open a PRAXIS file
- [ ] Perform save operation
- [ ] Make more changes
- [ ] Perform save operation again
- [ ] Repeat 5-10 times
- [ ] Verify no crashes occur
- [ ] Verify no memory warnings or slowdowns

### 3. Window Refresh After Save
- [ ] Open a PRAXIS file
- [ ] Select an event
- [ ] Note the current display state
- [ ] Modify some defenses
- [ ] Save the changes
- [ ] Verify the event list refreshes correctly
- [ ] Verify the selected event remains selected (or reselects correctly)
- [ ] Verify all displayed values are updated correctly

### 4. Defense State Changes
- [ ] Open a PRAXIS file
- [ ] Select an event
- [ ] Toggle frequency reducers on/off multiple times
- [ ] Toggle barriers on/off multiple times
- [ ] Toggle consequence reducers on/off multiple times
- [ ] Verify no crashes occur during toggles
- [ ] Verify UI updates correctly after each toggle

### 5. Event Navigation
- [ ] Open a PRAXIS file
- [ ] Click through multiple events (10-20)
- [ ] Verify no crashes occur
- [ ] Verify each event displays correctly
- [ ] Check memory usage doesn't grow excessively

### 6. Stress Testing
- [ ] Open a large PRAXIS file with many events
- [ ] Rapidly switch between events
- [ ] Perform multiple save operations
- [ ] Toggle many defenses
- [ ] Run for extended period (15-30 minutes)
- [ ] Verify stable operation throughout

### 7. Memory Testing (if tools available)
- [ ] Run application under Valgrind or similar tool
- [ ] Perform normal operations including saves
- [ ] Check for memory leaks
- [ ] Verify all allocated memory is freed on exit

## Expected Results

### All Tests Should Show:
✅ No application crashes
✅ Proper memory management (no leaks)
✅ Correct UI updates and refreshes
✅ Stable memory usage over time
✅ Proper data persistence

### Red Flags to Watch For:
❌ Application crashes or freezes
❌ Increasing memory usage over time
❌ UI elements not updating
❌ Lost selections or incorrect displays
❌ Error messages or warnings

## Regression Testing
Ensure these previously working features still work:
- [ ] Opening PRAXIS files
- [ ] Creating new models
- [ ] Exporting to Excel/CSV
- [ ] Generating reports
- [ ] Scripting engine functionality
- [ ] Stage and substage navigation

## Platform Testing
If possible, test on multiple platforms:
- [ ] Windows
- [ ] macOS
- [ ] Linux

## Notes Section
Use this space to record any issues found during testing:

```
Date: ___________
Tester: ___________

Issues Found:
1. 
2. 
3. 

Additional Observations:
-
-
-
```

## Sign-off
- [ ] All critical tests passed
- [ ] No regressions detected
- [ ] Ready for deployment

Tested by: ________________
Date: ________________
