# Quick Start Guide - Issue #3 Changes

## 🚀 For Developers Who Want to Get Started Fast

### What Was Fixed?
The **Defenses API** was returning incorrect active/inactive status for defenses. Now it works correctly.

### How to Test in 5 Minutes

1. **Build the project**
   ```bash
   cd /home/runner/work/MARRTA/MARRTA
   qmake marrta.pro
   make
   ./marrta
   ```

2. **Load a model**
   - File > Open > Select a .pdb file

3. **Open script editor**
   - Menu: Herramientas > Editor de Scripts

4. **Test the fix**
   - Click "Cargar" button
   - Navigate to `test_scripts/test_defenses_api.js`
   - Click "Ejecutar"
   - **Look for:** `✓ getDefenseStatistics() funciona correctamente`
   - **Verify:** Active and inactive defense counts are both > 0 (not all defenses as active)

### Expected Output (Correct)
```
6. Probando getDefenseStatistics()...
✓ getDefenseStatistics() funciona correctamente
  Total de defensas: 127
  Defensas activas: 95      ← Should show real count
  Defensas inactivas: 32    ← Should show real count
  Defensas humanas: 58
  Defensas no humanas: 69
```

### What If I See This (Wrong)
```
  Defensas activas: 127     ← All showing as active
  Defensas inactivas: 0     ← None showing as inactive
```
**This means:** The fix didn't apply. Check that you compiled the latest code.

## 📚 Quick Documentation Reference

### Using the APIs in Scripts

#### Get All Events
```javascript
var events = Events.getAllEvents();
console.log("Total events: " + events.length);
```

#### Get Defenses with Status
```javascript
var defenses = Defenses.getAllDefenses();
for (var i = 0; i < defenses.length; i++) {
    var d = defenses[i];
    var status = d.active ? "ACTIVE" : "INACTIVE";
    console.log(d.nombre + " [" + status + "]");
}
```

#### Create a Chart
```javascript
var counts = Events.countEventsByStage();
var chart = Charts.generateTextBarChart("Events by Stage", counts);
console.log(chart);
```

#### Filter and Analyze
```javascript
var events = Events.getAllEvents();
var highRisk = Charts.filterByProperty(events, "riesgo", 3);
console.log("High risk events: " + highRisk.length);
```

## 🔍 What Files Changed?

### Bug Fix (The Important Part)
- `dbdefense.cpp` - Fixed SQL query to include `activo` field
- `adapters/defenseadapter.cpp` - Simplified code
- `adapters/defenseadapter.h` - Cleaned up interface

### Documentation (For Reference)
- All adapter `.h` files - Comprehensive inline docs
- `scriptingdocumentation.cpp` - Enhanced in-app docs

### Test Scripts (To Verify Everything Works)
- `test_scripts/test_charts_api.js` - NEW
- `test_scripts/test_marrta_api.js` - NEW

## 🎯 Key Points

1. **The Bug:** SQL query was missing the `activo` field
2. **The Fix:** Added `defensa.activo` to SELECT statement
3. **The Result:** Defenses now show correct active/inactive status
4. **Bonus:** Performance improved (1 query instead of N+1)

## 📖 Where to Learn More

- **Quick overview:** `CHANGES_VISUAL.md` (diagrams and comparisons)
- **Technical details:** `ISSUE_3_IMPLEMENTATION_SUMMARY.md` (complete guide)
- **API reference:** Open script editor > Click "Documentación" button
- **Working examples:** All files in `test_scripts/` directory

## ✅ Success Criteria

Your implementation is working correctly if:
- [x] All test scripts show ✓ marks (no ✗)
- [x] Defense statistics show both active and inactive counts
- [x] No JavaScript errors in console
- [x] Save/Load buttons work in script editor
- [x] Documentation button shows detailed API info

## 🆘 Troubleshooting

### Problem: Test script shows ✗ errors
**Solution:** 
1. Check that model (.pdb file) is loaded
2. Verify model has data (events, defenses)
3. Check console for error messages

### Problem: All defenses show as active
**Solution:**
1. Rebuild the project completely: `make clean && qmake && make`
2. Verify you're running the new executable
3. Check that `dbdefense.cpp` changes were compiled

### Problem: Script editor is empty/broken
**Solution:**
1. Check that JavaScript engine is enabled
2. Verify Qt version supports QJSEngine
3. Look for error messages on application startup

## 🎉 You're Done!

If all tests pass with ✓ marks, the scripting system is working perfectly.

Now you can:
- Write custom scripts to analyze your risk models
- Use the comprehensive API documentation
- Create reports and visualizations
- Automate repetitive analysis tasks

Happy scripting! 🚀
