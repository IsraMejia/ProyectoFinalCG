#!/bin/bash

# Mover todas las texturas de Textures/Textures/ a Textures/
mv ProyectoFinal/Textures/Textures/* ProyectoFinal/Textures/ 2>/dev/null

# Eliminar la carpeta vacía
rmdir ProyectoFinal/Textures/Textures/ 2>/dev/null

echo "Texturas movidas a ProyectoFinal/Textures/"
