# Feature: Ventana de Listado de Barreras Únicas

## 📋 Descripción

Nueva funcionalidad que permite visualizar todas las barreras únicas del modelo en una ventana dedicada, accesible desde el menú Administración. Simplifica la lista mostrando cada barrera solo una vez (sin duplicados).

## 🎯 Requisito Original

> "Necesito en el menú administración del mainwindow una opción que me muestre una ventana con el listado completo de barreras únicas del modelo (no todas, simplifica la lista incluyendo solo una vez cada id de barrera)."

## ✅ Solución Implementada

### Ubicación
**Menú Principal → Administración → Lista de barreras**

### Características

#### 1. Ventana Modal
- Título: "Lista de Barreras Únicas"
- Tamaño: 900x600 píxeles
- Modal (bloquea la ventana principal mientras está abierta)

#### 2. Tabla de Barreras
| Columna | Descripción | Ancho |
|---------|-------------|-------|
| Código | Código único de la barrera | 150px |
| Nombre | Nombre descriptivo | 300px |
| Robustez | Baja/Media/Alta | 100px |
| Tipo | Siempre "Barrera" | 150px |

**Propiedades:**
- Filas alternadas para mejor legibilidad
- Ordenable por cualquier columna
- Selección de fila completa
- Solo lectura (no editable)

#### 3. Filtro de Búsqueda
- Campo de texto en la parte superior
- Placeholder: "Buscar por código o nombre..."
- Filtrado en tiempo real (mientras se escribe)
- Busca en columnas de Código y Nombre
- Oculta/muestra filas automáticamente

#### 4. Contador
- Muestra: "Total de barreras únicas: X"
- Se actualiza dinámicamente con los datos cargados

#### 5. Descripción Detallada
- **Acción:** Doble clic en cualquier fila
- **Efecto:** Abre un `QMessageBox` con:
  - Título: Código de la barrera
  - Texto principal: Nombre de la barrera (en negrita)
  - Texto informativo: Descripción completa
  - Mensaje si no hay descripción: "Sin descripción"

#### 6. Botón Cerrar
- Cierra la ventana
- Ubicado en la esquina inferior derecha

## 🏗️ Arquitectura

### Componentes Principales

```
ListaBarreras (QDialog)
├── UI Components
│   ├── QTableView (tableBarreras)
│   ├── QLineEdit (edFiltro)
│   ├── QLabel (lblTotal)
│   └── QPushButton (btCerrar)
├── Data Model
│   └── QStandardItemModel (model)
└── Data Source
    └── QList<DefenseDescription> (barrerasUnicas)
```

### Flujo de Datos

```
Database
    ↓
DbManager::listUniqueDefenses()
    ↓ (SQL: GROUP BY nombre, WHERE tipo=1)
QList<DefenseDescription>
    ↓
ListaBarreras::cargarBarreras()
    ↓ (Filtrar solo Barriers)
QStandardItemModel
    ↓
QTableView
    ↓ (UI)
Usuario
```

### Métodos Clave

#### `cargarBarreras()`
```cpp
void ListaBarreras::cargarBarreras()
```
- Obtiene barreras de la base de datos
- Filtra solo tipo `DefenseType::Barrier`
- Llena el modelo con los datos
- Actualiza el contador

#### `aplicarFiltro()`
```cpp
void ListaBarreras::aplicarFiltro()
```
- Lee el texto del campo de filtro
- Itera sobre todas las filas
- Oculta/muestra filas según coincidencias
- Búsqueda case-insensitive

#### `mostrarDescripcion()`
```cpp
void ListaBarreras::mostrarDescripcion(const QModelIndex &index)
```
- Obtiene la fila seleccionada
- Busca la descripción en la lista original
- Muestra un diálogo con la información completa

## 📂 Estructura de Archivos

### Nuevos Archivos

```
marrta/
├── listabarreras.h              # Declaración de la clase
├── listabarreras.cpp            # Implementación
├── listabarreras.ui             # Interfaz Qt Designer
├── BARRIERS_LIST_IMPLEMENTATION.md
├── IMPLEMENTATION_VISUAL_SUMMARY.md
└── FEATURE_BARRIERS_LIST_README.md (este archivo)
```

### Archivos Modificados

```
marrta/
├── marrta.pro                   # Agregados nuevos archivos
├── mainwindow.h                 # Declarado slot
├── mainwindow.cpp               # Implementado slot + include
└── mainwindow.ui                # Agregada acción al menú
```

## 🔧 Integración

### 1. En `marrta.pro`

```qmake
SOURCES += \
    ...
    listabarreras.cpp \
    ...

HEADERS += \
    ...
    listabarreras.h \
    ...

FORMS += \
    ...
    listabarreras.ui \
    ...
```

### 2. En `mainwindow.ui`

```xml
<widget class="QMenu" name="menuAdministraci_n">
    <addaction name="actionEtapas"/>
    <addaction name="mnuSucesosIniciadores"/>
    <addaction name="actionListaBarreras"/>
</widget>

<action name="actionListaBarreras">
    <property name="icon">
        <iconset resource="icons.qrc">
            <normaloff>:/icons/resources/icons/no-entry.svg</normaloff>
        </iconset>
    </property>
    <property name="text">
        <string>Lista de barreras</string>
    </property>
    <property name="toolTip">
        <string>Mostrar listado completo de barreras únicas del modelo</string>
    </property>
</action>
```

### 3. En `mainwindow.h`

```cpp
private slots:
    void on_actionListaBarreras_triggered();
```

### 4. En `mainwindow.cpp`

```cpp
#include "listabarreras.h"

void MainWindow::on_actionListaBarreras_triggered()
{
    ListaBarreras *listaBarreras = new ListaBarreras(this);
    listaBarreras->exec();
    delete listaBarreras;
}
```

## 💾 Base de Datos

### Query SQL Utilizado

```sql
SELECT 
    codigo,
    nombre,
    descripcion,
    robustez,
    activo,
    tipo
FROM defensa
GROUP BY nombre
ORDER BY nombre
```

### Filtro Adicional en Código

```cpp
if (barrera.type != DefenseType::Barrier) {
    continue; // Solo mostrar tipo = 1 (Barriers)
}
```

### Enum DefenseType

```cpp
enum class DefenseType {
    FrecuencyReducer,    // 0
    Barrier,             // 1 ← El que nos interesa
    ConsequenceReducer   // 2
};
```

## 🎨 Diseño UI

### Layout Jerárquico

```
QVBoxLayout
├── QLabel (instrucciones)
├── QHBoxLayout
│   ├── QLabel ("Filtro:")
│   └── QLineEdit (edFiltro)
├── QTableView (tableBarreras)
└── QHBoxLayout
    ├── QLabel (lblTotal)
    ├── QSpacerItem
    └── QPushButton (btCerrar)
```

### Propiedades Importantes

- **QDialog:**
  - windowModality: ApplicationModal
  - modal: true
  - geometry: 900x600

- **QTableView:**
  - alternatingRowColors: true
  - selectionMode: SingleSelection
  - selectionBehavior: SelectRows
  - sortingEnabled: true

## 🧪 Testing

### Casos de Prueba

#### 1. Visualización Básica
- [ ] Abrir MARRTA
- [ ] Cargar un modelo PRAXIS
- [ ] Menú → Administración → Lista de barreras
- [ ] Verificar que se abre la ventana

#### 2. Datos en Tabla
- [ ] Verificar que solo aparecen barreras (no otros tipos)
- [ ] Verificar que no hay duplicados
- [ ] Verificar que el contador es correcto
- [ ] Verificar columnas: Código, Nombre, Robustez, Tipo

#### 3. Filtro
- [ ] Escribir texto en el campo de filtro
- [ ] Verificar que se ocultan filas que no coinciden
- [ ] Verificar búsqueda por código
- [ ] Verificar búsqueda por nombre
- [ ] Borrar filtro y verificar que vuelven todas las filas

#### 4. Ordenamiento
- [ ] Clic en encabezado "Código" → ordenar ascendente
- [ ] Clic nuevamente → ordenar descendente
- [ ] Repetir para "Nombre", "Robustez", "Tipo"

#### 5. Descripción
- [ ] Doble clic en una fila
- [ ] Verificar que se abre diálogo con descripción
- [ ] Verificar título = código
- [ ] Verificar texto = nombre + descripción
- [ ] Cerrar diálogo

#### 6. Cerrar Ventana
- [ ] Clic en botón "Cerrar"
- [ ] Clic en [X] de la ventana
- [ ] Tecla ESC
- [ ] Verificar que se cierra correctamente

### Comandos de Compilación

```bash
# Generar Makefile
qmake marrta.pro

# Compilar
make

# Ejecutar
./marrta

# O en un solo comando
qmake && make && ./marrta
```

## 📊 Valores de Robustez

| Valor | Texto Mostrado |
|-------|----------------|
| 0 | Baja |
| 1 | Media |
| 2 | Alta |
| Otro | Desconocida |

## 🔍 Ejemplo de Uso

```
1. Usuario abre MARRTA
2. Usuario carga "modelo_hospital.pdb"
3. Usuario va a Menú → Administración → Lista de barreras
4. Se abre ventana con 15 barreras únicas
5. Usuario escribe "incendio" en el filtro
6. La tabla muestra solo 3 barreras relacionadas
7. Usuario hace doble clic en "BAR-05 Detector de humo"
8. Se muestra diálogo con la descripción completa
9. Usuario cierra el diálogo
10. Usuario cierra la ventana de lista
```

## 🎯 Beneficios

1. **Vista consolidada** - Todas las barreras en un solo lugar
2. **Sin duplicados** - Solo una entrada por barrera única
3. **Búsqueda rápida** - Filtro en tiempo real
4. **Información completa** - Código, nombre, robustez, descripción
5. **Fácil acceso** - Directamente desde el menú Administración
6. **Ordenamiento flexible** - Por cualquier columna
7. **No destructivo** - Solo lectura, no modifica datos

## 📝 Notas Técnicas

### Memoria
- El diálogo se crea y destruye cada vez que se abre
- No mantiene estado entre aperturas
- Memoria liberada automáticamente con `delete`

### Performance
- Query SQL optimizado con `GROUP BY`
- Filtrado en memoria (no re-query)
- Modelo Qt estándar para manejo eficiente de datos

### Compatibilidad
- Qt 5.x compatible
- Usa componentes estándar de Qt
- No requiere librerías adicionales
- Compatible con el estilo existente de MARRTA

### Patrón de Diseño
- Sigue el patrón Model-View establecido en el proyecto
- Consistente con otros diálogos (Etapas, Sucesos, etc.)
- Reutiliza código existente de `DbManager`

## 🚀 Próximas Mejoras Posibles (Futuro)

1. **Exportar a CSV/Excel** - Botón para exportar la lista
2. **Estadísticas** - Gráficos de distribución de robustez
3. **Edición in-place** - Permitir modificar desde la tabla
4. **Filtros avanzados** - Por robustez, por etapa, etc.
5. **Búsqueda en descripción** - Incluir en el filtro
6. **Vista detallada** - Panel lateral con toda la info
7. **Imprimir** - Generar reporte imprimible
8. **Comparación** - Comparar barreras lado a lado

## 📚 Referencias

- `dbmanager.h/cpp` - Método `listUniqueDefenses()`
- `defensedefinition.h` - Estructura `DefenseDescription`
- `dlgimportardefensa.cpp` - Patrón de implementación similar
- `mainwindow.cpp` - Integración con menú principal

---

**Fecha de Implementación:** 2025-10-09  
**Versión de MARRTA:** 1.4.5+  
**Estado:** ✅ Completado - Pendiente de testing con compilación Qt
