# Building MARRTA with Python Scripting Support

## Overview

MARRTA includes optional Python 3.12 scripting support. By default, the application compiles **without** Python support to avoid dependency issues. When Python is not enabled, the Python menu entry still appears but displays a helpful error message with build instructions.

## Building WITHOUT Python (Default)

The default build does not require Python to be installed:

```bash
qmake marrta.pro
make
```

The application will compile successfully. When users click **Modelo → Scripting → Python**, they will see:
```
Error: Python scripting support not compiled in.
To enable Python support, rebuild with:
qmake "PYTHON_ENABLED=1" "PYTHON_INCLUDE=<path>" "PYTHON_LIB=<path>"
```

## Building WITH Python Support

### Requirements

- Python 3.12 installed on your system
- Python development headers and libraries

### Linux/Unix

**Option 1: Auto-detect (if Python is in standard location)**
```bash
qmake "PYTHON_ENABLED=1" marrta.pro
make
```

**Option 2: Specify paths manually**
```bash
qmake "PYTHON_ENABLED=1" \
      "PYTHON_INCLUDE=/usr/include/python3.12" \
      marrta.pro
make
```

### Windows

**Step 1: Install Python 3.12**
- Download from https://www.python.org/downloads/
- Standard installation location: `C:\Python312`
- Ensure "Add Python to PATH" is checked during installation
- Ensure development files are included (default)

**Step 2: Build with Python support**

```cmd
qmake "PYTHON_ENABLED=1" ^
      "PYTHON_INCLUDE=C:/Python312/include" ^
      "PYTHON_LIB=C:/Python312/libs" ^
      marrta.pro
      
nmake   (for MSVC)
or
mingw32-make   (for MinGW)
```

**Note**: Adjust paths if Python is installed in a different location.

### macOS

```bash
# Using Homebrew Python
qmake "PYTHON_ENABLED=1" \
      "PYTHON_INCLUDE=/usr/local/opt/python@3.12/Frameworks/Python.framework/Versions/3.12/include/python3.12" \
      marrta.pro
make
```

## Verifying Python Support

After building, you can verify Python support by:

1. Launch MARRTA
2. Open a model
3. Go to **Modelo → Scripting → Python**
4. If Python is enabled, the editor opens and you can execute:
   ```python
   print(Marrta.get_version())
   ```
5. If Python is disabled, you'll see the error message with build instructions

## Troubleshooting

### "Python.h not found" Error

**Problem**: Compiler cannot find Python header files.

**Solution**: 
- Verify Python 3.12 is installed: `python --version` or `python3 --version`
- Check include path exists: `ls /usr/include/python3.12` (Linux) or check `C:\Python312\include` (Windows)
- Specify correct path with `PYTHON_INCLUDE` parameter

### Linker Errors on Windows

**Problem**: `python312.lib not found` or similar linker errors.

**Solution**:
- Verify `C:\Python312\libs\python312.lib` exists
- Use forward slashes in paths: `C:/Python312/libs` (not `C:\Python312\libs`)
- Ensure you're using the correct library name: `python312` (not `python3.12`)

### Python Works but Scripts Fail

**Problem**: Editor opens but scripts don't execute properly.

**Solution**:
- Ensure Python 3.12 runtime is installed (not just dev headers)
- On Windows, ensure `python312.dll` is in PATH or in the same directory as `marrta.exe`
- Test Python installation: Open command prompt and run `python` to verify

## Advanced Configuration

### Custom Python Locations

```bash
# Linux with custom Python build
qmake "PYTHON_ENABLED=1" \
      "PYTHON_INCLUDE=/opt/python3.12/include/python3.12" \
      "PYTHON_LIB_NAME=python3.12" \
      marrta.pro

# Windows with custom Python location
qmake "PYTHON_ENABLED=1" ^
      "PYTHON_INCLUDE=D:/MyPython/include" ^
      "PYTHON_LIB=D:/MyPython/libs" ^
      "PYTHON_LIB_NAME=python312" ^
      marrta.pro
```

### Available qmake Variables

- `PYTHON_ENABLED=1` - Enable Python support (required)
- `PYTHON_INCLUDE=<path>` - Path to Python include directory (optional, has defaults)
- `PYTHON_LIB=<path>` - Path to Python lib directory (Windows only, optional)
- `PYTHON_LIB_NAME=<name>` - Python library name (optional, defaults: `python3.12` on Unix, `python312` on Windows)

## Distribution

### Linux
- Application links dynamically to `libpython3.12.so`
- Users need Python 3.12 runtime installed
- Most modern Linux distributions include Python 3.12

### Windows
- Application links to `python312.dll`
- Include `python312.dll` with your distribution OR
- Require Python 3.12 installation on user's system

### macOS
- Similar to Linux, links dynamically
- Can bundle Python framework with application if desired

## FAQ

**Q: Can I use Python 3.11 or 3.13 instead of 3.12?**
A: The code is written for Python 3.12 API. You may need to adjust the Python C API calls for other versions.

**Q: Why is Python support optional?**
A: To simplify building and distribution. Not all users need Python scripting, and requiring Python would add a dependency for all installations.

**Q: Will the Python menu disappear if Python is not compiled in?**
A: No, the menu entry remains visible but shows a helpful error message explaining how to enable Python support.

**Q: Can I switch between enabled/disabled Python without reinstalling?**
A: Yes, just rebuild with or without `PYTHON_ENABLED=1`. Clean first: `make clean` or `nmake clean`.

**Q: Does this affect the JavaScript scripting engine?**
A: No, JavaScript scripting is always available and unaffected by Python configuration.

## Support

For more information about using the Python scripting API, see:
- `Documentation/Support/PYTHON_SCRIPTING_IMPLEMENTATION.md` - Technical details
- `Documentation/Support/PYTHON_SCRIPTING_QUICK_GUIDE.md` - User guide
- `Documentation/Support/PYTHON_SCRIPTING_VISUAL_GUIDE.md` - Visual guide
- `test_scripts/` - Example Python scripts
