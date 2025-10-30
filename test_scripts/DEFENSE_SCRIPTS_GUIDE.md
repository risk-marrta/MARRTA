# Guía de Scripts de Análisis de Defensas

Este documento describe los dos nuevos scripts de análisis de defensas creados para MARRTA.

## Script 1: defense_list_grouped.js

### Propósito
Este script genera un listado completo de todas las defensas únicas en el modelo, agrupadas por tipo.

### ¿Qué es una defensa única?
Una defensa se considera única cuando tiene la combinación única de:
- Código
- Nombre
- Nivel de robustez (hardiness)
- Tipo (Reductor de Frecuencia, Barrera, Reductor de Consecuencia)

Si dos defensas tienen el mismo código y nombre pero diferente robustez, se consideran defensas diferentes.

### Información que proporciona
Para cada defensa única, el script muestra:

1. **Código**: Identificador de la defensa (ej: BM-PRO-001)
2. **Nombre**: Nombre descriptivo de la defensa
3. **Robustez**: Nivel de robustez (0-3: Blanda, Normal, Robusta, Muy robusta)
4. **Descripción**: Descripción de la defensa (limitada a 80 caracteres)
5. **Sucesos iniciadores**: Lista de códigos de los sucesos donde aparece la defensa
6. **Total de apariciones**: Número de sucesos en los que aparece

### Salida del script

```
=== REDUCTORES DE FRECUENCIA ===
Total de defensas únicas: 5

1. Código: RF-OP-001
   Nombre: Procedimientos de operación
   Robustez: Normal (1)
   Descripción: Procedimientos operativos estándar para reducir frecuencia...
   Aparece en 3 suceso(s) iniciador(es):
   SI-001, SI-003, SI-007
   
...

=== BARRERAS ===
Total de defensas únicas: 12

1. Código: BM-PRO-001
   Nombre: Sistema de protección automático
   Robustez: Robusta (2)
   Aparece en 5 suceso(s) iniciador(es):
   SI-002, SI-004, SI-005, SI-008, SI-010
   
...

=== RESUMEN ===
Reductores de Frecuencia: 5 defensas únicas
Barreras: 12 defensas únicas
Reductores de Consecuencia: 8 defensas únicas
Total de defensas únicas en el modelo: 25
```

### Casos de uso

1. **Inventario de defensas**: Obtener un listado completo de todas las defensas implementadas
2. **Identificar defensas clave**: Ver qué defensas se usan en múltiples sucesos
3. **Análisis de cobertura**: Entender cuántos sucesos están protegidos por cada defensa
4. **Documentación**: Generar documentación del sistema de defensas

## Script 2: defense_importance_analysis.js

### Propósito
Este script realiza un análisis cuantitativo de la importancia de cada defensa basado en métricas de sensibilidad de riesgo reconocidas en la industria nuclear.

### Fundamento teórico
El análisis se basa en las métricas de importancia de componentes definidas en el documento de la NRC (Nuclear Regulatory Commission):
- Referencia: https://www.nrc.gov/docs/ML1216/ML12160A479.pdf

Estas métricas miden el impacto de cada defensa en el riesgo total del sistema.

### Valores calculados

Para cada defensa única, el script calcula:

#### Valores de riesgo base
- **R**: Riesgo medio de todos los sucesos del modelo
- **Ri**: Riesgo medio solo de los sucesos donde aparece la defensa
- **R1**: Riesgo estimado si la defensa siempre fuera aplicable
- **R0**: Riesgo estimado si la defensa nunca fuera aplicable
- **R-**: Riesgo estimado si la robustez se redujera un nivel
- **R+**: Riesgo estimado si la robustez se incrementara un nivel

#### Métricas de importancia

1. **Fussell-Vesely (F-V)**
   - Fórmula: F-V = Ri/R
   - Interpretación: Contribución relativa de la defensa al riesgo total
   - Valores > 0.1 indican defensas significativas
   - Valores altos: la defensa está presente en sucesos de alto riesgo

2. **Risk Reduction Worth (RRW)**
   - Fórmula: RRW = R/R1
   - Interpretación: Beneficio de tener la defensa operativa
   - Valores cercanos a 1 indican mayor importancia
   - Mide cuánto se reduce el riesgo gracias a la defensa

3. **Risk Reduction Interval (RRI)**
   - Fórmula: RRI = R - R1
   - Interpretación: Reducción absoluta del riesgo
   - Valores positivos: la defensa reduce el riesgo efectivamente
   - Complementa RRW con valores absolutos

4. **Risk Achievement Worth (RAW)**
   - Fórmula: RAW = R0/R
   - Interpretación: Impacto si la defensa falla completamente
   - Valores > 1.5 indican defensas críticas
   - Valores altos: pérdida de la defensa aumentaría mucho el riesgo
   - **Esta es la métrica principal para ordenar defensas por importancia**

5. **Risk Increase Interval (RII)**
   - Fórmula: RII = R0 - R
   - Interpretación: Incremento absoluto del riesgo si falla
   - Complementa RAW con valores absolutos

6. **Risk Reduction Interval para robustez aumentada (RRI+)**
   - Fórmula: RRI+ = R - R+
   - Interpretación: Beneficio de aumentar la robustez
   - Positivo: vale la pena reforzar la defensa

7. **Risk Increase Interval para robustez reducida (RRI-)**
   - Fórmula: RRI- = R- - R
   - Interpretación: Perjuicio de reducir la robustez
   - Positivo: la degradación aumentaría el riesgo

8. **Birnbaum Importance (B)**
   - Fórmula: B = (R0 - R1) / R
   - Interpretación: Sensibilidad del riesgo total a cambios en la defensa
   - Valores altos: pequeños cambios tienen gran impacto
   - Mide la pendiente de la función riesgo vs disponibilidad

9. **Birnbaum Importance para robustez (B+)**
   - Fórmula: B+ = (R- - R+) / R
   - Interpretación: Sensibilidad a cambios en robustez
   - Mide el impacto de mejorar/degradar la fortaleza

### Nota importante sobre cálculos reales

**IMPORTANTE**: Los valores R1, R0, R-, R+ son calculados mediante **modificación real del estado de las defensas**.

El script realiza los siguientes pasos para cada escenario:
1. Modifica el estado de la defensa en la base de datos (activo/inactivo o robustez)
2. Recarga los eventos desde la base de datos, lo cual desencadena el recálculo automático de:
   - Frecuencia reducida (según reductores de frecuencia activos)
   - Probabilidad (según barreras activas)
   - Consecuencia reducida (según reductores de consecuencia activos)
   - Riesgo (mediante la matriz de riesgo)
3. Calcula el riesgo medio con el nuevo escenario y genera el histograma de distribución
4. Restaura el estado original de la defensa

Este proceso garantiza que los cálculos reflejan el comportamiento real del modelo. El script guarda y restaura automáticamente todos los estados de las defensas al finalizar.

**Tiempo de ejecución**: El análisis puede tardar varios minutos en modelos grandes, ya que cada defensa requiere múltiples recálculos del modelo completo.

### Salida del script

```
=== ANÁLISIS DE IMPORTANCIA DE DEFENSAS ===
Calculando métricas de importancia para cada defensa...

Total de eventos: 45
Total de defensas: 120

R (Riesgo medio de todos los sucesos): 1.8667

TOP 20 DEFENSAS MÁS IMPORTANTES (por RAW):

1. BM-PRO-001 - Sistema de protección automático
   Tipo: Barrera | Robustez: Robusta
   Aparece en: 5 evento(s)

   Valores de riesgo:
   - Ri (riesgo en eventos donde aparece): 2.4000
     Histograma Ri:
       R0 (Tolerable):   5 (11.1%)
       R1 (Moderado):    10 (22.2%)
       R2 (Importante):  20 (44.4%)
       R3 (Inaceptable): 10 (22.2%)
   - R1 (si siempre aplicable):            2.1000
     Histograma R1:
       R0 (Tolerable):   8 (17.8%)
       R1 (Moderado):    15 (33.3%)
       R2 (Importante):  18 (40.0%)
       R3 (Inaceptable): 4 (8.9%)
   - R0 (si nunca aplicable):              2.8000
     Histograma R0:
       R0 (Tolerable):   2 (4.4%)
       R1 (Moderado):    8 (17.8%)
       R2 (Importante):  22 (48.9%)
       R3 (Inaceptable): 13 (28.9%)

   Métricas de importancia:
   - F-V  (Fussell-Vesely):                1.2857
   - RRW  (Risk Reduction Worth):          0.8889
   - RRI  (Risk Reduction Interval):       -0.2333
   - RAW  (Risk Achievement Worth):        1.5000
   - RII  (Risk Increase Interval):        0.9333
   - RRI+ (RRI para mayor robustez):       0.1500
   - RRI- (RII para menor robustez):       0.4000
   - B    (Birnbaum Importance):           0.3750
   - B+   (Birnbaum para robustez):        0.2500

...

=== RESUMEN POR TIPO DE DEFENSA ===

Reductor de Frecuencia:
  Total: 15 defensas
  RAW promedio: 1.2345
  RRW promedio: 0.8901
  F-V promedio: 0.9876
  B promedio:   0.3210

Barrera:
  Total: 42 defensas
  RAW promedio: 1.4567
  RRW promedio: 0.8123
  F-V promedio: 1.1234
  B promedio:   0.4321

Reductor de Consecuencia:
  Total: 28 defensas
  RAW promedio: 1.1234
  RRW promedio: 0.9012
  F-V promedio: 0.8765
  B promedio:   0.2890

=== GUÍA DE INTERPRETACIÓN ===
...
```

### Casos de uso

1. **Priorización de mantenimiento**: Identificar las defensas más críticas que requieren mayor atención
2. **Análisis de sensibilidad**: Entender qué defensas tienen mayor impacto en el riesgo
3. **Decisiones de inversión**: Evaluar dónde es más efectivo invertir en mejoras
4. **Identificación de puntos débiles**: Encontrar defensas cuya pérdida tendría mayor impacto
5. **Optimización de robustez**: Determinar qué defensas se beneficiarían más de un aumento en robustez
6. **Análisis de redundancia**: Identificar defensas críticas que podrían necesitar respaldo

### Interpretación de resultados

#### Defensas muy importantes (RAW > 2.0)
- Críticas para la seguridad del sistema
- Su fallo tendría un impacto severo
- Requieren máxima atención en mantenimiento
- Candidatas para redundancia

#### Defensas importantes (1.5 < RAW < 2.0)
- Significativas para el control del riesgo
- Su degradación aumentaría notablemente el riesgo
- Requieren mantenimiento regular

#### Defensas moderadas (1.2 < RAW < 1.5)
- Contribuyen al control del riesgo
- Su fallo incrementaría moderadamente el riesgo
- Mantenimiento según programa estándar

#### Defensas de bajo impacto (RAW < 1.2)
- Contribución limitada al riesgo total
- Pueden ser candidatas para simplificación
- Revisar si son realmente necesarias

## Cómo usar los scripts

### Requisitos previos
1. Tener MARRTA abierto con un modelo cargado (archivo .pdb)
2. El modelo debe contener eventos iniciadores con defensas

### Pasos para ejecutar

1. Abrir MARRTA
2. Cargar un modelo de análisis de riesgos
3. Ir a `Herramientas > Editor de Scripts`
4. Hacer clic en el botón "Cargar" (📂)
5. Seleccionar el script deseado:
   - `defense_list_grouped.js` para el listado
   - `defense_importance_analysis.js` para el análisis de importancia
6. Hacer clic en "Ejecutar" (▶️)
7. Ver los resultados en la consola del editor

### Guardar resultados
Los resultados aparecen en la consola del editor de scripts. Para guardarlos:
1. Seleccionar todo el texto de la consola
2. Copiar (Ctrl+C)
3. Pegar en un editor de texto o documento
4. Guardar como archivo de texto (.txt) o documento

### Recomendaciones

1. **Ejecutar primero defense_list_grouped.js**
   - Proporciona una visión general del modelo
   - Ayuda a familiarizarse con las defensas presentes
   - Útil para identificar defensas de interés

2. **Luego ejecutar defense_importance_analysis.js**
   - Proporciona análisis cuantitativo detallado
   - Los resultados son más útiles si ya conoces las defensas del modelo
   - Puede tardar más en ejecutarse en modelos grandes

3. **Interpretar con contexto**
   - Las métricas son cuantitativas pero requieren juicio experto
   - Considerar el contexto operativo y normativo
   - Las estimaciones deben validarse para decisiones críticas

## Ejemplos de análisis

### Ejemplo 1: Identificar defensa más crítica

```javascript
// Después de ejecutar defense_importance_analysis.js
// La defensa con mayor RAW es la más crítica

TOP 1: BM-PRO-001 - Sistema de protección automático
   RAW = 1.7486  -> Si falla, el riesgo aumenta 74.86%
   RII = 1.3973  -> El riesgo aumentaría 1.40 niveles
   B   = 0.4629  -> Alta sensibilidad a cambios

Interpretación: Esta es la defensa más crítica. Su fallo
tendría el mayor impacto en el riesgo del sistema.
Requiere máxima atención y posiblemente redundancia.
```

### Ejemplo 2: Evaluar mejora de robustez

```javascript
// Para una defensa específica:
BM-PRO-002 - Alarma de alta presión
   RRI+ = 0.5200  -> Beneficio significativo
   B+   = 0.3100  -> Alta sensibilidad a robustez

Interpretación: Aumentar la robustez de esta defensa
reduciría el riesgo en 0.52 niveles. Es un buen
candidato para inversión en mejoras.
```

### Ejemplo 3: Comparar tipos de defensa

```javascript
// Del resumen por tipo:
Barrera: RAW promedio = 1.4567
Reductor de Frecuencia: RAW promedio = 1.2345
Reductor de Consecuencia: RAW promedio = 1.1234

Interpretación: Las barreras tienen en promedio
mayor impacto que los otros tipos. El modelo
depende más de las barreras para controlar el riesgo.
```

## Limitaciones conocidas

1. **Tiempo de ejecución**: El análisis modifica y recalcula el modelo múltiples veces por defensa, lo que puede tomar varios minutos en modelos grandes
2. **Defensas complejas**: Interacciones entre defensas no se consideran en el análisis individual
3. **Dependencias**: No se modelan dependencias entre defensas
4. **Causa común**: No se consideran fallos de causa común
5. **Modificación temporal**: Durante el análisis, el estado de las defensas se modifica temporalmente (se restaura al final)

Para análisis críticos, se recomienda:
- Ejecutar el script cuando no se esté trabajando activamente en el modelo
- Guardar una copia de seguridad antes de ejecutar
- Considerar análisis de sensibilidad adicionales
- Consultar con expertos en análisis de riesgo

## Preguntas frecuentes

**P: ¿Por qué dos defensas con el mismo nombre tienen diferentes valores?**
R: Si tienen diferente robustez, se consideran defensas diferentes y tendrán diferentes métricas de importancia.

**P: ¿Qué métrica es más importante?**
R: RAW (Risk Achievement Worth) es generalmente la métrica principal para priorización, pero todas las métricas proporcionan información valiosa desde diferentes perspectivas.

**P: ¿Los scripts modifican el modelo?**
R: No, estos scripts son de solo lectura. No modifican ningún dato del modelo.

**P: ¿Puedo exportar los resultados?**
R: Sí, copia el texto de la consola y pégalo en un documento. También puedes modificar los scripts para generar salida en formato CSV o JSON.

**P: ¿Funcionan con modelos grandes?**
R: Sí, pero el tiempo de ejecución aumenta con el número de defensas y eventos. Para modelos con cientos de defensas, puede tardar varios segundos.

## Referencias

1. NRC NUREG/CR-3385: "Measures of Risk Importance and Their Applications"
2. NRC ML12160A479: "An Overview of Risk Importance Measures"
3. ASME RA-S Standard: "Standard for Level 1/Large Early Release Frequency Probabilistic Risk Assessment"

## Historial de versiones

- **v1.0** (2024): Versión inicial
  - Script 1: Listado de defensas agrupadas
  - Script 2: Análisis de importancia con 9 métricas
