# Update Checker Feature

## Descripción

MARRTA ahora incluye la funcionalidad de comprobar actualizaciones automáticamente o manualmente.

## Funcionalidades

### 1. Comprobación de Versiones de MARRTA

La aplicación puede comprobar si hay nuevas versiones disponibles en el repositorio de GitHub.

#### Comprobación Manual
- Ve al menú **Ayuda → Comprobar actualizaciones...**
- La aplicación consultará la API de GitHub y te informará si hay una nueva versión disponible
- Si hay una actualización, podrás visitar directamente la página de descargas

#### Comprobación Automática
- La aplicación comprueba automáticamente cada 2 semanas al iniciar
- Si hay una nueva versión disponible, te lo notificará
- La comprobación es silenciosa: solo te notifica si hay actualizaciones

### 2. Comprobación de Nuevos Modelos

La aplicación también puede comprobar si hay nuevos modelos de riesgo disponibles en el repositorio `theotocopulitos/marrta-modelos-de-riesgo`.

#### Comprobación Manual
- Ve al menú **Ayuda → Comprobar nuevos modelos...**
- La aplicación consultará la API de GitHub y te informará si hay nuevos modelos desde la última comprobación
- Si hay nuevos modelos, podrás visitar directamente la página de descargas

#### Comprobación Automática
- La aplicación comprueba automáticamente cada 2 semanas al iniciar
- Si hay nuevos modelos disponibles, te lo notificará con los nombres de los modelos
- La comprobación es silenciosa: solo te notifica si hay nuevos modelos

## Configuración

Las fechas de las últimas comprobaciones se guardan en el archivo de configuración (`marrta.ini`):
- `lastVersionCheck`: Última comprobación de versión
- `lastModelsCheck`: Última comprobación de modelos

## Detalles Técnicos

### Clase UpdateChecker

La clase `UpdateChecker` gestiona todas las comprobaciones:

- **checkForUpdates(bool silent)**: Comprueba nuevas versiones de MARRTA
- **checkForNewModels(bool silent)**: Comprueba nuevos modelos en el repositorio
- **autoCheckOnStartup(QSettings*)**: Realiza comprobaciones automáticas si han pasado 14 días

### Comparación de Versiones

El sistema compara versiones usando semántica de versiones (MAJOR.MINOR.PATCH):
- Ejemplo: `1.4.4` vs `1.4.5` → Nueva versión disponible
- Ejemplo: `1.4.4` vs `2.0.0` → Nueva versión disponible  
- Ejemplo: `1.4.4` vs `1.4.4` → Sin actualizaciones
- Ejemplo: `1.4.4` vs `1.4.3` → Sin actualizaciones

### API de GitHub

La aplicación utiliza la API pública de GitHub:
- **Releases de MARRTA**: `https://api.github.com/repos/theotocopulitos/MARRTA/releases/latest`
- **Releases de modelos**: `https://api.github.com/repos/theotocopulitos/marrta-modelos-de-riesgo/releases`

No se requiere autenticación para estas consultas.

## Requisitos

- Conexión a Internet para realizar las comprobaciones
- Qt Network module (ya incluido en el proyecto)

## Archivos Modificados/Añadidos

### Nuevos Archivos
- `updatechecker.h`: Declaración de la clase UpdateChecker
- `updatechecker.cpp`: Implementación de la clase UpdateChecker

### Archivos Modificados
- `mainwindow.h`: Añadida referencia a UpdateChecker y nuevos slots
- `mainwindow.cpp`: Integración de UpdateChecker en la ventana principal
- `mainwindow.ui`: Añadidos menús "Comprobar actualizaciones" y "Comprobar nuevos modelos"
- `marrta.pro`: Añadido módulo Qt Network y nuevos archivos al proyecto

## Pruebas

### Prueba Manual de Versiones
1. Abrir MARRTA
2. Ir a **Ayuda → Comprobar actualizaciones...**
3. Verificar que aparece un mensaje (con o sin actualizaciones)

### Prueba Manual de Modelos
1. Abrir MARRTA
2. Ir a **Ayuda → Comprobar nuevos modelos...**
3. Verificar que aparece un mensaje (con o sin nuevos modelos)

### Prueba Automática
1. Borrar las entradas `lastVersionCheck` y `lastModelsCheck` del archivo `marrta.ini`
2. Abrir MARRTA
3. Si hay actualizaciones o nuevos modelos, deberían aparecer notificaciones

## Notas de Implementación

- La comprobación automática solo se realiza si han pasado 14 días (2 semanas) desde la última comprobación
- Las comprobaciones automáticas son silenciosas: solo notifican si hay novedades
- Las comprobaciones manuales siempre muestran un mensaje, incluso si no hay novedades
- Los errores de red en comprobaciones silenciosas no se muestran al usuario (solo se registran en el log)
- Los errores de red en comprobaciones manuales se muestran al usuario
