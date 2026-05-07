# Sistema de Animación por Keyframes - Halo Pelican

## Descripción General

Este sistema modular permite crear animaciones por keyframes para modelos 3D en OpenGL. Soporta dos métodos de entrada:
- **Método A**: Cargar keyframes desde archivos de texto
- **Método B**: Grabar keyframes interactivamente durante la ejecución

## Archivos Implementados

### Archivos Principales
- `animacion_KF.h` - Interfaz del sistema de keyframes
- `animacion_KF.cpp` - Implementación completa del sistema
- `animKFcamara.h` - Interfaz de la cámara de animación
- `animKFcamara.cpp` - Implementación de la cámara que sigue al modelo
- `halo_pelican.h` - Modelo modificado con rotación en 3 ejes (actualizado)
- `halo_pelican.cpp` - Implementación del modelo (actualizado)

### Archivos de Datos
- `pelican_halo_code.kf` - Archivo de ejemplo con 7 keyframes predefinidos
- `pelican_halo_runtime.kf` - Archivo generado al grabar keyframes en tiempo real

## Características Implementadas

### ✅ Modificaciones a HaloPelican
- Variables públicas de transformación: `position`, `rotationX`, `rotationY`, `rotationZ`
- Soporte completo para rotación en 3 ejes (pitch, yaw, roll)
- Estado inicial: posición (-58.58, 1.15, 35.59), rotación Y = 80°

### ✅ Sistema de Keyframes
- Estructura de datos para almacenar hasta 100 keyframes
- Interpolación lineal suave entre keyframes
- Animación en loop de 60 segundos a 24 FPS
- Gestión de estado (grabación vs reproducción)

### ✅ Sistema de Archivos I/O
- **Parser**: Lee archivos .kf en formato texto legible
- **Pretty Printer**: Escribe archivos con formato consistente
- Manejo robusto de errores (archivos faltantes, datos corruptos)
- Formato de archivo simple y editable manualmente

### ✅ Modo de Grabación
- Activar/desactivar con tecla **K**
- Controles de movimiento:
  - **Flechas**: Mover modelo (arriba/abajo/izquierda/derecha)
  - **Mouse**: Rotar modelo (yaw y pitch)
- Controles de keyframes:
  - **L**: Guardar keyframe actual
  - **P**: Habilitar guardado del siguiente keyframe
- Menú de ayuda en consola

### ✅ Modo de Reproducción
- Activar/desactivar con **BARRA ESPACIADORA**
- Interpolación automática entre keyframes
- Loop infinito de 60 segundos
- Validación: requiere mínimo 2 keyframes

### ✅ Cámara de Animación
- Sigue automáticamente al modelo durante grabación
- Distancia y altura configurables
- Transición suave entre cámara normal y cámara de seguimiento

### ✅ Controles Generales
- **0**: Resetear animación al primer keyframe
- Exclusión mutua entre modos (no se puede grabar y reproducir simultáneamente)

## Formato de Archivo de Keyframes

```
# Animation: nombre_animacion
# Created: 2026-05-07 14:30:22
# Total Keyframes: 7

KEYFRAME 0
posX=-58.58 posY=1.15 posZ=35.59 rotX=0.00 rotY=80.00 rotZ=0.00

KEYFRAME 1
posX=-55.00 posY=3.00 posZ=40.00 rotX=0.00 rotY=90.00 rotZ=0.00

...
```

### Especificaciones del Formato
- Líneas que empiezan con `#` son comentarios
- `KEYFRAME N` indica el inicio del keyframe N
- Siguiente línea contiene los 6 valores de transformación
- Valores flotantes con 2 decimales de precisión
- Posiciones en unidades del mundo
- Rotaciones en grados

## Integración con Aplicación Principal

### Paso 1: Incluir Headers
```cpp
#include "Integrantes/Isra/animacion_KF.h"
#include "Integrantes/Isra/animKFcamara.h"
#include "Integrantes/Isra/halo_pelican.h"
```

### Paso 2: Crear Instancias Globales
```cpp
HaloPelican pelican;
Keyframe_System animSystem(100);  // Capacidad de 100 keyframes
Animation_Camera animCamera;
Camera mainCamera;  // Cámara principal existente
```

### Paso 3: Inicialización
```cpp
// En la función de inicialización
pelican.Initialize();
animSystem.loadKeyframesFromFile("Integrantes/Isra/pelican_halo_code.kf");
```

### Paso 4: Loop Principal - Manejo de Input
```cpp
// Detectar teclas (una vez por presión)
static bool kKeyPressed = false;
static bool spaceKeyPressed = false;
static bool zeroKeyPressed = false;

if (keys[GLFW_KEY_K] && !kKeyPressed) {
    kKeyPressed = true;
    animSystem.toggleRecordingMode();
    
    // Guardar al salir del modo de grabación
    if (!animSystem.isRecording()) {
        animSystem.saveKeyframesToFile("Integrantes/Isra/pelican_halo_runtime.kf");
    }
} else if (!keys[GLFW_KEY_K]) {
    kKeyPressed = false;
}

if (keys[GLFW_KEY_SPACE] && !spaceKeyPressed) {
    spaceKeyPressed = true;
    animSystem.togglePlaybackMode();
} else if (!keys[GLFW_KEY_SPACE]) {
    spaceKeyPressed = false;
}

if (keys[GLFW_KEY_0] && !zeroKeyPressed) {
    zeroKeyPressed = true;
    animSystem.resetAnimation();
} else if (!keys[GLFW_KEY_0]) {
    zeroKeyPressed = false;
}

// Manejo de input en modo grabación
animSystem.handleRecordingInput(keys, mouseX, mouseY, deltaTime);
```

### Paso 5: Loop Principal - Actualización
```cpp
// Actualizar reproducción
animSystem.updatePlayback(deltaTime);

// Aplicar transformación al modelo
animSystem.applyTransformationToModel(
    pelican.position,
    pelican.rotationX,
    pelican.rotationY,
    pelican.rotationZ
);

// Gestión de cámara
glm::mat4 viewMatrix;
if (animSystem.isRecording()) {
    // Usar cámara de animación durante grabación
    animCamera.enableFollowMode(true);
    animCamera.setFollowTarget(pelican.position);
    animCamera.update(deltaTime);
    viewMatrix = animCamera.getViewMatrix();
} else {
    // Usar cámara principal
    animCamera.enableFollowMode(false);
    mainCamera.keyControl(keys, deltaTime);
    mainCamera.mouseControl(mouseX, mouseY);
    viewMatrix = mainCamera.calculateViewMatrix();
}

// Renderizar modelo
pelican.Render(uniformModel, uniformColor, 
               uniformSpecularIntensity, uniformShininess, 
               toRadians);
```

## Flujo de Trabajo Típico

### Crear Nueva Animación (Método B - Interactivo)
1. Ejecutar aplicación
2. Presionar **K** para entrar en modo de grabación
3. Usar **flechas** y **mouse** para posicionar el modelo
4. Presionar **L** para guardar el keyframe actual
5. Presionar **P** para habilitar guardar el siguiente keyframe
6. Repetir pasos 3-5 para cada keyframe deseado
7. Presionar **K** para salir (guarda automáticamente a `pelican_halo_runtime.kf`)
8. Presionar **BARRA ESPACIADORA** para reproducir la animación

### Usar Animación Existente (Método A - Archivo)
1. Editar o crear archivo `.kf` con keyframes deseados
2. Ejecutar aplicación (carga automáticamente `pelican_halo_code.kf`)
3. Presionar **BARRA ESPACIADORA** para reproducir
4. Presionar **0** para resetear al inicio

## Requisitos del Sistema

- C++11 o superior
- OpenGL 3.3+
- GLFW 3.x
- GLM (OpenGL Mathematics)
- GLEW (OpenGL Extension Wrangler)

## Notas Técnicas

### Rendimiento
- Objetivo: 24 FPS para animación
- Interpolación pre-calculada al iniciar reproducción
- Sin asignación dinámica de memoria durante reproducción
- Cálculos de interpolación < 1ms por frame

### Limitaciones
- Máximo 100 keyframes por animación
- Solo interpolación lineal (sin curvas de aceleración)
- Animación de 60 segundos fija
- No soporta animación de escala

### Extensibilidad
El sistema está diseñado para ser modular y reutilizable:
- Puede aplicarse a cualquier modelo con variables públicas de transformación
- Fácil de extender para soportar más propiedades animables
- Formato de archivo simple permite edición manual

## Solución de Problemas

### "Need at least 2 keyframes to play animation"
- Asegúrate de tener al menos 2 keyframes guardados
- Verifica que el archivo .kf existe y contiene datos válidos

### "Maximum keyframe capacity (100) reached"
- Has alcanzado el límite de 100 keyframes
- Considera reducir el número de keyframes o aumentar MAX_FRAMES

### Archivo no se carga
- Verifica que la ruta del archivo es correcta
- Asegúrate de que el formato del archivo es válido
- Revisa la consola para mensajes de error específicos

### Mouse no controla rotación en modo grabación
- Asegúrate de estar en modo de grabación (presiona K)
- Verifica que el mouse está capturado por la ventana

## Créditos

Sistema desarrollado siguiendo especificaciones de diseño modular para animación por keyframes en OpenGL.
Basado en el ejemplo de laboratorio EJ10-315348079.cpp.

## Próximas Mejoras Sugeridas

- [ ] Interpolación con curvas de Bezier
- [ ] Editor visual de keyframes
- [ ] Soporte para múltiples pistas de animación
- [ ] Exportación a formatos estándar (FBX, COLLADA)
- [ ] Control de velocidad de reproducción
- [ ] Animación de cámara independiente
