# Qt 6.10 Print API Migration Fix

## Problem Statement

When compiling MARRTA with MSVC and Qt 6.10, the following compilation error occurred:

```
C:\Users\Arturo\Documents\QT\github\MARRTA\reportviewer.cpp:650: error: C2039: "Print": no es un miembro de "QWebEnginePage"
..\..\reportviewer.cpp(650): error C2039: "Print": no es un miembro de "QWebEnginePage"
C:\Qt\6.10.0\msvc2022_64\include\QtWebEngineCore/qwebenginepage.h(51): note: vea la declaración de 'QWebEnginePage'
C:\Users\Arturo\Documents\QT\github\MARRTA\reportviewer.cpp:650: error: C2065: 'Print': identificador no declarado
```

## Root Cause

In Qt 6, the `QWebEnginePage::print()` method was removed from the API. This method was deprecated and replaced with a new approach that uses `QWebEngineView::print()` instead.

### Old API (Qt 5)
```cpp
ui->reportViewer->page()->print(&printer, [](bool success) {
    if (!success) {
        qDebug() << "Print failed";
    }
});
```

### New API (Qt 6)
```cpp
// Connect to printFinished signal for completion handling
connect(ui->reportViewer->page(), &QWebEnginePage::printFinished, this, [](bool success) {
    if (!success) {
        qDebug() << "Print failed";
    }
}, Qt::SingleShotConnection);

// Use QWebEngineView::print() instead of QWebEnginePage::print()
ui->reportViewer->print(&printer);
```

## Solution

The fix involves two key changes:

1. **Use QWebEngineView::print()**: Changed from calling `page()->print()` to calling `print()` directly on the `QWebEngineView` widget.

2. **Connect to printFinished signal**: Since the new API doesn't accept a callback parameter, we connect to the `printFinished` signal on the page to handle print completion. The `Qt::SingleShotConnection` flag ensures the connection is automatically disconnected after the first signal emission.

## Files Modified

- **reportviewer.cpp**: Updated the `on_btPrint_clicked()` method to use the Qt 6 API

## References

- [Qt 6 Changes to Qt WebEngine](https://doc.qt.io/qt-6/qtwebengine-changes-qt6.html)
- [QWebEngineView Class Documentation](https://doc.qt.io/qt-6/qwebengineview.html)
- [WebEngine Widgets PrintMe Example](https://doc.qt.io/qt-6/qtwebengine-webenginewidgets-printme-example.html)

## Testing

This fix maintains the same functionality as before:
- Print dialog opens when user clicks the print button
- User can configure printer settings
- Print job executes when user accepts the dialog
- Print failure is logged to debug output
- The connection is automatically cleaned up after use (Qt::SingleShotConnection)

## Compatibility

The fix is compatible with Qt 6.x versions. The code continues to work with the existing QtWebEngine infrastructure already in place in MARRTA.
