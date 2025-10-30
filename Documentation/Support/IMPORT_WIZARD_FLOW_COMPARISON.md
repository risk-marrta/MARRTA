# Comparación de Flujo: Antes y Después de la Optimización

## Flujo ANTES de la Optimización

```
Usuario → Archivo → Importar desde Excel
│
├─ No se verifica si hay BD abierta
│  └─ Se abre el ImportWizard directamente
│
└─ Usuario completa el wizard
   │
   ├─ Si tiene éxito
   │  └─ Botón "Cerrar" cierra el diálogo
   │     └─ Usuario debe abrir manualmente el archivo .pdb generado
   │        └─ Archivo → Abrir → Buscar el archivo
   │
   └─ Si cancela
      └─ Botón "Cancelar" cierra el diálogo
         └─ No pasa nada
```

**Problemas:**
- ❌ Si hay una BD abierta con cambios, se pierden sin aviso
- ❌ Después de importar, hay que abrir manualmente el nuevo archivo
- ❌ Experiencia inconsistente con el asistente de nuevos modelos
- ❌ El botón "Cerrar" no indica claramente que la operación fue exitosa

## Flujo DESPUÉS de la Optimización

```
Usuario → Archivo → Importar desde Excel
│
├─ ¿Hay una BD abierta?
│  │
│  ├─ SÍ → Mostrar diálogo de advertencia
│  │  │
│  │  ├─ Usuario elige "Guardar"
│  │  │  └─ Guarda la BD actual → Abre ImportWizard
│  │  │
│  │  ├─ Usuario elige "Descartar"
│  │  │  └─ Descarta cambios → Abre ImportWizard
│  │  │
│  │  └─ Usuario elige "Cancelar"
│  │     └─ No abre el ImportWizard → Fin
│  │
│  └─ NO → Abre ImportWizard directamente
│
└─ Usuario completa el wizard
   │
   ├─ Si tiene éxito
   │  └─ Botón "Terminar" acepta el diálogo
   │     └─ MainWindow carga automáticamente el nuevo .pdb
   │        └─ Pantalla principal se actualiza con el nuevo modelo
   │
   └─ Si cancela
      └─ Botón "Cancelar" rechaza el diálogo
         └─ La BD original (si había una) permanece abierta
```

**Mejoras:**
- ✅ Se protegen los cambios no guardados con aviso previo
- ✅ El modelo importado se carga automáticamente
- ✅ Experiencia consistente con NewModelWizard
- ✅ El botón "Terminar" indica claramente éxito en la operación
- ✅ La pantalla principal se actualiza sin intervención del usuario

## Detalles de Implementación

### 1. Verificación de BD Abierta (mainwindow.cpp)

**Código añadido en `on_actionImportarExcel_triggered()`:**

```cpp
// Check if there's a database open
if (praxisDb.isOpen() && !settings.lastFile.isEmpty()) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("Base de datos abierta"));
    msgBox.setText(tr("Hay una base de datos abierta actualmente."));
    msgBox.setInformativeText(tr("¿Desea guardar los cambios antes de importar desde Excel?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        saveModel();
    } else if (ret == QMessageBox::Cancel) {
        return;  // Sale sin abrir el wizard
    }
    // If Discard, continue without saving
}
```

### 2. Manejo del Botón Terminar (importwizard.cpp)

**Cambio en `updateButtons()`:**

```cpp
// ANTES:
cancelButton->setText(tr("Cerrar"));

// DESPUÉS:
cancelButton->setText(tr("Terminar"));
```

**Nuevo slot `on_cancelButtonClicked()`:**

```cpp
void ImportWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && conversionSuccessful) {
        accept();  // Acepta el diálogo cuando la conversión es exitosa
    } else {
        reject();  // Rechaza en caso de cancelación
    }
}
```

### 3. Carga Automática del Modelo (mainwindow.cpp)

**Cambio en `on_actionImportarExcel_triggered()`:**

```cpp
// ANTES:
ImportWizard wizard(this);
wizard.exec();

// DESPUÉS:
ImportWizard wizard(this);
if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
    // Load the newly imported model
    openPraxisFile(wizard.getPdbFile());
}
```

## Diagrama de Estados del Botón

```
┌─────────────────────────────────────────────────────────┐
│                   ImportWizard                          │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Paso 1, 2, 3:          │  Paso 4 (Conversión):       │
│  ┌──────────┐           │  ┌──────────┐               │
│  │ Cancelar │           │  │ Cancelar │  (si falla)   │
│  └──────────┘           │  └──────────┘               │
│                         │                              │
│                         │  ┌──────────┐               │
│                         │  │ Terminar │  (si éxito)   │
│                         │  └──────────┘               │
│                         │                              │
└─────────────────────────────────────────────────────────┘

                     │
                     ├─ "Cancelar" → reject() → No se carga nada
                     │
                     └─ "Terminar" → accept() → openPraxisFile()
```

## Comparación con NewModelWizard

Ambos asistentes ahora comparten el mismo patrón de diseño:

| Aspecto | NewModelWizard | ImportWizard |
|---------|----------------|--------------|
| Verificación de BD abierta | ✅ `praxisDb.isOpen()` | ✅ `praxisDb.isOpen()` |
| Diálogo de advertencia | ✅ Save/Discard/Cancel | ✅ Save/Discard/Cancel |
| Botón final en éxito | ✅ "Terminar" | ✅ "Terminar" |
| Botón final en fallo | ✅ "Cancelar" | ✅ "Cancelar" |
| accept() en éxito | ✅ Sí | ✅ Sí |
| reject() en cancelación | ✅ Sí | ✅ Sí |
| Carga automática | ✅ `openPraxisFile()` | ✅ `openPraxisFile()` |

## Código de Referencia en NewModelWizard

El código de `ImportWizard` ahora sigue el mismo patrón que `NewModelWizard`:

```cpp
// newmodelwizard.cpp - on_cancelButtonClicked()
void NewModelWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && creationSuccessful) {
        accept();  // Acepta el diálogo cuando es exitoso
    } else {
        reject();  // Rechaza en caso contrario (cancelar)
    }
}

// mainwindow.cpp - on_actionEmpty_triggered()
NewModelWizard wizard(this);
if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
    // Load the newly created model
    openPraxisFile(wizard.getPdbFile());
}
```

Este patrón garantiza consistencia y facilita el mantenimiento futuro del código.
