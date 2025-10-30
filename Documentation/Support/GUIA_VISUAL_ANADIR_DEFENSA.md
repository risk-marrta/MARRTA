# Guía Visual: Añadir Defensa a Nuevos Sucesos

## Flujo de Usuario

### 1. Ventana de Defensas (defensas.cpp)

```
┌─────────────────────────────────────────────────────────┐
│ Gestión de defensas - Barreras                         │
├─────────────────────────────────────────────────────────┤
│ Suceso iniciador: E1-1 - Nombre del suceso             │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Lista de Defensas        │  Detalles                   │
│  ┌──────────────────┐    │  ┌────────────────────────┐ │
│  │ Defensa 1        │    │  │ Nombre: Defensa 1      │ │
│  │ Defensa 2        │◄───┤  │ Robustez: Alta         │ │
│  │ Defensa 3        │    │  │ Descripción: ...       │ │
│  └──────────────────┘    │  └────────────────────────┘ │
│  [Eliminar] [Reutilizar] │                             │
│           [Nueva]        │                             │
│                          │  [Guardar]                  │
├─────────────────────────────────────────────────────────┤
│                                    [Cerrar]             │
└─────────────────────────────────────────────────────────┘
```

### 2. Menú Contextual (clic derecho en una defensa)

```
┌─────────────────────────────┐
│ ✓ Copiar                    │
│ ────────────────────────    │  ← Separador
│ ⊕ Añadir a nuevos sucesos   │  ← NUEVA OPCIÓN
└─────────────────────────────┘
```

### 3. Diálogo de Selección de Sucesos (DlgAddDefenseToEvents)

```
┌──────────────────────────────────────────────────────────┐
│ Añadir defensa a sucesos                                 │
├──────────────────────────────────────────────────────────┤
│                                                          │
│ Seleccionar sucesos para añadir la defensa:             │
│                                                          │
│ ┌────────────────────────────────────────────────────┐  │
│ │ Etapas / Subetapas / Sucesos iniciadores          │  │
│ │ ☐ E1 - Preparación del tratamiento               │  │
│ │   ☐ Subetapa 1                                    │  │
│ │     ☐ E1-1 - Suceso iniciador 1                   │  │
│ │     ☑ E1-2 - Suceso iniciador 2                   │  │
│ │   ☑ Subetapa 2                                    │  │
│ │     ☑ E1-3 - Suceso iniciador 3                   │  │
│ │     ☑ E1-4 - Suceso iniciador 4                   │  │
│ │ ☑ E2 - Administración del tratamiento            │  │
│ │   ☑ E2-1 - Suceso iniciador 5                     │  │
│ │   ☑ E2-2 - Suceso iniciador 6                     │  │
│ └────────────────────────────────────────────────────┘  │
│                                                          │
│ ┌─ Aplicación de la defensa ────────────────────────┐   │
│ │ ⦿ Aplicar la defensa                              │   │
│ │ ○ No aplicar la defensa                           │   │
│ └────────────────────────────────────────────────────┘  │
│                                                          │
│                               [Cancelar]  [Añadir]      │
└──────────────────────────────────────────────────────────┘
```

## Comportamiento de los Checkboxes

### Checkboxes Tri-estado (Etapas y Subetapas)

- **☑ Marcado**: Todos los hijos están marcados
- **☐ Desmarcado**: Todos los hijos están desmarcados
- **⊟ Parcialmente marcado**: Algunos hijos marcados, otros no

### Propagación al marcar/desmarcar

1. **Marcar una Etapa** → Marca todas las subetapas y sucesos dentro
2. **Marcar una Subetapa** → Marca todos los sucesos dentro
3. **Desmarcar un hijo** → El padre pasa a estado parcial o desmarcado

## Proceso de Añadir Defensa

### Cuando el usuario hace clic en "Añadir":

1. **Validación**: Verifica que al menos un suceso esté seleccionado
2. **Creación**: Para cada suceso marcado:
   - Crea una nueva entrada en la tabla `defensa`
   - Copia todos los atributos de la defensa original
   - Asigna el ID del suceso destino
   - Establece `activo` según la opción de radio button
3. **Recálculo de Riesgo**: Para cada suceso afectado:
   - Obtiene todas las defensas activas del suceso
   - Calcula frecuencia, probabilidad y consecuencia
   - Obtiene el riesgo de la matriz de riesgo
   - Actualiza el campo `riesgo` en `suceso_iniciador`
4. **Actualización de Vista**: 
   - Recarga la lista de defensas si el suceso actual fue afectado
   - Muestra mensaje de confirmación con el número de sucesos afectados

## Ejemplo de Uso

### Caso: Añadir "Control de dosis" a 5 sucesos

1. Usuario abre defensas del suceso E1-1
2. Clic derecho en "Control de dosis"
3. Selecciona "Añadir a nuevos sucesos"
4. En el diálogo:
   - Marca la etapa E2 completa (3 sucesos)
   - Marca manualmente E3-1 y E3-2 (2 sucesos)
   - Selecciona "Aplicar la defensa"
5. Clic en "Añadir"
6. El sistema:
   - Crea 5 entradas nuevas en la tabla defensa
   - Recalcula el riesgo de los 5 sucesos
   - Muestra: "La defensa se ha añadido a 5 suceso(s)."

## Tablas de Base de Datos Afectadas

### Tabla `defensa` (INSERT)
```sql
INSERT INTO defensa (
    suceso_iniciador_id,  -- ID del suceso seleccionado
    codigo,               -- Código de la defensa
    nombre,               -- Nombre de la defensa
    robustez,             -- Robustez (0-3)
    tipo,                 -- Tipo (0=Frec, 1=Barrera, 2=Cons)
    descripcion,          -- Descripción
    etapa_orden_id,       -- ID de etapa para orden
    activo                -- TRUE/FALSE según radio button
) VALUES (...)
```

### Tabla `suceso_iniciador` (UPDATE)
```sql
UPDATE suceso_iniciador 
SET riesgo = [valor_calculado]
WHERE id = [suceso_id]
```

## Beneficios de la Implementación

✅ **Eficiencia**: Añadir una defensa a múltiples sucesos con unos pocos clics
✅ **Flexibilidad**: Selección granular a nivel de etapa, subetapa o suceso
✅ **Control**: Opción de aplicar o no aplicar la defensa
✅ **Consistencia**: Recálculo automático de riesgos
✅ **Usabilidad**: Interfaz intuitiva con checkboxes tri-estado
