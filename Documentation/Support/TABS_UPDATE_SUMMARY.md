# Actualización: Pestañas Separadas para Tipos de Defensas

## Cambio Solicitado

**Usuario:** "Ademas de las barreras, pon los Reductores en secciones separadas de la ventana"

## Implementación

Se modificó la ventana "Lista de Defensas Únicas" para incluir **tres pestañas separadas**, una para cada tipo de defensa.

## Estructura Nueva

```
┌─────────────────────────────────────────────────────────────────┐
│ Lista de Defensas Únicas                                    [X] │
├─────────────────────────────────────────────────────────────────┤
│ Lista completa de defensas únicas del modelo...                │
│                                                                  │
│ Filtro: [Buscar por código o nombre...                ]        │
│                                                                  │
│ ┌──────────────────────────────────────────────────────────┐   │
│ │ [Reductores de Frecuencia (N)] [Barreras (N)] [Red...] │   │
│ ├──────────────────────────────────────────────────────────┤   │
│ │                                                           │   │
│ │  Código    │ Nombre                │ Robustez            │   │
│ │ ──────────┼───────────────────────┼────────────────     │   │
│ │  RF-01     │ Reductor ejemplo      │ Alta                │   │
│ │  RF-02     │ Otro reductor         │ Media               │   │
│ │  ...       │ ...                   │ ...                 │   │
│ │                                                           │   │
│ └──────────────────────────────────────────────────────────┘   │
│                                                [Cerrar]          │
└─────────────────────────────────────────────────────────────────┘
```

## Pestañas

### 1. Reductores de Frecuencia (Tab 0)
- Título: "Reductores de Frecuencia (N)" donde N es el total
- Muestra solo defensas de tipo `DefenseType::FrecuencyReducer`
- Columnas: Código, Nombre, Robustez

### 2. Barreras (Tab 1) - Default
- Título: "Barreras (N)" donde N es el total
- Muestra solo defensas de tipo `DefenseType::Barrier`
- Columnas: Código, Nombre, Robustez
- **Esta pestaña se abre por defecto**

### 3. Reductores de Consecuencia (Tab 2)
- Título: "Reductores de Consecuencia (N)" donde N es el total
- Muestra solo defensas de tipo `DefenseType::ConsequenceReducer`
- Columnas: Código, Nombre, Robustez

## Funcionalidades

### Filtro Global
- El campo de filtro en la parte superior **funciona en todas las pestañas simultáneamente**
- Al escribir, filtra en las tres tablas a la vez
- Busca en columnas de Código y Nombre

### Contador Dinámico
- Cada pestaña muestra el número de defensas de ese tipo en el título
- Ejemplo: "Barreras (15)" indica 15 barreras únicas
- Se actualiza automáticamente al cargar datos

### Descripción Detallada
- Doble clic en cualquier fila de cualquier pestaña
- Muestra diálogo con:
  - Código de la defensa
  - Nombre en negrita
  - Tipo de defensa en cursiva
  - Descripción completa

### Ordenamiento
- Cada tabla es independiente
- Clic en encabezado de columna para ordenar
- Funciona en todas las pestañas

## Cambios en el Código

### listabarreras.h
```cpp
// Tres modelos separados
QStandardItemModel *modelReductoresFrecuencia;
QStandardItemModel *modelBarreras;
QStandardItemModel *modelReductoresConsecuencia;

// Tres slots para doble clic
void on_tableReductoresFrecuencia_doubleClicked(const QModelIndex &index);
void on_tableBarreras_doubleClicked(const QModelIndex &index);
void on_tableReductoresConsecuencia_doubleClicked(const QModelIndex &index);

// Métodos auxiliares
QString obtenerTextoRobustez(int hardiness);
QString obtenerTextoTipo(DefenseType tipo);
```

### listabarreras.cpp
```cpp
void ListaBarreras::cargarDefensas()
{
    // Obtener todas las defensas únicas
    defensasUnicas = Database::DbManager::listUniqueDefenses();
    
    // Limpiar los tres modelos
    modelReductoresFrecuencia->removeRows(0, modelReductoresFrecuencia->rowCount());
    modelBarreras->removeRows(0, modelBarreras->rowCount());
    modelReductoresConsecuencia->removeRows(0, modelReductoresConsecuencia->rowCount());
    
    // Separar por tipo y contar
    int countRF = 0, countB = 0, countRC = 0;
    
    for (const auto &defensa : defensasUnicas) {
        // Crear fila
        QList<QStandardItem*> row;
        // ... agregar items ...
        
        // Agregar a la tabla correspondiente
        switch (defensa.type) {
            case DefenseType::FrecuencyReducer:
                modelReductoresFrecuencia->appendRow(row);
                countRF++;
                break;
            case DefenseType::Barrier:
                modelBarreras->appendRow(row);
                countB++;
                break;
            case DefenseType::ConsequenceReducer:
                modelReductoresConsecuencia->appendRow(row);
                countRC++;
                break;
        }
    }
    
    // Actualizar títulos de pestañas con contadores
    ui->tabWidget->setTabText(0, QString("Reductores de Frecuencia (%1)").arg(countRF));
    ui->tabWidget->setTabText(1, QString("Barreras (%1)").arg(countB));
    ui->tabWidget->setTabText(2, QString("Reductores de Consecuencia (%1)").arg(countRC));
}
```

### listabarreras.ui
```xml
<widget class="QTabWidget" name="tabWidget">
  <property name="currentIndex">
    <number>1</number> <!-- Abre en Barreras por defecto -->
  </property>
  
  <!-- Tab 1: Reductores de Frecuencia -->
  <widget class="QWidget" name="tabReductoresFrecuencia">
    <attribute name="title">
      <string>Reductores de Frecuencia (0)</string>
    </attribute>
    <layout>
      <widget class="QTableView" name="tableReductoresFrecuencia">
        <!-- Configuración de tabla -->
      </widget>
    </layout>
  </widget>
  
  <!-- Tab 2: Barreras -->
  <widget class="QWidget" name="tabBarreras">
    <!-- Similar estructura -->
  </widget>
  
  <!-- Tab 3: Reductores de Consecuencia -->
  <widget class="QWidget" name="tabReductoresConsecuencia">
    <!-- Similar estructura -->
  </widget>
</widget>
```

## Comparación Antes/Después

### Antes
```
Una sola tabla con columna "Tipo"
┌────────┬─────────┬──────────┬────────────────────┐
│ Código │ Nombre  │ Robustez │ Tipo               │
├────────┼─────────┼──────────┼────────────────────┤
│ BAR-01 │ ...     │ Alta     │ Barrera            │
│ RF-02  │ ...     │ Media    │ Red. Frecuencia    │
│ RC-03  │ ...     │ Baja     │ Red. Consecuencia  │
└────────┴─────────┴──────────┴────────────────────┘
```

### Después
```
Tres pestañas separadas, sin columna "Tipo"
┌─[Reductores F.]─[Barreras]─[Reductores C.]─────┐
│                                                  │
│  Código    │ Nombre            │ Robustez       │
│ ──────────┼───────────────────┼──────────────  │
│  BAR-01    │ ...               │ Alta           │
│  BAR-02    │ ...               │ Media          │
│                                                  │
└──────────────────────────────────────────────────┘
```

## Ventajas de la Nueva Implementación

1. **Mejor organización**: Defensas agrupadas por tipo
2. **Más espacio**: Sin columna "Tipo", más espacio para otros datos
3. **Contadores claros**: Número de defensas de cada tipo visible
4. **Navegación fácil**: Pestañas para cambiar rápido entre tipos
5. **Filtrado eficiente**: Buscar en todos los tipos a la vez
6. **Consistencia**: Similar a otras interfaces con pestañas en MARRTA

## Testing

Para probar:
```bash
qmake marrta.pro && make && ./marrta
```

1. Abrir un modelo PRAXIS
2. Menú → Administración → Lista de barreras
3. Verificar tres pestañas
4. Verificar contadores en títulos
5. Probar filtro en las tres pestañas
6. Probar ordenamiento en cada tabla
7. Probar doble clic en cada tabla

## Archivos Modificados

- `listabarreras.h` (+14 líneas)
- `listabarreras.cpp` (+208 líneas, -89 líneas eliminadas)
- `listabarreras.ui` (+95 líneas, -89 líneas eliminadas)

**Total:** 228 líneas agregadas, 89 líneas eliminadas

---

**Commit:** `0f7f83f` - "Add separate tabs for all defense types"  
**Fecha:** 2025-10-09
