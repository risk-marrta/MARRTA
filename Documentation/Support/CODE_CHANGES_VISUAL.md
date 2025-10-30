# Cambios Visuales en el Código

## Resumen de Cambios

Este documento muestra visualmente los cambios exactos realizados en el código fuente.

## mainwindow.cpp - Cambio 1: on_actionListaBarreras_triggered() (Línea 177)

### ANTES:
```cpp
void MainWindow::on_actionListaBarreras_triggered()
{
    ListaBarreras *listaBarreras = new ListaBarreras(this);
    
    // Conectar señal para refrescar pantalla cuando cambien defensas
    connect(listaBarreras, &ListaBarreras::defensesChanged, this, &MainWindow::refreshMainScreen);
    
    listaBarreras->exec();
    delete listaBarreras;
}
```

### DESPUÉS:
```cpp
void MainWindow::on_actionListaBarreras_triggered()
{
    // ✨ NUEVO: Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;
    }
    
    ListaBarreras *listaBarreras = new ListaBarreras(this);
    
    // Conectar señal para refrescar pantalla cuando cambien defensas
    connect(listaBarreras, &ListaBarreras::defensesChanged, this, &MainWindow::refreshMainScreen);
    
    listaBarreras->exec();
    delete listaBarreras;
}
```

**Diferencia:** Se añadieron 10 líneas al inicio de la función para validar el modelo.

---

## mainwindow.cpp - Cambio 2: on_actionEtapas_triggered() (Línea 1121)

### ANTES:
```cpp
void MainWindow::on_actionEtapas_triggered()
{
    Etapas *etapas = new Etapas(this);
    etapas->accepted();
    QObject::connect(etapas, &QDialog::finished, [=](){
        this->loadModel();
    });
    etapas->exec();
}
```

### DESPUÉS:
```cpp
void MainWindow::on_actionEtapas_triggered()
{
    // ✨ NUEVO: Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;
    }
    
    Etapas *etapas = new Etapas(this);
    etapas->accepted();
    QObject::connect(etapas, &QDialog::finished, [=](){
        this->loadModel();
    });
    etapas->exec();
}
```

**Diferencia:** Se añadieron 10 líneas al inicio de la función para validar el modelo.

---

## mainwindow.cpp - Cambio 3: on_mnuSucesosIniciadores_triggered() (Línea 1142)

### ANTES:
```cpp
void MainWindow::on_mnuSucesosIniciadores_triggered()
{
    sucesos *wndSucesos = new sucesos(this);
    QObject::connect(wndSucesos, &QDialog::finished, [=](){
        events.clear();
        loadModel();
    });
    wndSucesos->exec();
}
```

### DESPUÉS:
```cpp
void MainWindow::on_mnuSucesosIniciadores_triggered()
{
    // ✨ NUEVO: Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;
    }
    
    sucesos *wndSucesos = new sucesos(this);
    QObject::connect(wndSucesos, &QDialog::finished, [=](){
        events.clear();
        loadModel();
    });
    wndSucesos->exec();
}
```

**Diferencia:** Se añadieron 10 líneas al inicio de la función para validar el modelo.

---

## Patrón de Código Añadido

El siguiente código se añadió idénticamente en las tres funciones:

```cpp
// Check if there's a database open
if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("No hay modelo cargado"));
    msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
    msgBox.exec();
    return;
}
```

### Análisis del Código:

1. **Condición de validación:**
   ```cpp
   if (!praxisDb.isOpen() || settings.lastFile.isEmpty())
   ```
   - Verifica que la base de datos esté abierta
   - Verifica que haya un archivo cargado
   - Si cualquiera es falso, muestra advertencia

2. **Creación del mensaje:**
   ```cpp
   QMessageBox msgBox;
   msgBox.setIcon(QMessageBox::Warning);  // Icono ⚠️
   ```

3. **Textos traducibles:**
   ```cpp
   msgBox.setWindowTitle(tr("No hay modelo cargado"));
   msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
   ```
   - Ambos textos usan `tr()` para traducción
   - Exactamente el texto solicitado en el issue

4. **Detener ejecución:**
   ```cpp
   msgBox.exec();
   return;  // No continúa con el resto de la función
   ```

---

## Archivos de Traducción

### marrta_es.ts (Español)

**Añadido en línea 536:**
```xml
<message>
    <location filename="mainwindow.cpp" line="1127"/>
    <location filename="mainwindow.cpp" line="1148"/>
    <location filename="mainwindow.cpp" line="183"/>
    <source>No hay modelo cargado</source>
    <translation type="unfinished"></translation>
</message>
<message>
    <location filename="mainwindow.cpp" line="1128"/>
    <location filename="mainwindow.cpp" line="1149"/>
    <location filename="mainwindow.cpp" line="184"/>
    <source>No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo.</source>
    <translation type="unfinished"></translation>
</message>
```

**Nota:** `type="unfinished"` porque es el idioma por defecto (español).

### marrta_en.ts (Inglés)

**Añadido en línea 536:**
```xml
<message>
    <location filename="mainwindow.cpp" line="1127"/>
    <location filename="mainwindow.cpp" line="1148"/>
    <location filename="mainwindow.cpp" line="183"/>
    <source>No hay modelo cargado</source>
    <translation>No Model Loaded</translation>
</message>
<message>
    <location filename="mainwindow.cpp" line="1128"/>
    <location filename="mainwindow.cpp" line="1149"/>
    <location filename="mainwindow.cpp" line="184"/>
    <source>No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo.</source>
    <translation>No risk model is loaded. Load a model to be able to edit it.</translation>
</message>
```

**Nota:** Traducciones completas al inglés incluidas.

---

## Diagrama de Flujo del Cambio

```
Usuario intenta abrir diálogo
         |
         v
┌────────────────────────┐
│ Función triggered()    │
└────────────────────────┘
         |
         v
┌────────────────────────────────────┐
│ ✨ NUEVO: Validación                │
│                                    │
│ if (!praxisDb.isOpen() ||          │
│     settings.lastFile.isEmpty())   │
└────────────────────────────────────┘
         |
    ┌────┴────┐
    |         |
    v         v
  TRUE      FALSE
    |         |
    v         |
┌───────────┐ |
│ Mostrar   │ |
│ Advertencia│ |
│ y return  │ |
└───────────┘ |
              |
              v
        ┌──────────────┐
        │ Continuar    │
        │ con código   │
        │ original     │
        └──────────────┘
```

---

## Impacto Visual en la Aplicación

### Antes del Cambio:
```
Usuario sin modelo → Click "Etapas" → [Dialog vacío o error] ❌
```

### Después del Cambio:
```
Usuario sin modelo → Click "Etapas" → [Advertencia clara] ✅

┌─────────────────────────────────────┐
│  ⚠️  No hay modelo cargado          │
├─────────────────────────────────────┤
│                                     │
│  No hay ningún modelo de riesgo     │
│  cargado. Cargue un modelo para     │
│  poder editarlo.                    │
│                                     │
│              [ OK ]                 │
└─────────────────────────────────────┘
```

---

## Resumen de Cambios

| Archivo | Líneas Originales | Líneas Añadidas | Líneas Finales |
|---------|-------------------|-----------------|----------------|
| mainwindow.cpp (función 1) | 9 | +10 | 19 |
| mainwindow.cpp (función 2) | 8 | +10 | 18 |
| mainwindow.cpp (función 3) | 8 | +10 | 18 |
| marrta_es.ts | - | +14 | +14 |
| marrta_en.ts | - | +14 | +14 |
| **TOTAL** | **25** | **+58** | **83** |

**Líneas de código C++ añadidas:** 30  
**Líneas de traducción añadidas:** 28  
**Total de cambios en código:** 58 líneas

---

## Verificación de Compatibilidad con Qt

### ✅ Uso correcto de tr():
```cpp
// ✅ BIEN - Traducible
tr("No hay modelo cargado")

// ❌ MAL (ejemplo de lo que NO se hizo)
"No hay modelo cargado"
```

### ✅ Registrado en archivos .ts:
- Español: marrta_es.ts ✓
- Inglés: marrta_en.ts ✓

### ✅ Referencias correctas:
```xml
<location filename="mainwindow.cpp" line="1127"/>
<location filename="mainwindow.cpp" line="1148"/>
<location filename="mainwindow.cpp" line="183"/>
```

Cada ubicación corresponde a una de las tres funciones modificadas.

---

## Conclusión

**Los cambios son:**
1. ✅ Mínimos (solo 30 líneas de código)
2. ✅ Consistentes (mismo patrón en las 3 funciones)
3. ✅ No invasivos (no modifican lógica existente)
4. ✅ Traducibles (uso correcto de Qt tr())
5. ✅ Bien documentados

**El código implementado cumple exactamente con los requisitos del issue.**
