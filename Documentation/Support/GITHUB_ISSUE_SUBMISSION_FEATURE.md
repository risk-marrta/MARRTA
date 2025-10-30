# GitHub Issue Submission Feature

## Overview

This feature allows users to submit feedback, bug reports, and feature requests directly from MARRTA to the GitHub repository at https://github.com/risk-marrta/MARRTA/issues.

## Menu Location

The feature is accessible from the **Help** menu (Ayuda):
- **Enviar comentario** - Opens the feedback submission dialog
- **Enviar modelo de riesgo** - Placeholder for future implementation

## Dialog Features

### Comment Types
Users can select from three types of feedback:
1. **Fallo/Bug** - Report a bug or error
2. **Comentario** - General comment or feedback
3. **Solicitud de nueva función** - Feature request

### Required Fields
- **Name** (Nombre)
- **Email** (Email de contacto) - With basic validation
- **Description** (Descripción del asunto)

### Optional Fields
- **Institution** (Institución)

### Bug-Specific Fields
When "Fallo/Bug" is selected, additional required fields are shown:
- **Steps to reproduce** (Pasos para reproducirlo)
- **Expected result** (Resultado esperado)
- **Actual result** (Resultado obtenido)

### File Attachments
Users can attach:
- Images/screenshots (png, jpg, jpeg, gif, bmp)
- MARRTA database files (.pbd)

**Note**: Due to GitHub API limitations, attachments are listed in the issue body but cannot be automatically uploaded. The issue will note that attachments are available upon request from the user.

## GitHub Integration

### API Endpoint
Issues are created at: `https://api.github.com/repos/risk-marrta/MARRTA/issues`

### Issue Format
Each submitted issue includes:
- **Title**: Prefixed with [Bug], [Feature Request], or [Comentario]
- **Body**: Contains sender information, description, bug details (if applicable), and version info
- **Labels**: Automatically assigned based on comment type (bug, enhancement, question) plus "user-submitted"

### Authentication
The current implementation attempts to create issues without authentication. If GitHub requires authentication (HTTP 401/403), the dialog provides a fallback option to:
1. Display the formatted issue content
2. Direct users to manually create the issue at GitHub
3. Allow copying the content for manual submission

### Success Response
Upon successful submission:
- Shows confirmation message
- Displays the direct link to the created issue
- Offers to open the issue in the default web browser

## Translation Support

All UI text uses Qt's `tr()` function for internationalization:
- Strings are extracted to `marrta_es.ts` and `marrta_en.ts`
- Supports Spanish (default) and English translations
- Update translations with: `lupdate marrta.pro`
- Compile translations with: `lrelease marrta.pro`

## Implementation Files

### New Files
- `dlgsendfeedback.h` - Dialog class header
- `dlgsendfeedback.cpp` - Dialog implementation with validation and API calls
- `dlgsendfeedback.ui` - Qt Designer form

### Modified Files
- `mainwindow.ui` - Added menu actions
- `mainwindow.h` - Added slot declarations
- `mainwindow.cpp` - Added slot implementations and dialog include
- `marrta.pro` - Added new files to build configuration
- `RELEASE_INFO.md` - Documented changes

## User Workflow

1. User selects **Help → Enviar comentario**
2. Dialog opens with feedback form
3. User fills in required fields
4. For bugs, user provides reproduction steps and expected/actual results
5. User can optionally attach files
6. User clicks OK to submit
7. Confirmation dialog asks for approval
8. Issue is submitted to GitHub
9. Success message shows with link to the created issue
10. User can optionally open the issue in browser

## Error Handling

- **Form Validation**: Required fields are validated before submission
- **Email Validation**: Basic check for @ and . characters
- **Network Errors**: Displays user-friendly error messages
- **Authentication Errors**: Provides fallback for manual submission
- **API Errors**: Shows error message with option to try again or submit manually

## Future Enhancements

Potential improvements:
1. Implement "Enviar modelo de riesgo" feature
2. Add GitHub authentication token support for automatic submission
3. Implement automatic file upload to GitHub or external hosting
4. Add screenshot capture tool directly in the dialog
5. Save draft submissions for later
6. Track submission status and notifications within MARRTA

## Disclaimer

As noted in the dialog:
> "No se garantiza un tiempo de respuesta específico. El tiempo de respuesta variará en función de la gravedad del problema reportado. Se le notificará directamente al email indicado cuando el tema esté resuelto."

Translation: "No specific response time is guaranteed. Response time will vary based on the severity of the reported issue. You will be notified directly at the provided email when the issue is resolved."
