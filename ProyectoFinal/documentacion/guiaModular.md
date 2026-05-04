# Guía de Código Modular - Proyecto Final

## Objetivo
Mantener `main.cpp` limpio y legible, moviendo la lógica de cada elemento a módulos separados en `Integrantes/<nombre>/`.

## Estructura de Archivos

```
ProyectoFinal/
├── main.cpp                          # Solo declaraciones e inicialización
├── Integrantes/
│   └── <Nombre>/
│       ├── <modulo>.h                # Header con clase
│       └── <modulo>.cpp              # Implementación completa
```

## Patrón de Modularización

### 1. Crear Header (.h)
```cpp
#pragma once
#include <glew.h>
#include <glm.hpp>
#include "../../dependencias/Model.h"
#include "../../dependencias/Material.h"

class MiModulo
{
public:
    MiModulo();
    ~MiModulo();
    
    void Initialize();  // Carga modelos/genera geometría
    void Render(GLuint uniformModel, GLuint uniformColor,
                GLuint uniformSpecularIntensity, GLuint uniformShininess,
                const glm::mat4& parentTransform, const float toRadians);

private:
    Model modelo;
    Material material;
    // ... otros miembros privados
};
```

### 2. Implementar (.cpp)
```cpp
#include "mimodulo.h"

MiModulo::MiModulo()
{
    // Inicializar parámetros (posición, escala, etc.)
}

void MiModulo::Initialize()
{
    // Cargar modelos o generar geometría procedural
    modelo.LoadModel("Models/mimodelo.obj");
}

void MiModulo::Render(...)
{
    // Aplicar transformaciones y renderizar
    glm::mat4 model = parentTransform;
    model = glm::translate(model, posicion);
    model = glm::rotate(model, rotacion, eje);
    model = glm::scale(model, escala);
    
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    material.UseMaterial(uniformSpecularIntensity, uniformShininess);
    modelo.RenderModel();
}
```

### 3. Usar en main.cpp

**Incluir header:**
```cpp
#include "Integrantes/<Nombre>/<modulo>.h"
```

**Declarar globalmente:**
```cpp
MiModulo miModulo;
```

**Inicializar (en main, antes del loop):**
```cpp
miModulo.Initialize();
```

**Renderizar (en el loop, 1 línea):**
```cpp
miModulo.Render(uniformModel, uniformColor, uniformSpecularIntensity, uniformShininess, parentTransform, toRadians);
```

## Jerarquía de Transformaciones

Para elementos que dependen de otro (ej: vías sobre la isla):

```cpp
// En main.cpp - guardar transformación del padre
glm::mat4 islandTransform = model;
islandModel.RenderModel();

// Pasar al hijo
viasTren.Render(..., islandTransform, ...);
```

En el módulo hijo:
```cpp
glm::mat4 model = parentTransform;  // Heredar transformación
model = glm::translate(model, offsetLocal);  // Aplicar offset local
```

## Ejemplos en el Proyecto

### ViasTren (Geometría Procedural)
- **Archivos**: `Integrantes/Isra/vias_tren.h`, `vias_tren.cpp`
- **En main.cpp**: 3 líneas (include, declaración, Initialize, Render)
- **Genera**: Anillos ovalados + tablas mediante vértices
- **Jerarquía**: Hereda transformación de `islandTransform`

### Forerunner (Modelo 3D)
- **En main.cpp**: Código inline (puede modularizarse igual)
- **Carga**: `forerunnerModel.LoadModel("Models/forerunner.obj")`
- **Render**: Transformaciones + `RenderModel()`

## Ventajas

✅ **main.cpp limpio**: Solo 1-3 líneas por elemento  
✅ **Código aislado**: Cambios no afectan otros módulos  
✅ **Reutilizable**: Fácil copiar patrón para nuevos elementos  
✅ **Legible para AI**: Archivos pequeños, contexto claro  
✅ **Escalable**: Agregar elementos sin saturar main.cpp  

## Notas Importantes

- **Visual Studio**: Agregar `.cpp` al proyecto manualmente (Add → Existing Item)
- **Paths relativos**: Headers usan `../../dependencias/`
- **Auto-inicialización**: Opcional en `Render()` si se olvida `Initialize()`
- **Parámetros uniformes**: Pasar todos los uniforms necesarios al `Render()`
- **Material propio**: Cada módulo puede tener su propio `Material`

## Checklist para Nuevo Módulo

1. [ ] Crear `Integrantes/<Nombre>/<modulo>.h`
2. [ ] Crear `Integrantes/<Nombre>/<modulo>.cpp`
3. [ ] Definir clase con `Initialize()` y `Render()`
4. [ ] Include en `main.cpp`
5. [ ] Declarar instancia global
6. [ ] Llamar `Initialize()` antes del loop
7. [ ] Llamar `Render()` en el loop (1 línea)
8. [ ] Agregar `.cpp` al proyecto de Visual Studio
