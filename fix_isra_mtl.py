#!/usr/bin/env python3
import os
import glob

# Buscar todos los archivos .mtl en la carpeta de Isra
mtl_files = glob.glob("ProyectoFinal/Integrantes/Isra/modelos/**/*.mtl", recursive=True)

for mtl_file in mtl_files:
    try:
        with open(mtl_file, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # Corregir rutas duplicadas
        content = content.replace('../Textures/', '')
        content = content.replace('Textures/textures/', '')
        content = content.replace('textures/', '')
        
        # Corregir extensiones
        content = content.replace('.PNG', '.png')
        content = content.replace('.JPEG', '.jpeg')
        content = content.replace('.JPG', '.jpg')
        
        with open(mtl_file, 'w', encoding='utf-8') as f:
            f.write(content)
        
        print(f"✓ Corregido: {mtl_file}")
    except Exception as e:
        print(f"✗ Error en {mtl_file}: {e}")

print("\n✓ Proceso completado")
