# Cambios Visuales en la UI - Asistente de Nuevo Modelo

## 1. Advertencia de Base de Datos Abierta

### Diálogo de Advertencia
```
┌──────────────────────────────────────────────────┐
│ ⚠ Base de datos abierta                          │
├──────────────────────────────────────────────────┤
│                                                  │
│  Hay una base de datos abierta actualmente.     │
│                                                  │
│  ¿Desea guardar los cambios antes de crear un   │
│  nuevo modelo?                                   │
│                                                  │
│              ┌─────────┐  ┌──────────┐           │
│              │ Guardar │  │ Descartar│           │
│              └─────────┘  └──────────┘           │
│                       ┌──────────┐               │
│                       │ Cancelar │               │
│                       └──────────┘               │
└──────────────────────────────────────────────────┘
```

## 2. Paso 2: Campo Obligatorio en Negrita

### ANTES:
```
Nombre del modelo:   [________________]
```

### DESPUÉS:
```
**Nombre del modelo:**   [________________]
```

## 3. Paso 3: Botones más Anchos

### ANTES:
```
┌─────────────┐  ┌──────────────┐
│Añadir Etapa │  │Eliminar Etapa│
└─────────────┘  └──────────────┘
```

### DESPUÉS:
```
┌──────────────────┐  ┌──────────────────┐  ┌───────┐  ┌───────┐
│  Añadir Etapa    │  │  Eliminar Etapa  │  │ Subir │  │ Bajar │
│   (150px min)    │  │   (150px min)    │  │(100px)│  │(100px)│
└──────────────────┘  └──────────────────┘  └───────┘  └───────┘
```

## 4. Diálogo Combinado para Etapas

### ANTES (3 diálogos):
1. Preguntar nombre
2. Preguntar código
3. Confirmar subetapas

### DESPUÉS (1 diálogo + confirmación):
```
┌────────────────────────────────────┐
│ Añadir Etapa                       │
├────────────────────────────────────┤
│  Nombre:       [________________]  │
│  Código:       [________________]  │
│  Descripción:  ┌──────────────┐   │
│                │              │   │
│                └──────────────┘   │
│         [Cancel]  [OK]             │
└────────────────────────────────────┘
```

## 5. Diálogo Combinado para Subetapas

### DESPUÉS:
```
┌────────────────────────────────────┐
│ Añadir Subetapa                    │
├────────────────────────────────────┤
│  Nombre:       [________________]  │
│  Descripción:  ┌──────────────┐   │
│                │              │   │
│                └──────────────┘   │
│         [Cancel]  [OK]             │
└────────────────────────────────────┘
```

## 6. Botón "Terminar" y Carga Automática

### DESPUÉS de creación exitosa:
```
┌─────────────────────────────────────┐
│  ¡Modelo creado exitosamente!      │
│  ██████████████████████ 100%       │
│                    ┌──────────┐    │
│                    │ Terminar │    │
│                    └──────────┘    │
│          ↓                         │
│   (Carga el modelo automáticamente)│
└─────────────────────────────────────┘
```
