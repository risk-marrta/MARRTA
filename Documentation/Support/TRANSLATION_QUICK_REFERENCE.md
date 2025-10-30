# Qt Translation Quick Reference - MARRTA

## TL;DR - The Golden Rule

**ALWAYS use `tr()` for ANY text that appears on screen!**

## Quick Decision Tree

```
Is this text shown to the user in the UI?
├─ YES → Use tr("text")
└─ NO  → Is it a database field, debug log, or internal ID?
         ├─ YES → Don't use tr()
         └─ NO  → When in doubt, use tr()
```

## Common Cases

### ✅ ALWAYS Use tr()

```cpp
// QMessageBox
QMessageBox::warning(this, tr("Error"), tr("Mensaje"));
msgBox.setText(tr("Texto"));
msgBox.setInformativeText(tr("Información adicional"));

// Widget text
ui->label->setText(tr("Etiqueta"));
ui->button->setText(tr("Aceptar"));
ui->widget->setToolTip(tr("Descripción"));
setWindowTitle(tr("Título de ventana"));

// Dynamic text with placeholders
QString msg = tr("Archivo: %1").arg(filename);
ui->status->setText(tr("Total: %1 de %2").arg(n).arg(total));

// Button creation
QPushButton *btn = new QPushButton(tr("Guardar"));
```

### ❌ DON'T Use tr()

```cpp
// Database fields
query.value("nombre")
QString sql = "SELECT * FROM tabla WHERE id = :id";

// Internal identifiers
QString key = "internal_key";
settings.setValue("app/version", "1.0");

// Debug/log messages (optional)
qDebug() << "Debug message";

// Data format codes (not UI)
QString code = "FMB";  // Frequency code
QString risk = "RA";   // Risk code
```

## Quick Patterns

### Simple Text
```cpp
ui->label->setText(tr("Mi texto"));
```

### Text with One Variable
```cpp
QString msg = tr("Usuario: %1").arg(username);
```

### Text with Multiple Variables
```cpp
QString msg = tr("Página %1 de %2").arg(current).arg(total);
```

### Numbers and Formatting
```cpp
// ✅ Correct
ui->label->setText(tr("Cantidad: %1").arg(count));

// ❌ Wrong
ui->label->setText("Cantidad: " + QString::number(count));
```

## Translation Workflow

```bash
# 1. Write code with tr()
ui->label->setText(tr("Nuevo texto"));

# 2. Extract strings
lupdate marrta.pro

# 3. (Optional) Edit translations
linguist marrta_es.ts

# 4. Compile
lrelease marrta.pro

# 5. Test
./marrta
```

## Common Mistakes

### ❌ Mistake 1: Forgetting tr()
```cpp
// Wrong
QMessageBox::warning(this, "Error", "Algo salió mal");

// Correct
QMessageBox::warning(this, tr("Error"), tr("Algo salió mal"));
```

### ❌ Mistake 2: String Concatenation
```cpp
// Wrong
setText("Total: " + QString::number(n) + " items");

// Correct
setText(tr("Total: %1 items").arg(n));
```

### ❌ Mistake 3: Translating Internal Data
```cpp
// Wrong
query.bindValue(":nombre", tr("nombre"));

// Correct
query.bindValue(":nombre", "nombre");
```

## Special Cases

### Context for Disambiguation
When the same word has different meanings:
```cpp
tr("Open", "verb - as in open a file")
tr("Open", "adjective - as in door is open")
```

### HTML/Markdown Content
```cpp
// If the markup contains visible text, use tr()
QString html = tr("<b>Texto en negrita</b>");

// Pure formatting without visible text
QString style = "<style>color: red;</style>";  // No tr() needed
```

### Dynamic Button Text
```cpp
// Dialog buttons
ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Aceptar"));
ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));
```

## Checklist Before Committing

- [ ] All QMessageBox messages use tr()
- [ ] All setText() calls use tr()
- [ ] All setToolTip() calls use tr()
- [ ] All button text uses tr()
- [ ] All window titles use tr()
- [ ] Dynamic text uses tr("text %1").arg(value)
- [ ] No hardcoded Spanish in UI code
- [ ] Only internal identifiers lack tr()

## Files Already Following Best Practices

Good examples to follow:
- `importwizard.cpp` - Excellent tr() usage throughout
- Check these for patterns to emulate

## Files Needing Updates

Examples with hardcoded text (to avoid this pattern):
- `mainwindow.cpp` - Many hardcoded messages
- `resumen.cpp` - Button text without tr()
- `defensas.cpp` - QMessageBox without tr()

## Testing Translation

```bash
# Build with translations
qmake marrta.pro
make

# Run and check UI displays Spanish correctly
./marrta

# To test English (requires translator loading in main.cpp)
# Set language in marrta.ini: language=en
./marrta
```

## Resources

- **Full Guidelines**: `TRANSLATION_GUIDELINES_IMPLEMENTATION.md`
- **Copilot Instructions**: `.github/copilot-instructions.md`
- **Qt Docs**: https://doc.qt.io/qt-5/internationalization.html
- **Translation Files**: `marrta_es.ts`, `marrta_en.ts`

---

**Remember**: When in doubt, use `tr()`. It's better to have an unnecessary translation marker than to miss one!
