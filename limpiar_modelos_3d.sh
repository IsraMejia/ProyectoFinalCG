#!/bin/bash

# ============================================================
# Script para limpiar modelos 3D del historial de Git
# ============================================================

set -e  # Salir si hay algún error

echo "=========================================="
echo "Limpieza de Modelos 3D del Historial Git"
echo "=========================================="
echo ""

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Función para mostrar advertencias
warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

# Función para mostrar éxito
success() {
    echo -e "${GREEN}✓ $1${NC}"
}

# Función para mostrar errores
error() {
    echo -e "${RED}✗ $1${NC}"
}

# Verificar que estamos en un repositorio Git
if [ ! -d .git ]; then
    error "Este directorio no es un repositorio Git"
    exit 1
fi

# Mostrar advertencia
warning "ADVERTENCIA: Esta operación reescribirá el historial de Git"
warning "Todos los colaboradores deberán clonar el repositorio de nuevo"
echo ""
read -p "¿Deseas continuar? (escribe 'SI' para confirmar): " confirm

if [ "$confirm" != "SI" ]; then
    echo "Operación cancelada"
    exit 0
fi

# Paso 1: Mostrar tamaño actual
echo ""
echo "📊 Tamaño actual del repositorio:"
git count-objects -vH

# Paso 2: Verificar si git-filter-repo está instalado
echo ""
echo "🔍 Verificando git-filter-repo..."
if ! command -v git-filter-repo &> /dev/null; then
    warning "git-filter-repo no está instalado"
    echo "Instalando git-filter-repo..."
    
    # Intentar instalar con pip
    if command -v pip &> /dev/null; then
        pip install git-filter-repo
        success "git-filter-repo instalado con pip"
    elif command -v pip3 &> /dev/null; then
        pip3 install git-filter-repo
        success "git-filter-repo instalado con pip3"
    else
        error "No se pudo instalar git-filter-repo automáticamente"
        echo "Por favor, instálalo manualmente: pip install git-filter-repo"
        exit 1
    fi
else
    success "git-filter-repo está instalado"
fi

# Paso 3: Hacer backup del remoto
echo ""
echo "💾 Guardando información del remoto..."
REMOTE_URL=$(git remote get-url origin 2>/dev/null || echo "")
if [ -z "$REMOTE_URL" ]; then
    warning "No se encontró un remoto 'origin'"
else
    success "Remoto guardado: $REMOTE_URL"
fi

# Paso 4: Limpiar carpetas y archivos
echo ""
echo "🧹 Limpiando archivos del historial..."

# Limpiar carpetas
echo "  - Limpiando ProyectoFinal/Models..."
git filter-repo --path ProyectoFinal/Models --invert-paths --force 2>/dev/null || true

echo "  - Limpiando ProyectoFinal/Textures..."
git filter-repo --path ProyectoFinal/Textures --invert-paths --force 2>/dev/null || true

# Limpiar archivos por extensión
echo "  - Limpiando archivos .obj..."
git filter-repo --path-glob '*.obj' --invert-paths --force 2>/dev/null || true

echo "  - Limpiando archivos .mtl..."
git filter-repo --path-glob '*.mtl' --invert-paths --force 2>/dev/null || true

echo "  - Limpiando archivos .3ds..."
git filter-repo --path-glob '*.3ds' --invert-paths --force 2>/dev/null || true

echo "  - Limpiando archivos de imagen..."
git filter-repo --path-glob '*.jpg' --invert-paths --force 2>/dev/null || true
git filter-repo --path-glob '*.jpeg' --invert-paths --force 2>/dev/null || true
git filter-repo --path-glob '*.png' --invert-paths --force 2>/dev/null || true
git filter-repo --path-glob '*.tga' --invert-paths --force 2>/dev/null || true
git filter-repo --path-glob '*.bmp' --invert-paths --force 2>/dev/null || true
git filter-repo --path-glob '*.dds' --invert-paths --force 2>/dev/null || true

success "Archivos limpiados del historial"

# Paso 5: Limpiar referencias y garbage collection
echo ""
echo "🗑️  Limpiando referencias y ejecutando garbage collection..."

# Eliminar referencias antiguas si existen
if [ -d .git/refs/original ]; then
    rm -rf .git/refs/original/
fi

# Expirar reflogs
git reflog expire --expire=now --all

# Garbage collection agresivo
git gc --prune=now --aggressive

success "Limpieza completada"

# Paso 6: Mostrar nuevo tamaño
echo ""
echo "📊 Nuevo tamaño del repositorio:"
git count-objects -vH

# Paso 7: Restaurar remoto
if [ -n "$REMOTE_URL" ]; then
    echo ""
    echo "🔗 Restaurando remoto..."
    git remote add origin "$REMOTE_URL" 2>/dev/null || true
    success "Remoto restaurado"
fi

# Paso 8: Instrucciones finales
echo ""
echo "=========================================="
echo "✅ Limpieza completada exitosamente"
echo "=========================================="
echo ""
echo "📋 Próximos pasos:"
echo ""
echo "1. Revisar los cambios:"
echo "   git status"
echo ""
echo "2. Forzar push al repositorio remoto:"
echo "   git push origin --force --all"
echo "   git push origin --force --tags"
echo ""
echo "3. Notificar a todos los colaboradores que deben:"
echo "   - Hacer backup de sus cambios locales"
echo "   - Eliminar su copia local del repositorio"
echo "   - Clonar de nuevo el repositorio"
echo ""
warning "IMPORTANTE: No olvides hacer el push forzado para actualizar el remoto"
echo ""
