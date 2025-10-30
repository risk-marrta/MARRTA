# Scripts de Análisis de Defensas - Referencia Rápida

## Script 1: defense_list_grouped.js
**Propósito**: Listado de defensas únicas agrupadas por tipo

### Entrada
- Modelo MARRTA cargado (.pdb)

### Salida
```
=== REDUCTORES DE FRECUENCIA ===
Total de defensas únicas: 5
1. Código: RF-OP-001
   Nombre: Procedimientos de operación
   Robustez: Normal (1)
   Aparece en 3 suceso(s): SI-001, SI-003, SI-007

=== BARRERAS ===
Total de defensas únicas: 12
...

=== RESUMEN ===
Total de defensas únicas: 25
```

### Cuándo usar
✓ Inventario completo de defensas  
✓ Identificar defensas repetidas  
✓ Ver cobertura de defensas por sucesos  
✓ Documentación del modelo  

---

## Script 2: defense_importance_analysis.js
**Propósito**: Análisis cuantitativo de importancia de defensas

### Entrada
- Modelo MARRTA cargado (.pdb)

### Salida
```
R (Riesgo medio): 1.8667

TOP 20 DEFENSAS MÁS IMPORTANTES:
1. BM-PRO-001 - Sistema de protección
   RAW = 1.7486 (si falla, riesgo +74%)
   F-V = 1.2857 (contribución al riesgo)
   B = 0.4629 (sensibilidad)
   ...

RESUMEN POR TIPO:
Barreras: RAW promedio = 1.4567
```

### Métricas clave

| Métrica | Fórmula | Interpretación | Valores importantes |
|---------|---------|----------------|---------------------|
| **RAW** | R0/R | Impacto si falla | > 1.5 = crítica |
| **F-V** | Ri/R | Contribución al riesgo | > 0.1 = significativa |
| **RRW** | R/R1 | Beneficio de tenerla | ≈ 1 = importante |
| **B** | (R0-R1)/R | Sensibilidad | alto = gran impacto |

### Cuándo usar
✓ Priorizar mantenimiento  
✓ Identificar defensas críticas  
✓ Decidir inversiones en mejoras  
✓ Análisis de sensibilidad  

---

## Comparación rápida

| Aspecto | Script 1 | Script 2 |
|---------|----------|----------|
| **Tipo** | Descriptivo | Cuantitativo |
| **Complejidad** | Simple | Avanzado |
| **Tiempo** | Rápido (<5s) | Medio (5-30s) |
| **Uso típico** | Inventario | Análisis de riesgo |
| **Audiencia** | General | Analistas de riesgo |
| **Salida** | Listas | Métricas numéricas |

---

## Flujo de trabajo recomendado

```
1. defense_list_grouped.js
   ↓
   Obtener visión general
   ↓
2. defense_importance_analysis.js
   ↓
   Identificar defensas críticas
   ↓
3. Análisis manual de defensas TOP
   ↓
4. Decisiones de mejora
```

---

## Interpretación rápida de RAW

| RAW | Importancia | Acción |
|-----|-------------|--------|
| > 2.0 | **Crítica** | Máxima prioridad, considerar redundancia |
| 1.5-2.0 | **Alta** | Alta prioridad, mantenimiento estricto |
| 1.2-1.5 | **Media** | Prioridad normal, seguimiento regular |
| < 1.2 | **Baja** | Revisar necesidad, optimizar recursos |

---

## Notas importantes

⚠️ **Los valores R0, R1, R-, R+ son estimaciones aproximadas**
- Para decisiones críticas, validar con cálculo manual
- Considerar el contexto operativo específico

✓ **Los scripts son de solo lectura**
- No modifican el modelo
- Pueden ejecutarse múltiples veces

📊 **Guardar resultados**
- Copiar de la consola a archivo de texto
- Útil para comparar diferentes versiones del modelo

---

## Contacto y soporte

Para más información, consultar:
- `DEFENSE_SCRIPTS_GUIDE.md` - Guía completa
- `README.md` - Documentación general de scripts
- Documentación oficial de MARRTA
