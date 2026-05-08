# ✅ Solución Simple: Subir a Rama Andrea

## 🎯 Situación Actual
Estás en medio de un cherry-pick con conflictos. La solución más simple es:

1. **Abortar el cherry-pick**
2. **Hacer los cambios directamente en la rama Andrea**
3. **Subir a GitHub**

## 📝 Comandos para Ejecutar

### Paso 1: Abortar el cherry-pick
Abre una terminal **NUEVA** (Git Bash o PowerShell) y ejecuta:

```bash
cd "C:\Users\andre\Desktop\ProyectoFinalCG\ProyectoFinal"
git cherry-pick --abort
```

### Paso 2: Verificar que estás en rama Andrea
```bash
git branch
# Deberías ver: * Andrea
```

### Paso 3: Subir lo que ya tienes en Andrea
```bash
git push origin Andrea
```

## 🔄 Alternativa: Empezar Limpio

Si lo anterior no funciona, puedes empezar limpio:

```bash
# 1. Abortar todo
git cherry-pick --abort
git reset --hard origin/Andrea

# 2. Hacer los cambios de las luces manualmente
# (Los archivos ya están modificados localmente)

# 3. Agregar y commit
git add ProyectoFinal/dependencias/Window.h
git add ProyectoFinal/dependencias/Window.cpp  
git add ProyectoFinal/Integrantes/Andrea/farola.h
git add ProyectoFinal/main.cpp
git add ProyectoFinal/documentacion/luces_faroles.md

git commit -m "Agregadas luces spotlight en faroles con tecla L"

# 4. Subir
git push origin Andrea
```

## 🆘 Si Nada Funciona

Cierra TODAS las ventanas de terminal y editores, luego:

1. Abre una terminal NUEVA
2. Navega al proyecto:
```bash
cd "C:\Users\andre\Desktop\ProyectoFinalCG\ProyectoFinal"
```

3. Ejecuta:
```bash
git cherry-pick --abort
git status
```

4. Dime qué dice `git status` y te ayudo desde ahí.

---

**IMPORTANTE**: Si ves un editor de vim abierto, presiona:
- `ESC` (tecla escape)
- Luego escribe: `:q!`
- Presiona `ENTER`

Esto cerrará el editor sin guardar cambios.
