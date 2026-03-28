#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"

# Parse arguments
BUILD_TYPE="Debug"
CMAKE_DEBUG_FLAG="-DIS_DEBUG=ON"

case "${1:-}" in
    release)
        BUILD_TYPE="Release"
        CMAKE_DEBUG_FLAG="-DIS_DEBUG=OFF"
        ;;
    clean)
        echo "[*] Cleaning build directory..."
        rm -rf "$BUILD_DIR"
        echo "[*] Done."
        exit 0
        ;;
esac

# Prerequisites
if ! command -v cmake &>/dev/null; then
    echo "[ERROR] cmake not found. Please install CMake first."
    exit 1
fi

echo "[*] CMake version:"
cmake --version
echo

# Detect platform and generator
PLATFORM_FLAGS=()

case "$(uname -s)" in
    Darwin)
        GENERATOR="Xcode"
        export MACOSX_DEPLOYMENT_TARGET=10.15
        PLATFORM_FLAGS+=("-DCMAKE_OSX_DEPLOYMENT_TARGET=10.15")
        ;;
    Linux)
        GENERATOR="Unix Makefiles"
        ;;
    *)
        echo "[ERROR] Unsupported platform: $(uname -s)"
        exit 1
        ;;
esac

# Configure
echo "[*] Configuring ($BUILD_TYPE, $GENERATOR)..."
cmake -B "$BUILD_DIR" -G"$GENERATOR" --log-level=STATUS \
    "$CMAKE_DEBUG_FLAG" "${PLATFORM_FLAGS[@]}"

# Build
echo "[*] Building..."
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --parallel

echo "[*] Build succeeded ($BUILD_TYPE)."
