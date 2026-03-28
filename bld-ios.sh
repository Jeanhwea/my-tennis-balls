#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build-ios"
TOOLCHAIN="cocos2d/cmake/ios.toolchain.cmake"

# Parse arguments
BUILD_TYPE="Debug"
CMAKE_DEBUG_FLAG="-DIS_DEBUG=ON"
IOS_PLATFORM="OS"

case "${1:-}" in
    release)
        BUILD_TYPE="Release"
        CMAKE_DEBUG_FLAG="-DIS_DEBUG=OFF"
        ;;
    simulator)
        IOS_PLATFORM="SIMULATOR64"
        ;;
    clean)
        echo "[*] Cleaning iOS build directory..."
        rm -rf "$BUILD_DIR"
        echo "[*] Done."
        exit 0
        ;;
esac

# Allow second arg to override platform (e.g. `bld-ios.sh release simulator`)
case "${2:-}" in
    simulator)  IOS_PLATFORM="SIMULATOR64" ;;
    device)     IOS_PLATFORM="OS" ;;
esac

# Prerequisites
if [[ "$(uname -s)" != "Darwin" ]]; then
    echo "[ERROR] iOS builds require macOS."
    exit 1
fi

if ! command -v cmake &>/dev/null; then
    echo "[ERROR] cmake not found. Please install CMake first."
    exit 1
fi

if ! xcode-select -p &>/dev/null; then
    echo "[ERROR] Xcode command-line tools not found. Run: xcode-select --install"
    exit 1
fi

echo "[*] CMake version:"
cmake --version
echo

# Configure
echo "[*] Configuring iOS ($BUILD_TYPE, platform=$IOS_PLATFORM)..."
cmake -B "$BUILD_DIR" -GXcode \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
    -DIOS_PLATFORM="$IOS_PLATFORM" \
    "$CMAKE_DEBUG_FLAG"

# Build
echo "[*] Building..."
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --parallel

echo "[*] Build succeeded (iOS $BUILD_TYPE, platform=$IOS_PLATFORM)."
