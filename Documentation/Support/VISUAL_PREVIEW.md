# Visual Preview: GitHub Issue Submission Feature

## Menu Preview

When users open the **Help** (Ayuda) menu in MARRTA, they will see:

```
╔════════════════════════════════════════════════════════════╗
║                    MARRTA Application                      ║
╠════════════════════════════════════════════════════════════╣
║  Archivo  Modelo  Administración  Ayuda                    ║
║                                       ▼                     ║
║                        ┌──────────────────────────────────┐║
║                        │ ⏱  Comprobar actualizaciones...  │║
║                        │ 📁  Comprobar nuevos modelos...   │║
║                        │ ──────────────────────────────── │║
║                        │ 📤  Enviar comentario      [NEW] │║ ◄── New Item
║                        │ 🔗  Enviar modelo de riesgo      │║ ◄── New Item (Placeholder)
║                        │ ──────────────────────────────── │║
║                        │ ℹ️   Acerca de...                 │║
║                        └──────────────────────────────────┘║
╚════════════════════════════════════════════════════════════╝
```

## Dialog Preview - Default State (Bug Selected)

```
╔══════════════════════════════════════════════════════════════════════════╗
║  Enviar comentario a MARRTA                                         [X]  ║
╠══════════════════════════════════════════════════════════════════════════╣
║ ┌────────────────────────────────────────────────────────────────────┐ ║
║ │                     SCROLLABLE CONTENT AREA                         │ ║
║ │ ┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄│ ║
║ │                                                                    │ ║
║ │ Tipo de comentario:                                                │ ║
║ │   ⦿ Fallo/Bug                                                      │ ║
║ │   ○ Comentario                                                     │ ║
║ │   ○ Solicitud de nueva función                                     │ ║
║ │                                                                    │ ║
║ │ Nombre: *                                                          │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │                                                              │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ Institución:                                                       │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │                                                              │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ Email de contacto: *                                               │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │ email@example.com                                            │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ Descripción del asunto: *                                          │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │                                                              │  │ ║
║ │ │                                                              │  │ ║
║ │ │                                                              │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ ╔════════════════════════════════════════════════════════════════╗│ ║
║ │ ║ Detalles del error (solo para fallos/bugs)                    ║│ ║
║ │ ╠════════════════════════════════════════════════════════════════╣│ ║
║ │ ║ Pasos para reproducirlo: *                                     ║│ ║
║ │ ║ ┌────────────────────────────────────────────────────────────┐ ║│ ║
║ │ ║ │                                                            │ ║│ ║
║ │ ║ │                                                            │ ║│ ║
║ │ ║ └────────────────────────────────────────────────────────────┘ ║│ ║
║ │ ║                                                                ║│ ║
║ │ ║ Resultado esperado: *                                          ║│ ║
║ │ ║ ┌────────────────────────────────────────────────────────────┐ ║│ ║
║ │ ║ │                                                            │ ║│ ║
║ │ ║ └────────────────────────────────────────────────────────────┘ ║│ ║
║ │ ║                                                                ║│ ║
║ │ ║ Resultado obtenido: *                                          ║│ ║
║ │ ║ ┌────────────────────────────────────────────────────────────┐ ║│ ║
║ │ ║ │                                                            │ ║│ ║
║ │ ║ └────────────────────────────────────────────────────────────┘ ║│ ║
║ │ ╚════════════════════════════════════════════════════════════════╝│ ║
║ │                                                                    │ ║
║ │ ╔════════════════════════════════════════════════════════════════╗│ ║
║ │ ║ Archivos adjuntos                                              ║│ ║
║ │ ╠════════════════════════════════════════════════════════════════╣│ ║
║ │ ║ Imágenes/pantallazos y archivos .pbd:                          ║│ ║
║ │ ║ ┌────────────────────────────────────────────────────────────┐ ║│ ║
║ │ ║ │                                                            │ ║│ ║
║ │ ║ │           (List of attached files appears here)           │ ║│ ║
║ │ ║ │                                                            │ ║│ ║
║ │ ║ └────────────────────────────────────────────────────────────┘ ║│ ║
║ │ ║                                                                ║│ ║
║ │ ║ [Añadir imagen...] [Añadir base de datos .pbd...]             ║│ ║
║ │ ║                    [Eliminar seleccionado]                     ║│ ║
║ │ ╚════════════════════════════════════════════════════════════════╝│ ║
║ │                                                                    │ ║
║ │ ┌────────────────────────────────────────────────────────────────┐│ ║
║ │ │ ℹ️  Nota: No se garantiza un tiempo de respuesta específico.  ││ ║
║ │ │    El tiempo de respuesta variará en función de la gravedad   ││ ║
║ │ │    del problema reportado. Se le notificará directamente al   ││ ║
║ │ │    email indicado cuando el tema esté resuelto.                ││ ║
║ │ └────────────────────────────────────────────────────────────────┘│ ║
║ │                                                                    │ ║
║ └────────────────────────────────────────────────────────────────────┘ ║
║                                                                        ║
║                                         [Cancelar]      [Aceptar]     ║
╚══════════════════════════════════════════════════════════════════════════╝

* = Required field
```

## Dialog Preview - Comment Selected (Bug Fields Hidden)

```
╔══════════════════════════════════════════════════════════════════════════╗
║  Enviar comentario a MARRTA                                         [X]  ║
╠══════════════════════════════════════════════════════════════════════════╣
║ ┌────────────────────────────────────────────────────────────────────┐ ║
║ │                                                                    │ ║
║ │ Tipo de comentario:                                                │ ║
║ │   ○ Fallo/Bug                                                      │ ║
║ │   ⦿ Comentario                                                     │ ║
║ │   ○ Solicitud de nueva función                                     │ ║
║ │                                                                    │ ║
║ │ Nombre: *                                                          │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │                                                              │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ Institución:                                                       │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │                                                              │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ Email de contacto: *                                               │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │ email@example.com                                            │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ Descripción del asunto: *                                          │ ║
║ │ ┌──────────────────────────────────────────────────────────────┐  │ ║
║ │ │                                                              │  │ ║
║ │ │                                                              │  │ ║
║ │ │                                                              │  │ ║
║ │ └──────────────────────────────────────────────────────────────┘  │ ║
║ │                                                                    │ ║
║ │ ╔════════════════════════════════════════════════════════════════╗│ ║
║ │ ║ Archivos adjuntos                                              ║│ ║
║ │ ╠════════════════════════════════════════════════════════════════╣│ ║
║ │ ║ Imágenes/pantallazos y archivos .pbd:                          ║│ ║
║ │ ║ ┌────────────────────────────────────────────────────────────┐ ║│ ║
║ │ ║ │                                                            │ ║│ ║
║ │ ║ └────────────────────────────────────────────────────────────┘ ║│ ║
║ │ ║                                                                ║│ ║
║ │ ║ [Añadir imagen...] [Añadir base de datos .pbd...]             ║│ ║
║ │ ║                    [Eliminar seleccionado]                     ║│ ║
║ │ ╚════════════════════════════════════════════════════════════════╝│ ║
║ │                                                                    │ ║
║ │ ┌────────────────────────────────────────────────────────────────┐│ ║
║ │ │ ℹ️  Nota: No se garantiza un tiempo de respuesta específico... ││ ║
║ │ └────────────────────────────────────────────────────────────────┘│ ║
║ │                                                                    │ ║
║ └────────────────────────────────────────────────────────────────────┘ ║
║                                                                        ║
║                                         [Cancelar]      [Aceptar]     ║
╚══════════════════════════════════════════════════════════════════════════╝

Note: Bug details section is HIDDEN when "Comentario" or "Solicitud de nueva 
función" is selected.
```

## Confirmation Dialog

When user clicks "Aceptar":

```
╔════════════════════════════════════════════════════════════╗
║  Confirmar envío                                      [?]  ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  ¿Está seguro de que desea enviar este comentario?        ║
║                                                            ║
║  Se creará un issue público en el repositorio de GitHub   ║
║  de MARRTA.                                                ║
║                                                            ║
║                                          [No]      [Sí]   ║
╚════════════════════════════════════════════════════════════╝
```

## Success Dialog

After successful submission:

```
╔════════════════════════════════════════════════════════════╗
║  Comentario enviado                                   [i]  ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  Su comentario ha sido enviado exitosamente.              ║
║                                                            ║
║  Puede ver y seguir el progreso en:                       ║
║  https://github.com/risk-marrta/MARRTA/issues/XXX         ║
║                                                            ║
║  ¿Desea abrir el issue en su navegador?                   ║
║                                                            ║
║                                          [No]      [Sí]   ║
╚════════════════════════════════════════════════════════════╝
```

## Error Dialog (Authentication Required)

If GitHub requires authentication:

```
╔════════════════════════════════════════════════════════════╗
║  Error de autenticación                               [X]  ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  No se pudo crear el issue automáticamente porque         ║
║  GitHub requiere autenticación.                           ║
║                                                            ║
║  Por favor, cree el issue manualmente visitando:          ║
║  https://github.com/risk-marrta/MARRTA/issues/new         ║
║                                                            ║
║  Puede copiar el contenido de este formulario y           ║
║  pegarlo en el issue.                                     ║
║                                                            ║
║                                                 [Aceptar]  ║
╚════════════════════════════════════════════════════════════╝

Followed by:

╔════════════════════════════════════════════════════════════╗
║  Contenido del issue                                  [i]  ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  Copie el siguiente contenido para crear el issue         ║
║  manualmente:                                             ║
║                                                            ║
║  [Show Details ▼]                                          ║
║  ┌────────────────────────────────────────────────────┐   ║
║  │ Título: [Bug] Description first line...           │   ║
║  │                                                    │   ║
║  │ ## Información del remitente                       │   ║
║  │ **Nombre:** John Doe                               │   ║
║  │ **Email:** john@example.com                        │   ║
║  │ ...                                                │   ║
║  └────────────────────────────────────────────────────┘   ║
║                                                            ║
║                                                 [Aceptar]  ║
╚════════════════════════════════════════════════════════════╝
```

## Validation Error Example

If required field is empty:

```
╔════════════════════════════════════════════════════════════╗
║  Error                                                [!]  ║
╠════════════════════════════════════════════════════════════╣
║                                                            ║
║  Por favor, ingrese su nombre.                            ║
║                                                            ║
║                                                 [Aceptar]  ║
╚════════════════════════════════════════════════════════════╝

(Focus returns to the Name field)
```

## File Selection Dialog

When clicking "Añadir imagen...":

```
╔════════════════════════════════════════════════════════════════════╗
║  Seleccionar imágenes                                         [X]  ║
╠════════════════════════════════════════════════════════════════════╣
║                                                                    ║
║  [Buscar en: v] Documentos                             [⌂] [↑]   ║
║  ┌────────────────────────────────────────────────────────────┐   ║
║  │  screenshot1.png                                           │   ║
║  │  error_dialog.png                                          │   ║
║  │  calculation_bug.jpg                                       │   ║
║  └────────────────────────────────────────────────────────────┘   ║
║                                                                    ║
║  Archivo:  screenshot1.png                                        ║
║                                                                    ║
║  Tipo:  Imágenes (*.png *.jpg *.jpeg *.gif *.bmp) [v]            ║
║                                                                    ║
║                                           [Cancelar]  [Abrir]     ║
╚════════════════════════════════════════════════════════════════════╝
```

## Feature Highlights

### Visual Cues
- 📤 Upload icon for "Enviar comentario"
- 🔗 Share icon for "Enviar modelo de riesgo"
- ⦿ Radio button selection indicator
- * Asterisk marks required fields
- Scrollable content area for long forms
- Grouped sections with visual borders

### Responsive Behavior
- Bug details section shows/hides based on comment type
- File list updates as attachments are added/removed
- Form validation provides immediate feedback
- Error messages focus on relevant field

### User Experience
- Clear field labels in Spanish
- Helpful placeholder text (e.g., "email@example.com")
- Comprehensive disclaimer at bottom
- Confirmation before submission
- Success feedback with actionable link
- Graceful error handling with alternatives
