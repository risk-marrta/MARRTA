# DevTools Implementation for Report Preview Dialog

## Overview

This document describes the implementation of Developer Tools (DevTools) support in the MARRTA HTML report preview dialog (dlgconfiginforme). DevTools allows users to inspect HTML, debug CSS, view JavaScript console output, and monitor network requests in the report preview.

## Access Method

DevTools is accessible via **right-click context menu** on the HTML report preview in the "Vista Previa" dialog:

```
┌─────────────────────────────────────────────────────────────────────┐
│                   Vista Previa del Informe HTML                     │
├─────────────────────────────────────────────────────────────────────┤
│                                                                       │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │                                                               │  │
│  │              [HTML Report Preview]                            │  │
│  │                                                               │  │
│  │  Right-click here → Context Menu → "Inspect Element"         │  │
│  │                                                               │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                       │
│                          [Guardar HTML] [Cerrar]                     │
└─────────────────────────────────────────────────────────────────────┘
```

When "Inspect Element" is clicked from the context menu, DevTools window opens:

```
┌─────────────────────────────────────────────────────────────────────┐
│                    Herramientas de desarrollo                       │
├─────────────────────────────────────────────────────────────────────┤
│  Elements | Console | Network | Performance | ...                   │
│  ─────────────────────────────────────────────────────────────────  │
│  <html>                                                              │
│    <head>                                                            │
│      <style>                                                         │
│        ...                                                           │
│      </style>                                                        │
│    </head>                                                           │
│    <body>                                                            │
│      [Full Chromium DevTools Interface]                             │
│    </body>                                                           │
│  </html>                                                             │
│                                                                       │
│                                                                       │
└─────────────────────────────────────────────────────────────────────┘
```

## Problem Statement

Users requested the ability to use developer tools in the HTML report preview. This is useful for:
- Debugging custom report styles
- Inspecting HTML structure
- Troubleshooting JavaScript issues
- Analyzing network requests
- Testing CSS modifications

## Solution

The implementation adds DevTools support using Qt WebEngine's built-in Chromium developer tools functionality. The solution consists of:

1. **Changed QTextBrowser to QWebEngineView**: The preview dialog now uses QWebEngineView instead of QTextBrowser to support DevTools
2. **Context Menu Integration**: DevTools is accessible via right-click "Inspect Element" option
3. **Automatic Setup**: DevTools page is configured when the preview dialog is created

### Technical Implementation

#### File Changes (`dlgconfiginforme.cpp`)

Added includes for QWebEngineView:
```cpp
// Qt 5/6 compatibility for QtWebEngine headers
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QWebEngineView>
    #include <QWebEnginePage>
    #include <QWebEngineSettings>
#else
    #include <QtWebEngineWidgets/QWebEngineView>
    #include <QtWebEngineWidgets/QWebEnginePage>
    #include <QtWebEngineCore/QWebEngineSettings>
#endif
```

Modified `on_btnVistaPrevia_clicked()` method to use QWebEngineView and setup DevTools:

```cpp
// Create a dialog to show the HTML with DevTools support
QDialog previewDialog(this);
previewDialog.setWindowTitle(tr("Vista Previa del Informe HTML"));
previewDialog.resize(900, 700);

QVBoxLayout *layout = new QVBoxLayout(&previewDialog);

// Use QWebEngineView instead of QTextBrowser to support DevTools
QWebEngineView *webView = new QWebEngineView(&previewDialog);
webView->setHtml(html);
layout->addWidget(webView);

// Set up DevTools for right-click inspection
QDialog *devToolsDialog = new QDialog(&previewDialog);
devToolsDialog->setWindowTitle(tr("Herramientas de desarrollo"));
devToolsDialog->resize(1200, 800);

QVBoxLayout *devLayout = new QVBoxLayout(devToolsDialog);
devLayout->setContentsMargins(0, 0, 0, 0);

QWebEngineView *devToolsView = new QWebEngineView(devToolsDialog);
devLayout->addWidget(devToolsView);

// Connect DevTools to the web view - enables "Inspect Element" in context menu
webView->page()->setDevToolsPage(devToolsView->page());
```

#### Files Reverted

- `reportviewer.h` - Reverted to original (not used in application)
- `reportviewer.cpp` - Reverted to original (not used in application)  
- `reportviewer.ui` - Reverted to original (not used in application)

## How It Works

1. User opens the HTML report configuration dialog (DlgConfigInforme)
2. User clicks "Vista Previa" button to preview the report
3. The preview dialog opens with QWebEngineView showing the HTML
4. DevTools is automatically configured when the dialog is created
5. User can right-click on the HTML preview and select "Inspect Element"
6. DevTools dialog opens automatically showing the Chromium inspector
7. Users can interact with the full Chromium DevTools interface
8. The DevTools dialog is a child of the preview dialog and is cleaned up automatically

## Features

- **Full Chromium DevTools**: Access to all standard DevTools features:
  - Elements inspector (HTML/CSS)
  - Console (JavaScript debugging)
  - Network monitor
  - Performance profiler
  - Application/Storage inspector
  
- **Persistent Window**: The DevTools dialog persists across multiple uses (only created once)

- **Proper Cleanup**: Dialog is properly deleted when ReportViewer is destroyed

- **Translation Support**: Window title uses `tr()` for internationalization

## Files Modified

- `dlgconfiginforme.cpp` - Modified on_btnVistaPrevia_clicked() to use QWebEngineView and setup DevTools
- `reportviewer.h`, `reportviewer.cpp`, `reportviewer.ui` - Reverted to original (not used)
- `RELEASE_INFO.md` - Documented changes for release notes

## Testing Instructions

1. Open MARRTA application
2. Navigate to the HTML report configuration dialog (DlgConfigInforme)
3. Configure report options and click "Vista Previa" button
4. Verify that the preview dialog opens showing the HTML report
5. Right-click on the HTML preview area
6. Verify that context menu appears with "Inspect Element" option
7. Click "Inspect Element"
8. Verify that DevTools window opens showing the Chromium DevTools interface
9. Test various DevTools features:
   - Inspect HTML elements
   - View console output
   - Check network requests
   - Modify CSS live
10. Close DevTools window
11. Right-click again and select "Inspect Element" to verify it still works
12. Close the preview dialog to verify proper cleanup

## Qt API Used

- `QWebEnginePage::setDevToolsPage(QWebEnginePage*)` - Connects DevTools inspector
- `QWebEngineView` - Displays both the report and the DevTools
- `QDialog` - Container for the DevTools window
- `QVBoxLayout` - Layout for DevTools window

## References

- [Qt WebEngine DevTools Documentation](https://doc.qt.io/qt-6/qwebenginepage.html#setDevToolsPage)
- Issue: [Reports] - Add devtools
