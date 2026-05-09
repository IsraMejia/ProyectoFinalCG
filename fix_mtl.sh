#!/bin/bash

# CageFreddy
sed -i 's|texturesCageFreddy/||g' "ProyectoFinal/Integrantes/Andrea/modelos/CageFreddy/CageFreddy.mtl"
echo "✓ CageFreddy.mtl"

# Escenario  
sed -i 's|texturesStage/||g' "ProyectoFinal/Integrantes/Andrea/modelos/Escenario/EscenarioCompleto.mtl"
echo "✓ EscenarioCompleto.mtl"

# FuntimeFoxy
sed -i 's|texturesFoxy/||g' "ProyectoFinal/Integrantes/Andrea/modelos/FuntimeFoxy/FuntimeFoxyOptimizado.mtl"
echo "✓ FuntimeFoxyOptimizado.mtl"

# SalaMonitores
sed -i 's|textures/||g' "ProyectoFinal/Integrantes/Andrea/modelos/SalaMonitores/SalaMonitores.mtl"
echo "✓ SalaMonitores.mtl"

# Speakers
sed -i 's|texturesSpeakers/||g' "ProyectoFinal/Integrantes/Andrea/modelos/Speakers/Speakers.mtl"
echo "✓ Speakers.mtl"

echo ""
echo "✓ Todos los archivos .mtl han sido actualizados"
