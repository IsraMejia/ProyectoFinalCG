# Guía: Texturas para Modelos 3D

## Regla Importante

`Model.cpp` **siempre** busca texturas en `ProyectoFinal/Textures/`, sin importar dónde esté tu modelo.

## Pasos para usar modelos con texturas

### 1. Organiza tu modelo
```
Integrantes/<Nombre>/modelos/<MiModelo>/
├── mimodelo.obj
├── mimodelo.mtl
└── textures/
    ├── textura1.jpg
    └── textura2.jpg
```

### 2. Copia texturas a carpeta global
**Desde:** `Integrantes/<Nombre>/modelos/<MiModelo>/textures/*.jpg`  
**Hacia:** `ProyectoFinal/Textures/`

Copia **todos** los archivos de imagen (jpg, png, tga) a la carpeta `Textures/` en la raíz.

### 3. Edita el archivo MTL
El MTL debe tener **solo el nombre del archivo**, sin carpetas:

❌ **Incorrecto:**
```mtl
map_Kd textures/mi_textura.jpg
map_Kd ../texturas/mi_textura.jpg
```

✅ **Correcto:**
```mtl
map_Kd mi_textura.jpg
```

### 4. Ejemplo real: EstacionTren

**Estructura:**
```
Integrantes/Isra/modelos/EstacionTren/
├── estacion_tren.obj
├── estacion_tren.mtl
└── textures/
    ├── Estacion_de_tren_baseColor.jpg
    ├── Estacion_de_tren1_baseColor.jpg
    ├── Estacion_de_tren2_baseColor.jpg
    └── Estacion_de_tren3_baseColor.jpg
```

**MTL correcto:**
```mtl
newmtl Estacion_de_tren
map_Kd Estacion_de_tren_baseColor.jpg

newmtl Estacion_de_tren1
map_Kd Estacion_de_tren1_baseColor.jpg
```

**Texturas copiadas a:**
```
ProyectoFinal/Textures/
├── Estacion_de_tren_baseColor.jpg
├── Estacion_de_tren1_baseColor.jpg
├── Estacion_de_tren2_baseColor.jpg
└── Estacion_de_tren3_baseColor.jpg
```

## Por qué funciona así

`Model.cpp` (línea 157) construye la ruta: `"Textures/" + nombre_archivo`

No importa dónde esté tu OBJ, las texturas **siempre** se buscan en `Textures/`.

## Checklist

- [ ] Texturas copiadas a `ProyectoFinal/Textures/`
- [ ] MTL tiene solo nombres de archivo (sin `textures/` ni rutas)
- [ ] Nombres de archivo coinciden exactamente (case-sensitive)
- [ ] Extensiones correctas (.jpg, .png, .tga)

✅ Si sigues esto, tus modelos tendrán texturas correctamente
