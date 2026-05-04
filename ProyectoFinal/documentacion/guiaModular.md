# Guía Modular - Proyecto Final

## Regla: 1 línea por feature en main.cpp

Cada modelo/feature debe estar en su propio módulo: `Integrantes/<Nombre>/<modulo>.h/.cpp`

## Patrón Rápido

### 1. Header (.h)
```cpp
#pragma once
#include <glew.h>
#include <glm.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class MiModulo {
public:
    MiModulo();
    void Initialize();
    void Render(GLuint uniformModel, GLuint uniformColor,
                GLuint uniformSpecularIntensity, GLuint uniformShininess,
                const float toRadians);
private:
    Model modelo;
    Material material;
    glm::vec3 position, scale;
    float rotationY;
};
```

### 2. Implementación (.cpp)
```cpp
#include "mimodulo.h"

MiModulo::MiModulo() {
    position = glm::vec3(x, y, z);
    scale = glm::vec3(sx, sy, sz);
    rotationY = angulo;
    material = Material(0.3f, 4);
}

void MiModulo::Initialize() {
    modelo.LoadModel("Models/mimodelo.obj");
}

void MiModulo::Render(GLuint uniformModel, GLuint uniformColor,
    GLuint uniformSpecularIntensity, GLuint uniformShininess,
    const float toRadians) {
    
    glm::mat4 model(1.0);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotationY * toRadians, glm::vec3(0,1,0));
    model = glm::scale(model, scale);
    
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1,1,1)));
    material.UseMaterial(uniformSpecularIntensity, uniformShininess);
    modelo.RenderModel();
}
```

### 3. En main.cpp (4 líneas total)
```cpp
#include "Integrantes/<Nombre>/<modulo>.h"  // 1. Include
MiModulo miModulo;                          // 2. Declarar global
miModulo.Initialize();                      // 3. En main(), antes del loop
miModulo.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians); // 4. En loop
```

## Ejemplo Real: HaloForerunner

**Antes (13 líneas en main.cpp):**
```cpp
Model forerunnerModel;
forerunnerModel.LoadModel("Models/forerunner.obj");
// En loop:
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(70.0f, -5.0f, 70.0f));
model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0,1,0));
model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
// ... más código
```

**Después (1 línea en main.cpp):**
```cpp
haloForerunner.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, toRadians);
```

Ver `Integrantes/Isra/halo.h` y `halo.cpp` para implementación completa.

---

## Jerarquía (opcional)

Si tu feature va sobre otro objeto (ej: vías sobre isla):

```cpp
// En Render(), recibe parentTransform:
void Render(..., const glm::mat4& parentTransform, ...) {
    glm::mat4 model = parentTransform;  // Heredar
    model = glm::translate(model, offsetLocal);
    // ...
}
```

## Checklist

- [ ] Crear `.h` y `.cpp` en `Integrantes/<Nombre>/`
- [ ] Include en `main.cpp`
- [ ] Declarar instancia global
- [ ] Llamar `Initialize()` antes del loop
- [ ] Llamar `Render()` en el loop (1 línea)
- [ ] Agregar `.cpp` al proyecto Visual Studio

✅ **Resultado:** main.cpp limpio, código reutilizable, fácil para Kiro
