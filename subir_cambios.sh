#!/bin/bash
# Script para subir cambios a GitHub de forma segura

echo "==================================="
echo "  SUBIR CAMBIOS A GITHUB"
echo "==================================="
echo ""

# 1. Mostrar archivos modificados
echo "📝 Archivos modificados:"
git status --short
echo ""

# 2. Agregar archivos al staging
echo "➕ Agregando archivos..."
git add ProyectoFinal/dependencias/Window.h
git add ProyectoFinal/dependencias/Window.cpp
git add ProyectoFinal/Integrantes/Andrea/farola.h
git add ProyectoFinal/Integrantes/Andrea/farola.cpp
git add ProyectoFinal/main.cpp
git add ProyectoFinal/documentacion/luces_faroles.md
git add ProyectoFinal/documentacion/guia_git.md
echo "✅ Archivos agregados"
echo ""

# 3. Crear commit
echo "💾 Creando commit..."
git commit -m "Agregadas luces spotlight en faroles controladas con tecla L"
echo "✅ Commit creado"
echo ""

# 4. Obtener cambios de compañeros
echo "⬇️  Obteniendo cambios de compañeros..."
git pull origin main
echo ""

# 5. Verificar si hay conflictos
if [ $? -eq 0 ]; then
    echo "✅ No hay conflictos"
    echo ""
    
    # 6. Subir cambios
    echo "⬆️  Subiendo cambios a GitHub..."
    git push origin main
    
    if [ $? -eq 0 ]; then
        echo ""
        echo "🎉 ¡Cambios subidos exitosamente!"
    else
        echo ""
        echo "❌ Error al subir cambios"
        echo "Verifica tu conexión y permisos"
    fi
else
    echo "⚠️  HAY CONFLICTOS - Debes resolverlos manualmente"
    echo ""
    echo "Pasos para resolver:"
    echo "1. Abre los archivos con conflicto"
    echo "2. Busca las marcas <<<<<<< ======= >>>>>>>"
    echo "3. Edita y guarda los archivos"
    echo "4. Ejecuta:"
    echo "   git add ."
    echo "   git commit -m 'Resueltos conflictos'"
    echo "   git push origin main"
fi

echo ""
echo "==================================="
