# Cómo Añadir Iconos de Ikonate a MARRTA

## Descripción General

MARRTA utiliza iconos SVG de la colección [Ikonate](https://ikonate.com/) integrados mediante el sistema de recursos de Qt (QRC). Los iconos se encuentran en el directorio `resources/icons/` y se cargan a través del archivo `icons.qrc`.

## Iconos Actuales

Los siguientes iconos de Ikonate ya están incluidos en el proyecto:

- `activity.svg` - Gráfico de actividad
- `book-opened.svg` - Libro abierto
- `box.svg` - Caja
- `celluar.svg` - Red celular
- `checked.svg` - Marcado/verificado
- `chevron-down.svg` - Flecha abajo
- `chevron-up.svg` - Flecha arriba
- `circle-ok.svg` - Círculo OK
- `close.svg` - Cerrar
- `dashboard.svg` - Panel de control
- `excel.svg` - Excel
- `info.svg` - Información
- `line-chart.svg` - Gráfico de líneas
- `new.svg` - Nuevo
- `news.svg` - Noticias
- `next.svg` - Siguiente
- `no-entry.svg` - Prohibido
- `print.svg` - Imprimir
- `remove.svg` - Eliminar
- `save.svg` - Guardar
- `stop.svg` - Parar
- `trending-down.svg` - Tendencia descendente
- `upload.svg` - Subir

## Cómo Añadir Nuevos Iconos

### Paso 1: Descargar Iconos de Ikonate

1. Descarga el archivo `ikonate.zip` proporcionado en el issue
2. Extrae el contenido del archivo ZIP
3. Los iconos de Ikonate son archivos SVG con características específicas:
   - Formato SVG
   - Tamaño viewBox de 24x24 píxeles
   - Atributos stroke configurables
   - Sin relleno (stroke-based)

### Paso 2: Copiar Iconos al Proyecto

Copia los archivos SVG que necesites al directorio de iconos:

```bash
cp ruta/a/ikonate/icons/*.svg /home/runner/work/MARRTA/MARRTA/resources/icons/
```

Por ejemplo, si tienes iconos adicionales:
```bash
cp ikonate/folder.svg resources/icons/
cp ikonate/edit.svg resources/icons/
cp ikonate/settings.svg resources/icons/
```

### Paso 3: Añadir Iconos al Archivo QRC

Edita el archivo `icons.qrc` y añade las entradas para los nuevos iconos dentro de la etiqueta `<qresource prefix="/icons">`:

```xml
<RCC>
    <qresource prefix="/icons">
        <!-- Iconos existentes -->
        <file>resources/icons/chevron-down.svg</file>
        <file>resources/icons/chevron-up.svg</file>
        <!-- ... más iconos existentes ... -->
        
        <!-- NUEVOS ICONOS - Añadir aquí -->
        <file>resources/icons/folder.svg</file>
        <file>resources/icons/edit.svg</file>
        <file>resources/icons/settings.svg</file>
    </qresource>
</RCC>
```

### Paso 4: Recompilar el Proyecto

Para que Qt compile los recursos y los integre en el ejecutable, debes recompilar:

```bash
# Navega al directorio del proyecto
cd /home/runner/work/MARRTA/MARRTA

# Genera el Makefile con qmake
qmake marrta.pro

# Compila el proyecto
make

# O en un solo comando
qmake && make
```

**Nota**: Si tienes Qt5 instalado, es posible que el comando sea `qmake-qt5` en lugar de `qmake`:

```bash
qmake-qt5 && make
```

### Paso 5: Usar los Iconos en el Código

Una vez compilados, puedes usar los iconos en tu código C++ de la siguiente manera:

#### Ejemplo 1: Icono en QAction (Menú)

```cpp
menu.addAction(
    QIcon(":/icons/resources/icons/folder.svg"),
    "Abrir carpeta",
    [=]{
        // Código de acción
    }
);
```

#### Ejemplo 2: Icono en QPushButton

```cpp
QPushButton *button = new QPushButton("Configuración", this);
button->setIcon(QIcon(":/icons/resources/icons/settings.svg"));
```

#### Ejemplo 3: Icono en QToolButton

```cpp
QToolButton *toolButton = new QToolButton(this);
toolButton->setIcon(QIcon(":/icons/resources/icons/edit.svg"));
toolButton->setToolTip("Editar");
```

## Patrón de Uso de Iconos en MARRTA

Los iconos se utilizan actualmente en el proyecto siguiendo este patrón:

```cpp
QIcon(":/icons/resources/icons/nombre-del-icono.svg")
```

Donde:
- `:/icons/` es el prefijo definido en el archivo QRC
- `resources/icons/` es la ruta al archivo dentro del proyecto
- `nombre-del-icono.svg` es el nombre del archivo de icono

### Ejemplos del Código Actual

En `mainwindow.cpp`:

```cpp
// Icono de verificación
menu.addAction(
    QIcon(":/icons/resources/icons/checked.svg"),
    "Marcar todos en este iniciador",
    [=]{ /* ... */ }
);

// Icono de parar
menu.addAction(
    QIcon(":/icons/resources/icons/stop.svg"),
    "Desmarcar todos en este iniciador",
    [=]{ /* ... */ }
);

// Icono de prohibido
menu.addAction(
    QIcon(":/icons/resources/icons/no-entry.svg"),
    "Analizar fortaleza",
    [=]{ /* ... */ }
);
```

## Reemplazar Iconos de Theme/XDG

Si actualmente estás usando iconos de Theme o XDG Theme (como `QIcon::fromTheme("nombre-icono")`), puedes reemplazarlos con iconos de Ikonate:

### Antes (Theme Icon):
```cpp
button->setIcon(QIcon::fromTheme("document-open"));
```

### Después (Ikonate Icon):
```cpp
button->setIcon(QIcon(":/icons/resources/icons/folder.svg"));
```

## Ventajas de Usar Recursos QRC

1. **Portabilidad**: Los iconos se empaquetan dentro del ejecutable
2. **No requiere archivos externos**: No hay dependencias de archivos en disco
3. **Rendimiento**: Acceso rápido a los recursos
4. **Consistencia**: Los iconos se ven igual en todos los sistemas operativos
5. **Sin dependencias de themes**: No depende de los temas del sistema

## Personalizar Iconos Ikonate

Los iconos de Ikonate son SVG con stroke (sin relleno). Puedes personalizar el color editando el archivo SVG:

```xml
<svg stroke="#2329D6" stroke-width="1" ...>
```

Cambia `#2329D6` al color hexadecimal que desees. Por ejemplo:
- Negro: `#000000`
- Azul oscuro: `#0066CC`
- Verde: `#00AA00`

## Verificar que los Iconos Funcionan

Después de compilar, ejecuta la aplicación:

```bash
./marrta
```

Los iconos deberían aparecer en los menús, botones y otros elementos de la interfaz donde los hayas configurado.

## Solución de Problemas

### Los iconos no aparecen después de compilar

1. Verifica que el archivo `icons.qrc` esté correctamente formado (XML válido)
2. Asegúrate de que las rutas en el QRC coincidan con la ubicación real de los archivos
3. Recompila completamente:
   ```bash
   make clean
   qmake && make
   ```

### Error al compilar el archivo QRC

Si obtienes errores de compilación relacionados con el QRC:

1. Verifica que todos los archivos listados en `icons.qrc` existan
2. Asegúrate de que los archivos SVG sean válidos
3. Comprueba que no haya caracteres especiales en los nombres de archivo

### Los iconos se ven borrosos o mal

Los iconos Ikonate son SVG, por lo que deberían verse nítidos a cualquier tamaño. Si se ven mal:

1. Verifica que el SVG sea válido
2. Asegúrate de que el viewBox esté configurado correctamente (24x24 para Ikonate)

## Lista de Iconos Disponibles en Ikonate

Ikonate ofrece más de 100 iconos. Algunos útiles para MARRTA podrían ser:

- `alert` - Alerta
- `archive` - Archivar
- `calendar` - Calendario
- `camera` - Cámara
- `code` - Código
- `copy` - Copiar
- `delete` - Eliminar
- `document` - Documento
- `download` - Descargar
- `edit` - Editar
- `filter` - Filtrar
- `folder` - Carpeta
- `grid` - Cuadrícula
- `help` - Ayuda
- `home` - Inicio
- `link` - Enlace
- `list` - Lista
- `lock` - Candado
- `mail` - Correo
- `menu` - Menú
- `pin` - Pin
- `refresh` - Actualizar
- `search` - Buscar
- `settings` - Configuración
- `share` - Compartir
- `star` - Estrella
- `tag` - Etiqueta
- `trash` - Papelera
- `user` - Usuario
- `video` - Video
- `warning` - Advertencia

## Recursos Adicionales

- **Ikonate Official**: https://ikonate.com/
- **Documentación Qt QRC**: https://doc.qt.io/qt-5/resources.html
- **Qt Icon System**: https://doc.qt.io/qt-5/qicon.html

## Resumen Rápido

Para añadir un nuevo icono de Ikonate:

1. Copia el archivo SVG a `resources/icons/`
2. Añade la línea al `icons.qrc`: `<file>resources/icons/nuevo-icono.svg</file>`
3. Recompila: `qmake && make`
4. Usa en código: `QIcon(":/icons/resources/icons/nuevo-icono.svg")`

¡Listo!
