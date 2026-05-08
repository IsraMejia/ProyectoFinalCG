# 🚀 Guía Rápida: Cómo Usar Git en Este Proyecto

## 📌 Situación Actual
Tienes cambios en tu computadora y quieres:
1. ✅ Subir tus cambios a GitHub
2. ✅ Ver los cambios de tus compañeros
3. ✅ NO perder tu trabajo

---

## 🎯 Opción 1: Usar Scripts Automáticos (MÁS FÁCIL)

### Para SUBIR tus cambios:
```bash
# En Windows (doble clic o desde terminal)
subir_cambios.bat
```

### Para OBTENER cambios de compañeros:
```bash
# En Windows (doble clic o desde terminal)
obtener_cambios.bat
```

---

## 🎯 Opción 2: Comandos Manuales (MÁS CONTROL)

### 📤 SUBIR TUS CAMBIOS

```bash
# Paso 1: Ver qué has modificado
git status

# Paso 2: Agregar tus archivos
git add .

# Paso 3: Crear un commit (guardar cambios)
git commit -m "Descripción de lo que hiciste"

# Paso 4: Obtener cambios de compañeros primero
git pull origin main

# Paso 5: Subir tus cambios
git push origin main
```

### 📥 OBTENER CAMBIOS DE COMPAÑEROS

```bash
# Opción A: Si NO tienes cambios sin guardar
git pull origin main

# Opción B: Si TIENES cambios sin guardar
git stash              # Guarda temporalmente tus cambios
git pull origin main   # Descarga cambios
git stash pop          # Recupera tus cambios
```

---

## ⚠️ ¿Qué Hacer Si Hay CONFLICTOS?

Un conflicto ocurre cuando tú y un compañero modificaron las mismas líneas de código.

### Identificar conflictos:
```bash
git status
# Verás archivos marcados como "both modified"
```

### Resolver conflictos:

1. **Abre el archivo con conflicto** en tu editor

2. **Busca estas marcas**:
```cpp
<<<<<<< HEAD
// Tu código aquí
=======
// Código de tu compañero aquí
>>>>>>> origin/main
```

3. **Decide qué código mantener**:
   - Opción A: Mantener solo tu código
   - Opción B: Mantener solo el de tu compañero
   - Opción C: Combinar ambos códigos
   - **IMPORTANTE**: Elimina las líneas con `<<<<<<<`, `=======`, `>>>>>>>`

4. **Guarda el archivo**

5. **Marca como resuelto**:
```bash
git add archivo-resuelto.cpp
git commit -m "Resueltos conflictos"
git push origin main
```

---

## 🌿 RECOMENDACIÓN: Trabajar en Ramas

Para evitar conflictos, cada persona puede trabajar en su propia rama:

```bash
# Crear tu rama personal
git checkout -b andrea/mi-feature

# Hacer cambios y commits
git add .
git commit -m "Mi cambio"

# Subir tu rama
git push origin andrea/mi-feature

# Cuando termines, fusionar con main
git checkout main
git pull origin main
git merge andrea/mi-feature
git push origin main
```

---

## 📋 Comandos Útiles del Día a Día

```bash
# Ver en qué rama estás
git branch

# Ver historial de cambios
git log --oneline

# Ver diferencias antes de commit
git diff

# Descartar cambios en un archivo (¡CUIDADO!)
git checkout -- archivo.cpp

# Ver quién modificó cada línea
git blame archivo.cpp

# Crear una rama nueva
git checkout -b nombre-rama

# Cambiar de rama
git checkout nombre-rama

# Ver ramas remotas
git branch -r
```

---

## 🆘 Comandos de Emergencia

### Si quieres guardar cambios temporalmente:
```bash
git stash              # Guardar
git stash list         # Ver qué guardaste
git stash pop          # Recuperar último guardado
git stash apply        # Recuperar sin eliminar de la lista
```

### Si quieres deshacer el último commit (mantiene cambios):
```bash
git reset --soft HEAD~1
```

### Si quieres ver cambios de compañeros sin fusionar:
```bash
git fetch origin
git diff main origin/main
```

---

## 📊 Flujo de Trabajo Recomendado

```
┌─────────────────────────────────────────────────┐
│  CADA VEZ QUE VAYAS A TRABAJAR                  │
└─────────────────────────────────────────────────┘
                      ↓
         git pull origin main
         (Obtener cambios de compañeros)
                      ↓
         Hacer tus modificaciones
                      ↓
         git add .
         git commit -m "Descripción"
                      ↓
         git pull origin main
         (Por si hubo cambios nuevos)
                      ↓
         ¿Hay conflictos?
         ↙           ↘
       SÍ            NO
        ↓             ↓
   Resolver      git push origin main
   conflictos    (Subir cambios)
        ↓
   git add .
   git commit -m "Resueltos conflictos"
        ↓
   git push origin main
```

---

## 💡 Consejos Importantes

1. ✅ **Haz commits frecuentes** con mensajes claros
2. ✅ **Haz pull ANTES de push** para evitar conflictos
3. ✅ **Comunícate con tu equipo** sobre cambios importantes
4. ✅ **No subas archivos grandes** (ejecutables, DLLs, etc.)
5. ✅ **Revisa antes de commit** con `git status` y `git diff`
6. ✅ **Usa ramas** para features grandes
7. ✅ **Haz backup** de tu trabajo importante

---

## 🎓 Tu Caso Específico: Luces en Faroles

### Archivos que modificaste:
- `ProyectoFinal/dependencias/Window.h`
- `ProyectoFinal/dependencias/Window.cpp`
- `ProyectoFinal/Integrantes/Andrea/farola.h`
- `ProyectoFinal/main.cpp`
- `ProyectoFinal/documentacion/luces_faroles.md`

### Comandos exactos para ti:

```bash
# 1. Ver estado
git status

# 2. Agregar tus archivos
git add ProyectoFinal/dependencias/Window.h
git add ProyectoFinal/dependencias/Window.cpp
git add ProyectoFinal/Integrantes/Andrea/farola.h
git add ProyectoFinal/main.cpp
git add ProyectoFinal/documentacion/luces_faroles.md

# 3. Hacer commit
git commit -m "Agregadas luces spotlight en faroles con tecla L"

# 4. Obtener cambios de compañeros
git pull origin main

# 5. Subir tus cambios
git push origin main
```

---

## 📞 ¿Necesitas Ayuda?

- **Git no funciona**: Verifica que Git esté instalado con `git --version`
- **No puedes hacer push**: Verifica tus credenciales de GitHub
- **Conflictos complicados**: Pide ayuda a un compañero o usa una herramienta visual como GitHub Desktop
- **Perdiste cambios**: Revisa `git reflog` para recuperar commits

---

## 🔗 Recursos Adicionales

- [GitHub Desktop](https://desktop.github.com/) - Interfaz gráfica
- [Git Cheat Sheet](https://education.github.com/git-cheat-sheet-education.pdf)
- [Visualizing Git](https://git-school.github.io/visualizing-git/)

---

**¡Recuerda!** Git está diseñado para NO perder tu trabajo. Siempre hay forma de recuperar cambios. 💪
