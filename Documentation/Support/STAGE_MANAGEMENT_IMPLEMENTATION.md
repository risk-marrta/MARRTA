# Implementation Summary: New Stage Management Dialog

## Overview

This document describes the implementation of a completely new stage/substage management window (`DlgGestionEtapas`) that replaces the previous separate dialogs (`Etapas` and `Subetapas`). The new design provides an integrated, dynamic interface where all changes are saved directly to the database as they are made.

## Problem Statement

The original issue requested:
- A redesigned window for managing stages (etapas) and substages (subetapas)
- Better design and more dynamic interface
- Direct database persistence (no "Save" button needed)
- Ability to create, delete, and reorder both stages and substages
- Required fields: Stage Name and Code; Substage Name
- Optional fields: Stage and Substage Descriptions
- Substages should have an "Active" checkbox
- On closing, the model should be reloaded in MainWindow

## Solution Design

### Architecture

The new solution consists of:

1. **Single Integrated Dialog**: `DlgGestionEtapas` combines stage and substage management in one window
2. **Immediate Persistence**: All edits are saved to the database as they happen
3. **Split Panel Layout**: 
   - Left panel shows the stage list
   - Right panel shows stage details and substage management
4. **Database-Driven**: Uses existing `DbElementStage` and `DbElementSubstage` classes

### UI Layout

```
┌─────────────────────────────────────────────────────────┐
│  Gestión de Etapas y Subetapas                         │
├──────────────────┬──────────────────────────────────────┤
│ Etapas           │ Detalles de la Etapa                 │
│ ┌──────────────┐ │ ┌────────────────────────────────┐   │
│ │ Stage 1      │ │ │ Nombre: [__________________]   │   │
│ │ Stage 2      │◄┼─┤ Código: [___]                  │   │
│ │ Stage 3      │ │ │ Descripción:                   │   │
│ │              │ │ │ [________________________]     │   │
│ └──────────────┘ │ └────────────────────────────────┘   │
│ [Nueva][Eliminar]│                                       │
│ [Subir][Bajar]   │ Subetapas                            │
│                  │ ┌────────────────────────────────┐   │
│                  │ │ Substage 1                     │   │
│                  │ │ Substage 2                     │   │
│                  │ └────────────────────────────────┘   │
│                  │ [Nueva][Eliminar][Subir][Bajar]      │
│                  │                                       │
│                  │ Detalles de la Subetapa              │
│                  │ ┌────────────────────────────────┐   │
│                  │ │ Nombre: [__________________]   │   │
│                  │ │ Descripción:                   │   │
│                  │ │ [________________________]     │   │
│                  │ │ ☑ La subetapa está activa      │   │
│                  │ └────────────────────────────────┘   │
└──────────────────┴──────────────────────────────────────┤
                                             [Cerrar]      │
└──────────────────────────────────────────────────────────┘
```

## Implementation Details

### Files Created

1. **dlggestionetapas.h** - Header file with class definition
2. **dlggestionetapas.cpp** - Implementation with all logic
3. **dlggestionetapas.ui** - Qt Designer UI layout file

### Files Modified

1. **mainwindow.h** - Added include for new dialog
2. **mainwindow.cpp** - Updated `on_actionEtapas_triggered()` to use new dialog
3. **marrta.pro** - Added new files to build system

### Key Features

#### Stage Management
- **Create**: Dialog prompts for Name and Code (both required, Code max 3 chars)
- **Delete**: Confirmation dialog, cascades to delete all substages
- **Reorder**: Up/Down buttons swap order with adjacent stages
- **Edit**: Name, Code, and Description editable inline with immediate save

#### Substage Management
- **Create**: Dialog prompts for Name (required)
- **Delete**: Confirmation dialog
- **Reorder**: Up/Down buttons swap order with adjacent substages
- **Edit**: Name, Description, and Active status editable inline with immediate save
- **Display**: Inactive substages shown with "(Inactiva)" suffix

#### Database Operations

All database operations happen immediately:

1. **Stage Insert**:
   ```sql
   INSERT INTO etapas (nombre, descripcion, codigo, orden)
   VALUES (:nombre, :descripcion, :codigo, :orden)
   ```

2. **Stage Update**:
   ```sql
   UPDATE etapas 
   SET nombre = :nombre, descripcion = :descripcion, codigo = :codigo
   WHERE id = :id
   ```

3. **Stage Delete** (cascades):
   ```sql
   DELETE FROM subetapas WHERE etapa_id = :stageId;
   DELETE FROM etapas WHERE id = :id;
   ```

4. **Order Update** (for both stages and substages):
   ```sql
   UPDATE etapas SET orden = :orden WHERE id = :id;
   UPDATE subetapas SET orden = :orden WHERE id = :id;
   ```

#### Translation Support

All user-facing strings use Qt's `tr()` function for internationalization:
- Window title: `tr("Gestión de Etapas y Subetapas")`
- Buttons: `tr("Nueva")`, `tr("Eliminar")`, `tr("Subir")`, `tr("Bajar")`
- Labels: `tr("Nombre:")`, `tr("Código:")`, `tr("Descripción:")`
- Messages: `tr("Error al crear la etapa: %1")`
- Placeholders: `tr("Nombre de la etapa (obligatorio)")`

### Technical Implementation

#### Preventing Recursive Updates

The dialog uses an `updatingUI` flag to prevent recursive signal firing when programmatically updating UI elements:

```cpp
void DlgGestionEtapas::showStageDetails()
{
    updatingUI = true;  // Prevent signals while updating UI
    ui->edNombreEtapa->setText(stage.nombre);
    // ... other updates ...
    updatingUI = false;
}

void DlgGestionEtapas::on_edNombreEtapa_textChanged(const QString &text)
{
    if (updatingUI || selectedStageIndex < 0) {
        return;  // Skip if updating UI or no selection
    }
    // ... save to database ...
}
```

#### Transaction Management for Ordering

Order changes require updating two records atomically:

```cpp
QSqlDatabase::database().transaction();
bool success = true;
success = success && updateStageOrder(stages[i].id, newOrder1);
success = success && updateStageOrder(stages[i+1].id, newOrder2);
if (success) {
    QSqlDatabase::database().commit();
} else {
    QSqlDatabase::database().rollback();
}
```

#### Selection Handling

Uses Qt's selection model with custom handling:

```cpp
connect(ui->listEtapas->selectionModel(), 
        &QItemSelectionModel::selectionChanged,
        this, &DlgGestionEtapas::onStageSelectionChanged);
```

Each item stores its array index in UserRole for quick lookup.

## Benefits of New Design

1. **Better UX**: Single window for all stage/substage operations
2. **Immediate Feedback**: Changes saved instantly, no "Save" button needed
3. **Clearer Structure**: Visual hierarchy shows stage-substage relationship
4. **Less Code**: No need to pass data between dialogs
5. **Safer**: Immediate database writes with transaction support
6. **More Maintainable**: Single class instead of two separate dialogs
7. **Internationalization Ready**: All strings wrapped with tr()

## Backward Compatibility

The old `Etapas` and `Subetapas` classes remain in the codebase but are no longer used by MainWindow. They can be removed in a future cleanup if no other code references them.

The database schema remains unchanged, so existing databases work without migration.

## Testing Recommendations

1. **Create Stage**: Verify name and code are required, code limited to 3 chars
2. **Edit Stage**: Verify inline editing saves to database
3. **Delete Stage**: Verify confirmation and cascade delete of substages
4. **Reorder Stages**: Verify up/down buttons work correctly
5. **Create Substage**: Verify name is required
6. **Edit Substage**: Verify inline editing saves, including active checkbox
7. **Delete Substage**: Verify confirmation
8. **Reorder Substages**: Verify up/down buttons work correctly
9. **Selection**: Verify substage list updates when selecting different stages
10. **Close**: Verify MainWindow reloads model after closing dialog

## Future Enhancements

Possible improvements for future versions:
- Drag and drop reordering (instead of buttons)
- Inline editing in list views
- Undo/Redo support
- Bulk operations (delete multiple, import/export)
- Keyboard shortcuts (Ctrl+N for new, Delete key, etc.)
- Search/filter functionality
- Duplicate stage/substage

## Related Files

- `/home/runner/work/MARRTA/MARRTA/dlggestionetapas.h` - Header
- `/home/runner/work/MARRTA/MARRTA/dlggestionetapas.cpp` - Implementation
- `/home/runner/work/MARRTA/MARRTA/dlggestionetapas.ui` - UI Layout
- `/home/runner/work/MARRTA/MARRTA/mainwindow.cpp` - Integration point
- `/home/runner/work/MARRTA/MARRTA/dbelementstage.h/cpp` - Database layer for stages
- `/home/runner/work/MARRTA/MARRTA/dbelementsubstage.h/cpp` - Database layer for substages

## Notes

- All database operations use prepared statements for SQL injection safety
- Error messages are user-friendly and translated
- The dialog properly cleans up resources in destructor
- Qt parent-child ownership handles most memory management
