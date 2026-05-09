# Guía del Sistema de Cámaras

## Descripción General

El proyecto ahora cuenta con un sistema dual de cámaras que permite alternar entre dos modos de visualización:

1. **Cámara de Desarrollo** (Modo 1) - Cámara libre tipo FPS
2. **Cámara en Tercera Persona** (Modo 2) - Cámara jerárquica vinculada al Jefe Maestro

## Controles

### Cambio de Cámara

- **Tecla 1**: Activar cámara de desarrollo
- **Tecla 2**: Activar cámara en tercera persona

### Cámara de Desarrollo (Modo 1)

**Movimiento:**
- **W**: Avanzar
- **S**: Retroceder
- **A**: Moverse a la izquierda
- **D**: Moverse a la derecha

**Rotación:**
- **Mouse**: Controla la dirección de la vista

**Características:**
- Movimiento libre en todas direcciones
- Ideal para explorar el mapa y posicionar elementos
- La posición se guarda automáticamente en `camera_position.txt`

### Cámara en Tercera Persona (Modo 2)

**Movimiento del Personaje:**
- **↑ (Flecha Arriba)**: Mover hacia adelante
- **↓ (Flecha Abajo)**: Mover hacia atrás
- **← (Flecha Izquierda)**: Mover a la izquierda
- **→ (Flecha Derecha)**: Mover a la derecha

**Rotación de la Vista:**
- **Mouse**: Controla la dirección de la cámara

**Características:**
- La cámara sigue automáticamente al Jefe Maestro
- Offset fijo: 1 unidad detrás, 1 unidad arriba del personaje
- El personaje siempre permanece centrado en pantalla
- El movimiento está restringido al plano horizontal (XZ)
- El personaje rota automáticamente en la dirección del movimiento
- La animación de caminata se activa al mover el personaje
- Los parámetros de la cámara se guardan en `camera_position_personaje.txt`

## Arquitectura del Sistema

### Archivos Principales

#### 1. `personaje_camara.h` / `personaje_camara.cpp`
Implementa la clase `ThirdPersonCamera` que encapsula toda la lógica de la cámara en tercera persona.

**Características clave:**
- Sistema de tiempo independiente usando `glfwGetTime()`
- Cálculo de vectores de dirección (front, right, up)
- Posicionamiento jerárquico respecto al personaje
- Control de mouse con límites de pitch

#### 2. `camara_position.h` / `camara_position.cpp`
Clase `CameraPositionTracker` que gestiona ambas cámaras y el cambio entre ellas.

**Enum CameraMode:**
```cpp
enum class CameraMode
{
    DEVELOPMENT,      // Cámara de desarrollo
    THIRD_PERSON      // Cámara en tercera persona
};
```

**Métodos principales:**
- `Update()`: Actualiza la cámara activa y maneja el cambio de modo
- `GetViewMatrix()`: Retorna la matriz de vista de la cámara activa
- `GetCameraPosition()`: Retorna la posición de la cámara activa
- `GetCameraDirection()`: Retorna la dirección de la cámara activa

#### 3. `personaje_principal.h` / `personaje_principal.cpp`
Clase `MasterChief` actualizada con métodos para movimiento controlado.

**Nuevos métodos:**
```cpp
void Move(const glm::vec3& direction, float deltaTime, float speed);
void SetRotationY(float newRotation);
float GetRotationY() const;
```

### Integración en main.cpp

El flujo de actualización en el loop principal es:

```cpp
// 1. Actualizar sistema de cámaras
cameraTracker.Update(keys, xChange, yChange, deltaTime, masterChief.GetPosition());

// 2. Obtener modo de cámara actual
CameraMode cameraMode = cameraTracker.GetCameraMode();

// 3. Manejar movimiento según el modo
if (cameraMode == CameraMode::THIRD_PERSON)
{
    // Mover personaje con flechas
    // Actualizar animación basada en movimiento
}
else
{
    // Comportamiento original (animación con flecha arriba)
}

// 4. Obtener matrices de la cámara activa
glm::mat4 viewMatrix = cameraTracker.GetViewMatrix();
glm::vec3 eyePosition = cameraTracker.GetCameraPosition();
```

## Persistencia de Datos

### camera_position.txt
Guarda la posición de la cámara de desarrollo:
```
x y z
```

### camera_position_personaje.txt
Guarda los ángulos de rotación de la cámara en tercera persona:
```
yaw pitch
```

## Parámetros Configurables

### En ThirdPersonCamera (personaje_camara.cpp)

```cpp
// Offset de la cámara respecto al personaje
offsetDistance = 1.0f;  // Distancia detrás del personaje
offsetHeight = 1.0f;    // Altura sobre el personaje

// Velocidades
moveSpeed = 0.3f;       // Velocidad de movimiento
turnSpeed = 0.5f;       // Sensibilidad del mouse
```

### En main.cpp (movimiento del personaje)

```cpp
// Velocidad de movimiento del personaje
masterChief.Move(moveDirection, deltaTime, 5.0f);  // 5 unidades/segundo
```

## Restricciones Importantes

1. **Movimiento en Y**: El personaje NO puede moverse en el eje Y. La altura se mantiene constante.

2. **Relación Jerárquica**: La cámara está vinculada jerárquicamente al personaje, por lo que:
   - Siempre mantiene el offset definido
   - Sigue automáticamente al personaje
   - El personaje permanece centrado en pantalla

3. **Rotación del Personaje**: En modo tercera persona, el personaje rota automáticamente para mirar en la dirección del movimiento.

## Futuras Mejoras

Posibles extensiones del sistema:

1. **Colisiones**: Detectar cuando la cámara choca con objetos y ajustar su posición
2. **Zoom dinámico**: Permitir ajustar la distancia de la cámara con la rueda del mouse
3. **Suavizado**: Agregar interpolación para transiciones más suaves
4. **Múltiples personajes**: Extender el sistema para soportar cambio entre diferentes personajes
5. **Modos de cámara adicionales**: Primera persona, vista cenital, etc.

## Solución de Problemas

### La cámara no sigue al personaje
- Verificar que estás en modo tercera persona (tecla 2)
- Confirmar que `masterChief.GetPosition()` se pasa correctamente en `Update()`

### El personaje no se mueve
- Asegurarse de estar en modo tercera persona
- Verificar que las teclas de flecha están siendo detectadas

### La animación no se activa
- En modo tercera persona, la animación se activa automáticamente al mover
- En modo desarrollo, solo se activa con flecha arriba (comportamiento original)

### Los archivos de posición no se guardan
- Verificar permisos de escritura en el directorio del proyecto
- Confirmar que los nombres de archivo son correctos

## Créditos

Sistema desarrollado siguiendo la arquitectura modular del proyecto, con encapsulación completa y manejo independiente del tiempo, similar a los módulos existentes como `personaje_principal.cpp` e `iluminacion_dia_noche.cpp`.
