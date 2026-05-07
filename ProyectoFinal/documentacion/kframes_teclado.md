# Sistema de Animación por Keyframes con Captura por Teclado

## Resumen Ejecutivo

Este documento describe la implementación de un sistema modular de animación por keyframes para el modelo Halo Pelican, que permite capturar posiciones y rotaciones mediante controles de teclado, calcular frames intermedios automáticamente, y reproducir animaciones en loop de 60 segundos a 12 FPS.

---

## 1. Arquitectura del Sistema

### 1.1 Componentes Principales

El sistema está compuesto por los siguientes módulos:

- **`Keyframe_System`**: Clase principal que gestiona todo el sistema de animación
- **`Keyframe_Parser`**: Clase interna para leer archivos `.kf`
- **`Keyframe_Pretty_Printer`**: Clase interna para escribir archivos `.kf` en formato legible
- **`Keyframe` (struct)**: Estructura de datos que almacena transformaciones y metadatos

### 1.2 Archivos del Sistema

```
ProyectoFinal/Integrantes/Isra/
├── animacion_KF.h              # Declaraciones de clases y estructuras
├── animacion_KF.cpp            # Implementación del sistema
├── halo_pelican.h              # Modelo del Pelican (modificado para 3 ejes de rotación)
├── halo_pelican.cpp            # Implementación del modelo
├── pelican_halo_runtime.kf     # Archivo de keyframes guardados (generado en runtime)
└── camara_position.h/cpp       # Sistema de cámara (integrado)
```

---

## 2. Estructura de Datos

### 2.1 Estructura Keyframe

```cpp
struct Keyframe {
    // Transformación capturada
    float posX, posY, posZ;           // Posición en espacio 3D
    float rotX, rotY, rotZ;           // Rotación en 3 ejes (pitch, yaw, roll)
    float timestamp;                  // Tiempo de captura (segundos desde inicio)
    
    // Incrementos calculados para interpolación
    float posXInc, posYInc, posZInc;
    float rotXInc, rotYInc, rotZInc;
};
```

### 2.2 Variables de Estado

```cpp
// Almacenamiento
Keyframe keyframes[MAX_FRAMES];       // Array de 100 keyframes máximo
int frameCount;                       // Keyframes capturados
int framesPerSegment[MAX_FRAMES];     // Frames calculados por segmento

// Flags de modo
bool recordingMode;                   // Modo captura activo
bool playbackMode;                    // Modo reproducción activo
bool framesCalculated;                // Frames intermedios calculados

// Timing
float frameRate;                      // 12 FPS
float recordingStartTime;             // Tiempo inicio de captura
float totalRecordingTime;             // Duración total de captura
```

---

## 3. Modo de Captura de Keyframes

### 3.1 Activación del Modo Captura

**Tecla**: `K`

**Proceso al activar**:
1. Limpia todos los keyframes anteriores
2. Resetea contadores y flags
3. Desactiva el `CameraPositionTracker`
4. Fija la cámara en posición estática: `(-198.46, 37.29, -94.11)`
5. Inicia el timer de captura
6. Muestra menú de controles

```cpp
void Keyframe_System::toggleRecordingMode() {
    if (recordingMode) {
        // Limpiar keyframes previos
        frameCount = 0;
        framesCalculated = false;
        
        // Iniciar timer
        recordingStartTime = glfwGetTime();
        
        // Deshabilitar tracking de cámara
        cameraTracker->SetEnabled(false);
    }
}
```

### 3.2 Controles de Vuelo

El sistema implementa controles direccionales jerárquicos basados en la rotación Y del modelo:

| Tecla | Función | Implementación |
|-------|---------|----------------|
| **UP Arrow** | Avanzar hacia adelante | `position += forward * moveSpeed` |
| **DOWN Arrow** | Retroceder | `position -= forward * moveSpeed` |
| **RIGHT Arrow** | Girar derecha | `rotationY += rotationSpeed` |
| **LEFT Arrow** | Girar izquierda | `rotationY -= rotationSpeed` |
| **N** | Subir (Y+) | `position.y += verticalSpeed` |
| **M** | Bajar (Y-) | `position.y -= verticalSpeed` |

**Cálculo de vectores direccionales**:
```cpp
float yawRadians = modelRotY * (PI / 180.0f);
float forwardX = sin(yawRadians);
float forwardZ = cos(yawRadians);
```

### 3.3 Captura de Keyframes

**Proceso de guardado**:
1. Presionar `P` → Habilita el guardado del siguiente keyframe
2. Presionar `L` → Captura el keyframe actual

**Datos capturados**:
```cpp
keyframes[frameCount].posX = modelPosition.x;
keyframes[frameCount].posY = modelPosition.y;
keyframes[frameCount].posZ = modelPosition.z;
keyframes[frameCount].rotX = modelRotX;
keyframes[frameCount].rotY = modelRotY;
keyframes[frameCount].rotZ = modelRotZ;
keyframes[frameCount].timestamp = glfwGetTime() - recordingStartTime;
```

### 3.4 Salida del Modo Captura

Al presionar `K` nuevamente:
1. Calcula la duración total de captura
2. Reactiva el `CameraPositionTracker`
3. Muestra resumen (keyframes capturados, duración)
4. Marca que los frames necesitan ser recalculados

---

## 4. Sistema de Cálculo de Frames Intermedios

### 4.1 Algoritmo de Distribución por Porcentajes

El sistema utiliza un algoritmo basado en porcentajes para distribuir keyframes proporcionalmente en 60 segundos:

```cpp
void Keyframe_System::calculateAllFrames() {
    const float TARGET_DURATION = 60.0f;
    const float TOTAL_FRAMES = 720.0f;  // 60s * 12 FPS
    
    // Calcular porcentaje de cada keyframe
    for (int i = 0; i < frameCount; i++) {
        float percentage = keyframes[i].timestamp / totalRecordingTime;
        targetFrames[i] = (int)(percentage * TOTAL_FRAMES);
    }
    
    // Calcular frames entre keyframes consecutivos
    for (int i = 0; i < frameCount - 1; i++) {
        int framesInSegment = targetFrames[i+1] - targetFrames[i];
        framesPerSegment[i] = (framesInSegment < 1) ? 1 : framesInSegment;
        
        // Calcular incrementos para interpolación lineal
        keyframes[i].posXInc = (keyframes[i+1].posX - keyframes[i].posX) / framesInSegment;
        // ... (similar para Y, Z, rotaciones)
    }
}
```

**Ejemplo**:
- Captura: 40 keyframes en 200 segundos
- Keyframe 20 está en segundo 100 (50% de la captura)
- Se mapea a frame 360 (50% de 720 frames = segundo 30)

### 4.2 Interpolación Lineal

Entre cada par de keyframes se calcula una interpolación lineal:

```
increment = (valorFinal - valorInicial) / numeroDeFrames
```

Cada frame de animación aplica estos incrementos:
```cpp
currentPosition.x += keyframes[currentPlayIndex].posXInc;
currentPosition.y += keyframes[currentPlayIndex].posYInc;
// ... (similar para Z y rotaciones)
```

### 4.3 Loop Seamless

El último keyframe se conecta con el primero para crear un loop continuo:

```cpp
int framesInSegment = TOTAL_FRAMES - targetFrames[frameCount - 1];
keyframes[frameCount-1].posXInc = (keyframes[0].posX - keyframes[frameCount-1].posX) / framesInSegment;
```

---

## 5. Sistema de Reproducción Automática

### 5.1 Inicio Automático al Cargar

Cuando el programa inicia:

```cpp
bool Keyframe_System::loadKeyframesFromFile(const string& filename) {
    // 1. Parsear archivo
    parser.parseFile(filename, keyframes, loadedCount, maxFrames);
    
    // 2. Calcular frames intermedios
    calculateAllFrames();
    
    // 3. Activar playback automáticamente
    playbackMode = true;
    currentPlayIndex = 0;
    currentInterpolationStep = 0;
    
    // 4. Establecer posición inicial
    currentPosition = keyframes[0].position;
    currentRotation = keyframes[0].rotation;
}
```

### 5.2 Timer Independiente

El sistema usa un timer independiente basado en `glfwGetTime()` para mantener 12 FPS constantes independientemente del framerate de renderizado (60 FPS):

```cpp
void Keyframe_System::updatePlayback(float deltaTime) {
    static double lastUpdateTime = 0.0;
    double currentTime = glfwGetTime();
    double realDeltaTime = currentTime - lastUpdateTime;
    
    animationTimer += realDeltaTime;
    
    float frameTime = 1.0f / 12.0f;  // 0.0833 segundos
    if (animationTimer >= frameTime) {
        animationTimer -= frameTime;
        
        // Aplicar incrementos
        currentPosition += increments;
        currentInterpolationStep++;
        
        // Avanzar al siguiente keyframe si es necesario
        if (currentInterpolationStep >= framesPerSegment[currentPlayIndex]) {
            currentPlayIndex++;
            currentInterpolationStep = 0;
            
            // Loop back
            if (currentPlayIndex >= frameCount) {
                currentPlayIndex = 0;
            }
        }
    }
}
```

### 5.3 Control Manual de Reproducción

| Tecla | Función |
|-------|---------|
| **SPACE** | Iniciar/Pausar reproducción |
| **0** | Resetear a primer keyframe |

---

## 6. Sistema de Persistencia

### 6.1 Formato de Archivo `.kf`

Formato de texto legible para humanos:

```
# Animation: pelican_halo_runtime
# Created: 2026-05-07 03:40:26
# Total Keyframes: 41

KEYFRAME 0
posX=-49.89 posY=10.77 posZ=40.78 rotX=0.92 rotY=91.83 rotZ=0.00 timestamp=8.17

KEYFRAME 1
posX=-28.30 posY=15.17 posZ=40.09 rotX=0.92 rotY=91.83 rotZ=0.00 timestamp=11.90
```

### 6.2 Guardado Automático

**Tecla**: `G` (cuando no está en modo captura)

```cpp
if (keys[GLFW_KEY_G] && !animSystem.isRecording() && animSystem.getKeyframeCount() > 0) {
    animSystem.saveKeyframesToFile("Integrantes/Isra/pelican_halo_runtime.kf");
}
```

### 6.3 Compatibilidad con Archivos Antiguos

El parser detecta archivos sin timestamps y asigna timestamps distribuidos uniformemente:

```cpp
if (!hasTimestamps && outFrameCount > 1) {
    float timeStep = 60.0f / (outFrameCount - 1);
    for (int i = 0; i < outFrameCount; i++) {
        keyframes[i].timestamp = i * timeStep;
    }
}
```

---

## 7. Integración con el Modelo Halo Pelican

### 7.1 Modificaciones al Modelo

Se agregaron variables públicas para transformación:

```cpp
class Halo_Pelican {
public:
    glm::vec3 position;
    float rotationX, rotationY, rotationZ;
    
    void Draw(/* ... */);
};
```

### 7.2 Aplicación de Transformaciones

```cpp
void Keyframe_System::applyTransformationToModel(
    glm::vec3& position, 
    float& rotX, float& rotY, float& rotZ
) {
    if (playbackMode) {
        position = currentPosition;
        rotX = currentRotX;
        rotY = currentRotY;
        rotZ = currentRotZ;
    }
}
```

### 7.3 Integración en Main Loop

```cpp
// Update playback animation
animSystem.updatePlayback(deltaTime);
animSystem.applyTransformationToModel(
    haloPelican.position, 
    haloPelican.rotationX,
    haloPelican.rotationY, 
    haloPelican.rotationZ
);

// Render model
haloPelican.Draw(/* ... */);
```

---

## 8. Características Técnicas

### 8.1 Especificaciones

| Característica | Valor |
|----------------|-------|
| **Capacidad máxima** | 100 keyframes |
| **Frame rate de animación** | 12 FPS |
| **Frame rate de renderizado** | 60 FPS |
| **Duración del loop** | 60 segundos exactos |
| **Total de frames calculados** | 720 frames |
| **Método de interpolación** | Lineal |
| **Formato de archivo** | Texto plano (.kf) |

### 8.2 Ventajas del Sistema

1. **Modular**: Sistema completamente externo al modelo
2. **Reutilizable**: Puede aplicarse a cualquier modelo con transformaciones similares
3. **Persistente**: Guarda y carga animaciones automáticamente
4. **Preciso**: Distribución proporcional garantiza duración exacta
5. **Independiente**: Timer propio no afecta el framerate del programa
6. **Intuitivo**: Controles de vuelo naturales basados en orientación

### 8.3 Limitaciones

1. Solo interpola linealmente (no hay curvas suaves)
2. Máximo 100 keyframes por animación
3. No soporta animación de escala
4. Un solo archivo de animación activo a la vez

---

## 9. Flujo de Trabajo Completo

### 9.1 Primera Ejecución (Sin Animación Guardada)

```
1. Programa inicia
   └─> Intenta cargar pelican_halo_runtime.kf
       └─> No existe → Muestra warning
       
2. Usuario presiona 'K'
   └─> Entra en modo captura
   └─> Limpia keyframes previos
   └─> Fija cámara en posición estática
   
3. Usuario vuela el Pelican con flechas/N/M
   └─> Presiona 'P' para habilitar guardado
   └─> Presiona 'L' para capturar keyframe
   └─> Repite para múltiples keyframes
   
4. Usuario presiona 'K' para salir
   └─> Calcula duración total
   └─> Muestra resumen
   
5. Usuario presiona 'SPACE'
   └─> Calcula frames intermedios (distribución por porcentajes)
   └─> Inicia reproducción automática a 12 FPS
   └─> Loop infinito de 60 segundos
   
6. Usuario presiona 'G'
   └─> Guarda en pelican_halo_runtime.kf
```

### 9.2 Ejecuciones Posteriores (Con Animación Guardada)

```
1. Programa inicia
   └─> Carga pelican_halo_runtime.kf
   └─> Calcula frames intermedios automáticamente
   └─> Inicia reproducción automática
   └─> Loop infinito de 60 segundos
   
2. Usuario puede:
   ├─> Presionar 'SPACE' para pausar/reanudar
   ├─> Presionar '0' para resetear
   └─> Presionar 'K' para capturar nueva animación
       └─> Limpia animación anterior
       └─> Inicia nueva captura desde cero
```

---

## 10. Mensajes del Sistema

### 10.1 Modo Captura

```
========================================
INICIANDO NUEVA CAPTURA
Limpiando keyframes anteriores...
========================================

========================================
=== MODO CAPTURA DE KEYFRAMES ===
========================================
CONTROLES DE VUELO:
  UP Arrow    : Avanzar hacia adelante
  DOWN Arrow  : Retroceder
  RIGHT Arrow : Girar a la derecha
  LEFT Arrow  : Girar a la izquierda
  N           : Subir (Y+)
  M           : Bajar (Y-)

GUARDAR KEYFRAMES:
  P : Habilitar guardado
  L : Guardar keyframe actual

SALIR:
  K : Salir del modo captura
========================================
Keyframes guardados: 0/100
========================================
```

### 10.2 Cálculo de Frames

```
========================================
CALCULANDO FRAMES INTERMEDIOS...
Por favor espera...
========================================

=== DISTRIBUCION BASADA EN PORCENTAJES ===
Keyframe 0: 0.00% -> Frame 0 (segundo 0.00)
Keyframe 1: 5.35% -> Frame 38 (segundo 3.17)
...

=== CALCULO DE FRAMES POR SEGMENTO ===
Segmento 0 -> 1: 38 frames (3.17s)
Segmento 1 -> 2: 25 frames (2.08s)
...

=== RESUMEN ===
Total frames calculados: 720 / 720
Duracion total: 60.00 segundos

========================================
CALCULO COMPLETADO!
Iniciando animacion automaticamente...
========================================
```

---

## 11. Conclusión

El sistema de animación por keyframes implementado proporciona una solución completa y robusta para capturar, calcular y reproducir animaciones del modelo Halo Pelican. La arquitectura modular permite fácil extensión y reutilización, mientras que el algoritmo de distribución por porcentajes garantiza precisión temporal independientemente de la cantidad de keyframes capturados.

La integración con el sistema de cámara y la persistencia automática hacen que el flujo de trabajo sea intuitivo y eficiente, permitiendo iteraciones rápidas en el diseño de animaciones complejas.

---

**Autor**: Sistema implementado para el proyecto de Computación Gráfica  
**Fecha**: Mayo 2026  
**Versión**: 1.0
