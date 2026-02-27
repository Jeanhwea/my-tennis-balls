#!/usr/bin/env bash

# Check if cmake is installed
if ! command -v cmake &> /dev/null
then
    echo "cmake could not be found"
    exit
fi

# Display cmake version
cmake --version

cmake -B build -G"Xcode" --log-level=STATUS

cmake --build build
