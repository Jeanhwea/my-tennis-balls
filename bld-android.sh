#!/usr/bin/env bash
set -euo pipefail

PROJ_DIR="proj.android"

# Parse arguments
BUILD_TYPE="debug"

case "${1:-}" in
    release)
        BUILD_TYPE="release"
        ;;
    clean)
        echo "[*] Cleaning Android build..."
        pushd "$PROJ_DIR" > /dev/null
        ./gradlew clean
        popd > /dev/null
        echo "[*] Done."
        exit 0
        ;;
esac

# Prerequisites
if [[ -z "${ANDROID_SDK_ROOT:-}" && -z "${ANDROID_HOME:-}" ]]; then
    echo "[ERROR] ANDROID_SDK_ROOT or ANDROID_HOME not set. Please configure the Android SDK."
    exit 1
fi

# Sync resources to assets
echo "[*] Syncing Resources to assets..."
ASSETS_DIR="$PROJ_DIR/app/assets"
rm -rf "$ASSETS_DIR"
cp -r Resources "$ASSETS_DIR"

# Build
echo "[*] Building Android APK ($BUILD_TYPE)..."
pushd "$PROJ_DIR" > /dev/null
./gradlew "assemble${BUILD_TYPE}" --parallel
popd > /dev/null

echo "[*] Build succeeded ($BUILD_TYPE)."
