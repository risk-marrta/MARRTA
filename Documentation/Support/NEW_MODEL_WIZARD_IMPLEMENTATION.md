# New Model Creation Wizard Implementation

## Overview
This document describes the implementation of the new model creation wizard (Asistente para la creación de nuevos modelos) as requested in the issue.

## Files Created

### 1. `newmodelwizard.h`
Header file defining the `NewModelWizard` class which extends `QDialog`. Contains:
- Structure `StageInfo` to store stage information with substages
- Dialog class with all necessary slots for UI interaction
- Private members to store model configuration data

### 2. `newmodelwizard.cpp`
Implementation file with the following key methods:
- **Constructor**: Initializes the wizard, sets default date to current date
- **updateButtons()**: Manages button visibility and states based on current step
- **validateCurrentStep()**: Validates input on each step before proceeding
- **addStageDialog()**: Shows dialog to add a new stage with its code
- **addSubstageDialog()**: Shows dialogs to add substages to a stage
- **startCreation()**: Initiates model database creation
- **createModelDatabase()**: Creates the SQLite database with all required tables and data

### 3. `newmodelwizard.ui`
Qt Designer UI file with 4 wizard pages:

#### Page 1: File Location
- File selection dialog for .pdb file location
- Browse button to open file save dialog

#### Page 2: Basic Model Information
- Form with all configuration fields:
  - Nombre (Name) - required
  - Fecha (Date) - defaults to current date, editable
  - Técnica (Technique)
  - Analistas (Analysts) - multi-line text
  - Institución (Institution)
  - Descripción (Description) - multi-line text
  - Observaciones (Observations) - multi-line text

#### Page 3: Process Stages (Optional)
- Question: "¿Quiere añadir las etapas del proceso?"
- Tree widget to display stages and substages hierarchically
- Add Stage button - opens dialog for stage name and code
- Remove Stage button - removes selected stage or substage
- When adding a stage, asks if user wants to add substages
- Continues asking for substages until user declines

#### Page 4: Creation Progress
- Progress bar showing creation status
- Status label with messages
- Automatically starts creation when reaching this page

## Integration with MainWindow

Modified `mainwindow.cpp`:
- Added include for `newmodelwizard.h`
- Updated `on_actionEmpty_triggered()` to launch the new wizard instead of the old simple file dialog
- Wizard is launched from menu: Archivo > Modelo > Crear...

## Database Structure

The wizard creates a complete MARRTA database with the following tables:
1. **configuracion**: Stores model metadata (version, nombre, fecha, técnica, analistas, institución, descripción, observaciones)
2. **etapas**: Stores process stages with id, nombre, orden, activo, codigo
3. **subetapas**: Stores substages linked to stages
4. **suceso_iniciador**: Events table (empty initially)
5. **defensa**: Defenses table (empty initially)

All stages and substages entered by the user are inserted into the database with proper ordering.

## User Flow

1. User selects "Archivo > Modelo > Crear..." from menu
2. Wizard opens showing Step 1: File location selection
3. User clicks "Examinar" and selects where to save .pdb file
4. User clicks "Siguiente" to proceed to Step 2
5. User fills in basic model information (nombre is required, fecha defaults to today)
6. User clicks "Siguiente" to proceed to Step 3
7. User decides whether to add stages:
   - If yes, clicks "Añadir Etapa"
   - Enters stage name and code
   - Wizard asks if they want to add substages
   - User can add multiple substages
   - Process repeats for additional stages
8. User clicks "Crear modelo" to proceed to Step 4
9. Wizard creates the database automatically
10. Success or error message is shown
11. User clicks "Cerrar" to exit wizard

## Key Features

- **Aesthetics similar to ImportWizard**: Uses same style with QStackedWidget for wizard pages
- **Default current date**: fecha field automatically set to today's date
- **Optional stages**: User can skip stage creation if desired
- **Interactive substage collection**: Continues asking for substages until user declines
- **Proper validation**: Ensures required fields are filled before proceeding
- **Database integrity**: Creates complete database structure matching existing MARRTA models
- **User-friendly dialogs**: Clear prompts and error messages in Spanish
