@echo off

rem Check if cmake is available
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo. Error: cmake command not found!
    echo. Please ensure cmake is installed and added to system PATH environment variable.
    echo.
    exit /b 1
)

rem Display cmake version
cmake --version
echo.

cmake -B build -G"Visual Studio 17 2022" -A win32 --log-level=STATUS -DIS_DEBUG=ON

cmake --build build
