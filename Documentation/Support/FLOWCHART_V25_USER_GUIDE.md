# Flujograma V25 desde Base de Datos - Guía de Usuario

## Resumen

El script `generar_flujograma_v25_desde_bd.py` genera un flujograma interactivo en HTML a partir del modelo de riesgo cargado en MARRTA. Es una réplica exacta del generador v25 original pero lee datos desde la base de datos en lugar de archivos Excel.

## Características

### Visualización Completa del Modelo

- **Etapas y Subetapas**: Organización jerárquica vertical
- **Sucesos Iniciadores**: Eventos que inician la secuencia de riesgo
- **Defensas Únicas**: Barreras, Reductores de Frecuencia y Reductores de Consecuencia
  - Agrupadas por (Etapa, Código) para evitar duplicados
  - Conectadas a sus sucesos objetivo con líneas
- **Etapas Ficticias**: Representadas en los extremos del diagrama

### Layout Vertical Multi-columna

```
┌──────────┬──────────┬────────────────┬──────────┬──────────┬──────────┬──────────┐
│ Etapas   │ Barreras │    Etapas      │ Barreras │   RF     │   RC     │   RF     │
│ Ficticias│ Ficticias│    Reales      │  Reales  │  Reales  │  Reales  │ Ficticias│
│ (izq.)   │          │  + Subetapas   │          │          │          │          │
│          │          │  + Iniciadores │          │          │          │          │
└──────────┴──────────┴────────────────┴──────────┴──────────┴──────────┴──────────┘
```

### Interactividad

#### Navegación
- **Zoom**: Rueda del ratón (centrado en columna de iniciadores)
- **Pan**: Arrastrar canvas con el ratón
- **Turbo Pan**: Mantener Shift mientras arrastras (8x más rápido)
- **Botones**: +, -, Reiniciar en el panel

#### Filtros
- **Por Etapa**: Filtrar sucesos y defensas por etapa
- **Por Tipo**: Mostrar/ocultar Barreras, RF, RC
- **Por Texto**: Buscar por nombre o código
- **Por Robustez**: 0 (baja), 1 (normal), 2 (robusta), 3 (muy robusta)
- **Mostrar/Ocultar**: Subetapas, Iniciadores
- **Solo Defensas Activas**: Mostrar solo defensas conectadas a sucesos visibles

#### Selección
- **Click en Iniciador**: Resalta defensas conectadas y sus flechas
- **Click en Defensa**: Resalta iniciadores conectados
- **Click en Fondo**: Limpia selección

#### Tooltips
- Pasa el ratón sobre cualquier elemento para ver detalles

### Exportación

- **SVG**: Vector escalable, editable en Inkscape/Illustrator
- **PNG**: Imagen raster, lista para documentos

## Uso

### 1. Preparación

Asegúrate de tener un modelo cargado en MARRTA con:
- Etapas definidas (reales y ficticias)
- Subetapas (opcional)
- Sucesos iniciadores
- Defensas (Barreras, RF, RC) asociadas a sucesos

### 2. Ejecución

#### Desde el Editor de Python de MARRTA

1. Abre MARRTA
2. Carga tu modelo de riesgo
3. Ve a **Modelo → Scripting → Python**
4. Carga el script: `test_scripts/generar_flujograma_v25_desde_bd.py`
5. Ejecuta (botón "Ejecutar" o F5)
6. El script generará `flujograma_vertical_v25_desde_bd.html` en la carpeta `test_scripts/`

#### Desde Línea de Comandos (alternativa)

```bash
# Nota: Este método requiere tener los módulos Python de MARRTA disponibles
# Es preferible usar el editor integrado de MARRTA
python generar_flujograma_v25_desde_bd.py [archivo_salida.html]
```

### 3. Visualización

1. Abre `flujograma_vertical_v25_desde_bd.html` en un navegador web moderno
   - Chrome, Firefox, Safari, Edge (recomendados)
2. El flujograma se cargará con todos los elementos visibles
3. Usa el panel de control (esquina superior derecha) para filtrar y explorar

## Codificación por Colores

### Tipos de Nodos

- **Etapas Reales**: Fondo azul claro (#dbeafe)
- **Subetapas**: Fondo azul muy claro (#eff6ff), borde punteado
- **Iniciadores**: Fondo naranja claro (#fff7ed)
- **Etapas Ficticias**: Fondo gris (#e5e7eb)

### Robustez de Defensas

- **0 - Baja**: Fondo blanco (#ffffff)
- **1 - Normal**: Fondo verde claro (#eefdd3)
- **2 - Robusta**: Fondo naranja claro (#ffedd5)
- **3 - Muy Robusta**: Fondo rojo claro (#fee2e2)

### Flechas

- **Barreras**: Negro (por defecto)
- **Reductores de Frecuencia**: Verde (#16a34a)
- **Reductores de Consecuencia**: Azul (#2563eb)
- **Seleccionadas**: Más gruesas y opacas
- **Atenuadas**: Semi-transparentes cuando hay selección activa

## Estructura del HTML Generado

El archivo HTML es **completamente autónomo**:
- CSS embebido
- JavaScript embebido
- Sin dependencias externas
- Funciona offline

Componentes:
- **SVG Canvas**: Gráfico vectorial con todos los elementos
- **Panel de Control**: Interfaz de filtros y herramientas
- **JavaScript**: Lógica de interacción, filtros, zoom/pan, exportación

## Solución de Problemas

### El script no encuentra los módulos

**Error:** `NameError: name 'Etapas' is not defined`

**Solución:** El script debe ejecutarse desde el editor de Python de MARRTA, no directamente desde la terminal. Los módulos `Etapas`, `Sucesos`, `Defensas` solo están disponibles en el contexto del editor.

### El HTML está vacío o sin elementos

**Causas posibles:**
1. Modelo no tiene datos en las tablas
2. Todas las defensas/sucesos están inactivos
3. Error en la generación (revisar console de Python)

**Solución:** Verifica que el modelo tiene:
```python
# Ejecuta en editor de Python para verificar
print(f"Etapas: {len(Etapas.get_all())}")
print(f"Sucesos: {len(Sucesos.get_all())}")
print(f"Defensas: {len(Defensas.get_all())}")
```

### El zoom/pan no funciona

**Causa:** JavaScript deshabilitado en el navegador

**Solución:** Habilita JavaScript y recarga la página

### Exportación PNG falla

**Causa:** Limitaciones de seguridad del navegador (CORS)

**Solución:** 
1. Abre el HTML desde el sistema de archivos local (`file://`)
2. O usa un servidor web local simple:
   ```bash
   python -m http.server 8000
   # Luego abre http://localhost:8000/flujograma_vertical_v25_desde_bd.html
   ```

## Diferencias con Versión Original (Excel)

| Aspecto | Versión Excel | Versión BD |
|---------|---------------|------------|
| **Fuente de Datos** | Archivo .xlsx | Base de datos cargada en MARRTA |
| **Agregación** | Explícita en Excel (listas separadas por `;`) | Automática desde relaciones BD |
| **Subetapas** | Hoja dedicada | Extraídas de sucesos |
| **Layout/UX** | ✓ | ✓ (idéntico) |
| **Interactividad** | ✓ | ✓ (idéntica) |
| **Exportación** | ✓ | ✓ (idéntica) |

## Requisitos Técnicos

### En MARRTA

- MARRTA con soporte para Python scripting
- Módulos Python API: `Etapas`, `Sucesos`, `Defensas`
- pandas instalado (incluido en MARRTA)
- numpy instalado (incluido en MARRTA)

### Para Visualización

- Navegador web moderno (Chrome 90+, Firefox 88+, Safari 14+, Edge 90+)
- JavaScript habilitado
- Resolución mínima: 1024x768 (recomendado: 1920x1080+)

## Personalización

El script puede personalizarse modificando las constantes al inicio:

```python
# Parámetros de layout
STAGE_HEADER_PAD = 50    # Padding superior de etapas
SUB_HEADER_PAD   = 42    # Padding superior de subetapas
BOTTOM_PAD       = 12    # Padding inferior

node_w, node_h_min = 420, 120        # Ancho/alto mínimo etapas
sub_w, sub_h_min = 380, 100          # Ancho/alto mínimo subetapas
init_w_inside, init_h_min = 340, 64  # Ancho/alto mínimo iniciadores
def_w, def_h_min = 300, 60           # Ancho/alto mínimo defensas

v_gap, sub_gap, init_gap, def_gap = 50, 14, 10, 10  # Espaciados
```

## Soporte

Para problemas o preguntas:
1. Revisa la documentación técnica: `PYTHON_API_FLOWCHART_IMPLEMENTATION.md`
2. Consulta ejemplos: `PYTHON_FLOWCHART_EXAMPLES.md`
3. Referencia API: `PYTHON_API_REFERENCE.md`

## Licencia

Este script es parte de MARRTA y sigue la misma licencia del proyecto principal.
