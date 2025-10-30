# Cambios según Feedback del Review

## Resumen de Cambios Implementados

Todos los puntos del feedback han sido implementados correctamente:

### 1. ✅ Aviso al abrir el asistente con base de datos abierta

**Implementación:**
- Al invocar el asistente, se comprueba si hay una base de datos abierta (`praxisDb.isOpen()`)
- Se muestra un diálogo con icono de advertencia (QMessageBox::Warning)
- Opciones: "Guardar", "Descartar cambios", "Cancelar"
- Si el usuario elige "Guardar", se llama a `saveModel()` antes de continuar
- Si elige "Cancelar", el asistente no se abre

**Código:**
```cpp
void MainWindow::on_actionEmpty_triggered()
{
    // Check if there's a database open
    if (praxisDb.isOpen() && !settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Base de datos abierta"));
        msgBox.setText(tr("Hay una base de datos abierta actualmente."));
        msgBox.setInformativeText(tr("¿Desea guardar los cambios antes de crear un nuevo modelo?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        
        int ret = msgBox.exec();
        if (ret == QMessageBox::Save) {
            saveModel();
        } else if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    ...
}
```

### 2. ✅ Título en negrita para campo obligatorio

**Implementación:**
- El campo "Nombre del modelo" ahora aparece en negrita usando HTML
- Indica claramente que es un campo obligatorio

**Código en UI:**
```xml
<widget class="QLabel" name="lblNombre">
    <property name="text">
        <string>&lt;b&gt;Nombre del modelo:&lt;/b&gt;</string>
    </property>
</widget>
```

### 3. ✅ Botones más anchos

**Implementación:**
- Botón "Añadir Etapa": ancho mínimo 150px
- Botón "Eliminar Etapa": ancho mínimo 150px
- Botón "Subir": ancho mínimo 100px
- Botón "Bajar": ancho mínimo 100px

**Código en UI:**
```xml
<widget class="QPushButton" name="btnAddStage">
    <property name="minimumSize">
        <size>
            <width>150</width>
            <height>0</height>
        </size>
    </property>
    ...
</widget>
```

### 4. ✅ Diálogo combinado para añadir etapas

**Implementación:**
- Un único diálogo pregunta por: Nombre, Código y Descripción
- Solo la Descripción puede quedar en blanco
- Usa QFormLayout con campos apropiados:
  - Nombre: QLineEdit (obligatorio)
  - Código: QLineEdit (obligatorio)
  - Descripción: QPlainTextEdit (opcional)

**Estructura del diálogo:**
```
┌─────────────────────────────┐
│ Añadir Etapa                │
├─────────────────────────────┤
│ Nombre:       [________]    │
│ Código:       [________]    │
│ Descripción:  [________]    │
│               [________]    │
│                             │
│        [Cancel]  [OK]       │
└─────────────────────────────┘
```

### 5. ✅ Diálogo combinado para añadir subetapas

**Implementación:**
- Un único diálogo pregunta por: Nombre y Descripción
- Solo la Descripción puede quedar en blanco
- Usa QFormLayout con campos apropiados:
  - Nombre: QLineEdit (obligatorio)
  - Descripción: QPlainTextEdit (opcional)

**Estructura del diálogo:**
```
┌─────────────────────────────┐
│ Añadir Subetapa             │
├─────────────────────────────┤
│ Nombre:       [________]    │
│ Descripción:  [________]    │
│               [________]    │
│                             │
│        [Cancel]  [OK]       │
└─────────────────────────────┘
```

### 6. ✅ Permitir modificar el orden

**Implementación:**
- Añadidos botones "Subir" y "Bajar"
- Funcionan tanto para etapas como para subetapas
- Al seleccionar un elemento en el árbol:
  - "Subir" mueve el elemento una posición arriba
  - "Bajar" mueve el elemento una posición abajo
- El orden final en el árbol se respeta al crear la base de datos

**Funcionalidad:**
```cpp
void NewModelWizard::on_btnMoveUp_clicked()
{
    QTreeWidgetItem *currentItem = ui->treeStages->currentItem();
    if (!currentItem) return;
    
    if (currentItem->parent() == nullptr) {
        // Es una etapa de nivel superior
        int index = ui->treeStages->indexOfTopLevelItem(currentItem);
        if (index > 0) {
            QTreeWidgetItem *item = ui->treeStages->takeTopLevelItem(index);
            ui->treeStages->insertTopLevelItem(index - 1, item);
            ui->treeStages->setCurrentItem(item);
        }
    } else {
        // Es una subetapa
        QTreeWidgetItem *parent = currentItem->parent();
        int index = parent->indexOfChild(currentItem);
        if (index > 0) {
            QTreeWidgetItem *item = parent->takeChild(index);
            parent->insertChild(index - 1, item);
            ui->treeStages->setCurrentItem(item);
        }
    }
}
```

### 7. ✅ Terminar y cargar modelo

**Implementación:**
- Después de crear la base de datos exitosamente, el botón cambia de "Cerrar" a "Terminar"
- Al hacer clic en "Terminar":
  - El diálogo se acepta (en lugar de rechazarse)
  - MainWindow detecta la aceptación exitosa
  - Carga automáticamente el modelo recién creado usando `openPraxisFile()`

**Flujo:**
```cpp
// En NewModelWizard::on_cancelButtonClicked()
void NewModelWizard::on_cancelButtonClicked()
{
    if (currentStep == 3 && creationSuccessful) {
        accept();  // Acepta el diálogo cuando es exitoso
    } else {
        reject();  // Rechaza en caso contrario (cancelar)
    }
}

// En MainWindow::on_actionEmpty_triggered()
NewModelWizard wizard(this);
if (wizard.exec() == QDialog::Accepted && wizard.isSuccessful()) {
    // Carga el modelo recién creado
    openPraxisFile(wizard.getPdbFile());
}
```

## Almacenamiento de Descripciones

Las descripciones se almacenan correctamente:

**Para Etapas:**
```sql
INSERT INTO etapas (nombre, descripcion, orden, activo, codigo) 
VALUES(?, ?, ?, 1, ?)
```

**Para Subetapas:**
```sql
INSERT INTO subetapas (nombre, descripcion, orden, activo, etapa_id) 
VALUES(?, ?, ?, 1, ?)
```

Las descripciones se guardan en `Qt::UserRole` del QTreeWidgetItem y se recuperan al crear la base de datos.

## Validaciones

- **Nombre y Código de Etapa**: Obligatorios (se muestra error si faltan)
- **Nombre de Subetapa**: Obligatorio (se muestra error si falta)
- **Descripciones**: Opcionales (pueden quedar en blanco)

## Archivos Modificados

1. **newmodelwizard.h**: Añadida estructura `SubstageInfo`, slots para move up/down y cancelButton
2. **newmodelwizard.cpp**: Implementación completa de todas las funcionalidades
3. **newmodelwizard.ui**: Botones más anchos, label en negrita, botones de movimiento
4. **mainwindow.cpp**: Verificación de base de datos abierta y carga automática del modelo

## Commits

- `b0ba435`: Implementación principal (warn, bold, wider buttons, dialogs, move buttons)
- `39c4331`: Botón "Terminar" y carga automática del modelo
