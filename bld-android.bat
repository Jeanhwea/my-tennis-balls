@echo off
setlocal enabledelayedexpansion

set "PROJ_DIR=proj.android"

rem Parse arguments
set "BUILD_TYPE=debug"

if /i "%~1"=="release" (
    set "BUILD_TYPE=release"
)
if /i "%~1"=="clean" (
    echo [*] Cleaning Android build...
    pushd "%PROJ_DIR%"
    call gradlew.bat clean
    popd
    echo [*] Done.
    exit /b 0
)

rem Prerequisites
if not defined ANDROID_SDK_ROOT (
    if not defined ANDROID_HOME (
        echo [ERROR] ANDROID_SDK_ROOT or ANDROID_HOME not set. Please configure the Android SDK.
        exit /b 1
    )
)

rem Sync resources to assets
echo [*] Syncing Resources to assets...
if exist "%PROJ_DIR%\app\assets" rmdir /s /q "%PROJ_DIR%\app\assets"
xcopy /s /e /i /q "Resources" "%PROJ_DIR%\app\assets" >nul
if %errorlevel% neq 0 (
    echo [ERROR] Failed to copy resources.
    exit /b 1
)

rem Build
echo [*] Building Android APK (%BUILD_TYPE%)...
pushd "%PROJ_DIR%"
call gradlew.bat assemble%BUILD_TYPE% --parallel
set "RESULT=%errorlevel%"
popd

if %RESULT% neq 0 (
    echo [ERROR] Android build failed.
    exit /b 1
)

echo [*] Build succeeded (%BUILD_TYPE%).
