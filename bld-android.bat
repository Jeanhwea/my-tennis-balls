@echo off
setlocal enabledelayedexpansion


set "JAVA_HOME=D:\tool\OpenJDK8U-jdk_x64_windows_hotspot_8u482b08\jdk8u482-b08"
set "PATH=%JAVA_HOME%\bin;%PATH%"

set "PROJ_DIR=proj.android"
set "APP_NAME=my-tennis-balls"
for /f "tokens=*" %%g in ('git describe --tags --always --dirty^="+dev" 2^>nul') do set "GIT_DESC=%%g"
if not defined GIT_DESC set "GIT_DESC=unknown"

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

rem Copy APK to dist
set "DIST_DIR=dist"
if not exist "%DIST_DIR%" mkdir "%DIST_DIR%"
set "APK_SRC=%PROJ_DIR%\app\build\outputs\apk\%BUILD_TYPE%\%APP_NAME%-%BUILD_TYPE%.apk"
if not exist "!APK_SRC!" (
    set "APK_SRC=%PROJ_DIR%\app\build\outputs\apk\%BUILD_TYPE%\%APP_NAME%-%BUILD_TYPE%-unsigned.apk"
)
set "APK_DEST=%DIST_DIR%\%APP_NAME%-%GIT_DESC%-%BUILD_TYPE%.apk"
if exist "!APK_SRC!" (
    copy /y "!APK_SRC!" "!APK_DEST!" >nul
    echo [*] APK copied to !APK_DEST!
) else (
    echo [WARN] APK not found at !APK_SRC!
)

echo [*] Build succeeded (%BUILD_TYPE%).
