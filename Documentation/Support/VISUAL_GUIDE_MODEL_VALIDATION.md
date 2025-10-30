# Guía Visual: Validación de Modelo Cargado

## 🎯 Flujo de Validación Implementado

### Escenario 1: Sin Modelo Cargado (Nueva Funcionalidad)

```
Usuario sin modelo cargado
         |
         v
Intenta abrir menú:
- Etapas
- Sucesos iniciadores  
- Defensas (Barreras)
         |
         v
┌─────────────────────────────────────┐
│  VALIDACIÓN                         │
│  if (!praxisDb.isOpen() ||          │
│      settings.lastFile.isEmpty())   │
└─────────────────────────────────────┘
         |
         v
┌─────────────────────────────────────┐
│  ⚠️  DIÁLOGO DE ADVERTENCIA         │
│                                     │
│  Título: "No hay modelo cargado"    │
│                                     │
│  Mensaje: "No hay ningún modelo de  │
│  riesgo cargado. Cargue un modelo   │
│  para poder editarlo."              │
│                                     │
│         [    OK    ]                │
└─────────────────────────────────────┘
         |
         v
   Vuelve al menú principal
```

### Escenario 2: Con Modelo Cargado (Funcionamiento Normal)

```
Usuario con modelo cargado
         |
         v
Intenta abrir menú:
- Etapas
- Sucesos iniciadores  
- Defensas (Barreras)
         |
         v
┌─────────────────────────────────────┐
│  VALIDACIÓN                         │
│  if (!praxisDb.isOpen() ||          │
│      settings.lastFile.isEmpty())   │
└─────────────────────────────────────┘
         |
         v
   Validación PASA ✅
         |
         v
Se abre el diálogo correspondiente:
- Diálogo de Etapas
- Diálogo de Sucesos Iniciadores
- Diálogo de Lista de Barreras
         |
         v
Usuario puede editar normalmente
```

## 🔍 Ubicación de los Cambios

### Estructura de Código

```
mainwindow.cpp
│
├── on_actionListaBarreras_triggered() (línea 177)
│   ├── ✨ NUEVA: Validación de modelo cargado
│   └── Abre ListaBarreras (si pasa validación)
│
├── on_actionEtapas_triggered() (línea 1121)
│   ├── ✨ NUEVA: Validación de modelo cargado
│   └── Abre Etapas (si pasa validación)
│
└── on_mnuSucesosIniciadores_triggered() (línea 1142)
    ├── ✨ NUEVA: Validación de modelo cargado
    └── Abre sucesos (si pasa validación)
```

### Patrón de Código Implementado

```cpp
void MainWindow::on_action[DialogName]_triggered()
{
    // ✨ NUEVA VALIDACIÓN
    if (!praxisDb.isOpen() || settings.lastFile.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);           // ⚠️ Icono advertencia
        msgBox.setWindowTitle(tr("No hay modelo cargado"));
        msgBox.setText(tr("No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo."));
        msgBox.exec();
        return;  // 🛑 Detiene ejecución
    }
    
    // CÓDIGO ORIGINAL (solo se ejecuta si hay modelo)
    Dialog *dialog = new Dialog(this);
    dialog->exec();
}
```

## 📊 Comparación Antes/Después

### ANTES (Comportamiento Antiguo) ❌

```
┌──────────────────────────────────────┐
│  Usuario sin modelo cargado          │
└──────────────────────────────────────┘
              ↓
┌──────────────────────────────────────┐
│  Abre menú Etapas/Iniciadores/       │
│  Barreras                            │
└──────────────────────────────────────┘
              ↓
┌──────────────────────────────────────┐
│  ❌ Se abre diálogo vacío o con      │
│  errores (comportamiento indefinido) │
└──────────────────────────────────────┘
              ↓
         Usuario confundido
        Posibles errores/crashes
```

### DESPUÉS (Comportamiento Nuevo) ✅

```
┌──────────────────────────────────────┐
│  Usuario sin modelo cargado          │
└──────────────────────────────────────┘
              ↓
┌──────────────────────────────────────┐
│  Abre menú Etapas/Iniciadores/       │
│  Barreras                            │
└──────────────────────────────────────┘
              ↓
┌──────────────────────────────────────┐
│  ✅ VALIDACIÓN: Detecta que no hay   │
│  modelo cargado                      │
└──────────────────────────────────────┘
              ↓
┌──────────────────────────────────────┐
│  ⚠️  MENSAJE CLARO:                  │
│  "No hay ningún modelo de riesgo     │
│  cargado. Cargue un modelo para      │
│  poder editarlo."                    │
└──────────────────────────────────────┘
              ↓
    Usuario informado correctamente
      Sabe qué hacer (cargar modelo)
```

## 🌍 Soporte Multiidioma

### Español (por defecto)

```
┌───────────────────────────────────────┐
│  ⚠️  No hay modelo cargado            │
├───────────────────────────────────────┤
│                                       │
│  No hay ningún modelo de riesgo       │
│  cargado. Cargue un modelo para       │
│  poder editarlo.                      │
│                                       │
│                [ OK ]                 │
└───────────────────────────────────────┘
```

### English

```
┌───────────────────────────────────────┐
│  ⚠️  No Model Loaded                  │
├───────────────────────────────────────┤
│                                       │
│  No risk model is loaded. Load a      │
│  model to be able to edit it.         │
│                                       │
│                [ OK ]                 │
└───────────────────────────────────────┘
```

## 📝 Archivos de Traducción

### Estructura en marrta_es.ts y marrta_en.ts

```xml
<context>
    <name>MainWindow</name>
    ...
    <message>
        <location filename="mainwindow.cpp" line="1127"/>
        <location filename="mainwindow.cpp" line="1148"/>
        <location filename="mainwindow.cpp" line="183"/>
        <source>No hay modelo cargado</source>
        <translation>No Model Loaded</translation>  <!-- En inglés -->
        <!-- o -->
        <translation type="unfinished"></translation> <!-- En español -->
    </message>
    <message>
        <location filename="mainwindow.cpp" line="1128"/>
        <location filename="mainwindow.cpp" line="1149"/>
        <location filename="mainwindow.cpp" line="184"/>
        <source>No hay ningún modelo de riesgo cargado. Cargue un modelo para poder editarlo.</source>
        <translation>No risk model is loaded. Load a model to be able to edit it.</translation>
        <!-- o -->
        <translation type="unfinished"></translation>
    </message>
    ...
</context>
```

## 🔄 Estados de la Aplicación

### Estado: Sin Modelo

```
┌────────────────────────────────┐
│  MARRTA - Sin modelo           │
├────────────────────────────────┤
│  Archivo  Administración  ...  │
│  ├─ Nuevo modelo               │
│  ├─ Abrir modelo               │
│  └─ ...                        │
│                                │
│  🔒 Etapas             (bloqueado con validación)
│  🔒 Sucesos iniciadores (bloqueado con validación)
│  🔒 Defensas           (bloqueado con validación)
│                                │
│  [Área de trabajo vacía]       │
└────────────────────────────────┘
```

### Estado: Con Modelo Cargado

```
┌────────────────────────────────┐
│  MARRTA - modelo_riesgo.pdb    │
├────────────────────────────────┤
│  Archivo  Administración  ...  │
│  ├─ Cerrar modelo              │
│  ├─ Guardar                    │
│  └─ ...                        │
│                                │
│  ✅ Etapas             (habilitado)
│  ✅ Sucesos iniciadores (habilitado)
│  ✅ Defensas           (habilitado)
│                                │
│  [Modelo cargado y visible]    │
└────────────────────────────────┘
```

## 🧪 Casos de Prueba

### Caso 1: Inicio sin modelo
```
1. Iniciar MARRTA
2. Click en "Etapas"
   → ⚠️  Aparece advertencia
3. Click en "Sucesos iniciadores"
   → ⚠️  Aparece advertencia
4. Click en "Defensas"
   → ⚠️  Aparece advertencia
```

### Caso 2: Cargar modelo
```
1. Iniciar MARRTA
2. Archivo → Abrir modelo
3. Seleccionar modelo.pdb
4. Click en "Etapas"
   → ✅ Se abre diálogo
5. Click en "Sucesos iniciadores"
   → ✅ Se abre diálogo
6. Click en "Defensas"
   → ✅ Se abre diálogo
```

### Caso 3: Cerrar modelo
```
1. Con modelo cargado
2. Archivo → Cerrar
3. Click en "Etapas"
   → ⚠️  Aparece advertencia
```

## 📦 Componentes del Sistema de Validación

```
┌─────────────────────────────────────────────┐
│  Sistema de Validación de Modelo           │
├─────────────────────────────────────────────┤
│                                             │
│  ┌────────────┐      ┌──────────────────┐  │
│  │ praxisDb   │──────│ Estado: isOpen() │  │
│  └────────────┘      └──────────────────┘  │
│                                             │
│  ┌────────────┐      ┌──────────────────┐  │
│  │ settings   │──────│ lastFile path    │  │
│  └────────────┘      └──────────────────┘  │
│                                             │
│         ↓                      ↓            │
│  ┌─────────────────────────────────────┐   │
│  │  Condición de Validación:           │   │
│  │  if (!praxisDb.isOpen() ||          │   │
│  │      settings.lastFile.isEmpty())   │   │
│  └─────────────────────────────────────┘   │
│                                             │
│         ↓                                   │
│  ┌─────────────────────────────────────┐   │
│  │  Resultado:                         │   │
│  │  - TRUE  → Mostrar advertencia      │   │
│  │  - FALSE → Permitir edición         │   │
│  └─────────────────────────────────────┘   │
└─────────────────────────────────────────────┘
```

## ✨ Ventajas de la Implementación

1. **🛡️ Prevención de Errores**
   - Evita abrir diálogos sin datos
   - Previene posibles crashes
   - Mejora estabilidad

2. **👥 Mejor UX**
   - Mensaje claro y descriptivo
   - Usuario sabe qué hacer
   - Icono apropiado (⚠️)

3. **🌍 Multiidioma**
   - Soporta español e inglés
   - Fácil añadir más idiomas
   - Sistema Qt estándar

4. **🔧 Mantenible**
   - Código claro y simple
   - Patrón consistente
   - Bien documentado

5. **✅ Mínimos Cambios**
   - Solo 30 líneas añadidas
   - No afecta código existente
   - Fácil de revisar

## 📚 Documentación Relacionada

- **ISSUE_MODEL_VALIDATION_IMPLEMENTATION.md** - Documentación técnica completa
- **TRANSLATION_COMPATIBILITY_GUIDE.md** - Guía del sistema de traducciones
- **mainwindow.cpp** - Código fuente con los cambios
- **marrta_es.ts / marrta_en.ts** - Archivos de traducción actualizados
