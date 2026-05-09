#!/usr/bin/env python3
import re

# Lista de archivos .mtl a modificar
mtl_files = [
    "ProyectoFinal/Integrantes/Andrea/modelos/CageFreddy/CageFreddy.mtl",
    "ProyectoFinal/Integrantes/Andrea/modelos/Escenario/EscenarioCompleto.mtl",
    "ProyectoFinal/Integrantes/Andrea/modelos/FuntimeFoxy/FuntimeFoxyOptimizado.mtl",
    "ProyectoFinal/Integrantes/Andrea/modelos/SalaMonitores/SalaMonitores.mtl",
    "ProyectoFinal/Integrantes/Andrea/modelos/Speakers/Speakers.mtl"
]

# Patrones a eliminar
patterns = [
    'texturesBallora/',
    'texturesCageFreddy/',
    'texturesStage/',
    'texturesFoxy/',
    'textures/',
    'texturesSpeakers/'
]

for mtl_file in mtl_files:
    try:
        with open(mtl_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Eliminar todos los prefijos
        for pattern in patterns:
            content = content.replace(pattern, '')
        
        with open(mtl_file, 'w', encoding='utf-8') as f:
            f.write(content)
        
        print(f"✓ Modificado: {mtl_file}")
    except Exception as e:
        print(f"✗ Error en {mtl_file}: {e}")

print("\n✓ Proceso completado")
