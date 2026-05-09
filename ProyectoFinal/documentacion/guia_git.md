# Guía de Git para Trabajo Colaborativo

## 🎯 Objetivo
Subir tus cambios a GitHub y obtener los cambios de tus compañeros sin perder tu trabajo.

## 📝 Flujo de Trabajo Básico

### 1. Verificar el Estado Actual
Antes de hacer cualquier cosa, verifica qué archivos has modificado:

```bash
git status
```

### 2. Guardar tus Cambios Localmente (Commit)

#### Paso 2.1: Agregar archivos al staging
```bash
# Agregar archivos específicos
git add ProyectoFinal/dependencias/Window.h
git add ProyectoFinal/dependencias/Window.cpp
git add ProyectoFinal/Integrantes/Andrea/farola.h
git add ProyectoFinal/main.cpp
git add ProyectoFinal/documentacion/luces_faroles.md

# O agregar todos los cambios de una vez
git add .
```

#### Paso 2.2: Crear un commit con mensaje descriptivo
```bash
git commit -m "Agregadas luces spotlight en faroles con tecla L"
```

### 3. Obtener Cambios de tus Compañeros

#### Opción A: Pull (Recomendado para principiantes)
```bash
# Descargar y fusionar cambios automáticamente
git pull origin main
```

Si hay conflictos, Git te avisará y tendrás que resolverlos manualmente.

#### Opción B: Fetch + Merge (Más control)
```bash
# Descargar cambios sin fusionar
git fetch origin

# Ver qué cambios hay
git log origin/main

# Fusionar cambios
git merge origin/main
```

### 4. Subir tus Cambios a GitHub
```bash
# Subir a la rama main
git push origin main

# O si trabajas en una rama específica
git push origin nombre-de-tu-rama
```

## 🔄 Flujo Completo Recomendado

```bash
# 1. Ver qué has cambiado
git status

# 2. Guardar tus cambios
git add .
git commit -m "Descripción de tus cambios"

# 3. Obtener cambios de compañeros
git pull origin main

# 4. Si hay conflictos, resolverlos (ver sección abajo)

# 5. Subir tus cambios
git push origin main
```

## ⚠️ Resolver Conflictos

Si Git encuentra conflictos al hacer `pull`, verás algo como:

```
CONFLICT (content): Merge conflict in ProyectoFinal/main.cpp
```

### Pasos para resolver:

1. **Abrir el archivo con conflicto** en tu editor
2. **Buscar las marcas de conflicto**:
```cpp
<<<<<<< HEAD
// Tu código
=======
// Código de tu compañero
>>>>>>> origin/main
```

3. **Decidir qué código mantener**:
   - Puedes mantener tu código
   - Puedes mantener el de tu compañero
   - Puedes combinar ambos
   - Elimina las marcas `<<<<<<<`, `=======`, `>>>>>>>`

4. **Guardar el archivo**

5. **Marcar como resuelto y continuar**:
```bash
git add archivo-resuelto.cpp
git commit -m "Resueltos conflictos en main.cpp"
git push origin main
```

## 🌿 Trabajar con Ramas (Recomendado para Equipos)

### Crear tu propia rama
```bash
# Crear y cambiar a una nueva rama
git checkout -b andrea/luces-faroles

# Hacer tus cambios y commits
git add .
git commit -m "Agregadas luces spotlight"

# Subir tu rama
git push origin andrea/luces-faroles
```

### Ventajas de usar ramas:
- ✅ No afectas el código de tus compañeros
- ✅ Puedes trabajar en múltiples features
- ✅ Más fácil revisar cambios antes de integrar

### Integrar tu rama a main
```bash
# Cambiar a main
git checkout main

# Actualizar main
git pull origin main

# Fusionar tu rama
git merge andrea/luces-faroles

# Subir a GitHub
git push origin main
```

## 🚨 Comandos de Emergencia

### Si hiciste cambios y quieres guardarlos temporalmente
```bash
# Guardar cambios sin commit
git stash

# Hacer pull
git pull origin main

# Recuperar tus cambios
git stash pop
```

### Si quieres descartar todos tus cambios locales (¡CUIDADO!)
```bash
# Ver qué se descartará
git status

# Descartar cambios en archivos específicos
git checkout -- archivo.cpp

# Descartar TODOS los cambios (¡NO RECUPERABLE!)
git reset --hard HEAD
```

### Si subiste algo por error
```bash
# Deshacer el último commit (mantiene cambios)
git reset --soft HEAD~1

# Deshacer el último commit (descarta cambios)
git reset --hard HEAD~1
```

## 📊 Comandos Útiles

```bash
# Ver historial de commits
git log

# Ver historial resumido
git log --oneline

# Ver quién modificó cada línea
git blame archivo.cpp

# Ver diferencias antes de commit
git diff

# Ver diferencias de un archivo específico
git diff archivo.cpp

# Ver ramas
git branch

# Ver ramas remotas
git branch -r

# Cambiar de rama
git checkout nombre-rama
```

## 💡 Mejores Prácticas

1. **Hacer commits frecuentes** con mensajes descriptivos
2. **Hacer pull antes de push** para evitar conflictos
3. **Revisar cambios antes de commit** con `git status` y `git diff`
4. **Usar ramas** para features nuevas
5. **Comunicarse con el equipo** sobre cambios importantes
6. **No subir archivos grandes** (binarios, DLLs, ejecutables)
7. **Usar .gitignore** para excluir archivos temporales

## 🎓 Ejemplo Práctico: Tu Caso

```bash
# 1. Ver tus cambios actuales
git status

# 2. Agregar tus archivos modificados
git add ProyectoFinal/dependencias/Window.h
git add ProyectoFinal/dependencias/Window.cpp
git add ProyectoFinal/Integrantes/Andrea/farola.h
git add ProyectoFinal/main.cpp
git add ProyectoFinal/documentacion/luces_faroles.md

# 3. Hacer commit
git commit -m "Agregadas luces spotlight en faroles controladas con tecla L"

# 4. Obtener cambios de compañeros
git pull origin main

# 5. Si no hay conflictos, subir
git push origin main

# 6. Si hay conflictos, resolverlos y luego:
git add .
git commit -m "Resueltos conflictos"
git push origin main
```

## 🔗 Recursos Adicionales

- [Git Cheat Sheet](https://education.github.com/git-cheat-sheet-education.pdf)
- [GitHub Desktop](https://desktop.github.com/) - Interfaz gráfica para Git
- [Visualizing Git](https://git-school.github.io/visualizing-git/) - Herramienta visual

## ❓ Preguntas Frecuentes

**P: ¿Qué pasa si dos personas modifican el mismo archivo?**
R: Git intentará fusionar automáticamente. Si modificaron las mismas líneas, habrá un conflicto que deberás resolver manualmente.

**P: ¿Puedo ver los cambios antes de hacer pull?**
R: Sí, usa `git fetch origin` y luego `git diff main origin/main`

**P: ¿Cómo sé en qué rama estoy?**
R: Usa `git branch` (la rama actual tiene un asterisco *)

**P: ¿Puedo deshacer un push?**
R: Técnicamente sí, pero es complicado y puede afectar a tus compañeros. Mejor hacer un nuevo commit que corrija el error.
