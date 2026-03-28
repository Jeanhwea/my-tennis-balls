#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
APP_NAME="my-tennis-balls"
BUILD_TYPE="${1:-}"

case "$(uname -s)" in
    Darwin)
        APP_BUNDLE=$(find "$BUILD_DIR" -name "${APP_NAME}.app" -type d 2>/dev/null | head -1)
        if [[ -z "$APP_BUNDLE" ]]; then
            echo "[ERROR] ${APP_NAME}.app not found. Please run ./bld.sh first."
            exit 1
        fi
        echo "[*] Running $APP_BUNDLE ..."
        open "$APP_BUNDLE"
        ;;
    Linux)
        EXE=$(find "$BUILD_DIR" -name "$APP_NAME" -type f -executable 2>/dev/null | head -1)
        if [[ -z "$EXE" ]]; then
            echo "[ERROR] $APP_NAME not found. Please run ./bld.sh first."
            exit 1
        fi
        echo "[*] Running $EXE ..."
        "$EXE"
        ;;
    *)
        echo "[ERROR] Unsupported platform: $(uname -s)"
        exit 1
        ;;
esac
