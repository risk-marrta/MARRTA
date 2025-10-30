# Optimización del Asistente de Importación desde Excel

## Resumen de Cambios Implementados

Se han implementado dos mejoras principales en el asistente de importación desde Excel para que coincida con el comportamiento del asistente de creación de nuevos modelos:

### 1. ✅ Aviso al abrir el asistente con base de datos abierta

**Implementación:**
- Al invocar el asistente de importación, se comprueba si hay una base de datos abierta (`praxisDb.isOpen()`)
- Se muestra un diálogo con icono de advertencia (QMessageBox::Warning)
- Opciones: "Guardar", "Descartar cambios", "Cancelar"
- Si el usuario elige "Guardar", se llama a `saveModel()` antes de continuar
- Si elige "Cancelar", el asistente no se abre
- Si elige "Descartar cambios", el asistente se abre sin guardar

**Código:**
```cpp
void MainWindow::on_actionImportarExcel_triggered()
{
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
            return;
        }
        // If Discard, continue without saving
    }
    
    ImportWizard wizard(this);
    if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
        // Load the newly imported model
        openPraxisFile(wizard.getPdbFile());
    }
}
```

**Ubicación del código:**
- Archivo: `mainwindow.cpp`
- Función: `MainWindow::on_actionImportarExcel_triggered()`

### 2. ✅ Terminar y cargar modelo

**Implementación:**
- Después de importar el Excel exitosamente, el botón cambia de "Cerrar" a "Terminar"
- Al hacer clic en "Terminar":
  - El diálogo se acepta (en lugar de rechazarse)
  - MainWindow detecta la aceptación exitosa
  - Carga automáticamente el modelo recién importado usando `openPraxisFile()`

**Flujo:**
```cpp
// En ImportWizard::updateButtons()
if (cancelButton) {
    if (currentStep == 3 && conversionSuccessful) {
        cancelButton->setText(tr("Terminar"));  // Cambiado de "Cerrar"
    } else {
        cancelButton->setText(tr("Cancelar"));
    }
}

// En ImportWizard::on_cancelButtonClicked()
void ImportWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && conversionSuccessful) {
        accept();  // Acepta el diálogo cuando es exitoso
    } else {
        reject();  // Rechaza en caso contrario (cancelar)
    }
}

// En MainWindow::on_actionImportarExcel_triggered()
ImportWizard wizard(this);
if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
    // Carga el modelo recién importado
    openPraxisFile(wizard.getPdbFile());
}
```

**Archivos modificados:**
- `mainwindow.cpp`: Lógica para cargar el modelo después de la importación exitosa
- `importwizard.h`: Declaración del slot `on_cancelButtonClicked()`
- `importwizard.cpp`: 
  - Cambio de texto del botón a "Terminar"
  - Implementación del slot `on_cancelButtonClicked()`
  - Conexión del signal rejected al slot personalizado

## Consistencia con NewModelWizard

Estos cambios hacen que el `ImportWizard` funcione de manera idéntica al `NewModelWizard`, proporcionando una experiencia de usuario consistente:

| Característica | NewModelWizard | ImportWizard |
|---------------|----------------|--------------|
| Aviso si hay BD abierta | ✅ | ✅ |
| Opción de guardar cambios | ✅ | ✅ |
| Botón "Terminar" al finalizar | ✅ | ✅ |
| Carga automática del nuevo modelo | ✅ | ✅ |
| Actualización de la pantalla principal | ✅ | ✅ |

## Beneficios

1. **Prevención de pérdida de datos**: El usuario es avisado antes de que se cierre la base de datos actual
2. **Flujo de trabajo mejorado**: El modelo importado se carga automáticamente sin pasos adicionales
3. **Consistencia**: Ambos asistentes (crear y importar) funcionan de la misma manera
4. **Claridad**: El botón "Terminar" indica claramente que la operación ha finalizado exitosamente

## Pruebas Recomendadas

Para verificar que la implementación funciona correctamente:

1. **Prueba de aviso al abrir con BD abierta:**
   - Abrir una base de datos existente
   - Ir a Archivo → Importar desde Excel
   - Verificar que aparece el diálogo de advertencia
   - Probar las tres opciones: Guardar, Descartar, Cancelar

2. **Prueba de carga automática:**
   - Completar el asistente de importación exitosamente
   - Verificar que el botón dice "Terminar" (no "Cerrar")
   - Hacer clic en "Terminar"
   - Verificar que se carga el modelo importado
   - Verificar que la pantalla principal muestra los datos del nuevo modelo

3. **Prueba de cancelación:**
   - Abrir el asistente de importación
   - Hacer clic en "Cancelar" en cualquier paso
   - Verificar que no se carga ningún modelo
   - Verificar que la base de datos actual (si la hay) sigue abierta
