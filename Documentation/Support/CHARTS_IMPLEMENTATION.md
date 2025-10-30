# Implementación de Gráficas de Distribución

## Resumen

Se han añadido nuevas gráficas de distribución por frecuencia y consecuencia al diálogo de resumen del modelo. Estas gráficas permiten visualizar la diferencia entre los valores base (asignados originalmente) y los valores calculados (después de aplicar las defensas). Además, se incluye funcionalidad para exportar las gráficas como imágenes PNG de alta resolución para su inclusión en informes.

## Características Implementadas

### 1. Gráfica de Distribución por Frecuencia

**Ubicación**: Pestaña "Distribución por frecuencia" en el diálogo de Resumen

**Contenido**:
- Muestra la distribución de eventos por niveles de frecuencia
- Superpone dos series de barras:
  - **Valores Base** (azul claro): Frecuencias asignadas originalmente a los eventos
  - **Valores Calculados** (gris oscuro): Frecuencias después de aplicar reductores de frecuencia

**Categorías del eje X**:
- Muy Baja (0)
- Baja (1)
- Media (2)
- Alta (3)

**Colores utilizados**:
- Valores Base: #7cb5ec (azul claro)
- Valores Calculados: #434348 (gris oscuro)

**Exportación**: Botón "Exportar como PNG" en la parte inferior de la pestaña

### 2. Gráfica de Distribución por Consecuencia

**Ubicación**: Pestaña "Distribución por consecuencia" en el diálogo de Resumen

**Contenido**:
- Muestra la distribución de eventos por niveles de consecuencia
- Superpone dos series de barras:
  - **Valores Base** (verde claro): Consecuencias asignadas originalmente a los eventos
  - **Valores Calculados** (naranja): Consecuencias después de aplicar reductores de consecuencia

**Categorías del eje X**:
- Bajas (0)
- Medias (1)
- Altas (2)
- Muy Altas (3)

**Colores utilizados**:
- Valores Base: #90ed7d (verde claro)
- Valores Calculados: #f7a35c (naranja)

**Exportación**: Botón "Exportar como PNG" en la parte inferior de la pestaña

### 3. Gráfica de Distribución de Riesgo por Etapas (Existente)

**Ubicación**: Pestaña "Riesgo por etapas" en el diálogo de Resumen

**Contenido**:
- Muestra la distribución de riesgo calculado agrupado por etapas
- Utiliza únicamente valores calculados (después de aplicar defensas)
- Series apiladas por nivel de riesgo: RB, RM, RA, RMA

**Exportación**: Botón "Exportar como PNG" en la parte inferior de la pestaña

### 4. Exportación a PNG (Nueva Funcionalidad)

**Características de la exportación**:
- **Resolución**: 1200x800 píxeles (alta calidad para informes)
- **Formato**: PNG con fondo blanco
- **Antialiasing**: Activado para mejor calidad visual
- **Nombres por defecto**:
  - Frecuencia: `distribucion_frecuencia.png`
  - Consecuencia: `distribucion_consecuencia.png`
  - Riesgo por etapas: `distribucion_riesgo_etapas.png`

**Proceso de exportación**:
1. El usuario hace clic en el botón "Exportar como PNG" en la pestaña deseada
2. Se abre un diálogo para seleccionar ubicación y nombre del archivo
3. La gráfica se renderiza en alta resolución
4. Se guarda el archivo PNG
5. Se muestra un mensaje de confirmación con la ruta del archivo guardado

**Mensajes de confirmación**:
- Éxito: "Gráfica exportada correctamente a: [ruta]"
- Error: "No se pudo guardar la imagen en: [ruta]"
- Sin gráfica: "No hay gráfica disponible para exportar"

## Archivos Modificados

### resumen.h
- Añadidas declaraciones de métodos:
  - `void drawFrequencyChart()`
  - `void drawConsequenceChart()`
  - `void exportChartToPNG(QChartView *chartView, const QString &defaultFileName)`
- Añadidos slots para exportación:
  - `void on_btExportFrequencyChart_clicked()`
  - `void on_btExportConsequenceChart_clicked()`
  - `void on_btExportRiskChart_clicked()`
- Añadidas variables miembro para almacenar referencias a las vistas de gráficas:
  - `QChartView *frequencyChartView`
  - `QChartView *consequenceChartView`
  - `QChartView *riskChartView`

### resumen.cpp
- Implementados los nuevos métodos de gráficas
- Implementado método `exportChartToPNG()` para exportación de imágenes PNG
- Implementados slots de exportación para cada gráfica
- Llamadas a `drawFrequencyChart()` y `drawConsequenceChart()` en el constructor
- Añadido uso de `tr()` para todos los textos mostrados en la UI (internacionalización)
- Añadidos includes necesarios: `QFileDialog`, `QMessageBox`, `QPixmap`, `QPainter`

### resumen.ui
- Añadida pestaña "Distribución por frecuencia" con widget `frequencyChartsWidget`
- Añadida pestaña "Distribución por consecuencia" con widget `consequenceChartsWidget`
- Añadidos botones "Exportar como PNG" en cada una de las 3 pestañas de gráficas
- Reorganización de pestañas para mejor flujo de visualización

## Cálculo de Valores

### Valores Base
Los valores base son los asignados originalmente al evento:
- `event.frecuencia`: Frecuencia base (0-3)
- `event.consecuencia`: Consecuencia base (0-3)

### Valores Calculados
Los valores calculados se obtienen después de aplicar las defensas:
- `event.frecuenciaCalculada`: Frecuencia ajustada por reductores de frecuencia y error humano
- `event.consecuenciaCalculada`: Consecuencia ajustada por reductores de consecuencia

## Consideraciones Técnicas

1. **Iteración sobre eventos**: Las gráficas iteran sobre todos los eventos almacenados en el mapa `events` (organizado por etapas)

2. **Rangos de valores**: Se valida que los valores estén en el rango 0-3 antes de contabilizarlos

3. **Eje Y dinámico**: El rango del eje Y se ajusta automáticamente al valor máximo + 5 para mejor visualización

4. **Internacionalización**: Todos los textos visibles utilizan `tr()` para permitir traducciones futuras

5. **Bibliotecas Qt Charts**: Se utilizan las clases estándar de Qt Charts:
   - QBarSet
   - QBarSeries
   - QChart
   - QChartView
   - QBarCategoryAxis
   - QValueAxis

## Uso

1. Abrir el diálogo de Resumen desde el menú principal
2. Navegar a las pestañas:
   - "Distribución por frecuencia" para ver el impacto de los reductores de frecuencia
   - "Distribución por consecuencia" para ver el impacto de los reductores de consecuencia
   - "Riesgo por etapas" para ver la distribución de riesgo calculado

3. Las gráficas muestran visualmente:
   - Cómo las defensas reducen los niveles de frecuencia y consecuencia
   - La efectividad general del sistema de defensas implementado
   - Distribución de eventos por categorías de riesgo

4. Exportar gráficas para informes:
   - Hacer clic en el botón "Exportar como PNG" en la pestaña deseada
   - Seleccionar ubicación y nombre del archivo en el diálogo
   - La imagen PNG se guardará en alta resolución (1200x800)
   - Las imágenes exportadas pueden incluirse directamente en informes

## Beneficios

- **Visualización clara**: Permite ver de un vistazo el impacto de las defensas aplicadas
- **Comparación directa**: Las barras superpuestas facilitan la comparación entre valores base y calculados
- **Análisis de efectividad**: Ayuda a identificar si las defensas están funcionando correctamente
- **Soporte para decisiones**: Proporciona información visual para decisiones sobre mejoras en el modelo
- **Exportación para informes**: Las imágenes PNG de alta calidad pueden incluirse en documentación y presentaciones
- **Calidad profesional**: Resolución 1200x800 con antialiasing para presentaciones y reportes formales
