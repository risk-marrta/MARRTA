# Migración de Iconos Theme a Ikonate
# Theme to Ikonate Icons Migration

## Resumen / Summary

Se han reemplazado los iconos de Theme con iconos de Ikonate en `mainwindow.ui`.

Theme icons have been replaced with Ikonate icons in `mainwindow.ui`.

## Cambios Realizados / Changes Made

| Theme Icon (Anterior) | Ikonate Icon (Nuevo) | Línea Aprox. | Uso / Usage |
|----------------------|---------------------|--------------|-------------|
| `folder-open` | `folder.svg` | ~1929 | Menú Modelo |
| `media-seek-backward` | `back.svg` | ~1902 | Menú Importar |
| `media-seek-forward` | `forward.svg` | ~1911 | Menú Exportar |
| `mail-message-new` | `edit.svg` | ~1920 | Menú Scripting |
| `system-log-out` | `exit.svg` | ~2045 | Acción Cerrar modelo |
| `dialog-information` | `info.svg` | ~2086 | Acción Acerca de |
| `system-software-update` | `timer.svg` | ~2094 | Comprobar actualizaciones |
| `system-shutdown` | `power.svg` | ~2126 | Acción Salir |
| `document-new` | `file.svg` | ~2250 | Acción Crear modelo |

## Formato de Cambio / Change Format

### Antes / Before:
```xml
<property name="icon">
  <iconset theme="theme-name"/>
</property>
```

### Después / After:
```xml
<property name="icon">
  <iconset resource="icons.qrc">
    <normaloff>:/icons/resources/icons/icon-name.svg</normaloff>:/icons/resources/icons/icon-name.svg
  </iconset>
</property>
```

## Verificación / Verification

✅ Todos los iconos están disponibles en `resources/icons/`
✅ Todos los iconos están registrados en `icons.qrc`
✅ El archivo XML es válido
✅ No quedan referencias a theme icons (excepto `folder-download` no especificado)

All icons are available in `resources/icons/`
All icons are registered in `icons.qrc`
XML file is valid
No theme icon references remain (except `folder-download` not specified)

## Iconos No Modificados / Unchanged Icons

- `folder-download` - No estaba en la lista de cambios solicitados / Not in requested changes list

## Beneficios / Benefits

1. ✅ **Consistencia visual** - Los iconos se ven igual en todos los sistemas operativos
2. ✅ **No dependencias** - No requiere temas del sistema instalados
3. ✅ **Portabilidad** - Iconos empaquetados en el ejecutable
4. ✅ **Personalizable** - Fácil cambiar colores editando SVG

1. ✅ **Visual consistency** - Icons look the same across all operating systems
2. ✅ **No dependencies** - Doesn't require system themes installed
3. ✅ **Portability** - Icons packaged in the executable
4. ✅ **Customizable** - Easy to change colors by editing SVG

## Compilación / Compilation

Para aplicar los cambios, recompila el proyecto:

To apply the changes, recompile the project:

```bash
qmake marrta.pro && make
```

## Archivo Modificado / Modified File

- `mainwindow.ui` - 9 iconos cambiados de theme a resource / 9 icons changed from theme to resource
