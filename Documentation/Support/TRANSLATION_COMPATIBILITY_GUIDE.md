# Guía de Compatibilidad con Traducciones de Qt

## Cómo Funciona el Sistema de Traducción en MARRTA

MARRTA utiliza el sistema de traducción de Qt para soportar múltiples idiomas. Este documento explica cómo asegurar que los nuevos textos sean compatibles con las traducciones.

## Pasos para Añadir Texto Traducible

### 1. Usar la Función `tr()` en el Código

Todos los textos visibles al usuario deben estar envueltos en la función `tr()`:

```cpp
// ✅ CORRECTO - Texto traducible
msgBox.setText(tr("No hay ningún modelo de riesgo cargado."));

// ❌ INCORRECTO - Texto hardcoded
msgBox.setText("No hay ningún modelo de riesgo cargado.");
```

### 2. Contexto de Traducción

La función `tr()` funciona en el contexto de la clase donde se usa. Por ejemplo:
- Textos en `MainWindow` se agrupan en el contexto `<name>MainWindow</name>`
- Textos en `Etapas` se agrupan en el contexto `<name>Etapas</name>`

### 3. Actualizar Archivos de Traducción

Los archivos de traducción en MARRTA son:
- **marrta_es.ts** - Traducciones al español
- **marrta_en.ts** - Traducciones al inglés

#### Proceso Automático (Recomendado)

Ejecutar el comando `lupdate` para extraer automáticamente todos los textos `tr()`:

```bash
# Desde la raíz del proyecto
lupdate marrta.pro
```

Este comando:
1. Escanea todos los archivos fuente (*.cpp, *.ui)
2. Encuentra todos los textos envueltos en `tr()`
3. Actualiza los archivos .ts con las nuevas cadenas
4. Mantiene las traducciones existentes

#### Proceso Manual (Si lupdate no está disponible)

Si `lupdate` no está disponible, se deben añadir manualmente las entradas en los archivos .ts:

**Formato de entrada en marrta_es.ts:**
```xml
<message>
    <location filename="mainwindow.cpp" line="1127"/>
    <source>No hay modelo cargado</source>
    <translation type="unfinished"></translation>
</message>
<message>
    <location filename="mainwindow.cpp" line="1128"/>
    <source>No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo.</source>
    <translation type="unfinished"></translation>
</message>
```

**Formato de entrada en marrta_en.ts:**
```xml
<message>
    <location filename="mainwindow.cpp" line="1127"/>
    <source>No hay modelo cargado</source>
    <translation>No Model Loaded</translation>
</message>
<message>
    <location filename="mainwindow.cpp" line="1128"/>
    <source>No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo.</source>
    <translation>No risk model is loaded. Load a model to be able to edit it.</translation>
</message>
```

### 4. Compilar las Traducciones

Después de actualizar los archivos .ts, se deben compilar a archivos .qm:

```bash
# Compilar traducciones
lrelease marrta.pro
```

Esto genera los archivos .qm que la aplicación usa en tiempo de ejecución.

## Implementación en el Issue Actual

### Textos Añadidos

Para el issue de validación de modelo cargado, se añadieron los siguientes textos traducibles:

1. **Título del diálogo:**
   ```cpp
   msgBox.setWindowTitle(tr("No hay modelo cargado"));
   ```

2. **Mensaje del diálogo:**
   ```cpp
   msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
   ```

### Ubicaciones del Código

Los textos se añadieron en `mainwindow.cpp` en tres funciones:
- `on_actionEtapas_triggered()` (línea ~1127-1128)
- `on_mnuSucesosIniciadores_triggered()` (línea ~1148-1149)
- `on_actionListaBarreras_triggered()` (línea ~183-184)

### Proceso de Actualización de Traducciones

1. **Automático (si hay Qt instalado):**
   ```bash
   lupdate marrta.pro
   # Editar marrta_en.ts para añadir traducciones al inglés
   lrelease marrta.pro
   ```

2. **Manual (si no hay Qt):**
   - Los archivos .ts se actualizarán automáticamente cuando se ejecute `lupdate` en el entorno de desarrollo
   - O se pueden añadir manualmente siguiendo los formatos mostrados arriba

## Verificación

Para verificar que las traducciones funcionan:

1. Ejecutar la aplicación en español (idioma por defecto)
   - Los textos deben aparecer en español tal como están en el código

2. Cambiar el idioma a inglés (si está configurado)
   - Los textos deben aparecer en inglés según las traducciones en marrta_en.ts

## Buenas Prácticas

1. **Siempre usar `tr()` para textos visibles al usuario**
2. **Mantener textos descriptivos y completos** - facilita la traducción
3. **Evitar concatenación de strings traducibles** - dificulta la traducción
4. **Usar placeholders cuando sea necesario:**
   ```cpp
   tr("Error al cargar el archivo: %1").arg(filename)
   ```
5. **Ejecutar `lupdate` regularmente** durante el desarrollo

## Notas Adicionales

- Los archivos .ts son XML y pueden editarse manualmente si es necesario
- El sistema de traducción de Qt es muy robusto y soporta plurales, contextos específicos, etc.
- Para más información: https://doc.qt.io/qt-5/internationalization.html

## Resumen

✅ El código implementado usa `tr()` correctamente
✅ Las traducciones son compatibles con el sistema Qt
✅ Se siguen las convenciones del proyecto MARRTA
✅ Los textos son claros y descriptivos
