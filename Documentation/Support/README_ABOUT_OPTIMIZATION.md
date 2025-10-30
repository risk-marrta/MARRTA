# About Window Optimization - README

## 🎯 Overview

This feature implements the requested optimizations to the MARRTA About window, specifically:

1. **Header layout reorganization**: Icon on the left, text on the right
2. **Build number display**: Git commit hash shown in brackets after version

![Visual Comparison](docs/mockups/about_window_optimization.png)

## 📋 Issue Requirements (Original Spanish)

**Issue:** [Optimización] Mejorar ventana about

- En el encabezado de la ventana "about":
  - cambiar la posición del texto y subtexto "Marrta..." y el icono: icono marta a la izquierda, textos a la derecha ✅
  - donde se pone la versión y fecha, añadir a la derecha el número de compilación entre [] (ultima commit) ✅

## ✅ Implementation Status

**COMPLETED** - All requirements have been successfully implemented.

## 🔧 Technical Implementation

### Files Modified

| File | Purpose | Changes |
|------|---------|---------|
| `about.ui` | UI Layout | Swapped icon and text positions in header layout |
| `about.cpp` | Display Logic | Added build number to version string |
| `marrta.pro` | Build Config | Extract git commit hash at build time |

### Code Changes

#### 1. Layout Swap (about.ui)

Reordered items in `QHBoxLayout` named `horizontalLayout_header`:

```
BEFORE: [Spacer] → [Text] → [Spacer] → [Icon] → [Spacer]
AFTER:  [Spacer] → [Icon] → [Spacer] → [Text] → [Spacer]
```

#### 2. Build Number Extraction (marrta.pro)

```qmake
# Get git commit hash for build number
GIT_COMMIT = $$system(git log -1 --format=%h)
isEmpty(GIT_COMMIT) {
    GIT_COMMIT = "unknown"
}
DEFINES += GIT_COMMIT=\\\"$$GIT_COMMIT\\\"
```

#### 3. Build Number Display (about.cpp)

```cpp
if (version != "") {
    strEtVersion = "Versión " + version;
    // Add build number (git commit hash) if available
    #ifdef GIT_COMMIT
        strEtVersion += " [" + QString(GIT_COMMIT) + "]";
    #endif
}
```

## 📊 Version Format

### Before
```
Versión 1.5 (2025-10-11)
```

### After
```
Versión 1.5 (2025-10-11) [d3337da]
                          ^^^^^^^^
                          Build number (git commit hash)
```

## 🎨 Visual Changes

### BEFORE (Original Layout)
```
┌────────────────────────────────────────┐
│  MARRTA                    ┌─────────┐ │
│  Versión 1.5 (2025-10-11)  │  LOGO   │ │
│  Código de cálculo...      └─────────┘ │
└────────────────────────────────────────┘
```

### AFTER (Optimized Layout)
```
┌────────────────────────────────────────┐
│  ┌─────────┐  MARRTA                   │
│  │  LOGO   │  Versión 1.5 (2025-10-11) │
│  └─────────┘  [d3337da]                │
│               Código de cálculo...     │
└────────────────────────────────────────┘
```

## 🧪 Testing

### Prerequisites
- Qt development environment installed
- Git available in PATH

### Build & Test Steps

1. **Build the project:**
   ```bash
   qmake marrta.pro
   make
   ```

2. **Run the application:**
   ```bash
   ./marrta
   ```

3. **Open About dialog:**
   - Navigate to: Help → Acerca de

4. **Verify the changes:**
   - ✅ Icon appears on the LEFT
   - ✅ Text appears on the RIGHT
   - ✅ Version displays: `Versión X.X (YYYY-MM-DD) [hash]`
   - ✅ Build number shown in brackets
   - ✅ Version text is clickable (opens GitHub releases)

### Expected Behavior

- **Build number source**: Last git commit hash (7 characters)
- **Fallback**: Shows "unknown" if git not available
- **Runtime**: No overhead (hash embedded at compile time)

## 📚 Documentation

### Main Documents
- **[ABOUT_OPTIMIZATION_SUMMARY.md](ABOUT_OPTIMIZATION_SUMMARY.md)** - Complete implementation summary
- **[ABOUT_WINDOW_HEADER_OPTIMIZATION.md](ABOUT_WINDOW_HEADER_OPTIMIZATION.md)** - Detailed implementation guide
- **[ABOUT_WINDOW_HEADER_VISUAL.md](ABOUT_WINDOW_HEADER_VISUAL.md)** - Visual comparison diagrams

### Additional Resources
- **[docs/about_window_changes.txt](docs/about_window_changes.txt)** - Text-based visualization
- **[docs/mockups/about_window_optimization.png](docs/mockups/about_window_optimization.png)** - Visual comparison image

## ✨ Benefits

1. **Better Visual Balance**: Icon-first layout is more appealing
2. **Version Traceability**: Build number enables exact version identification
3. **Debugging Support**: Developers can identify exact commit of any binary
4. **Technical Support**: Support team can verify user's precise build version
5. **Minimal Impact**: Only 3 files modified with surgical precision

## 🔍 Technical Details

### Build Number Mechanism
- **Extraction**: Uses qmake's `$$system()` to run `git log -1 --format=%h`
- **Timing**: Extracted during qmake configuration (not on every build)
- **Storage**: Embedded as compile-time constant via `DEFINES`
- **Overhead**: None at runtime

### Fallback Handling
- If git unavailable: Build number shows "unknown"
- If `GIT_COMMIT` undefined: Build number omitted entirely
- Maintains backward compatibility with manual builds

### Version Link Behavior
- Version text remains clickable
- Opens: https://github.com/risk-marrta/MARRTA/releases
- Style preserved: black text, no underline
- Build number included in link text

## 📈 Statistics

### Code Changes
- **Files modified**: 3 (about.ui, about.cpp, marrta.pro)
- **Lines added**: 75
- **Lines removed**: 32
- **Net change**: +43 lines

### Documentation
- **Documentation files**: 4 markdown files + 1 text file
- **Visual assets**: 1 PNG image (54KB, 1400x800px)
- **Total documentation**: ~600 lines

## 🚀 Deployment

### Build System Integration
The build number is automatically extracted during the qmake configuration phase. No additional build steps required.

### Continuous Integration
For CI/CD pipelines:
- Ensure git is available in build environment
- Git history must be present (avoid shallow clones)
- Build number will reflect the commit being built

### Manual Builds
For environments without git:
- Build number will show "unknown"
- Application still functions normally
- Only affects display in About dialog

## ✅ Quality Assurance

- [x] Requirements fully implemented
- [x] Code changes are minimal and surgical
- [x] No dependencies added
- [x] Backward compatible
- [x] Well documented
- [x] Visual assets provided
- [ ] Testing in Qt environment (requires Qt installation)

## 📝 Commits

| Commit | Description | Files |
|--------|-------------|-------|
| `0e42b58` | Core implementation | about.ui, about.cpp, marrta.pro |
| `d3337da` | Documentation and visuals | 4 docs + 1 image |

## 🎯 Success Criteria

✅ Icon positioned on the left  
✅ Text positioned on the right  
✅ Build number displayed in brackets  
✅ Minimal code changes  
✅ Comprehensive documentation  
✅ Visual comparison provided  

**Status: Implementation Complete - Ready for Testing**

---

For questions or issues, please refer to the detailed documentation files or contact the development team.
