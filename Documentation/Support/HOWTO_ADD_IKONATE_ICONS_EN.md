# How to Add Ikonate Icons to MARRTA

## Overview

MARRTA uses SVG icons from the [Ikonate](https://ikonate.com/) collection integrated through Qt's resource system (QRC). Icons are located in the `resources/icons/` directory and loaded via the `icons.qrc` file.

## Current Icons

The following Ikonate icons are already included in the project:

- `activity.svg` - Activity chart
- `book-opened.svg` - Open book
- `box.svg` - Box
- `celluar.svg` - Cellular network
- `checked.svg` - Checked/verified
- `chevron-down.svg` - Down arrow
- `chevron-up.svg` - Up arrow
- `circle-ok.svg` - Circle OK
- `close.svg` - Close
- `dashboard.svg` - Dashboard
- `excel.svg` - Excel
- `info.svg` - Information
- `line-chart.svg` - Line chart
- `new.svg` - New
- `news.svg` - News
- `next.svg` - Next
- `no-entry.svg` - Forbidden
- `print.svg` - Print
- `remove.svg` - Remove
- `save.svg` - Save
- `stop.svg` - Stop
- `trending-down.svg` - Trending down
- `upload.svg` - Upload

## How to Add New Icons

### Step 1: Download Ikonate Icons

1. Download the `ikonate.zip` file provided in the issue
2. Extract the ZIP file contents
3. Ikonate icons are SVG files with specific characteristics:
   - SVG format
   - 24x24 pixels viewBox size
   - Configurable stroke attributes
   - No fill (stroke-based)

### Step 2: Copy Icons to the Project

Copy the SVG files you need to the icons directory:

```bash
cp path/to/ikonate/icons/*.svg /home/runner/work/MARRTA/MARRTA/resources/icons/
```

For example, if you have additional icons:
```bash
cp ikonate/folder.svg resources/icons/
cp ikonate/edit.svg resources/icons/
cp ikonate/settings.svg resources/icons/
```

### Step 3: Add Icons to the QRC File

Edit the `icons.qrc` file and add entries for the new icons inside the `<qresource prefix="/icons">` tag:

```xml
<RCC>
    <qresource prefix="/icons">
        <!-- Existing icons -->
        <file>resources/icons/chevron-down.svg</file>
        <file>resources/icons/chevron-up.svg</file>
        <!-- ... more existing icons ... -->
        
        <!-- NEW ICONS - Add here -->
        <file>resources/icons/folder.svg</file>
        <file>resources/icons/edit.svg</file>
        <file>resources/icons/settings.svg</file>
    </qresource>
</RCC>
```

### Step 4: Recompile the Project

For Qt to compile the resources and integrate them into the executable, you need to recompile:

```bash
# Navigate to the project directory
cd /home/runner/work/MARRTA/MARRTA

# Generate the Makefile with qmake
qmake marrta.pro

# Compile the project
make

# Or in a single command
qmake && make
```

**Note**: If you have Qt5 installed, the command might be `qmake-qt5` instead of `qmake`:

```bash
qmake-qt5 && make
```

### Step 5: Use the Icons in Code

Once compiled, you can use the icons in your C++ code as follows:

#### Example 1: Icon in QAction (Menu)

```cpp
menu.addAction(
    QIcon(":/icons/resources/icons/folder.svg"),
    "Open folder",
    [=]{
        // Action code
    }
);
```

#### Example 2: Icon in QPushButton

```cpp
QPushButton *button = new QPushButton("Settings", this);
button->setIcon(QIcon(":/icons/resources/icons/settings.svg"));
```

#### Example 3: Icon in QToolButton

```cpp
QToolButton *toolButton = new QToolButton(this);
toolButton->setIcon(QIcon(":/icons/resources/icons/edit.svg"));
toolButton->setToolTip("Edit");
```

## Icon Usage Pattern in MARRTA

Icons are currently used in the project following this pattern:

```cpp
QIcon(":/icons/resources/icons/icon-name.svg")
```

Where:
- `:/icons/` is the prefix defined in the QRC file
- `resources/icons/` is the path to the file within the project
- `icon-name.svg` is the icon file name

### Examples from Current Code

In `mainwindow.cpp`:

```cpp
// Checked icon
menu.addAction(
    QIcon(":/icons/resources/icons/checked.svg"),
    "Mark all in this initiator",
    [=]{ /* ... */ }
);

// Stop icon
menu.addAction(
    QIcon(":/icons/resources/icons/stop.svg"),
    "Unmark all in this initiator",
    [=]{ /* ... */ }
);

// No-entry icon
menu.addAction(
    QIcon(":/icons/resources/icons/no-entry.svg"),
    "Analyze strength",
    [=]{ /* ... */ }
);
```

## Replacing Theme/XDG Icons

If you're currently using Theme or XDG Theme icons (like `QIcon::fromTheme("icon-name")`), you can replace them with Ikonate icons:

### Before (Theme Icon):
```cpp
button->setIcon(QIcon::fromTheme("document-open"));
```

### After (Ikonate Icon):
```cpp
button->setIcon(QIcon(":/icons/resources/icons/folder.svg"));
```

## Advantages of Using QRC Resources

1. **Portability**: Icons are packaged inside the executable
2. **No external files required**: No dependencies on disk files
3. **Performance**: Fast access to resources
4. **Consistency**: Icons look the same across all operating systems
5. **No theme dependencies**: Does not depend on system themes

## Customizing Ikonate Icons

Ikonate icons are SVG with stroke (no fill). You can customize the color by editing the SVG file:

```xml
<svg stroke="#2329D6" stroke-width="1" ...>
```

Change `#2329D6` to the hexadecimal color you want. For example:
- Black: `#000000`
- Dark blue: `#0066CC`
- Green: `#00AA00`

## Verify Icons Work

After compiling, run the application:

```bash
./marrta
```

The icons should appear in menus, buttons, and other interface elements where you've configured them.

## Troubleshooting

### Icons don't appear after compiling

1. Verify that the `icons.qrc` file is properly formed (valid XML)
2. Ensure the paths in the QRC match the actual file locations
3. Recompile completely:
   ```bash
   make clean
   qmake && make
   ```

### Error compiling the QRC file

If you get compilation errors related to the QRC:

1. Verify that all files listed in `icons.qrc` exist
2. Ensure SVG files are valid
3. Check for no special characters in file names

### Icons look blurry or wrong

Ikonate icons are SVG, so they should look sharp at any size. If they look wrong:

1. Verify the SVG is valid
2. Ensure the viewBox is set correctly (24x24 for Ikonate)

## Available Icons in Ikonate

Ikonate offers over 100 icons. Some useful for MARRTA might be:

- `alert` - Alert
- `archive` - Archive
- `calendar` - Calendar
- `camera` - Camera
- `code` - Code
- `copy` - Copy
- `delete` - Delete
- `document` - Document
- `download` - Download
- `edit` - Edit
- `filter` - Filter
- `folder` - Folder
- `grid` - Grid
- `help` - Help
- `home` - Home
- `link` - Link
- `list` - List
- `lock` - Lock
- `mail` - Mail
- `menu` - Menu
- `pin` - Pin
- `refresh` - Refresh
- `search` - Search
- `settings` - Settings
- `share` - Share
- `star` - Star
- `tag` - Tag
- `trash` - Trash
- `user` - User
- `video` - Video
- `warning` - Warning

## Additional Resources

- **Ikonate Official**: https://ikonate.com/
- **Qt QRC Documentation**: https://doc.qt.io/qt-5/resources.html
- **Qt Icon System**: https://doc.qt.io/qt-5/qicon.html

## Quick Summary

To add a new Ikonate icon:

1. Copy the SVG file to `resources/icons/`
2. Add the line to `icons.qrc`: `<file>resources/icons/new-icon.svg</file>`
3. Recompile: `qmake && make`
4. Use in code: `QIcon(":/icons/resources/icons/new-icon.svg")`

Done!
