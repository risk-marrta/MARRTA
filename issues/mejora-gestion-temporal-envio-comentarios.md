### Descripción
Actualmente, cuando el usuario intenta enviar un comentario desde el menú de ayuda de la aplicación, el sistema de envío automático no está completamente habilitado. Esto genera una experiencia de usuario confusa y puede llevar a la pérdida de comentarios valiosos. Para mitigar esta situación temporal, se propone implementar un mensaje informativo que guíe al usuario sobre cómo enviar sus comentarios de forma manual.

### Comportamiento actual
1. El usuario accede al menú de ayuda.
2. Selecciona la opción "Enviar comentarios".
3. Intenta realizar el envío, pero no sucede nada porque el sistema automático aún no está habilitado.

### Solución Propuesta
- **Mensaje informativo:** Mostrar un mensaje claro indicando que el sistema automático de envío de comentarios no está disponible temporalmente. 
- **Instrucciones para el usuario:** 
  - Pedir al usuario que envíe un correo manualmente a la dirección: `matrices.marrta@gmail.com`.
  - Sugerir el siguiente formato para el asunto del correo:
    ```
    [marrta][software][bug/comentario según las opciones de la ventana de envío] Título de la propuesta
    ```
  - Incluir un ejemplo del asunto completo:
    ```
    [marrta][software][bug] Error en la funcionalidad de exportación
    ```
- **Formulario enriquecido:** 
  - En el mensaje informativo, mostrar al usuario el contenido del formulario que estaba intentando enviar.
  - Sugerir que copie y pegue este contenido directamente en el cuerpo del correo.
  - Recomendar que adjunte cualquier archivo relacionado (logs, capturas de pantalla, etc.) para facilitar el análisis.

### Detalles Técnicos y Consideraciones
- **Formato del mensaje:** El mensaje debe ser claro, visible y no intrusivo. 
- **Idiomas:** Si la aplicación admite varios idiomas, el mensaje debe estar localizado en todos los idiomas soportados.
- **Validaciones previas:** Si el formulario contiene campos obligatorios, validar que toda la información esté completa antes de mostrar el mensaje.
- **Adjuntos:** Aunque no es posible validar los adjuntos en este paso, sugerir que los incluyan si es relevante.

### Versión
Este cambio debe ser incluido en la versión 2.0.

### Tipo
Enhancement (Mejora)

### Impacto
- **Usuarios afectados:** Todos los usuarios que intenten usar la funcionalidad de envío de comentarios.
- **Beneficio esperado:** Reducción de la frustración del usuario y mejora de la experiencia mientras se habilita el sistema automático.