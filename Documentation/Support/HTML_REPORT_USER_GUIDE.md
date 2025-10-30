# Guía de Usuario: Generador de Informes HTML

## Descripción General

El Generador de Informes HTML es una herramienta en MARRTA que permite crear informes profesionales en formato PDF a partir de los eventos de análisis de riesgo. Los informes siguen un formato A4 estandarizado y profesional que incluye toda la información relevante del evento.

## ¿Qué incluye un informe?

Cada informe generado contiene las siguientes secciones:

### 1. Información de la Etapa
- Nombre de la etapa a la que pertenece el evento
- Mostrado en un encabezado azul destacado

### 2. Suceso Iniciador
- Nombre del evento o suceso iniciador
- Mostrado en una barra gris

### 3. Descripción del Error
- Descripción detallada del evento
- Presentada en un cuadro con bordes

### 4. Análisis de Riesgo
- **Frecuencia**: Nivel de frecuencia del evento (Muy Baja, Baja, Media, Alta)
- **Consecuencias**: Nivel de consecuencias (Muy Baja, Baja, Media, Alta)
- Mostrados en una barra azul informativa

### 5. Contexto
- Información contextual adicional del evento
- Texto justificado para mejor lectura

### 6. Barreras (si existen)
- Tabla con barreras del evento
- Columnas: ID, Descripción, Implementación
- Solo incluye barreras activas

### 7. Reductores de Frecuencia (si existen)
- Tabla con medidas que reducen la frecuencia
- Columnas: ID, Descripción
- Solo incluye reductores activos

### 8. Reductores de Consecuencias (si existen)
- Tabla con medidas que reducen las consecuencias
- Columnas: ID, Descripción
- Solo incluye reductores activos

## ¿Cómo generar un informe?

### Paso 1: Abrir el Generador de Informes

1. Asegúrese de tener un modelo abierto en MARRTA
2. Vaya al menú principal
3. Seleccione **Informes → Generar informe HTML...**

![Ubicación del menú](../images/menu-generar-informe.png)

### Paso 2: Seleccionar el Evento

En el diálogo que se abre:

1. Use el menú desplegable **Evento** para seleccionar el evento que desea reportar
2. Los eventos se muestran en el formato: `[Código] - [Nombre del Evento]`
3. La vista previa se actualiza automáticamente mostrando un resumen del evento

```
┌─────────────────────────────────────────┐
│  Generador de Informes HTML             │
├─────────────────────────────────────────┤
│                                         │
│  Seleccione un evento para generar...   │
│                                         │
│  Evento: [E001 - Fallo en sistema ▼]    │
│                                         │
│  ┌─── Vista Previa ──────────────────┐ │
│  │ Etapa: Producción                 │ │
│  │ Iniciador: Fallo en sistema...    │ │
│  │ Descripción: El sistema presenta..│ │
│  │ Barreras: 3                       │ │
│  │ Reductores de Frecuencia: 2       │ │
│  │ Reductores de Consecuencia: 1     │ │
│  └───────────────────────────────────┘ │
│                                         │
│              [Generar PDF] [Cancelar]   │
└─────────────────────────────────────────┘
```

### Paso 3: Generar el PDF

1. Revise la vista previa para confirmar que es el evento correcto
2. Haga clic en el botón **Generar PDF**
3. Se abrirá un diálogo para guardar el archivo:
   - Elija la ubicación donde desea guardar el informe
   - Ingrese el nombre del archivo (la extensión .pdf se añade automáticamente)
   - Haga clic en **Guardar**

### Paso 4: Confirmación

Si el informe se genera correctamente:
- Aparecerá un mensaje de éxito mostrando la ruta del archivo
- El diálogo se cerrará automáticamente
- Puede abrir el PDF con su visor preferido

Si hay algún error:
- Aparecerá un mensaje de error
- Revise que la ubicación de guardado sea válida
- Verifique que tiene permisos de escritura en la carpeta

## Formato del Informe

### Especificaciones Técnicas

- **Tamaño de página**: A4 (210mm x 297mm)
- **Márgenes**: 15mm en todos los lados
- **Fuente**: Calibri (Windows), Lucida Sans (Linux), Segoe UI (alternativa)
- **Tamaño de fuente**: 10pt (cuerpo), 11pt (títulos)
- **Colores**:
  - Azul principal: #4472C4 (encabezados)
  - Gris: #D9D9D9 (secciones secundarias)
  - Gris claro: #F2F2F2 (encabezados de tabla)

### Estructura Visual

```
╔════════════════════════════════════════════╗
║  ETAPA: [Nombre de la Etapa]              ║ <- Azul
╠════════════════════════════════════════════╣
║  INICIADOR: [Nombre del Evento]           ║ <- Gris
╠════════════════════════════════════════════╣
║ ┌────────────────────────────────────────┐ ║
║ │ [Descripción del error]                │ ║
║ └────────────────────────────────────────┘ ║
╠════════════════════════════════════════════╣
║  FRECUENCIA: [Nivel]  CONSECUENCIAS: [Niv]║ <- Azul
╠════════════════════════════════════════════╣
║  [Contexto del evento...]                 ║
║                                            ║
║  BARRERAS                                  ║
║  ┌────┬─────────────────┬────────────────┐ ║
║  │ ID │ Descripción     │ Implementación │ ║
║  ├────┼─────────────────┼────────────────┤ ║
║  │ 1  │ ...             │ ...            │ ║
║  └────┴─────────────────┴────────────────┘ ║
║                                            ║
║  REDUCTORES DE FRECUENCIA                  ║
║  ┌────┬─────────────────────────────────┐  ║
║  │ ID │ Descripción                     │  ║
║  ├────┼─────────────────────────────────┤  ║
║  │ 1  │ ...                             │  ║
║  └────┴─────────────────────────────────┘  ║
╚════════════════════════════════════════════╝
```

## Casos de Uso

### 1. Documentación para Auditorías
- Genere informes de eventos críticos
- Incluya evidencia de barreras implementadas
- Muestre las medidas de reducción de riesgo

### 2. Presentaciones a Stakeholders
- Cree informes profesionales para reuniones
- Documente claramente los riesgos identificados
- Presente las medidas de mitigación

### 3. Archivo Histórico
- Genere informes de eventos importantes
- Mantenga registro de análisis de riesgo
- Documente la evolución del análisis

### 4. Reportes Regulatorios
- Cumpla con requisitos de documentación
- Presente informes estandarizados
- Facilite la trazabilidad

## Consejos y Mejores Prácticas

### Antes de Generar

1. **Verifique los datos del evento**:
   - Asegúrese que la descripción sea clara y completa
   - Revise que las barreras estén activas si deben incluirse
   - Confirme que los reductores estén marcados como activos

2. **Organice la información**:
   - Complete todas las secciones del evento
   - Añada descripciones detalladas a las barreras
   - Especifique la implementación de cada barrera

3. **Revise la vista previa**:
   - Use la vista previa para confirmar el contenido
   - Verifique el conteo de barreras y reductores
   - Confirme que la etapa sea correcta

### Durante la Generación

1. **Nombre de archivos**:
   - Use nombres descriptivos: `Informe_E001_Fallo_Sistema_2025.pdf`
   - Incluya la fecha si genera versiones múltiples
   - Evite caracteres especiales en el nombre

2. **Ubicación de guardado**:
   - Cree una carpeta específica para informes
   - Use una estructura organizada por fecha o etapa
   - Asegúrese de tener espacio en disco

3. **Verificación**:
   - Abra el PDF inmediatamente para verificar
   - Revise que todas las secciones estén presentes
   - Confirme que el formato sea correcto

### Después de Generar

1. **Revisión del contenido**:
   - Verifique que toda la información sea precisa
   - Asegúrese que las tablas sean legibles
   - Confirme que no falten datos

2. **Distribución**:
   - Comparta el PDF con las partes interesadas
   - Mantenga una copia en el archivo del proyecto
   - Considere el control de versiones

3. **Archivo**:
   - Organice los informes por fecha o proyecto
   - Mantenga backup de informes importantes
   - Documente la fecha de generación

## Solución de Problemas

### El menú "Generar informe HTML..." está deshabilitado

**Causa**: No hay un modelo abierto en MARRTA

**Solución**:
1. Abra un modelo existente: Archivo → Abrir
2. O cree un nuevo modelo: Archivo → Nuevo
3. Intente acceder al menú nuevamente

### No aparecen eventos en el selector

**Causa**: El modelo no tiene eventos registrados

**Solución**:
1. Vaya a Gestión de Sucesos
2. Añada al menos un evento al modelo
3. Vuelva a abrir el generador de informes

### El PDF no se genera o muestra error

**Posibles causas y soluciones**:

1. **Permisos insuficientes**:
   - Elija una ubicación donde tenga permisos de escritura
   - Intente guardar en Documentos o Escritorio

2. **Espacio en disco**:
   - Verifique que hay espacio disponible
   - Libere espacio si es necesario

3. **Ruta inválida**:
   - Evite caracteres especiales en la ruta
   - Use nombres cortos y descriptivos

4. **Archivo en uso**:
   - Cierre el PDF si está abierto
   - Use un nombre diferente

### Las tablas no muestran datos

**Causa**: Las barreras/reductores están inactivos

**Solución**:
1. Abra el evento en Gestión de Sucesos
2. Marque las defensas como activas
3. Guarde los cambios
4. Genere el informe nuevamente

### El formato no se ve correctamente

**Causa**: Visor de PDF incompatible

**Solución**:
1. Abra con Adobe Reader o similar
2. Verifique la configuración de zoom
3. Pruebe con otro visor de PDF

## Limitaciones Conocidas

1. **Un evento por informe**: Actualmente solo se puede generar un informe para un evento a la vez
2. **Sin personalización**: El formato y colores son fijos
3. **Solo PDF**: No se puede exportar a otros formatos directamente
4. **Sin gráficos**: No incluye gráficos de matrices de riesgo

## Preguntas Frecuentes

### ¿Puedo personalizar el formato del informe?

Actualmente el formato es fijo y sigue el estándar profesional de MARRTA. En futuras versiones podrían añadirse opciones de personalización.

### ¿Puedo generar múltiples informes a la vez?

No, actualmente solo se puede generar un informe por evento. Para múltiples eventos, genere un informe por cada uno.

### ¿Se puede incluir el logo de mi empresa?

En la versión actual no. Esta característica podría añadirse en futuras actualizaciones.

### ¿El informe incluye gráficos?

No, el informe actual es principalmente textual con tablas. Los gráficos de matriz de riesgo no se incluyen.

### ¿Puedo editar el PDF después de generarlo?

El PDF generado es estático. Si necesita modificaciones, deberá usar un editor de PDF externo o generar un nuevo informe con datos actualizados.

### ¿El informe está disponible en otros idiomas?

Sí, el informe respeta la configuración de idioma de MARRTA. Si tiene MARRTA en inglés, el informe se generará en inglés.

## Soporte Adicional

Para más información:
- Consulte el manual de usuario de MARRTA
- Revise la documentación técnica en `Documentation/Support/`
- Visite: https://github.com/theotocopulitos/MARRTA

---

*Guía creada: 2025-10-13*
*Versión de MARRTA: 1.5+*
*Idiomas disponibles: Español, Inglés*
