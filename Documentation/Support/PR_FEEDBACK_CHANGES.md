# Updated Dialog Preview - Changes from PR Feedback

## Change 1: Enhanced Privacy Warning Dialog

### NEW Confirmation Dialog (Before Submission)

```
╔══════════════════════════════════════════════════════════════════════╗
║  Información importante                                         [i]  ║
╠══════════════════════════════════════════════════════════════════════╣
║                                                                      ║
║  La información remitida por este medio estará visible a otros      ║
║  usuarios y públicamente en https://github.com/risk-marrta/MARRTA.  ║
║                                                                      ║
║  ¿Desea continuar y remitir su comentario?                          ║
║                                                                      ║
║                                            [No*]          [Sí]      ║
╚══════════════════════════════════════════════════════════════════════╝

* Default button is "No" for safety
```

### OLD Confirmation Dialog (Replaced)

```
╔══════════════════════════════════════════════════════════════╗
║  Confirmar envío                                        [?]  ║
╠══════════════════════════════════════════════════════════════╣
║                                                              ║
║  ¿Está seguro de que desea enviar este comentario?          ║
║                                                              ║
║  Se creará un issue público en el repositorio de GitHub     ║
║  de MARRTA.                                                  ║
║                                                              ║
║                                        [No]          [Sí]   ║
╚══════════════════════════════════════════════════════════════╝
```

**Key Changes:**
- ✅ More explicit warning about public visibility
- ✅ Information icon instead of question icon
- ✅ Clearer title: "Información importante"
- ✅ Default button is "No" for safety
- ✅ Direct mention of visibility to "otros usuarios"

---

## Change 2: File Links Instead of File Upload

### NEW - File Links Section

```
╔════════════════════════════════════════════════════════════════════╗
║ Enlaces a archivos adjuntos                                        ║
╠════════════════════════════════════════════════════════════════════╣
║                                                                    ║
║ ℹ️  Nota: Para adjuntar archivos (imágenes, pantallazos, bases    ║
║    de datos .pbd), súbalos primero a un servicio de hospedaje    ║
║    de archivos online (Google Drive, iCloud, OneDrive, Dropbox,   ║
║    etc.) y proporcione los enlaces en el campo de descripción     ║
║    o en la lista a continuación.                                  ║
║                                                                    ║
║ Enlaces a archivos:                                                ║
║ ┌────────────────────────────────────────────────────────────────┐ ║
║ │ https://drive.google.com/file/d/xxx/screenshot.png            │ ║
║ │ https://1drv.ms/u/s!xxx/model_error.pbd                       │ ║
║ └────────────────────────────────────────────────────────────────┘ ║
║                                                                    ║
║ [Añadir enlace...]  [Eliminar seleccionado]                       ║
║                                                                    ║
╚════════════════════════════════════════════════════════════════════╝
```

### OLD - File Upload Section (Replaced)

```
╔════════════════════════════════════════════════════════════════════╗
║ Archivos adjuntos                                                  ║
╠════════════════════════════════════════════════════════════════════╣
║                                                                    ║
║ Imágenes/pantallazos y archivos .pbd:                              ║
║ ┌────────────────────────────────────────────────────────────────┐ ║
║ │ screenshot1.png                                                │ ║
║ │ model_error.pbd                                                │ ║
║ └────────────────────────────────────────────────────────────────┘ ║
║                                                                    ║
║ [Añadir imagen...]  [Añadir base de datos .pbd...]                ║
║                     [Eliminar seleccionado]                        ║
║                                                                    ║
╚════════════════════════════════════════════════════════════════════╝
```

**Key Changes:**
- ✅ Title changed to "Enlaces a archivos adjuntos"
- ✅ Prominent note explaining users must upload files first to hosting services
- ✅ Mentions specific services: Google Drive, iCloud, OneDrive, Dropbox
- ✅ Single "Añadir enlace..." button instead of two file selection buttons
- ✅ Links are displayed as full URLs (not just filenames)
- ✅ Users provide links instead of selecting local files

### Link Input Dialog (NEW)

When user clicks "Añadir enlace...":

```
╔══════════════════════════════════════════════════════════════════════╗
║  Añadir enlace a archivo                                             ║
╠══════════════════════════════════════════════════════════════════════╣
║                                                                      ║
║  Ingrese el enlace al archivo hospedado                             ║
║  (Google Drive, OneDrive, iCloud, Dropbox, etc.):                   ║
║                                                                      ║
║  ┌──────────────────────────────────────────────────────────────┐   ║
║  │ https://                                                     │   ║
║  └──────────────────────────────────────────────────────────────┘   ║
║                                                                      ║
║                                          [Cancelar]      [Aceptar]  ║
╚══════════════════════════════════════════════════════════════════════╝
```

---

## Benefits of Changes

### Privacy Warning Enhancement
1. **More explicit** - Users clearly understand their information will be public
2. **Safer default** - "No" button is default, preventing accidental submissions
3. **Better UX** - Information icon and clearer wording improve understanding

### File Links Instead of Uploads
1. **API limitation addressed** - GitHub API doesn't support file uploads anyway
2. **User responsibility** - Users manage their own file hosting and privacy
3. **Simpler implementation** - No file handling, encoding, or size limits
4. **More flexible** - Users can use their preferred hosting service
5. **Direct access** - Links in issue work immediately without manual intervention
6. **Better privacy control** - Users control who has access to their files

## Technical Implementation

### Confirmation Dialog Changes
```cpp
// OLD
QMessageBox::StandardButton reply = QMessageBox::question(
    this,
    tr("Confirmar envío"),
    tr("¿Está seguro de que desea enviar este comentario?\n\n"
       "Se creará un issue público en el repositorio de GitHub de MARRTA."),
    QMessageBox::Yes | QMessageBox::No
);

// NEW
QMessageBox msgBox(this);
msgBox.setWindowTitle(tr("Información importante"));
msgBox.setIcon(QMessageBox::Information);
msgBox.setText(tr("La información remitida por este medio estará visible a otros usuarios y públicamente en https://github.com/risk-marrta/MARRTA."));
msgBox.setInformativeText(tr("¿Desea continuar y remitir su comentario?"));
msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
msgBox.setDefaultButton(QMessageBox::No);  // Safety default
```

### File Links Implementation
```cpp
// OLD - File selection
void DlgSendFeedback::on_btnAddImage_clicked() {
    QStringList filePaths = QFileDialog::getOpenFileNames(...);
    // Process file paths
}

// NEW - Link input
void DlgSendFeedback::on_btnAddLink_clicked() {
    QString link = QInputDialog::getText(
        this,
        tr("Añadir enlace a archivo"),
        tr("Ingrese el enlace al archivo hospedado\n(Google Drive, OneDrive, iCloud, Dropbox, etc.):"),
        QLineEdit::Normal,
        QString(),
        &ok
    );
    if (ok && !link.trimmed().isEmpty()) {
        attachmentLinks.append(link.trimmed());
        ui->listAttachments->addItem(link.trimmed());
    }
}
```

### Issue Body Generation
```cpp
// OLD
if (!attachmentPaths.isEmpty()) {
    body += tr("## Archivos adjuntos\n\n");
    body += tr("El usuario ha indicado los siguientes archivos adjuntos:\n");
    for (const QString &path : attachmentPaths) {
        QFileInfo fileInfo(path);
        body += QString("- %1\n").arg(fileInfo.fileName());
    }
    body += tr("\n*Nota: Los archivos adjuntos no se pueden subir automáticamente...");
}

// NEW
if (!attachmentLinks.isEmpty()) {
    body += tr("## Enlaces a archivos adjuntos\n\n");
    body += tr("El usuario ha proporcionado los siguientes enlaces a archivos:\n");
    for (const QString &link : attachmentLinks) {
        body += QString("- %1\n").arg(link);
    }
}
```

## Example GitHub Issue with Links

```markdown
## Información del remitente

**Nombre:** Juan Pérez
**Institución:** Universidad de Madrid
**Email de contacto:** juan.perez@example.com
**Versión de MARRTA:** 1.5 (2025-10-11)

## Descripción

Al intentar calcular el riesgo, la aplicación se cierra inesperadamente.

## Detalles del error

### Pasos para reproducir

1. Abrir modelo de ejemplo
2. Ir a la sección de cálculo de riesgo
3. Hacer clic en "Calcular"

### Resultado esperado

La aplicación debe mostrar los resultados del cálculo.

### Resultado obtenido

La aplicación se cierra sin mensaje de error.

## Enlaces a archivos adjuntos

El usuario ha proporcionado los siguientes enlaces a archivos:
- https://drive.google.com/file/d/1234567890/error_screenshot.png
- https://1drv.ms/u/s!AbCdEfGhIjKlMnOp/model_database.pbd

---
*Este issue fue creado automáticamente desde la aplicación MARRTA.*
```

## Summary of Changes

**Commit:** `851786e` - Update feedback dialog: add privacy warning and change to file links instead of uploads

**Changes Made:**
1. ✅ Enhanced privacy warning in confirmation dialog
2. ✅ Changed file uploads to link input system
3. ✅ Added prominent note about uploading to hosting services
4. ✅ Simplified UI with single "Añadir enlace" button
5. ✅ Removed unused file upload code
6. ✅ Updated RELEASE_INFO.md with changes
7. ✅ All text properly uses tr() for translation

**Files Modified:**
- `dlgsendfeedback.ui` - Updated UI with new labels and link input
- `dlgsendfeedback.h` - Changed variable names and method signatures
- `dlgsendfeedback.cpp` - Replaced file dialogs with link input, updated confirmation
- `RELEASE_INFO.md` - Documented changes at top of unreleased section
