# About Window Header Optimization

## Visual Comparison

![About Window Optimization](docs/mockups/about_window_optimization.png)

*Visual comparison showing the header layout changes and build number addition*

## Changes Made

This document describes the changes made to optimize the About window header layout as requested in the issue.

## Issue Requirements

1. **Header Layout Change**: Swap positions - MARRTA icon on the left, text on the right
2. **Build Number Addition**: Add build number (git commit hash) next to version and date in brackets []

## Implementation

### 1. Header Layout Change (about.ui)

**BEFORE:**
```
[Spacer] [Text: MARRTA + Version + Description] [Spacer] [Icon] [Spacer]
```

**AFTER:**
```
[Spacer] [Icon] [Spacer] [Text: MARRTA + Version + Description] [Spacer]
```

The horizontal layout in `about.ui` was modified to swap the order of elements:
- Logo (LOGO_MARRTA) is now positioned first (left side)
- Text layout (verticalLayout_title) is now positioned after the logo (right side)

### 2. Build Number Addition

#### Changes to marrta.pro:
- Added qmake code to extract git commit hash at build time
- Defined `GIT_COMMIT` macro with the short commit hash (7 characters)
- Falls back to "unknown" if git is not available

```qmake
# Get git commit hash for build number
GIT_COMMIT = $$system(git log -1 --format=%h)
isEmpty(GIT_COMMIT) {
    GIT_COMMIT = "unknown"
}
DEFINES += GIT_COMMIT=\\\"$$GIT_COMMIT\\\"
```

#### Changes to about.cpp:
- Modified version string construction to include build number
- Build number is appended in brackets after the version and date
- Only added if GIT_COMMIT is defined at compile time

```cpp
if (version != "") {
    strEtVersion = "Versión " + version;
    // Add build number (git commit hash) if available
    #ifdef GIT_COMMIT
        strEtVersion += " [" + QString(GIT_COMMIT) + "]";
    #endif
}
```

## Visual Example

### Current Version Display:
```
Versión 1.5 (2025-10-11)
```

### New Version Display (with build number):
```
Versión 1.5 (2025-10-11) [ef30372]
```

Where `ef30372` is the short git commit hash of the current build.

## Files Modified

1. **about.ui** - Swapped icon and text positions in header layout
2. **marrta.pro** - Added git commit hash extraction and GIT_COMMIT define
3. **about.cpp** - Added logic to append build number to version string

## Testing

To verify these changes:

1. Build the project:
   ```bash
   qmake marrta.pro
   make
   ```

2. Run the application and open Help → About

3. Verify:
   - ✅ MARRTA icon appears on the LEFT side
   - ✅ Text (title, version, description) appears on the RIGHT side
   - ✅ Version line shows: `Versión X.X (YYYY-MM-DD) [commit_hash]`

## Benefits

1. **Improved Layout**: Icon-first layout is more visually balanced
2. **Better Traceability**: Build number allows exact identification of the binary version
3. **Debugging Aid**: Developers can quickly identify which commit a binary was built from
4. **Minimal Changes**: Only 3 files modified with surgical precision
