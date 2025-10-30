# Guía de Pruebas: Añadir Defensa a Nuevos Sucesos

## Requisitos Previos

- Tener compilado el proyecto MARRTA con Qt
- Tener una base de datos con al menos:
  - 2 etapas con sucesos
  - Alguna subetapa (opcional)
  - Al menos 3 sucesos iniciadores
  - Al menos 1 defensa (barrera, reductor de frecuencia o de consecuencia)

## Escenarios de Prueba

### Prueba 1: Funcionalidad Básica

**Objetivo**: Verificar que se puede añadir una defensa a un suceso

**Pasos**:
1. Abrir la ventana de defensas de cualquier suceso (ej: E1-1)
2. Si no hay defensas, crear una nueva
3. Hacer clic derecho en la defensa
4. Verificar que aparece el menú contextual con:
   - "Copiar"
   - Línea separadora
   - "Añadir a nuevos sucesos"
5. Seleccionar "Añadir a nuevos sucesos"
6. Verificar que se abre el diálogo con el árbol de sucesos

**Resultado esperado**: ✅ El diálogo se abre correctamente

### Prueba 2: Árbol de Sucesos

**Objetivo**: Verificar que el árbol muestra correctamente la jerarquía

**Pasos**:
1. En el diálogo abierto, verificar:
   - Las etapas aparecen en el orden correcto
   - Las subetapas aparecen bajo sus etapas correspondientes
   - Los sucesos aparecen en su lugar correspondiente
   - Todos los elementos tienen checkbox
   - Los checkboxes de etapas y subetapas son tri-estado

**Resultado esperado**: ✅ La jerarquía se muestra correctamente

### Prueba 3: Checkboxes Tri-estado

**Objetivo**: Verificar el comportamiento de los checkboxes

**Pasos**:
1. Marcar una etapa completa
2. Verificar que todos sus hijos se marcan
3. Desmarcar un suceso dentro de la etapa
4. Verificar que la etapa pasa a estado "parcialmente marcado"
5. Desmarcar todos los sucesos de la etapa
6. Verificar que la etapa se desmarca completamente

**Resultado esperado**: ✅ Los checkboxes funcionan correctamente

### Prueba 4: Aplicar Defensa

**Objetivo**: Verificar que se puede añadir una defensa aplicada

**Pasos**:
1. Seleccionar 2-3 sucesos diferentes al actual
2. Seleccionar radio button "Aplicar la defensa"
3. Hacer clic en "Añadir"
4. Verificar mensaje de éxito con el número correcto de sucesos
5. Abrir la ventana de defensas de uno de los sucesos seleccionados
6. Verificar que la defensa aparece en la lista
7. Verificar que el checkbox de la defensa está marcado (aplicada)

**Resultado esperado**: ✅ La defensa se añade y está aplicada

### Prueba 5: No Aplicar Defensa

**Objetivo**: Verificar que se puede añadir una defensa no aplicada

**Pasos**:
1. Repetir el proceso anterior
2. Seleccionar radio button "No aplicar la defensa"
3. Hacer clic en "Añadir"
4. Verificar mensaje de éxito
5. Abrir la ventana de defensas de uno de los sucesos seleccionados
6. Verificar que la defensa aparece en la lista
7. Verificar que el checkbox de la defensa NO está marcado (no aplicada)

**Resultado esperado**: ✅ La defensa se añade pero no está aplicada

### Prueba 6: Recálculo de Riesgo (Defensa Aplicada)

**Objetivo**: Verificar que el riesgo se recalcula correctamente

**Pasos**:
1. Abrir la ventana principal y anotar el riesgo de un suceso
2. Añadir una barrera aplicada al suceso usando el nuevo diálogo
3. Verificar mensaje de éxito
4. Volver a la ventana principal
5. Verificar que el riesgo del suceso ha cambiado (debería disminuir)

**Resultado esperado**: ✅ El riesgo se recalcula automáticamente

### Prueba 7: Recálculo de Riesgo (Defensa No Aplicada)

**Objetivo**: Verificar que el riesgo no cambia si la defensa no se aplica

**Pasos**:
1. Abrir la ventana principal y anotar el riesgo de un suceso
2. Añadir una barrera NO aplicada al suceso
3. Verificar mensaje de éxito
4. Volver a la ventana principal
5. Verificar que el riesgo del suceso NO ha cambiado

**Resultado esperado**: ✅ El riesgo permanece igual

### Prueba 8: Añadir a Suceso Actual

**Objetivo**: Verificar que se puede añadir una defensa al mismo suceso donde se encuentra

**Pasos**:
1. Abrir defensas de E1-1 (que tiene la defensa "Control de dosis")
2. Clic derecho en "Control de dosis"
3. Seleccionar "Añadir a nuevos sucesos"
4. En el árbol, marcar E1-1 (el mismo suceso)
5. Seleccionar "Aplicar"
6. Clic en "Añadir"
7. Verificar que se añade otra instancia de la defensa

**Resultado esperado**: ✅ Se puede añadir al mismo suceso (duplicar)

### Prueba 9: Selección Múltiple con Etapas

**Objetivo**: Verificar selección de etapas completas

**Pasos**:
1. Abrir el diálogo
2. Marcar 2 etapas completas (ej: E1 y E2)
3. Verificar el contador mental de sucesos
4. Aplicar la defensa
5. Verificar que el mensaje muestra el número correcto de sucesos

**Resultado esperado**: ✅ Todos los sucesos de las etapas se afectan

### Prueba 10: Cancelar Operación

**Objetivo**: Verificar que cancelar no hace cambios

**Pasos**:
1. Abrir el diálogo
2. Seleccionar varios sucesos
3. Hacer clic en "Cancelar"
4. Verificar que no se hace ningún cambio en la base de datos

**Resultado esperado**: ✅ No se hacen cambios al cancelar

### Prueba 11: Validación sin Selección

**Objetivo**: Verificar validación cuando no se selecciona nada

**Pasos**:
1. Abrir el diálogo
2. No marcar ningún suceso
3. Hacer clic en "Añadir"
4. Verificar que aparece mensaje de advertencia
5. Verificar que el diálogo no se cierra

**Resultado esperado**: ✅ Aparece mensaje "No se ha seleccionado ningún suceso"

### Prueba 12: Diferentes Tipos de Defensas

**Objetivo**: Verificar que funciona con todos los tipos

**Pasos**:
1. Probar con Reductor de Frecuencia
2. Probar con Barrera
3. Probar con Reductor de Consecuencia
4. Verificar que en todos los casos:
   - El diálogo se abre
   - La defensa se añade correctamente
   - El tipo se preserva

**Resultado esperado**: ✅ Funciona con todos los tipos

## Verificación en Base de Datos

### Consulta SQL para verificar defensas añadidas

```sql
-- Ver defensas de un suceso específico
SELECT d.id, d.nombre, d.tipo, d.activo, d.robustez, si.nombre as suceso
FROM defensa d
JOIN suceso_iniciador si ON d.suceso_iniciador_id = si.id
WHERE si.id = [ID_SUCESO]
ORDER BY d.nombre;

-- Ver riesgo calculado de sucesos
SELECT id, codigo, nombre, riesgo
FROM suceso_iniciador
WHERE id IN ([IDS_SUCESOS_AFECTADOS])
ORDER BY id;
```

## Checklist de Validación

- [ ] El menú contextual muestra la nueva opción correctamente
- [ ] El diálogo se abre con el árbol de sucesos
- [ ] Los checkboxes tri-estado funcionan correctamente
- [ ] Se puede aplicar la defensa
- [ ] Se puede no aplicar la defensa
- [ ] El riesgo se recalcula para defensas aplicadas
- [ ] El riesgo no cambia para defensas no aplicadas
- [ ] Se actualiza la ventana de defensas del suceso actual
- [ ] El mensaje de confirmación muestra el número correcto
- [ ] La cancelación no hace cambios
- [ ] La validación funciona sin selección
- [ ] Funciona con todos los tipos de defensas

## Problemas Conocidos a Verificar

1. **Rendimiento**: Si hay muchos sucesos (>100), verificar que el árbol se carga rápidamente
2. **Memoria**: Verificar que no hay fugas de memoria al abrir/cerrar el diálogo múltiples veces
3. **Concurrencia**: Si otro usuario modifica los sucesos, verificar el comportamiento

## Notas para el Testing

- Usar una base de datos de prueba, no producción
- Hacer backup antes de probar
- Anotar cualquier comportamiento inesperado
- Verificar los logs para mensajes de error o warning
