@echo off
setlocal enabledelayedexpansion

set "BUILD_DIR=build"
set "APP_NAME=my-tennis-balls"

rem ── Parse arguments ────────────────────────────────────────────
set "BUILD_TYPE=Debug"
if /i "%~1"=="release" set "BUILD_TYPE=Release"

rem ── Locate executable ──────────────────────────────────────────
set "EXE_PATH=%BUILD_DIR%\bin\%APP_NAME%\%BUILD_TYPE%\%APP_NAME%.exe"
if not exist "%EXE_PATH%" (
    echo [ERROR] Executable not found: %EXE_PATH%
    echo [*] Please run bld.bat first.
    exit /b 1
)

echo [*] Running %EXE_PATH% ...
start "" "%EXE_PATH%"
