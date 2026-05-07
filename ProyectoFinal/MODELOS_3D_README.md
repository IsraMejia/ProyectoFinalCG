# Modelos 3D y Texturas - Instrucciones

## 📦 Ubicación de los Archivos

Los modelos 3D y texturas **NO están incluidos en el repositorio Git** debido a su tamaño.

### Estructura de Carpetas Requerida

```
ProyectoFinal/
├── Models/          # Archivos .obj, .mtl, .3ds
└── Textures/        # Archivos .jpg, .png, .tga, etc.
```

---

## 🔽 Cómo Obtener los Modelos 3D

### Opción 1: Descarga desde almacenamiento en la nube

**[PENDIENTE: Agregar enlace a Google Drive / Dropbox / OneDrive]**

1. Descarga el archivo comprimido desde: `[ENLACE AQUÍ]`
2. Extrae el contenido en la carpeta `ProyectoFinal/`
3. Verifica que las carpetas `Models/` y `Textures/` estén en su lugar

### Opción 2: Solicitar a un miembro del equipo

Contacta a cualquiera de los siguientes miembros del equipo:
- **[Nombre]** - [email/contacto]
- **[Nombre]** - [email/contacto]

---

## ✅ Verificación de la Instalación

Después de copiar los archivos, verifica que tienes la siguiente estructura:

```
ProyectoFinal/
├── Models/
│   ├── grunt_cuerpo.obj
│   ├── grunt_cuerpo.mtl
│   ├── grunt_rostro.obj
│   ├── grunt_rostro.mtl
│   ├── grunt_brazo_der.obj
│   ├── grunt_brazo_der.mtl
│   ├── grunt_brazo_izq.obj
│   ├── grunt_brazo_izq.mtl
│   ├── elite_cuerpo.obj
│   ├── elite_cuerpo.mtl
│   ├── elite_rostro.obj
│   ├── elite_rostro.mtl
│   ├── elite_brazo_der.obj
│   ├── elite_brazo_der.mtl
│   ├── elite_brazo_izq.obj
│   ├── elite_brazo_izq.mtl
│   ├── raven.obj
│   ├── raven.mtl
│   ├── pelican.obj
│   ├── pelican.mtl
│   ├── estacion_tren.obj
│   ├── estacion_tren.mtl
│   ├── faro.obj
│   ├── faro.mtl
│   ├── gato.obj
│   ├── gato.mtl
│   ├── maquina_vapor.obj
│   ├── maquina_vapor.mtl
│   ├── tren.obj
│   ├── tren.mtl
│   ├── vagon.obj
│   ├── vagon.mtl
│   ├── forerunner.obj
│   ├── forerunner.mtl
│   └── ... (otros modelos)
│
└── Textures/
    ├── material_0_baseColor.png
    ├── big_raven_Material_baseColor.jpg
    ├── LandingLightBase.0010_material_baseColor.jpg
    ├── RightDoor0_material_baseColor.jpg
    ├── RightDoor0_material_normal_scale0_norm.jpg
    ├── f_blackout0_material_baseColor.jpg
    ├── hull_base1_001.0010_material_baseColor.jpg
    ├── hull_base1_001.0010_material_normal_scale0_norm.jpg
    ├── hull_base1_0100_material_normal_scale0_norm.jpg
    ├── hull_stand0_material_baseColor.jpg
    ├── hull_stand0_material_normal_scale0_norm.jpg
    ├── rightclamp0_material_baseColor.jpg
    ├── 24_Bodysuit_1_0_0_diffuse.jpg
    ├── 24_FPBase_1_0_0_diffuse.jpg
    ├── 24_Head_1_0_0_diffuse.jpg
    ├── 26_BaseLower_0.21_0_0_diffuse.jpg
    ├── 26_BaseUpper_0.21_0_0_diffuse.jpg
    ├── 26_Chest_0.21_0_0_diffuse.jpg
    ├── 26_Gauntlet_0.21_0_0_diffuse.jpg
    ├── 26_Helmet_0.21_0_0_diffuse.jpg
    ├── 26_Helmet_0.21_0_0_normal.jpg
    ├── 26_Shoulder_0.21_0_0_diffuse.jpg
    ├── 26_Thighs_0.21_0_0_diffuse.jpg
    ├── 5_Eye_1_0_0_diffuse.jpg
    ├── Estacion_de_tren_baseColor.jpg
    ├── Estacion_de_tren1_baseColor.jpg
    ├── Estacion_de_tren2_baseColor.jpg
    ├── Estacion_de_tren3_baseColor.jpg
    ├── lambert2_baseColor.jpg
    ├── gato_body.jpg
    ├── gato_eye_white.jpg
    ├── gato_eye_red.png
    ├── Material_0.001_baseColor.jpg
    ├── lambert6SG_baseColor.jpg
    ├── lambert6SG_normal.jpg
    ├── lambert7SG_baseColor.png
    ├── lambert7SG_normal.jpg
    ├── lambert8SG_emissive.jpg
    ├── lambert8SG_normal.jpg
    └── ... (otras texturas)
```

---

## 🚫 Importante: NO Subir al Repositorio

**Los archivos en `Models/` y `Textures/` están excluidos del repositorio Git.**

El archivo `.gitignore` está configurado para ignorar:
- `ProyectoFinal/Models/`
- `ProyectoFinal/Textures/`
- Todos los archivos `.obj`, `.mtl`, `.3ds`
- Todos los archivos de imagen (`.jpg`, `.png`, `.tga`, etc.)

Si intentas hacer `git add`, estos archivos serán ignorados automáticamente.

---

## 🔧 Solución de Problemas

### Error: "No se encuentra el archivo .obj"

**Causa**: Los modelos 3D no están en la ubicación correcta.

**Solución**:
1. Verifica que las carpetas `Models/` y `Textures/` existen en `ProyectoFinal/`
2. Verifica que los archivos `.obj` y `.mtl` están dentro de `Models/`
3. Verifica que las texturas están dentro de `Textures/`

### Error: "No se encuentra la textura"

**Causa**: Los archivos `.mtl` referencian texturas con la ruta `../Textures/`

**Solución**:
1. Asegúrate de que la carpeta `Textures/` está al mismo nivel que `Models/`
2. Verifica que los nombres de las texturas coinciden exactamente (sensible a mayúsculas/minúsculas)

### Los modelos se ven sin textura (blancos o negros)

**Causa**: Las texturas no se están cargando correctamente.

**Solución**:
1. Verifica que todos los archivos de textura están en `Textures/`
2. Verifica que los archivos `.mtl` tienen las rutas correctas (`../Textures/nombre_textura.jpg`)
3. Revisa la consola de la aplicación para ver mensajes de error

---

## 📝 Notas para Desarrolladores

### Rutas en el Código

Los archivos `.cpp` cargan los modelos con rutas relativas desde la raíz del proyecto:

```cpp
// Ejemplo en grunt.cpp
gruntCuerpo.LoadModel("Models/grunt_cuerpo.obj");
```

### Rutas en los Archivos .mtl

Los archivos `.mtl` referencian las texturas con rutas relativas desde `Models/`:

```mtl
# Ejemplo en grunt_cuerpo.mtl
map_Kd ../Textures/material_0_baseColor.png
```

### Agregar Nuevos Modelos

Si agregas nuevos modelos 3D:

1. Coloca los archivos `.obj` y `.mtl` en `Models/`
2. Coloca las texturas en `Textures/`
3. Actualiza los archivos `.mtl` para usar rutas `../Textures/`
4. Actualiza este README con la lista de nuevos archivos
5. Comparte los archivos con el equipo a través del almacenamiento en la nube

---

## 📊 Tamaño de los Archivos

**Tamaño aproximado total**: [PENDIENTE: Agregar tamaño]

- `Models/`: ~[XX] MB
- `Textures/`: ~[XX] MB

**Total**: ~[XX] MB

Por esta razón, estos archivos no se incluyen en el repositorio Git.

---

## 🤝 Contribuciones

Si tienes modelos 3D o texturas para agregar al proyecto:

1. Colócalos en las carpetas correspondientes localmente
2. Compártelos con el equipo a través del almacenamiento en la nube
3. Actualiza este README con la información de los nuevos archivos
4. Notifica al equipo sobre los cambios

---

## 📞 Contacto

Si tienes problemas para obtener o configurar los modelos 3D, contacta a:

**[PENDIENTE: Agregar información de contacto del equipo]**

---

**Última actualización**: [Fecha]
