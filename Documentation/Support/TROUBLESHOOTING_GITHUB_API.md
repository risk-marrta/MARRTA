# Solución de Problemas - API de GitHub

## Error: "Error transferring ... - server replied: "

Este error indica que la conexión con la API de GitHub está fallando. Aquí hay varias causas posibles y sus soluciones:

## Causas Comunes

### 1. Falta de Encabezados Requeridos

**Problema:** GitHub API v3 requiere el encabezado `Accept` específico.

**Solución:** ✅ Ya implementada en el código
```cpp
request.setRawHeader("Accept", "application/vnd.github.v3+json");
```

### 2. Problemas de SSL/TLS

**Problema:** Certificados SSL no válidos o problemas con la cadena de certificados.

**Síntomas:**
- Error "SSL handshake failed"
- Error "Error transferring" sin más detalles

**Diagnóstico:**
Ejecutar MARRTA con modo verbose para ver errores SSL:
```bash
./marrta --verboso
```

Buscar en la salida:
```
SSL errors encountered:
  - [detalle del error]
```

**Posibles causas SSL:**
1. Certificados del sistema desactualizados
2. Proxy corporativo interceptando HTTPS
3. Antivirus escaneando tráfico HTTPS
4. Firewall bloqueando certificados

**Soluciones:**
```bash
# Linux: Actualizar certificados
sudo update-ca-certificates

# Verificar que OpenSSL funciona
openssl s_client -connect api.github.com:443
```

### 3. Firewall o Proxy Corporativo

**Problema:** Red corporativa bloqueando acceso a api.github.com

**Diagnóstico:**
```bash
# Probar conectividad
curl -v https://api.github.com/repos/theotocopulitos/MARRTA/releases/latest

# Verificar si hay proxy configurado
echo $http_proxy
echo $https_proxy
```

**Soluciones:**

**A) Configurar proxy en Qt:**
Agregar al inicio de main.cpp, antes de crear MainWindow:
```cpp
QNetworkProxy proxy;
proxy.setType(QNetworkProxy::HttpProxy);
proxy.setHostName("proxy.empresa.com");
proxy.setPort(8080);
// Si requiere autenticación:
// proxy.setUser("usuario");
// proxy.setPassword("contraseña");
QNetworkProxy::setApplicationProxy(proxy);
```

**B) Variables de entorno:**
```bash
export http_proxy=http://proxy.empresa.com:8080
export https_proxy=http://proxy.empresa.com:8080
export no_proxy=localhost,127.0.0.1
./marrta
```

**C) Solicitar al administrador de red:**
- Permitir tráfico HTTPS a api.github.com (IP: 140.82.114.6)
- Puerto 443
- Agregar a whitelist de firewall

### 4. Límite de Tasa de GitHub (Rate Limit)

**Problema:** Demasiadas peticiones a la API de GitHub.

**Síntomas:**
- HTTP Status Code: 403
- Mensaje: "API rate limit exceeded"

**Solución:**
Esperar 1 hora. GitHub permite 60 peticiones/hora sin autenticación.

Para aumentar el límite, usar token de acceso personal:
```cpp
// En updatechecker.cpp, en checkForUpdates():
request.setRawHeader("Authorization", "token TU_TOKEN_AQUI");
```

Generar token en: https://github.com/settings/tokens

### 5. Respuesta Vacía del Servidor

**Problema:** Servidor responde pero sin contenido.

**Diagnóstico mejorado:**
Con los cambios implementados, ahora verás en el log:
```
Error checking for updates: [error]
HTTP Status Code: [código]
Response: [respuesta del servidor]
```

**Interpretación de códigos HTTP:**
- **200**: OK (sin error)
- **301/302**: Redirección (raro en API)
- **403**: Prohibido (rate limit o permisos)
- **404**: No encontrado (repo o release no existe)
- **500**: Error del servidor de GitHub
- **0**: Sin conexión o bloqueado por firewall

## Verificación Paso a Paso

### 1. Verificar Conectividad Básica

```bash
# ¿Puedes llegar a GitHub?
ping github.com

# ¿Puedes hacer HTTPS?
curl -I https://github.com

# ¿Funciona la API?
curl -H "Accept: application/vnd.github.v3+json" \
     -H "User-Agent: MARRTA" \
     https://api.github.com/repos/theotocopulitos/MARRTA/releases/latest
```

Si alguno falla, hay un problema de red.

### 2. Verificar SSL

```bash
# ¿El certificado es válido?
openssl s_client -connect api.github.com:443 -showcerts

# Buscar:
# Verify return code: 0 (ok)  <- Debe decir 0
```

Si dice otro número, hay problema con certificados.

### 3. Verificar Variables de Entorno

```bash
env | grep -i proxy
env | grep -i ssl
```

### 4. Ejecutar MARRTA con Debug

```bash
./marrta --verboso 2>&1 | tee marrta.log
```

Luego revisar `marrta.log` para ver detalles completos.

## Soluciones Implementadas en el Código

### ✅ Cambio 1: Encabezado Accept
```cpp
request.setRawHeader("Accept", "application/vnd.github.v3+json");
```

### ✅ Cambio 2: Logging Mejorado
```cpp
qWarning() << "Error checking for updates:" << errorDetails;
qWarning() << "HTTP Status Code:" << httpStatus;
qWarning() << "Response:" << responseData;
```

### ✅ Cambio 3: Manejo de Errores SSL
```cpp
connect(networkManager, &QNetworkAccessManager::sslErrors, ...);
```

## Solución Temporal para Testing

**SOLO PARA DESARROLLO - NO EN PRODUCCIÓN**

Si necesitas probar rápidamente y estás en una red problemática:

1. Edita `updatechecker.cpp`
2. En el handler de sslErrors, descomenta:
   ```cpp
   reply->ignoreSslErrors();  // ⚠️ INSEGURO - Solo para testing
   ```
3. Recompila
4. Prueba
5. **IMPORTANTE:** Vuelve a comentar esa línea antes de distribuir

## Alternativas

### Opción 1: Usar HTTP en lugar de HTTPS (NO RECOMENDADO)

Solo para diagnóstico:
```cpp
QUrl url("http://api.github.com/...");  // Sin 's' en http
```

⚠️ **Problema:** GitHub redirige a HTTPS de todos modos.

### Opción 2: Mirror Local

Para redes muy restrictivas, podrías:
1. Crear un script que consulte GitHub API externamente
2. Publicar resultados en un servidor interno
3. Modificar URLs en updatechecker.cpp para apuntar a servidor interno

### Opción 3: Comprobación Manual

Si todo lo demás falla:
1. Desactivar comprobaciones automáticas
2. Avisar al usuario que visite manualmente GitHub
3. Agregar enlace directo en menú Ayuda

## Información para Reportar Issues

Si el problema persiste, recopilar:

```bash
# 1. Información del sistema
uname -a
ldd ./marrta | grep ssl

# 2. Log completo
./marrta --verboso 2>&1 > debug.log

# 3. Test de conectividad
curl -v -H "Accept: application/vnd.github.v3+json" \
     https://api.github.com/repos/theotocopulitos/MARRTA/releases/latest \
     > curl_test.txt 2>&1

# 4. Variables de red
env | grep -i proxy > env.txt
```

Adjuntar estos archivos al issue en GitHub.

## Preguntas Frecuentes

**P: ¿Por qué funcionaba curl pero no MARRTA?**

R: Curl y Qt pueden usar diferentes librerías SSL y configuraciones de proxy.

**P: ¿Es seguro ignorar errores SSL?**

R: No. Solo hazlo temporalmente para diagnóstico en entorno de desarrollo.

**P: ¿Necesito autenticación con GitHub?**

R: No para consultas básicas. El límite de 60/hora es suficiente.

**P: ¿El firewall corporativo puede ser el problema?**

R: Sí, es muy común. Muchas empresas bloquean o interceptan HTTPS.

## Contacto

Si ninguna solución funciona, abre un issue en GitHub con:
- Sistema operativo y versión
- Versión de Qt
- Output de `./marrta --verboso`
- Resultado de los tests de conectividad
- Configuración de red (proxy, firewall)
