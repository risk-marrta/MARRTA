# Resumen de Implementación - Optimización ImportWizard

## Issue Original

**Título:** [Optimización] Guardar base de datos antes de importar un excel y cargar la nueva al terminar

**Descripción:**
Igual que tenemos con el asistente para crear nuevos modelos, tenemos que hacerlo para el asistente de importación desde Excel:
- Si hay una base de datos abierta, avisar si se quiere guardar, porque se van a perder las modificaciones (si las hay)
- Al "Terminar" se carga la nueva base de datos y se actualiza la pantalla principal

## Solución Implementada

Se ha implementado una solución que sigue **exactamente el mismo patrón** que el `NewModelWizard`, garantizando consistencia en la experiencia del usuario.

### Cambios Realizados

#### 1. Archivo: `mainwindow.cpp`

**Función modificada:** `MainWindow::on_actionImportarExcel_triggered()`

**Cambios:**
- Se añadió verificación de base de datos abierta antes de mostrar el wizard
- Se muestra un diálogo de advertencia con opciones: Guardar/Descartar/Cancelar
- Se llama a `saveModel()` si el usuario elige guardar
- Se carga automáticamente el modelo importado después de éxito

**Líneas de código añadidas:** 22 líneas

#### 2. Archivo: `importwizard.h`

**Cambios:**
- Se añadió declaración del slot `on_cancelButtonClicked()`

**Líneas de código añadidas:** 1 línea

#### 3. Archivo: `importwizard.cpp`

**Cambios:**
- Se cambió el texto del botón de "Cerrar" a "Terminar" en caso de éxito
- Se conectó el signal `rejected` a un handler personalizado
- Se implementó `on_cancelButtonClicked()` que llama a `accept()` en éxito o `reject()` en cancelación

**Líneas de código añadidas/modificadas:** 13 líneas

### Total de Cambios en Código

- **3 archivos modificados**
- **36 líneas añadidas**
- **4 líneas eliminadas**
- **Total neto:** +32 líneas de código

## Documentación Creada

1. **IMPORT_WIZARD_OPTIMIZATION.md** (138 líneas)
   - Descripción detallada de los cambios
   - Código de ejemplo con comentarios
   - Tabla de consistencia con NewModelWizard
   - Sección de pruebas recomendadas

2. **IMPORT_WIZARD_FLOW_COMPARISON.md** (198 líneas)
   - Comparación visual del flujo antes/después
   - Diagramas de estado del botón
   - Comparación detallada con NewModelWizard
   - Código de referencia

## Funcionamiento

### Flujo Completo

```
1. Usuario → Menú → Archivo → Importar desde Excel
   ↓
2. ¿Hay base de datos abierta?
   ├─ SÍ → Mostrar diálogo "¿Desea guardar los cambios?"
   │   ├─ Guardar → Guarda BD → Continúa
   │   ├─ Descartar → No guarda → Continúa
   │   └─ Cancelar → Termina (no abre wizard)
   └─ NO → Continúa directamente
   ↓
3. Se abre ImportWizard
   ↓
4. Usuario completa los pasos (Excel, práctica, ubicación)
   ↓
5. Conversión automática
   ├─ ÉXITO → Botón "Terminar"
   │   ↓
   │   Click en "Terminar" → accept()
   │   ↓
   │   MainWindow carga el nuevo .pdb
   │   ↓
   │   Pantalla principal se actualiza
   │
   └─ FALLO → Botón "Cancelar"
       ↓
       Click → reject() → No se carga nada
```

## Beneficios

1. **Prevención de pérdida de datos**
   - El usuario es avisado antes de cerrar la BD actual
   - Puede guardar cambios pendientes

2. **Experiencia de usuario mejorada**
   - No necesita buscar y abrir manualmente el archivo importado
   - La carga es automática e inmediata

3. **Consistencia**
   - Comportamiento idéntico a NewModelWizard
   - Mismos mensajes, mismas opciones, mismo flujo

4. **Claridad**
   - El botón "Terminar" indica claramente éxito
   - El botón "Cancelar" indica claramente que no hubo éxito

## Patrón de Código Reutilizado

Este patrón ahora es estándar para todos los wizards que crean/importan modelos:

```cpp
// 1. Verificar si hay BD abierta
if (praxisDb.isOpen() && !settings.lastFile.isEmpty()) {
    // Mostrar diálogo Save/Discard/Cancel
    // ...
}

// 2. Abrir el wizard
MyWizard wizard(this);

// 3. Si se acepta y es exitoso, cargar el modelo
if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
    openPraxisFile(wizard.getPdbFile());
}
```

```cpp
// En el wizard: manejar el botón de cerrar/terminar
void MyWizard::on_cancelButtonClicked()
{
    if (lastStep && successFlag) {
        accept();  // Terminar con éxito
    } else {
        reject();  // Cancelar
    }
}
```

## Pruebas Recomendadas

### Caso 1: BD Abierta - Guardar
1. Abrir una base de datos
2. Hacer algún cambio
3. Ir a Archivo → Importar desde Excel
4. Verificar que aparece el diálogo
5. Elegir "Guardar"
6. Verificar que se guarda
7. Completar la importación
8. Verificar que se carga el nuevo modelo

### Caso 2: BD Abierta - Descartar
1. Abrir una base de datos
2. Hacer algún cambio
3. Ir a Archivo → Importar desde Excel
4. Verificar que aparece el diálogo
5. Elegir "Descartar"
6. Completar la importación
7. Verificar que se carga el nuevo modelo

### Caso 3: BD Abierta - Cancelar
1. Abrir una base de datos
2. Ir a Archivo → Importar desde Excel
3. Verificar que aparece el diálogo
4. Elegir "Cancelar"
5. Verificar que no se abre el wizard
6. Verificar que la BD actual sigue abierta

### Caso 4: Sin BD Abierta
1. No tener ninguna BD abierta
2. Ir a Archivo → Importar desde Excel
3. Verificar que NO aparece el diálogo
4. Verificar que el wizard se abre directamente
5. Completar la importación
6. Verificar que se carga el nuevo modelo

### Caso 5: Botón Terminar
1. Completar la importación exitosamente
2. Verificar que el botón dice "Terminar" (no "Cerrar")
3. Hacer clic en "Terminar"
4. Verificar que se cierra el wizard
5. Verificar que se carga el modelo automáticamente

### Caso 6: Cancelación en Medio del Wizard
1. Abrir el wizard
2. Avanzar algunos pasos
3. Hacer clic en "Cancelar"
4. Verificar que se cierra el wizard
5. Verificar que no se carga ningún modelo

## Referencias

- Patrón original en: `FEEDBACK_CHANGES.md` (NewModelWizard)
- Código de referencia: `mainwindow.cpp::on_actionEmpty_triggered()`
- Código de referencia: `newmodelwizard.cpp::on_cancelButtonClicked()`

## Archivos Modificados

```
mainwindow.cpp          | 24 +++++++++++++++++++++++-
importwizard.h          |  1 +
importwizard.cpp        | 15 ++++++++++++---
```

## Commits

1. `69255d0` - Implement database save and load for ImportWizard
2. `e652ced` - Add documentation for ImportWizard optimization

## Estado

✅ **IMPLEMENTADO Y DOCUMENTADO**

La implementación está completa y lista para ser probada en un entorno Qt.
