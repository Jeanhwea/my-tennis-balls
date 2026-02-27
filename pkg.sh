#!/usr/bin/env bash
set -eux

GIT_TAG=$(git describe --tags --always --dirty="+dev")
OUT_DIR=output
DST_DIR=dist
OS_NAME=$(uname -s)
OS_ARCH=$(uname -m)
DMG_DIR=${OUT_DIR}/dmg

APP_PKG=my-tennis-balls
RLE_APP=${OUT_DIR}/bin/${APP_PKG}/Release/${APP_PKG}.app
DMG_VOL=${APP_PKG}-${OS_NAME}-${OS_ARCH}-${GIT_TAG}
DMG_IMG=${OUT_DIR}/${APP_PKG}-${OS_NAME}-${OS_ARCH}-${GIT_TAG}.dmg

# Check if cmake is installed
if ! command -v cmake &> /dev/null
then
    echo "cmake could not be found"
    exit
fi

# Check if cmake is installed
if ! command -v cmake &> /dev/null; then
    echo "cmake could not be found"
    exit 1
fi

# Check if Xcode is installed
if ! command -v xcode-select &> /dev/null; then
    echo "Xcode could not be found"
    exit 1
fi

# Display cmake version
cmake --version

# Set macOS deployment target for compatibility with macOS 10
export MACOSX_DEPLOYMENT_TARGET=10.15

# Build application
cmake -B ${OUT_DIR} -G"Xcode" --log-level=STATUS \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=$MACOSX_DEPLOYMENT_TARGET
if [ $? -ne 0 ]; then
    echo "cmake configuration failed"
    exit 1
fi

# Release application with Xcode
cmake --build ${OUT_DIR} --config Release
if [ $? -ne 0 ]; then
    echo "cmake build failed"
    exit 1
fi

# Remove quarantine attribute if present
xattr -cr ${RLE_APP}

# Sign the application with ad-hoc signature
codesign --force --deep --sign io.github.jeanhwea.${APP_PKG} ${RLE_APP}
if [ $? -ne 0 ]; then
    echo "codesign failed"
    exit 1
fi

mkdir -p ${DMG_DIR}
mv ${RLE_APP} ${DMG_DIR}
ln -s /Applications ${DMG_DIR}/Applications
hdiutil create -volname ${DMG_VOL} -srcfolder ${DMG_DIR} -ov -format UDZO ${DMG_IMG}
mkdir -p ${DST_DIR}
mv ${DMG_IMG} ${DST_DIR}

rm -rf ${OUT_DIR}
