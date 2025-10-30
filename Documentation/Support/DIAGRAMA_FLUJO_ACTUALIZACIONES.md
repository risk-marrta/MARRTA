# Diagrama de Flujo - Sistema de Comprobación de Actualizaciones

## Flujo General

```
┌─────────────────────────────────────────────────────────────────┐
│                        Inicio de MARRTA                          │
└───────────────────────────┬─────────────────────────────────────┘
                            │
                            ▼
                  ┌─────────────────────┐
                  │  loadSettings()     │
                  │  en MainWindow      │
                  └──────────┬──────────┘
                            │
                            ▼
                  ┌─────────────────────┐
                  │  updateChecker->    │
                  │  autoCheckOnStartup │
                  └──────────┬──────────┘
                            │
                            ▼
              ┌─────────────────────────────┐
              │ ¿Han pasado 14 días desde   │
              │ la última comprobación?     │
              └─────┬──────────────┬────────┘
                 NO │              │ SÍ
                    │              │
                    ▼              ▼
            ┌─────────────┐  ┌─────────────────────────┐
            │  Continuar  │  │ Comprobación silenciosa │
            │   normal    │  │ (en segundo plano)      │
            └─────────────┘  └────────────┬────────────┘
                                          │
                                          ▼
                            ┌──────────────────────────┐
                            │ ¿Hay actualizaciones o   │
                            │ nuevos modelos?          │
                            └────┬──────────────┬──────┘
                              NO │              │ SÍ
                                 │              │
                                 ▼              ▼
                         ┌─────────────┐  ┌─────────────────┐
                         │ No mostrar  │  │ Mostrar diálogo │
                         │   nada      │  │ con información │
                         └─────────────┘  └─────────────────┘
```

## Flujo de Comprobación Manual de Versión

```
                    Usuario selecciona:
         "Ayuda → Comprobar actualizaciones..."
                            │
                            ▼
              ┌──────────────────────────────┐
              │ MainWindow::                 │
              │ on_actionComprobar          │
              │ Actualizaciones_triggered() │
              └──────────────┬───────────────┘
                            │
                            ▼
              ┌──────────────────────────────┐
              │ updateChecker->              │
              │ checkForUpdates(false)       │
              └──────────────┬───────────────┘
                            │
                            ▼
              ┌──────────────────────────────┐
              │ GET request a GitHub API:    │
              │ /repos/theotocopulitos/      │
              │ MARRTA/releases/latest       │
              └──────────────┬───────────────┘
                            │
                            ▼
              ┌──────────────────────────────┐
              │ ¿Respuesta exitosa?          │
              └───┬──────────────────┬───────┘
               NO │                  │ SÍ
                  │                  │
                  ▼                  ▼
      ┌──────────────────┐  ┌──────────────────────┐
      │ Mostrar error:   │  │ Parsear JSON:        │
      │ "No se pudo      │  │ - tag_name           │
      │  comprobar..."   │  │ - html_url           │
      └──────────────────┘  └──────────┬───────────┘
                                       │
                                       ▼
                            ┌────────────────────────┐
                            │ Comparar versión:      │
                            │ isNewerVersion()       │
                            └───┬──────────────┬─────┘
                             NO │              │ SÍ
                                │              │
                                ▼              ▼
                    ┌──────────────────┐  ┌──────────────────────┐
                    │ Mostrar:         │  │ Mostrar:             │
                    │ "Ya tienes la    │  │ "Nueva versión       │
                    │  última versión" │  │  disponible: X.Y.Z"  │
                    │                  │  │ Botón: Abrir página  │
                    └──────────────────┘  └──────────┬───────────┘
                                                     │
                                                     ▼
                                          ┌────────────────────┐
                                          │ ¿Usuario hace clic │
                                          │ en "Sí"?           │
                                          └───┬────────────┬───┘
                                           NO │            │ SÍ
                                              │            │
                                              ▼            ▼
                                      ┌──────────┐  ┌────────────────┐
                                      │ Cerrar   │  │ Abrir navegador│
                                      │ diálogo  │  │ en GitHub      │
                                      └──────────┘  └────────────────┘
```

## Flujo de Comprobación Manual de Modelos

```
                    Usuario selecciona:
         "Ayuda → Comprobar nuevos modelos..."
                            │
                            ▼
              ┌──────────────────────────────┐
              │ MainWindow::                 │
              │ on_actionComprobar          │
              │ NuevosModelos_triggered()   │
              └──────────────┬───────────────┘
                            │
                            ▼
              ┌──────────────────────────────┐
              │ updateChecker->              │
              │ checkForNewModels(false)     │
              └──────────────┬───────────────┘
                            │
                            ▼
              ┌──────────────────────────────┐
              │ GET request a GitHub API:    │
              │ /repos/theotocopulitos/      │
              │ marrta-modelos-de-riesgo/    │
              │ releases                     │
              └──────────────┬───────────────┘
                            │
                            ▼
              ┌──────────────────────────────┐
              │ ¿Respuesta exitosa?          │
              └───┬──────────────────┬───────┘
               NO │                  │ SÍ
                  │                  │
                  ▼                  ▼
      ┌──────────────────┐  ┌──────────────────────┐
      │ Mostrar error:   │  │ Parsear JSON array:  │
      │ "No se pudo      │  │ Para cada release:   │
      │  comprobar..."   │  │ - published_at       │
      └──────────────────┘  │ - name / tag_name    │
                            └──────────┬───────────┘
                                       │
                                       ▼
                            ┌────────────────────────┐
                            │ Filtrar releases:      │
                            │ published_at >         │
                            │ lastModelsCheck        │
                            └───┬──────────────┬─────┘
                          NO HAY│              │ HAY
                          NUEVOS│              │NUEVOS
                                │              │
                                ▼              ▼
                    ┌──────────────────┐  ┌──────────────────────┐
                    │ Mostrar:         │  │ Mostrar:             │
                    │ "Sin nuevos      │  │ "Hay N nuevos        │
                    │  modelos..."     │  │  modelos:"           │
                    │                  │  │ • Modelo 1           │
                    └──────────────────┘  │ • Modelo 2           │
                                          │ ...                  │
                                          │ Botón: Abrir página  │
                                          └──────────┬───────────┘
                                                     │
                                                     ▼
                                          ┌────────────────────┐
                                          │ ¿Usuario hace clic │
                                          │ en "Sí"?           │
                                          └───┬────────────┬───┘
                                           NO │            │ SÍ
                                              │            │
                                              ▼            ▼
                                      ┌──────────┐  ┌────────────────┐
                                      │ Cerrar   │  │ Abrir navegador│
                                      │ diálogo  │  │ en repositorio │
                                      └──────────┘  └────────────────┘
```

## Flujo de Comparación de Versiones

```
                        isNewerVersion(current, latest)
                                    │
                                    ▼
                  ┌─────────────────────────────────┐
                  │ parseVersion(current)           │
                  │ Resultado: [1, 4, 4]           │
                  └──────────────┬──────────────────┘
                                 │
                                 ▼
                  ┌─────────────────────────────────┐
                  │ parseVersion(latest)            │
                  │ Resultado: [1, 4, 5]           │
                  └──────────────┬──────────────────┘
                                 │
                                 ▼
                  ┌─────────────────────────────────┐
                  │ Comparar componente a           │
                  │ componente (mayor → menor):     │
                  │                                 │
                  │ i=0: latest[0] vs current[0]   │
                  │      1 vs 1 → Iguales          │
                  │                                 │
                  │ i=1: latest[1] vs current[1]   │
                  │      4 vs 4 → Iguales          │
                  │                                 │
                  │ i=2: latest[2] vs current[2]   │
                  │      5 vs 4 → latest es mayor  │
                  └──────────────┬──────────────────┘
                                 │
                                 ▼
                          ┌────────────┐
                          │ return true│
                          │ (hay nueva │
                          │  versión)  │
                          └────────────┘
```

## Flujo de parseVersion()

```
                    Input: "1.4.4 (2025-10-07)"
                                │
                                ▼
                    ┌───────────────────────┐
                    │ Buscar paréntesis '(' │
                    │ y extraer parte       │
                    │ anterior              │
                    └──────────┬────────────┘
                               │
                               ▼
                        Resultado: "1.4.4 "
                               │
                               ▼
                    ┌───────────────────────┐
                    │ Eliminar espacios     │
                    │ finales               │
                    └──────────┬────────────┘
                               │
                               ▼
                        Resultado: "1.4.4"
                               │
                               ▼
                    ┌───────────────────────┐
                    │ Eliminar prefijo 'v'  │
                    │ si existe             │
                    └──────────┬────────────┘
                               │
                               ▼
                    ┌───────────────────────┐
                    │ Split por '.'         │
                    │ parts = ["1","4","4"] │
                    └──────────┬────────────┘
                               │
                               ▼
                    ┌───────────────────────┐
                    │ Convertir a enteros:  │
                    │ components = [1,4,4]  │
                    └──────────┬────────────┘
                               │
                               ▼
                    ┌───────────────────────┐
                    │ Asegurar 3 componentes│
                    │ (rellenar con 0)      │
                    └──────────┬────────────┘
                               │
                               ▼
                      return [1, 4, 4]
```

## Persistencia de Configuración

```
                    marrta.ini
     ┌────────────────────────────────────┐
     │                                    │
     │ [General]                          │
     │ lastVersionCheck=2025-01-15T10:30  │
     │ lastModelsCheck=2025-01-15T10:31   │
     │                                    │
     └───────────┬────────────┬───────────┘
                 │            │
                 │ Read       │ Write
                 ▼            ▼
     ┌────────────────────────────────────┐
     │        UpdateChecker               │
     │                                    │
     │ daysSinceLastCheck():              │
     │   - Lee fecha de settings          │
     │   - Calcula días transcurridos     │
     │   - return días                    │
     │                                    │
     │ updateLastCheckDate():             │
     │   - Obtiene DateTime actual        │
     │   - Guarda en settings             │
     │   - Sincroniza archivo             │
     │                                    │
     └────────────────────────────────────┘
```

## Arquitectura de Clases

```
┌─────────────────────────────────────────────────────────┐
│                      MainWindow                          │
├─────────────────────────────────────────────────────────┤
│ - updateChecker: UpdateChecker*                         │
│ - qSettings: QSettings*                                 │
├─────────────────────────────────────────────────────────┤
│ + MainWindow(parent)                                    │
│ + loadSettings(settings)                                │
│ + on_actionComprobarActualizaciones_triggered()        │
│ + on_actionComprobarNuevosModelos_triggered()          │
└────────────────────┬────────────────────────────────────┘
                     │ tiene
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│                    UpdateChecker                         │
│                  : QObject                               │
├─────────────────────────────────────────────────────────┤
│ - networkManager: QNetworkAccessManager*                │
│ - silentVersionCheck: bool                              │
│ - silentModelsCheck: bool                               │
│ - currentVersion: QString                               │
│ - settings: QSettings*                                  │
├─────────────────────────────────────────────────────────┤
│ + checkForUpdates(silent: bool)                         │
│ + checkForNewModels(silent: bool)                       │
│ + shouldAutoCheck(settings: QSettings*): bool           │
│ + autoCheckOnStartup(settings: QSettings*)              │
│ - onVersionCheckFinished(reply: QNetworkReply*)         │
│ - onModelsCheckFinished(reply: QNetworkReply*)          │
│ - parseVersion(versionStr: QString): QList<int>         │
│ - isNewerVersion(current: QString, latest: QString)     │
│ - updateLastCheckDate(key: QString)                     │
│ - daysSinceLastCheck(key: QString): int                 │
├─────────────────────────────────────────────────────────┤
│ signals:                                                 │
│ • newVersionAvailable(version, url)                     │
│ • newModelsAvailable(count, url)                        │
│ • noUpdatesAvailable()                                  │
│ • noNewModels()                                         │
│ • checkError(message)                                   │
└─────────────────────────────────────────────────────────┘
```

## Interacción con GitHub API

```
     MARRTA                                  GitHub API
        │                                        │
        │  GET /repos/.../releases/latest        │
        │───────────────────────────────────────>│
        │                                        │
        │              JSON Response             │
        │<───────────────────────────────────────│
        │  {                                     │
        │    "tag_name": "v1.4.5",              │
        │    "name": "Version 1.4.5",           │
        │    "html_url": "https://...",         │
        │    "published_at": "2025-01-15..."    │
        │  }                                     │
        │                                        │
        │  GET /repos/.../releases               │
        │───────────────────────────────────────>│
        │                                        │
        │            JSON Array Response          │
        │<───────────────────────────────────────│
        │  [                                     │
        │    {                                   │
        │      "tag_name": "modelo-v2",         │
        │      "name": "Modelo Nuevo",          │
        │      "published_at": "2025-01-10..."  │
        │    },                                  │
        │    ...                                 │
        │  ]                                     │
        │                                        │
```

## Estados del Sistema

```
┌────────────────────────────────────────────────────────┐
│                   Estado Inicial                        │
│                                                         │
│ • updateChecker = nullptr                              │
│ • lastVersionCheck no existe en settings               │
│ • lastModelsCheck no existe en settings                │
└─────────────────────┬──────────────────────────────────┘
                      │ Primer inicio
                      ▼
┌────────────────────────────────────────────────────────┐
│              Después de Primera Comprobación            │
│                                                         │
│ • updateChecker inicializado                           │
│ • lastVersionCheck = fecha actual                      │
│ • lastModelsCheck = fecha actual                       │
└─────────────────────┬──────────────────────────────────┘
                      │ < 14 días
                      ▼
┌────────────────────────────────────────────────────────┐
│                  Estado En Espera                       │
│                                                         │
│ • No se realizan comprobaciones automáticas            │
│ • Usuario puede hacer comprobaciones manuales          │
└─────────────────────┬──────────────────────────────────┘
                      │ >= 14 días
                      ▼
┌────────────────────────────────────────────────────────┐
│              Estado Listo para Comprobar                │
│                                                         │
│ • En próximo inicio se comprueban actualizaciones      │
│ • Comprobaciones manuales siempre disponibles          │
└────────────────────────────────────────────────────────┘
```

---

**Leyenda:**
- `┌─┐ └─┘` = Contenedores/Clases
- `│` = Conexiones verticales
- `►` `▼` = Flujo de ejecución
- `•` = Items de lista
- `-` = Miembros privados
- `+` = Métodos públicos
