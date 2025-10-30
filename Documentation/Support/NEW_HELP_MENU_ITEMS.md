# New Help Menu Items - Visual Preview

## Updated Help Menu Structure

### Complete Menu Layout

```
╔════════════════════════════════════════════════════════════╗
║                    MARRTA Application                      ║
╠════════════════════════════════════════════════════════════╣
║  Archivo  Modelo  Administración  Ayuda                    ║
║                                       ▼                     ║
║                        ┌──────────────────────────────────┐║
║                        │ ❓ Ayuda                    [NEW]│║ ◄── NEW (at top)
║                        │ ──────────────────────────────── │║
║                        │ ⏱  Comprobar actualizaciones...  │║
║                        │ 📁  Comprobar nuevos modelos...   │║
║                        │ ──────────────────────────────── │║
║                        │ 📤  Enviar comentario            │║
║                        │ 🔗  Enviar modelo de riesgo      │║
║                        │ ──────────────────────────────── │║
║                        │ ℹ️   Acerca de...                 │║
║                        │ 📋  Aviso legal            [NEW] │║ ◄── NEW (after Acerca de)
║                        │ 📖  Licencia de uso        [NEW] │║ ◄── NEW (after Aviso legal)
║                        └──────────────────────────────────┘║
╚════════════════════════════════════════════════════════════╝
```

## Menu Items Added

### 1. Ayuda (Help) - At Top

**Position:** First item in Help menu, with separator below  
**Icon:** help.svg (❓)  
**Text:** "Ayuda"  
**Tooltip:** "Abrir la documentación de ayuda de MARRTA"

**Dialog Preview:**
```
╔══════════════════════════════════════════════════════════════╗
║  Ayuda                                               [i]     ║
╠══════════════════════════════════════════════════════════════╣
║                                                              ║
║  La documentación de ayuda estará disponible próximamente.  ║
║                                                              ║
║  Por favor, consulte la documentación en el repositorio     ║
║  de GitHub:                                                 ║
║  https://github.com/risk-marrta/MARRTA                      ║
║                                                              ║
║                                              [Aceptar]       ║
╚══════════════════════════════════════════════════════════════╝
```

### 2. Aviso legal (Legal Notice) - After "Acerca de..."

**Position:** After "Acerca de..." item  
**Icon:** clipboard.svg (📋)  
**Text:** "Aviso legal"  
**Tooltip:** "Ver el aviso legal de MARRTA"

**Dialog Preview:**
```
╔══════════════════════════════════════════════════════════════╗
║  Aviso legal                                         [i]     ║
╠══════════════════════════════════════════════════════════════╣
║                                                              ║
║  MARRTA - Metodología de Análisis de Riesgos y Rendimiento  ║
║                                                              ║
║  Esta aplicación se proporciona "tal cual" sin garantías    ║
║  de ningún tipo.                                            ║
║                                                              ║
║  El uso de esta aplicación es bajo su propia                ║
║  responsabilidad.                                           ║
║                                                              ║
║  Para más información, consulte la documentación oficial    ║
║  en:                                                        ║
║  https://github.com/risk-marrta/MARRTA                      ║
║                                                              ║
║                                              [Aceptar]       ║
╚══════════════════════════════════════════════════════════════╝
```

### 3. Licencia de uso (Usage License) - After "Aviso legal"

**Position:** After "Aviso legal" item  
**Icon:** book.svg (📖)  
**Text:** "Licencia de uso"  
**Tooltip:** "Ver la licencia de uso de MARRTA"

**Dialog Preview:**
```
╔══════════════════════════════════════════════════════════════╗
║  Licencia de uso                                     [i]     ║
╠══════════════════════════════════════════════════════════════╣
║                                                              ║
║  MARRTA - Licencia de uso                                   ║
║                                                              ║
║  Esta aplicación está disponible para uso libre.            ║
║                                                              ║
║  Para obtener detalles completos sobre la licencia y        ║
║  términos de uso, consulte el archivo LICENSE en el         ║
║  repositorio:                                               ║
║  https://github.com/risk-marrta/MARRTA                      ║
║                                                              ║
║                                              [Aceptar]       ║
╚══════════════════════════════════════════════════════════════╝
```

## Before and After Comparison

### BEFORE (Help Menu)
```
Help (Ayuda)
├── Comprobar actualizaciones...
├── Comprobar nuevos modelos...
├── ──────────
├── Enviar comentario
├── Enviar modelo de riesgo
├── ──────────
└── Acerca de...
```

### AFTER (Help Menu with New Items)
```
Help (Ayuda)
├── Ayuda                          ◄── NEW
├── ──────────                     ◄── NEW separator
├── Comprobar actualizaciones...
├── Comprobar nuevos modelos...
├── ──────────
├── Enviar comentario
├── Enviar modelo de riesgo
├── ──────────
├── Acerca de...
├── Aviso legal                    ◄── NEW
└── Licencia de uso                ◄── NEW
```

## Technical Implementation

### Action Definitions (mainwindow.ui)

```xml
<action name="actionAyuda">
  <property name="icon">
    <iconset resource="icons.qrc">
      <normaloff>:/icons/resources/icons/help.svg</normaloff>
    </iconset>
  </property>
  <property name="text">
    <string>Ayuda</string>
  </property>
  <property name="toolTip">
    <string>Abrir la documentación de ayuda de MARRTA</string>
  </property>
</action>

<action name="actionAvisoLegal">
  <property name="icon">
    <iconset resource="icons.qrc">
      <normaloff>:/icons/resources/icons/clipboard.svg</normaloff>
    </iconset>
  </property>
  <property name="text">
    <string>Aviso legal</string>
  </property>
  <property name="toolTip">
    <string>Ver el aviso legal de MARRTA</string>
  </property>
</action>

<action name="actionLicenciaUso">
  <property name="icon">
    <iconset resource="icons.qrc">
      <normaloff>:/icons/resources/icons/book.svg</normaloff>
    </iconset>
  </property>
  <property name="text">
    <string>Licencia de uso</string>
  </property>
  <property name="toolTip">
    <string>Ver la licencia de uso de MARRTA</string>
  </property>
</action>
```

### Slot Implementations (mainwindow.cpp)

```cpp
void MainWindow::on_actionAyuda_triggered()
{
    QMessageBox::information(
        this,
        tr("Ayuda"),
        tr("La documentación de ayuda estará disponible próximamente.\n\n"
           "Por favor, consulte la documentación en el repositorio de GitHub:\n"
           "https://github.com/risk-marrta/MARRTA")
    );
}

void MainWindow::on_actionAvisoLegal_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Aviso legal"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("MARRTA - Metodología de Análisis de Riesgos y Rendimiento"));
    msgBox.setInformativeText(
        tr("Esta aplicación se proporciona \"tal cual\" sin garantías de ningún tipo.\n\n"
           "El uso de esta aplicación es bajo su propia responsabilidad.\n\n"
           "Para más información, consulte la documentación oficial en:\n"
           "https://github.com/risk-marrta/MARRTA")
    );
    msgBox.exec();
}

void MainWindow::on_actionLicenciaUso_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Licencia de uso"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("MARRTA - Licencia de uso"));
    msgBox.setInformativeText(
        tr("Esta aplicación está disponible para uso libre.\n\n"
           "Para obtener detalles completos sobre la licencia y términos de uso,\n"
           "consulte el archivo LICENSE en el repositorio:\n"
           "https://github.com/risk-marrta/MARRTA")
    );
    msgBox.exec();
}
```

## Features

### Translation Support
All text uses `tr()` for Qt translation:
- Menu item labels
- Tooltips
- Dialog titles and messages
- All user-facing text

### Icons
- **Ayuda:** help.svg - Question mark icon
- **Aviso legal:** clipboard.svg - Clipboard icon for legal documents
- **Licencia de uso:** book.svg - Book icon for license information

### User Experience
- Clear menu organization with logical grouping
- Help documentation easily accessible at the top
- Legal information grouped together at the bottom
- Tooltips provide additional context
- Information dialogs with links to GitHub repository

## Summary of Changes

**Commit:** `0683589` - Add new Help menu items: Ayuda, Aviso legal, and Licencia de uso

**Files Modified:**
- `mainwindow.ui` - Added 3 new action definitions and updated menu structure
- `mainwindow.h` - Added 3 new slot declarations
- `mainwindow.cpp` - Added 3 new slot implementations with information dialogs
- `RELEASE_INFO.md` - Documented changes

**Benefits:**
1. ✅ Users can easily access help documentation
2. ✅ Legal notice is readily available
3. ✅ License information is transparent and accessible
4. ✅ Better menu organization with logical grouping
5. ✅ All text is translatable for internationalization
6. ✅ Consistent with existing UI patterns and conventions
