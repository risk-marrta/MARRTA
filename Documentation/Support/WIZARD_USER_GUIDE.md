# Guía de Usuario - Asistente de Creación de Nuevo Modelo

## Acceso al Asistente

Para crear un nuevo modelo, vaya al menú principal:
**Archivo → Modelo → Crear...**

## Flujo del Asistente

### Paso 1: Ubicación del Modelo

En el primer paso, debe seleccionar dónde guardar el archivo del modelo (.pdb):

- Haga clic en el botón **"Examinar..."**
- Seleccione la ubicación y nombre del archivo
- El archivo se guardará con extensión .pdb automáticamente si no la especifica
- Haga clic en **"Siguiente"** para continuar

**Nota:** Si ya existe un archivo con el mismo nombre, será reemplazado.

### Paso 2: Información Básica del Modelo

En este paso, ingrese la información descriptiva del modelo:

**Campos disponibles:**
- **Nombre del modelo** (obligatorio): Por ejemplo, "Acelerador Lineal"
- **Fecha de inicio**: Por defecto es la fecha actual, pero puede cambiarla usando el selector de fechas
- **Técnica**: Por ejemplo, "Radioterapia"
- **Analistas**: Nombres de los analistas (campo multilínea)
- **Institución/Hospital**: Nombre de la institución
- **Descripción**: Descripción general del modelo (campo multilínea)
- **Observaciones**: Observaciones adicionales (campo multilínea)

**Nota:** Solo el nombre del modelo es obligatorio. Los demás campos son opcionales.

Haga clic en **"Siguiente"** para continuar.

### Paso 3: Etapas del Proceso

Este paso es completamente opcional. Puede:
- Añadir etapas y subetapas ahora
- Omitir este paso y añadirlas más tarde en la aplicación

**Para añadir una etapa:**
1. Haga clic en **"Añadir Etapa"**
2. Se le pedirá el nombre de la etapa (por ejemplo, "Control de calidad")
3. Se le pedirá el código de la etapa (3 caracteres, por ejemplo, "CON")
4. El sistema preguntará si desea añadir subetapas a esta etapa

**Para añadir subetapas:**
1. Cuando se le pregunte, seleccione "Sí"
2. Ingrese el nombre de la subetapa (por ejemplo, "Calibración")
3. El sistema preguntará si desea añadir más subetapas
4. Continúe añadiendo subetapas o seleccione "No" cuando haya terminado

**Características:**
- Las etapas aparecen en el árbol con su nombre y código
- Las subetapas aparecen anidadas bajo su etapa padre
- Solo las etapas tienen código, las subetapas no
- Puede eliminar etapas o subetapas seleccionándolas y haciendo clic en **"Eliminar Etapa"**

Haga clic en **"Crear modelo"** para continuar.

### Paso 4: Creación del Modelo

En este paso final, el asistente creará automáticamente el modelo:

1. Verá una barra de progreso
2. El sistema creará la base de datos SQLite con todas las tablas necesarias
3. Se insertará la información de configuración
4. Se crearán las etapas y subetapas (si las añadió)

**Mensajes posibles:**
- **"¡Modelo creado exitosamente!"**: El modelo se creó correctamente
- **"Error al crear el modelo"**: Hubo un problema durante la creación

Una vez completado, haga clic en **"Cerrar"** para salir del asistente.

## Estructura de la Base de Datos Creada

El asistente crea un archivo .pdb (base de datos SQLite) con las siguientes tablas:

1. **configuracion**: Almacena toda la información básica del modelo
2. **etapas**: Almacena las etapas del proceso con sus códigos
3. **subetapas**: Almacena las subetapas vinculadas a cada etapa
4. **suceso_iniciador**: Tabla de eventos (inicialmente vacía)
5. **defensa**: Tabla de defensas (inicialmente vacía)

Puede abrir el modelo creado inmediatamente desde:
**Archivo → Modelo → Abrir**

## Consejos

- La fecha por defecto es la fecha actual, pero puede cambiarla si lo desea
- No es obligatorio añadir etapas durante la creación; puede hacerlo después
- Los códigos de etapa deben ser cortos y descriptivos (recomendado 3 caracteres)
- Las subetapas no tienen códigos en el sistema
- Puede crear un modelo mínimo con solo el nombre y añadir el resto más tarde
