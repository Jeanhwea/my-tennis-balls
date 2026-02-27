#!/usr/bin/env bash

# Check if cmake is installed
if ! command -v cmake &> /dev/null
then
    echo "cmake could not be found"
    exit
fi

# Display cmake version
cmake --version

# Set macOS deployment target for compatibility with macOS 10
export MACOSX_DEPLOYMENT_TARGET=10.15

cmake -B build -G"Xcode" --log-level=STATUS -DIS_DEBUG=ON -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15

cmake --build build
