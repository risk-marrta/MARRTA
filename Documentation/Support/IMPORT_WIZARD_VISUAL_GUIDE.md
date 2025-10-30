# Guía Visual - Optimización del Asistente de Importación

## Comparación Visual: Antes vs. Después

### ANTES: Proceso sin optimización ❌

```
┌─────────────────────────────────────────────────────────────┐
│ Usuario hace clic en "Importar desde Excel"                 │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │  ImportWizard se abre │
            │  (sin verificaciones) │
            └───────────┬───────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │  Usuario completa     │
            │  los 3 pasos          │
            └───────────┬───────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │  Conversión exitosa   │
            └───────────┬───────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │  Botón "Cerrar"       │
            │  [Click]              │
            └───────────┬───────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │  Wizard se cierra     │
            │  ⚠️ Usuario debe abrir│
            │  manualmente el .pdb  │
            └───────────────────────┘

⚠️ PROBLEMA: Si había una BD abierta, se pierden los cambios sin aviso
⚠️ PROBLEMA: El usuario debe buscar y abrir el archivo manualmente
```

### DESPUÉS: Proceso optimizado ✅

```
┌─────────────────────────────────────────────────────────────┐
│ Usuario hace clic en "Importar desde Excel"                 │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        ▼
            ┌───────────────────────┐
            │  ¿BD abierta?         │
            └─────┬────────┬────────┘
                  │ Sí     │ No
                  ▼        │
        ┌────────────────┐ │
        │  Diálogo:      │ │
        │  - Guardar     │ │
        │  - Descartar   │ │
        │  - Cancelar    │ │
        └─────┬──────────┘ │
              │            │
              ▼            ▼
    ┌──────────────────────────┐
    │  ImportWizard se abre    │
    └──────────┬───────────────┘
               │
               ▼
    ┌──────────────────────────┐
    │  Usuario completa        │
    │  los 3 pasos             │
    └──────────┬───────────────┘
               │
               ▼
    ┌──────────────────────────┐
    │  Conversión exitosa      │
    └──────────┬───────────────┘
               │
               ▼
    ┌──────────────────────────┐
    │  Botón "Terminar"        │
    │  [Click]                 │
    └──────────┬───────────────┘
               │
               ▼
    ┌──────────────────────────┐
    │  ✅ Wizard acepta        │
    │  ✅ Carga automática     │
    │  ✅ Pantalla actualizada │
    └──────────────────────────┘

✅ MEJORA: Los cambios se protegen
✅ MEJORA: Carga automática del nuevo modelo
```

## Diálogos de Usuario

### Diálogo 1: Base de Datos Abierta

```
╔═══════════════════════════════════════════════════╗
║  ⚠️  Base de datos abierta                       ║
╠═══════════════════════════════════════════════════╣
║                                                   ║
║  Hay una base de datos abierta actualmente.      ║
║                                                   ║
║  ¿Desea guardar los cambios antes de importar    ║
║  desde Excel?                                     ║
║                                                   ║
║                                                   ║
║     [ Guardar ]  [ Descartar ]  [ Cancelar ]     ║
║                                                   ║
╚═══════════════════════════════════════════════════╝
```

**Opciones:**
- **Guardar**: Guarda la BD actual, luego abre el wizard
- **Descartar**: Descarta cambios, luego abre el wizard  
- **Cancelar**: No abre el wizard, permanece en la BD actual

### Paso Final del Wizard: Conversión Exitosa

**ANTES:**
```
╔═══════════════════════════════════════════════════╗
║  Paso 4 de 4: Conversión                         ║
╠═══════════════════════════════════════════════════╣
║                                                   ║
║  ✓ ¡Conversión completada exitosamente!          ║
║                                                   ║
║  Archivo creado:                                  ║
║  C:\Users\...\mi_modelo.pdb                       ║
║                                                   ║
║                          [ Cerrar ]  ❌           ║
╚═══════════════════════════════════════════════════╝
          (cierra pero no carga)
```

**DESPUÉS:**
```
╔═══════════════════════════════════════════════════╗
║  Paso 4 de 4: Conversión                         ║
╠═══════════════════════════════════════════════════╣
║                                                   ║
║  ✓ ¡Conversión completada exitosamente!          ║
║                                                   ║
║  Archivo creado:                                  ║
║  C:\Users\...\mi_modelo.pdb                       ║
║                                                   ║
║                         [ Terminar ] ✅           ║
╚═══════════════════════════════════════════════════╝
        (cierra Y carga automáticamente)
```

## Flujo de Botones en el Wizard

```
┌─────────┬─────────┬─────────┬──────────────────┐
│ Paso 1  │ Paso 2  │ Paso 3  │ Paso 4           │
├─────────┼─────────┼─────────┼──────────────────┤
│         │         │         │                  │
│ Excel   │ Tipo de │ Destino │ Conversión       │
│ Origen  │ Práctica│ .pdb    │                  │
│         │         │         │                  │
├─────────┼─────────┼─────────┼──────────────────┤
│         │         │         │                  │
│ Botones:│ Botones:│ Botones:│ Botones:         │
│         │         │         │                  │
│ [Sig >] │[< Atr]  │[< Atr]  │ Si éxito:        │
│[Cancel] │[Sig >]  │[Iniciar]│  [Terminar] ✅   │
│         │[Cancel] │[Cancel] │                  │
│         │         │         │ Si fallo:        │
│         │         │         │  [Cancelar] ❌   │
└─────────┴─────────┴─────────┴──────────────────┘
```

## Estados del Botón Final

```
                   ┌─────────────────┐
                   │  Conversión     │
                   │  en progreso    │
                   └────────┬────────┘
                            │
                ┌───────────┴───────────┐
                │                       │
                ▼                       ▼
    ┌──────────────────┐    ┌──────────────────┐
    │   ÉXITO          │    │   FALLO          │
    │   ✓ exitoso      │    │   ✗ con errores  │
    └────────┬─────────┘    └────────┬─────────┘
             │                       │
             ▼                       ▼
    ┌──────────────────┐    ┌──────────────────┐
    │ "Terminar"       │    │ "Cancelar"       │
    │                  │    │                  │
    │ onClick():       │    │ onClick():       │
    │  accept()        │    │  reject()        │
    │  ↓               │    │  ↓               │
    │  openPraxisFile()│    │  (no hace nada)  │
    └──────────────────┘    └──────────────────┘
```

## Secuencia de Llamadas en el Código

### Iniciar Importación

```
MainWindow::on_actionImportarExcel_triggered()
    │
    ├─ praxisDb.isOpen() ?
    │   │
    │   └─ YES → QMessageBox::Save/Discard/Cancel
    │       │
    │       ├─ Save → saveModel()
    │       ├─ Discard → (continúa)
    │       └─ Cancel → return (sale)
    │
    └─ ImportWizard wizard(this)
        │
        └─ wizard.exec() → QDialog::Accepted ?
            │
            └─ YES && wizard.isSuccessful() ?
                │
                └─ YES → openPraxisFile(wizard.getPdbFile())
```

### Cerrar el Wizard

```
Usuario hace clic en botón de cerrar/terminar
    │
    ▼
ImportWizard::on_cancelButtonClicked()
    │
    ├─ currentStep == 3 && conversionSuccessful ?
    │   │
    │   ├─ YES → accept()
    │   │           │
    │   │           └─ QDialog retorna QDialog::Accepted
    │   │                   │
    │   │                   └─ MainWindow detecta Accepted
    │   │                           │
    │   │                           └─ openPraxisFile()
    │   │
    │   └─ NO → reject()
    │               │
    │               └─ QDialog retorna QDialog::Rejected
    │                       │
    │                       └─ MainWindow no hace nada
```

## Comparación con NewModelWizard

Ambos wizards ahora funcionan **idénticamente**:

```
┌────────────────────────┬──────────────────┬──────────────────┐
│ Característica         │ NewModelWizard   │ ImportWizard     │
├────────────────────────┼──────────────────┼──────────────────┤
│ Verificar BD abierta   │        ✅        │       ✅         │
├────────────────────────┼──────────────────┼──────────────────┤
│ Diálogo Save/Discard   │        ✅        │       ✅         │
├────────────────────────┼──────────────────┼──────────────────┤
│ Botón "Terminar"       │        ✅        │       ✅         │
├────────────────────────┼──────────────────┼──────────────────┤
│ accept() en éxito      │        ✅        │       ✅         │
├────────────────────────┼──────────────────┼──────────────────┤
│ Carga automática       │        ✅        │       ✅         │
├────────────────────────┼──────────────────┼──────────────────┤
│ Actualiza UI           │        ✅        │       ✅         │
└────────────────────────┴──────────────────┴──────────────────┘
```

## Líneas de Código Modificadas

### mainwindow.cpp

```cpp
// LÍNEAS AÑADIDAS: 22

void MainWindow::on_actionImportarExcel_triggered()
{
    // +++ NUEVO: Verificación de BD abierta
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
    }
    // --- FIN NUEVO
    
    ImportWizard wizard(this);
    
    // +++ MODIFICADO: Verificar aceptación y cargar
    if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
        openPraxisFile(wizard.getPdbFile());
    }
    // --- FIN MODIFICADO
}
```

### importwizard.cpp

```cpp
// LÍNEAS MODIFICADAS: 2
// LÍNEAS AÑADIDAS: 11

// En updateButtons():
cancelButton->setText(tr("Terminar"));  // ← Cambiado de "Cerrar"

// En constructor:
connect(ui->buttonBox, &QDialogButtonBox::rejected, 
        this, &ImportWizard::on_cancelButtonClicked);  // ← Nuevo handler

// Nueva función añadida:
void ImportWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && conversionSuccessful) {
        accept();  // ← Acepta en caso de éxito
    } else {
        reject();  // ← Rechaza en caso de cancelación
    }
}
```

### importwizard.h

```cpp
// LÍNEAS AÑADIDAS: 1

private slots:
    void on_btnBrowseExcel_clicked();
    void on_btnBrowsePdb_clicked();
    void on_btnNext_clicked();
    void on_btnBack_clicked();
    void on_rbOther_toggled(bool checked);
    void on_cancelButtonClicked();  // ← Nuevo slot
```

## Resultado Final

### Experiencia del Usuario Mejorada

```
ANTES:                              DESPUÉS:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1. Click en Importar               1. Click en Importar
                                       ↓
                                   2. ¿Guardar cambios?
                                      [Elige opción]
                                       ↓
2. Completa wizard                 3. Completa wizard
   ↓                                  ↓
3. "Cerrar"                        4. "Terminar"
   ↓                                  ↓
4. Cerrado                         5. ¡Modelo cargado! ✅
   ↓
5. Buscar archivo
   ↓
6. Abrir manualmente
   ↓
7. ¡Modelo cargado!

PASOS: 7                           PASOS: 5
MANUAL: Sí                         MANUAL: No
SEGURO: ⚠️                         SEGURO: ✅
```

## Resumen Visual

```
┌──────────────────────────────────────────────────────┐
│                OPTIMIZACIÓN COMPLETA                 │
├──────────────────────────────────────────────────────┤
│                                                      │
│  ✅ Protege cambios no guardados                    │
│  ✅ Carga automática del modelo                     │
│  ✅ Botón "Terminar" claro                          │
│  ✅ Consistente con NewModelWizard                  │
│  ✅ Experiencia de usuario mejorada                 │
│                                                      │
│  📝 3 archivos modificados                          │
│  📝 +32 líneas netas de código                      │
│  📝 3 documentos de guía creados                    │
│                                                      │
└──────────────────────────────────────────────────────┘
```
