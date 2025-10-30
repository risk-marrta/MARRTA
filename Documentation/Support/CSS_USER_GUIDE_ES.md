# Guía Rápida: Personalización de Estilos CSS en Informes

## ¿Qué es la gestión de CSS?

MARRTA ahora permite personalizar los estilos visuales de los informes HTML/PDF mediante archivos CSS. Puedes cambiar colores, fuentes, espaciados y más sin modificar el código de la aplicación.

## Ubicación de los Botones

En el diálogo **"Configurar Informe"**, encontrarás tres botones nuevos en la sección **"Gestión de Estilos CSS"**:

```
┌─────────────────────────────────────────────────────┐
│        Gestión de Estilos CSS                       │
├─────────────────────────────────────────────────────┤
│  [Guardar CSS] [Importar CSS] [Restaurar por defecto] │
└─────────────────────────────────────────────────────┘
```

## Uso Básico

### 1. Exportar el CSS Actual

**Cuándo usar:** Cuando quieres editar los estilos actuales

**Pasos:**
1. Click en **"Guardar CSS"**
2. Elige dónde guardar el archivo (ej: `Escritorio/mi_estilo.css`)
3. Abre el archivo con un editor de texto (Notepad++, VS Code, etc.)
4. Edita los estilos que desees cambiar
5. Guarda el archivo

### 2. Importar CSS Personalizado

**Cuándo usar:** Después de editar el archivo CSS

**Pasos:**
1. Click en **"Importar CSS"**
2. Selecciona tu archivo CSS editado
3. Confirma la importación
4. Los próximos informes usarán tu estilo personalizado

### 3. Restaurar Estilo por Defecto

**Cuándo usar:** Para volver al estilo original de MARRTA

**Pasos:**
1. Click en **"Restaurar por defecto"**
2. Confirma la acción
3. Los informes volverán a usar el estilo azul profesional original

## Ejemplos de Personalización

### Cambiar el Color de la Portada

**Por defecto:** Azul (#4A7EBB)

Busca en el CSS:
```css
.cover-title {
  background-color: #4A7EBB;
  ...
}
```

Cambia a verde:
```css
.cover-title {
  background-color: #2E7D32;
  ...
}
```

### Cambiar la Fuente

**Por defecto:** Arial, Helvetica, sans-serif

Busca:
```css
body {
  font-family: Arial, Helvetica, sans-serif;
  ...
}
```

Cambia a Times New Roman:
```css
body {
  font-family: 'Times New Roman', serif;
  ...
}
```

### Cambiar Colores de Encabezados de Sección

**Por defecto:** Azul oscuro (#2E5C8A)

Busca:
```css
.section-title {
  background-color: #2E5C8A;
  ...
}
```

Cambia a naranja:
```css
.section-title {
  background-color: #E65100;
  ...
}
```

### Cambiar Tamaño de Fuente

**Por defecto:** 10pt

Busca en múltiples lugares y ajusta:
```css
body {
  font-size: 10pt;
  ...
}
```

Cambia a 12pt:
```css
body {
  font-size: 12pt;
  ...
}
```

### Cambiar Espaciado de Tablas

Busca:
```css
table {
  margin: 20px 0;
  ...
}
```

Aumenta el espaciado:
```css
table {
  margin: 30px 0;
  ...
}
```

## Paleta de Colores por Defecto

```
Azul Principal:       #4A7EBB
Azul Oscuro:          #2E5C8A
Azul Claro:           #6B9FD4
Gris Claro:           #F5F5F5
Gris Medio:           #DDDDDD
Gris Borde:           #D9D9D9
Negro Texto:          #333333
Gris Texto:           #666666
```

## Consejos de Personalización

### ✅ Buenas Prácticas

- **Prueba incremental**: Cambia una cosa a la vez
- **Guarda copias**: Mantén una copia del CSS original
- **Valida colores**: Usa herramientas online para elegir colores
- **Contraste**: Asegura que el texto sea legible
- **Consistencia**: Mantén un esquema de colores coherente

### ❌ Errores Comunes

- **No cerrar llaves**: Cada `{` debe tener su `}`
- **Olvidar punto y coma**: Cada propiedad CSS termina en `;`
- **Colores inválidos**: Usa formato hexadecimal (#RRGGBB) o nombres CSS
- **Unidades faltantes**: Usa `px`, `pt`, `em`, etc. para tamaños
- **Selectores incorrectos**: Respeta mayúsculas en nombres de clases

## Clases CSS Principales

| Clase | Descripción |
|-------|-------------|
| `.cover-title` | Título de la portada |
| `.section-title` | Títulos de sección (alineados a derecha) |
| `.section-header` | Encabezados de sección generales |
| `.subsection-header` | Subencabezados |
| `.content-block` | Bloques de contenido con borde |
| `.initiator-card` | Tarjetas de iniciadores |
| `.defense-section` | Secciones de defensas |
| `table` | Todas las tablas |
| `.summary-table` | Tablas de resumen |
| `.page-break` | Saltos de página |
| `.centered-table` | Tablas centradas |

## Recursos Útiles

### Editores de CSS Recomendados
- **Visual Studio Code** (Gratis, con syntax highlighting)
- **Notepad++** (Gratis, ligero)
- **Sublime Text** (Gratis/Pago, muy rápido)

### Herramientas Online
- **Color Picker**: https://htmlcolorcodes.com/
- **CSS Validator**: https://jigsaw.w3.org/css-validator/
- **Google Fonts**: https://fonts.google.com/ (para fuentes)

### Aprender CSS
- **MDN Web Docs**: https://developer.mozilla.org/es/docs/Web/CSS
- **W3Schools CSS**: https://www.w3schools.com/css/

## Solución de Problemas

### Problema: Los cambios no se aplican

**Soluciones:**
1. Verifica que importaste el CSS después de editarlo
2. Cierra y reabre el diálogo de configuración
3. Genera un nuevo informe (no uses uno viejo)
4. Verifica que el archivo CSS no tenga errores de sintaxis

### Problema: El informe se ve mal

**Soluciones:**
1. Click en "Restaurar por defecto"
2. Verifica el CSS en un validador online
3. Revisa que todas las llaves `{}` estén balanceadas
4. Busca puntos y coma `;` faltantes

### Problema: No encuentro qué cambiar

**Solución:**
1. Genera un informe de prueba
2. Click en "Vista Previa HTML"
3. Click en "Inspeccionar estilos" (botón inferior izquierdo)
4. Usa el inspector para ver qué clases CSS aplican a cada elemento
5. Anota los nombres de las clases
6. Busca esas clases en tu archivo CSS

## Archivos de Ubicación

### Windows
```
Estilo personalizado: C:\Users\<tu_usuario>\AppData\Roaming\MARRTA\report_style.css
```

### Linux
```
Estilo personalizado: ~/.local/share/MARRTA/report_style.css
```

### macOS
```
Estilo personalizado: ~/Library/Application Support/MARRTA/report_style.css
```

## Preguntas Frecuentes

### ¿Los cambios son permanentes?
Sí, una vez importado un CSS personalizado, todos los informes futuros lo usarán hasta que lo cambies o restaures el por defecto.

### ¿Puedo compartir mi CSS con otros?
Sí, usa "Guardar CSS" para exportar tu estilo y compártelo. Otros pueden importarlo con "Importar CSS".

### ¿Afecta a informes ya generados?
No, solo afecta a informes nuevos. Los PDFs ya generados mantienen su estilo original.

### ¿Puedo tener múltiples estilos?
No directamente, pero puedes guardar múltiples archivos CSS y cambiar entre ellos importándolos cuando necesites.

### ¿Qué pasa si mi CSS tiene errores?
El informe puede verse mal o no generarse correctamente. Usa "Restaurar por defecto" para volver a un estado funcional.

### ¿El logo también se puede cambiar?
No mediante CSS, el logo está embebido en el HTML. Para cambiarlo necesitas modificar el código fuente.

## Soporte

Si tienes problemas o preguntas:
1. Consulta la guía de testing: `CSS_MANAGEMENT_TESTING_GUIDE.md`
2. Revisa la arquitectura: `CSS_ARCHITECTURE.md`
3. Contacta al soporte técnico de MARRTA

---

**Versión:** 1.0  
**Fecha:** 2025-10-17  
**Aplicable a:** MARRTA v1.0+
