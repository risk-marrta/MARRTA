# Implementación: Añadir defensa a nuevos sucesos

## Resumen

Se ha implementado la funcionalidad para añadir una defensa existente a múltiples sucesos desde la ventana de defensas, según lo solicitado en el issue #86.

## Cambios Realizados

### 1. Nuevo Diálogo: DlgAddDefenseToEvents

Se crearon tres archivos nuevos:

- `dlgadddefensetoevents.h` - Declaración de la clase
- `dlgadddefensetoevents.cpp` - Implementación
- `dlgadddefensetoevents.ui` - Interfaz de usuario Qt

**Características:**
- Muestra un árbol jerárquico de Etapas → Subetapas → Sucesos iniciadores
- Checkboxes en todos los niveles (etapas, subetapas e iniciadores)
- Sistema de checkboxes tri-estado para niveles padre
- Radio buttons para seleccionar si aplicar o no la defensa
- Botones "Añadir" y "Cancelar"

### 2. Actualización de defensas.cpp

**Menú contextual mejorado:**
- Se agregó un separador después de "Copiar"
- Se añadió la opción "Añadir a nuevos sucesos"
- Al seleccionarla, abre el diálogo DlgAddDefenseToEvents
- Tras seleccionar sucesos, crea la defensa en cada uno
- Recalcula el riesgo de cada suceso afectado
- Actualiza la ventana de defensas
- Muestra mensaje de confirmación

**Nuevo método:**
- `recalcularRiesgoEvento(int eventoId)` - Recalcula el riesgo de un evento basado en sus defensas activas

### 3. Actualización de dbdefense.cpp

**DbDefense::create mejorado:**
- Se agregó el campo `activo` al INSERT
- Ahora respeta el valor de `DefenseDefinition.active`

### 4. Actualización de marrta.pro

Se añadieron los nuevos archivos al proyecto Qt:
- `dlgadddefensetoevents.cpp` en SOURCES
- `dlgadddefensetoevents.h` en HEADERS  
- `dlgadddefensetoevents.ui` en FORMS

## Flujo de Uso

1. Usuario abre la ventana de defensas de un suceso
2. Hace clic derecho en una defensa existente
3. Selecciona "Añadir a nuevos sucesos" del menú contextual
4. Se abre el diálogo con el árbol de sucesos
5. Usuario marca las etapas/subetapas/sucesos deseados
6. Selecciona "Aplicar" o "No aplicar" la defensa
7. Hace clic en "Añadir"
8. El sistema:
   - Crea la defensa en cada suceso marcado
   - Recalcula el riesgo de cada suceso
   - Actualiza la vista de defensas
   - Muestra mensaje de éxito

## Cálculo de Riesgos

El método `recalcularRiesgoEvento` implementa la misma lógica que `ListaBarreras::recalcularYActualizarRiesgo`:

1. Obtiene datos base del evento (frecuencia, consecuencia, error humano)
2. Consulta todas las defensas activas del evento
3. Agrupa defensas por tipo y robustez
4. Usa DefenseCalc para calcular:
   - Frecuencia calculada
   - Probabilidad calculada  
   - Consecuencia calculada
5. Usa MatrizRiesgo para obtener el riesgo final
6. Actualiza el campo `riesgo` en la tabla `suceso_iniciador`

## Archivos Modificados

- `defensas.cpp` - Menú contextual y recálculo de riesgo
- `defensas.h` - Declaración de recalcularRiesgoEvento
- `dbdefense.cpp` - Soporte para campo activo
- `marrta.pro` - Registro de nuevos archivos

## Archivos Nuevos

- `dlgadddefensetoevents.h`
- `dlgadddefensetoevents.cpp`
- `dlgadddefensetoevents.ui`
