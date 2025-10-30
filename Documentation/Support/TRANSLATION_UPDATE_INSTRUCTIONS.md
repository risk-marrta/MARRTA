# Translation Update Instructions

After implementing the new DlgGestionEtapas dialog, the translation files need to be updated to include all the new translatable strings.

## Steps to Update Translations

### 1. Run lupdate
```bash
cd /home/runner/work/MARRTA/MARRTA
lupdate marrta.pro
```

This will scan all source files for `tr()` calls and update the `.ts` files:
- `marrta_es.ts` - Spanish translations
- `marrta_en.ts` - English translations

### 2. Review and Translate (Optional)

Use Qt Linguist to review and edit translations:
```bash
linguist marrta_es.ts
linguist marrta_en.ts
```

Since the default language is Spanish, the Spanish translations should already be correct (the strings inside `tr()` are in Spanish).

For English translations, you would need to provide English equivalents for:

#### Dialog and Group Titles
- "Gestión de Etapas y Subetapas" → "Stage and Substage Management"
- "Detalles de la Etapa" → "Stage Details"
- "Subetapas" → "Substages"
- "Detalles de la Subetapa" → "Substage Details"

#### Labels
- "Etapas" → "Stages"
- "Nombre:" → "Name:"
- "Código:" → "Code:"
- "Descripción:" → "Description:"
- "Activa:" → "Active:"

#### Buttons
- "Nueva" → "New"
- "Eliminar" → "Delete"
- "Subir" → "Move Up"
- "Bajar" → "Move Down"
- "Cerrar" → "Close"

#### Placeholders
- "Nombre de la etapa (obligatorio)" → "Stage name (required)"
- "Código de 3 caracteres (obligatorio)" → "3-character code (required)"
- "Descripción de la etapa (opcional)" → "Stage description (optional)"
- "Nombre de la subetapa (obligatorio)" → "Substage name (required)"
- "Descripción de la subetapa (opcional)" → "Substage description (optional)"
- "La subetapa está activa" → "The substage is active"

#### Dialog Messages
- "Nueva Etapa" → "New Stage"
- "Nombre de la etapa:" → "Stage name:"
- "Código de la etapa (máximo 3 caracteres):" → "Stage code (max 3 characters):"
- "El código no puede tener más de 3 caracteres" → "The code cannot have more than 3 characters"
- "Confirmar eliminación" → "Confirm Deletion"
- "¿Está seguro de que desea eliminar la etapa '%1'?" → "Are you sure you want to delete stage '%1'?"
- "Esta acción eliminará también todas sus subetapas y no se puede deshacer." → "This action will also delete all its substages and cannot be undone."
- "Nueva Subetapa" → "New Substage"
- "Nombre de la subetapa:" → "Substage name:"
- "¿Está seguro de que desea eliminar la subetapa '%1'?" → "Are you sure you want to delete substage '%1'?"
- "Esta acción no se puede deshacer." → "This action cannot be undone."
- " (Inactiva)" → " (Inactive)"

#### Error Messages
- "Error" → "Error"
- "Error al cargar las etapas: %1" → "Error loading stages: %1"
- "Error al cargar las subetapas: %1" → "Error loading substages: %1"
- "Error al crear la etapa: %1" → "Error creating stage: %1"
- "Error al actualizar la etapa: %1" → "Error updating stage: %1"
- "Error al eliminar las subetapas: %1" → "Error deleting substages: %1"
- "Error al eliminar la etapa: %1" → "Error deleting stage: %1"
- "Error al actualizar el orden de la etapa: %1" → "Error updating stage order: %1"
- "Error al crear la subetapa: %1" → "Error creating substage: %1"
- "Error al actualizar la subetapa: %1" → "Error updating substage: %1"
- "Error al eliminar la subetapa: %1" → "Error deleting substage: %1"
- "Error al actualizar el orden de la subetapa: %1" → "Error updating substage order: %1"
- "Validación" → "Validation"
- "El nombre de la etapa es obligatorio" → "Stage name is required"
- "El código de la etapa es obligatorio" → "Stage code is required"
- "El nombre de la subetapa es obligatorio" → "Substage name is required"

### 3. Compile Translations
```bash
lrelease marrta.pro
```

This will generate `.qm` files that are embedded in the application via `translations.qrc`.

### 4. Verify Translations

Build and run MARRTA:
1. Default (Spanish): All strings should appear in Spanish
2. Switch to English (if supported): All strings should appear in English
3. Verify no untranslated strings appear

## New Strings Added

The DlgGestionEtapas implementation adds approximately 48 new translatable strings across:
- Window and group titles
- Field labels
- Button text
- Placeholder text
- Confirmation dialogs
- Error messages
- Status indicators

All strings follow the project's translation guidelines:
- Use `tr()` for all user-facing text
- Use `tr("text", "context")` when disambiguation needed
- Use `.arg()` for dynamic content
- Never hardcode language-specific strings

## Testing Translation

After updating translations:
1. Test with Spanish locale (default)
2. Test with English locale (if available)
3. Verify all UI elements are translated
4. Verify message boxes are translated
5. Verify placeholder texts are translated
6. Check for truncated text in small widgets
7. Verify special characters display correctly

## Maintenance

Whenever new features are added or text is changed in DlgGestionEtapas:
1. Run `lupdate` to extract new strings
2. Update translations in Qt Linguist
3. Run `lrelease` to compile
4. Test both languages
5. Commit updated `.ts` and `.qm` files
