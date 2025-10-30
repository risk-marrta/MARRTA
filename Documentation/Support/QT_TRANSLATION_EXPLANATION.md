# Explicación del Sistema de Traducciones Qt

## ¿Cómo se hace para que los textos sean compatibles con las traducciones de Qt?

### 1. Uso de la función `tr()` 

La clave está en **envolver todos los textos visibles al usuario con la función `tr()`**:

```cpp
// ✅ CORRECTO - Texto traducible
QMessageBox msgBox;
msgBox.setWindowTitle(tr("No hay modelo cargado"));
msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));

// ❌ INCORRECTO - Texto fijo que no se puede traducir
msgBox.setWindowTitle("No hay modelo cargado");
msgBox.setText("No hay ningún modelo de riesgo cargado...");
```

### 2. ¿Qué hace `tr()`?

La función `tr()` (translate) es proporcionada por Qt y funciona así:

1. **En desarrollo**: Marca el texto como traducible
2. **Al compilar**: Qt extrae estos textos a archivos especiales (.ts)
3. **Al traducir**: Los traductores completan los archivos .ts
4. **En ejecución**: Qt carga la traducción según el idioma del sistema

### 3. El Flujo Completo

```
┌─────────────────┐
│ 1. DESARROLLO   │
│                 │
│ Código C++:     │
│ tr("Texto")     │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ 2. EXTRACCIÓN   │
│                 │
│ Comando:        │
│ lupdate         │
│                 │
│ Genera:         │
│ marrta_es.ts    │
│ marrta_en.ts    │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ 3. TRADUCCIÓN   │
│                 │
│ Editar .ts:     │
│ <translation>   │
│   No Model      │
│   Loaded        │
│ </translation>  │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ 4. COMPILACIÓN  │
│                 │
│ Comando:        │
│ lrelease        │
│                 │
│ Genera:         │
│ marrta_es.qm    │
│ marrta_en.qm    │
└────────┬────────┘
         │
         v
┌─────────────────┐
│ 5. EJECUCIÓN    │
│                 │
│ App carga .qm   │
│ según idioma    │
│                 │
│ Usuario ve      │
│ texto traducido │
└─────────────────┘
```

### 4. Archivos Involucrados

#### Archivos .ts (Translation Source)
Son archivos XML que contienen:
- El texto original (en español en nuestro caso)
- La ubicación en el código (archivo y línea)
- La traducción a otro idioma

**Ejemplo de marrta_es.ts:**
```xml
<message>
    <location filename="mainwindow.cpp" line="1127"/>
    <source>No hay modelo cargado</source>
    <translation type="unfinished"></translation>
</message>
```

**Ejemplo de marrta_en.ts:**
```xml
<message>
    <location filename="mainwindow.cpp" line="1127"/>
    <source>No hay modelo cargado</source>
    <translation>No Model Loaded</translation>
</message>
```

#### Archivos .qm (Compiled Translations)
Son archivos binarios compilados que la aplicación usa en tiempo de ejecución.

### 5. Comandos Qt para Traducciones

#### lupdate - Actualizar traducciones desde código
```bash
# Extrae todos los tr() del código y actualiza los .ts
lupdate marrta.pro
```

#### lrelease - Compilar traducciones
```bash
# Compila los .ts a .qm para usar en la app
lrelease marrta.pro
```

#### Qt Linguist - Editor visual
```bash
# Abre editor gráfico para traducir
linguist marrta_en.ts
```

### 6. En Nuestro Proyecto MARRTA

#### Estructura de archivos:
```
MARRTA/
├── mainwindow.cpp       (usa tr("..."))
├── etapas.cpp          (usa tr("..."))
├── sucesos.cpp         (usa tr("..."))
├── ...
├── marrta_es.ts        (traducciones español)
├── marrta_en.ts        (traducciones inglés)
├── marrta_es.qm        (compilado español)
├── marrta_en.qm        (compilado inglés)
└── marrta.pro          (configuración de proyecto)
```

#### En marrta.pro:
```qmake
TRANSLATIONS = marrta_es.ts \
               marrta_en.ts
```

### 7. Ejemplo Completo de Nuestro Código

#### Código C++ (mainwindow.cpp):
```cpp
void MainWindow::on_actionEtapas_triggered()
{
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;
    }
    // ...
}
```

#### Archivo de Traducción Español (marrta_es.ts):
```xml
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.cpp" line="1127"/>
        <source>No hay modelo cargado</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1128"/>
        <source>No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
```

**Nota:** En español está como "unfinished" porque es el idioma por defecto.

#### Archivo de Traducción Inglés (marrta_en.ts):
```xml
<context>
    <name>MainWindow</name>
    <message>
        <location filename="mainwindow.cpp" line="1127"/>
        <source>No hay modelo cargado</source>
        <translation>No Model Loaded</translation>
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1128"/>
        <source>No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo.</source>
        <translation>No risk model is loaded. Load a model to be able to edit it.</translation>
    </message>
</context>
```

### 8. ¿Cómo Sabe Qt Qué Traducción Usar?

Qt determina el idioma automáticamente:

```cpp
// En main.cpp (usualmente ya configurado):
QTranslator translator;
QString locale = QLocale::system().name(); // Ejemplo: "es_ES" o "en_US"

if (translator.load("marrta_" + locale)) {
    app.installTranslator(&translator);
}
```

### 9. Ventajas de Este Sistema

1. ✅ **Separación de código y traducciones**
   - El código no cambia
   - Solo se editan archivos .ts

2. ✅ **Fácil añadir idiomas**
   - Crear nuevo .ts para cada idioma
   - No tocar código C++

3. ✅ **Herramientas profesionales**
   - Qt Linguist para traducir
   - Validación automática

4. ✅ **Mantenimiento simple**
   - lupdate detecta nuevos textos
   - Mantiene traducciones existentes

5. ✅ **Cambio dinámico de idioma**
   - Posible cambiar idioma sin reiniciar
   - (si se implementa)

### 10. Checklist para Nuevos Textos

Cuando añadas nuevo texto visible al usuario:

- [ ] Envolver con `tr("...")`
- [ ] Ejecutar `lupdate marrta.pro`
- [ ] Editar `marrta_en.ts` para añadir traducción inglesa
- [ ] Ejecutar `lrelease marrta.pro`
- [ ] Compilar y probar

### 11. Buenas Prácticas

```cpp
// ✅ BIEN - Texto completo en tr()
msgBox.setText(tr("Error al cargar el archivo: %1").arg(filename));

// ❌ MAL - Concatenación de tr()
msgBox.setText(tr("Error al cargar") + filename); 

// ✅ BIEN - Usar placeholders
tr("Encontrados %n elementos", "", count);

// ❌ MAL - Plurales manualmente
tr("Encontrados " + QString::number(count) + " elementos");

// ✅ BIEN - Contexto claro
tr("No hay modelo cargado"); // Se entiende el contexto

// ❌ MAL - Texto ambiguo
tr("OK"); // ¿OK de qué?
```

### 12. Resolución de Problemas Comunes

#### Problema: Las traducciones no aparecen
```bash
# Solución:
1. Verificar que los .qm existan
2. Verificar que estén en el lugar correcto
3. Recompilar con lrelease
```

#### Problema: Texto sin traducir
```bash
# Verificar:
1. ¿Está envuelto en tr()?
2. ¿Se ejecutó lupdate?
3. ¿Existe traducción en .ts?
4. ¿Se ejecutó lrelease?
```

#### Problema: Actualizar traducción existente
```bash
# Pasos:
1. Modificar código con nuevo texto en tr()
2. lupdate marrta.pro
3. Editar .ts con nueva traducción
4. lrelease marrta.pro
```

### 13. Resumen Ejecutivo

**Para que un texto sea traducible en Qt:**

1. Usar `tr("texto")` en el código C++
2. Ejecutar `lupdate` para extraer a .ts
3. Traducir en archivos .ts
4. Ejecutar `lrelease` para compilar a .qm
5. La app carga automáticamente según idioma del sistema

**En nuestro caso específico:**
- ✅ Usamos `tr()` en los tres diálogos modificados
- ✅ Añadimos traducciones en marrta_es.ts y marrta_en.ts
- ✅ Los textos son compatibles con el sistema Qt
- ✅ Funcionará automáticamente cuando se ejecute lrelease

## 🎓 Conclusión

El sistema de traducciones de Qt es robusto, profesional y muy fácil de usar. La clave está en:

1. **Siempre usar `tr()`** para textos visibles
2. **Ejecutar lupdate** regularmente
3. **Mantener archivos .ts** actualizados
4. **Compilar con lrelease** antes de distribuir

Nuestro código sigue estas buenas prácticas y es totalmente compatible con el sistema de traducciones de Qt. 🚀
