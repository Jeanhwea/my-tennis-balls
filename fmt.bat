@echo off
rem Batch file for formatting all source files in the project

rem Check if clang-format is available
where clang-format >nul 2>nul
if %errorlevel% neq 0 (
    echo. Error: clang-format command not found!
    echo. Please ensure clang-format is installed and added to system PATH environment variable.
    echo.
    exit /b 1
)

rem Display clang-format version
clang-format --version
echo.

rem Get script directory (project root directory)
set "PROJECT_DIR=%~dp0"
set "PROJECT_DIR=%PROJECT_DIR:~0,-1%"

echo. Project directory: %PROJECT_DIR%
echo.

rem Format all source files in source directory
echo. Formatting source files in source directory...
for /r "%PROJECT_DIR%\Classes" %%f in (*.cpp, *.h) do (
    echo.   Processing: %%f
    clang-format -i "%%f"
    if errorlevel 1 (
        echo.   Error: Failed to format %%f!
    )
)
