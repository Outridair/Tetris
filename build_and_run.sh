#!/bin/bash
set -e

BUILD_DIR=build
VCPKG_TOOLCHAIN=~/vcpkg/scripts/buildsystems/vcpkg.cmake  # Update this if needed

mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_TOOLCHAIN -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

echo "Running TetrisSDL..."
./TetrisSDL
