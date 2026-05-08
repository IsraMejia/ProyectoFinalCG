@echo off
REM Script para subir cambios a GitHub en la rama Andrea

echo ===================================
echo   SUBIR CAMBIOS A RAMA ANDREA
echo ===================================
echo.

REM 1. Mostrar archivos modificados
echo [96m Archivos modificados:[0m
git status --short
echo.

REM 2. Cambiar a la rama Andrea (o crearla si no existe localmente)
echo [93m Cambiando a rama Andrea...[0m
git checkout Andrea 2>nul || git checkout -b Andrea
echo [92m En rama Andrea[0m
echo.

REM 3. Agregar archivos al staging
echo [92m Agregando archivos...[0m
git add ProyectoFinal/dependencias/Window.h
git add ProyectoFinal/dependencias/Window.cpp
git add ProyectoFinal/Integrantes/Andrea/farola.h
git add ProyectoFinal/Integrantes/Andrea/farola.cpp
git add ProyectoFinal/main.cpp
git add ProyectoFinal/documentacion/luces_faroles.md
git add ProyectoFinal/documentacion/guia_git.md
git add COMO_USAR_GIT.md
git add subir_cambios.bat
git add obtener_cambios.bat
echo [92m Archivos agregados[0m
echo.

REM 4. Crear commit
echo [92m Creando commit...[0m
git commit -m "Agregadas luces spotlight en faroles controladas con tecla L"
echo [92m Commit creado[0m
echo.

REM 5. Obtener cambios de la rama Andrea
echo [93m Obteniendo cambios de la rama Andrea...[0m
git pull origin Andrea
echo.

if %ERRORLEVEL% EQU 0 (
    echo [92m No hay conflictos[0m
    echo.
    
    REM 6. Subir cambios a la rama Andrea
    echo [93m Subiendo cambios a rama Andrea en GitHub...[0m
    git push origin Andrea
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo [92m ========================================[0m
        echo [92m   CAMBIOS SUBIDOS EXITOSAMENTE![0m
        echo [92m   Rama: Andrea[0m
        echo [92m ========================================[0m
    ) else (
        echo.
        echo [91m Error al subir cambios[0m
        echo Verifica tu conexion y permisos
        echo Si es la primera vez, puede que necesites:
        echo    git push -u origin Andrea
    )
) else (
    echo [91m HAY CONFLICTOS - Debes resolverlos manualmente[0m
    echo.
    echo Pasos para resolver:
    echo 1. Abre los archivos con conflicto
    echo 2. Busca las marcas ^<^<^<^<^<^<^< ======= ^>^>^>^>^>^>^>
    echo 3. Edita y guarda los archivos
    echo 4. Ejecuta:
    echo    git add .
    echo    git commit -m "Resueltos conflictos"
    echo    git push origin Andrea
)

echo.
echo ===================================
pause
