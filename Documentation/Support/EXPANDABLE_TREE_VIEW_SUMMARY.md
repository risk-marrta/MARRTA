# Actualización: Vista de Árbol Expandible con Iniciadores

## Cambio Solicitado

**Usuario:** "Para cada defensa, al hacer click debería poderse desplegar debajo de la defensa el una lista de los iniciadores en los que interviene esa defensa, con el código, título, riesgo base y riesgo calculado. Son listas colapsables para cada barrera."

## Implementación

Se modificó la ventana "Lista de Defensas Únicas" para usar **vista de árbol (QTreeView)** en lugar de tabla plana, permitiendo expandir cada defensa para ver los iniciadores que la utilizan.

## Estructura Nueva

```
┌─────────────────────────────────────────────────────────────────────┐
│ Lista de Defensas Únicas                                        [X] │
├─────────────────────────────────────────────────────────────────────┤
│ Filtro: [Buscar por código o nombre...                ]            │
│                                                                      │
│ ┌──────────────────────────────────────────────────────────────┐   │
│ │ [Reductores Frec.] [Barreras] [Reductores Cons.]            │   │
│ ├──────────────────────────────────────────────────────────────┤   │
│ │ Código / Nombre    │ Robustez  │ Riesgo Base │ Riesgo Calc. │   │
│ │ ═══════════════════════════════════════════════════════════  │   │
│ │ ▶ [BAR-01] Detector de humo       │ Robusta   │     │      │   │
│ │ ▼ [BAR-02] Sistema sprinklers     │ Normal    │     │      │   │
│ │     ↳ [EV-01] Fallo eléctrico     │           │ RMA │ RA   │   │
│ │     ↳ [EV-03] Sobrecarga sistema  │           │ RA  │ RM   │   │
│ │     ↳ [EV-07] Cortocircuito       │           │ RA  │ RB   │   │
│ │ ▶ [BAR-03] Puertas cortafuegos    │ Muy rob.  │     │      │   │
│ └──────────────────────────────────────────────────────────────┘   │
│                                                [Cerrar]              │
└─────────────────────────────────────────────────────────────────────┘
```

## Funcionalidades

### Vista de Árbol Jerárquica

**Nivel 1 - Defensas (Filas Padre):**
- Código y nombre de la defensa: `[RF-01] Nombre de la defensa`
- Robustez: Blanda, Normal, Robusta, Muy robusta
- Riesgo Base: (vacío en nivel padre)
- Riesgo Calculado: (vacío en nivel padre)
- **Icono ▶/▼**: Indica si está colapsada o expandida

**Nivel 2 - Iniciadores (Filas Hija):**
- Código y nombre del iniciador: `    ↳ [EV-01] Nombre del evento`
- Robustez: (vacío en nivel hijo)
- Riesgo Base: Riesgo del iniciador SIN la defensa aplicada
- Riesgo Calculado: Riesgo del iniciador CON la defensa aplicada

### Interacción

1. **Expandir/Colapsar:**
   - Click en cualquier parte de la fila de defensa
   - También funciona el icono ▶/▼
   - Animación suave al expandir/colapsar

2. **Ver Impacto de Defensa:**
   - Comparar columna "Riesgo Base" vs "Riesgo Calculado"
   - Ver cómo la defensa reduce el riesgo en cada iniciador
   - Ejemplo: RMA (Muy Alto) → RA (Alto) = reducción efectiva

3. **Filtrado:**
   - El filtro busca en nombres de defensas
   - Las filas de iniciadores se ocultan junto con su padre

### Nomenclatura de Riesgos

Usa `MatrizRiesgo::Riesgos`:
- **RB**: Riesgo Bajo
- **RM**: Riesgo Medio
- **RA**: Riesgo Alto
- **RMA**: Riesgo Muy Alto

## Comparación Antes/Después

### Antes (Tabla Plana con Doble Click)
```
┌────────┬─────────────────┬──────────┐
│ Código │ Nombre          │ Robustez │
├────────┼─────────────────┼──────────┤
│ BAR-01 │ Detector humo   │ Robusta  │
│ BAR-02 │ Sprinklers      │ Normal   │
│ BAR-03 │ Puertas CF      │ Muy rob. │
└────────┴─────────────────┴──────────┘

Doble click → Ventana emergente con descripción
```

### Después (Árbol Expandible con Iniciadores)
```
┌────────────────────┬──────────┬────────┬──────────┐
│ Código / Nombre    │ Robustez │ Base   │ Calc.    │
├────────────────────┼──────────┼────────┼──────────┤
│ ▼ [BAR-02] Sprinklers │ Normal │        │          │
│   ↳ [EV-01] Fallo   │          │ RMA    │ RA       │
│   ↳ [EV-03] Sobre   │          │ RA     │ RM       │
└────────────────────┴──────────┴────────┴──────────┘

Click → Expande/colapsa lista de iniciadores
```

## Detalles Técnicos

### Cambios en la UI

**De QTableView a QTreeView:**
```xml
<!-- Antes -->
<widget class="QTableView" name="tableBarreras">
  <property name="sortingEnabled">true</property>
</widget>

<!-- Después -->
<widget class="QTreeView" name="treeBarreras">
  <property name="sortingEnabled">true</property>
  <property name="animated">true</property>
</widget>
```

### Cambios en el Modelo de Datos

**Estructura Jerárquica:**
```cpp
// Fila padre: Defensa
QStandardItem *itemDefensa = new QStandardItem("[BAR-01] Detector");
itemDefensa->setEditable(false);

// Filas hijas: Iniciadores
QStandardItem *itemIniciador1 = new QStandardItem("  ↳ [EV-01] Fallo");
QStandardItem *itemIniciador2 = new QStandardItem("  ↳ [EV-03] Sobre");

// Agregar hijos al padre
itemDefensa->appendRow({itemIniciador1, ...});
itemDefensa->appendRow({itemIniciador2, ...});

// Agregar padre al modelo
model->appendRow({itemDefensa, ...});
```

### Query SQL para Obtener Iniciadores

```sql
SELECT DISTINCT
    si.codigo,
    si.nombre,
    si.frecuencia,
    si.consecuencia,
    si.riesgo,
    (si.frecuencia * 10 + si.consecuencia) as riesgo_base
FROM defensa d
INNER JOIN suceso_iniciador si ON si.id = d.suceso_iniciador_id
WHERE d.nombre = :nombre AND d.tipo = :tipo
ORDER BY si.codigo
```

**Explicación:**
- Busca iniciadores que usan una defensa específica
- `riesgo`: Riesgo calculado (con defensa aplicada)
- `riesgo_base`: Riesgo base (sin defensa, calculado como F*10+C)
- Filtrado por nombre de defensa y tipo

### Estructura de Datos

```cpp
struct EventoInfo {
    QString codigo;        // Código del iniciador (ej: "EV-01")
    QString nombre;        // Nombre del iniciador
    int frecuencia;        // Frecuencia del evento
    int consecuencia;      // Consecuencia del evento
    int riesgo;           // Riesgo calculado (con defensa)
    int riesgoBase;       // Riesgo base (sin defensa)
};
```

### Métodos Clave

#### `cargarDefensasConEventos()`
```cpp
void ListaBarreras::cargarDefensasConEventos(QStandardItemModel *model, DefenseType tipo)
{
    for (const auto &defensa : defensasUnicas) {
        if (defensa.type != tipo) continue;
        
        // Crear fila padre (defensa)
        QStandardItem *itemDefensa = new QStandardItem(...);
        
        // Obtener iniciadores que usan esta defensa
        QList<EventoInfo> eventos = obtenerEventosPorDefensa(defensa.name, tipo);
        
        // Agregar cada iniciador como hijo
        for (const auto &evento : eventos) {
            QStandardItem *itemIniciador = new QStandardItem(...);
            itemDefensa->appendRow({itemIniciador, ...});
        }
        
        model->appendRow({itemDefensa, ...});
    }
}
```

#### `obtenerEventosPorDefensa()`
```cpp
QList<EventoInfo> ListaBarreras::obtenerEventosPorDefensa(
    const QString &nombreDefensa, 
    DefenseType tipo)
{
    // Ejecuta query SQL
    // Retorna lista de EventoInfo con datos de iniciadores
}
```

## Columnas de la Vista

| Columna | Nivel Defensa | Nivel Iniciador |
|---------|---------------|-----------------|
| **Código / Nombre** | `[BAR-01] Nombre defensa` | `↳ [EV-01] Nombre evento` |
| **Robustez** | Blanda/Normal/Robusta/Muy robusta | (vacío) |
| **Riesgo Base** | (vacío) | RB/RM/RA/RMA |
| **Riesgo Calculado** | (vacío) | RB/RM/RA/RMA |

## Casos de Uso

### 1. Evaluar Efectividad de una Defensa
```
Usuario expande "Detector de humo"
├─ Ve 5 iniciadores que lo usan
├─ Compara Riesgo Base vs Calculado
└─ Identifica que reduce RMA → RA en 3 casos
Conclusión: Defensa efectiva
```

### 2. Identificar Iniciadores Críticos
```
Usuario expande todas las barreras
├─ Busca iniciadores con Riesgo Base = RMA
├─ Ve qué defensas afectan a esos iniciadores
└─ Identifica [EV-03] aparece en múltiples defensas
Conclusión: EV-03 es crítico, necesita múltiples barreras
```

### 3. Analizar Cobertura de Defensa
```
Usuario mira cuántos hijos tiene cada defensa
├─ [BAR-01] tiene 2 iniciadores
├─ [BAR-02] tiene 8 iniciadores
└─ [BAR-03] tiene 1 iniciador
Conclusión: BAR-02 es la más utilizada
```

## Ventajas de la Nueva Implementación

1. **Información Contextual**: Ver iniciadores sin abrir ventanas adicionales
2. **Comparación Directa**: Riesgo base vs calculado en la misma fila
3. **Navegación Eficiente**: Expandir/colapsar según necesidad
4. **Análisis de Impacto**: Ver efectividad real de cada defensa
5. **Vista Consolidada**: Toda la información en un solo lugar
6. **Interacción Simple**: Un solo click para expandir
7. **Mejor UX**: No requiere doble click ni ventanas emergentes

## Testing

### Casos de Prueba

1. **Expansión/Colapso:**
   - [ ] Click en defensa expande lista de iniciadores
   - [ ] Click nuevamente colapsa la lista
   - [ ] Icono cambia de ▶ a ▼ al expandir

2. **Datos de Iniciadores:**
   - [ ] Código correcto del iniciador
   - [ ] Nombre correcto del iniciador
   - [ ] Riesgo base muestra valor sin defensa
   - [ ] Riesgo calculado muestra valor con defensa

3. **Navegación:**
   - [ ] Se puede expandir múltiples defensas a la vez
   - [ ] Scroll funciona correctamente con listas expandidas
   - [ ] Ordenamiento por columnas mantiene jerarquía

4. **Filtrado:**
   - [ ] Filtro busca en nombres de defensas
   - [ ] Defensas que no coinciden se ocultan con sus hijos
   - [ ] Limpiar filtro restaura todas las defensas

5. **Pestañas:**
   - [ ] Tres pestañas funcionan independientemente
   - [ ] Cada pestaña muestra defensas del tipo correcto
   - [ ] Contadores en títulos son correctos

## Archivos Modificados

- `listabarreras.h` (+16 líneas)
  - Agregada estructura `EventoInfo`
  - Nuevos métodos para cargar eventos por defensa
  - Eliminados slots de doble click

- `listabarreras.cpp` (+154 líneas, -116 líneas)
  - Implementada vista de árbol jerárquica
  - Query SQL para obtener iniciadores por defensa
  - Método `obtenerTextoRiesgo()` para nomenclatura

- `listabarreras.ui` (+15 líneas, -12 líneas)
  - Cambiado `QTableView` a `QTreeView` en 3 pestañas
  - Agregada propiedad `animated` para transiciones suaves
  - Actualizado ancho de columnas

**Total:** 185 líneas agregadas, 128 líneas modificadas

---

**Commit:** `25925d2` - "Add expandable tree view to show initiators for each defense"  
**Fecha:** 2025-10-09
