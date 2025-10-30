# Guía Rápida: Añadir Iconos Ikonate / Quick Guide: Adding Ikonate Icons

## 🇪🇸 Español

### Resumen Ejecutivo

Este proyecto usa iconos SVG de [Ikonate](https://ikonate.com/). Para añadir más iconos:

1. **Copiar** archivos SVG a `resources/icons/`
2. **Registrar** en `icons.qrc`
3. **Recompilar** con `qmake && make`
4. **Usar** en código: `QIcon(":/icons/resources/icons/nombre.svg")`

### Métodos Disponibles

#### Método 1: Script Bash Interactivo (Recomendado)
```bash
./add_ikonate_icons.sh
```

#### Método 2: Script Python con Validación
```bash
# Modo interactivo
./manage_ikonate_icons.py

# Línea de comandos
./manage_ikonate_icons.py add /ruta/a/ikonate/icons/
./manage_ikonate_icons.py list
./manage_ikonate_icons.py check
```

#### Método 3: Manual

```bash
# 1. Copiar icono
cp nuevo-icono.svg resources/icons/

# 2. Editar icons.qrc (añadir antes de </qresource>):
#    <file>resources/icons/nuevo-icono.svg</file>

# 3. Recompilar
qmake && make
```

### Uso en Código C++

```cpp
// En menús
menu.addAction(
    QIcon(":/icons/resources/icons/folder.svg"),
    "Abrir carpeta",
    [=]{ /* código */ }
);

// En botones
QPushButton *btn = new QPushButton("Guardar", this);
btn->setIcon(QIcon(":/icons/resources/icons/save.svg"));

// En tool buttons
QToolButton *tool = new QToolButton(this);
tool->setIcon(QIcon(":/icons/resources/icons/edit.svg"));
```

### Iconos Actualmente Disponibles

- `activity.svg` - Actividad
- `book-opened.svg` - Libro abierto
- `box.svg` - Caja
- `checked.svg` - Verificado
- `close.svg` - Cerrar
- `dashboard.svg` - Panel
- `excel.svg` - Excel
- `info.svg` - Información
- `new.svg` - Nuevo
- `print.svg` - Imprimir
- `save.svg` - Guardar
- `upload.svg` - Subir
- ... y más (ver `icons.qrc`)

### Compilación

```bash
# Con qmake estándar
qmake marrta.pro && make

# O con Qt5 específico
qmake-qt5 marrta.pro && make

# Limpieza completa
make clean && qmake && make
```

### Solución de Problemas

**Iconos no aparecen:**
```bash
# Verificar QRC
./manage_ikonate_icons.py check

# Recompilar completamente
make clean && qmake && make
```

**Error de compilación:**
- Verificar que el XML del QRC sea válido
- Asegurar que todos los archivos existen
- Comprobar permisos de archivos

---

## 🇬🇧 English

### Executive Summary

This project uses SVG icons from [Ikonate](https://ikonate.com/). To add more icons:

1. **Copy** SVG files to `resources/icons/`
2. **Register** in `icons.qrc`
3. **Recompile** with `qmake && make`
4. **Use** in code: `QIcon(":/icons/resources/icons/name.svg")`

### Available Methods

#### Method 1: Interactive Bash Script (Recommended)
```bash
./add_ikonate_icons.sh
```

#### Method 2: Python Script with Validation
```bash
# Interactive mode
./manage_ikonate_icons.py

# Command line
./manage_ikonate_icons.py add /path/to/ikonate/icons/
./manage_ikonate_icons.py list
./manage_ikonate_icons.py check
```

#### Method 3: Manual

```bash
# 1. Copy icon
cp new-icon.svg resources/icons/

# 2. Edit icons.qrc (add before </qresource>):
#    <file>resources/icons/new-icon.svg</file>

# 3. Recompile
qmake && make
```

### Usage in C++ Code

```cpp
// In menus
menu.addAction(
    QIcon(":/icons/resources/icons/folder.svg"),
    "Open folder",
    [=]{ /* code */ }
);

// In buttons
QPushButton *btn = new QPushButton("Save", this);
btn->setIcon(QIcon(":/icons/resources/icons/save.svg"));

// In tool buttons
QToolButton *tool = new QToolButton(this);
tool->setIcon(QIcon(":/icons/resources/icons/edit.svg"));
```

### Currently Available Icons

- `activity.svg` - Activity
- `book-opened.svg` - Open book
- `box.svg` - Box
- `checked.svg` - Checked
- `close.svg` - Close
- `dashboard.svg` - Dashboard
- `excel.svg` - Excel
- `info.svg` - Information
- `new.svg` - New
- `print.svg` - Print
- `save.svg` - Save
- `upload.svg` - Upload
- ... and more (see `icons.qrc`)

### Compilation

```bash
# With standard qmake
qmake marrta.pro && make

# Or with Qt5 specific
qmake-qt5 marrta.pro && make

# Complete clean build
make clean && qmake && make
```

### Troubleshooting

**Icons don't appear:**
```bash
# Check QRC
./manage_ikonate_icons.py check

# Complete rebuild
make clean && qmake && make
```

**Compilation error:**
- Verify QRC XML is valid
- Ensure all files exist
- Check file permissions

---

## 📚 Documentación Completa / Full Documentation

- **Español**: `HOWTO_ADD_IKONATE_ICONS.md`
- **English**: `HOWTO_ADD_IKONATE_ICONS_EN.md`

## 🛠️ Herramientas / Tools

1. `add_ikonate_icons.sh` - Bash interactive script
2. `manage_ikonate_icons.py` - Python management tool with validation
3. Manual editing of `icons.qrc`

## 🔗 Enlaces Útiles / Useful Links

- [Ikonate Official Website](https://ikonate.com/)
- [Qt Resource System Documentation](https://doc.qt.io/qt-5/resources.html)
- [QIcon Documentation](https://doc.qt.io/qt-5/qicon.html)

## 📋 Checklist Rápido / Quick Checklist

- [ ] Descargar iconos de ikonate.zip / Download icons from ikonate.zip
- [ ] Copiar SVGs a resources/icons/ / Copy SVGs to resources/icons/
- [ ] Añadir entradas en icons.qrc / Add entries to icons.qrc
- [ ] Recompilar con qmake && make / Recompile with qmake && make
- [ ] Verificar en la aplicación / Verify in application
- [ ] Usar en código con QIcon(":/icons/...") / Use in code with QIcon(":/icons/...")
