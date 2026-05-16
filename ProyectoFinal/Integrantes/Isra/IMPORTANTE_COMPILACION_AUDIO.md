# ⚠️ IMPORTANTE: Configuración de Compilación del Sistema de Audio

## 📋 Archivos que DEBES agregar al proyecto de Visual Studio

Para que el sistema de audio funcione correctamente, debes agregar estos **3 archivos** a tu proyecto:

### 1. `dependencias/miniaudio_impl.cpp` ⭐ CRÍTICO
   - **Este archivo DEBE compilarse PRIMERO**
   - Contiene la implementación de miniaudio
   - Solo se compila UNA VEZ en todo el proyecto

### 2. `Integrantes/Isra/audio_universos.cpp`
   - Implementación del sistema de audio posicional
   - Usa las funciones de miniaudio

### 3. `Integrantes/Isra/audio_universos.h`
   - Header con las declaraciones de funciones públicas
   - Ya está incluido en `main.cpp`

## 🔧 Cómo agregar los archivos en Visual Studio

### Paso 1: Abrir el proyecto
Abre tu proyecto `ProyectoFinal.vcxproj` en Visual Studio

### Paso 2: Agregar archivos
1. En el **Solution Explorer**, clic derecho en tu proyecto
2. **Add** → **Existing Item...**
3. Navega y selecciona:
   - `dependencias/miniaudio_impl.cpp`
   - `Integrantes/Isra/audio_universos.cpp`
   - `Integrantes/Isra/audio_universos.h`
4. Clic en **Add**

### Paso 3: Verificar orden de compilación (Opcional)
Para asegurarte de que `miniaudio_impl.cpp` se compile primero:
1. Clic derecho en el proyecto → **Properties**
2. **C/C++** → **General**
3. Verifica que los directorios de include incluyan la carpeta raíz del proyecto

### Paso 4: Compilar
Presiona **F7** para compilar

## ✅ Verificación

Después de compilar, deberías ver en la salida:
```
Compiling miniaudio_impl.cpp...
Compiling audio_universos.cpp...
Compiling main.cpp...
```

## 🐛 Solución de Problemas

### Error: "LNK2005: ma_* already defined"
**Causa:** `MINIAUDIO_IMPLEMENTATION` está definido en más de un archivo

**Solución:** 
- Asegúrate de que SOLO `miniaudio_impl.cpp` tenga `#define MINIAUDIO_IMPLEMENTATION`
- `audio_universos.cpp` NO debe tener esa definición

### Error: "unresolved external symbol ma_*"
**Causa:** `miniaudio_impl.cpp` no está siendo compilado

**Solución:**
- Verifica que `miniaudio_impl.cpp` esté agregado al proyecto
- Verifica que no esté excluido de la compilación (clic derecho → Properties → Excluded From Build debe ser "No")

### Error: "cannot open source file glm.hpp"
**Causa:** Los directorios de include no están configurados correctamente

**Solución:**
1. Clic derecho en el proyecto → **Properties**
2. **C/C++** → **General** → **Additional Include Directories**
3. Agrega: `$(ProjectDir);$(ProjectDir)glm`

## 📝 Estructura Final

Tu proyecto debe verse así:

```
ProyectoFinal/
├── dependencias/
│   ├── miniaudio.h
│   └── miniaudio_impl.cpp          ← NUEVO - Agregar al proyecto
├── Integrantes/
│   └── Isra/
│       ├── audio_universos.cpp     ← NUEVO - Agregar al proyecto
│       └── audio_universos.h       ← NUEVO - Agregar al proyecto
└── main.cpp                        ← Ya modificado con las llamadas
```

## 🎯 Siguiente Paso

Una vez agregados los archivos:
1. Compila (**F7**)
2. Ejecuta (**F5** o **Ctrl+F5**)
3. Deberías escuchar el audio funcionando

Si tienes problemas, revisa el archivo `audio/test_audio/DIAGNOSTICO_AUDIO.md` para más ayuda.
