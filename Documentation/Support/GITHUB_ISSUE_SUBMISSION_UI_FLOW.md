# GitHub Issue Submission UI Flow

## Menu Structure
```
┌─────────────────────────────────────────┐
│ MARRTA - Main Window                    │
├─────────────────────────────────────────┤
│ Archivo  Modelo  Administración  Ayuda  │
│                                  │      │
│                                  └──────┼─> Comprobar actualizaciones...
│                                         │
│                                         ├─> Comprobar nuevos modelos...
│                                         │
│                                         ├─> ──────────────── (separator)
│                                         │
│                                         ├─> Enviar comentario  [NEW]
│                                         │
│                                         ├─> Enviar modelo de riesgo  [NEW - Placeholder]
│                                         │
│                                         ├─> ──────────────── (separator)
│                                         │
│                                         └─> Acerca de...
```

## Dialog Layout (DlgSendFeedback)

```
┌────────────────────────────────────────────────────────────────────┐
│ Enviar comentario a MARRTA                                    [X]  │
├────────────────────────────────────────────────────────────────────┤
│ ┌────────────────────────────────────────────────────────────────┐ │
│ │ Tipo de comentario:                                            │ │
│ │   ⦿ Fallo/Bug                                                  │ │
│ │   ○ Comentario                                                 │ │
│ │   ○ Solicitud de nueva función                                 │ │
│ │                                                                │ │
│ │ Nombre:                                                        │ │
│ │ [_______________________________________________________]      │ │
│ │                                                                │ │
│ │ Institución:                                                   │ │
│ │ [_______________________________________________________]      │ │
│ │                                                                │ │
│ │ Email de contacto:                                             │ │
│ │ [_______________________________________________________]      │ │
│ │                                                                │ │
│ │ Descripción del asunto:                                        │ │
│ │ [                                                          ]   │ │
│ │ [                                                          ]   │ │
│ │ [                                                          ]   │ │
│ │                                                                │ │
│ ├────────────────────────────────────────────────────────────────┤ │
│ │ Detalles del error (solo para fallos/bugs)                    │ │
│ │                                                                │ │
│ │ Pasos para reproducirlo:                                       │ │
│ │ [                                                          ]   │ │
│ │ [                                                          ]   │ │
│ │                                                                │ │
│ │ Resultado esperado:                                            │ │
│ │ [                                                          ]   │ │
│ │                                                                │ │
│ │ Resultado obtenido:                                            │ │
│ │ [                                                          ]   │ │
│ └────────────────────────────────────────────────────────────────┘ │
│                                                                    │
│ ┌────────────────────────────────────────────────────────────────┐ │
│ │ Archivos adjuntos                                              │ │
│ │                                                                │ │
│ │ Imágenes/pantallazos y archivos .pbd:                          │ │
│ │ ┌────────────────────────────────────────────────────────────┐ │ │
│ │ │ screenshot1.png                                            │ │ │
│ │ │ model_error.pbd                                            │ │ │
│ │ └────────────────────────────────────────────────────────────┘ │ │
│ │                                                                │ │
│ │ [Añadir imagen...] [Añadir base de datos .pbd...]             │ │
│ │                    [Eliminar seleccionado]                     │ │
│ └────────────────────────────────────────────────────────────────┘ │
│                                                                    │
│ ┌────────────────────────────────────────────────────────────────┐ │
│ │ Nota: No se garantiza un tiempo de respuesta específico.      │ │
│ │ El tiempo de respuesta variará en función de la gravedad del  │ │
│ │ problema reportado. Se le notificará directamente al email    │ │
│ │ indicado cuando el tema esté resuelto.                         │ │
│ └────────────────────────────────────────────────────────────────┘ │
│                                                                    │
│                                             [Cancelar]  [Aceptar] │
└────────────────────────────────────────────────────────────────────┘
```

## User Flow Diagram

```
┌─────────────────┐
│ User selects    │
│ Help → Enviar   │
│ comentario      │
└────────┬────────┘
         │
         ▼
┌─────────────────────────────┐
│ DlgSendFeedback opens       │
│ - Form is empty             │
│ - Bug is selected by default│
│ - Bug fields are visible    │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐
│ User fills in form:         │
│ - Selects comment type      │
│ - Enters name, email, desc  │
│ - If bug: steps/expected/   │
│   actual results            │
│ - Optionally adds files     │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐
│ User clicks Aceptar         │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐     ┌──────────────────────┐
│ Validation:                 │────>│ Error?               │
│ - Name not empty?           │     │ Show warning dialog  │
│ - Email valid?              │     │ Focus on field       │
│ - Description not empty?    │     │ Return to form       │
│ - If bug: steps/exp/actual? │     └──────────────────────┘
└────────┬────────────────────┘
         │ Valid
         ▼
┌─────────────────────────────┐     ┌──────────────────────┐
│ Confirmation dialog:        │     │ User clicks "No"?    │
│ "¿Está seguro de que desea  │────>│ Return to form       │
│  enviar este comentario?"   │     └──────────────────────┘
└────────┬────────────────────┘
         │ Yes
         ▼
┌─────────────────────────────┐
│ Submit to GitHub API:       │
│ POST /repos/.../issues      │
│ - Build JSON payload        │
│ - Include title, body,      │
│   labels                    │
│ - Send network request      │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐     ┌──────────────────────────────┐
│ Response received           │     │ Error (401/403 auth)?        │
└────────┬────────────────────┘     │ - Show error dialog          │
         │                          │ - Offer manual submission    │
         │                          │ - Display formatted content  │
         │                          │ - Link to GitHub issues page │
         ├─────────────────────────>└──────────────────────────────┘
         │
         │ Success
         ▼
┌─────────────────────────────┐
│ Parse response:             │
│ - Extract issue URL         │
│ - Extract issue number      │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────────────────┐     ┌──────────────────────┐
│ Success dialog:             │     │ User clicks "Yes"?   │
│ "Su comentario ha sido      │────>│ Open issue URL in    │
│  enviado exitosamente"      │     │ default browser      │
│                             │     └──────────────────────┘
│ Show issue link             │
│ "¿Desea abrir el issue?"    │
└────────┬────────────────────┘
         │
         ▼
┌─────────────────┐
│ Dialog closes   │
│ Returns to main │
│ window          │
└─────────────────┘
```

## Bug Details Visibility Logic

```
User selects "Fallo/Bug"
    │
    ▼
groupBugDetails.setVisible(true)
    │
    └──> Show:
         - Pasos para reproducirlo (required)
         - Resultado esperado (required)
         - Resultado obtenido (required)

User selects "Comentario" or "Solicitud de nueva función"
    │
    ▼
groupBugDetails.setVisible(false)
    │
    └──> Hide bug-specific fields
         Validation skips these fields
```

## GitHub Issue Format

```
Title: [Bug] First line of description (max 100 chars)

Body:
## Información del remitente

**Nombre:** John Doe
**Institución:** Example University
**Email de contacto:** john.doe@example.com
**Versión de MARRTA:** 1.5 (2025-10-11)

## Descripción

[User's description text]

## Detalles del error

### Pasos para reproducir

[User's steps]

### Resultado esperado

[User's expected result]

### Resultado obtenido

[User's actual result]

## Archivos adjuntos

El usuario ha indicado los siguientes archivos adjuntos:
- screenshot1.png
- model_error.pbd

*Nota: Los archivos adjuntos no se pueden subir automáticamente 
a través de la API de GitHub. Por favor, contacte con el usuario 
en el email proporcionado para obtener los archivos.*

---
*Este issue fue creado automáticamente desde la aplicación MARRTA.*

Labels: bug, user-submitted
```

## Error Handling Flow

```
Network Request Fails
    │
    ├─> HTTP 401/403 (Authentication)
    │   │
    │   └──> Show error dialog with:
    │        - Explanation about auth requirement
    │        - Link to manual submission
    │        - Button to show formatted content
    │        - Content dialog with copyable text
    │
    ├─> Other HTTP errors (500, 404, etc.)
    │   │
    │   └──> Show error dialog with:
    │        - Error message
    │        - Link to manual submission
    │        - Suggestion to try again later
    │
    └─> Network errors (timeout, DNS, etc.)
        │
        └──> Show error dialog with:
             - Error description
             - Link to manual submission
             - Suggestion to check connection
```

## Translation Keys

All strings in the dialog are marked for translation:
- Window title: "Enviar comentario"
- Form labels: "Nombre:", "Institución:", etc.
- Radio buttons: "Fallo/Bug", "Comentario", etc.
- Button labels: "Añadir imagen...", "Eliminar seleccionado", etc.
- Validation messages: "Por favor, ingrese su nombre.", etc.
- Success/error dialogs: All message text uses tr()

Translation files:
- Spanish: marrta_es.ts (default)
- English: marrta_en.ts
