@echo off
REM Script para obtener cambios de compañeros sin perder los tuyos

echo ===================================
echo   OBTENER CAMBIOS DE COMPANEROS
echo ===================================
echo.

REM 1. Verificar si hay cambios sin guardar
echo [96m Verificando estado actual...[0m
git status --short
echo.

REM 2. Preguntar si quiere guardar cambios temporalmente
set /p GUARDAR="Tienes cambios sin guardar. Quieres guardarlos temporalmente? (S/N): "

if /i "%GUARDAR%"=="S" (
    echo [93m Guardando cambios temporalmente...[0m
    git stash save "Cambios guardados automaticamente"
    echo [92m Cambios guardados[0m
    echo.
)

REM 3. Obtener cambios
echo [93m Descargando cambios de GitHub...[0m
git pull origin main
echo.

if %ERRORLEVEL% EQU 0 (
    echo [92m Cambios descargados exitosamente[0m
    echo.
    
    if /i "%GUARDAR%"=="S" (
        echo [93m Restaurando tus cambios...[0m
        git stash pop
        
        if %ERRORLEVEL% EQU 0 (
            echo [92m Tus cambios han sido restaurados[0m
        ) else (
            echo [91m Hay conflictos al restaurar tus cambios[0m
            echo Resuelve los conflictos manualmente
        )
    )
) else (
    echo [91m Error al descargar cambios[0m
    echo Verifica tu conexion a internet
)

echo.
echo ===================================
pause
