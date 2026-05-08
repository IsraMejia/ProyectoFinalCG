@echo off
REM Batch script to compile and run the minimal bug demonstration test

echo ========================================
echo Compiling Train Animation Bug Test
echo ========================================
echo.

REM Try to compile with g++ if available
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Using g++ compiler...
    g++ -std=c++11 -o train_animation_bug_minimal_test.exe train_animation_bug_minimal_test.cpp
    if %ERRORLEVEL% EQU 0 (
        echo Compilation successful!
        echo.
        echo ========================================
        echo Running Test
        echo ========================================
        echo.
        train_animation_bug_minimal_test.exe
        goto :end
    ) else (
        echo Compilation failed!
        goto :end
    )
)

REM Try to compile with cl (Microsoft C++ compiler) if available
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Using Microsoft C++ compiler...
    cl /EHsc /std:c++14 train_animation_bug_minimal_test.cpp /Fe:train_animation_bug_minimal_test.exe
    if %ERRORLEVEL% EQU 0 (
        echo Compilation successful!
        echo.
        echo ========================================
        echo Running Test
        echo ========================================
        echo.
        train_animation_bug_minimal_test.exe
        goto :end
    ) else (
        echo Compilation failed!
        goto :end
    )
)

echo ERROR: No C++ compiler found in PATH
echo Please install MinGW (g++) or Visual Studio (cl)
echo Or compile manually with your preferred compiler

:end
pause
