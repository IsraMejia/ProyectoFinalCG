# Guía para Limpiar Archivos Pesados del Historial de Git

## ⚠️ ADVERTENCIA IMPORTANTE
Esta operación reescribe el historial de Git. Todos los colaboradores deberán hacer un `git clone` fresco después de completar estos pasos.

---

## Paso 1: Verificar el tamaño actual del repositorio

```bash
# Ver el tamaño del repositorio
git count-objects -vH

# Ver los archivos más grandes en el historial
git rev-list --objects --all | \
  git cat-file --batch-check='%(objecttype) %(objectname) %(objectsize) %(rest)' | \
  sed -n 's/^blob //p' | \
  sort --numeric-sort --key=2 | \
  tail -n 20
```

---

## Paso 2: Hacer backup del repositorio

```bash
# Crear un backup completo antes de proceder
cd ..
cp -r ProyectoFinal ProyectoFinal_backup
cd ProyectoFinal
```

---

## Paso 3: Instalar git-filter-repo (Recomendado)

### Opción A: Usando pip (Python)
```bash
pip install git-filter-repo
```

### Opción B: Descarga manual
```bash
# Descargar el script
curl -O https://raw.githubusercontent.com/newren/git-filter-repo/main/git-filter-repo

# Hacerlo ejecutable (Linux/Mac)
chmod +x git-filter-repo

# Moverlo a un directorio en PATH (Linux/Mac)
sudo mv git-filter-repo /usr/local/bin/
```

### Opción C: En Windows con Git Bash
```bash
# Descargar y colocar en la carpeta de Git
curl -O https://raw.githubusercontent.com/newren/git-filter-repo/main/git-filter-repo
mv git-filter-repo "C:/Program Files/Git/cmd/git-filter-repo"
```

---

## Paso 4: Limpiar archivos del historial

### Método 1: Usando git-filter-repo (RECOMENDADO - Más rápido y seguro)

```bash
# Asegurarse de estar en la rama principal
git checkout main

# Limpiar carpetas completas
git filter-repo --path ProyectoFinal/Models --invert-paths --force
git filter-repo --path ProyectoFinal/Textures --invert-paths --force

# Limpiar archivos por extensión en todo el repositorio
git filter-repo --path-glob '*.obj' --invert-paths --force
git filter-repo --path-glob '*.mtl' --invert-paths --force
git filter-repo --path-glob '*.3ds' --invert-paths --force
git filter-repo --path-glob '*.jpg' --invert-paths --force
git filter-repo --path-glob '*.jpeg' --invert-paths --force
git filter-repo --path-glob '*.png' --invert-paths --force
git filter-repo --path-glob '*.tga' --invert-paths --force
git filter-repo --path-glob '*.bmp' --invert-paths --force
git filter-repo --path-glob '*.dds' --invert-paths --force
```

### Método 2: Usando BFG Repo-Cleaner (Alternativa)

```bash
# Descargar BFG (requiere Java)
# https://rtyley.github.io/bfg-repo-cleaner/

# Limpiar archivos grandes (ejemplo: mayores a 10MB)
java -jar bfg.jar --strip-blobs-bigger-than 10M

# O limpiar carpetas específicas
java -jar bfg.jar --delete-folders Models
java -jar bfg.jar --delete-folders Textures
```

### Método 3: Usando git filter-branch (Más lento, pero disponible por defecto)

```bash
# ADVERTENCIA: Este método es más lento y puede tomar mucho tiempo

# Limpiar carpetas
git filter-branch --force --index-filter \
  "git rm -rf --cached --ignore-unmatch ProyectoFinal/Models ProyectoFinal/Textures" \
  --prune-empty --tag-name-filter cat -- --all

# Limpiar archivos por extensión
git filter-branch --force --index-filter \
  "git rm -rf --cached --ignore-unmatch '*.obj' '*.mtl' '*.jpg' '*.jpeg' '*.png' '*.tga' '*.3ds'" \
  --prune-empty --tag-name-filter cat -- --all
```

---

## Paso 5: Limpiar referencias y recolectar basura

```bash
# Eliminar referencias al historial antiguo
rm -rf .git/refs/original/

# Expirar todos los reflogs
git reflog expire --expire=now --all

# Ejecutar garbage collection agresivo
git gc --prune=now --aggressive

# Verificar el nuevo tamaño
git count-objects -vH
```

---

## Paso 6: Forzar push al repositorio remoto

⚠️ **ADVERTENCIA**: Esto sobrescribirá el historial remoto. Coordina con tu equipo.

```bash
# Agregar el remoto si fue eliminado por filter-repo
git remote add origin <URL_DEL_REPOSITORIO>

# Forzar push de todas las ramas
git push origin --force --all

# Forzar push de todos los tags
git push origin --force --tags
```

---

## Paso 7: Limpiar en todas las ramas

Si tienes múltiples ramas (por ejemplo, `main` y `develop`):

```bash
# Cambiar a cada rama y repetir el proceso
git checkout develop
git filter-repo --path ProyectoFinal/Models --invert-paths --force
git filter-repo --path ProyectoFinal/Textures --invert-paths --force

# Después de limpiar todas las ramas, hacer push
git push origin --force --all
```

---

## Paso 8: Instrucciones para colaboradores

Después de limpiar el historial, todos los colaboradores deben:

```bash
# 1. Hacer backup de cambios locales no commiteados
git stash

# 2. Eliminar el repositorio local
cd ..
rm -rf ProyectoFinal

# 3. Clonar de nuevo el repositorio limpio
git clone <URL_DEL_REPOSITORIO>
cd ProyectoFinal

# 4. Restaurar cambios locales si los había
git stash pop
```

---

## Paso 9: Verificación final

```bash
# Verificar que los archivos ya no están en el historial
git log --all --full-history -- "ProyectoFinal/Models/*"
git log --all --full-history -- "*.obj"

# Verificar el tamaño final
git count-objects -vH

# Verificar que el .gitignore está funcionando
git status
# No deberían aparecer archivos de Models/ o Textures/
```

---

## Solución de Problemas

### Error: "refusing to run without --force"
```bash
# Agregar --force al comando git-filter-repo
git filter-repo --path ProyectoFinal/Models --invert-paths --force
```

### Error: "remote origin already exists"
```bash
# Eliminar y volver a agregar el remoto
git remote remove origin
git remote add origin <URL_DEL_REPOSITORIO>
```

### Los archivos siguen apareciendo en el historial
```bash
# Asegurarse de limpiar TODAS las ramas
git branch -a  # Ver todas las ramas
# Repetir el proceso de limpieza en cada rama
```

---

## Alternativa: Crear un nuevo repositorio limpio

Si la limpieza es muy complicada, considera:

1. Crear un nuevo repositorio en GitHub
2. Copiar solo los archivos de código (sin .git)
3. Inicializar un nuevo repositorio con el .gitignore actualizado
4. Hacer el primer commit con el código limpio

```bash
# En una nueva carpeta
cp -r ProyectoFinal ProyectoFinal_limpio
cd ProyectoFinal_limpio
rm -rf .git
git init
git add .
git commit -m "Repositorio limpio sin modelos 3D"
git remote add origin <URL_NUEVO_REPOSITORIO>
git push -u origin main
```

---

## Notas Importantes

1. **Comunicación**: Avisa a todos los colaboradores antes de hacer esto
2. **Backup**: Siempre mantén un backup del repositorio original
3. **Timing**: Hazlo cuando nadie esté trabajando activamente
4. **Documentación**: Guarda los modelos 3D en un lugar externo (Google Drive, Dropbox, etc.)
5. **README**: Actualiza el README con instrucciones sobre dónde obtener los modelos 3D

---

## Recursos Adicionales

- [git-filter-repo documentation](https://github.com/newren/git-filter-repo)
- [BFG Repo-Cleaner](https://rtyley.github.io/bfg-repo-cleaner/)
- [GitHub: Removing sensitive data](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/removing-sensitive-data-from-a-repository)
