# DevTools Manual Testing Guide

## Prerequisites

- MARRTA application built with Qt 6.10 and QtWebEngine support
- Access to report generation functionality
- Sample data/model loaded in the application

## Test Scenarios

### Test 1: DevTools Button Visibility
**Objective**: Verify the DevTools button is visible in the report viewer toolbar

**Steps**:
1. Launch MARRTA application
2. Navigate to any report viewer (generate a report)
3. Observe the toolbar at the bottom of the report viewer window

**Expected Result**: 
- DevTools button should be visible in the toolbar
- Button should be positioned between the spacer and XML button
- Button should display "DevTools" text

**Status**: ⬜ Not tested

---

### Test 2: DevTools Window Opens
**Objective**: Verify clicking the DevTools button opens the inspector window

**Steps**:
1. Open a report in the report viewer
2. Click the "DevTools" button in the toolbar
3. Observe the new window that opens

**Expected Result**:
- A new dialog window titled "Herramientas de desarrollo" should open
- Window size should be 1200x800 pixels
- Window should contain the Chromium DevTools interface

**Status**: ⬜ Not tested

---

### Test 3: DevTools Functionality - Elements Inspector
**Objective**: Verify HTML/CSS inspection works correctly

**Steps**:
1. Open DevTools window (Test 2)
2. Click on the "Elements" tab (if not already selected)
3. Use the element selector tool
4. Click on various elements in the report viewer

**Expected Result**:
- Selected elements should be highlighted in the report viewer
- Element HTML should be visible in the DevTools inspector
- CSS styles should be displayed in the styles pane
- Able to modify CSS live and see changes in report viewer

**Status**: ⬜ Not tested

---

### Test 4: DevTools Functionality - Console
**Objective**: Verify JavaScript console works correctly

**Steps**:
1. Open DevTools window
2. Click on the "Console" tab
3. Type a JavaScript command (e.g., `document.title`)
4. Press Enter

**Expected Result**:
- Console should execute the command
- Results should be displayed in the console
- Any JavaScript errors from the report should be visible

**Status**: ⬜ Not tested

---

### Test 5: DevTools Functionality - Network Monitor
**Objective**: Verify network requests can be monitored

**Steps**:
1. Open DevTools window
2. Click on the "Network" tab
3. Reload the report or trigger a new report generation
4. Observe the network tab

**Expected Result**:
- Network requests should be visible (if any)
- Resource loading should be tracked
- Request details should be inspectable

**Status**: ⬜ Not tested

---

### Test 6: DevTools Window Persistence
**Objective**: Verify DevTools window is reused across multiple opens

**Steps**:
1. Open DevTools window (Test 2)
2. Make some changes in DevTools (e.g., switch to Console tab)
3. Close the DevTools window
4. Click the DevTools button again

**Expected Result**:
- Same DevTools window should reopen
- Previous state/tab should be preserved (Console tab still selected)
- Window should maintain the same size and position

**Status**: ⬜ Not tested

---

### Test 7: DevTools Window Management
**Objective**: Verify proper window behavior

**Steps**:
1. Open DevTools window
2. Try to minimize, maximize, and restore the window
3. Try to move the window to different screen positions
4. Click the DevTools button multiple times

**Expected Result**:
- Window should support minimize/maximize/restore operations
- Window should be movable
- Multiple clicks should bring the existing window to front (not create new windows)
- `show()`, `raise()`, and `activateWindow()` should work correctly

**Status**: ⬜ Not tested

---

### Test 8: Memory Cleanup
**Objective**: Verify proper cleanup when report viewer is closed

**Steps**:
1. Open DevTools window
2. Keep DevTools window open
3. Close the main report viewer window
4. Check for memory leaks (use a memory profiler if available)

**Expected Result**:
- DevTools window should close automatically when report viewer closes
- No memory leaks should occur
- Dialog should be properly deleted in destructor

**Status**: ⬜ Not tested

---

### Test 9: Translation Support
**Objective**: Verify window title is translatable

**Steps**:
1. Change application language to English (if available)
2. Open report viewer
3. Click DevTools button
4. Observe window title

**Expected Result**:
- Window title should be translated to "Development Tools" (or appropriate English translation)
- If no English translation exists yet, should use Spanish "Herramientas de desarrollo"

**Status**: ⬜ Not tested

---

### Test 10: Multiple Report Viewers
**Objective**: Verify DevTools works with multiple report instances

**Steps**:
1. Open first report viewer
2. Open DevTools for first report
3. Open second report viewer (if possible)
4. Click DevTools in second report

**Expected Result**:
- Each report viewer should have its own DevTools instance
- DevTools should inspect the correct report viewer
- No conflicts between multiple DevTools windows

**Status**: ⬜ Not tested

---

## Test Summary

| Test # | Description | Status | Notes |
|--------|-------------|--------|-------|
| 1 | Button Visibility | ⬜ | |
| 2 | Window Opens | ⬜ | |
| 3 | Elements Inspector | ⬜ | |
| 4 | Console | ⬜ | |
| 5 | Network Monitor | ⬜ | |
| 6 | Window Persistence | ⬜ | |
| 7 | Window Management | ⬜ | |
| 8 | Memory Cleanup | ⬜ | |
| 9 | Translation | ⬜ | |
| 10 | Multiple Reports | ⬜ | |

## Known Limitations

1. **Qt WebEngine Requirement**: DevTools functionality requires QtWebEngine to be properly installed and configured
2. **First-Time Creation**: DevTools dialog is created on first click, there may be a slight delay
3. **Window Management**: DevTools window is modal to the report viewer (parent-child relationship)

## Debugging Issues

If DevTools doesn't work:

1. **Check Qt Version**: Ensure Qt 6.0+ is installed (6.10 recommended)
2. **Check WebEngine**: Verify QtWebEngine module is available
3. **Check Console Output**: Look for error messages in application console
4. **Verify Build**: Ensure reportviewer.cpp includes QVBoxLayout and QMessageBox headers
5. **Check Signal/Slot**: Verify on_btDevTools_clicked() slot is properly connected

## Reporter Information

- **Tester Name**: _____________
- **Date**: _____________
- **MARRTA Version**: _____________
- **Qt Version**: _____________
- **OS**: _____________

## Notes

_Use this section to record any additional observations, bugs, or suggestions_

---
