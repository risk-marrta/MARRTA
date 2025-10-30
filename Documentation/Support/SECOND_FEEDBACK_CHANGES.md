# Segunda Ronda de Feedback - Cambios Implementados

## Commit: 79e3756

### 1. ✅ Etiquetas en Negrita para Campos Obligatorios

#### Diálogo de Añadir Etapa

**ANTES:**
```
┌────────────────────────────────────┐
│ Añadir Etapa                       │
├────────────────────────────────────┤
│  Nombre:       [________________]  │
│  Código:       [________________]  │
│  Descripción:  ┌──────────────┐   │
│                │              │   │
│                └──────────────┘   │
│         [Cancel]  [OK]             │
└────────────────────────────────────┘
```

**DESPUÉS:**
```
┌────────────────────────────────────┐
│ Añadir Etapa                       │
├────────────────────────────────────┤
│  **Nombre:**   [________________]  │  ← EN NEGRITA
│  **Código:**   [________________]  │  ← EN NEGRITA
│  Descripción:  ┌──────────────┐   │
│                │              │   │
│                └──────────────┘   │
│         [Cancel]  [OK]             │
└────────────────────────────────────┘
```

#### Diálogo de Añadir Subetapa

**ANTES:**
```
┌────────────────────────────────────┐
│ Añadir Subetapa                    │
├────────────────────────────────────┤
│  Nombre:       [________________]  │
│  Descripción:  ┌──────────────┐   │
│                │              │   │
│                └──────────────┘   │
│         [Cancel]  [OK]             │
└────────────────────────────────────┘
```

**DESPUÉS:**
```
┌────────────────────────────────────┐
│ Añadir Subetapa                    │
├────────────────────────────────────┤
│  **Nombre:**   [________________]  │  ← EN NEGRITA
│  Descripción:  ┌──────────────┐   │
│                │              │   │
│                └──────────────┘   │
│         [Cancel]  [OK]             │
└────────────────────────────────────┘
```

**Implementación:**
```cpp
// En lugar de pasar strings directamente a formLayout->addRow()
// Ahora creamos QLabel explícitos con HTML para negrita

QLabel *nameLabel = new QLabel(tr("<b>Nombre:</b>"), &dialog);
QLabel *codeLabel = new QLabel(tr("<b>Código:</b>"), &dialog);
QLabel *descLabel = new QLabel(tr("Descripción:"), &dialog);

formLayout->addRow(nameLabel, nameEdit);
formLayout->addRow(codeLabel, codeEdit);
formLayout->addRow(descLabel, descEdit);
```

### 2. ✅ Diálogos NO se Cierran con Error de Validación

#### Problema Original:

Cuando el usuario dejaba un campo obligatorio vacío y hacía clic en OK:

```
1. Usuario hace clic en OK
2. dialog.accept() se ejecuta → diálogo se cierra
3. Validación falla después de cerrar
4. Se muestra mensaje de error
5. Usuario ya perdió lo que había escrito
```

#### Solución Implementada:

Ahora la validación ocurre ANTES de aceptar el diálogo:

```
1. Usuario hace clic en OK
2. Lambda de validación se ejecuta primero
3. Si hay error:
   - Muestra mensaje de error
   - return; (NO llama a dialog.accept())
   - Diálogo permanece abierto
4. Si no hay error:
   - Llama a dialog.accept()
   - Diálogo se cierra normalmente
```

**Implementación Técnica:**

**ANTES (MAL):**
```cpp
// Conexión directa - el diálogo se acepta inmediatamente
connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

// Validación DESPUÉS de cerrar (demasiado tarde)
if (dialog.exec() == QDialog::Accepted) {
    QString stageName = nameEdit->text().trimmed();
    if (stageName.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("El nombre es obligatorio."));
        return; // Diálogo ya cerrado, datos perdidos!
    }
}
```

**DESPUÉS (BIEN):**
```cpp
// Conexión a lambda con validación
connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
    QString stageName = nameEdit->text().trimmed();
    QString stageCode = codeEdit->text().trimmed();
    
    if (stageName.isEmpty() || stageCode.isEmpty()) {
        QMessageBox::warning(&dialog, tr("Error"), tr("El nombre y código son obligatorios."));
        return; // NO cierra el diálogo - usuario puede corregir
    }
    
    dialog.accept(); // Solo acepta si validación OK
});

// Ahora sabemos que si llega aquí, la validación pasó
if (dialog.exec() == QDialog::Accepted) {
    // Datos ya validados, solo procesamos
    QString stageName = nameEdit->text().trimmed();
    QString stageCode = codeEdit->text().trimmed();
    // ... crear item ...
}
```

### Flujo Mejorado de Usuario

#### Escenario: Usuario Olvida Llenar Campo Obligatorio

**ANTES:**
```
[Usuario abre diálogo]
    ↓
[Llena "Código: CON"]
[Deja "Nombre" vacío]
[Llena "Descripción: Control de calidad inicial"]
    ↓
[Hace clic en OK]
    ↓
[Diálogo se cierra] ❌
    ↓
[Ve mensaje de error: "El nombre y código son obligatorios"]
    ↓
[Hace clic en OK del error]
    ↓
[TODO LO QUE ESCRIBIÓ SE PERDIÓ] ❌
[Tiene que volver a empezar desde cero]
```

**DESPUÉS:**
```
[Usuario abre diálogo]
    ↓
[Llena "Código: CON"]
[Deja "Nombre" vacío]
[Llena "Descripción: Control de calidad inicial"]
    ↓
[Hace clic en OK]
    ↓
[Validación detecta error]
    ↓
[Ve mensaje de error: "El nombre y código son obligatorios"]
    ↓
[Hace clic en OK del error]
    ↓
[DIÁLOGO SIGUE ABIERTO] ✅
[Código: CON aún está allí] ✅
[Descripción aún está allí] ✅
    ↓
[Puede escribir el nombre]
[Hace clic en OK de nuevo]
    ↓
[Validación pasa]
[Diálogo se cierra con todos los datos]
```

### Archivos Modificados

- **newmodelwizard.cpp**: Actualizado ambos métodos `addStageDialog()` y `addSubstageDialog()`
  - Añadido `#include <QLabel>`
  - Cambiadas las etiquetas a QLabel con formato HTML `<b>`
  - Movida la validación a lambdas conectadas a buttonBox accepted signal
  - Validación ahora previene el cierre del diálogo en caso de error

### Beneficios

1. **Claridad Visual**: Los usuarios ven inmediatamente qué campos son obligatorios (negrita)
2. **Mejor UX**: No se pierde información cuando hay un error de validación
3. **Menos Frustración**: Usuario puede corregir errores sin reescribir todo
4. **Consistencia**: Mismo comportamiento en ambos diálogos (etapas y subetapas)

### Validaciones que Permanecen Abiertas

- **Etapas**: Si "Nombre" o "Código" están vacíos → error sin cerrar
- **Subetapas**: Si "Nombre" está vacío → error sin cerrar

El campo "Descripción" es opcional en ambos casos, por lo que puede quedar en blanco sin problema.
