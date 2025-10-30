# Implementación: Validación de Modelo Cargado para Edición

## 📋 Resumen

Se ha implementado la validación de que exista un modelo de riesgo cargado antes de permitir la edición de etapas, iniciadores (sucesos iniciadores) y barreras (defensas).

## 🎯 Objetivo del Issue

Asegurarse de que no se pueden editar iniciadores, etapas y barreras si no hay abierto un modelo.

Las ventanas de etapas, iniciadores y barreras no deben poder abrirse si no hay abierto un modelo de riesgo (Base de datos).

**Mensaje requerido:** "No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo" (con icono de exclamación/advertencia)

## ✅ Solución Implementada

### 1. Validación Añadida a Tres Funciones

Se añadió la validación al inicio de cada función que abre los diálogos de edición:

#### a) `on_actionEtapas_triggered()` - Edición de Etapas
**Ubicación:** `mainwindow.cpp` líneas 1121-1139

```cpp
void MainWindow::on_actionEtapas_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;
    }
    
    // ... resto del código original
}
```

#### b) `on_mnuSucesosIniciadores_triggered()` - Edición de Sucesos Iniciadores
**Ubicación:** `mainwindow.cpp` líneas 1142-1160

```cpp
void MainWindow::on_mnuSucesosIniciadores_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;
    }
    
    // ... resto del código original
}
```

#### c) `on_actionListaBarreras_triggered()` - Edición de Defensas/Barreras
**Ubicación:** `mainwindow.cpp` líneas 177-196

```cpp
void MainWindow::on_actionListaBarreras_triggered()
{
    // Check if there's a database open
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;
    }
    
    // ... resto del código original
}
```

### 2. Condición de Validación

La validación verifica dos condiciones:

```cpp
if (!praxisDb.isOpen() || settings.lastFile.isEmpty())
```

- `!praxisDb.isOpen()`: Verifica que la base de datos esté abierta
- `settings.lastFile.isEmpty()`: Verifica que haya un archivo cargado

Esta doble comprobación asegura que realmente hay un modelo de riesgo activo.

### 3. Mensaje de Advertencia

El mensaje utiliza:
- **Icono:** `QMessageBox::Warning` (icono de exclamación/advertencia)
- **Título:** "No hay modelo cargado"
- **Texto:** "No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."

### 4. Compatibilidad con Traducciones Qt

Todos los textos usan la función `tr()` para ser compatibles con el sistema de traducción de Qt:

```cpp
msgBox.setWindowTitle(tr("No hay modelo cargado"));
msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
```

#### Archivos de Traducción Actualizados

**marrta_es.ts (Español):**
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

**marrta_en.ts (Inglés):**
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

## 🔍 Patrón Utilizado

La implementación sigue el mismo patrón ya existente en el proyecto, específicamente el usado en `on_actionEmpty_triggered()` (líneas 1178-1204), que también valida si hay una base de datos abierta antes de continuar.

## 📁 Archivos Modificados

1. **mainwindow.cpp**
   - Líneas 177-196: Validación en `on_actionListaBarreras_triggered()`
   - Líneas 1121-1139: Validación en `on_actionEtapas_triggered()`
   - Líneas 1142-1160: Validación en `on_mnuSucesosIniciadores_triggered()`

2. **marrta_es.ts**
   - Añadidas entradas de traducción para los nuevos textos

3. **marrta_en.ts**
   - Añadidas entradas de traducción con traducciones al inglés

4. **TRANSLATION_COMPATIBILITY_GUIDE.md** (nuevo)
   - Documentación sobre cómo funciona el sistema de traducción
   - Guía para añadir textos traducibles en el futuro

## 🧪 Pruebas Recomendadas

### Caso 1: Sin Modelo Cargado
1. Iniciar MARRTA sin cargar ningún modelo
2. Intentar abrir "Etapas" desde el menú Administración
3. **Resultado esperado:** Aparece diálogo de advertencia con el mensaje
4. Repetir con "Sucesos iniciadores" y "Defensas"

### Caso 2: Con Modelo Cargado
1. Cargar un modelo de riesgo (.pdb)
2. Intentar abrir "Etapas" desde el menú Administración
3. **Resultado esperado:** Se abre el diálogo de edición normalmente
4. Repetir con "Sucesos iniciadores" y "Defensas"

### Caso 3: Cerrar Modelo
1. Cargar un modelo
2. Cerrar el modelo (Archivo → Cerrar)
3. Intentar abrir cualquiera de los tres diálogos
4. **Resultado esperado:** Aparece el diálogo de advertencia

## 📝 Notas Técnicas

### Sistema de Traducción Qt

El sistema de traducción funciona así:

1. **Desarrollo:** Los textos se envuelven en `tr()`
2. **Extracción:** Se ejecuta `lupdate` para extraer textos a archivos .ts
3. **Traducción:** Se editan los archivos .ts con las traducciones
4. **Compilación:** Se ejecuta `lrelease` para generar archivos .qm
5. **Ejecución:** La aplicación carga los .qm según el idioma configurado

### Ventajas del Enfoque Implementado

1. ✅ **Mínimos cambios:** Solo se añaden las validaciones necesarias
2. ✅ **Consistencia:** Sigue patrones existentes en el código
3. ✅ **Traducciones:** Compatible con Qt Linguist
4. ✅ **Mantenibilidad:** Fácil de entender y mantener
5. ✅ **Robustez:** Doble validación (DB abierta + archivo cargado)

### Compatibilidad

- Qt 5.x: ✅ Compatible
- Traducciones existentes: ✅ No afectadas
- Código existente: ✅ Sin modificaciones adicionales

## 📚 Referencias

- **Patrón base:** `MainWindow::on_actionEmpty_triggered()` (línea 1178)
- **Sistema de validación:** `praxisDb.isOpen()` y `settings.lastFile`
- **Documentación Qt:** https://doc.qt.io/qt-5/qmessagebox.html
- **Guía de traducciones:** TRANSLATION_COMPATIBILITY_GUIDE.md

## ✨ Mejoras Futuras Sugeridas

1. Considerar deshabilitar los menús en lugar de mostrar advertencias
   - Usar `setEnabled(false)` en las acciones cuando no hay modelo
   - Habilitar automáticamente al cargar un modelo

2. Actualizar el estado visual en la barra de estado
   - Mostrar "Modelo cargado: [nombre]" o "Sin modelo"

3. Añadir tooltips informativos
   - Explicar por qué las opciones están deshabilitadas

## 🎉 Resultado

La implementación cumple con todos los requisitos del issue:
- ✅ No se pueden editar etapas sin modelo cargado
- ✅ No se pueden editar iniciadores sin modelo cargado  
- ✅ No se pueden editar barreras sin modelo cargado
- ✅ Se muestra el mensaje requerido con icono de advertencia
- ✅ Los textos son compatibles con traducciones Qt
- ✅ Documentación completa sobre traducciones incluida
