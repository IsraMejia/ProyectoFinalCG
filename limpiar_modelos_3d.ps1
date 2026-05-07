# ============================================================
# Script PowerShell para limpiar modelos 3D del historial de Git
# ============================================================

# Configurar para detener en errores
$ErrorActionPreference = "Stop"

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "Limpieza de Modelos 3D del Historial Git" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""

# Funciones para output con colores
function Write-Warning-Custom {
    param([string]$Message)
    Write-Host "⚠️  $Message" -ForegroundColor Yellow
}

function Write-Success {
    param([string]$Message)
    Write-Host "✓ $Message" -ForegroundColor Green
}

function Write-Error-Custom {
    param([string]$Message)
    Write-Host "✗ $Message" -ForegroundColor Red
}

# Verificar que estamos en un repositorio Git
if (-not (Test-Path .git)) {
    Write-Error-Custom "Este directorio no es un repositorio Git"
    exit 1
}

# Mostrar advertencia
Write-Warning-Custom "ADVERTENCIA: Esta operación reescribirá el historial de Git"
Write-Warning-Custom "Todos los colaboradores deberán clonar el repositorio de nuevo"
Write-Host ""
$confirm = Read-Host "¿Deseas continuar? (escribe 'SI' para confirmar)"

if ($confirm -ne "SI") {
    Write-Host "Operación cancelada"
    exit 0
}

# Paso 1: Mostrar tamaño actual
Write-Host ""
Write-Host "📊 Tamaño actual del repositorio:" -ForegroundColor Cyan
git count-objects -vH

# Paso 2: Verificar si git-filter-repo está instalado
Write-Host ""
Write-Host "🔍 Verificando git-filter-repo..." -ForegroundColor Cyan

$filterRepoInstalled = $false
try {
    $null = git filter-repo --version 2>$null
    $filterRepoInstalled = $true
    Write-Success "git-filter-repo está instalado"
} catch {
    Write-Warning-Custom "git-filter-repo no está instalado"
    Write-Host "Intentando instalar git-filter-repo..."
    
    # Intentar instalar con pip
    try {
        pip install git-filter-repo
        Write-Success "git-filter-repo instalado con pip"
        $filterRepoInstalled = $true
    } catch {
        try {
            pip3 install git-filter-repo
            Write-Success "git-filter-repo instalado con pip3"
            $filterRepoInstalled = $true
        } catch {
            Write-Error-Custom "No se pudo instalar git-filter-repo automáticamente"
            Write-Host "Por favor, instálalo manualmente: pip install git-filter-repo"
            exit 1
        }
    }
}

# Paso 3: Hacer backup del remoto
Write-Host ""
Write-Host "💾 Guardando información del remoto..." -ForegroundColor Cyan
try {
    $remoteUrl = git remote get-url origin 2>$null
    Write-Success "Remoto guardado: $remoteUrl"
} catch {
    Write-Warning-Custom "No se encontró un remoto 'origin'"
    $remoteUrl = ""
}

# Paso 4: Limpiar carpetas y archivos
Write-Host ""
Write-Host "🧹 Limpiando archivos del historial..." -ForegroundColor Cyan

# Limpiar carpetas
Write-Host "  - Limpiando ProyectoFinal/Models..."
try { git filter-repo --path ProyectoFinal/Models --invert-paths --force 2>$null } catch {}

Write-Host "  - Limpiando ProyectoFinal/Textures..."
try { git filter-repo --path ProyectoFinal/Textures --invert-paths --force 2>$null } catch {}

# Limpiar archivos por extensión
Write-Host "  - Limpiando archivos .obj..."
try { git filter-repo --path-glob '*.obj' --invert-paths --force 2>$null } catch {}

Write-Host "  - Limpiando archivos .mtl..."
try { git filter-repo --path-glob '*.mtl' --invert-paths --force 2>$null } catch {}

Write-Host "  - Limpiando archivos .3ds..."
try { git filter-repo --path-glob '*.3ds' --invert-paths --force 2>$null } catch {}

Write-Host "  - Limpiando archivos de imagen..."
try { git filter-repo --path-glob '*.jpg' --invert-paths --force 2>$null } catch {}
try { git filter-repo --path-glob '*.jpeg' --invert-paths --force 2>$null } catch {}
try { git filter-repo --path-glob '*.png' --invert-paths --force 2>$null } catch {}
try { git filter-repo --path-glob '*.tga' --invert-paths --force 2>$null } catch {}
try { git filter-repo --path-glob '*.bmp' --invert-paths --force 2>$null } catch {}
try { git filter-repo --path-glob '*.dds' --invert-paths --force 2>$null } catch {}

Write-Success "Archivos limpiados del historial"

# Paso 5: Limpiar referencias y garbage collection
Write-Host ""
Write-Host "🗑️  Limpiando referencias y ejecutando garbage collection..." -ForegroundColor Cyan

# Eliminar referencias antiguas si existen
if (Test-Path .git/refs/original) {
    Remove-Item -Recurse -Force .git/refs/original/
}

# Expirar reflogs
git reflog expire --expire=now --all

# Garbage collection agresivo
git gc --prune=now --aggressive

Write-Success "Limpieza completada"

# Paso 6: Mostrar nuevo tamaño
Write-Host ""
Write-Host "📊 Nuevo tamaño del repositorio:" -ForegroundColor Cyan
git count-objects -vH

# Paso 7: Restaurar remoto
if ($remoteUrl -ne "") {
    Write-Host ""
    Write-Host "🔗 Restaurando remoto..." -ForegroundColor Cyan
    try {
        git remote add origin $remoteUrl 2>$null
        Write-Success "Remoto restaurado"
    } catch {
        # El remoto ya existe, no hacer nada
    }
}

# Paso 8: Instrucciones finales
Write-Host ""
Write-Host "==========================================" -ForegroundColor Green
Write-Host "✅ Limpieza completada exitosamente" -ForegroundColor Green
Write-Host "==========================================" -ForegroundColor Green
Write-Host ""
Write-Host "📋 Próximos pasos:" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. Revisar los cambios:"
Write-Host "   git status"
Write-Host ""
Write-Host "2. Forzar push al repositorio remoto:"
Write-Host "   git push origin --force --all"
Write-Host "   git push origin --force --tags"
Write-Host ""
Write-Host "3. Notificar a todos los colaboradores que deben:"
Write-Host "   - Hacer backup de sus cambios locales"
Write-Host "   - Eliminar su copia local del repositorio"
Write-Host "   - Clonar de nuevo el repositorio"
Write-Host ""
Write-Warning-Custom "IMPORTANTE: No olvides hacer el push forzado para actualizar el remoto"
Write-Host ""
