@echo off
echo ========================================
echo   SUBIR ARCHIVOS A RAMA ANDREA
echo ========================================
echo.

REM Limpiar cualquier operacion pendiente
echo Limpiando estado...
git cherry-pick --abort 2>nul
git merge --abort 2>nul
git rebase --abort 2>nul

REM Verificar rama
echo.
echo Verificando rama actual...
git branch

REM Agregar TODOS los archivos
echo.
echo Agregando todos tus archivos...
git add -A

REM Crear commit
echo.
echo Creando commit...
git commit -m "Subiendo modelos, texturas y luces spotlight de Andrea"

REM Subir a GitHub
echo.
echo Subiendo a GitHub (rama Andrea)...
git push origin Andrea

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo   EXITO! Archivos subidos a GitHub
    echo   Revisa: github.com/IsraMejia/ProyectoFinalCG
    echo   Rama: Andrea
    echo ========================================
) else (
    echo.
    echo ========================================
    echo   ERROR al subir
    echo   Intenta: git push -f origin Andrea
    echo ========================================
)

echo.
pause
