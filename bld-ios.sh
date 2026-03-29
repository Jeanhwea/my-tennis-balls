#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build-ios"

# Parse arguments
BUILD_TYPE="Debug"
SDK="iphoneos"
ARCHS="arm64"

case "${1:-}" in
    release)
        BUILD_TYPE="Release"
        ;;
    simulator)
        SDK="iphonesimulator"
        ARCHS="x86_64;arm64"
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
    simulator)
        SDK="iphonesimulator"
        ARCHS="x86_64;arm64"
        ;;
    device)
        SDK="iphoneos"
        ARCHS="arm64"
        ;;
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

# Configure — use CMake's built-in iOS support (3.14+)
echo "[*] Configuring iOS ($BUILD_TYPE, sdk=$SDK, archs=$ARCHS)..."
cmake -B "$BUILD_DIR" -GXcode \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_SYSROOT="$SDK" \
    -DCMAKE_OSX_ARCHITECTURES="$ARCHS" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="12.0" \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED="NO" \
    --log-level=STATUS

# Build
echo "[*] Building..."
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --parallel -- \
    IPHONEOS_DEPLOYMENT_TARGET=12.0

echo "[*] Build succeeded (iOS $BUILD_TYPE, sdk=$SDK)."
