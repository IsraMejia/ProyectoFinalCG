# 🚀 Subir Cambios a la Rama Andrea

## ✅ Opción 1: Usar el Script (MÁS FÁCIL)

**Doble clic en:** `subir_cambios.bat`

El script hará todo automáticamente:
- ✅ Cambiará a la rama Andrea
- ✅ Agregará tus archivos
- ✅ Creará un commit
- ✅ Descargará cambios de la rama Andrea
- ✅ Subirá tus cambios a la rama Andrea

---

## 💻 Opción 2: Comandos Manuales

Abre una terminal (Git Bash, PowerShell o CMD) y ejecuta:

```bash
# 1. Cambiar a la rama Andrea (o crearla si no existe localmente)
git checkout Andrea

# 2. Ver qué archivos modificaste
git status

# 3. Agregar tus archivos
git add ProyectoFinal/dependencias/Window.h
git add ProyectoFinal/dependencias/Window.cpp
git add ProyectoFinal/Integrantes/Andrea/farola.h
git add ProyectoFinal/main.cpp
git add ProyectoFinal/documentacion/luces_faroles.md

# 4. Crear un commit
git commit -m "Agregadas luces spotlight en faroles con tecla L"

# 5. Obtener cambios de la rama Andrea (por si hay actualizaciones)
git pull origin Andrea

# 6. Subir tus cambios a la rama Andrea
git push origin Andrea
```

---

## 🔄 Flujo de Trabajo con Tu Rama

```
┌─────────────────────────────────────┐
│  CADA VEZ QUE VAYAS A TRABAJAR      │
└─────────────────────────────────────┘
              ↓
    git checkout Andrea
    (Asegurarte de estar en tu rama)
              ↓
    git pull origin Andrea
    (Obtener actualizaciones)
              ↓
    Hacer tus modificaciones
              ↓
    git add .
    git commit -m "Descripción"
              ↓
    git pull origin Andrea
    (Por si hay cambios nuevos)
              ↓
    git push origin Andrea
    (Subir tus cambios)
```

---

## 📥 Ver Cambios de Compañeros (Otras Ramas)

### Ver cambios de Isra:
```bash
git checkout Isra
git pull origin Isra
```

### Ver cambios de main:
```bash
git checkout main
git pull origin main
```

### Volver a tu rama:
```bash
git checkout Andrea
```

---

## 🔀 Cuando Quieras Fusionar con Main

Cuando tu trabajo esté listo y quieras integrarlo a la rama principal:

```bash
# 1. Asegúrate de que tu rama esté actualizada
git checkout Andrea
git add .
git commit -m "Últimos cambios"
git push origin Andrea

# 2. Ve a GitHub y crea un Pull Request
# Desde la rama Andrea hacia main
```

O si quieres hacerlo localmente:

```bash
# 1. Actualizar main
git checkout main
git pull origin main

# 2. Fusionar tu rama
git merge Andrea

# 3. Resolver conflictos si los hay

# 4. Subir a main
git push origin main
```

---

## ⚠️ Si Hay Conflictos

Si al hacer `git pull origin Andrea` aparece un conflicto:

1. **Abre el archivo** con conflicto
2. **Busca** las líneas con `<<<<<<<`, `=======`, `>>>>>>>`
3. **Edita** para quedarte con el código correcto
4. **Elimina** las marcas de conflicto
5. **Guarda** el archivo
6. Ejecuta:
```bash
git add .
git commit -m "Resueltos conflictos"
git push origin Andrea
```

---

## 📊 Comandos Útiles para Tu Rama

```bash
# Ver en qué rama estás
git branch

# Ver todas las ramas (locales y remotas)
git branch -a

# Ver historial de tu rama
git log --oneline

# Ver diferencias con main
git diff main..Andrea

# Ver diferencias con la rama de Isra
git diff Isra..Andrea

# Crear una nueva rama desde Andrea
git checkout -b Andrea/nueva-feature
```

---

## 💡 Ventajas de Trabajar en Tu Rama

✅ **No afectas el trabajo de otros** - Cada quien en su rama
✅ **Puedes experimentar** sin romper el código principal
✅ **Fácil de revisar** - Los cambios están aislados
✅ **Menos conflictos** - Solo fusionas cuando estés lista

---

## 🎯 Resumen Rápido

### Para subir cambios:
```bash
git checkout Andrea
git add .
git commit -m "Descripción"
git push origin Andrea
```

### Para obtener actualizaciones:
```bash
git checkout Andrea
git pull origin Andrea
```

### Para ver otras ramas:
```bash
git checkout nombre-rama
git pull origin nombre-rama
```

---

## 🆘 Ayuda Rápida

**¿En qué rama estoy?**
```bash
git branch
# La rama actual tiene un asterisco (*)
```

**¿Cómo vuelvo a mi rama?**
```bash
git checkout Andrea
```

**¿Cómo veo qué cambié?**
```bash
git status
git diff
```

**¿Cómo descarto cambios que no quiero?**
```bash
git checkout -- archivo.cpp
```

---

**¡Listo!** Ahora puedes trabajar tranquila en tu rama Andrea sin afectar a tus compañeros. 🎉
