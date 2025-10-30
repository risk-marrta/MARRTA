# QtWebEngine Installation Guide

## Overview
MARRTA now uses QtWebEngine for improved report rendering. QtWebEngine requires separate installation on some platforms.

## Installation Instructions

### Windows (Qt 6.8.0 MinGW)

QtWebEngine is **not included** by default in Qt 6 MinGW distributions. You have two options:

#### Option 1: Install QtWebEngine via Qt Maintenance Tool (Recommended)
1. Run Qt Maintenance Tool
2. Go to "Add or remove components"
3. Under Qt → Qt 6.8.0 → Additional Libraries
4. Check "Qt WebEngine" (MSVC version required, MinGW doesn't support WebEngine on Windows)
5. Complete the installation

**Important**: QtWebEngine is **only available for MSVC compiler** on Windows, not MinGW.

#### Option 2: Switch to MSVC Compiler
1. Install Qt with MSVC compiler (e.g., MSVC 2019 64-bit)
2. Install QtWebEngine component via Maintenance Tool
3. Configure Qt Creator to use MSVC kit instead of MinGW

#### Option 3: Continue with Qt 5.15
Qt 5.15 has better WebEngine support on Windows with MinGW:
1. Install Qt 5.15.x
2. Install Qt WebEngine component
3. Switch project to use Qt 5.15 kit

### Linux (Ubuntu/Debian)

#### Qt 5:
```bash
sudo apt-get install qtwebengine5-dev libqt5webenginewidgets5
```

#### Qt 6:
```bash
sudo apt-get install qt6-webengine-dev libqt6webenginewidgets6
```

### macOS

QtWebEngine is included in the Qt installation by default.

## Troubleshooting

### Error: "Unknown module(s) in QT: webenginewidgets"

This means QtWebEngine is not installed. Follow the installation instructions above for your platform.

### Build Errors on Windows MinGW

QtWebEngine is **not supported** on Windows with MinGW compiler. Solutions:
1. Switch to MSVC compiler (recommended)
2. Use Qt 5.15 (may have better MinGW support)
3. Use Linux or macOS for development

### Compilation Errors with Headers

The code includes Qt 5/6 compatibility:
- Qt 5: Uses `#include <QtWebEngineWidgets/QWebEngineView>`
- Qt 6: Uses `#include <QWebEngineView>`

This is handled automatically via version checks in the code.

## Verifying Installation

To verify QtWebEngine is installed:

1. Open Qt Creator
2. Go to Help → About Plugins
3. Look for "QtWebEngine" in the list
4. Or check your Qt installation directory for WebEngine libraries

## Alternative: Revert to QTextEdit

If QtWebEngine cannot be installed, you can revert to the previous QTextEdit implementation by checking out the commit before this change:

```bash
git checkout <commit-before-webengine>
```

Or contact the development team for a QTextEdit-compatible version.
