@echo off
REM Batch script to compile and run the preservation property tests

echo ========================================
echo Compiling Preservation Property Tests
echo ========================================
echo.

REM Try to compile with g++ if available
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Using g++ compiler...
    g++ -std=c++11 -o preservation_tests.exe preservation_property_tests.cpp
    if %ERRORLEVEL% EQU 0 (
        echo Compilation successful!
        echo.
        echo ========================================
        echo Running Tests
        echo ========================================
        echo.
        preservation_tests.exe
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
    cl /EHsc /std:c++14 preservation_property_tests.cpp /Fe:preservation_tests.exe
    if %ERRORLEVEL% EQU 0 (
        echo Compilation successful!
        echo.
        echo ========================================
        echo Running Tests
        echo ========================================
        echo.
        preservation_tests.exe
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
