# Lista de Verificación - Issue #3

Esta lista de verificación ayuda a confirmar que todos los cambios funcionan correctamente.

## 🔧 Compilación

- [ ] El proyecto compila sin errores
- [ ] No hay warnings relacionados con los nuevos archivos
- [ ] Los archivos del adaptador están incluidos en el binario

```bash
# Comandos de compilación
qmake marrta.pro
make
# o
qmake-qt5 marrta.pro
make
```

## 🎨 Verificación de Interfaz

### Editor de Scripts
- [ ] El editor de scripts se abre correctamente (Herramientas > Editor de Scripts)
- [ ] Los botones están visibles en la interfaz:
  - [ ] Botón "Ejecutar" (posición 20x)
  - [ ] Botón "Documentación" (posición 120x)
  - [ ] Botón "Guardar" (posición 250x) - **NUEVO**
  - [ ] Botón "Cargar" (posición 350x) - **NUEVO**
  - [ ] Botón "Borrar" (posición 610x)

### Funcionalidad de Botones

#### Guardar Script
- [ ] Escribir un script simple: `console.log("Test");`
- [ ] Hacer clic en "Guardar"
- [ ] Se abre diálogo de guardar archivo
- [ ] Seleccionar ubicación y guardar como `test.js`
- [ ] Verificar que el archivo existe en el disco
- [ ] Verificar que el contenido del archivo es correcto

#### Cargar Script
- [ ] Hacer clic en "Cargar"
- [ ] Se abre diálogo de abrir archivo
- [ ] Seleccionar el archivo `test.js` guardado anteriormente
- [ ] El contenido aparece en el editor
- [ ] El script se puede ejecutar correctamente

## 📚 Verificación de Documentación

### Acceso a Documentación
- [ ] Hacer clic en botón "Documentación"
- [ ] Se abre ventana de documentación
- [ ] La ventana tiene panel izquierdo con secciones

### Secciones Existentes
- [ ] "Introducción" está presente
  - [ ] Menciona el objeto "Stages" - **NUEVO**
- [ ] "API de Eventos" está presente
- [ ] "API de Defensas" está presente
- [ ] "API de Gráficas" está presente

### Nueva Sección
- [ ] "API de Etapas" está presente - **NUEVO**
  - [ ] Describe el objeto `Stages`
  - [ ] Lista todos los métodos:
    - [ ] getAllStages()
    - [ ] getAllSubstages()
    - [ ] getSubstagesByStage(stageId)
    - [ ] getStagesWithSubstages()
    - [ ] getStageById(stageId)
    - [ ] getSubstageById(substageId)
    - [ ] countSubstagesByStage()
  - [ ] Explica propiedades de retorno

### Ejemplos
- [ ] Sección "Ejemplos" está presente
- [ ] Ejemplo 1: Iniciadores por Etapa
- [ ] Ejemplo 2: Iniciadores por Consecuencias
- [ ] Ejemplo 3: Barreras Humanas No Aplicables
- [ ] Ejemplo 4: Barreras No Humanas Aplicables
- [ ] Ejemplo 5: Explorar Estructura de Etapas y Subetapas - **NUEVO**

### Insertar Ejemplo
- [ ] Seleccionar "Ejemplo 5" en el árbol
- [ ] Hacer clic en "Insertar Ejemplo"
- [ ] El código se inserta en el editor
- [ ] La ventana de documentación se cierra

## 🔍 Verificación de API de Stages

### Prerequisito: Cargar un Modelo
- [ ] Abrir MARRTA
- [ ] Cargar un archivo .pdb con un modelo válido
- [ ] Verificar que el modelo se carga correctamente

### Test 1: getAllStages()
Script de prueba:
```javascript
var stages = Stages.getAllStages();
console.log("Etapas: " + stages.length);
for (var i = 0; i < stages.length; i++) {
    console.log(stages[i].nombre);
}
```

Verificar:
- [ ] El script se ejecuta sin errores
- [ ] Muestra el número correcto de etapas
- [ ] Lista nombres de etapas

### Test 2: getAllSubstages()
Script de prueba:
```javascript
var substages = Stages.getAllSubstages();
console.log("Subetapas: " + substages.length);
for (var i = 0; i < Math.min(5, substages.length); i++) {
    console.log(substages[i].nombre);
}
```

Verificar:
- [ ] El script se ejecuta sin errores
- [ ] Muestra el número correcto de subetapas
- [ ] Lista nombres de subetapas

### Test 3: getStagesWithSubstages()
Script de prueba:
```javascript
var stagesWithSubs = Stages.getStagesWithSubstages();
for (var stageName in stagesWithSubs) {
    var stage = stagesWithSubs[stageName];
    console.log(stage.nombre + ": " + stage.substages.length + " subetapas");
}
```

Verificar:
- [ ] El script se ejecuta sin errores
- [ ] Muestra etapas con conteo de subetapas
- [ ] Los números coinciden con el modelo

### Test 4: Scripts de Prueba Completos
Usar los scripts en `test_scripts/`:

#### test_stages_api.js
- [ ] Cargar el script desde `test_scripts/test_stages_api.js`
- [ ] Ejecutar el script
- [ ] Verificar que todas las pruebas muestran ✓
- [ ] No hay errores (✗)

#### test_events_api.js
- [ ] Cargar el script desde `test_scripts/test_events_api.js`
- [ ] Ejecutar el script
- [ ] Verificar que todas las pruebas muestran ✓
- [ ] No hay errores (✗)

#### test_defenses_api.js
- [ ] Cargar el script desde `test_scripts/test_defenses_api.js`
- [ ] Ejecutar el script
- [ ] Verificar que todas las pruebas muestran ✓
- [ ] No hay errores (✗)

## 🔍 Verificación de Logging

### Ejecutar con Salida de Consola
Ejecutar la aplicación desde terminal:
```bash
./marrta
```

### Verificar Logs al Ejecutar Scripts
Ejecutar cualquier script que use las APIs y verificar que aparecen logs:

#### Logs de EventAdapter
- [ ] Ejecutar: `Events.getAllEvents()`
- [ ] Verificar en consola:
  ```
  EventAdapter::getAllEvents() - Starting
  EventAdapter::getAllEvents() - Retrieved N events
  ```

#### Logs de DefenseAdapter
- [ ] Ejecutar: `Defenses.getAllDefenses()`
- [ ] Verificar en consola:
  ```
  DefenseAdapter::getAllDefenses(0) - Starting
  DefenseAdapter::getAllDefenses() - Processing defenses for N events
  ```

#### Logs de StageAdapter
- [ ] Ejecutar: `Stages.getAllStages()`
- [ ] Verificar en consola:
  ```
  StageAdapter::getAllStages() - Retrieved N stages
  ```

### Logs de Depuración
- [ ] Los logs usan `qDebug()` para información normal
- [ ] Los logs usan `qWarning()` para errores
- [ ] Los logs incluyen parámetros de entrada
- [ ] Los logs incluyen conteos de resultados

## 📝 Verificación de Archivos

### Archivos Nuevos Creados
- [ ] `adapters/stageadapter.h` existe
- [ ] `adapters/stageadapter.cpp` existe
- [ ] `test_scripts/test_stages_api.js` existe
- [ ] `test_scripts/test_events_api.js` existe
- [ ] `test_scripts/test_defenses_api.js` existe
- [ ] `test_scripts/README.md` existe
- [ ] `SCRIPTING_API_IMPROVEMENTS.md` existe
- [ ] `CHANGES_SUMMARY.md` existe

### Archivos Modificados
- [ ] `scriptingeditor.ui` - Botones añadidos
- [ ] `scriptingeditor.h` - Nuevos métodos declarados
- [ ] `scriptingeditor.cpp` - Métodos implementados
- [ ] `scriptingengine.h` - StageAdapter incluido
- [ ] `scriptingengine.cpp` - StageAdapter inicializado
- [ ] `scriptingdocumentation.cpp` - Documentación ampliada
- [ ] `adapters/eventadapter.cpp` - Logging añadido
- [ ] `adapters/defenseadapter.cpp` - Logging añadido
- [ ] `marrta.pro` - Archivos incluidos en proyecto

## 🎯 Verificación de Requisitos del Issue

### Requisito 1: Botones guardar/cargar
- [ ] ✅ Botones presentes en interfaz
- [ ] ✅ Funcionalidad de guardar funciona
- [ ] ✅ Funcionalidad de cargar funciona
- [ ] ✅ Manejo de errores implementado

### Requisito 2: Documentación más detallada
- [ ] ✅ Nueva sección API de Etapas
- [ ] ✅ Todos los métodos documentados
- [ ] ✅ Parámetros y retornos explicados
- [ ] ✅ Nuevo ejemplo añadido

### Requisito 3: API para etapas/subetapas
- [ ] ✅ StageAdapter implementado
- [ ] ✅ 7 métodos disponibles
- [ ] ✅ Integrado en ScriptingEngine
- [ ] ✅ Accesible desde JavaScript

### Requisito 4: Identificar bugs en API
- [ ] ✅ Logging extensivo implementado
- [ ] ✅ qDebug() para información
- [ ] ✅ qWarning() para errores
- [ ] ✅ Facilita depuración

## ✨ Verificación Final

### Funcionalidad Completa
- [ ] Todos los botones funcionan
- [ ] Toda la documentación es accesible
- [ ] Todos los métodos de API funcionan
- [ ] Todos los logs aparecen correctamente

### Sin Regresiones
- [ ] La funcionalidad existente sigue funcionando
- [ ] No se rompió ninguna característica anterior
- [ ] La aplicación es estable

### Documentación
- [ ] README de test_scripts es claro
- [ ] SCRIPTING_API_IMPROVEMENTS.md es completo
- [ ] CHANGES_SUMMARY.md es útil

## 📊 Resultado Final

Total de ítems: ~100
Completados: ___/100

**Porcentaje de éxito: ____%**

---

## 🐛 Problemas Encontrados

Si encuentras problemas durante la verificación, documentarlos aquí:

### Problema 1
- **Descripción**: 
- **Pasos para reproducir**: 
- **Comportamiento esperado**: 
- **Comportamiento actual**: 

### Problema 2
- **Descripción**: 
- **Pasos para reproducir**: 
- **Comportamiento esperado**: 
- **Comportamiento actual**: 

---

## ✅ Aprobación

Verificado por: ________________
Fecha: ________________
Firma: ________________

**Estado final: [ ] APROBADO  [ ] RECHAZADO  [ ] NECESITA CAMBIOS**
