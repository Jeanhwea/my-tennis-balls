#!/usr/bin/env bash
set -euo pipefail

PROJ_DIR="proj.android"
APP_NAME="my-tennis-balls"

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

# Copy APK to dist with git describe version
GIT_DESC=$(git describe --tags --always --dirty="+dev" 2>/dev/null || echo "unknown")
DIST_DIR="dist"
mkdir -p "$DIST_DIR"
APK_SRC="$PROJ_DIR/app/build/outputs/apk/$BUILD_TYPE/${APP_NAME}-${BUILD_TYPE}.apk"
if [[ ! -f "$APK_SRC" ]]; then
    APK_SRC="$PROJ_DIR/app/build/outputs/apk/$BUILD_TYPE/${APP_NAME}-${BUILD_TYPE}-unsigned.apk"
fi
APK_DEST="$DIST_DIR/${APP_NAME}-${GIT_DESC}-${BUILD_TYPE}.apk"
if [[ -f "$APK_SRC" ]]; then
    cp "$APK_SRC" "$APK_DEST"
    echo "[*] APK copied to $APK_DEST"
else
    echo "[WARN] APK not found at $APK_SRC"
fi

echo "[*] Build succeeded ($BUILD_TYPE)."
