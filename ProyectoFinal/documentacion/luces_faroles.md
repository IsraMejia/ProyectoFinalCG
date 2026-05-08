# Luces Spotlight en Faroles

## Descripción
Se han agregado luces spotlight en cada uno de los 5 faroles distribuidos por el mapa. Las luces se pueden encender y apagar con la tecla **L**.

## Características

### Posiciones de los Faroles con Spotlight
1. **Farola 1**: (-69.55, 5.0, 19.05) - Con cámara
2. **Farola 2**: (10.94, 5.0, 81.13) - Sin cámara
3. **Farola 3**: (-26.55, 5.0, -66.53) - Con cámara
4. **Farola 4**: (93.23, 5.0, 44.67) - Sin cámara
5. **Farola 5**: (22.57, 5.0, -0.40) - Con cámara

### Propiedades de las Luces
- **Color**: Amarillo cálido (1.0, 0.9, 0.7) - simula luz de farol tradicional
- **Intensidad**: 1.5 (cuando están encendidas), 0.0 (cuando están apagadas)
- **Dirección**: Hacia abajo (0, -1, 0)
- **Atenuación**: 
  - Constante: 1.0
  - Lineal: 0.01
  - Cuadrática: 0.001
- **Ángulo del cono**: 15 grados
- **Altura**: A la altura de la cámara (~8 unidades sobre la base del farol)

### Control
- **Tecla L**: Alterna entre encender y apagar todas las luces de los faroles simultáneamente
- Estado inicial: **Encendidas**

## Archivos Modificados

### 1. `ProyectoFinal/dependencias/Window.h`
- Agregada variable `bool farolesEncendidos`
- Agregado método `getFarolesEncendidos()`

### 2. `ProyectoFinal/dependencias/Window.cpp`
- Inicialización de `farolesEncendidos = true`
- Manejo de tecla L para toggle de faroles

### 3. `ProyectoFinal/Integrantes/Andrea/farola.h`
- Agregado método `GetLightPosition()` para obtener la posición de la luz

### 4. `ProyectoFinal/main.cpp`
- Creación de 5 spotlights (uno por farol)
- Lógica de actualización de intensidad según estado de `farolesEncendidos`
- Control dinámico de luces en el bucle principal

## Uso
1. Ejecutar el programa
2. Las luces de los faroles estarán encendidas por defecto
3. Presionar **L** para apagar todas las luces de los faroles
4. Presionar **L** nuevamente para encenderlas

## Notas Técnicas
- Las luces se actualizan en cada frame del bucle principal
- La intensidad se controla modificando el parámetro diffuse de cada spotlight
- Se mantiene el límite de MAX_SPOT_LIGHTS = 5 definido en CommonValues.h
- Las luces están posicionadas a la altura de la cámara de vigilancia (~8 unidades sobre la base)
