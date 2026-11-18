@echo off
setlocal
cd /d "%~dp0"

where cmake >nul 2>nul
if errorlevel 1 goto MSBUILD

cmake -S . -B build -A x64
if errorlevel 1 goto ERROR
cmake --build build --config Release
if errorlevel 1 goto ERROR
ctest --test-dir build -C Release --output-on-failure
if errorlevel 1 goto ERROR

echo.
echo Compilacion completada.
echo Ejecutable: build\Release\SCRABBLE.exe
exit /b 0

:MSBUILD
where msbuild >nul 2>nul
if errorlevel 1 (
    echo No se encontro CMake ni MSBuild.
    echo Abra la solucion SCRABBLE.sln desde Visual Studio y compile el proyecto.
    exit /b 1
)
msbuild SCRABBLE.sln /t:Build /p:Configuration=Release /p:Platform=x64
if errorlevel 1 goto ERROR
echo Compilacion completada con MSBuild.
exit /b 0

:ERROR
echo La compilacion fallo. Revise los mensajes anteriores.
exit /b 1
