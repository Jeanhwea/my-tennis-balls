@echo off
setlocal enabledelayedexpansion

rem ── Configuration ──────────────────────────────────────────────
set "BUILD_DIR=build"
set "GENERATOR=Visual Studio 17 2022"
set "ARCH=win32"

rem ── Parse arguments ────────────────────────────────────────────
set "BUILD_TYPE=Debug"
set "CMAKE_DEBUG_FLAG=-DIS_DEBUG=ON"

if /i "%~1"=="release" (
    set "BUILD_TYPE=Release"
    set "CMAKE_DEBUG_FLAG=-DIS_DEBUG=OFF"
)
if /i "%~1"=="clean" (
    echo [*] Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
    echo [*] Done.
    exit /b 0
)

rem ── Prerequisite check ─────────────────────────────────────────
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] cmake not found. Please install CMake and add it to PATH.
    exit /b 1
)

echo [*] CMake version:
cmake --version
echo.

rem ── Configure ──────────────────────────────────────────────────
echo [*] Configuring (%BUILD_TYPE%)...
cmake -B "%BUILD_DIR%" -G"%GENERATOR%" -A %ARCH% --log-level=STATUS %CMAKE_DEBUG_FLAG%
if %errorlevel% neq 0 (
    echo [ERROR] CMake configure failed.
    exit /b 1
)

rem ── Build ──────────────────────────────────────────────────────
echo [*] Building...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE% --parallel
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    exit /b 1
)

echo [*] Build succeeded (%BUILD_TYPE%).
