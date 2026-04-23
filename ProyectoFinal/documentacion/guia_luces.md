# Guía de Iluminación — Sistema de Luces del Proyecto

**Archivos relevantes:** `dependencias/Light.h/.cpp`, `dependencias/DirectionalLight.h/.cpp`, `dependencias/PointLight.h/.cpp`, `dependencias/SpotLight.h/.cpp`, `dependencias/Material.h/.cpp`, `dependencias/CommonValues.h`

---

## Jerarquía de clases

```
Light                        ← clase base (color, ambientIntensity, diffuseIntensity)
├── DirectionalLight         ← luz direccional (sol)
└── PointLight               ← luz puntual (foco, lámpara)
    └── SpotLight            ← luz cónica (linterna, faro)
```

Todos los tipos heredan los parámetros de color e intensidad de `Light`. Cada nivel agrega sus propios parámetros específicos.

---

## Límites del sistema

Definidos en `dependencias/CommonValues.h`:

```cpp
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS  = 5;
```

Solo puede existir **1 DirectionalLight** en la escena. Las PointLight y SpotLight se declaran como arreglos:

```cpp
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight  spotLights[MAX_SPOT_LIGHTS];
```

---

## 1. DirectionalLight — Luz Direccional

Simula una fuente de luz infinitamente lejana (como el sol). Ilumina todos los objetos desde la misma dirección, sin atenuación por distancia. **Solo puede haber una** en la escena y siempre debe existir.

### Constructor

```cpp
DirectionalLight(
    GLfloat red,        // color R  [0.0 - 1.0]
    GLfloat green,      // color G  [0.0 - 1.0]
    GLfloat blue,       // color B  [0.0 - 1.0]
    GLfloat aIntensity, // intensidad ambiente  [0.0 - 1.0]
    GLfloat dIntensity, // intensidad difusa    [0.0 - 1.0]
    GLfloat xDir,       // dirección X del vector de luz
    GLfloat yDir,       // dirección Y del vector de luz
    GLfloat zDir        // dirección Z del vector de luz
);
```

**Constructor por defecto:** `color = (1,1,1)`, `ambientIntensity = 1.0f`, `diffuseIntensity = 0.0f`, `direction = (0, -1, 0)`.

### Parámetros explicados

| Parámetro | Descripción | Ejemplo |
|---|---|---|
| `red, green, blue` | Color de la luz | `1.0f, 1.0f, 1.0f` = blanco |
| `aIntensity` | Cuánta luz ambiente emite (ilumina todo por igual) | `0.5f` = 50% |
| `dIntensity` | Intensidad de la componente difusa (depende del ángulo) | `0.3f` |
| `xDir, yDir, zDir` | Vector que indica hacia dónde apunta la luz | `0.0f, -1.0f, 0.0f` = hacia abajo |

### Ejemplo de uso

```cpp
// main.cpp
DirectionalLight mainLight;

mainLight = DirectionalLight(
    1.0f, 1.0f, 1.0f,   // luz blanca
    0.5f, 0.3f,          // ambiente moderada, difusa suave
    0.0f, 0.0f, -1.0f); // apunta hacia -Z

// Enviar al shader cada frame:
shaderList[0].SetDirectionalLight(&mainLight);
```

### Notas
- El vector `(xDir, yDir, zDir)` es la **dirección hacia donde apunta** la luz, no su posición. `(0, -1, 0)` significa que la luz apunta hacia abajo (viene de arriba).
- `aIntensity` alta evita que los objetos en sombra queden completamente negros.
- No tiene atenuación — la distancia no afecta su intensidad.

---

## 2. PointLight — Luz Puntual

Emite luz en todas direcciones desde un punto en el espacio, como una bombilla o una vela. Su intensidad disminuye con la distancia según una fórmula de atenuación.

### Constructor

```cpp
PointLight(
    GLfloat red,        // color R  [0.0 - 1.0]
    GLfloat green,      // color G  [0.0 - 1.0]
    GLfloat blue,       // color B  [0.0 - 1.0]
    GLfloat aIntensity, // intensidad ambiente  [0.0 - 1.0]
    GLfloat dIntensity, // intensidad difusa    [0.0 - 1.0]
    GLfloat xPos,       // posición X en el mundo
    GLfloat yPos,       // posición Y en el mundo
    GLfloat zPos,       // posición Z en el mundo
    GLfloat con,        // atenuación constante
    GLfloat lin,        // atenuación lineal
    GLfloat exp         // atenuación exponencial (cuadrática)
);
```

**Constructor por defecto:** `position = (0,0,0)`, `constant = 1.0f`, `linear = 0.0f`, `exponent = 0.0f`.

### Parámetros explicados

| Parámetro | Descripción | Rango típico |
|---|---|---|
| `red, green, blue` | Color de la luz | `[0.0 - 1.0]` |
| `aIntensity` | Luz ambiente que emite | `0.0f` a `0.3f` |
| `dIntensity` | Intensidad difusa | `0.5f` a `3.0f` |
| `xPos, yPos, zPos` | Posición en espacio mundo | cualquier valor |
| `con` | Factor constante de atenuación | `1.0f` (mínimo recomendado) |
| `lin` | Factor lineal de atenuación | `0.0f` a `0.5f` |
| `exp` | Factor cuadrático de atenuación | `0.0f` a `0.1f` |

### Fórmula de atenuación

El shader calcula la atenuación como:

```
atenuacion = con + lin * distancia + exp * distancia²
color_final = color_luz / atenuacion
```

- `con = 1.0, lin = 0.0, exp = 0.0` → sin atenuación (luz constante)
- `con = 0.3, lin = 0.2, exp = 0.1` → atenuación moderada, alcance medio
- `con = 0.1, lin = 0.1, exp = 0.02` → atenuación suave, alcance largo

### Declaración y uso

```cpp
// main.cpp
PointLight pointLights[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;

// Luz puntual roja
pointLights[0] = PointLight(
    1.0f, 0.0f, 0.0f,   // color rojo
    0.0f, 1.0f,          // sin ambiente, difusa normal
    -6.0f, 1.5f, 1.5f,  // posicion en la escena
    0.3f, 0.2f, 0.1f);  // atenuacion moderada
pointLightCount++;

// Luz puntual blanca (lampara)
pointLights[1] = PointLight(
    1.0f, 1.0f, 1.0f,   // color blanco
    0.1f, 2.0f,          // poca ambiente, difusa fuerte
    -13.0f, 6.7f, -5.0f, // posicion: punta de la lampara
    0.1f, 0.1f, 0.02f); // atenuacion baja, alcance largo
pointLightCount++;

// Enviar al shader:
shaderList[0].SetPointLights(pointLights, pointLightCount);

// Para apagar una luz (buena práctica):
// NO usar color negro — reducir el contador para que el shader no la procese
shaderList[0].SetPointLights(pointLights, pointLightCount - 1);
```

### Posición jerárquica (modelado jerárquico)

Si la luz debe seguir a un modelo 3D, calcular su posición desde la matriz `model`:

```cpp
// La punta del lamppost está en el punto local (0, 9.6, 0)
glm::mat4 modelLampara = glm::mat4(1.0);
modelLampara = glm::translate(modelLampara, glm::vec3(-13.0f, -1.0f, -5.0f));
modelLampara = glm::scale(modelLampara, glm::vec3(0.8f, 0.8f, 0.8f));
glm::vec3 puntaWorld = glm::vec3(modelLampara * glm::vec4(0.0f, 9.6f, 0.0f, 1.0f));

pointLights[1] = PointLight(1.0f, 1.0f, 1.0f, 0.1f, 2.0f,
    puntaWorld.x, puntaWorld.y, puntaWorld.z,
    0.1f, 0.1f, 0.02f);
```

---

## 3. SpotLight — Luz Cónica

Hereda de `PointLight`. Emite luz desde un punto pero solo dentro de un cono definido por una dirección y un ángulo. Útil para linternas, faros de coche, reflectores.

### Constructor

```cpp
SpotLight(
    GLfloat red,        // color R  [0.0 - 1.0]
    GLfloat green,      // color G  [0.0 - 1.0]
    GLfloat blue,       // color B  [0.0 - 1.0]
    GLfloat aIntensity, // intensidad ambiente
    GLfloat dIntensity, // intensidad difusa
    GLfloat xPos,       // posición X
    GLfloat yPos,       // posición Y
    GLfloat zPos,       // posición Z
    GLfloat xDir,       // dirección X del cono
    GLfloat yDir,       // dirección Y del cono
    GLfloat zDir,       // dirección Z del cono
    GLfloat con,        // atenuación constante
    GLfloat lin,        // atenuación lineal
    GLfloat exp,        // atenuación exponencial
    GLfloat edg         // ángulo del cono en grados
);
```

**Constructor por defecto:** `direction = (0, -1, 0)`, `edge = 0.0f`, `procEdge = cos(0°) = 1.0f`.

### Parámetros adicionales respecto a PointLight

| Parámetro | Descripción | Ejemplo |
|---|---|---|
| `xDir, yDir, zDir` | Dirección hacia donde apunta el cono (se normaliza internamente) | `0.0f, -1.0f, 0.0f` = hacia abajo |
| `edg` | Ángulo de apertura del cono en grados | `5.0f` = cono cerrado, `25.0f` = cono amplio |

El ángulo se convierte internamente a coseno: `procEdge = cosf(glm::radians(edg))`. Cuanto menor el ángulo, más concentrado el haz. El vector de dirección se normaliza automáticamente en el constructor.

### Métodos adicionales

```cpp
void SetFlash(glm::vec3 pos, glm::vec3 dir); // actualiza posición Y dirección simultáneamente
void SetPos(glm::vec3 pos);                  // actualiza solo la posición
```

### Declaración y uso

```cpp
// main.cpp
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int spotLightCount = 0;

// Linterna ligada a la camara
spotLights[0] = SpotLight(
    1.0f, 1.0f, 1.0f,   // color blanco
    0.0f, 2.0f,          // sin ambiente, difusa fuerte
    0.0f, 0.0f, 0.0f,   // posicion inicial (se sobreescribe cada frame)
    0.0f, -1.0f, 0.0f,  // direccion inicial (se sobreescribe cada frame)
    1.0f, 0.0f, 0.0f,   // sin atenuacion
    5.0f);               // cono muy cerrado (linterna)
spotLightCount++;

// Faro de coche
spotLights[1] = SpotLight(
    1.0f, 1.0f, 0.0f,   // color amarillo
    0.0f, 3.0f,
    0.0f, 0.0f, 0.0f,   // posicion (se calcula jerarquicamente)
    0.0f, 0.0f, -1.0f,  // apunta hacia -Z
    1.0f, 0.0f, 0.0f,
    20.0f);              // cono de 20 grados
spotLightCount++;

// Enviar al shader:
shaderList[0].SetSpotLights(spotLights, spotLightCount);
```

### Actualizar posición y dirección en tiempo real

`SetFlash` actualiza posición y dirección simultáneamente. Se llama cada frame para luces que se mueven:

```cpp
// Linterna ligada a la camara
glm::vec3 lowerLight = camera.getCameraPosition();
lowerLight.y -= 0.3f;
spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

// Faro de coche con modelado jerarquico
glm::vec4 faroLocalPos = glm::vec4(1.4f, 1.5f, -3.5f, 1.0f);
glm::vec4 faroLocalDir = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
glm::vec3 faroPosWorld = glm::vec3(modelMustang * faroLocalPos);
glm::vec3 faroDirWorld = glm::normalize(glm::vec3(modelMustang * faroLocalDir));
spotLights[1].SetFlash(faroPosWorld, faroDirWorld);

// Siempre llamar SetSpotLights DESPUÉS de SetFlash para que el shader reciba los valores actualizados
shaderList[0].SetSpotLights(spotLights, spotLightCount);
```

---

## 4. Material

No es una luz, pero controla cómo los objetos reaccionan a la iluminación especular. Se aplica por objeto antes de renderizarlo.

### Constructor

```cpp
Material(
    GLfloat sIntensity, // intensidad especular [0.0 - 10.0+]
    GLfloat shine       // brillo (shininess)   [0.0 en adelante, típico 2 - 1024]
);
```

**Constructor por defecto:** `specularIntensity = 0.0f`, `shininess = 0.0f`.

| Parámetro | Descripción | Ejemplo |
|---|---|---|
| `sIntensity` | Qué tan brillante es el reflejo especular | `4.0f` = muy brillante, `0.3f` = opaco |
| `shine` | Concentración del reflejo (mayor = más pequeño y definido) | `256` = espejo, `4` = plástico mate |

> **Nota:** `shine` es `GLfloat`, no `int`. Aunque se puede pasar un entero como `4` o `256`, internamente se almacena como flotante y se envía al shader con `glUniform1f`.

### Uso

```cpp
// main.cpp
Material Material_brillante;
Material Material_opaco;

Material_brillante = Material(4.0f, 256);  // metal, vidrio
Material_opaco     = Material(0.3f, 4);    // madera, tela, asfalto

// Aplicar antes de renderizar cada objeto:
Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
miModelo.RenderModel();

Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
meshList[0]->RenderMesh();
```

---

## Resumen de buenas prácticas

| Situación | Forma incorrecta | Forma correcta |
|---|---|---|
| Apagar una luz | Poner color `(0,0,0)` o `IA=ID=0` | Reducir el contador: `SetPointLights(lights, count - 1)` |
| Apagar una luz | Mover a posición muy lejana | Reducir el contador |
| Luz que sigue a un modelo | Hardcodear posición | Multiplicar punto local por matriz `model` |
| Actualizar spotlight móvil | Llamar `SetSpotLights` antes de `SetFlash` | Llamar `SetFlash` primero, luego `SetSpotLights` |
| Luces para toda la escena | Llamar `SetPointLights` después de renderizar | Llamar `SetPointLights` al inicio del frame |
