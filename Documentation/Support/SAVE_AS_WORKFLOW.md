# Save As Workflow Diagram

## User Interaction Flow

```
                                    START
                                      |
                                      v
                        ┌──────────────────────────────┐
                        │  User clicks "Guardar como"  │
                        │     in Archivo menu          │
                        └──────────────────────────────┘
                                      |
                                      v
                        ┌──────────────────────────────┐
                        │   Is database open?          │
                        │   settings.lastFile exists?  │
                        └──────────────────────────────┘
                                      |
                            ┌─────────┴─────────┐
                            NO                  YES
                            |                    |
                            v                    v
                ┌─────────────────────┐   ┌──────────────────────┐
                │ Show warning:       │   │ Save pending changes │
                │ "No hay modelo      │   │ DbEvent.checkAndUp   │
                │  cargado"           │   │       date()         │
                └─────────────────────┘   └──────────────────────┘
                            |                    |
                            v                    v
                          END          ┌──────────────────────────┐
                                      │  Show Save File Dialog   │
                                      │  - Filter: *.pdb         │
                                      │  - Default: databaseFolder│
                                      └──────────────────────────┘
                                                  |
                                      ┌───────────┴───────────┐
                                      |                       |
                                   CANCEL                    OK
                                      |                       |
                                      v                       v
                                    END          ┌─────────────────────────┐
                                                │ Add .pdb extension      │
                                                │ if not present          │
                                                └─────────────────────────┘
                                                            |
                                                            v
                                                ┌─────────────────────────┐
                                                │  Does file exist?       │
                                                └─────────────────────────┘
                                                            |
                                                ┌───────────┴───────────┐
                                                NO                     YES
                                                |                       |
                                                v                       v
                                                |           ┌──────────────────────┐
                                                |           │ Ask confirmation:    │
                                                |           │ "¿Desea sobreescri-  │
                                                |           │  birlo?"             │
                                                |           └──────────────────────┘
                                                |                       |
                                                |           ┌───────────┴───────┐
                                                |           NO                 YES
                                                |           |                   |
                                                |           v                   v
                                                |         END       ┌────────────────────┐
                                                |                   │ Remove existing    │
                                                |                   │ file               │
                                                |                   └────────────────────┘
                                                |                           |
                                                |                           v
                                                |                   ┌────────────────────┐
                                                |                   │ Remove successful? │
                                                |                   └────────────────────┘
                                                |                           |
                                                |               ┌───────────┴────────┐
                                                |               NO                  YES
                                                |               |                    |
                                                |               v                    |
                                                |   ┌───────────────────┐           |
                                                |   │ Show error:       │           |
                                                |   │ "No se pudo       │           |
                                                |   │  eliminar..."     │           |
                                                |   └───────────────────┘           |
                                                |               |                    |
                                                |               v                    |
                                                |             END                    |
                                                |                                    |
                                                └────────────────┬───────────────────┘
                                                                 |
                                                                 v
                                                    ┌────────────────────────┐
                                                    │ Copy database file     │
                                                    │ QFile::copy()          │
                                                    └────────────────────────┘
                                                                 |
                                                    ┌────────────┴────────────┐
                                                    |                         |
                                                  SUCCESS                   FAIL
                                                    |                         |
                                                    v                         v
                                        ┌─────────────────────┐   ┌──────────────────────┐
                                        │ Show success:       │   │ Show error:          │
                                        │ "El modelo se ha    │   │ "No se pudo guardar  │
                                        │  guardado en: X"    │   │  el archivo..."      │
                                        └─────────────────────┘   └──────────────────────┘
                                                    |                         |
                                                    v                         v
                                                  END                       END
```

## Key Decision Points

1. **Database Check**: Prevents Save As when no model is loaded
2. **User Cancellation**: Respects user's choice to cancel at file dialog
3. **Overwrite Protection**: Asks for confirmation before overwriting existing files
4. **Error Handling**: Provides feedback at each failure point

## Message Types

- **Warning** (Yellow): No model loaded
- **Question** (Blue): Confirm file overwrite
- **Critical** (Red): File operation errors
- **Information** (Blue): Successful save confirmation
